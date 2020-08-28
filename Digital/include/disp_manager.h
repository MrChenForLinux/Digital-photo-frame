#ifndef _DISP_MANAGER_H
#define _DISP_MANAGER_H

#include <pic_operation.h>

/* ��ʾ���� */
typedef struct Layout {
	int iTopLeftX;			//���Ͻ�����
	int iTopLeftY;			
	int iBotRightX;			//���½�����
	int iBotRightY;
	char *strIconName;		//ͼ��icon
}T_Layout, *PT_Layout;


/* VideoMem��״̬:
 * 	����
 * 	����Ԥ��׼����ʾ����
 * 	���ڵ�ǰ�߳�
 */
typedef enum {
	VMS_FREE = 0,
	VMS_USED_FOR_PREPARE,
	VMS_USED_FOR_CUR,	
}E_VideoMemState;

/* VideoMem���ڴ���ͼƬ��״̬:
 * �հ�
 * ��������
 * �Ѿ�����
 */
typedef enum {
	PS_BLANK = 0,
	PS_GENERATING,
	PS_GENERATED,	
}E_PicState;


typedef struct VideoMem {
	int iID;                        // ��ʶ��ͬ��ҳ�棬��ҳ�桢���ҳ��...
	int bDevFrameBuffer;            // 1��ʶ�Դ�; 0��ʶ��ͨ����
	E_VideoMemState eVideoMemState; // VideoMem����״̬
	E_PicState ePicState;           // VideoMem���ڴ���ͼƬ��״̬
	T_PixelDatas tPixelDatas;       // �洢����
	struct VideoMem *ptNext;      
}T_VideoMem, *PT_VideoMem;

typedef struct DispOpr {
	char *name;              										// ��ʾģ�������
	int iXres;               										// X�ֱ���
	int iYres;               										// Y�ֱ��� 
	int iBpp;                										// ����λ�� 
	int iLineWidth;          										// ���ֽ��� 
	unsigned char *DispMem;   										// �Դ��ַ 
	int (*DeviceInit)(void);     									// �豸��ʼ������ 
	int (*ShowPixel)(int iPenX, int iPenY, unsigned int dwColor);   // ָ��������ʾ��ɫ 
	int (*CleanScreen)(unsigned int dwBackColor);                   // ����Ϊĳ��ɫ 
	int (*ShowPage)(PT_VideoMem ptVideoMem);                        // ��ʾһҳ
	struct DispOpr *ptNext;     
}T_DispOpr, *PT_DispOpr;

/* ע��ĳ����ʾģ�飬���뵽�����У������д���Ѿ�ע�����ʾ�豸��*/
int RegisterDispOpr(PT_DispOpr ptDispOpr);

/* ��ʾĳ����ע�����ʾ�豸ģ�� */
void ShowDispOpr(void);


/* ��������ȡ��ָ������ʾ�豸ģ�� */
PT_DispOpr GetDispOpr(char *pcName);

/* ��������ȡ��ָ������ʾ�豸ģ�飬�����ó�ʼ����������ΪĬ���豸*/
void SelectAndInitDefaultDispDev(char *name);

/* ����ѡ���Ĭ���豸 */
PT_DispOpr GetDefaultDispDev(void);

/* ��ȡ��ʾ�豸�ķֱ��ʺ�Bpp */
int GetDispResolution(int *piXres, int *piYres, int *piBpp);

/* �������ڻ����VideoMem
 *  VideoMem: 
 *            Ϊ�ӿ���ʾ�ٶ�,���������ڻ����й������ʾ��ҳ�������,
 *            ����VideoMem����ʾʱ�ٰ�VideoMem�е����ݸ��Ƶ��豸���Դ���       
 */
int AllocVideoMem(int iNum);

/* ���һ��ɲ�����VideoMem */
PT_VideoMem GetVideoMem(int iID, int bCur);

/* �ͷ�ȡ����VideoMem */
void PutVideoMem(PT_VideoMem ptVideoMem);

/* ����Դ� */
PT_VideoMem GetDevVideoMem(void);

/* ���VideoMemΪĳ����ɫ */
void ClearVideoMem(PT_VideoMem ptVideoMem, unsigned int dwColor);

/* ���VideoMemָ���ڴ�����Ϊĳ����ɫ*/
void ClearVideoMemRegion(PT_VideoMem ptVideoMem, PT_Layout ptLayout, unsigned int dwColor);

/* ע����ʾ�豸 */
int DisplayInit(void);

/* ע��FB�豸 */
int FBInit(void);

#endif 
