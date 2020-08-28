#include <config.h>
#include <fonts_manager.h>
#include <string.h>

static PT_FontOpr g_ptFontOprHead = NULL;
static int g_dwFontSize;

/* 注册字体模块 */
int RegisterFontOpr(PT_FontOpr ptFontOpr)
{
	PT_FontOpr ptTmp;

	if (!g_ptFontOprHead)
	{
		g_ptFontOprHead   = ptFontOpr;
		ptFontOpr->ptNext = NULL;
	}
	else
	{
		ptTmp = g_ptFontOprHead;
		while (ptTmp->ptNext)
		{
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext     = ptFontOpr;
		ptFontOpr->ptNext = NULL;
	}

	return 0;
}


/* 显示支持的字体模块 */
void ShowFontOpr(void)
{
	int i = 0;
	PT_FontOpr ptTmp = g_ptFontOprHead;

	while (ptTmp)
	{
		printf("%02d %s\n", i++, ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}
}

/* 根据名字取出指定字体模块 */
PT_FontOpr GetFontOpr(char *pcName)
{
	PT_FontOpr ptTmp = g_ptFontOprHead;
	
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


/* 设置字符的尺寸 */
void SetFontSize(unsigned int dwFontSize)
{
	PT_FontOpr ptTmp = g_ptFontOprHead;
	
	g_dwFontSize = dwFontSize;

	while (ptTmp)
	{
		if (ptTmp->SetFontSize)
		{
			ptTmp->SetFontSize(dwFontSize);
		}
		ptTmp = ptTmp->ptNext;
	}
}

/* 获得字符尺寸 */
unsigned int GetFontSize(void)
{
	return g_dwFontSize;
}

/* 获取字符位图 */
int GetFontBitmap(unsigned int dwCode, PT_FontBitMap ptFontBitMap)
{
	int ret = 0;
	PT_FontOpr ptTmp = g_ptFontOprHead;

	while (ptTmp)
	{
		ret = ptTmp->GetFontBitmap(dwCode, ptFontBitMap);
		if (0 == ret)
		{
			return 0;
		}
		ptTmp = ptTmp->ptNext;
	}
	return -1;
}


/* 设置字体模块的信息,比如指定字库文件,指定字符尺寸 */
int SetFontsDetail(char *pcFontsName, char *pcFontsFile, unsigned int dwFontSize)
{
	int ret = 0;
	PT_FontOpr ptFontOpr;

	ptFontOpr = GetFontOpr(pcFontsName);
	if (NULL == ptFontOpr)
	{
		return -1;
	}

	g_dwFontSize = dwFontSize;

	ret = ptFontOpr->FontInit(pcFontsFile, dwFontSize);
	
	return ret;
}


/* 调用各个模块的初始化函数 */
int FontsInit(void)
{
	int ret;

#if 0	
	ret = ASCIIInit();
	if (ret)
	{
		printf("ASCIIInit error!\n");
		return -1;
	}

	ret = GBKInit();
	if (ret)
	{
		printf("GBKInit error!\n");
		return -1;
	}
#endif	
	ret = FreeTypeInit();
	if (ret)
	{
		printf("FreeTypeInit error!\n");
		return -1;
	}

	return 0;
}

