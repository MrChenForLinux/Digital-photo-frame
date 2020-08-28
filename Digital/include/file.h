
#ifndef _FILE_H
#define _FILE_H

#include <stdio.h>

typedef struct FileMap {
	char strFileName[256];   /* 文件名 */
	// int iFd; 
	FILE * tFp;              /* 文件句柄 */
	int iFileSize;           /* 文件大小 */
	unsigned char *pucFileMapMem;  /* 使用mmap函数映射文件得到的内存 */
}T_FileMap, *PT_FileMap;

/* 文件类别 */
typedef enum {
	FILETYPE_DIR = 0,  /* 目录 */
	FILETYPE_FILE,     /* 文件 */
}E_FileType;

/* 目录里的内容 */
typedef struct DirContent {
	char strName[256];     /* 名字 */
	E_FileType eFileType;  /* 类别 */	
}T_DirContent, *PT_DirContent;

/* 将一个文件映射到内存 mmap */
int MapFile(PT_FileMap ptFileMap);

/* munmap */
void UnMapFile(PT_FileMap ptFileMap);

/* 把某目录下所含的顶层子目录、顶层文件都记录下来,并且按名字排序 */
int GetDirContents(char *strDirName, PT_DirContent **pptDirContents, int *piNumber);

/* 释放内存 */
void FreeDirContents(PT_DirContent *aptDirContents, int iNumber);

/* 以深度优先的方式获得目录下的文件 
 *            即: 先获得顶层目录下的文件, 再进入一级子目录A
 *                再获得一级子目录A下的文件, 再进入二级子目录AA, ...
 *                处理完一级子目录A后, 再进入一级子目录B
 *
 *  "连播模式"下调用该函数获得要显示的文件
 *  有两种方法获得这些文件:
 *  1. 事先把所有文件的名字保存到某个缓冲区中
 *  2. 用到时再去搜索取出若干个文件名
 *  第1种方法比较简单,但是当文件很多时有可能导致内存不足.
 *  我们使用第2种方法:
 *  假设某目录(包括所有子目录)下所有的文件都给它编一个号
 */
int GetFilesIndir(char *strDirName, int *piStartNumberToRecord, int *piCurFileNumber, int *piFileCountHaveGet, int iFileCountTotal, char apstrFileNames[][256]);

#endif

