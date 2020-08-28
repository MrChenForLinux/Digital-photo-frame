
#include <config.h>
#include <pic_operation.h>
#include <picfmt_manager.h>
#include <string.h>


static PT_PicFileParser g_ptPicFileParserHead;

/* ע��ͼƬ����ģ�飨�Ӳ�ͬ��ʽͼƬ��ȡ���������ݣ�*/
int RegisterPicFileParser(PT_PicFileParser ptPicFileParser)
{
	PT_PicFileParser ptTmp;

	if (!g_ptPicFileParserHead)
	{
		g_ptPicFileParserHead   = ptPicFileParser;
		ptPicFileParser->ptNext = NULL;
	}
	else
	{
		ptTmp = g_ptPicFileParserHead;
		while (ptTmp->ptNext)
		{
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext	  = ptPicFileParser;
		ptPicFileParser->ptNext = NULL;
	}

	return 0;
}


/* ��ʾ֧�ֵĽ���ģ��*/
void ShowPicFmts(void)
{
	int i = 0;
	PT_PicFileParser ptTmp = g_ptPicFileParserHead;

	while (ptTmp)
	{
		printf("%02d %s\n", i++, ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}
}

/* ��������ȡ������ģ�� */
PT_PicFileParser Parser(char *pcName)
{
	PT_PicFileParser ptTmp = g_ptPicFileParserHead;
	
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

/* �ҵ�֧��ָ���ļ���ʽ�Ľ���ģ�� */
PT_PicFileParser GetParser(PT_FileMap ptFileMap)
{
	PT_PicFileParser ptTmp = g_ptPicFileParserHead;
	
	while (ptTmp)
	{
		if (ptTmp->isSupport(ptFileMap))
		{
			return ptTmp;
		}
		ptTmp = ptTmp->ptNext;
	}
	return NULL;
}

/* ע�����ģ�� */
int PicFmtsInit(void)
{
	int ret;

	ret = BMPParserInit();
	ret |= JPGParserInit();
		
	return ret;
}




