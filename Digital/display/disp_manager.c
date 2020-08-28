#include <config.h>
#include <disp_manager.h>
#include <string.h>

static PT_DispOpr g_ptDispOprHead;
static PT_DispOpr g_ptDefaultDispOpr;
static PT_VideoMem g_ptVideoMemHead;

/* ע��ĳ����ʾģ�飬���뵽�����У������д���Ѿ�ע�����ʾ�豸��*/
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


/* ��ʾĳ����ע�����ʾ�豸ģ�� */
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

/* ��������ȡ��ָ������ʾ�豸ģ�� */
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

/* ��������ȡ��ָ������ʾ�豸ģ�飬�����ó�ʼ����������ΪĬ���豸*/
void SelectAndInitDefaultDispDev(char *name)
{
	g_ptDefaultDispOpr = GetDispOpr(name);
	if (g_ptDefaultDispOpr)
	{
		g_ptDefaultDispOpr->DeviceInit();
		g_ptDefaultDispOpr->CleanScreen(0);
	}
}

/* ����ѡ���Ĭ���豸 */
PT_DispOpr GetDefaultDispDev(void)
{
	return g_ptDefaultDispOpr;
}

/* ��ȡ��ʾ�豸�ķֱ��ʺ�Bpp */
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

/* �������ڻ����VideoMem
 *  VideoMem: 
 *            Ϊ�ӿ���ʾ�ٶ�,���������ڻ����й������ʾ��ҳ�������,
 *            ����VideoMem����ʾʱ�ٰ�VideoMem�е����ݸ��Ƶ��豸���Դ���       
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

	/* �Ȱ��豸�����framebuffer��������
	   ��tPixelDatas.aucPixelDatasָ����ʾ�豸��framebuffer */
	ptNew = malloc(sizeof(T_VideoMem));
	if (ptNew == NULL)
	{
		return -1;
	}

	ptNew->tPixelDatas.aucPixelDatas = g_ptDefaultDispOpr->DispMem;
	
	ptNew->iID = 0;
	ptNew->bDevFrameBuffer = 1;       	//�����豸�����Framebuffer
	ptNew->eVideoMemState  = VMS_FREE;  
	ptNew->ePicState	   = PS_BLANK;
	ptNew->tPixelDatas.iWidth  = iXres;
	ptNew->tPixelDatas.iHeight = iYres;
	ptNew->tPixelDatas.iBpp    = iBpp;
	ptNew->tPixelDatas.iLineBytes  = iLineBytes;
	ptNew->tPixelDatas.iTotalBytes = iVMSize;

	/* �������Ҫ�������ڻ����VideoMem, 
	 * ���豸����framebuffer��Ӧ��VideoMem״̬����ΪVMS_USED_FOR_CUR,
	 * ��ʾ���VideoMem���ᱻ��Ϊ��������ȥ
	 */
	if (iNum != 0)
	{
		ptNew->eVideoMemState = VMS_USED_FOR_CUR;
	}
	
	/* �������� */
	ptNew->ptNext = g_ptVideoMemHead;
	g_ptVideoMemHead = ptNew;
	

	/* �������ڻ����VideoMem */
	 
	for (i = 0; i < iNum; i++)
	{
		/* ����T_VideoMem�ṹ�屾���"��framebufferͬ����С�Ļ���"
		 * ��T_VideoMem�ṹ�������������ݻ�����������ptNew+1�������ݻ�������ַ
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

		/* �������� */
		ptNew->ptNext = g_ptVideoMemHead;
		g_ptVideoMemHead = ptNew;
	}
	
	return 0;
}

/* ���һ��ɲ�����VideoMem */
PT_VideoMem GetVideoMem(int iID, int bCur)
{
	PT_VideoMem tmp = g_ptVideoMemHead;
	
	/* 1. ����: ȡ�����еġ�ID��ͬ��videomem */
	while (tmp)
	{
		if ((tmp->eVideoMemState == VMS_FREE) && (tmp->iID == iID))
		{
			tmp->eVideoMemState = bCur ? VMS_USED_FOR_CUR : VMS_USED_FOR_PREPARE;
			return tmp;
		}
		tmp = tmp->ptNext;
	}

	/* 2. ���ǰ�治�ɹ�, ȡ��һ�����еĲ�������û������(ptVideoMem->ePicState = PS_BLANK)��VideoMem */
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
	
	/* 3. ���ǰ�治�ɹ�: ȡ������һ�����е�VideoMem */
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

    /* 4. ���û�п��е�VideoMem����bCurΪ1, ��ȡ������һ��VideoMem(�������Ƿ����) */
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

/* �ͷ�ȡ����VideoMem */
void PutVideoMem(PT_VideoMem ptVideoMem)
{
	ptVideoMem->eVideoMemState = VMS_FREE;  /* ����VideoMem״̬Ϊ���� */
    if (ptVideoMem->iID == -1)
    {
        ptVideoMem->ePicState = PS_BLANK;  /* ��ʾ���������û������ */
    }
}

/* ����Դ� */
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


/* ���VideoMemΪĳ����ɫ */
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


/* ���VideoMemָ���ڴ�����Ϊĳ����ɫ*/
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

/* ע����ʾ�豸 */
int DisplayInit(void)
{
	int ret;
	
	ret = FBInit();

	return ret;
}

