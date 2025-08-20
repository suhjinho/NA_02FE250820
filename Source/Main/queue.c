#include "queue.h"

#define MAX 10
static int8_t front=-1;
static int8_t rear=-1;
static uint8_t queue[MAX];
 
uint8_t IsEmpty(void)
{
	if(front==rear)//front와 rear가 같으면 큐는 비어있는 상태 
		return 1;
	else 
		return 0;
}
uint8_t IsFull(void)
{
	uint8_t tmp=(rear+1)%MAX; //원형 큐에서 rear+1을 MAX로 나눈 나머지값이
	if(tmp==front)//front와 같으면 큐는 가득차 있는 상태
		return 1;
    else
		return 0;
}
void addq(uint8_t value){
    if(IsFull() == 0)
	{
		rear = (rear+1)%MAX;
		queue[rear]=value;
	}
}

uint8_t deleteq(uint8_t *value)
{
	uint8_t re = 0;
    if(IsEmpty() == 0)
	{
        front = (front+1)%MAX;
		*value = queue[front];
        re = 1;
    }
	return re;
}
