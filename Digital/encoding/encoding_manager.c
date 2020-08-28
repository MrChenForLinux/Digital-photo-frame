#include <config.h>
#include <encoding_manager.h>
#include <string.h>
#include <stdlib.h>

static PT_EncodingOpr g_ptEncodingOprHead;

/* ע�����ģ�飬����ģ��ָ����ĳһ�����ʽ�ļ������ֵ�ȡ����ʽ*/
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


/* ��������ȡ��ָ���ı���ģ�� */
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


/* ��ʾ֧�ֵı��뷽ʽ */
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

/* ���ļ�ѡ��һ������ģ�� */
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

/* ���ݱ��뷽ʽ��������ģ�飬�Ի���ַ�λͼ
 * ��ʾһ���ַ���
 * 1����ȡ����ֵ
 * 2�����ݱ���ֵ��ȥ�ֿ��ļ����ҵ���Ӧ��λͼ
 * 3����λͼ��LCD�軭����
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
        /* ����һ���µ�T_FontOpr�ṹ�� */
		ptFontOprCpy = malloc(sizeof(T_FontOpr));
		if (!ptFontOprCpy)
		{
			return -1;
		}
		else
		{
            /* ���ƽṹ��������� */
			memcpy(ptFontOprCpy, ptFontOpr, sizeof(T_FontOpr));

            /* ���½ṹ�����ptEncodingOpr->ptFontOprSupportedHead���� 
             * Ϊ�β�ֱ�Ӱ�ԭ����ptFontOpr��������?
             * ��Ϊԭ����ptFontOpr->ptNext�Ѿ���RegisterFontOpr�����б�ռ����
             */
			ptFontOprCpy->ptNext = ptEncodingOpr->ptFontOprSupportedHead;
			ptEncodingOpr->ptFontOprSupportedHead = ptFontOprCpy;
			return 0;
		}		
	}
}


/* ȥ��ĳ������ģ�� */
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
			/* ɾ��ͷ�ڵ� */
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
				/* ��������ȡ�����ͷ� */
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

/* ���ø��ֱ��뷽ʽ�ĳ�ʼ������ */
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

/* ���ַ�����ȡ��UTF8��ʽ���ַ�����UNICODE����ֵ */
int GetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode)
{
    return Encode("utf-8")->GetCodeFrmBuf(pucBufStart, pucBufEnd, pdwCode);
}
