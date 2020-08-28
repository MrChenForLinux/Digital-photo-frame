#include <config.h>
#include <disp_manager.h>
#include <string.h>

static PT_DispOpr g_ptDispOprHead;
static PT_DispOpr g_ptDefaultDispOpr;
static PT_VideoMem g_ptVideoMemHead;

/* 注册某个显示模块，加入到链表中（链表中存放已经注册的显示设备）*/
int RegisterDispOpr(PT_DispOpr ptDispOpr)
{
	PT_DispOpr tmp;

	if (!g_ptDispOprHead)
	{
		g_ptDispOprHead   = ptDispOpr;
		ptDispOpr->ptNext = NULL;
	}
	else
	{
		tmp = g_ptDispOprHead;
		while (tmp->ptNext)
		{
			tmp = tmp->ptNext;
		}
		tmp->ptNext	  = ptDispOpr;
		ptDispOpr->ptNext = NULL;
	}

	return 0;
}


/* 显示某个已注册的显示设备模块 */
void ShowDispOpr(void)
{
	int i = 0;
	PT_DispOpr tmp = g_ptDispOprHead;

	while (tmp)
	{
		printf("%02d %s\n", i++, tmp->name);
		tmp = tmp->ptNext;
	}
}

/* 根据名字取出指定的显示设备模块 */
PT_DispOpr GetDispOpr(char *pcName)
{
	PT_DispOpr tmp = g_ptDispOprHead;
	
	while (tmp)
	{
		if (strcmp(tmp->name, pcName) == 0)
		{
			return tmp;
		}
		tmp = tmp->ptNext;
	}
	return NULL;
}

/* 根据名字取出指定的显示设备模块，并调用初始化函数，设为默认设备*/
void SelectAndInitDefaultDispDev(char *name)
{
	g_ptDefaultDispOpr = GetDispOpr(name);
	if (g_ptDefaultDispOpr)
	{
		g_ptDefaultDispOpr->DeviceInit();
		g_ptDefaultDispOpr->CleanScreen(0);
	}
}

/* 返回选择的默认设备 */
PT_DispOpr GetDefaultDispDev(void)
{
	return g_ptDefaultDispOpr;
}

/* 获取显示设备的分辨率和Bpp */
int GetDispResolution(int *piXres, int *piYres, int *piBpp)
{
	if (g_ptDefaultDispOpr)
	{
		*piXres = g_ptDefaultDispOpr->iXres;
		*piYres = g_ptDefaultDispOpr->iYres;
		*piBpp  = g_ptDefaultDispOpr->iBpp;
		return 0;
	}
	else
	{
		return -1;
	}
}

/* 分配用于缓存的VideoMem
 *  VideoMem: 
 *            为加快显示速度,我们事先在缓存中构造好显示的页面的数据,
 *            存入VideoMem，显示时再把VideoMem中的数据复制到设备的显存上       
 */
int AllocVideoMem(int iNum)
{
	int i;

	int iXres = 0;
	int iYres = 0;
	int iBpp  = 0;

	int iVMSize;
	int iLineBytes;

	PT_VideoMem ptNew;

	GetDispResolution(&iXres, &iYres, &iBpp);
	iVMSize = iXres * iYres * iBpp / 8;
	iLineBytes = iXres * iBpp / 8;

	/* 先把设备本身的framebuffer放入链表
	   将tPixelDatas.aucPixelDatas指向显示设备的framebuffer */
	ptNew = malloc(sizeof(T_VideoMem));
	if (ptNew == NULL)
	{
		return -1;
	}

	ptNew->tPixelDatas.aucPixelDatas = g_ptDefaultDispOpr->DispMem;
	
	ptNew->iID = 0;
	ptNew->bDevFrameBuffer = 1;       	//表明设备本身的Framebuffer
	ptNew->eVideoMemState  = VMS_FREE;  
	ptNew->ePicState	   = PS_BLANK;
	ptNew->tPixelDatas.iWidth  = iXres;
	ptNew->tPixelDatas.iHeight = iYres;
	ptNew->tPixelDatas.iBpp    = iBpp;
	ptNew->tPixelDatas.iLineBytes  = iLineBytes;
	ptNew->tPixelDatas.iTotalBytes = iVMSize;

	/* 如果下面要分配用于缓存的VideoMem, 
	 * 把设备本身framebuffer对应的VideoMem状态设置为VMS_USED_FOR_CUR,
	 * 表示这个VideoMem不会被作为缓存分配出去
	 */
	if (iNum != 0)
	{
		ptNew->eVideoMemState = VMS_USED_FOR_CUR;
	}
	
	/* 放入链表 */
	ptNew->ptNext = g_ptVideoMemHead;
	g_ptVideoMemHead = ptNew;
	

	/* 分配用于缓存的VideoMem */
	 
	for (i = 0; i < iNum; i++)
	{
		/* 分配T_VideoMem结构体本身和"跟framebuffer同样大小的缓存"
		 * 即T_VideoMem结构体后面紧跟着数据缓存区，这样ptNew+1即是数据缓冲区地址
		 */
		ptNew = malloc(sizeof(T_VideoMem) + iVMSize);
		if (ptNew == NULL)
		{
			return -1;
		}

		ptNew->tPixelDatas.aucPixelDatas = (unsigned char *)(ptNew + 1);

		ptNew->iID = 0;
		ptNew->bDevFrameBuffer = 0;
		ptNew->eVideoMemState = VMS_FREE;
		ptNew->ePicState      = PS_BLANK;
		ptNew->tPixelDatas.iWidth  = iXres;
		ptNew->tPixelDatas.iHeight = iYres;
		ptNew->tPixelDatas.iBpp    = iBpp;
		ptNew->tPixelDatas.iLineBytes = iLineBytes;
		ptNew->tPixelDatas.iTotalBytes = iVMSize;

		/* 放入链表 */
		ptNew->ptNext = g_ptVideoMemHead;
		g_ptVideoMemHead = ptNew;
	}
	
	return 0;
}

