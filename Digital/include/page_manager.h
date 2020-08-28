#ifndef _PAGE_MANAGER_H
#define _PAGE_MANAGER_H

#include <input_manager.h>
#include <disp_manager.h>

typedef struct PageParams {
    int iPageID;                  /* 页面ID */
    char strCurPictureFile[256];  /* 要处理的第1个图片文件 */
}T_PageParams, *PT_PageParams;

typedef struct PageLayout {
	int iTopLeftX;        //左上角
	int iTopLeftY;
	int iBotRightX;		  //右下角
	int iBotRightY;
	int iBpp;             
	int iMaxTotalBytes;
	PT_Layout atLayout;   //显示区域数组
}T_PageLayout, *PT_PageLayout;

typedef struct PageAction {
	char *name;            							
	void (*Run)(PT_PageParams ptParentPageParams);  //运行，显示页面
	int (*GetInputEvent)(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent);  //获取输入事件         
	struct PageAction *ptNext;    
}T_PageAction, *PT_PageAction;

//页面配置
typedef struct PageCfg {
    int iIntervalSecond;     	//连播模式下的时间间隔
    char strSeletedDir[256];  	//连播模式下要显示的目录
}T_PageCfg, *PT_PageCfg;


/* 注册页面显示模块 */
int RegisterPageAction(PT_PageAction ptPageAction);

/* 显示支持的模块 */
void ShowPages(void);

/* 根据名字取出模块 */
PT_PageAction Page(char *pcName);

/* 页面ID，根据名字算出唯一的ID */
int ID(char *strName);

/* 从图标文件中解析出图像数据并放在指定区域 */
int GeneratePage(PT_PageLayout ptPageLayout, PT_VideoMem ptVideoMem);

/* 时间间隔 */
int TimeMSBetween(struct timeval tTimeStart, struct timeval tTimeEnd);

/* 读取输入数据，判断位于哪个图标上 */
int GenericGetInputEvent(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent);

/* 获得页面的配置参数
 * 连播页面：要播放的目录、播放间隔
 */
void GetPageCfg(PT_PageCfg ptPageCfg);

/* 注册主页面 */
int MainPageInit(void);

/* 注册设置页面 */
int SettingPageInit(void);

/* 注册时间间隔页面 */
int IntervalPageInit(void);

/* 注册浏览页面 */
int BrowsePageInit(void);

/* 注册连播页面 */
int AutoPageInit(void);

/* 注册manual页面 */
int ManualPageInit(void);

/* 页面模块初始化，即注册 */
int PagesInit(void);

#endif 

