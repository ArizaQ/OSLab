
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			      console.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
						    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*
	回车键: 把光标移到第一列
	换行键: 把光标前进到下一行
*/

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
EXTERN VECTOR output_table;
EXTERN VECTOR output_newline_pos_table;
EXTERN VECTOR searching_mode_input;
EXTERN operations;
PUBLIC int seraching_mode_op_cnt;

PUBLIC CONSOLE *public_console;
PUBLIC void set_cursor(unsigned int position);
PRIVATE void set_video_start_addr(u32 addr);
PRIVATE void flush(CONSOLE *p_con);

EXTERN void init_vector(VECTOR *p_v);
EXTERN void vector_push(VECTOR *p_v, int val);
EXTERN int vector_pop(VECTOR *p_v);
EXTERN void init_vector_operation(VECTOR_OPRATION *p_v);

PUBLIC void set_cursor(unsigned int position);
PRIVATE void set_video_start_addr(u32 addr);
PRIVATE void flush(CONSOLE *p_con);

PUBLIC void clean_screen(TTY *P_tty);

/*======================================================================*
			   init_screen
			   初始化tty：
					设置对应的consoel
						设置显存开始位置
						设置显存大小
						设置当前显示到了显存的哪个位置
						设置光标位置：当前显示位置(相对于显存基址)
					初始化用到的vector
				cursor：位置是相对于屏幕而言的

 *======================================================================*/
PUBLIC void init_screen(TTY *p_tty)
{
	int nr_tty = p_tty - tty_table;
	p_tty->p_console = console_table + nr_tty;

	int v_mem_size = V_MEM_SIZE >> 1; /* 显存总大小 (以 2Byte计) */

	int con_v_mem_size = v_mem_size / NR_CONSOLES;
	p_tty->p_console->original_addr = nr_tty * con_v_mem_size;
	p_tty->p_console->v_mem_limit = con_v_mem_size;
	p_tty->p_console->current_start_addr = p_tty->p_console->original_addr;
	/* 默认光标位置在最开始处 */
	p_tty->p_console->cursor = p_tty->p_console->original_addr;

	// if (nr_tty == 0) {
	// 	/* 第一个控制台沿用原来的光标位置 */
	// 	p_tty->p_console->cursor = disp_pos / 2;
	// 	disp_pos = 0;
	// }
	// else {
	// 	 out_char(p_tty->p_console, nr_tty + '0');
	// 	out_char(p_tty->p_console, '#');
	// }

	clean_screen(p_tty);

	set_cursor(p_tty->p_console->cursor);
	init_vector(&output_table);
	init_vector(&output_newline_pos_table);
	init_vector(&searching_mode_input);
	init_vector_operation(&operations);
}

PUBLIC void clean_screen(TTY *p_tty)
{
	for (u32 original_addr = p_tty->p_console->original_addr; original_addr < p_tty->p_console->original_addr + p_tty->p_console->v_mem_limit; original_addr += 1)
	{
		u8 *p_vmem = (u8 *)(V_MEM_BASE + original_addr * 2);
		*p_vmem++ = ' ';
		p_vmem++;
	}
	p_tty->p_console->cursor = p_tty->p_console->original_addr;
	set_cursor(p_tty->p_console->cursor);
	// disp_int(p_tty->p_console->cursor);
	// disp_int(V_MEM_BASE);
	set_cursor(0); //为防止多tty时不从屏幕开始处显示，加上这句
	set_video_start_addr(p_tty->p_console->current_start_addr);
}

/*======================================================================*
			   is_current_console
*======================================================================*/
PUBLIC int is_current_console(CONSOLE *p_con)
{
	return (p_con == &console_table[nr_current_console]);
}

/*======================================================================*
			   out_char
 *======================================================================*/
