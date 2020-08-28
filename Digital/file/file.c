#include <config.h>
#include <file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ��һ���ļ�ӳ�䵽�ڴ� mmap */
int MapFile(PT_FileMap ptFileMap)
{
	int iFd;
    FILE *tFp;
	struct stat tStat;
	
	/* ���ļ� */
	tFp = fopen(ptFileMap->strFileName, "r+");
	if (tFp == NULL)
	{
		printf("can't open %s\n", ptFileMap->strFileName);
		return -1;
	}
	ptFileMap->tFp = tFp;
    iFd = fileno(tFp);

	fstat(iFd, &tStat);
	ptFileMap->iFileSize = tStat.st_size;
	ptFileMap->pucFileMapMem = (unsigned char *)mmap(NULL , tStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, iFd, 0);
	if (ptFileMap->pucFileMapMem == (unsigned char *)-1)
	{
		printf("mmap error!\n");
		return -1;
	}
	return 0;
}

/* munmap */
void UnMapFile(PT_FileMap ptFileMap)
{
	munmap(ptFileMap->pucFileMapMem, ptFileMap->iFileSize);
	fclose(ptFileMap->tFp);
}

/* �ж�һ���ļ��Ƿ�ΪĿ¼ */
static int isDir(char *strFilePath, char *strFileName)
{
    char strTmp[256];
    struct stat tStat;

    snprintf(strTmp, 256, "%s/%s", strFilePath, strFileName);
    strTmp[255] = '\0';

    if ((stat(strTmp, &tStat) == 0) && S_ISDIR(tStat.st_mode))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* �ж�һ��Ŀ¼�Ƿ񳣹��Ŀ¼,�ڱ������а�sbin��Ŀ¼��������Ŀ¼���Դ� */
static int isRegDir(char *strDirPath, char *strSubDirName)
{
    static const char *astrSpecailDirs[] = {"sbin", "bin", "usr", "lib", "proc", "tmp", "dev", "sys", NULL};
    int i;
    
    /* ���Ŀ¼������"astrSpecailDirs"�е�����һ��, �򷵻�0 */
    if (0 == strcmp(strDirPath, "/"))
    {
        while (astrSpecailDirs[i])
        {
            if (0 == strcmp(strSubDirName, astrSpecailDirs[i]))
                return 0;
            i++;
        }
    }
    return 1;    
}

/* �ж�һ���ļ��Ƿ񳣹���ļ�,�豸�ڵ�/�����ļ�/FIFO�ļ����������ļ� */
static int isRegFile(char *strFilePath, char *strFileName)
{
    char strTmp[256];
    struct stat tStat;

    snprintf(strTmp, 256, "%s/%s", strFilePath, strFileName);
    strTmp[255] = '\0';

    if ((stat(strTmp, &tStat) == 0) && S_ISREG(tStat.st_mode))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/* ��ĳĿ¼�������Ķ�����Ŀ¼�������ļ�����¼����,���Ұ��������� */
int GetDirContents(char *strDirName, PT_DirContent **pptDirContents, int *piNumber)	
{
    PT_DirContent *aptDirContents;
	struct dirent **aptNameList;
	int iNumber;
	int i;
	int j;

	/* ɨ��Ŀ¼,�������������,����aptNameList[0],aptNameList[1],... */
	iNumber = scandir(strDirName, &aptNameList, 0, alphasort);
	if (iNumber < 0)
	{
		printf("scandir error : %s!\n", strDirName);
		return -1;
	}

	/* ����".", ".."������Ŀ¼ */
	aptDirContents = malloc(sizeof(PT_DirContent) * (iNumber - 2));
	if (NULL == aptDirContents)
	{
		printf("malloc error!\n");
		return -1;
	}
    *pptDirContents = aptDirContents;

	for (i = 0; i < iNumber - 2; i++)
	{
		aptDirContents[i] = malloc(sizeof(T_DirContent));
		if (NULL == aptDirContents)
		{
			printf("malloc error!\n");
			return -1;
		}
	}

	/* �Ȱ�Ŀ¼����������aptDirContents */
	for (i = 0, j = 0; i < iNumber; i++)
	{
		/* ����".",".."������Ŀ¼ */
		if ((0 == strcmp(aptNameList[i]->d_name, ".")) || (0 == strcmp(aptNameList[i]->d_name, "..")))
			continue;
        /* ���������е��ļ�ϵͳ��֧��d_type, ���Բ���ֱ���ж�d_type */
		/* if (aptNameList[i]->d_type == DT_DIR) */
        if (isDir(strDirName, aptNameList[i]->d_name))
		{
			strncpy(aptDirContents[j]->strName, aptNameList[i]->d_name, 256);
			aptDirContents[j]->strName[255] = '\0';
			aptDirContents[j]->eFileType    = FILETYPE_DIR;
            free(aptNameList[i]);
            aptNameList[i] = NULL;
			j++;
		}
	}

	/* �ٰѳ����ļ�����������aptDirContents */
	for (i = 0; i < iNumber; i++)
	{
        if (aptNameList[i] == NULL)
            continue;
        
		/* ����".",".."������Ŀ¼ */
		if ((0 == strcmp(aptNameList[i]->d_name, ".")) || (0 == strcmp(aptNameList[i]->d_name, "..")))
			continue;
        /* ���������е��ļ�ϵͳ��֧��d_type, ���Բ���ֱ���ж�d_type */
		/* if (aptNameList[i]->d_type == DT_REG) */
        if (isRegFile(strDirName, aptNameList[i]->d_name))
		{
			strncpy(aptDirContents[j]->strName, aptNameList[i]->d_name, 256);
			aptDirContents[j]->strName[255] = '\0';
			aptDirContents[j]->eFileType    = FILETYPE_FILE;
            free(aptNameList[i]);
            aptNameList[i] = NULL;
			j++;
		}
	}

	/* �ͷ�aptDirContents��δʹ�õ��� */
	for (i = j; i < iNumber - 2; i++)
	{
		free(aptDirContents[i]);
	}

	/* �ͷ�scandir����������ڴ� */
	for (i = 0; i < iNumber; i++)
	{
        if (aptNameList[i])
        {
    		free(aptNameList[i]);
        }
	}
	free(aptNameList);

	*piNumber = j;
	
	return 0;
}

/* �ͷ��ڴ� */
void FreeDirContents(PT_DirContent *aptDirContents, int iNumber)
{
	int i;
	for (i = 0; i < iNumber; i++)
	{
		free(aptDirContents[i]);
	}
	free(aptDirContents);
}

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
int GetFilesIndir(char *strDirName, int *piStartNumberToRecord, int *piCurFileNumber, int *piFileCountHaveGet, int iFileCountTotal, char apstrFileNames[][256])
{
    int ret;
    PT_DirContent *aptDirContents;  /* ����:����Ŀ¼��"������Ŀ¼","�ļ�"������ */
    int iDirContentsNumber;         /* aptDirContents�����ж����� */
    int i;
    char strSubDirName[256];

    /* Ϊ������ʵ�Ŀ¼����Ƕ��, �����ܷ��ʵ�Ŀ¼���Ϊ10 */
#define MAX_DIR_DEEPNESS 10    
    static int iDirDeepness = 0;

    if (iDirDeepness > MAX_DIR_DEEPNESS)
    {
        return -1;
    }

    iDirDeepness++;    
    
    ret = GetDirContents(strDirName, &aptDirContents, &iDirContentsNumber);    
    if (ret)
    {
        printf("GetDirContents error!\n");
        iDirDeepness--;
        return -1;
    }

    /* �ȼ�¼�ļ� */
    for (i = 0; i < iDirContentsNumber; i++)
    {
        if (aptDirContents[i]->eFileType == FILETYPE_FILE)
        {
            if (*piCurFileNumber >= *piStartNumberToRecord)
            {
                snprintf(apstrFileNames[*piFileCountHaveGet], 256, "%s/%s", strDirName, aptDirContents[i]->strName);
                (*piFileCountHaveGet)++;
                (*piCurFileNumber)++;
                (*piStartNumberToRecord)++;
                if (*piFileCountHaveGet >= iFileCountTotal)
                {
                    FreeDirContents(aptDirContents, iDirContentsNumber);
                    iDirDeepness--;
                    return 0;
                }
            }
            else
            {
                (*piCurFileNumber)++;
            }
        }
    }

    /* �ݹ鴦��Ŀ¼ */
    for (i = 0; i < iDirContentsNumber; i++)
    {
        if ((aptDirContents[i]->eFileType == FILETYPE_DIR) && isRegDir(strDirName, aptDirContents[i]->strName))
        {
            snprintf(strSubDirName, 256, "%s/%s", strDirName, aptDirContents[i]->strName);
            GetFilesIndir(strSubDirName, piStartNumberToRecord, piCurFileNumber, piFileCountHaveGet, iFileCountTotal, apstrFileNames);
            if (*piFileCountHaveGet >= iFileCountTotal)
            {
                FreeDirContents(aptDirContents, iDirContentsNumber);
                iDirDeepness--;
                return 0;
            }
        }
    }

    FreeDirContents(aptDirContents, iDirContentsNumber);
    iDirDeepness--;
    return 0;
}


