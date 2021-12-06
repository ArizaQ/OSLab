#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"
#include "vector.h"
EXTERN put_key(TTY *p_tty, u32 key);
EXTERN VECTOR CTRLZ_BEFORE_NEXT_INPUT_INPUT;
EXTERN OPERATION nullOpration;
PUBLIC int seraching_mode_op_cnt;



void init_vector(VECTOR* p_v) {
	p_v->capacity = ORIGIN_CAPACITY;
	p_v->size = 0;
	p_v->head = p_v->array;
	p_v->tail = p_v->array;
}
void vector_push(VECTOR* p_v,int val) {
	if (p_v->size < p_v->capacity) {
		*(p_v->head) = val;
		p_v->head++;
		p_v->size++;
	}
	else {
		p_v->head = p_v->array;
		*(p_v->head) = val;
		p_v->head++;
		p_v->size = 1;
	}
}
 int vector_pop(VECTOR* p_v) {
	if (p_v->size > 0) {
		p_v->head--;
		int val = *(p_v->head);
		*(p_v->head) = 0;
		p_v->size--;
		return val;
	}
	return 0;

}

void init_vector_operation(VECTOR_OPRATION* p_v) {
	p_v->capacity = ORIGIN_CAPACITY;
	p_v->size = 0;
	p_v->head = p_v->array;
	p_v->tail = p_v->array;
    init_vector(&CTRLZ_BEFORE_NEXT_INPUT_INPUT);
}
void vector_operation_push(VECTOR_OPRATION* p_v,OPERATION val) {
	if (p_v->size < p_v->capacity) {
		*(p_v->head) = val;
		p_v->head++;
		p_v->size++;
	}
	else {
		p_v->head = p_v->array;
		*(p_v->head) = val;
		p_v->head++;
		p_v->size = 1;
	}
}

OPERATION vector_operation_pop(VECTOR_OPRATION* p_v,TTY* p_tty) {
	// 搜索模式下，搜索模式未输入字符时，不pop操作
	if(searching_mode!=0){
		if(seraching_mode_op_cnt<=0) return;
	}
	if (p_v->size > 0) {
		p_v->head--;
		OPERATION val = *(p_v->head);
		*(p_v->head) = nullOpration;
		p_v->size--;

		if(val.opcode==0){
			//输入操作，删除之
			put_key(p_tty,'\b');
		}else if(val.opcode==1){
			//删除操作，恢复之
			put_key(p_tty,val.oprand);
            vector_push(&CTRLZ_BEFORE_NEXT_INPUT_INPUT,val.oprand);

		}
		if(searching_mode!=0){
			seraching_mode_op_cnt--;
		}
		
		return val;
	}
	return nullOpration;

}