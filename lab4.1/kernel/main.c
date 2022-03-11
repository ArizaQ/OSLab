
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
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
char begin[]=    " begin....";
char writing[] = " write....";
char reading[] = " read.....";
char over[] =    " over....";
PUBLIC PROCESS* getFirst(SEMQUEUE* p_semQueue);
PUBLIC void addToQueue(SEMQUEUE* p_semQueue,PROCESS* p_toAddProcess);

/*======================================================================*
                            kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
	disp_str("-----\"kernel_main\" begins-----\n");

	TASK *p_task = task_table;
	PROCESS *p_proc = proc_table;
	char *p_task_stack = task_stack + STACK_SIZE_TOTAL;
	u16 selector_ldt = SELECTOR_LDT_FIRST;
	int i;
	for (i = 0; i < NR_TASKS; i++)
	{
		strcpy(p_proc->p_name, p_task->name); // name of the process
		p_proc->pid = i;					  // pid

		p_proc->ldt_sel = selector_ldt;

		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3],
			   sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3],
			   sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;
		p_proc->regs.cs = ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.ds = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.es = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.fs = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.ss = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | RPL_TASK;

		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack;
		p_proc->regs.eflags = 0x1202; /* IF=1, IOPL=1 */

		p_task_stack -= p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt += 1 << 3;
	}

	init_reader_writer();
	init_sem();

	k_reenter = 0;
	ticks = 0;

	p_proc_ready = proc_table;

	

	/* 初始化 8253 PIT */
	out_byte(TIMER_MODE, RATE_GENERATOR);
	out_byte(TIMER0, (u8)(TIMER_FREQ / HZ));
	out_byte(TIMER0, (u8)((TIMER_FREQ / HZ) >> 8));

	put_irq_handler(CLOCK_IRQ, clock_handler); /* 设定时钟中断处理程序 */
	enable_irq(CLOCK_IRQ);					   /* 让8259A可以接收时钟中断 */
	clean_screen();
	restart();

	while (1)
	{
	}
}
PUBLIC void clean_screen(){
	disp_pos = 0;
	for (int i = 0; i < 80 * 25; i++)
	{
		disp_str(" ");
	}
	disp_pos = 0;
}
PUBLIC void initSemQueue(SEMQUEUE* p_semQueue){
	p_semQueue->queue_count=0;
	p_semQueue->p_queue_tail=0;
	p_semQueue->p_queue_head=0;
	char temp[2]={'0'+p_semQueue->p_queue_head,'\0'};

}
PUBLIC PROCESS* getFirst(SEMQUEUE* p_semQueue){
	PROCESS* thisProcess=(p_semQueue->waiting_queue[p_semQueue->p_queue_tail]);
	p_semQueue->p_queue_tail++;
	if(p_semQueue->p_queue_tail==NR_TASKS){
		p_semQueue->p_queue_tail=0;
	} 
	p_semQueue->queue_count--;
	return thisProcess;
}
PUBLIC void addToQueue(SEMQUEUE* p_semQueue,PROCESS* p_toAddProcess){
	p_semQueue->waiting_queue[p_semQueue->p_queue_head]=p_toAddProcess;
	
	//print_str(t);
	p_semQueue->p_queue_head++;
	if(p_semQueue->p_queue_head==NR_TASKS){
		p_semQueue->p_queue_head=0;
	} 
	p_semQueue->queue_count++;
}
PUBLIC void init_reader_writer(){
	proc_table[0].rwMode='r';
	proc_table[1].rwMode='r';
	proc_table[2].rwMode='r';
	proc_table[3].rwMode='w';
	proc_table[4].rwMode='w';
	proc_table[5].rwMode='F';

	proc_table[0].time_slice=2;
	proc_table[1].time_slice=3;
	proc_table[2].time_slice=3;
	proc_table[3].time_slice=3;
	proc_table[4].time_slice=4;
	proc_table[5].time_slice=1;
	for(int i=0;i<NR_TASKS;i++){
		proc_table[i].sleep_ticks=0;
	}
	
}
PUBLIC void init_sem(){
	firstMode='w';
	solveHunger = 1;

	MAX_READERS_NUM = 2;
	MAX_WRITERS_NUM = 1;
	if(firstMode=='r'){
		init_semRF();
	}else{
		init_semWF();
	}
}
PUBLIC void init_semRF(){
	current_readers_cnt=0;
	rCount=0;

	readersSem.s = MAX_READERS_NUM;
	resourceSem.s = 1;
	rMutex.s = 1;
	writerQueueSem.s = 1;

	wHungSem.s=1;
	writer_wait_time=-1;
	SOHUNGRY=10000;

	initSemQueue(&readersSem);
	initSemQueue(&resourceSem);
	initSemQueue(&rMutex);
	initSemQueue(&writerQueueSem);
	initSemQueue(&wHungSem);


}
PUBLIC void init_semWF(){

	current_readers_cnt=0;
	readCount=0;
	writeCount=0;

	readersSem.s = MAX_READERS_NUM;
	resourceSem.s = 1;
	rMutex.s = 1;
	wMutex.s =1;
	readTry.s=1;
	readerQueueSem.s =1;
	writerQueueSem.s = 1;

	rHungSem.s=1;
	reader_wait_time=-1;
	SOHUNGRY=100;

	initSemQueue(&readersSem);
	initSemQueue(&resourceSem);
	initSemQueue(&rMutex);
	initSemQueue(&wMutex);
	initSemQueue(&readTry);
	initSemQueue(&readerQueueSem);
	initSemQueue(&writerQueueSem);
	initSemQueue(&rHungSem);


}
void A()
{
	reader(0);
}
void B()
{
	reader(1);
}
void C()
{
	reader(2);
}
void D()
{
	writer(3);
}
void E()
{
	writer(4);
}
void F(){
	while (1){
		if (current_rw_mode == 'r'){
			print_str("[READ :");
			char num = '0' + current_readers_cnt - 0;
			char readNum[4] = {num, ']', ' ', '\0'};
			print_str(readNum);
		}else if (current_rw_mode == 'w'){
			print_str("[WRITE: ]");
		}else{
			print_str("[BEGIN: ]");
		}
		sleep_milli(TIME_SLICE_LEN);
		//schedule();
	}
}
void reader(int pid){
	if(firstMode=='r'){
		RFreader(pid);
	}else{
		WFreader(pid);
	}
}
void writer(int pid){
	if(firstMode=='r'){
		RFwriter(pid);
	}else{
		WFwriter(pid);
	}
}

