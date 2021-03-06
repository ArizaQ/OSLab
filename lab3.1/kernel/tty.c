
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               tty.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"
#include "vector.h"

#define TTY_FIRST (tty_table)
#define TTY_END (tty_table + NR_CONSOLES)

EXTERN VECTOR searching_mode_input;
EXTERN VECTOR_OPRATION operations;

PRIVATE void init_tty(TTY *p_tty);
PRIVATE void tty_do_read(TTY *p_tty);
PRIVATE void tty_do_write(TTY *p_tty);
PUBLIC void put_key(TTY *p_tty, u32 key);
PUBLIC void start_search(TTY *p_tty, u8 color);
PUBLIC int is_match(TTY *p_tty, u8 *p_vmem, u8 color);
EXTERN int time_to_clean;
EXTERN init_vector(VECTOR *p_v);
EXTERN vector_push(VECTOR *p_v, int val);
EXTERN vector_pop(VECTOR *p_v);
EXTERN VECTOR output_table;
EXTERN VECTOR CTRLZ_BEFORE_NEXT_INPUT_INPUT;
EXTERN void init_vector_operation(VECTOR_OPRATION *p_v);
EXTERN OPERATION nullOpration;
EXTERN OPERATION vector_operation_pop(VECTOR_OPRATION *p_v, TTY *p_tty);
EXTERN void vector_operation_push(VECTOR_OPRATION *p_v, OPERATION val);
EXTERN PUBLIC void set_cursor(unsigned int position);

EXTERN clean_screen();
PUBLIC int is_ignore_input = 0;
PUBLIC int seraching_mode_op_cnt = 0;
// EXTERN PUBLIC void set_cursor(unsigned int position);
/*======================================================================*
                           task_tty
 *======================================================================*/
PUBLIC void task_tty()
{
	TTY *p_tty;

	init_keyboard();
	init_vector_operation(&operations);

	for (p_tty = TTY_FIRST; p_tty < TTY_END; p_tty++)
	{
		init_tty(p_tty);
	}
	select_console(0);
	while (1)
	{
		for (p_tty = TTY_FIRST; p_tty < TTY_END; p_tty++)
		{
			tty_do_read(p_tty);
			tty_do_write(p_tty);
		}
		if (time_to_clean == 1 && searching_mode == 0)
		{
			for (p_tty = TTY_FIRST; p_tty < TTY_END; p_tty++)
			{
				init_screen(p_tty);

				time_to_clean = 0;
			}
		}
	}
}

/*======================================================================*
			   init_tty
			   ?????????tty??????????????????????????????
 *======================================================================*/
PRIVATE void init_tty(TTY *p_tty)
{
	p_tty->inbuf_count = 0;
	p_tty->p_inbuf_head = p_tty->p_inbuf_tail = p_tty->in_buf;

	init_screen(p_tty);
}

/*======================================================================*
				in_process
 *======================================================================*/
