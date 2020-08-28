#ifndef _ENCODING_MANAGER_H
#define _ENCODING_MANAGER_H

#include <fonts_manager.h>
#include <disp_manager.h>

typedef struct EncodingOpr {
	char *name;    									// 编码模块名
	int iHeadLen;  									// 文件头的长度，文件开头几个字节表示他的编码方式
	PT_FontOpr ptFontOprSupportedHead;  			// 支持此文件的编码方式对应的结构体指针
	int (*isSupport)(unsigned char *pucBufHead);  	// 判断是否支持某文件
	int (*GetCodeFrmBuf)(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode);  //取出一个字符的编码方式
	struct EncodingOpr *ptNext;  				
}T_EncodingOpr, *PT_EncodingOpr;


/* 注册编码模块，编码模块指定了某一编码格式文件中文字的取出方式*/
int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr);


/* 根据名字取出指定的编码模块 */
PT_EncodingOpr Encode(char *pcName);

/* 显示支持的编码方式 */
void ShowEncodingOpr(void);

/* 给文件选择一个编码模块 */
PT_EncodingOpr SelectEncodingOprForFile(unsigned char *pucFileBufHead);

/* 根据编码方式分配字体模块，以获得字符位图
 * 显示一个字符：
 * 1、获取编码值
 * 2、根据编码值，去字库文件中找到对应的位图
 * 3、将位图在LCD描画出来
 */
int AddFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr);

/* 去除某个字体模块 */
int DelFontOprFrmEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr);

/* 调用各种编码方式的初始化函数 */
int EncodingInit(void);

/* 从字符串中取出UTF8格式的字符串的UNICODE编码值 */
int GetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode);

/* 注册ASCII编码模块 */
int AsciiEncodingInit(void);

/* 注册Utf16be编码模块 */
int  Utf16beEncodingInit(void);

/* 注册Utf16le编码模块 */
int  Utf16leEncodingInit(void);

/* 注册Utf8编码模块 */
int  Utf8EncodingInit(void);

#endif
