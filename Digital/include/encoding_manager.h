#ifndef _ENCODING_MANAGER_H
#define _ENCODING_MANAGER_H

#include <fonts_manager.h>
#include <disp_manager.h>

typedef struct EncodingOpr {
	char *name;    									// ����ģ����
	int iHeadLen;  									// �ļ�ͷ�ĳ��ȣ��ļ���ͷ�����ֽڱ�ʾ���ı��뷽ʽ
	PT_FontOpr ptFontOprSupportedHead;  			// ֧�ִ��ļ��ı��뷽ʽ��Ӧ�Ľṹ��ָ��
	int (*isSupport)(unsigned char *pucBufHead);  	// �ж��Ƿ�֧��ĳ�ļ�
	int (*GetCodeFrmBuf)(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode);  //ȡ��һ���ַ��ı��뷽ʽ
	struct EncodingOpr *ptNext;  				
}T_EncodingOpr, *PT_EncodingOpr;


/* ע�����ģ�飬����ģ��ָ����ĳһ�����ʽ�ļ������ֵ�ȡ����ʽ*/
int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr);


/* ��������ȡ��ָ���ı���ģ�� */
PT_EncodingOpr Encode(char *pcName);

/* ��ʾ֧�ֵı��뷽ʽ */
void ShowEncodingOpr(void);

/* ���ļ�ѡ��һ������ģ�� */
PT_EncodingOpr SelectEncodingOprForFile(unsigned char *pucFileBufHead);

/* ���ݱ��뷽ʽ��������ģ�飬�Ի���ַ�λͼ
 * ��ʾһ���ַ���
 * 1����ȡ����ֵ
 * 2�����ݱ���ֵ��ȥ�ֿ��ļ����ҵ���Ӧ��λͼ
 * 3����λͼ��LCD�軭����
 */
int AddFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr);

/* ȥ��ĳ������ģ�� */
int DelFontOprFrmEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr);

/* ���ø��ֱ��뷽ʽ�ĳ�ʼ������ */
int EncodingInit(void);

/* ���ַ�����ȡ��UTF8��ʽ���ַ�����UNICODE����ֵ */
int GetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode);

/* ע��ASCII����ģ�� */
int AsciiEncodingInit(void);

/* ע��Utf16be����ģ�� */
int  Utf16beEncodingInit(void);

/* ע��Utf16le����ģ�� */
int  Utf16leEncodingInit(void);

/* ע��Utf8����ģ�� */
int  Utf8EncodingInit(void);

#endif
