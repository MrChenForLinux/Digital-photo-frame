
#ifndef _FONTS_MANAGER_H
#define _FONTS_MANAGER_H

typedef struct FontBitMap {
	int iXLeft;  						/* λͼ���Ͻ�X���� */
	int iYTop;   						/* λͼ���Ͻ�Y���� */
	int iXMax;   						/* λͼ�����X����ֵ */
	int iYMax;   						/* λͼ�����Y����ֵ */
	int iBpp;    						/* λͼ��һ�������ö���λ����ʾ */
	int iPitch;  						/* ���ڵ�ɫλͼ, ��������֮��Ŀ��, ����N�С���N+1����������֮���ƫ��ֵ */
	int iCurOriginX;  					/* λͼ��ԭ��X����(һ�������½�), ʹ��ԭ����ȷ�������ַ���λ�� */
	int iCurOriginY;  					/* λͼ��ԭ��Y���� */
	int iNextOriginX; 					/* ��һ���ַ����ұ��ַ���ԭ��X���� */
	int iNextOriginY; 					/* ��һ���ַ����ұ��ַ���ԭ��Y���� */
	unsigned char *pucBuffer;  			/* �����ַ���λͼ���� */
}T_FontBitMap, *PT_FontBitMap;

typedef struct FontOpr {
	char *name;             			// ����ģ����
	int (*FontInit)(char *pcFontFile, unsigned int dwFontSize);  // ��ʼ������
	int (*GetFontBitmap)(unsigned int dwCode, PT_FontBitMap ptFontBitMap);  // ���ݱ���ֵ����ַ���λͼ
	void (*SetFontSize)(unsigned int dwFontSize);   // ��������ߴ�
	struct FontOpr *ptNext;  
}T_FontOpr, *PT_FontOpr;


/* ע������ģ�� */
int RegisterFontOpr(PT_FontOpr ptFontOpr);

/* ��ʾ֧�ֵ�����ģ�� */
void ShowFontOpr(void);

/* ��������ȡ��ָ������ģ�� */
PT_FontOpr GetFontOpr(char *pcName);

/* �����ַ��ĳߴ� */
void SetFontSize(unsigned int dwFontSize);

/* ����ַ��ߴ� */
unsigned int GetFontSize(void);

/* ��ȡ�ַ�λͼ */
int GetFontBitmap(unsigned int dwCode, PT_FontBitMap ptFontBitMap);

/* ��������ģ�����Ϣ,����ָ���ֿ��ļ�,ָ���ַ��ߴ� */
int SetFontsDetail(char *pcFontsName, char *pcFontsFile, unsigned int dwFontSize);

/* ���ø�������ģ��ĳ�ʼ������ */
int FontsInit(void);

/* ע��ASCII����ģ�� */
int ASCIIInit(void);

/* ע��GBK����ģ�� */
int GBKInit(void);

/* ע��FreeType����ģ�� */
int FreeTypeInit(void);

#endif 

