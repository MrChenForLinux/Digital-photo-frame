#ifndef _PAGE_MANAGER_H
#define _PAGE_MANAGER_H

#include <input_manager.h>
#include <disp_manager.h>

typedef struct PageParams {
    int iPageID;                  /* ҳ��ID */
    char strCurPictureFile[256];  /* Ҫ����ĵ�1��ͼƬ�ļ� */
}T_PageParams, *PT_PageParams;

typedef struct PageLayout {
	int iTopLeftX;        //���Ͻ�
	int iTopLeftY;
	int iBotRightX;		  //���½�
	int iBotRightY;
	int iBpp;             
	int iMaxTotalBytes;
	PT_Layout atLayout;   //��ʾ��������
}T_PageLayout, *PT_PageLayout;

typedef struct PageAction {
	char *name;            							
	void (*Run)(PT_PageParams ptParentPageParams);  //���У���ʾҳ��
	int (*GetInputEvent)(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent);  //��ȡ�����¼�         
	struct PageAction *ptNext;    
}T_PageAction, *PT_PageAction;

//ҳ������
typedef struct PageCfg {
    int iIntervalSecond;     	//����ģʽ�µ�ʱ����
    char strSeletedDir[256];  	//����ģʽ��Ҫ��ʾ��Ŀ¼
}T_PageCfg, *PT_PageCfg;


/* ע��ҳ����ʾģ�� */
int RegisterPageAction(PT_PageAction ptPageAction);

/* ��ʾ֧�ֵ�ģ�� */
void ShowPages(void);

/* ��������ȡ��ģ�� */
PT_PageAction Page(char *pcName);

/* ҳ��ID�������������Ψһ��ID */
int ID(char *strName);

/* ��ͼ���ļ��н�����ͼ�����ݲ�����ָ������ */
int GeneratePage(PT_PageLayout ptPageLayout, PT_VideoMem ptVideoMem);

/* ʱ���� */
int TimeMSBetween(struct timeval tTimeStart, struct timeval tTimeEnd);

/* ��ȡ�������ݣ��ж�λ���ĸ�ͼ���� */
int GenericGetInputEvent(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent);

/* ���ҳ������ò���
 * ����ҳ�棺Ҫ���ŵ�Ŀ¼�����ż��
 */
void GetPageCfg(PT_PageCfg ptPageCfg);

/* ע����ҳ�� */
int MainPageInit(void);

/* ע������ҳ�� */
int SettingPageInit(void);

/* ע��ʱ����ҳ�� */
int IntervalPageInit(void);

/* ע�����ҳ�� */
int BrowsePageInit(void);

/* ע������ҳ�� */
int AutoPageInit(void);

/* ע��manualҳ�� */
int ManualPageInit(void);

/* ҳ��ģ���ʼ������ע�� */
int PagesInit(void);

#endif 