void RFreader(int pid)
{
	sleep_milli(TIME_SLICE_LEN);
	char pname[2] = {'A'+pid, '\0'};
	while (1)
	{
		if(solveHunger){
			P(&wHungSem);
		}

		P(&rMutex);
		if (rCount == 0){
			P(&resourceSem);
		}
		rCount++;
		V(&rMutex);

		if(solveHunger){
			V(&wHungSem);
		}

		P(&readersSem);
		current_readers_cnt++;
		print_str(pname);
		print_str(begin);
		
		for (int i = 0; i < p_proc_ready->time_slice-1; i++)
		{
			print_str(pname);
			print_str(reading);
			milli_delay(TIME_SLICE_LEN);
		}
		print_str(pname);
		print_str(reading);
		print_str(pname);
		print_str(over);
		current_readers_cnt--;
		V(&readersSem);
		P(&rMutex);
		rCount--;
		if (rCount == 0)
		{
			V(&resourceSem);
		}
		V(&rMutex);
		milli_delay(TIME_SLICE_LEN);
		// while(solveHunger==1 && writer_wait_time>0 && writer_wait_time-get_ticks()>= SOHUNGRY ){
		// 	sleep_milli(10);
		// }
	}
}

void RFwriter(int pid)
{
	char pname[2] = {'A'+pid, '\0'};
	while (1){	
		P(&writerQueueSem); 
		if(solveHunger){
			P(&wHungSem);
		}
		writer_wait_time=get_ticks();

		P(&resourceSem);
		writer_wait_time=-1;//废弃掉
		if(solveHunger){
			V(&wHungSem);
		}
		
		print_str(pname);
		print_str(begin);
		for (int i = 0; i < p_proc_ready->time_slice-1; i++)
		{
			print_str(pname);
			print_str(writing);
			milli_delay(TIME_SLICE_LEN);
			
		}
		print_str(pname);
		print_str(writing);
		//milli_delay(TIME_SLICE_LEN);
		print_str(pname);
		print_str(over);
		V(&resourceSem);

		V(&writerQueueSem);

		milli_delay(TIME_SLICE_LEN);
		// while(solveHunger==1 && writer_wait_time>0 && writer_wait_time-get_ticks()>= SOHUNGRY ){
		// 	sleep_milli(10);
		// }
	}
}


