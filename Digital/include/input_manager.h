#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H
#include <sys/time.h>
#include <pthread.h>

// 输入事件类型
#define INPUT_TYPE_STDIN        0
#define INPUT_TYPE_TOUCHSCREEN  1

typedef struct InputEvent {
	struct timeval tTime;   //时间
	int iType;  			//类型
	int iX;    				//X坐标
	int iY;					//Y坐标
	int iKey;   			//按键值
	int iPressure; 			//压力值
}T_InputEvent, *PT_InputEvent;


typedef struct InputOpr {
	char *name;          		//输入模块名
	pthread_t tTreadID;  		//子线程ID
	int (*DeviceInit)(void);  	
	int (*DeviceExit)(void);  
	int (*GetInputEvent)(PT_InputEvent ptInputEvent); //获取输入数据
	struct InputOpr *ptNext;
}T_InputOpr, *PT_InputOpr;


/* 注册单个输入模块 */
int RegisterInputOpr(PT_InputOpr ptInputOpr);

/* 显示支持的模块 */
void ShowInputOpr(void);

/* 初始化输入设备，并创建用于读取设备输入数据的子线程 */
int AllInputDevicesInit(void);

/* 获取输入数据，休眠状态，当子线程读取到数据后会将其唤醒 */
int GetInputEvent(PT_InputEvent ptInputEvent);

/* 注册stdin输入模块 */
int StdinInit(void);

/* 注册触摸屏输入模块 */
int TouchScreenInit(void);

/* 注册所有输入模块 */
int InputInit(void);


#endif 

