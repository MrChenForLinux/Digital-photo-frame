
#ifndef _PIC_MANAGER_H
#define _PIC_MANAGER_H

#include <config.h>
#include <pic_operation.h>
#include <page_manager.h>
#include <file.h>

/* ע��ͼƬ����ģ�飨�Ӳ�ͬ��ʽͼƬ��ȡ���������ݣ�*/
int RegisterPicFileParser(PT_PicFileParser ptPicFileParser);

/* ��ʾ֧�ֵĽ���ģ��*/
void ShowPicFmts(void);

/* ��������ȡ������ģ�� */
PT_PicFileParser Parser(char *pcName);

/* �ҵ�֧��ָ���ļ���ʽ�Ľ���ģ�� */
PT_PicFileParser GetParser(PT_FileMap ptFileMap);

/* ע�����ģ�� */
int PicFmtsInit(void);

int JPGParserInit(void);

int BMPParserInit(void);

#endif

