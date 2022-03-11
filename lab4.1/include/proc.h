#ifndef SEM
#define SEM
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               proc.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

typedef struct s_stackframe
{					/* proc_ptr points here				↑ Low			*/
	u32 gs;			/* ┓						│			*/
	u32 fs;			/* ┃						│			*/
	u32 es;			/* ┃						│			*/
	u32 ds;			/* ┃						│			*/
	u32 edi;		/* ┃						│			*/
	u32 esi;		/* ┣ pushed by save()				│			*/
	u32 ebp;		/* ┃						│			*/
	u32 kernel_esp; /* <- 'popad' will ignore it			│			*/
	u32 ebx;		/* ┃						↑栈从高地址往低地址增长*/
	u32 edx;		/* ┃						│			*/
	u32 ecx;		/* ┃						│			*/
	u32 eax;		/* ┛						│			*/
	u32 retaddr;	/* return address for assembly code save()	│			*/
	u32 eip;		/*  ┓						│			*/
	u32 cs;			/*  ┃						│			*/
	u32 eflags;		/*  ┣ these are pushed by CPU during interrupt	│			*/
	u32 esp;		/*  ┃						│			*/
	u32 ss;			/*  ┛						┷High			*/
} STACK_FRAME;

typedef struct s_proc
{
	STACK_FRAME regs; /* process registers saved in stack frame */

	u16 ldt_sel;			   /* gdt selector giving ldt base and limit */
	DESCRIPTOR ldts[LDT_SIZE]; /* local descriptors for code and data */

	int ticks; 
	int sleep_ticks;
	int time_slice;		
	int is_blocked;		
	char rwMode;			
	u32 pid;	
	char p_name[16]; 
} PROCESS;

typedef struct s_task
{
	task_f initial_eip;
	int stacksize;
	char name[32];
} TASK;

/* Number of tasks */
#define NR_TASKS 6

/* stacks of tasks */
#define STACK_SIZE_A 0x8000
#define STACK_SIZE_B 0x8000
#define STACK_SIZE_C 0x8000
#define STACK_SIZE_D 0x8000
#define STACK_SIZE_E 0x8000
#define STACK_SIZE_F 0x8000

#define STACK_SIZE_TOTAL (STACK_SIZE_A + \
						  STACK_SIZE_B + \
						  STACK_SIZE_C+\
						  STACK_SIZE_D+\
						  STACK_SIZE_E+\
						  STACK_SIZE_F)


typedef struct semQueue{
	PROCESS* waiting_queue[NR_TASKS];
	int p_queue_head;//下一个空闲位置
	int p_queue_tail;//下一个待处理位置
	int queue_count;
} SEMQUEUE;

typedef struct semaphore{
    int s;
    PROCESS* waiting_queue[NR_TASKS];
	SEMQUEUE wQueue;
}SEMAPHORE;

char firstMode;
SEMAPHORE readersSem;
SEMAPHORE resourceSem;
SEMAPHORE rMutex;
SEMAPHORE writerQueueSem;
SEMAPHORE wHungSem;
int MAX_READERS_NUM;     // 允许同时读的个数
int MAX_WRITERS_NUM;    // 允许同时写的个数，默认为1
int rCount;
int current_readers_cnt;   // 正在读的个数
int solveHunger; // 是否解决饿死
int writer_wait_time;
char current_rw_mode;
int SOHUNGRY;

// SEMAPHORE writerQueueSem;

// int MAX_READERS_NUM;     
// int MAX_WRITERS_NUM;    

int readCount;
// SEMAPHORE rMutex;
int writeCount;
SEMAPHORE wMutex;
SEMAPHORE readTry;// 大门锁
// SEMAPHORE resourceSem;
// SEMAPHORE readersSem;
SEMAPHORE readerQueueSem;
SEMAPHORE rHungSem;
int reader_wait_time;
// int current_readers_cnt;   // 正在读的个数
// int solveHunger; 
#endif