PUBLIC void in_process(TTY *p_tty, u32 key)
{

	char output[2] = {'\0', '\0'};
	if (CTRLZ_BEFORE_NEXT_INPUT_INPUT.size > 0 && !(((u8)key == 'z' || (u8)key == 'Z') && ((key & FLAG_CTRL_L) || (key & FLAG_CTRL_R))))
	{
		for (int i = 0; i < CTRLZ_BEFORE_NEXT_INPUT_INPUT.size; i++)
		{
			// in_process(p_tty,*(CTRLZ_BEFORE_NEXT_INPUT_INPUT.tail+i));
			init_vector(&CTRLZ_BEFORE_NEXT_INPUT_INPUT);
		}
	}
	if (!(key & FLAG_EXT))
	{
		// ???????????????
		if (is_ignore_input != 1)
		{
			if (((u8)key == 'z' || (u8)key == 'Z') && ((key & FLAG_CTRL_L) || (key & FLAG_CTRL_R)))
			{
				vector_operation_pop(&operations, p_tty);
			}
			else if (((u8)key == 'a' || (u8)key == 'A') && ((key & FLAG_CTRL_L) || (key & FLAG_CTRL_R)))
			{
				// ctrl+a ?????????????????????
				for (int i = 0; i < operations.size; i++)
				{
					put_key(p_tty, (*(operations.head - 1 - i)).opcode + '0');
					put_key(p_tty, (*(operations.head - 1 - i)).oprand);
					put_key(p_tty, ' ');
				}
			}
			else
			{
				// ????????????
				put_key(p_tty, key);
				OPERATION this_operation = {0, key};
				vector_operation_push(&operations, this_operation);
				if (searching_mode != 0)
				{
					seraching_mode_op_cnt++;
				}
			}
		}
	}
	else
	{
		// ??????????????????
		int raw_code = key & MASK_RAW; //0b 0000 0001 1111 1111
		switch (raw_code)
		{
		case ENTER:
			if (is_ignore_input != 1)
			{
				if (searching_mode == 0)
				{
					put_key(p_tty, '\n');
					OPERATION this_operation = {0, '\n'};
					vector_operation_push(&operations, this_operation);
				}
				else
				{
					// searching_mode=-1;//ignore all input
					start_search(p_tty, RED);
					is_ignore_input = 1;
				}
			}

			break;
		case BACKSPACE:
			if (is_ignore_input != 1)
			{

				put_key(p_tty, '\b');
				OPERATION this_operation = {1, *(output_table.head - 1)};
				if (output_table.size != 0)
				{
					if (searching_mode != 0 && searching_mode_input.size == 0)
					{
						;
					}
					else
					{
						vector_operation_push(&operations, this_operation);
						if (searching_mode != 0)
						{
							seraching_mode_op_cnt++;
						}
					}
				}
			}
			break;
		case UP:
			if (is_ignore_input != 1)
				if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R))
				{
					scroll_screen(p_tty->p_console, SCR_DN);
				}
			break;
		case DOWN:
			if (is_ignore_input != 1)
				if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R))
				{
					scroll_screen(p_tty->p_console, SCR_UP);
				}
			break;
		case TAB:
			if (is_ignore_input != 1)
			{
				put_key(p_tty, '\t');
				OPERATION this_operation = {0, '\t'};
				vector_operation_push(&operations, this_operation);
				if (searching_mode != 0)
				{
					seraching_mode_op_cnt++;
				}
			}
			// TAB??????????????????????????????
			break;
		case ESC:
			if (searching_mode == 0)
			{
				searching_mode = 1;
				seraching_mode_op_cnt = 0;
			}
			else
			{
				/*
					?????????????????????
						???????????????????????????
						????????????
						?????????????????????????????????
						??????????????????
				
				*/
				searching_mode = 0;

				start_search(p_tty, DEFAULT_CHAR_COLOR);

				int searching_mode_input_sizt = searching_mode_input.size;
				for (int i = 0; i < searching_mode_input_sizt; i++)
				{
					// put_key(p_tty, '\b');
					vector_pop(&searching_mode_input);
				}
				for (int i = 0; i < seraching_mode_op_cnt; i++)
				{
					vector_operation_pop(&operations, p_tty);
				}

				is_ignore_input = 0;
			}
			// put_key(p_tty, '0' + searching_mode);
			break;
		case CTRL_L:
		case CTRL_R:

			break;
		case F1:
		case F2:
		case F3:
		case F4:
		case F5:
		case F6:
		case F7:
		case F8:
		case F9:
		case F10:
		case F11:
		case F12:

			/* Alt + F1~F12 */
			if (is_ignore_input != 1)
			{
				if ((key & FLAG_ALT_L) || (key & FLAG_ALT_R))
				{
					// select_console(raw_code - F1);
				}
			}
			break;
		default:

			break;
		}
	}
}

