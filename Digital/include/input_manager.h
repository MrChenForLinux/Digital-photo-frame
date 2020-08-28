#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H
#include <sys/time.h>
#include <pthread.h>

// �����¼�����
#define INPUT_TYPE_STDIN        0
#define INPUT_TYPE_TOUCHSCREEN  1

typedef struct InputEvent {
	struct timeval tTime;   //ʱ��
	int iType;  			//����
	int iX;    				//X����
	int iY;					//Y����
	int iKey;   			//����ֵ
	int iPressure; 			//ѹ��ֵ
}T_InputEvent, *PT_InputEvent;


typedef struct InputOpr {
	char *name;          		//����ģ����
	pthread_t tTreadID;  		//���߳�ID
	int (*DeviceInit)(void);  	
	int (*DeviceExit)(void);  
	int (*GetInputEvent)(PT_InputEvent ptInputEvent); //��ȡ��������
	struct InputOpr *ptNext;
}T_InputOpr, *PT_InputOpr;


/* ע�ᵥ������ģ�� */
int RegisterInputOpr(PT_InputOpr ptInputOpr);

/* ��ʾ֧�ֵ�ģ�� */
void ShowInputOpr(void);

/* ��ʼ�������豸�����������ڶ�ȡ�豸�������ݵ����߳� */
int AllInputDevicesInit(void);

/* ��ȡ�������ݣ�����״̬�������̶߳�ȡ�����ݺ�Ὣ�份�� */
int GetInputEvent(PT_InputEvent ptInputEvent);

/* ע��stdin����ģ�� */
int StdinInit(void);

/* ע�ᴥ��������ģ�� */
int TouchScreenInit(void);

/* ע����������ģ�� */
int InputInit(void);


#endif 