/* 获得一块可操作的VideoMem */
PT_VideoMem GetVideoMem(int iID, int bCur)
{
	PT_VideoMem tmp = g_ptVideoMemHead;
	
	/* 1. 优先: 取出空闲的、ID相同的videomem */
	while (tmp)
	{
		if ((tmp->eVideoMemState == VMS_FREE) && (tmp->iID == iID))
		{
			tmp->eVideoMemState = bCur ? VMS_USED_FOR_CUR : VMS_USED_FOR_PREPARE;
			return tmp;
		}
		tmp = tmp->ptNext;
	}

	/* 2. 如果前面不成功, 取出一个空闲的并且里面没有数据(ptVideoMem->ePicState = PS_BLANK)的VideoMem */
	tmp = g_ptVideoMemHead;
	while (tmp)
	{
		if ((tmp->eVideoMemState == VMS_FREE) && (tmp->ePicState == PS_BLANK))
		{
			tmp->iID = iID;
			tmp->eVideoMemState = bCur ? VMS_USED_FOR_CUR : VMS_USED_FOR_PREPARE;
			return tmp;
		}
		tmp = tmp->ptNext;
	}	
	
	/* 3. 如果前面不成功: 取出任意一个空闲的VideoMem */
	tmp = g_ptVideoMemHead;
	while (tmp)
	{
		if (tmp->eVideoMemState == VMS_FREE)
		{
			tmp->iID = iID;
			tmp->ePicState = PS_BLANK;
			tmp->eVideoMemState = bCur ? VMS_USED_FOR_CUR : VMS_USED_FOR_PREPARE;
			return tmp;
		}
		tmp = tmp->ptNext;
	}

    /* 4. 如果没有空闲的VideoMem并且bCur为1, 则取出任意一个VideoMem(不管它是否空闲) */
    if (bCur)
    {
    	tmp = g_ptVideoMemHead;
    	tmp->iID = iID;
    	tmp->ePicState = PS_BLANK;
    	tmp->eVideoMemState = bCur ? VMS_USED_FOR_CUR : VMS_USED_FOR_PREPARE;
    	return tmp;
    }
    
	return NULL;
}

/* 释放取出的VideoMem */
void PutVideoMem(PT_VideoMem ptVideoMem)
{
	ptVideoMem->eVideoMemState = VMS_FREE;  /* 设置VideoMem状态为空闲 */
    if (ptVideoMem->iID == -1)
    {
        ptVideoMem->ePicState = PS_BLANK;  /* 表示里面的数据没有用了 */
    }
}

/* 获得显存 */
PT_VideoMem GetDevVideoMem(void)
{
	PT_VideoMem tmp = g_ptVideoMemHead;
	
	while (tmp)
	{
		if (tmp->bDevFrameBuffer)
		{
			return tmp;
		}
		tmp = tmp->ptNext;
	}
	return NULL;
}


