#include <config.h>
#include <encoding_manager.h>
#include <string.h>
#include <stdlib.h>

static PT_EncodingOpr g_ptEncodingOprHead;

/* 注册编码模块，编码模块指定了某一编码格式文件中文字的取出方式*/
int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr)
{
	PT_EncodingOpr ptTmp;

	if (!g_ptEncodingOprHead)
	{
		g_ptEncodingOprHead   = ptEncodingOpr;
		ptEncodingOpr->ptNext = NULL;
	}
	else
	{
		ptTmp = g_ptEncodingOprHead;
		while (ptTmp->ptNext)
		{
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext	      = ptEncodingOpr;
		ptEncodingOpr->ptNext = NULL;
	}

	return 0;
}


/* 根据名字取出指定的编码模块 */
PT_EncodingOpr Encode(char *pcName)
{
	PT_EncodingOpr ptTmp = g_ptEncodingOprHead;
	
	while (ptTmp)
	{
		if (strcmp(ptTmp->name, pcName) == 0)
		{
			return ptTmp;
		}
		ptTmp = ptTmp->ptNext;
	}
	return NULL;
}


/* 显示支持的编码方式 */
void ShowEncodingOpr(void)
{
	int i = 0;
	PT_EncodingOpr ptTmp = g_ptEncodingOprHead;

	while (ptTmp)
	{
		printf("%02d %s\n", i++, ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}
}

/* 给文件选择一个编码模块 */
PT_EncodingOpr SelectEncodingOprForFile(unsigned char *pucFileBufHead)
{
	PT_EncodingOpr ptTmp = g_ptEncodingOprHead;

	while (ptTmp)
	{	
		if (ptTmp->isSupport(pucFileBufHead))
			return ptTmp;
		else
			ptTmp = ptTmp->ptNext;
	}
	return NULL;
}

/* 根据编码方式分配字体模块，以获得字符位图
 * 显示一个字符：
 * 1、获取编码值
 * 2、根据编码值，去字库文件中找到对应的位图
 * 3、将位图在LCD描画出来
 */
int AddFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr)
{
	PT_FontOpr ptFontOprCpy;
	
	if (!ptEncodingOpr || !ptFontOpr)
	{
		return -1;
	}
	else
	{
        /* 分配一个新的T_FontOpr结构体 */
		ptFontOprCpy = malloc(sizeof(T_FontOpr));
		if (!ptFontOprCpy)
		{
			return -1;
		}
		else
		{
            /* 复制结构体里的内容 */
			memcpy(ptFontOprCpy, ptFontOpr, sizeof(T_FontOpr));

            /* 把新结构体放入ptEncodingOpr->ptFontOprSupportedHead链表 
             * 为何不直接把原来的ptFontOpr放入链表?
             * 因为原来的ptFontOpr->ptNext已经在RegisterFontOpr函数中被占用了
             */
			ptFontOprCpy->ptNext = ptEncodingOpr->ptFontOprSupportedHead;
			ptEncodingOpr->ptFontOprSupportedHead = ptFontOprCpy;
			return 0;
		}		
	}
}


/* 去除某个字体模块 */
int DelFontOprFrmEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr)
{
	PT_FontOpr ptTmp;
	PT_FontOpr ptPre;
		
	if (!ptEncodingOpr || !ptFontOpr)
	{
		return -1;
	}
	else
	{
		ptTmp = ptEncodingOpr->ptFontOprSupportedHead;
		if (strcmp(ptTmp->name, ptFontOpr->name) == 0)
		{
			/* 删除头节点 */
			ptEncodingOpr->ptFontOprSupportedHead = ptTmp->ptNext;
			free(ptTmp);
			return 0;
		}

		ptPre = ptEncodingOpr->ptFontOprSupportedHead;
		ptTmp = ptPre->ptNext;
		while (ptTmp)
		{
			if (strcmp(ptTmp->name, ptFontOpr->name) == 0)
			{
				/* 从链表里取出、释放 */
				ptPre->ptNext = ptTmp->ptNext;
				free(ptTmp);
				return 0;
			}
			else
			{
				ptPre = ptTmp;
				ptTmp = ptTmp->ptNext;
			}
		}

		return -1;
	}
}

/* 调用各种编码方式的初始化函数 */
int EncodingInit(void)
{
	int ret;

	ret = AsciiEncodingInit();
	if (ret)
	{
		printf("AsciiEncodingInit error!\n");
		return -1;
	}

	ret = Utf16leEncodingInit();
	if (ret)
	{
		printf("Utf16leEncodingInit error!\n");
		return -1;
	}
	
	ret = Utf16beEncodingInit();
	if (ret)
	{
		printf("Utf16beEncodingInit error!\n");
		return -1;
	}
	
	ret = Utf8EncodingInit();
	if (ret)
	{
		printf("Utf8EncodingInit error!\n");
		return -1;
	}

	return 0;
}

/* 从字符串中取出UTF8格式的字符串的UNICODE编码值 */
int GetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode)
{
    return Encode("utf-8")->GetCodeFrmBuf(pucBufStart, pucBufEnd, pdwCode);
}
