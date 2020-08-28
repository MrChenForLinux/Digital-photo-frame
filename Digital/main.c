#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <config.h>
#include <encoding_manager.h>
#include <fonts_manager.h>
#include <disp_manager.h>
#include <input_manager.h>
#include <pic_operation.h>
#include <render.h>
#include <string.h>
#include <picfmt_manager.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


/* digitpic <freetype_file> */
int main(int argc, char **argv)
{	
	int ret;

	if (argc != 2)
	{
		printf("Usage:\n");
		printf("%s <freetype_file>\n", argv[0]);
		return 0;
	}

	/* 注册显示设备 */
	DisplayInit();
	/* 可能可支持多个显示设备: 选择和初始化指定的显示设备 */
	SelectAndInitDefaultDispDev("fb");

	/* 分配5个VideoMem缓存，用于预先存放要显示的数据，以加快显示 */
	AllocVideoMem(5);

    /* 注册输入设备 */
	InputInit();
    /* 调用所有输入设备的初始化函数 */
	AllInputDevicesInit();

    /* 注册编码模块 */
    EncodingInit();

    /* 注册字库模块 */
	ret = FontsInit();
	if (ret)
	{
		printf("FontsInit error!\n");
	}

    /* 设置freetype字库所用的文件和字体尺寸 */
	ret = SetFontsDetail("freetype", argv[1], 24);
	if (ret)
	{
		printf("SetFontsDetail error!\n");
	}

    /* 注册图片文件解析模块 */
    PicFmtsInit();

    /* 注册页面 */
	PagesInit();

    /* 运行主页面 */
	Page("main")->Run(NULL);
		
	return 0;
}

