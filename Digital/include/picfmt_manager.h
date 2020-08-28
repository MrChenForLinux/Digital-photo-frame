
#ifndef _PIC_MANAGER_H
#define _PIC_MANAGER_H

#include <config.h>
#include <pic_operation.h>
#include <page_manager.h>
#include <file.h>

/* 注册图片解析模块（从不同格式图片中取出像素数据）*/
int RegisterPicFileParser(PT_PicFileParser ptPicFileParser);

/* 显示支持的解析模块*/
void ShowPicFmts(void);

/* 根据名字取出解析模块 */
PT_PicFileParser Parser(char *pcName);

/* 找到支持指定文件格式的解析模块 */
PT_PicFileParser GetParser(PT_FileMap ptFileMap);

/* 注册各个模块 */
int PicFmtsInit(void);

int JPGParserInit(void);

int BMPParserInit(void);

#endif

