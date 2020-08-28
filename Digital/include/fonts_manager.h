
#ifndef _FONTS_MANAGER_H
#define _FONTS_MANAGER_H

typedef struct FontBitMap {
	int iXLeft;  						/* 位图左上角X座标 */
	int iYTop;   						/* 位图左上角Y座标 */
	int iXMax;   						/* 位图的最大X座标值 */
	int iYMax;   						/* 位图的最大Y座标值 */
	int iBpp;    						/* 位图中一个象素用多少位来表示 */
	int iPitch;  						/* 对于单色位图, 两行象素之间的跨度, 即第N行、第N+1行象素数据之间的偏移值 */
	int iCurOriginX;  					/* 位图的原点X座标(一般是左下角), 使用原点来确定相邻字符的位置 */
	int iCurOriginY;  					/* 位图的原点Y座标 */
	int iNextOriginX; 					/* 下一个字符即右边字符的原点X座标 */
	int iNextOriginY; 					/* 下一个字符即右边字符的原点Y座标 */
	unsigned char *pucBuffer;  			/* 存有字符的位图数据 */
}T_FontBitMap, *PT_FontBitMap;

typedef struct FontOpr {
	char *name;             			// 字体模块名
	int (*FontInit)(char *pcFontFile, unsigned int dwFontSize);  // 初始化函数
	int (*GetFontBitmap)(unsigned int dwCode, PT_FontBitMap ptFontBitMap);  // 根据编码值获得字符的位图
	void (*SetFontSize)(unsigned int dwFontSize);   // 设置字体尺寸
	struct FontOpr *ptNext;  
}T_FontOpr, *PT_FontOpr;


/* 注册字体模块 */
int RegisterFontOpr(PT_FontOpr ptFontOpr);

/* 显示支持的字体模块 */
void ShowFontOpr(void);

/* 根据名字取出指定字体模块 */
PT_FontOpr GetFontOpr(char *pcName);

/* 设置字符的尺寸 */
void SetFontSize(unsigned int dwFontSize);

/* 获得字符尺寸 */
unsigned int GetFontSize(void);

/* 获取字符位图 */
int GetFontBitmap(unsigned int dwCode, PT_FontBitMap ptFontBitMap);

/* 设置字体模块的信息,比如指定字库文件,指定字符尺寸 */
int SetFontsDetail(char *pcFontsName, char *pcFontsFile, unsigned int dwFontSize);

/* 调用各个字体模块的初始化函数 */
int FontsInit(void);

/* 注册ASCII字体模块 */
int ASCIIInit(void);

/* 注册GBK字体模块 */
int GBKInit(void);

/* 注册FreeType字体模块 */
int FreeTypeInit(void);

#endif 

