
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               proc.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"
EXTERN PROCESS* getFirst(SEMQUEUE* p_semQueue);
EXTERN void addToQueue(SEMQUEUE* p_semQueue,PROCESS* p_toAddProcess);
PUBLIC  PROCESS* next_proc = proc_table;
/*+++++++++++OVER+++++++++++++++++++++++++++++++++++++++++++++++*/
PUBLIC void schedule()
{
	disable_irq(CLOCK_IRQ);

	PROCESS *p_proc_F = proc_table+NR_TASKS-1;
	if(is_proc_ready(p_proc_F)){
		p_proc_ready = p_proc_F;
	}else{
		while (is_proc_ready(next_proc)==0){
			next_proc_inc();
		}
		p_proc_ready = next_proc;
		next_proc_inc();
		current_rw_mode = p_proc_ready->rwMode;
	}
	enable_irq(CLOCK_IRQ);

}
PUBLIC void schedule_F()
{
	disable_irq(CLOCK_IRQ);
	PROCESS *p_proc_F = proc_table+NR_TASKS-1;
	p_proc_ready = p_proc_F;
	enable_irq(CLOCK_IRQ);

}
PUBLIC void next_proc_inc(){
	next_proc++;
	if(next_proc==(proc_table+NR_TASKS-1)){
		next_proc = proc_table;
	}
}

/*======================================================================*
                           sys_get_ticks
 *======================================================================*/
PUBLIC int sys_get_ticks()
{
	return ticks;
}

/*=========== OVER ==================*/
PUBLIC void sys_print_str(char* str){
	//pid 0,1,2,3,4,5
	// 根据pid显示不同颜色
	u32 pid=p_proc_ready->pid;
	int color=WHITE;

	if (disp_pos > 80*50){
		//disp_pos=0;
		return;
	}
	
	switch(pid){
		case 0://A
		color=RED;
		break;
		case 1://B
		color=GREEN;
		break;
		case 2://C
		color=CYAN;
		break;
		case 3://D
		color=MAGENTA;
		break;
		case 4://E
		color=BROWN;
		break;
		case 5://F 非彩色
		color=WHITE;
		break;
	}
	disp_color_str(str,color);
}
/*=========== OVER ==================*/
PUBLIC void sys_sleep_milli(int milli_seconds){
	p_proc_ready->sleep_ticks=milli_seconds / (1000 / HZ);
	schedule();
}
/*=========== OVER ==================*/
PUBLIC void sys_p(void* p){
	disable_irq(CLOCK_IRQ);
	SEMAPHORE* p_sem=(SEMAPHORE*)p;
	p_sem->s--;
	if(p_sem->s<0){
		// 进入队列，阻塞自己
		int waiting_tasks_cnt=-p_sem->s;
		p_sem->waiting_queue[waiting_tasks_cnt - 1] = p_proc_ready;
		p_proc_ready->is_blocked = 1; 
		//addToQueue(&(p_sem->wQueue),p_proc_ready);
		// 阻塞自己后需要重新调度，因为自己睡着了
		schedule();
	}
	enable_irq(CLOCK_IRQ);
}
/*=========== OVER ==================*/
PUBLIC void sys_v(void* p){
	disable_irq(CLOCK_IRQ); 
	SEMAPHORE* p_sem=(SEMAPHORE*)p;
	p_sem->s++;
	if(p_sem->s<=0){
		// 唤醒等待的任务，唤醒后不需要重新调度，因为自己仍然在运行
		int waiting_tasks_cnt=-p_sem->s;
		PROCESS *p_to_wake = p_sem->waiting_queue[0];
		p_to_wake->is_blocked = 0;
		rearrange_waiting_queue(p_sem->waiting_queue,waiting_tasks_cnt);
		//PROCESS *p_to_wake = getFirst(&(p_sem->wQueue));
	}
	enable_irq(CLOCK_IRQ);
}

/*=========== OVER ==================*/
PUBLIC void rearrange_waiting_queue(PROCESS** waiting_queue,int waiting_tasks_cnt){
	for (int i = 0; i <waiting_tasks_cnt; i++){
			waiting_queue[i] = waiting_queue[i + 1];
	}
}
/*=========== OVER ==================*/
PUBLIC int is_proc_ready(PROCESS* p){
	if(p->is_blocked == 0 && p->sleep_ticks == 0){
		return 1;
	}else{
		return 0;
	}
}