PUBLIC void out_char(CONSOLE *p_con, char ch)
{
	u8 *p_vmem = (u8 *)(V_MEM_BASE + p_con->cursor * 2);
	public_console = p_con;
	switch (ch)
	{
	case '\n':
		if (p_con->cursor < p_con->original_addr +
								p_con->v_mem_limit - SCREEN_WIDTH)
		{
			vector_push(&output_newline_pos_table, p_con->cursor);

			p_con->cursor = p_con->original_addr + SCREEN_WIDTH *((p_con->cursor - p_con->original_addr) /SCREEN_WIDTH +1);

			vector_push(&output_table, ch);
			if (searching_mode != 0){
				vector_push(&searching_mode_input, ch);
			}
		
		}
		break;
	case '\b':
		if (p_con->cursor > p_con->original_addr)
		{

			if (searching_mode != 0 && searching_mode_input.size <= 0)
			{
				// 搜索模式下如果未输入字符则无需删除之
				break;
			}

			int to_delete_val = vector_pop(&output_table);
			if (to_delete_val != '\t' && to_delete_val != '\n')
			{
				// 删除普通字符
				p_con->cursor--;
				*(p_vmem - 2) = ' ';				//低位
				*(p_vmem - 1) = DEFAULT_CHAR_COLOR; //高位

				if (searching_mode != 0)
				{
					vector_pop(&searching_mode_input);
				}
			}
			else if (to_delete_val == '\n')
			{
				// 删除换行符
				char last_line_last_char = *(output_table.head - 1);
				u8 *temp_p_cursor = vector_pop(&output_newline_pos_table);
				p_con->cursor = temp_p_cursor;
				set_cursor(p_con->cursor);
			}
			else
			{
				// 删除TAB
				for (int i = 0; i < 4; i++)
				{
					p_con->cursor--;
					*(p_vmem - 2) = ' ';				//低位
					*(p_vmem - 1) = DEFAULT_CHAR_COLOR; //高位
				}

				if (searching_mode != 0)
				{
					vector_pop(&searching_mode_input);
					vector_pop(&searching_mode_input);
					vector_pop(&searching_mode_input);
					vector_pop(&searching_mode_input);
				}
			}
		}
		break;
	case '\t':
		if (p_con->cursor < p_con->original_addr +
								p_con->v_mem_limit - 4)
		{

			vector_push(&output_table, ch);
			if (searching_mode != 0)
			{
				vector_push(&searching_mode_input, ' ');
				vector_push(&searching_mode_input, ' ');
				vector_push(&searching_mode_input, ' ');
				vector_push(&searching_mode_input, ' ');
			}

			for (int i = 0; i < 8; i += 2)
			{
				*(p_vmem + i) = ' ';
				*(p_vmem + i + 1) = DEFAULT_CHAR_COLOR;
			}
			p_con->cursor += 4;
		}
		break;
	default:
		// 普通字符
		if (p_con->cursor <
			p_con->original_addr + p_con->v_mem_limit - 1)
		{
			*p_vmem++ = ch;
			*p_vmem++ = (searching_mode == 0 ? DEFAULT_CHAR_COLOR : RED);
			p_con->cursor++;

			vector_push(&output_table, ch);
			if (searching_mode != 0)
			{
				vector_push(&searching_mode_input, ch);
			}
		}
		break;
	}

	while (p_con->cursor >= p_con->current_start_addr + SCREEN_SIZE)
	{
		scroll_screen(p_con, SCR_DN);
	}

	flush(p_con);
}

/*======================================================================*
                           flush
*======================================================================*/
PRIVATE void flush(CONSOLE *p_con)
{
	set_cursor(p_con->cursor);
	set_video_start_addr(p_con->current_start_addr);
}

/*======================================================================*
			    set_cursor
 *======================================================================*/
PUBLIC void set_cursor(unsigned int position)
{
	disable_int();
	out_byte(CRTC_ADDR_REG, CURSOR_H);
	out_byte(CRTC_DATA_REG, (position >> 8) & 0xFF);
	out_byte(CRTC_ADDR_REG, CURSOR_L);
	out_byte(CRTC_DATA_REG, position & 0xFF);
	enable_int();
}

/*======================================================================*
			  set_video_start_addr
			  设置屏幕显示开始位置
			  从显存的某个位置开始显示
 *======================================================================*/
PRIVATE void set_video_start_addr(u32 addr)
{
	disable_int();
	out_byte(CRTC_ADDR_REG, START_ADDR_H);
	out_byte(CRTC_DATA_REG, (addr >> 8) & 0xFF);
	out_byte(CRTC_ADDR_REG, START_ADDR_L);
	out_byte(CRTC_DATA_REG, addr & 0xFF);
	enable_int();
}

/*======================================================================*
			   select_console
 *======================================================================*/
PUBLIC void select_console(int nr_console) /* 0 ~ (NR_CONSOLES - 1) */
{
	if ((nr_console < 0) || (nr_console >= NR_CONSOLES))
	{
		return;
	}

	nr_current_console = nr_console;

	set_cursor(console_table[nr_console].cursor);
	set_video_start_addr(console_table[nr_console].current_start_addr);
}

/*======================================================================*
			   scroll_screen
 *----------------------------------------------------------------------*
 滚屏.
 *----------------------------------------------------------------------*
 direction:
	SCR_UP	: 向上滚屏
	SCR_DN	: 向下滚屏
	其它	: 不做处理
 *======================================================================*/
PUBLIC void scroll_screen(CONSOLE *p_con, int direction)
{
	if (direction == SCR_UP)
	{
		if (p_con->current_start_addr > p_con->original_addr)
		{
			p_con->current_start_addr -= SCREEN_WIDTH;
		}
	}
	else if (direction == SCR_DN)
	{
		if (p_con->current_start_addr + SCREEN_SIZE <
			p_con->original_addr + p_con->v_mem_limit)
		{
			p_con->current_start_addr += SCREEN_WIDTH;
		}
	}
	else
	{
	}

	set_video_start_addr(p_con->current_start_addr);
	set_cursor(p_con->cursor);
}