void WFreader(int pid)
{
	char pname[2] = {'A'+pid, '\0'};
	char pRQS[]="PreaderQSem ";
	char pReadTry[]= "Preadtry.  ";
	char pResource[]= "Prsource  ";
	while (1)
	{
		P(&readerQueueSem);
		// print_str(pname);
		// print_str(pRQS);

		if(solveHunger){
			P(&rHungSem);
		}
		

		P(&readTry);
		// print_str(pname);
		// print_str(pReadTry);
		P(&rMutex);
		readCount++;
		if (readCount == 1){
			reader_wait_time=get_ticks();
			P(&resourceSem);
			// print_str(pname);
			// print_str(pResource);
		}
		V(&rMutex);

		if(solveHunger){
			V(&rHungSem);
		}
		V(&readTry);


		// 开始读了
		P(&readersSem);
		current_readers_cnt++;
		print_str(pname);
		print_str(begin);
		for (int i = 0; i < p_proc_ready->time_slice-1; i++)
		{
			print_str(pname);
			print_str(reading);
			milli_delay(TIME_SLICE_LEN);
		}
		print_str(pname);
		print_str(reading);
		// milli_delay(TIME_SLICE_LEN);
		print_str(pname);
		print_str(over);
		current_readers_cnt--;
		V(&readersSem);

		P(&rMutex);
		readCount--;
		if (readCount == 0)
		{
			V(&resourceSem);
		}
		V(&rMutex);


		V(&readerQueueSem);
		milli_delay(TIME_SLICE_LEN);
		// while(solveHunger==1 && reader_wait_time>0 && reader_wait_time-get_ticks()>= SOHUNGRY ){
		// 	sleep_milli(10);
		// }

	}
}

void WFwriter(int pid)
{
	sleep_milli(TIME_SLICE_LEN);
	char pname[2] = {'A'+pid, '\0'};
	char pReadTry[]=  "Preadtry  ";
	char pResource[]=" Prsource ";
	while (1)
	{	
		if(solveHunger){
			P(&rHungSem);
		}

		P(&wMutex);
		writeCount++;
		if(writeCount==1){
			P(&readTry);
			// print_str(pname);
			// print_str(pReadTry);
		}
		V(&wMutex);
		if(solveHunger){
			V(&rHungSem);
		}

		// 开始读啦
		P(&resourceSem);
		// print_str(pname);
		// print_str(pResource);
		reader_wait_time=-1;//废弃掉
		print_str(pname);
		print_str(begin);
		for (int i = 0; i < p_proc_ready->time_slice-1; i++){
			print_str(pname);
			print_str(writing);
			milli_delay(TIME_SLICE_LEN);
			
		}
		print_str(pname);
		print_str(writing);
		// milli_delay(TIME_SLICE_LEN);
		print_str(pname);
		print_str(over);
		V(&resourceSem);

		P(&wMutex);
		writeCount--;
		if(writeCount==0){
			V(&readTry);
		}
		V(&wMutex);
	
		milli_delay(TIME_SLICE_LEN);
		// while(solveHunger==1 && reader_wait_time>0 && reader_wait_time-get_ticks()>= SOHUNGRY ){
		// 	sleep_milli(10);
		// }
		// p_proc_ready->is_finished = solveHunger;

	}
}

