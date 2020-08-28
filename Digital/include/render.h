
#ifndef _RENDER_H
#define _RENDER_H

#include <pic_operation.h>
#include <disp_manager.h>
#include <page_manager.h>

/* ͼƬ���� */
int PicZoom(PT_PixelDatas ptOriginPic, PT_PixelDatas ptZoomPic);

/* СͼƬ�ϲ�����ͼƬ�� */
int PicMerge(int iX, int iY, PT_PixelDatas ptSmallPic, PT_PixelDatas ptBigPic);

/* ����ͼƬ��ĳ����, �ϲ�����ͼƬ��ָ������ */
int PicMergeRegion(int iStartXofNewPic, int iStartYofNewPic, int iStartXofOldPic, int iStartYofOldPic, int iWidth, int iHeight, PT_PixelDatas ptNewPic, PT_PixelDatas ptOldPic);

/* �ѻ������е�����ˢ����ʾ�豸��ȥ,������ʾ�豸����ʾ�������е�ͼ�� */
void FlushVideoMemToDev(PT_VideoMem ptVideoMem);

/* ȡ��BMP��ʽ��ͼ���ļ��е��������� */
int GetPixelDatasForIcon(char *strFileName, PT_PixelDatas ptPixelDatas);

/* �ͷ�ͼ��������ռ������ */
void FreePixelDatasForIcon(PT_PixelDatas ptPixelDatas);

/* ��ͼƬ��ȡ���������� */
int GetPixelDatasFrmFile(char *strFileName, PT_PixelDatas ptPixelDatas);

/* �ͷ��ڴ� */
void FreePixelDatasFrmFile(PT_PixelDatas ptPixelDatas);

/* ��VideoMem��ָ�����ξ�����ʾ�ַ��� */
int MergerStringToCenterOfRectangleInVideoMem(int iTopLeftX, int iTopLeftY, int iBotRightX, int iBotRightY, unsigned char *pucTextString, PT_VideoMem ptVideoMem);

/* ���ָ���������ɫΪĳһ��ɫ */
void ClearRectangleInVideoMem(int iTopLeftX, int iTopLeftY, int iBotRightX, int iBotRightY, PT_VideoMem ptVideoMem, unsigned int dwColor);

/* �ɿ�ͼ��,ֻ�Ǹı���ʾ�豸�ϵ�ͼ�갴ť��ɫ */
void ReleaseButton(PT_Layout ptLayout);

/* ����ͼ��,ֻ�Ǹı���ʾ�豸�ϵ�ͼ�갴ť��ɫ */
void PressButton(PT_Layout ptLayout);

/* �ж��Ƿ�֧�ָ��ļ���ʽ��BMP/JPG) */
int isPictureFileSupported(char *strFileName);


#endif 

