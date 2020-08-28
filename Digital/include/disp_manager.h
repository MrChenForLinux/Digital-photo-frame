#ifndef _DISP_MANAGER_H
#define _DISP_MANAGER_H

#include <pic_operation.h>

/* 显示区域 */
typedef struct Layout {
	int iTopLeftX;			//左上角坐标
	int iTopLeftY;			
	int iBotRightX;			//右下角坐标
	int iBotRightY;
	char *strIconName;		//图标icon
}T_Layout, *PT_Layout;


/* VideoMem的状态:
 * 	空闲
 * 	用于预先准备显示内容
 * 	用于当前线程
 */
typedef enum {
	VMS_FREE = 0,
	VMS_USED_FOR_PREPARE,
	VMS_USED_FOR_CUR,	
}E_VideoMemState;

/* VideoMem中内存里图片的状态:
 * 空白
 * 正在生成
 * 已经生成
 */
typedef enum {
	PS_BLANK = 0,
	PS_GENERATING,
	PS_GENERATED,	
}E_PicState;


typedef struct VideoMem {
	int iID;                        // 标识不同的页面，主页面、浏览页面...
	int bDevFrameBuffer;            // 1标识显存; 0标识普通缓存
	E_VideoMemState eVideoMemState; // VideoMem本身状态
	E_PicState ePicState;           // VideoMem中内存里图片的状态
	T_PixelDatas tPixelDatas;       // 存储数据
	struct VideoMem *ptNext;      
}T_VideoMem, *PT_VideoMem;

typedef struct DispOpr {
	char *name;              										// 显示模块的名字
	int iXres;               										// X分辨率
	int iYres;               										// Y分辨率 
	int iBpp;                										// 像素位数 
	int iLineWidth;          										// 行字节数 
	unsigned char *DispMem;   										// 显存地址 
	int (*DeviceInit)(void);     									// 设备初始化函数 
	int (*ShowPixel)(int iPenX, int iPenY, unsigned int dwColor);   // 指定像素显示颜色 
	int (*CleanScreen)(unsigned int dwBackColor);                   // 清屏为某颜色 
	int (*ShowPage)(PT_VideoMem ptVideoMem);                        // 显示一页
	struct DispOpr *ptNext;     
}T_DispOpr, *PT_DispOpr;

/* 注册某个显示模块，加入到链表中（链表中存放已经注册的显示设备）*/
int RegisterDispOpr(PT_DispOpr ptDispOpr);

/* 显示某个已注册的显示设备模块 */
void ShowDispOpr(void);


/* 根据名字取出指定的显示设备模块 */
PT_DispOpr GetDispOpr(char *pcName);

/* 根据名字取出指定的显示设备模块，并调用初始化函数，设为默认设备*/
void SelectAndInitDefaultDispDev(char *name);

/* 返回选择的默认设备 */
PT_DispOpr GetDefaultDispDev(void);

/* 获取显示设备的分辨率和Bpp */
int GetDispResolution(int *piXres, int *piYres, int *piBpp);

/* 分配用于缓存的VideoMem
 *  VideoMem: 
 *            为加快显示速度,我们事先在缓存中构造好显示的页面的数据,
 *            存入VideoMem，显示时再把VideoMem中的数据复制到设备的显存上       
 */
int AllocVideoMem(int iNum);

/* 获得一块可操作的VideoMem */
PT_VideoMem GetVideoMem(int iID, int bCur);

/* 释放取出的VideoMem */
void PutVideoMem(PT_VideoMem ptVideoMem);

/* 获得显存 */
PT_VideoMem GetDevVideoMem(void);

/* 清除VideoMem为某种颜色 */
void ClearVideoMem(PT_VideoMem ptVideoMem, unsigned int dwColor);

/* 清除VideoMem指定内存区域为某种颜色*/
void ClearVideoMemRegion(PT_VideoMem ptVideoMem, PT_Layout ptLayout, unsigned int dwColor);

/* 注册显示设备 */
int DisplayInit(void);

/* 注册FB设备 */
int FBInit(void);

#endif 