PUBLIC void start_search(TTY *p_tty, u8 color)
{
	CONSOLE *p_console = p_tty->p_console;
	u32 original_addr = p_console->original_addr;
	// int searching_end_pos=p_console->v_mem_limit-searching_mode_input.size<0?p_console->original_addr:p_console->v_mem_limit-searching_mode_input.size;
	// if(color==DEFAULT_CHAR_COLOR)
	// 	put_key(p_tty,searching_end_pos-p_console->original_addr+'0');
	for (u32 original_addr = p_console->original_addr; original_addr < p_console->original_addr + p_console->v_mem_limit - searching_mode_input.size; original_addr += 1)
	{
		u8 *p_vmem = (u8 *)(V_MEM_BASE + original_addr * 2);
		int matched = is_match(p_tty, p_vmem, color);
	}
	// u32 original_addr=p_console->original_addr;
	// for(int i=0;i<output_table.size+searching_mode_input.size;i++){
	// 	u8* p_vmem = (u8*)(V_MEM_BASE + original_addr*2);
	// 	// if(color==DEFAULT_CHAR_COLOR)
	// 	// put_key(p_tty,*p_vmem);
	// 	int matched=is_match(p_tty,p_vmem,color);
	// 	original_addr+=1;
	// }
}
PUBLIC int is_match0(TTY *p_tty, u8 *p_vmem, u8 color)
{
	u8 *origin_p_vmem = p_vmem;
	for (int i = 0; i < searching_mode_input.size; i++)
	{
		u8 this_char = *p_vmem++;
		p_vmem++;
		if (this_char != *(searching_mode_input.tail + i))
		{
			return 0;
		}
	}
	p_vmem = origin_p_vmem;
	for (int i = 0; i < searching_mode_input.size; i++)
	{
		// 	if(color==DEFAULT_CHAR_COLOR)
		// put_key(p_tty,*p_vmem);;
		p_vmem++;
		*p_vmem++ = color;
	}
	return 1;
}
PUBLIC int is_match(TTY *p_tty, u8 *p_vmem, u8 color)
{
	u8 *origin_p_vmem = p_vmem;
	for (int i = 0; i < searching_mode_input.size; i++)
	{
		char stack_char = *(searching_mode_input.tail + i);
		u8 this_char = *p_vmem++;
		p_vmem++;

		if (stack_char == '\t')
		{
			if ((*(p_vmem - 1) == 0x02) && this_char == ' ')
			{
				i++;
				continue;
			}
			else if ( (*(p_vmem - 1) != 0x02) &&this_char == ' ')
			{
				return 0;
			}else{
				return 0;
			}
		}
		else if (stack_char == ' ')
		{
			if ((*(p_vmem - 1) !=0x2)  && this_char == ' ')
			{
				i++;
				continue;
			}else if((*(p_vmem - 1) == 0x2) || this_char!=' '){
				return 0;
			}else{
				return 0;
			}
		}
		else
		{
			if (this_char != stack_char)
			{
				return 0;
			}
		}
	}
	p_vmem = origin_p_vmem;
	for (int i = 0; i < searching_mode_input.size; i++)
	{
		
		p_vmem++;
		if(*(p_vmem-1)!=' ')
			*p_vmem++ = color;
		else{
				p_vmem++;
		}
	}
	return 1;
}

/*======================================================================*
			      put_key
				  ??????tty?????????
*======================================================================*/
PUBLIC void put_key(TTY *p_tty, u32 key)
{
	if (p_tty->inbuf_count < TTY_IN_BYTES)
	{
		*(p_tty->p_inbuf_head) = key;
		p_tty->p_inbuf_head++;
		if (p_tty->p_inbuf_head == p_tty->in_buf + TTY_IN_BYTES)
		{
			p_tty->p_inbuf_head = p_tty->in_buf;
		}
		p_tty->inbuf_count++;
	}
}

/*======================================================================*
			      tty_do_read
				  ?????? keyboard_read(p_tty)
 *======================================================================*/
PRIVATE void tty_do_read(TTY *p_tty)
{
	if (is_current_console(p_tty->p_console))
	{
		keyboard_read(p_tty);
	}
}

/*======================================================================*
			      tty_do_write
				  ?????????console
 *======================================================================*/
PRIVATE void tty_do_write(TTY *p_tty)
{
	if (p_tty->inbuf_count)
	{
		char ch = *(p_tty->p_inbuf_tail);
		p_tty->p_inbuf_tail++;
		if (p_tty->p_inbuf_tail == p_tty->in_buf + TTY_IN_BYTES)
		{
			p_tty->p_inbuf_tail = p_tty->in_buf;
		}
		p_tty->inbuf_count--;

		out_char(p_tty->p_console, ch);
	}
}
