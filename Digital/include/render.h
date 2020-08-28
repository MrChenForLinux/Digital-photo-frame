
#ifndef _RENDER_H
#define _RENDER_H

#include <pic_operation.h>
#include <disp_manager.h>
#include <page_manager.h>

/* 图片缩放 */
int PicZoom(PT_PixelDatas ptOriginPic, PT_PixelDatas ptZoomPic);

/* 小图片合并到大图片中 */
int PicMerge(int iX, int iY, PT_PixelDatas ptSmallPic, PT_PixelDatas ptBigPic);

/* 把新图片的某部分, 合并入老图片的指定区域 */
int PicMergeRegion(int iStartXofNewPic, int iStartYofNewPic, int iStartXofOldPic, int iStartYofOldPic, int iWidth, int iHeight, PT_PixelDatas ptNewPic, PT_PixelDatas ptOldPic);

/* 把缓冲区中的数据刷到显示设备上去,即在显示设备上显示缓冲区中的图像 */
void FlushVideoMemToDev(PT_VideoMem ptVideoMem);

/* 取出BMP格式的图标文件中的象素数据 */
int GetPixelDatasForIcon(char *strFileName, PT_PixelDatas ptPixelDatas);

/* 释放图像数据所占缓冲区 */
void FreePixelDatasForIcon(PT_PixelDatas ptPixelDatas);

/* 从图片中取出像素数据 */
int GetPixelDatasFrmFile(char *strFileName, PT_PixelDatas ptPixelDatas);

/* 释放内存 */
void FreePixelDatasFrmFile(PT_PixelDatas ptPixelDatas);

/* 在VideoMem的指定矩形居中显示字符串 */
int MergerStringToCenterOfRectangleInVideoMem(int iTopLeftX, int iTopLeftY, int iBotRightX, int iBotRightY, unsigned char *pucTextString, PT_VideoMem ptVideoMem);

/* 清除指定区域的颜色为某一颜色 */
void ClearRectangleInVideoMem(int iTopLeftX, int iTopLeftY, int iBotRightX, int iBotRightY, PT_VideoMem ptVideoMem, unsigned int dwColor);

/* 松开图标,只是改变显示设备上的图标按钮颜色 */
void ReleaseButton(PT_Layout ptLayout);

/* 按下图标,只是改变显示设备上的图标按钮颜色 */
void PressButton(PT_Layout ptLayout);

/* 判断是否支持改文件格式（BMP/JPG) */
int isPictureFileSupported(char *strFileName);


#endif 

