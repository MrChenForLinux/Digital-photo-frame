
#include <input_manager.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>


/* ��׼����ģ����豸��ʼ������ */
static int StdinDevInit(void)
{
    struct termios tTTYState;
 
    //get the terminal state
    tcgetattr(STDIN_FILENO, &tTTYState);
 
    //turn off canonical mode
    tTTYState.c_lflag &= ~ICANON;
    //minimum of number input read.
    tTTYState.c_cc[VMIN] = 1;   /* ��һ������ʱ�����̷��� */

    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &tTTYState);

	return 0;
}

/* �˳���׼�����豸���ظ�ԭ������ */
static int StdinDevExit(void)
{

    struct termios tTTYState;
 
    //get the terminal state
    tcgetattr(STDIN_FILENO, &tTTYState);
 
    //turn on canonical mode
    tTTYState.c_lflag |= ICANON;
	
    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &tTTYState);	
	return 0;
}

/*  ��׼����ģ��Ķ�ȡ���ݺ��� */
static int StdinGetInputEvent(PT_InputEvent ptInputEvent)
{
	/* ��������ݾͶ�ȡ������������
	 * ���û������, ���̷���, ���ȴ�
	 */

 	char c;
	
	ptInputEvent->iType = INPUT_TYPE_STDIN;
	
	c = fgetc(stdin);  /* ������ֱ�������� */
	gettimeofday(&ptInputEvent->tTime, NULL);

	return 0;
 }

static T_InputOpr g_tStdinOpr = {
	.name          = "stdin",
	.DeviceInit    = StdinDevInit,
	.DeviceExit    = StdinDevExit,
	.GetInputEvent = StdinGetInputEvent,
};


/* ע���׼����ģ�� */
int StdinInit(void)
{
	return RegisterInputOpr(&g_tStdinOpr);
}
