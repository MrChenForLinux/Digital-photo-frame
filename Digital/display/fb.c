#include <config.h>
#include <disp_manager.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <string.h>

static int FBDeviceInit(void);
static int FBShowPixel(int iX, int iY, unsigned int dwColor);
static int FBCleanScreen(unsigned int dwBackColor);
static int FBShowPage(PT_VideoMem ptVideoMem);


static int fd;

static struct fb_var_screeninfo FBVar;
static struct fb_fix_screeninfo FBFix;			
static unsigned char *FBMem;
static unsigned int ScreenSize;

static unsigned int LineWidth;
static unsigned int PixelWidth;

static T_DispOpr FBOpr = {
	.name        = "fb",
	.DeviceInit  = FBDeviceInit,
	.ShowPixel   = FBShowPixel,
	.CleanScreen = FBCleanScreen,
	.ShowPage    = FBShowPage,
};

/* 初始化fb */
static int FBDeviceInit(void)
{
	int ret;
	
	fd = open(FB_DEVICE_NAME, O_RDWR);
	if (0 > fd)
	{
		printf("can't open %s\n", FB_DEVICE_NAME);
	}

	//获取可变参数
	ret = ioctl(fd, FBIOGET_VSCREENINFO, &FBVar);
	if (ret < 0)
	{
		printf("can't get fb's var\n");
		return -1;
	}

	//获取固定参数
	ret = ioctl(fd, FBIOGET_FSCREENINFO, &FBFix);
	if (ret < 0)
	{
		printf("can't get fb's fix\n");
		return -1;
	}
	
	ScreenSize = FBVar.xres * FBVar.yres * FBVar.bits_per_pixel / 8;
	//内存映射
	FBMem = (unsigned char *)mmap(NULL , ScreenSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (0 > FBMem)	
	{
		printf("can't mmap\n");
		return -1;
	}
	
	FBOpr.iXres       = FBVar.xres;
	FBOpr.iYres       = FBVar.yres;
	FBOpr.iBpp        = FBVar.bits_per_pixel;
	FBOpr.iLineWidth  = FBVar.xres * FBOpr.iBpp / 8;
	FBOpr.DispMem  	  = FBMem;

	LineWidth  = FBVar.xres * FBVar.bits_per_pixel / 8;
	PixelWidth = FBVar.bits_per_pixel / 8;
	
	return 0;
}


/* 指定fb上的某一像素为指定颜色 */
static int FBShowPixel(int iX, int iY, unsigned int dwColor)
{
	unsigned char *pucFB;
	unsigned short *pwFB16bpp;
	unsigned int *pdwFB32bpp;
	unsigned short wColor16bpp;
	int iRed;
	int iGreen;
	int iBlue;

	if ((iX >= FBVar.xres) || (iY >= FBVar.yres))
	{
		printf("out of region\n");
		return -1;
	}

	pucFB      = FBMem + LineWidth * iY + PixelWidth * iX;
	pwFB16bpp  = (unsigned short *)pucFB;
	pdwFB32bpp = (unsigned int *)pucFB;
	
	//根据像素位数，构建RGB
	switch (FBVar.bits_per_pixel)
	{
		case 8:
		{
			*pucFB = (unsigned char)dwColor;
			break;
		}
		case 16:
		{
			iRed   = (dwColor >> (16+3)) & 0x1f;
			iGreen = (dwColor >> (8+2)) & 0x3f;
			iBlue  = (dwColor >> 3) & 0x1f;
			wColor16bpp = (iRed << 11) | (iGreen << 5) | iBlue;
			*pwFB16bpp	= wColor16bpp;
			break;
		}
		case 32:
		{
			*pdwFB32bpp = dwColor;
			break;
		}
		default :
		{
			printf("can't support %d bpp\n", FBVar.bits_per_pixel);
			return -1;
		}
	}

	return 0;
}

/* 显示一整页 */
static int FBShowPage(PT_VideoMem ptVideoMem)
{
	memcpy(FBOpr.DispMem, ptVideoMem->tPixelDatas.aucPixelDatas, ptVideoMem->tPixelDatas.iTotalBytes);
	return 0;
}

/* 清屏 */
static int FBCleanScreen(unsigned int dwBackColor)
{
	unsigned char *pucFB;
	unsigned short *pwFB16bpp;
	unsigned int *pdwFB32bpp;
	unsigned short wColor16bpp;
	int iRed;
	int iGreen;
	int iBlue;
	int i = 0;

	pucFB      = FBMem;
	pwFB16bpp  = (unsigned short *)pucFB;
	pdwFB32bpp = (unsigned int *)pucFB;

	switch (FBVar.bits_per_pixel)
	{
		case 8:
		{
			memset(FBMem, dwBackColor, ScreenSize);
			break;
		}
		case 16:
		{
			iRed   = (dwBackColor >> (16+3)) & 0x1f;
			iGreen = (dwBackColor >> (8+2)) & 0x3f;
			iBlue  = (dwBackColor >> 3) & 0x1f;
			wColor16bpp = (iRed << 11) | (iGreen << 5) | iBlue;
			while (i < ScreenSize)
			{
				*pwFB16bpp	= wColor16bpp;
				pwFB16bpp++;
				i += 2;
			}
			break;
		}
		case 32:
		{
			while (i < ScreenSize)
			{
				*pdwFB32bpp	= dwBackColor;
				pdwFB32bpp++;
				i += 4;
			}
			break;
		}
		default :
		{
			printf("can't support %d bpp\n", FBVar.bits_per_pixel);
			return -1;
		}
	}

	return 0;
}

/* 注册FB设备 */
int FBInit(void)
{
	return RegisterDispOpr(&FBOpr);
}

