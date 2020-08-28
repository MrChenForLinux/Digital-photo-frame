
#ifndef _FILE_H
#define _FILE_H

#include <stdio.h>

typedef struct FileMap {
	char strFileName[256];   /* �ļ��� */
	// int iFd; 
	FILE * tFp;              /* �ļ���� */
	int iFileSize;           /* �ļ���С */
	unsigned char *pucFileMapMem;  /* ʹ��mmap����ӳ���ļ��õ����ڴ� */
}T_FileMap, *PT_FileMap;

/* �ļ���� */
typedef enum {
	FILETYPE_DIR = 0,  /* Ŀ¼ */
	FILETYPE_FILE,     /* �ļ� */
}E_FileType;

/* Ŀ¼������� */
typedef struct DirContent {
	char strName[256];     /* ���� */
	E_FileType eFileType;  /* ��� */	
}T_DirContent, *PT_DirContent;

/* ��һ���ļ�ӳ�䵽�ڴ� mmap */
int MapFile(PT_FileMap ptFileMap);

/* munmap */
void UnMapFile(PT_FileMap ptFileMap);

/* ��ĳĿ¼�������Ķ�����Ŀ¼�������ļ�����¼����,���Ұ��������� */
int GetDirContents(char *strDirName, PT_DirContent **pptDirContents, int *piNumber);

/* �ͷ��ڴ� */
void FreeDirContents(PT_DirContent *aptDirContents, int iNumber);

/* ��������ȵķ�ʽ���Ŀ¼�µ��ļ� 
 *            ��: �Ȼ�ö���Ŀ¼�µ��ļ�, �ٽ���һ����Ŀ¼A
 *                �ٻ��һ����Ŀ¼A�µ��ļ�, �ٽ��������Ŀ¼AA, ...
 *                ������һ����Ŀ¼A��, �ٽ���һ����Ŀ¼B
 *
 *  "����ģʽ"�µ��øú������Ҫ��ʾ���ļ�
 *  �����ַ��������Щ�ļ�:
 *  1. ���Ȱ������ļ������ֱ��浽ĳ����������
 *  2. �õ�ʱ��ȥ����ȡ�����ɸ��ļ���
 *  ��1�ַ����Ƚϼ�,���ǵ��ļ��ܶ�ʱ�п��ܵ����ڴ治��.
 *  ����ʹ�õ�2�ַ���:
 *  ����ĳĿ¼(����������Ŀ¼)�����е��ļ���������һ����
 */
int GetFilesIndir(char *strDirName, int *piStartNumberToRecord, int *piCurFileNumber, int *piFileCountHaveGet, int iFileCountTotal, char apstrFileNames[][256]);

#endif