/* 清除VideoMem为某种颜色 */
void ClearVideoMem(PT_VideoMem ptVideoMem, unsigned int dwColor)
{
	unsigned char *pucVM;
	unsigned short *pwVM16bpp;
	unsigned int *pdwVM32bpp;
	unsigned short wColor16bpp;
	int iRed;
	int iGreen;
	int iBlue;
	int i = 0;

	pucVM	   = ptVideoMem->tPixelDatas.aucPixelDatas;
	pwVM16bpp  = (unsigned short *)pucVM;
	pdwVM32bpp = (unsigned int *)pucVM;

	switch (ptVideoMem->tPixelDatas.iBpp)
	{
		case 8:
		{
			memset(pucVM, dwColor, ptVideoMem->tPixelDatas.iTotalBytes);
			break;
		}
		case 16:
		{
			iRed   = (dwColor >> (16+3)) & 0x1f;
			iGreen = (dwColor >> (8+2)) & 0x3f;
			iBlue  = (dwColor >> 3) & 0x1f;
			wColor16bpp = (iRed << 11) | (iGreen << 5) | iBlue;
			while (i < ptVideoMem->tPixelDatas.iTotalBytes)
			{
				*pwVM16bpp	= wColor16bpp;
				pwVM16bpp++;
				i += 2;
			}
			break;
		}
		case 32:
		{
			while (i < ptVideoMem->tPixelDatas.iTotalBytes)
			{
				*pdwVM32bpp = dwColor;
				pdwVM32bpp++;
				i += 4;
			}
			break;
		}
		default :
		{
			printf("can't support %d bpp\n", ptVideoMem->tPixelDatas.iBpp);
			return;
		}
	}

}


/* 清除VideoMem指定内存区域为某种颜色*/
void ClearVideoMemRegion(PT_VideoMem ptVideoMem, PT_Layout ptLayout, unsigned int dwColor)
{
	unsigned char *pucVM;
	unsigned short *pwVM16bpp;
	unsigned int *pdwVM32bpp;
	unsigned short wColor16bpp;
	int iRed;
	int iGreen;
	int iBlue;
	int iX;
	int iY;
    int iLineBytesClear;
    int i;

	pucVM	   = ptVideoMem->tPixelDatas.aucPixelDatas + ptLayout->iTopLeftY * ptVideoMem->tPixelDatas.iLineBytes + ptLayout->iTopLeftX * ptVideoMem->tPixelDatas.iBpp / 8;
	pwVM16bpp  = (unsigned short *)pucVM;
	pdwVM32bpp = (unsigned int *)pucVM;

    iLineBytesClear = (ptLayout->iBotRightX - ptLayout->iTopLeftX + 1) * ptVideoMem->tPixelDatas.iBpp / 8;

	switch (ptVideoMem->tPixelDatas.iBpp)
	{
		case 8:
		{
            for (iY = ptLayout->iTopLeftY; iY <= ptLayout->iBotRightY; iY++)
            {
    			memset(pucVM, dwColor, iLineBytesClear);
                pucVM += ptVideoMem->tPixelDatas.iLineBytes;
            }
			break;
		}
		case 16:
		{
			iRed   = (dwColor >> (16+3)) & 0x1f;
			iGreen = (dwColor >> (8+2)) & 0x3f;
			iBlue  = (dwColor >> 3) & 0x1f;
			wColor16bpp = (iRed << 11) | (iGreen << 5) | iBlue;
            for (iY = ptLayout->iTopLeftY; iY <= ptLayout->iBotRightY; iY++)
            {
                i = 0;
                for (iX = ptLayout->iTopLeftX; iX <= ptLayout->iBotRightX; iX++)
    			{
    				pwVM16bpp[i++]	= wColor16bpp;
    			}
                pwVM16bpp = (unsigned short *)((unsigned int)pwVM16bpp + ptVideoMem->tPixelDatas.iLineBytes);
            }
			break;
		}
		case 32:
		{
            for (iY = ptLayout->iTopLeftY; iY <= ptLayout->iBotRightY; iY++)
            {
                i = 0;
                for (iX = ptLayout->iTopLeftX; iX <= ptLayout->iBotRightX; iX++)
    			{
    				pdwVM32bpp[i++]	= dwColor;
    			}
                pdwVM32bpp = (unsigned int *)((unsigned int)pdwVM32bpp + ptVideoMem->tPixelDatas.iLineBytes);
            }
			break;
		}
		default :
		{
			printf("can't support %d bpp\n", ptVideoMem->tPixelDatas.iBpp);
			return;
		}
	}

}

/* 注册显示设备 */
int DisplayInit(void)
{
	int ret;
	
	ret = FBInit();

	return ret;
}

