//  *************************************************************
//  Copyright (C) Technologies Co., Ltd.
//  
//  File name  : SuperLog.cpp
//  Description: 参数管理
//  Version    : 1.0
//  Author     : 
//  Created    : 
//  *************************************************************

#include "StdAfx.h"
#include "ParamMgr.h"
#include "Markup.h"
#include "SuperLog.h"

#define XML_FILE_NAME _T("ParamConfig.xml")
#define PARAM_CONFIG_SHARE_MEM _T("ParamConfigShareMem")
#define MAGIC_VALID 0x12345678
#define MAGIC_INVALID 0x87654321


//针对参数做统一处理方法
//有关配置的全局变量 可以配置其默认的初始值
GLOBAL_FLASH g_stGlobal_Flash = {
	//PTZ_PARA
	{
		COM1,
		COM_BAUD_2400,
		0x00,
		0,
		PTZ_PELCO_D,
	},
	//VIDEO_PARA
	{
		VIDEO_SRC_ANALOG,
		VIDEO_ENC_H264,			//编码格式
		{
			ANALOG_VBIT_TYPE_RGB24,
			//ANALOG_VFRAME_TYPE_D1,
			ANALOG_VFRAME_TYPE_CIF,
		},
		{
			DIGITAL_DVRDVS_HK,
			DIGITAL_ALARM_IQ,
			"127.0.0.1",
		},
		128,
		128,
		128,
		128, 
	},
	//ALG_PARA
	{
		-1,
		{
			{
				0,
				{
					0,
				},
			},
			{
				0,
			},
			{
				0,
			},
			{
				0,
			},
			{
				0,
			},
		}
	},
	//ALG_QUAD_PARA
	//坐标定义：奇数点为内圈，顺序依次为x1,x3,x5,x7,x9,x11,x13,x15，偶数点为依次对应的外圈，x2和x1对应
	{
		{
			0,
		},

	},
	//ALG_AREA_PARA
	{
		{
			0,
		},
	},
	//ALG_LINE_PARA
	{
		{
			0,
		},
	},
	//ALG_COUNT_PARA
	{
		{
			0,
		},
	},
	//ALG_ENTER_PARA
	{
		{
			0,
		},
	},
	//ALG_OUT_PARA
	{
		{
			0,
		},
	},
// 	//ALG_STOP_PARA
// 	{
// 		{
// 			0,
// 		},
// 	},
	//ALG_LOST_PARA
	{
		{
			0,
		},
	},
	//ALG_DIRECT_PARA
	{
		{
			0,
		},
	},
	//ALG_DETECT_PARA
	{
		{
			0,
		},
	},
	//ALG_PEOPLEDENSE_PARA
	{
		{
			0,
		},
	},
	//ALG_CLUSTERDETE_PARA
	{
		{
			0,
		},
	},
	//ALG_MOTIONHISTORY_PARA
	{
		{
			0,
		},
	},
	//ALG_HOVERDETE_PARA
	{
		{
			0,
		},
	},
	//ALG_ALARMLINE_PARA
	{
		{
			0,
		},
	},
	//ALG_FINDBAG_PARA
	{
		{
			0,
		},
	},
	//ALG_RUNNINGOBJ_PARA
	{
		{
			0,
		},
	},
	//SYS_CONFIG_PARA
	{
		{
			"201204271900000001000",
		    "识别器一",
		},
		CAM_STATE_WORKING,
		{
			"192.168.3.144",
			"255.255.255.0",
			"192.168.3.1",
			"00-30-67-46-95-F1",
			8011,  //客户端连接服务器的视频图像端口
		},
		{
			"192.168.3.144",
			"255.255.255.0",
			"192.168.3.1",
			"00-30-67-46-95-F1",
			8010,  //客户端连接服务器的视频图像端口	
		},
	},
	//SYS_STORAGE_PARA
	{
		{
			SYSTEM_TYPE_WINDOWS,
			0,
		},
	},
};//与文件的交互同步
//直接对g_stGlobal_Flash做进一步的处理函数定义

GLOBAL_VARIABLE g_stGlobal_Variable;//运行时使用的文件

CRITICAL_SECTION g_csGlobal_Variable_Section;

FileNameList_T g_FileNameList;//报警图片一级队列 优先发送 发送失败加入二级队列
FileNameList_T g_FileNameListReserved;//报警图片二级队列 当一级队列为空时进行发送 发送失败则丢弃

bool FlashParam_Init()
{
	InitializeCriticalSection(&g_csGlobal_Variable_Section);//访问全局变量的互斥锁
	InitializeCriticalSection(&g_stGlobal_Variable.csFileNameList);//访问报警文件的互斥锁
	return true;
}

//FlashParam_DownLoad 将g_stGlobal_Flash数据备份到文件中
bool FlashParam_DownLoad()
{
	//暂时将默认值赋值给全局变量中
	g_stGlobal_Flash = g_stGlobal_Variable.globalflash;
	TCHAR szPath[MAX_PATH];
	char Cur_path[MAX_PATH],Param_file[MAX_PATH];
	if( !GetModuleFileName( NULL, szPath, MAX_PATH ) )//获取程序运行路径+名称
	{
		return false;
	}
	strcpy(Cur_path,szPath);
	(strrchr(Cur_path, '\\'))[0] = 0;
	sprintf(Param_file,"%s\\ParamConfig.ini",Cur_path);
	FILE *param_fp = fopen(Param_file,"wb+");
	int magic=MAGIC_INVALID;
	fwrite(&magic,sizeof(magic),1,param_fp);
	fwrite(&g_stGlobal_Flash,sizeof(g_stGlobal_Flash),1,param_fp);
	rewind(param_fp);
	magic=MAGIC_VALID;
	fwrite(&magic,sizeof(magic),1,param_fp); 
	fclose(param_fp);
	
	sprintf(Param_file,"%s\\ParamConfig2.ini",Cur_path);
	FILE *param_fp2 = fopen(Param_file,"wb+");
	magic=MAGIC_INVALID;
	fwrite(&magic,sizeof(magic),1,param_fp2);
	fwrite(&g_stGlobal_Flash,sizeof(g_stGlobal_Flash),1,param_fp2);
	rewind(param_fp2);
	magic=MAGIC_VALID;
	fwrite(&magic,sizeof(magic),1,param_fp2); 
	fclose(param_fp);
	return true;
}

bool FlashParam_UpLoad()
{
	//暂时将默认值赋值给全局变量中
	g_stGlobal_Variable.globalflash = g_stGlobal_Flash;
	TCHAR szPath[MAX_PATH];
	char Cur_path[MAX_PATH],Param_file[MAX_PATH];
    if( !GetModuleFileName( NULL, szPath, MAX_PATH ) )//获取程序运行路径+名称
    {
        return false;
    }
	strcpy(Cur_path,szPath);
	(strrchr(Cur_path, '\\'))[0] = 0;
	sprintf(Param_file,"%s\\ParamConfig.ini",Cur_path);
	int magic = MAGIC_INVALID;
	FILE* param_fp = fopen(Param_file,"rb");
	if (NULL == param_fp)//打开失败采用默认设置
	{
		return true;
	}
	fread(&magic,sizeof(magic),1,param_fp);
	if (MAGIC_VALID != magic)
	{
		//第一个文件无效 读取第二个文件
		fclose(param_fp);
		magic = MAGIC_INVALID;
		sprintf(Param_file,"%s\\ParamConfig2.ini",Cur_path);
		FILE* param_fp = fopen(Param_file,"rb");
		if (NULL == param_fp)//打开失败采用默认设置
		{
			return false;
		}
		fread(&magic,sizeof(magic),1,param_fp);
		if (MAGIC_VALID != magic)
		{
			return false;//第二个文件无效
		}
	}
	fread(&g_stGlobal_Flash,sizeof(g_stGlobal_Flash),1,param_fp);
	fclose(param_fp);
	//需要更新当前的存储信息
	size_t szAllDriveStrings = GetLogicalDriveStrings(0,NULL);//获取本地磁盘 盘符长度
	char *pDriveStrings = new char[szAllDriveStrings + sizeof(_T(""))];
	GetLogicalDriveStrings(szAllDriveStrings,pDriveStrings);
	size_t szDriveString = strlen(pDriveStrings);
	int iMaxnumDrive = sizeof(g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc)/sizeof(g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[0]);
	int DriveIndex = 0;
	for(DriveIndex = 0; DriveIndex < iMaxnumDrive && szDriveString > 0; DriveIndex++)
	{
		//获取盘符名称信息
		strcpy((char *)g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[DriveIndex].disk_name,pDriveStrings);
		//获取磁盘空间信息
		ULARGE_INTEGER FreeAv,TotalBytes,FreeBytes;
		if (GetDiskFreeSpaceEx(pDriveStrings,&FreeAv,&TotalBytes,&FreeBytes))
		{
			g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[DriveIndex].realsize = TotalBytes.QuadPart/(ULONGLONG)(1024*1024);
			g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[DriveIndex].emptysize = FreeAv.QuadPart/(ULONGLONG)(1024*1024);
		}
		g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[DriveIndex].drive_type = (DRIVE_TYPE)GetDriveType(pDriveStrings);
		char drive_fs_type[128];
		if(!GetVolumeInformation(pDriveStrings,NULL,0,NULL,NULL,NULL,drive_fs_type,128))
		{
			g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[DriveIndex].drive_fstype = DRIVE_FS_TYPE_UNKNOWN;
		}
		else
		{
			if(!strcmp(drive_fs_type,"NTFS"))
			{
				g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[DriveIndex].drive_fstype = DRIVE_FS_TYPE_NTFS;
			}
			else if(!strcmp(drive_fs_type,"FAT32"))
			{
				g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[DriveIndex].drive_fstype = DRIVE_FS_TYPE_FAT32;
			}
			else
			{
				g_stGlobal_Flash.sysstoragepara.storageconfig.diskdesc[DriveIndex].drive_fstype = DRIVE_FS_TYPE_UNKNOWN;
			}
		}
		pDriveStrings += szDriveString + 1;
		szDriveString = strlen(pDriveStrings);
	}
	g_stGlobal_Flash.sysstoragepara.storageconfig.diskcount = DriveIndex;

	//暂时将默认值赋值给全局变量中
	g_stGlobal_Variable.globalflash = g_stGlobal_Flash;
	return true;
}

bool FlashParam_SetPtzPara(PTZ_PARA &para)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.ptzpara = para;
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetVideoPara(VIDEO_PARA &para)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.videopara = para;
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetAlgPara(ALG_PARA &para)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.algpara = para;
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetAlgParaConf(ALG_PARA_CONF_UINT *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.algpara.AlgParaConfUint[num - CMD_NUM_ALG_CONF_00_SET] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetAlgQuadPara(ALG_QUAD_PARA *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.algquadpara[(num - CMD_NUM_ALG_QUAD_000003_SET)] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetAlgAreaPara(ALG_AREA_PARA *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.algareapara[(num - CMD_NUM_ALG_AREA_000003_SET)] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetAlgLinePara(ALG_LINE_PARA *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.alglinepara[(num - CMD_NUM_ALG_LINE_000003_SET)] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetAlgCountPara(ALG_COUNT_PARA *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.algcountpara[(num - CMD_NUM_ALG_COUNT_000003_SET)] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetAlgEnterPara(ALG_ENTER_PARA *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.algenterpara[(num - CMD_NUM_ALG_ENTER_000003_SET)] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetAlgOutPara(ALG_OUT_PARA *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.algoutpara[(num - CMD_NUM_ALG_OUT_000003_SET)] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

// bool FlashParam_SetAlgStopPara(ALG_STOP_PARA *para,UINT num)
// {
// 	EnterCriticalSection(&g_csGlobal_Variable_Section);
// 	g_stGlobal_Variable.globalflash.algstoppara[(num - CMD_NUM_ALG_STOP_000002_SET)] = para[0];
// 	LeaveCriticalSection(&g_csGlobal_Variable_Section);
// 	return true;
// }
bool FlashParam_SetAlgFindbagPara(ALG_FINDBAG_PARA *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.algfindbagpara[(num - CMD_NUM_ALG_FINDBAG_000003_SET)] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetAlgLostPara(ALG_LOST_PARA *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.alglostpara[(num - CMD_NUM_ALG_LOST_000002_SET)] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}


bool FlashParam_SetAlgDirectPara(ALG_DIRECT_PARA *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.algdirectpara[(num - CMD_NUM_ALG_DIRECT_000003_SET)] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetAlgDetectPara(ALG_DETECT_PARA *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.algdetectpara[(num - CMD_NUM_ALG_DETECT_000003_SET)] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetAlgPeopleDensePara(ALG_PEOPLEDENSE_PARA *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.algpeopledectpara[(num - CMD_NUM_ALG_PEOPLEDENSE_000003_SET)] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetAlgClusterDetePara(ALG_CLUSTERDETE_PARA *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.algclusterdectpara[(num - CMD_NUM_ALG_CLUSTERDETE_000003_SET)] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetAlgMotionHistoryPara(ALG_MOTIONHISTORY_PARA *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.algmotionhistorypara[(num - CMD_NUM_ALG_MOTIONHISTORY_000003_SET)] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetAlgHoverDetePara(ALG_HOVERDETE_PARA *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.alghoverdectpara[(num - CMD_NUM_ALG_HOVERDETE_000003_SET)] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetAlgAlarmLinePara(ALG_ALARMLINE_PARA *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.algalarmlinepara[(num - CMD_NUM_ALG_ALARMLINE_000003_SET)] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetAlgRunningPara(ALG_RUNNINGOBJ_PARA *para,UINT num)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.algrunningpara[(num - CMD_NUM_ALG_RUNNING_000003_SET)] = para[0];
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetSysConfigPara(SYS_CONFIG_PARA &para)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.sysconfigpara = para;
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}

bool FlashParam_SetSysStoragePara(SYS_STORAGE_PARA &para)
{
	EnterCriticalSection(&g_csGlobal_Variable_Section);
	g_stGlobal_Variable.globalflash.sysstoragepara.camstorage = para.camstorage;
	LeaveCriticalSection(&g_csGlobal_Variable_Section);
	return true;
}


//获取、修改函数由g_stGlobal_Flash中相关元素定义 不支持删除操作

//参数管理对象实例
/*CParamMgr ParamMgrInstance;*/

//静态成员必须初始化
// HANDLE				CParamMgr::m_hMapFile = NULL;
// LPTSTR				CParamMgr::m_psMapAddr = NULL;
// CRITICAL_SECTION	CParamMgr::m_stSection;
// unsigned long int	CParamMgr::m_lMemSize = 1024*2;
// unsigned long int	CParamMgr::m_lFixedParamSize = 0;
// unsigned long int   CParamMgr::m_lCurOffset = 0;
// vector<ParamNode_T>	CParamMgr::m_vParam;
// 
// CParamMgr::CParamMgr()
// {
// 	m_lMemSize = 1024*2;
// 	Param_MemInit();
// }
// 
// CParamMgr::CParamMgr(long int MemSize)
// {
// 	m_lMemSize = (MemSize+1024-1)/1024*1024;
// 	Param_MemInit();
// }
// 
// CParamMgr::~CParamMgr()
// {
// 	Param_MemDestroy();
// }
// 
// bool CParamMgr::Param_MemDestroy()
// {
// 	if (m_hMapFile != NULL)
// 	{
// 		CloseHandle(m_hMapFile);
// 		m_hMapFile = NULL;
// 	}
// 	if (m_psMapAddr != NULL)
// 	{
// 		delete m_psMapAddr;
// 		m_psMapAddr = NULL;
// 	}
// 	return true;
// }
// 
// bool CParamMgr::Param_MemInit()
// {
// 	// 初始化临界区变量
//     InitializeCriticalSection(&m_stSection); 
// 	//固定长度初始化为0
// 	m_lFixedParamSize = 0;
// 	m_lCurOffset = 0;
// 	m_vParam.clear();
// 	// 初始化共享内存映射
//     if(m_hMapFile == NULL || m_psMapAddr == NULL)
//     {
// 		if (NULL == m_hMapFile)
// 		{
// 			m_hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, PARAM_CONFIG_SHARE_MEM);
// 		} 
// 		if(NULL != m_hMapFile)
// 		{
// 			//显示共享的文件数据。
// 			if (m_psMapAddr == NULL)
// 			{
// 				m_psMapAddr = (LPTSTR)MapViewOfFile(m_hMapFile, FILE_MAP_ALL_ACCESS,0,0,0);
// 			}
// 		}
// 		else
// 		{
// 			//创建共享文件。
// 			m_hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,m_lMemSize, PARAM_CONFIG_SHARE_MEM);
// 			if (m_hMapFile != NULL)
// 			{
// 				//拷贝数据到共享文件里。
// 				m_psMapAddr = (LPTSTR)MapViewOfFile(m_hMapFile,FILE_MAP_ALL_ACCESS, 0,0,0);
// 			}
// 			else
// 			{
// 				//创建共享内存失败
// 				return false;
// 			}
// 		}
//     }
// 	return true;
// }
// 
// bool CParamMgr::Param_Define(string name, string type, int len, int flag, void *value)
// {
// 	EnterCriticalSection(&m_stSection);
// 	if(!m_psMapAddr)
// 	{
// 		LeaveCriticalSection(&m_stSection);
// 		return false;
// 	}
// 	if(NULL == value)
// 	{
// 		memset((void *)(m_psMapAddr+m_lCurOffset),0x00,len);
// 	}
// 	else
// 	{
// 		memcpy((void *)(m_psMapAddr+m_lCurOffset),value,len);
// 	}
// 	LeaveCriticalSection(&m_stSection);
// 	ParamNode_T paramnode_t;
// 	paramnode_t.v_name = name;
// 	paramnode_t.v_type = type;
// 	paramnode_t.v_len = len;
// 	paramnode_t.v_offset = m_lCurOffset;
// 	m_vParam.push_back(paramnode_t);
// 
// 	if(flag)//固定参数 需要备份
// 	{
// 		m_lFixedParamSize += len;
// 	}
// 	m_lCurOffset += len;
// 	return true;
// }
// 
// bool CParamMgr::Param_Set(string name, void *value)
// {
// 	vector<ParamNode_T>::iterator itor;
// 	if(NULL == value)
// 	{
// 		return false;
// 	} 
// 	for(itor = m_vParam.begin(); itor != m_vParam.end(); itor++)
// 	{
// 		if(!name.compare(((ParamNode_T*)itor)->v_name))
// 		{
// 			EnterCriticalSection(&m_stSection);
// 			memcpy(m_psMapAddr+((ParamNode_T*)itor)->v_offset,value,((ParamNode_T*)itor)->v_len);
// 			LeaveCriticalSection(&m_stSection);
// 			break;
// 		}
// 	}
// 	if(itor == m_vParam.end())
// 	{
// 		return false;
// 	}
// 	return true;
// }
// 
// void* CParamMgr::Param_Addr_Get(string name)
// {
// 	void *pvt = NULL;
// 	vector<ParamNode_T>::iterator itor;
// 	for(itor = m_vParam.begin(); itor != m_vParam.end(); itor++)
// 	{
// 		if(!name.compare(((ParamNode_T*)itor)->v_name))
// 		{
// 			pvt = m_psMapAddr+((ParamNode_T*)itor)->v_offset;
// 			break;
// 		}
// 	}
// 	if(itor == m_vParam.end())
// 	{
// 		pvt = NULL;
// 	}
// 	return pvt;
// }
// 
// bool CParamMgr::Param_Get(string name, void *value)
// {
// 	vector<ParamNode_T>::iterator itor;
// 	if (NULL == value)
// 	{
// 		return false;
// 	}
// 	for(itor = m_vParam.begin(); itor != m_vParam.end(); itor++)
// 	{
// 		if(!name.compare(((ParamNode_T*)itor)->v_name))
// 		{
// 			memcpy(value,m_psMapAddr+((ParamNode_T*)itor)->v_offset,((ParamNode_T*)itor)->v_len);
// 			break;
// 		}
// 	}
// 	if(itor == m_vParam.end())
// 	{
// 		value=NULL;
// 		return false;
// 	}
// 	return true;
// }
// 
// bool CParamMgr::Param_DownLoad()
// {
// 	if (m_hMapFile == NULL || m_psMapAddr == NULL)//映射区错误
// 	{
// 		return false;
// 	}
// 	TCHAR szPath[MAX_PATH];
// 	char Cur_path[MAX_PATH],Param_file[MAX_PATH];
//   if( !GetModuleFileName( NULL, szPath, MAX_PATH ) )//获取程序运行路径+名称
//   {
//      return false;
//   }
// 	strcpy(Cur_path,szPath);
// 	(strrchr(Cur_path, '\\'))[0] = 0;
// 	sprintf(Param_file,"%s\\ParamConfig.ini",Cur_path);
// 	FILE *param_fp = fopen(Param_file,"wb+");
// 	int magic=MAGIC_INVALID;
// 	fwrite(&magic,sizeof(magic),1,param_fp);
// 	fwrite(&m_lFixedParamSize,sizeof(m_lFixedParamSize),1,param_fp);	
// 	fwrite(m_psMapAddr,m_lFixedParamSize,1,param_fp);
// 	rewind(param_fp);
// 	magic=MAGIC_VALID;
// 	fwrite(&magic,sizeof(magic),1,param_fp); 
// 	fclose(param_fp);
// 
// 	sprintf(Param_file,"%s\\ParamConfig2.ini",Cur_path);
// 	FILE *param_fp2 = fopen(Param_file,"wb+");
// 	magic=MAGIC_INVALID;
// 	fwrite(&magic,sizeof(magic),1,param_fp2);
// 	fwrite(&m_lFixedParamSize,sizeof(m_lFixedParamSize),1,param_fp);
// 	fwrite(m_psMapAddr,m_lFixedParamSize,1,param_fp2);
// 	rewind(param_fp2);
// 	magic=MAGIC_VALID;
// 	fwrite(&magic,sizeof(magic),1,param_fp2); 
// 	fclose(param_fp);
// 	return true;
// }
// bool CParamMgr::Param_UpLoad()
// {
// 	if (m_hMapFile == NULL || m_psMapAddr == NULL)//映射区错误
// 	{
// 		return false;
// 	}
// 	TCHAR szPath[MAX_PATH];
// 	char Cur_path[MAX_PATH],Param_file[MAX_PATH];
//     if( !GetModuleFileName( NULL, szPath, MAX_PATH ) )//获取程序运行路径+名称
//     {
//         return false;
//     }
// 	strcpy(Cur_path,szPath);
// 	(strrchr(Cur_path, '\\'))[0] = 0;
// 	sprintf(Param_file,"%s\\ParamConfig.ini",Cur_path);
// 	int magic = MAGIC_INVALID;
// 	long int lFixedSize = 0;
// 	FILE* param_fp = fopen(Param_file,"rb");
// 	if (NULL == param_fp)//打开失败采用默认设置
// 	{
// 		return true;
// 	}
// 	fread(&magic,sizeof(magic),1,param_fp);
// 	fread(&lFixedSize,sizeof(lFixedSize),1,param_fp);
// 	if (MAGIC_VALID != magic || lFixedSize != m_lFixedParamSize)
// 	{
// 		//第一个文件无效 读取第二个文件
// 		fclose(param_fp);
// 		magic = MAGIC_INVALID;
// 		sprintf(Param_file,"%s\\ParamConfig2.ini",Cur_path);
// 		FILE* param_fp = fopen(Param_file,"rb");
// 		if (NULL == param_fp)//打开失败采用默认设置
// 		{
// 			return false;
// 		}
// 		fread(&magic,sizeof(magic),1,param_fp);
// 		fread(&lFixedSize,sizeof(lFixedSize),1,param_fp);
// 		if (MAGIC_VALID != magic || lFixedSize != m_lFixedParamSize)
// 		{
// 			return false;//第二个文件无效
// 		}
// 	}
// 	fread(m_psMapAddr,m_lFixedParamSize,1,param_fp);
// 	fclose(param_fp);
// 	return true;
// }

bool ParseINIFileToXML(unsigned int flag,string fname)
{
	GLOBAL_FLASH_INI_GROUP group;
	int i=0,j=0,k=0,m=0,n=0;
	GLOBAL_FLASH *pGlobal_Flash = NULL;
	TCHAR szPath[MAX_PATH];
	char Cur_path[MAX_PATH],Param_file[MAX_PATH],Xml_file[MAX_PATH];
    if( !GetModuleFileName( NULL, szPath, MAX_PATH ) )//获取程序运行路径+名称
    {
        return FALSE;
    }
	strcpy(Cur_path,szPath);
	(strrchr(Cur_path, '\\'))[0] = 0;
	sprintf(Param_file,"%s\\ParamConfig.ini",Cur_path);
	int magic = MAGIC_INVALID;
	FILE* param_fp = fopen(Param_file,"rb");
	if (NULL == param_fp)//打开失败采用默认设置
	{
		return FALSE;
	}
	fread(&magic,sizeof(magic),1,param_fp);
	if (MAGIC_VALID != magic)
	{
		//第一个文件无效 读取第二个文件
		fclose(param_fp);
		magic = MAGIC_INVALID;
		sprintf(Param_file,"%s\\ParamConfig2.ini",Cur_path);
		FILE* param_fp = fopen(Param_file,"rb");
		if (NULL == param_fp)//打开失败采用默认设置
		{
			return FALSE;
		}
		fread(&magic,sizeof(magic),1,param_fp);
		if (MAGIC_VALID != magic)
		{
			return FALSE;//第二个文件无效
		}
	}
	pGlobal_Flash = new GLOBAL_FLASH();
	if (!pGlobal_Flash)
	{
		return FALSE;
	}
	fread(pGlobal_Flash,sizeof(GLOBAL_FLASH),1,param_fp);
	fclose(param_fp);

	if (!fname.empty())
	{
		sprintf(Xml_file,"%s\\%s",Cur_path,fname.c_str());
	}
	else
	{
		sprintf(Xml_file,"%s\\ParamConfig.xml",Cur_path);
	}
	//WRITE_LOG(Xml_file,sizeof(Xml_file),LOG_TYPE_WARNING,LOG_LEVEL_RUN,true);
	CMarkup xml;
	if (!xml.Save(Xml_file))//如果给定的文件夹不存在就直接返回
	{
		return FALSE;
	}
	xml.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");//添加XML头
	xml.AddElem("Identifier-FlashParam-Information");
	group = *(GLOBAL_FLASH_INI_GROUP *)&flag;
	xml.AddChildElem("Setting-Show");
	xml.IntoElem();
	if (group.bPtzPara)
	{
		xml.AddChildElem("PTZ-Param-Setting");
		xml.IntoElem();
		
		xml.AddChildElem("COM",(int)pGlobal_Flash->ptzpara.commport);
		xml.AddChildElem("BAUD",(int)pGlobal_Flash->ptzpara.combaud);
		xml.AddChildElem("ADDR",(int)pGlobal_Flash->ptzpara.addr);
		xml.AddChildElem("SPEED",(int)pGlobal_Flash->ptzpara.speed);
		xml.AddChildElem("PROTOCAL",(int)pGlobal_Flash->ptzpara.protocol);
		xml.AddChildElem("INFO",
			"\r\n--COM% <1-5>\r\n--BAUD(1200*(1<<(%-1)) <1-8>\r\n--ADDR% <0-15>\r\n--SPEED% <0-63>\r\n--PROTOCAL% <1-3:1_P2_D3_YAAN>\r\n");
		xml.OutOfElem();
	}
	if (group.bVideoPara)
	{
		xml.AddElem("VIDEO-Param-Setting");
		xml.IntoElem();
		xml.AddElem("SRC",(int)pGlobal_Flash->videopara.src);
		xml.AddElem("ENCMODE",(int)pGlobal_Flash->videopara.mod);
		xml.AddElem("ANALOGINFO","valid when SRC==ANALOG");
		xml.AddChildElem("VBIT",(int)pGlobal_Flash->videopara.analoginfo.vbittype);
		xml.AddChildElem("VFRAME",(int)pGlobal_Flash->videopara.analoginfo.vframetype);
		xml.AddElem("DIGITALINFO","valid when SRC==DIGIT");
		xml.AddChildElem("DVRDVS",(int)pGlobal_Flash->videopara.digitalinfo.digital_dvrdvs);
		xml.AddChildElem("ALARM",(int)pGlobal_Flash->videopara.digitalinfo.digital_alarm);
		xml.AddChildElem("DEVIP",CString((char *)pGlobal_Flash->videopara.digitalinfo.ipaddr));
		xml.AddElem("BRIGHT",(int)pGlobal_Flash->videopara.bright);
		xml.AddElem("CONSTRAST",(int)pGlobal_Flash->videopara.contrast);
		xml.AddElem("SATURATION",(int)pGlobal_Flash->videopara.saturation);
		xml.AddElem("HUE",(int)pGlobal_Flash->videopara.hue);
		xml.AddElem("INFO",
			"\r\n--SRC% <1-4:1ANALOG_2DIGITV_3DIGITA_4SENSOR>\r\n--ENCMODE% <1-3:1JPEG_2H264_3MJPEG>\r\n--VBIT% <1-3:1RGB24_2YV12_3YVY2>\r\n--VFRAME% <1-3:1QCIF_2CIF_3D1>\r\n");
		xml.OutOfElem();
	}
	if (group.bAlgPara)
	{
		xml.AddElem("ALG-Param-Setting");
		xml.IntoElem();
		xml.AddElem("CONFINDEXUSED",(int)pGlobal_Flash->algpara.AlgParaConfUsedIndex);
		for (i=0;i<sizeof(pGlobal_Flash->algpara.AlgParaConfUint)/sizeof(pGlobal_Flash->algpara.AlgParaConfUint[0]);i++)
		{
			CString cstrConfIndexNo;
			cstrConfIndexNo.Format("CONFINDEX%d",i);
			xml.AddElem(cstrConfIndexNo);
			xml.AddChildElem("GROUP",*(unsigned int*)&pGlobal_Flash->algpara.AlgParaConfUint[i].AlgParaChooseGroup);
			xml.IntoElem();
			xml.AddElem("RUNTIME");
			xml.AddChildElem("START",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgScheduleUint.usAlgRuntimeStart);
			xml.AddChildElem("END",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgScheduleUint.usAlgRuntimeEnd);
			xml.AddElem("SVPARA");
			xml.AddChildElem("AREALINE");
			xml.IntoElem();
			xml.AddChildElem("IMGW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.imgw);
			xml.AddChildElem("IMGH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.imgh);
			xml.AddChildElem("INIFRAME",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.iniframe);
			xml.AddChildElem("THRESH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.thresh);
			xml.AddChildElem("DISTHRESH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.disthresh);
			xml.AddChildElem("MAXTRACKSIZE",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.max_tracksize);
			xml.AddChildElem("LOSTCOUNT",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.lostcount);
			xml.AddChildElem("MINFIND",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.minfind);
			xml.AddChildElem("MINMOVE",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.minmove);
			xml.AddChildElem("TRACKPOSDIFFTHRESH",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.track_pos_diff_thresh);
			xml.AddChildElem("TRACKRECTDIFFTHRESH",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.track_rect_diff_thresh);
			xml.AddChildElem("CONTMINW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.cont_minw);
			xml.AddChildElem("CONTMAXW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.cont_maxw);
			xml.AddChildElem("CONTMINH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.cont_minh);
			xml.AddChildElem("CONTMAXH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.cont_maxh);
			xml.AddChildElem("CONTAREARATE",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.cont_area_rate);
			xml.AddChildElem("RECTMERGEFLAG",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.rect_merge_flag);
			xml.AddChildElem("RECTMINW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.rect_minw);
			xml.AddChildElem("RECTMINH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.rect_minh);
			xml.AddChildElem("RECT_AREA_RATE",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAreaLineParamEnv.rect_area_rate);

			xml.AddElem("AREA");
			xml.AddChildElem("IMGW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAlarmAreaEnv._Imgw);
			xml.AddChildElem("IMGH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAlarmAreaEnv._Imgh);
			xml.AddChildElem("MHIDURATION",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAlarmAreaEnv._MHI_Duration);
			xml.AddChildElem("RATIONOISZE",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAlarmAreaEnv._RatioNoisze);
			xml.AddChildElem("RATIODetect",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAlarmAreaEnv._RatioDetect);
			xml.AddChildElem("DIFFTHRESHOLD",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAlarmAreaEnv._Diff_Threshold);
			
			xml.AddElem("LINE");
			xml.AddChildElem("IMGW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAlarmLineEnv._Imgw);
			xml.AddChildElem("IMGH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAlarmLineEnv._Imgh);
			xml.AddChildElem("MHIDURATION",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAlarmLineEnv._MHI_Duration);
			xml.AddChildElem("RATIONOISZE",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAlarmLineEnv._RatioNoisze);
			xml.AddChildElem("RATIODetect",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAlarmLineEnv._ObjWidth);
			xml.AddChildElem("DIFFTHRESHOLD",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAlarmLineEnv._Diff_Threshold);
			
			xml.AddElem("WINDOWN");
			xml.AddChildElem("IMGW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.imgw);
			xml.AddChildElem("IMGH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.imgh);
			xml.AddChildElem("INIFRAME",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.iniframe);
			xml.AddChildElem("GRAYTHRESH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.grayThresh);
			xml.AddChildElem("LIGHTAREARATIO",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.light_area_ratio);
			xml.AddChildElem("DISTHRESH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.disthresh);
			xml.AddChildElem("MAXTRACKSIZE",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.max_tracksize);
			xml.AddChildElem("LOSTCOUNT",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.lostcount);
			xml.AddChildElem("MINFIND",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.minfind);
			xml.AddChildElem("MINMOVE",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.minmove);
			xml.AddChildElem("TRACKPOSDIFFTHRESH",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.track_pos_diff_thresh);
			xml.AddChildElem("TRACKRECTDIFFTHRESH",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.track_rect_diff_thresh);
			xml.AddChildElem("CONTMINW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.cont_minw);
			xml.AddChildElem("CONTMAXW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.cont_maxw);
			xml.AddChildElem("CONTMINH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.cont_minh);
			xml.AddChildElem("CONTMAXH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.cont_maxh);
			xml.AddChildElem("CONTAREARATE",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.cont_area_rate);
			xml.AddChildElem("RECTMERGEFLAG",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.rect_merge_flag);
			xml.AddChildElem("RECTMINW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.rect_minw);
			xml.AddChildElem("RECTMINH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.rect_minh);
			xml.AddChildElem("RECTAREARATE",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgXianWindowParamEnv.rect_area_rate);

			xml.AddElem("LEAVEBAG");
			xml.AddChildElem("IMGW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFindBagEnv._imgw);
			xml.AddChildElem("IMGH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFindBagEnv._imgh);
			xml.AddChildElem("INIFRAME",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFindBagEnv._iniframe);
			xml.AddChildElem("THRESH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFindBagEnv._thresh);
			xml.AddChildElem("CONTMINW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFindBagEnv._cont_minw);
			xml.AddChildElem("CONTMAXW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFindBagEnv._cont_maxw);
			xml.AddChildElem("CONTMINH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFindBagEnv._cont_minh);
			xml.AddChildElem("CONTMAXH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFindBagEnv._cont_maxh);
			
			xml.AddElem("FIGHT");
			xml.AddChildElem("ISMOVECOUNTTHRESH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFightDetectionParamEnv._IsMovingCountThresh);
			xml.AddChildElem("UNIDIRECTIONCOUNTTHRESH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFightDetectionParamEnv._UniDirectionCountThresh);
			xml.AddChildElem("UNIDIRECTIONRATIO",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFightDetectionParamEnv._UnidirectionRatio);
			xml.AddChildElem("IMAGEWIDTH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFightDetectionParamEnv._ImageWidth);
			xml.AddChildElem("IMAGEHEIGHT",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFightDetectionParamEnv._ImageHeight);
			xml.AddChildElem("HYPOTENUSEMIN",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFightDetectionParamEnv._HypotenuseMin);
			xml.AddChildElem("HYPOTENUSEMAX",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFightDetectionParamEnv._HypotenuseMax);
			xml.AddChildElem("ISMOVINGTHRESH",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFightDetectionParamEnv._IsMovingCountThresh);
			xml.AddChildElem("ROIBLOCKTHRESH",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFightDetectionParamEnv._ROIBlockThresh);
			xml.AddChildElem("ROIBLOCKCOUNTMAX",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFightDetectionParamEnv._ROIBlockCountMax);
			xml.AddChildElem("ROIBLOCKCOUNTMIN",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgFightDetectionParamEnv._ROIBlockCountMin);

			xml.AddElem("CLUSTER");
			xml.AddChildElem("CLUSTERMOVINGCOUNTTHRESH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgClusterDetectionParamEnv._clustermovingCountThresh);
			xml.AddChildElem("IMAGEWIDTH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgClusterDetectionParamEnv._ImageWidth);
			xml.AddChildElem("IMAGEHEIGHT",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgClusterDetectionParamEnv._ImageHeight);
			xml.AddChildElem("HYPOTENUSEMIN",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgClusterDetectionParamEnv._HypotenuseMin);
			xml.AddChildElem("HYPOTENUSEMAX",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgClusterDetectionParamEnv._HypotenuseMax);
			xml.AddChildElem("ROIBLOCKTHRESH",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgClusterDetectionParamEnv._ROIBlockThresh);
			xml.AddChildElem("ROIBLOCKCOUNTMINWIDTH",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgClusterDetectionParamEnv._ROIBlockCountMinWidth);
			xml.AddChildElem("ROIBLOCKCOUNTMINHEIGHT",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgClusterDetectionParamEnv._ROIBlockCountMinHeight);

			xml.AddElem("PEOPLEDENSE");
			xml.AddChildElem("IMGW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgPeoPleDense2ParamEnv._imgw);
			xml.AddChildElem("IMGH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgPeoPleDense2ParamEnv._imgh);
			xml.AddChildElem("NLG",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgPeoPleDense2ParamEnv._nLg);
			xml.AddChildElem("NLM",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgPeoPleDense2ParamEnv._nLm);
			xml.AddChildElem("NNM",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgPeoPleDense2ParamEnv._nNm);
			xml.AddChildElem("NNS",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgPeoPleDense2ParamEnv._nNs);
			xml.AddChildElem("NSPNF",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgPeoPleDense2ParamEnv._nSPNF);
			xml.AddChildElem("FSIGMA0",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgPeoPleDense2ParamEnv._fSigma0);
			xml.AddChildElem("FSIGMA1",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgPeoPleDense2ParamEnv._fSigma1);
			xml.AddChildElem("NT0",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgPeoPleDense2ParamEnv._nT0);
			xml.AddChildElem("NOCCHIGHESTWARNINGTHR",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgPeoPleDense2ParamEnv._nOccHighestWarningThr);
			xml.AddChildElem("BVANISHPT",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgPeoPleDense2ParamEnv._bVanishPt);
	
			xml.AddChildElem("RUNING");
			xml.IntoElem();
			xml.AddChildElem("IMGW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._imgw);
			xml.AddChildElem("IMGH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._imgh);
			xml.AddChildElem("INIFRAME",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._iniframe);
			xml.AddChildElem("THRESH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._thresh);
			xml.AddChildElem("DISTHRESH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._disthresh);
			xml.AddChildElem("MAXTRACKSIZE",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._max_tracksize);
			xml.AddChildElem("LOSTCOUNT",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._lostcount);
			xml.AddChildElem("MINFIND",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._minfind);
			xml.AddChildElem("MINMOVE",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._minmove);
			xml.AddChildElem("TRACKPOSDIFFTHRESH",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._track_pos_diff_thresh);
			xml.AddChildElem("TRACKRECTDIFFTHRESH",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._track_rect_diff_thresh);
			xml.AddChildElem("CONTMINW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._cont_minw);
			xml.AddChildElem("CONTMAXW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._cont_maxw);
			xml.AddChildElem("CONTMINH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._cont_minh);
			xml.AddChildElem("CONTMAXH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._cont_maxh);
			xml.AddChildElem("CONTAREARATE",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._cont_area_rate);
			xml.AddChildElem("RECTMERGEFLAG",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._rect_merge_flag);
			xml.AddChildElem("RECTMINW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._rect_minw);
			xml.AddChildElem("RECTMINH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._rect_minh);
			xml.AddChildElem("RECT_AREA_RATE",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgRunningDetectEnv._rect_area_rate);

			xml.AddElem("ABNORMALFALL");
			xml.AddChildElem("ISMOVECOUNTTHRESH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAbnormalFallEnv._IsMovingCountThresh);
			xml.AddChildElem("UNIDIRECTIONCOUNTTHRESH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAbnormalFallEnv._UniDirectionCountThresh);
			xml.AddChildElem("UNIDIRECTIONRATIO",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAbnormalFallEnv._UnidirectionRatio);
			xml.AddChildElem("IMAGEWIDTH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAbnormalFallEnv._ImageWidth);
			xml.AddChildElem("IMAGEHEIGHT",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAbnormalFallEnv._ImageHeight);
			xml.AddChildElem("HYPOTENUSEMIN",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAbnormalFallEnv._HypotenuseMin);
			xml.AddChildElem("HYPOTENUSEMAX",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAbnormalFallEnv._HypotenuseMax);
			xml.AddChildElem("ISMOVINGTHRESH",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAbnormalFallEnv._IsMovingCountThresh);
			xml.AddChildElem("ROIBLOCKTHRESH",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAbnormalFallEnv._ROIBlockThresh);
			xml.AddChildElem("ROIBLOCKCOUNTMAX",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAbnormalFallEnv._ROIBlockCountMax);
			xml.AddChildElem("ROIBLOCKCOUNTMIN",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgAbnormalFallEnv._ROIBlockCountMin);

			xml.AddChildElem("HOVER");
			xml.AddChildElem("IMGW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._imgw);
			xml.AddChildElem("IMGH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._imgh);
			xml.AddChildElem("INIFRAME",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._iniframe);
			xml.AddChildElem("THRESH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._thresh);
			xml.AddChildElem("TIMEHRESH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._timeThresh);
			xml.AddChildElem("DISTHRESH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._disthresh);
			xml.AddChildElem("MAXTRACKSIZE",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._max_tracksize);
			xml.AddChildElem("LOSTCOUNT",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._lostcount);
			xml.AddChildElem("MINFIND",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._minfind);
			xml.AddChildElem("MINMOVE",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._minmove);
			xml.AddChildElem("TRACKPOSDIFFTHRESH",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._track_pos_diff_thresh);
			xml.AddChildElem("TRACKRECTDIFFTHRESH",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._track_rect_diff_thresh);
			xml.AddChildElem("CONTMINW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._cont_minw);
			xml.AddChildElem("CONTMAXW",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._cont_maxw);
			xml.AddChildElem("CONTMINH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._cont_minh);
			xml.AddChildElem("CONTMAXH",(int)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._cont_maxh);
			xml.AddChildElem("CONTAREARATE",(float)pGlobal_Flash->algpara.AlgParaConfUint[i].AlgConfSvPara.AlgHoverDetectionEnv._cont_area_rate);

			//xml.AddElem("");
			xml.OutOfElem();
			xml.OutOfElem();
			xml.OutOfElem();
		}
		xml.OutOfElem();

	}

	if (group.bAlgQuadPara)
	{
		xml.AddElem("QUAD-Param-Setting");
		xml.IntoElem();
		for (i = 0;i < sizeof(pGlobal_Flash->algquadpara)/sizeof(pGlobal_Flash->algquadpara[0]);i++)
		{
			for (j = 0;j < sizeof(pGlobal_Flash->algquadpara[0])/sizeof(UNIT_REC);j++)
			{
				CString cstrQuadIndexNo;
				cstrQuadIndexNo.Format("QUAD%d",i*sizeof(pGlobal_Flash->algquadpara[0])/sizeof(UNIT_REC)+j);
				xml.AddElem(cstrQuadIndexNo);			
				xml.AddChildElem("VALID",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].valid);
				xml.AddChildElem("WIDTH",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].width);
				xml.AddChildElem("X1START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].x1_start);
				xml.AddChildElem("Y1START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].y1_start);
				xml.AddChildElem("X2START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].x2_start);
				xml.AddChildElem("Y2START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].y2_start);
				xml.AddChildElem("X3START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].x3_start);
				xml.AddChildElem("Y3START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].y3_start);
				xml.AddChildElem("X4START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].x4_start);
				xml.AddChildElem("Y4START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].y4_start);
				xml.AddChildElem("X5START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].x5_start);
				xml.AddChildElem("Y5START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].y5_start);
				xml.AddChildElem("X6START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].x6_start);
				xml.AddChildElem("Y6START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].y6_start);
				xml.AddChildElem("X7START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].x7_start);
				xml.AddChildElem("Y7START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].y7_start);
				xml.AddChildElem("X8START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].x8_start);
				xml.AddChildElem("Y8START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].y8_start);
				xml.AddChildElem("X9START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].x9_start);
				xml.AddChildElem("Y9START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].y9_start);
				xml.AddChildElem("X10START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].x10_start);
				xml.AddChildElem("Y10START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].y10_start);
				xml.AddChildElem("X11START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].x11_start);
				xml.AddChildElem("Y11START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].y11_start);
				xml.AddChildElem("X12START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].x12_start);
				xml.AddChildElem("Y12START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].y12_start);
				xml.AddChildElem("X13START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].x13_start);
				xml.AddChildElem("Y13START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].y13_start);
				xml.AddChildElem("X14START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].x14_start);
				xml.AddChildElem("Y14START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].y14_start);
				xml.AddChildElem("X15START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].x15_start);
				xml.AddChildElem("Y15START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].y15_start);
				xml.AddChildElem("X16START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].x16_start);
				xml.AddChildElem("Y16START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].y16_start);
				xml.AddChildElem("COLOR",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].color);
				xml.AddChildElem("LOOPTYPE",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].looptype);
				//xml.Add
				xml.AddChildElem("SCHEDULE");
				xml.IntoElem();
				for( m = 0;m < 7;m++)
				{
					for (n = 0;n < 8;n++)
					{
						CString cstrScheduleInfo;
						cstrScheduleInfo.Format("WEEK%d-%dTH",m,n);
						xml.AddChildElem(cstrScheduleInfo);
						xml.IntoElem();
						xml.AddChildElem("START",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].scheduleunit[m][n].timestart);
						xml.AddChildElem("END",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].scheduleunit[m][n].timeend);
						xml.OutOfElem();				
					}
				}
				xml.OutOfElem();
				xml.AddChildElem("PRESET",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].preset);
				xml.AddChildElem("PRETIME",(unsigned short)pGlobal_Flash->algquadpara[i].REC[j].pretime);
			}
		}
		xml.OutOfElem();
	}

	if (group.bAlgAreaPara)
	{
		xml.AddElem("AREA-Param-Setting");
		xml.IntoElem();
		for (i = 0;i < sizeof(pGlobal_Flash->algareapara)/sizeof(pGlobal_Flash->algareapara[0]);i++)
		{
			for (j = 0;j < sizeof(pGlobal_Flash->algareapara[0])/sizeof(CONFIG_UNIT);j++)
			{
				CString cstrAreaIndexNo;
				cstrAreaIndexNo.Format("AREA%d",i*sizeof(pGlobal_Flash->algareapara[0])/sizeof(CONFIG_UNIT)+j);
				xml.AddElem(cstrAreaIndexNo);
				xml.AddChildElem("POINT");
				for (k = 0;k< 8;k++)
				{
					xml.IntoElem();
					xml.AddChildElem("VALID",(unsigned short)pGlobal_Flash->algareapara[i].configunit[j].config_unit[k].valid);
					xml.AddChildElem("X",(unsigned short)pGlobal_Flash->algareapara[i].configunit[j].config_unit[k].x);
					xml.AddChildElem("Y",(unsigned short)pGlobal_Flash->algareapara[i].configunit[j].config_unit[k].y);
					xml.OutOfElem();
				}
				xml.AddChildElem("COLOR",(int)pGlobal_Flash->algareapara[i].configunit[j].color);
				//xml.Add
				xml.AddChildElem("SCHEDULE");
				xml.IntoElem();
				for(m = 0;m < 7;m++)
				{
					for (n = 0;n < 8;n++)
					{
						CString cstrScheduleInfo;
						cstrScheduleInfo.Format("WEEK%d-%dTH",m,n);
						xml.AddChildElem(cstrScheduleInfo);
						xml.IntoElem();
						xml.AddChildElem("START",(unsigned short)pGlobal_Flash->algareapara[i].configunit[j].scheduleunit[m][n].timestart);
						xml.AddChildElem("END",(unsigned short)pGlobal_Flash->algareapara[i].configunit[j].scheduleunit[m][n].timeend);				
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
				xml.AddChildElem("VALID",(int)pGlobal_Flash->algareapara[i].configunit[j].validflag);
				xml.AddChildElem("PTZSET",(int)pGlobal_Flash->algareapara[i].configunit[j].preset);
				xml.AddChildElem("PTZTIME",(int)pGlobal_Flash->algareapara[i].configunit[j].pretime);
			}
		}
		xml.OutOfElem();
	}

	if (group.bAlgLinePara)
	{
		xml.AddElem("LINE-Param-Setting");
		xml.IntoElem();
		for (i = 0;i < sizeof(pGlobal_Flash->alglinepara)/sizeof(pGlobal_Flash->alglinepara[0]);i++)
		{
			for (j = 0;j < sizeof(pGlobal_Flash->alglinepara[0])/sizeof(UNIT_LINE);j++)
			{
				CString cstrLineIndexNo;
				cstrLineIndexNo.Format("LINE%d",i*sizeof(pGlobal_Flash->alglinepara[0])/sizeof(UNIT_LINE)+j);
				xml.AddElem(cstrLineIndexNo);
				xml.AddChildElem("XSTART",(int)pGlobal_Flash->alglinepara[i].lineunit[j].x_start);
				xml.AddChildElem("XEND",(int)pGlobal_Flash->alglinepara[i].lineunit[j].x_end);
				xml.AddChildElem("YSTART",(int)pGlobal_Flash->alglinepara[i].lineunit[j].y_start);
				xml.AddChildElem("YEND",(int)pGlobal_Flash->alglinepara[i].lineunit[j].y_end);
				xml.AddChildElem("DIR",(int)pGlobal_Flash->alglinepara[i].lineunit[j].direction);
				xml.AddChildElem("COLOR",(int)pGlobal_Flash->alglinepara[i].lineunit[j].color);
				//xml.Add
				xml.AddChildElem("SCHEDULE");
				xml.IntoElem();
				for(m = 0;m < 7;m++)
				{
					for (n = 0;n < 8;n++)
					{
						CString cstrScheduleInfo;
						cstrScheduleInfo.Format("WEEK%d-%dTH",m,n);
						xml.AddChildElem(cstrScheduleInfo);
						xml.IntoElem();
						xml.AddChildElem("START",(unsigned short)pGlobal_Flash->alglinepara[i].lineunit[j].scheduleunit[m][n].timestart);
						xml.AddChildElem("END",(unsigned short)pGlobal_Flash->alglinepara[i].lineunit[j].scheduleunit[m][n].timeend);				
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
				xml.AddChildElem("VALID",(int)pGlobal_Flash->alglinepara[i].lineunit[j].validflag);
				xml.AddChildElem("PTZSET",(int)pGlobal_Flash->alglinepara[i].lineunit[j].preset);
				xml.AddChildElem("PTZTIME",(int)pGlobal_Flash->alglinepara[i].lineunit[j].pretime);
			}
		}
		xml.OutOfElem();
	}
	
	if (group.bAlgCountPara)
	{
		xml.AddElem("COUNT-Param-Setting");
		xml.IntoElem();
		for (i = 0;i < sizeof(pGlobal_Flash->algcountpara)/sizeof(pGlobal_Flash->algcountpara[0]);i++)
		{
			for (j = 0;j < sizeof(pGlobal_Flash->algcountpara[0])/sizeof(UNIT_COUNT);j++)
			{
				CString cstrCountIndexNo;
				cstrCountIndexNo.Format("COUNT%d",i*sizeof(pGlobal_Flash->algcountpara[0])/sizeof(UNIT_LINE)+j);
				xml.AddElem(cstrCountIndexNo);
				xml.AddChildElem("XSTART",(int)pGlobal_Flash->algcountpara[i].countunit[j].x_start);
				xml.AddChildElem("XEND",(int)pGlobal_Flash->algcountpara[i].countunit[j].x_end);
				xml.AddChildElem("YSTART",(int)pGlobal_Flash->algcountpara[i].countunit[j].y_start);
				xml.AddChildElem("YEND",(int)pGlobal_Flash->algcountpara[i].countunit[j].y_end);
				xml.AddChildElem("DIR",(int)pGlobal_Flash->algcountpara[i].countunit[j].direction);
				xml.AddChildElem("CNT",(int)pGlobal_Flash->algcountpara[i].countunit[j].count);
				xml.AddChildElem("COLOR",(int)pGlobal_Flash->algcountpara[i].countunit[j].color);
				//xml.Add
				xml.AddChildElem("SCHEDULE");
				xml.IntoElem();
				for(m = 0;m < 7;m++)
				{
					for (n = 0;n < 8;n++)
					{
						CString cstrScheduleInfo;
						cstrScheduleInfo.Format("WEEK%d-%dTH",m,n);
						xml.AddChildElem(cstrScheduleInfo);
						xml.IntoElem();
						xml.AddChildElem("START",(unsigned short)pGlobal_Flash->algcountpara[i].countunit[j].scheduleunit[m][n].timestart);
						xml.AddChildElem("END",(unsigned short)pGlobal_Flash->algcountpara[i].countunit[j].scheduleunit[m][n].timeend);				
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
				xml.AddChildElem("VALID",(int)pGlobal_Flash->algcountpara[i].countunit[j].validflag);
				xml.AddChildElem("PTZSET",(int)pGlobal_Flash->algcountpara[i].countunit[j].preset);
				xml.AddChildElem("PTZTIME",(int)pGlobal_Flash->algcountpara[i].countunit[j].pretime);
			}
		}
		xml.OutOfElem();
	}

	if(group.bAlgEnterPara)
	{
		xml.AddElem("ENTER-Param-Setting");
		xml.IntoElem();
		for (i = 0;i < sizeof(pGlobal_Flash->algenterpara)/sizeof(pGlobal_Flash->algenterpara[0]);i++)
		{
			for (j = 0;j < sizeof(pGlobal_Flash->algenterpara[0])/sizeof(UNIT_ENTER);j++)
			{
				CString cstrEnterIndexNo;
				cstrEnterIndexNo.Format("ENTER%d",i*sizeof(pGlobal_Flash->algenterpara[0])/sizeof(UNIT_ENTER)+j);
				xml.AddElem(cstrEnterIndexNo);
				xml.AddChildElem("XSTART",(int)pGlobal_Flash->algenterpara[i].enterunit[j].x_start);
				xml.AddChildElem("XEND",(int)pGlobal_Flash->algenterpara[i].enterunit[j].x_end);
				xml.AddChildElem("YSTART",(int)pGlobal_Flash->algenterpara[i].enterunit[j].y_start);
				xml.AddChildElem("YEND",(int)pGlobal_Flash->algenterpara[i].enterunit[j].y_end);
				xml.AddChildElem("DIR",(int)pGlobal_Flash->algenterpara[i].enterunit[j].direction);
				xml.AddChildElem("COLOR",(int)pGlobal_Flash->algenterpara[i].enterunit[j].color);
				//xml.Add
				xml.AddChildElem("SCHEDULE");
				xml.IntoElem();
				for(m = 0;m < 7;m++)
				{
					for (n = 0;n < 8;n++)
					{
						CString cstrScheduleInfo;
						cstrScheduleInfo.Format("WEEK%d-%dTH",m,n);
						xml.AddChildElem(cstrScheduleInfo);
						xml.IntoElem();
						xml.AddChildElem("START",(unsigned short)pGlobal_Flash->algenterpara[i].enterunit[j].scheduleunit[m][n].timestart);
						xml.AddChildElem("END",(unsigned short)pGlobal_Flash->algenterpara[i].enterunit[j].scheduleunit[m][n].timeend);				
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
				xml.AddChildElem("VALID",(int)pGlobal_Flash->algenterpara[i].enterunit[j].validflag);
				xml.AddChildElem("PTZSET",(int)pGlobal_Flash->algenterpara[i].enterunit[j].preset);
				xml.AddChildElem("PTZTIME",(int)pGlobal_Flash->algenterpara[i].enterunit[j].pretime);
			}
		}
		xml.OutOfElem();
	}

	if (group.bAlgOutPara)
	{
		xml.AddElem("OUT-Param-Setting");
		xml.IntoElem();
		for (i = 0;i < sizeof(pGlobal_Flash->algoutpara)/sizeof(pGlobal_Flash->algoutpara[0]);i++)
		{
			for (j = 0;j < sizeof(pGlobal_Flash->algoutpara[0])/sizeof(UNIT_OUT);j++)
			{
				CString cstrOutIndexNo;
				cstrOutIndexNo.Format("OUT%d",i*sizeof(pGlobal_Flash->algoutpara[0])/sizeof(UNIT_OUT)+j);
				xml.AddElem(cstrOutIndexNo);
				xml.AddChildElem("XSTART",(int)pGlobal_Flash->algoutpara[i].outunit[j].x_start);
				xml.AddChildElem("XEND",(int)pGlobal_Flash->algoutpara[i].outunit[j].x_end);
				xml.AddChildElem("YSTART",(int)pGlobal_Flash->algoutpara[i].outunit[j].y_start);
				xml.AddChildElem("YEND",(int)pGlobal_Flash->algoutpara[i].outunit[j].y_end);
				xml.AddChildElem("DIR",(int)pGlobal_Flash->algoutpara[i].outunit[j].direction);
				xml.AddChildElem("COLOR",(int)pGlobal_Flash->algoutpara[i].outunit[j].color);
				//xml.Add
				xml.AddChildElem("SCHEDULE");
				xml.IntoElem();
				for(m = 0;m < 7;m++)
				{
					for (n = 0;n < 8;n++)
					{
						CString cstrScheduleInfo;
						cstrScheduleInfo.Format("WEEK%d-%dTH",m,n);
						xml.AddChildElem(cstrScheduleInfo);
						xml.IntoElem();
						xml.AddChildElem("START",(unsigned short)pGlobal_Flash->algoutpara[i].outunit[j].scheduleunit[m][n].timestart);
						xml.AddChildElem("END",(unsigned short)pGlobal_Flash->algoutpara[i].outunit[j].scheduleunit[m][n].timeend);				
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
				xml.AddChildElem("VALID",(int)pGlobal_Flash->algoutpara[i].outunit[j].validflag);
				xml.AddChildElem("PTZSET",(int)pGlobal_Flash->algoutpara[i].outunit[j].preset);
				xml.AddChildElem("PTZTIME",(int)pGlobal_Flash->algoutpara[i].outunit[j].pretime);
			}
		}
		xml.OutOfElem();
	}

	if (group.bAlgFindBagPara)
	{
		xml.AddElem("FINDBAG-Param-Setting");
		xml.IntoElem();
		for (i = 0;i < sizeof(pGlobal_Flash->algfindbagpara)/sizeof(pGlobal_Flash->algfindbagpara[0]);i++)
		{
			for (j = 0;j < sizeof(pGlobal_Flash->algfindbagpara[0])/sizeof(CONFIG_STOP);j++)
			{
				CString cstrStopIndexNo;
				cstrStopIndexNo.Format("FINDBAG%d",i*sizeof(pGlobal_Flash->algfindbagpara[0])/sizeof(CONFIG_STOP)+j);
				xml.AddElem(cstrStopIndexNo);
				xml.AddChildElem("POINT");
				for (k = 0;k< 8;k++)
				{
					xml.IntoElem();
					xml.AddChildElem("VALID",(unsigned short)pGlobal_Flash->algfindbagpara[i].findBagUnit[j].config_unit[k].valid);
					xml.AddChildElem("X",(unsigned short)pGlobal_Flash->algfindbagpara[i].findBagUnit[j].config_unit[k].x);
					xml.AddChildElem("Y",(unsigned short)pGlobal_Flash->algfindbagpara[i].findBagUnit[j].config_unit[k].y);
					xml.OutOfElem();
				}
				xml.AddChildElem("COLOR",(int)pGlobal_Flash->algfindbagpara[i].findBagUnit[j].color);
				//xml.Add
				xml.AddChildElem("SCHEDULE");
				xml.IntoElem();
				for(m = 0;m < 7;m++)
				{
					for (n = 0;n < 8;n++)
					{
						CString cstrScheduleInfo;
						cstrScheduleInfo.Format("WEEK%d-%dTH",m,n);
						xml.AddChildElem(cstrScheduleInfo);
						xml.IntoElem();
						xml.AddChildElem("START",(unsigned short)pGlobal_Flash->algfindbagpara[i].findBagUnit[j].scheduleunit[m][n].timestart);
						xml.AddChildElem("END",(unsigned short)pGlobal_Flash->algfindbagpara[i].findBagUnit[j].scheduleunit[m][n].timeend);				
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
				xml.AddChildElem("VALID",(int)pGlobal_Flash->algfindbagpara[i].findBagUnit[j].validflag);
				xml.AddChildElem("PTZSET",(int)pGlobal_Flash->algfindbagpara[i].findBagUnit[j].preset);
				xml.AddChildElem("PTZTIME",(int)pGlobal_Flash->algfindbagpara[i].findBagUnit[j].pretime);
			}
		}
		xml.OutOfElem();
	}

	if (group.bAlgLostPara)
	{
		xml.AddElem("LOST-Param-Setting");
		xml.IntoElem();
		for (i = 0;i < sizeof(pGlobal_Flash->alglostpara)/sizeof(pGlobal_Flash->alglostpara[0]);i++)
		{
			for (j = 0;j < sizeof(pGlobal_Flash->alglostpara[0])/sizeof(CONFIG_LOST);j++)
			{
				CString cstrLostIndexNo;
				cstrLostIndexNo.Format("LOST%d",i*sizeof(pGlobal_Flash->alglostpara[0])/sizeof(CONFIG_LOST)+j);
				xml.AddElem(cstrLostIndexNo);
				xml.AddChildElem("POINT");
				for (k = 0;k< 8;k++)
				{
					xml.IntoElem();
					xml.AddChildElem("VALID",(unsigned short)pGlobal_Flash->alglostpara[i].lostunit[j].config_unit[k].valid);
					xml.AddChildElem("X",(unsigned short)pGlobal_Flash->alglostpara[i].lostunit[j].config_unit[k].x);
					xml.AddChildElem("Y",(unsigned short)pGlobal_Flash->alglostpara[i].lostunit[j].config_unit[k].y);
					xml.OutOfElem();
				}
				xml.AddChildElem("COLOR",(int)pGlobal_Flash->alglostpara[i].lostunit[j].color);
				//xml.Add
				xml.AddChildElem("SCHEDULE");
				xml.IntoElem();
				for(m = 0;m < 7;m++)
				{
					for (n = 0;n < 8;n++)
					{
						CString cstrScheduleInfo;
						cstrScheduleInfo.Format("WEEK%d-%dTH",m,n);
						xml.AddChildElem(cstrScheduleInfo);
						xml.IntoElem();
						xml.AddChildElem("START",(unsigned short)pGlobal_Flash->alglostpara[i].lostunit[j].scheduleunit[m][n].timestart);
						xml.AddChildElem("END",(unsigned short)pGlobal_Flash->alglostpara[i].lostunit[j].scheduleunit[m][n].timeend);				
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
				xml.AddChildElem("VALID",(int)pGlobal_Flash->alglostpara[i].lostunit[j].validflag);
				xml.AddChildElem("PTZSET",(int)pGlobal_Flash->alglostpara[i].lostunit[j].preset);
				xml.AddChildElem("PTZTIME",(int)pGlobal_Flash->alglostpara[i].lostunit[j].pretime);
			}
		}
		xml.OutOfElem();
	}
	
	if (group.bAlgDirectPara)
	{
		xml.AddElem("DIRECT-Param-Setting");
		xml.IntoElem();
		for (i = 0;i < sizeof(pGlobal_Flash->algdirectpara)/sizeof(pGlobal_Flash->algdirectpara[0]);i++)
		{
			for (j = 0;j < sizeof(pGlobal_Flash->algdirectpara[0])/sizeof(UNIT_DIRECTION);j++)
			{
				CString cstrDirectIndexNo;
				cstrDirectIndexNo.Format("DIRECT%d",i*sizeof(pGlobal_Flash->algdirectpara[0])/sizeof(UNIT_DIRECTION)+j);
				xml.AddElem(cstrDirectIndexNo);
				xml.AddChildElem("XSTART",(int)pGlobal_Flash->algdirectpara[i].directunit[j].x_start);
				xml.AddChildElem("XEND",(int)pGlobal_Flash->algdirectpara[i].directunit[j].x_end);
				xml.AddChildElem("YSTART",(int)pGlobal_Flash->algdirectpara[i].directunit[j].y_start);
				xml.AddChildElem("YEND",(int)pGlobal_Flash->algdirectpara[i].directunit[j].y_end);
				xml.AddChildElem("DIR",(int)pGlobal_Flash->algdirectpara[i].directunit[j].direction);
				xml.AddChildElem("COLOR",(int)pGlobal_Flash->algdirectpara[i].directunit[j].color);
				//xml.Add
				xml.AddChildElem("SCHEDULE");
				xml.IntoElem();
				for(m = 0;m < 7;m++)
				{
					for (n = 0;n < 8;n++)
					{
						CString cstrScheduleInfo;
						cstrScheduleInfo.Format("WEEK%d-%dTH",m,n);
						xml.AddChildElem(cstrScheduleInfo);
						xml.IntoElem();
						xml.AddChildElem("START",(unsigned short)pGlobal_Flash->algdirectpara[i].directunit[j].scheduleunit[m][n].timestart);
						xml.AddChildElem("END",(unsigned short)pGlobal_Flash->algdirectpara[i].directunit[j].scheduleunit[m][n].timeend);				
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
				xml.AddChildElem("VALID",(int)pGlobal_Flash->algdirectpara[i].directunit[j].validflag);
				xml.AddChildElem("PTZSET",(int)pGlobal_Flash->algdirectpara[i].directunit[j].preset);
				xml.AddChildElem("PTZTIME",(int)pGlobal_Flash->algdirectpara[i].directunit[j].pretime);
			}
		}
		xml.OutOfElem();
	}

	if (group.bAlgDetectPara)
	{
		xml.AddElem("DETECT-Param-Setting");
		xml.IntoElem();
		for (i = 0;i < sizeof(pGlobal_Flash->algdetectpara)/sizeof(pGlobal_Flash->algdetectpara[0]);i++)
		{
			for (j = 0;j < sizeof(pGlobal_Flash->algdetectpara[0])/sizeof(UNIT_DETECTION);j++)
			{
				CString cstrDetectIndexNo;
				cstrDetectIndexNo.Format("DETECT%d",i*sizeof(pGlobal_Flash->algdetectpara[0])/sizeof(UNIT_DETECTION)+j);
				xml.AddElem(cstrDetectIndexNo);
				xml.AddChildElem("XSTART",(int)pGlobal_Flash->algdetectpara[i].dectunit[j].x_start);
				xml.AddChildElem("XEND",(int)pGlobal_Flash->algdetectpara[i].dectunit[j].x_end);
				xml.AddChildElem("YSTART",(int)pGlobal_Flash->algdetectpara[i].dectunit[j].y_start);
				xml.AddChildElem("YEND",(int)pGlobal_Flash->algdetectpara[i].dectunit[j].y_end);
				xml.AddChildElem("DIR",(int)pGlobal_Flash->algdetectpara[i].dectunit[j].direction);
				xml.AddChildElem("COLOR",(int)pGlobal_Flash->algdetectpara[i].dectunit[j].color);
				//xml.Add
				xml.AddChildElem("SCHEDULE");
				xml.IntoElem();
				for(m = 0;m < 7;m++)
				{
					for (n = 0;n < 8;n++)
					{
						CString cstrScheduleInfo;
						cstrScheduleInfo.Format("WEEK%d-%dTH",m,n);
						xml.AddChildElem(cstrScheduleInfo);
						xml.IntoElem();
						xml.AddChildElem("START",(unsigned short)pGlobal_Flash->algdetectpara[i].dectunit[j].scheduleunit[m][n].timestart);
						xml.AddChildElem("END",(unsigned short)pGlobal_Flash->algdetectpara[i].dectunit[j].scheduleunit[m][n].timeend);				
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
				xml.AddChildElem("VALID",(int)pGlobal_Flash->algdetectpara[i].dectunit[j].validflag);
				xml.AddChildElem("PTZSET",(int)pGlobal_Flash->algdetectpara[i].dectunit[j].preset);
				xml.AddChildElem("PTZTIME",(int)pGlobal_Flash->algdetectpara[i].dectunit[j].pretime);
			}
		}
		xml.OutOfElem();
	}

	if (group.bAlgDetectPara)
	{
		xml.AddElem("PEOPLEDENSE-Param-Setting");
		xml.IntoElem();
		for (i = 0;i < sizeof(pGlobal_Flash->algpeopledectpara)/sizeof(pGlobal_Flash->algpeopledectpara[0]);i++)
		{
			for (j = 0;j < sizeof(pGlobal_Flash->algpeopledectpara[0])/sizeof(CONFIG_UNIT);j++)
			{
				CString cstrPeopledenseIndexNo;
				cstrPeopledenseIndexNo.Format("PEOPLEDENSE%d",i*sizeof(pGlobal_Flash->algpeopledectpara[0])/sizeof(CONFIG_UNIT)+j);
				xml.AddElem(cstrPeopledenseIndexNo);
				xml.AddChildElem("POINT");
				for (k = 0;k< 8;k++)
				{
					xml.IntoElem();
					xml.AddChildElem("VALID",(unsigned short)pGlobal_Flash->algpeopledectpara[i].peopledenseunit[j].config_unit[k].valid);
					xml.AddChildElem("X",(unsigned short)pGlobal_Flash->algpeopledectpara[i].peopledenseunit[j].config_unit[k].x);
					xml.AddChildElem("Y",(unsigned short)pGlobal_Flash->algpeopledectpara[i].peopledenseunit[j].config_unit[k].y);
					xml.OutOfElem();
				}
				xml.AddChildElem("COLOR",(int)pGlobal_Flash->algpeopledectpara[i].peopledenseunit[j].color);
				//xml.Add
				xml.AddChildElem("SCHEDULE");
				xml.IntoElem();
				for(m = 0;m < 7;m++)
				{
					for (n = 0;n < 8;n++)
					{
						CString cstrScheduleInfo;
						cstrScheduleInfo.Format("WEEK%d-%dTH",m,n);
						xml.AddChildElem(cstrScheduleInfo);
						xml.IntoElem();
						xml.AddChildElem("START",(unsigned short)pGlobal_Flash->algpeopledectpara[i].peopledenseunit[j].scheduleunit[m][n].timestart);
						xml.AddChildElem("END",(unsigned short)pGlobal_Flash->algpeopledectpara[i].peopledenseunit[j].scheduleunit[m][n].timeend);				
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
				xml.AddChildElem("VALID",(int)pGlobal_Flash->algpeopledectpara[i].peopledenseunit[j].validflag);
				xml.AddChildElem("PTZSET",(int)pGlobal_Flash->algpeopledectpara[i].peopledenseunit[j].preset);
				xml.AddChildElem("PTZTIME",(int)pGlobal_Flash->algpeopledectpara[i].peopledenseunit[j].pretime);
			}
		}
		xml.OutOfElem();
	}

	if (group.bAlgPeopleClusterPara)
	{
		xml.AddElem("CLUSTER-Param-Setting");
		xml.IntoElem();
		for (i = 0;i < sizeof(pGlobal_Flash->algclusterdectpara)/sizeof(pGlobal_Flash->algclusterdectpara[0]);i++)
		{
			for (j = 0;j < sizeof(pGlobal_Flash->algclusterdectpara[0])/sizeof(CONFIG_UNIT);j++)
			{
				CString cstrClusterIndexNo;
				cstrClusterIndexNo.Format("CLUSTER%d",i*sizeof(pGlobal_Flash->algclusterdectpara[0])/sizeof(CONFIG_UNIT)+j);
				xml.AddElem(cstrClusterIndexNo);
				xml.AddChildElem("POINT");
				for (k = 0;k< 8;k++)
				{
					xml.IntoElem();
					xml.AddChildElem("VALID",(unsigned short)pGlobal_Flash->algclusterdectpara[i].clusterdeteunit[j].config_unit[k].valid);
					xml.AddChildElem("X",(unsigned short)pGlobal_Flash->algclusterdectpara[i].clusterdeteunit[j].config_unit[k].x);
					xml.AddChildElem("Y",(unsigned short)pGlobal_Flash->algclusterdectpara[i].clusterdeteunit[j].config_unit[k].y);
					xml.OutOfElem();
				}
				xml.AddChildElem("COLOR",(int)pGlobal_Flash->algclusterdectpara[i].clusterdeteunit[j].color);
				//xml.Add
				xml.AddChildElem("SCHEDULE");
				xml.IntoElem();
				for(m = 0;m < 7;m++)
				{
					for (n = 0;n < 8;n++)
					{
						CString cstrScheduleInfo;
						cstrScheduleInfo.Format("WEEK%d-%dTH",m,n);
						xml.AddChildElem(cstrScheduleInfo);
						xml.IntoElem();
						xml.AddChildElem("START",(unsigned short)pGlobal_Flash->algclusterdectpara[i].clusterdeteunit[j].scheduleunit[m][n].timestart);
						xml.AddChildElem("END",(unsigned short)pGlobal_Flash->algclusterdectpara[i].clusterdeteunit[j].scheduleunit[m][n].timeend);				
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
				xml.AddChildElem("VALID",(int)pGlobal_Flash->algclusterdectpara[i].clusterdeteunit[j].validflag);
				xml.AddChildElem("PTZSET",(int)pGlobal_Flash->algclusterdectpara[i].clusterdeteunit[j].preset);
				xml.AddChildElem("PTZTIME",(int)pGlobal_Flash->algclusterdectpara[i].clusterdeteunit[j].pretime);
			}
		}
		xml.OutOfElem();
	}

	if (group.bAlgPeopleMotionPara)
	{
		xml.AddElem("MOTION-Param-Setting");
		xml.IntoElem();
		for (i = 0;i < sizeof(pGlobal_Flash->algmotionhistorypara)/sizeof(pGlobal_Flash->algmotionhistorypara[0]);i++)
		{
			for (j = 0;j < sizeof(pGlobal_Flash->algmotionhistorypara[0])/sizeof(CONFIG_UNIT);j++)
			{
				CString cstrMotionIndexNo;
				cstrMotionIndexNo.Format("MOTION%d",i*sizeof(pGlobal_Flash->algmotionhistorypara[0])/sizeof(CONFIG_UNIT)+j);
				xml.AddElem(cstrMotionIndexNo);
				xml.AddChildElem("POINT");
				for (k = 0;k< 8;k++)
				{
					xml.IntoElem();
					xml.AddChildElem("VALID",(unsigned short)pGlobal_Flash->algmotionhistorypara[i].motionhistoryunit[j].config_unit[k].valid);
					xml.AddChildElem("X",(unsigned short)pGlobal_Flash->algmotionhistorypara[i].motionhistoryunit[j].config_unit[k].x);
					xml.AddChildElem("Y",(unsigned short)pGlobal_Flash->algmotionhistorypara[i].motionhistoryunit[j].config_unit[k].y);
					xml.OutOfElem();
				}
				xml.AddChildElem("COLOR",(int)pGlobal_Flash->algmotionhistorypara[i].motionhistoryunit[j].color);
				//xml.Add
				xml.AddChildElem("SCHEDULE");
				xml.IntoElem();
				for(m = 0;m < 7;m++)
				{
					for (n = 0;n < 8;n++)
					{
						CString cstrScheduleInfo;
						cstrScheduleInfo.Format("WEEK%d-%dTH",m,n);
						xml.AddChildElem(cstrScheduleInfo);
						xml.IntoElem();
						xml.AddChildElem("START",(unsigned short)pGlobal_Flash->algmotionhistorypara[i].motionhistoryunit[j].scheduleunit[m][n].timestart);
						xml.AddChildElem("END",(unsigned short)pGlobal_Flash->algmotionhistorypara[i].motionhistoryunit[j].scheduleunit[m][n].timeend);				
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
				xml.AddChildElem("VALID",(int)pGlobal_Flash->algmotionhistorypara[i].motionhistoryunit[j].validflag);
				xml.AddChildElem("PTZSET",(int)pGlobal_Flash->algmotionhistorypara[i].motionhistoryunit[j].preset);
				xml.AddChildElem("PTZTIME",(int)pGlobal_Flash->algmotionhistorypara[i].motionhistoryunit[j].pretime);
			}
		}
		xml.OutOfElem();
	}
	
	if (group.bAlgHoverPara)
	{
		xml.AddElem("HOVER-Param-Setting");
		xml.IntoElem();
		for (i = 0;i < sizeof(pGlobal_Flash->alghoverdectpara)/sizeof(pGlobal_Flash->alghoverdectpara[0]);i++)
		{
			for (j = 0;j < sizeof(pGlobal_Flash->alghoverdectpara[0])/sizeof(CONFIG_UNIT);j++)
			{
				CString cstrHoverIndexNo;
				cstrHoverIndexNo.Format("HOVER%d",i*sizeof(pGlobal_Flash->alghoverdectpara[0])/sizeof(CONFIG_UNIT)+j);
				xml.AddElem(cstrHoverIndexNo);
				xml.AddChildElem("POINT");
				for (k = 0;k< 8;k++)
				{
					xml.IntoElem();
					xml.AddChildElem("VALID",(unsigned short)pGlobal_Flash->alghoverdectpara[i].hoverdeteUnit[j].config_unit[k].valid);
					xml.AddChildElem("X",(unsigned short)pGlobal_Flash->alghoverdectpara[i].hoverdeteUnit[j].config_unit[k].x);
					xml.AddChildElem("Y",(unsigned short)pGlobal_Flash->alghoverdectpara[i].hoverdeteUnit[j].config_unit[k].y);
					xml.OutOfElem();
				}
				xml.AddChildElem("COLOR",(int)pGlobal_Flash->alghoverdectpara[i].hoverdeteUnit[j].color);
				//xml.Add
				xml.AddChildElem("SCHEDULE");
				xml.IntoElem();
				for(m = 0;m < 7;m++)
				{
					for (n = 0;n < 8;n++)
					{
						CString cstrScheduleInfo;
						cstrScheduleInfo.Format("WEEK%d-%dTH",m,n);
						xml.AddChildElem(cstrScheduleInfo);
						xml.IntoElem();
						xml.AddChildElem("START",(unsigned short)pGlobal_Flash->alghoverdectpara[i].hoverdeteUnit[j].scheduleunit[m][n].timestart);
						xml.AddChildElem("END",(unsigned short)pGlobal_Flash->alghoverdectpara[i].hoverdeteUnit[j].scheduleunit[m][n].timeend);				
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
				xml.AddChildElem("VALID",(int)pGlobal_Flash->alghoverdectpara[i].hoverdeteUnit[j].validflag);
				xml.AddChildElem("PTZSET",(int)pGlobal_Flash->alghoverdectpara[i].hoverdeteUnit[j].preset);
				xml.AddChildElem("PTZTIME",(int)pGlobal_Flash->alghoverdectpara[i].hoverdeteUnit[j].pretime);
			}
		}
		xml.OutOfElem();
	}
	
	if (group.bAlgAlarmLinePara)
	{
		xml.AddElem("ALARMLINE-Param-Setting");
		xml.IntoElem();
		for (i = 0;i < sizeof(pGlobal_Flash->algalarmlinepara)/sizeof(pGlobal_Flash->algalarmlinepara[0]);i++)
		{
			for (j = 0;j < sizeof(pGlobal_Flash->algalarmlinepara[0])/sizeof(UNIT_ALARMLINENOTDIR_ENTER);j++)
			{
				CString cstrDirectIndexNo;
				cstrDirectIndexNo.Format("ALARMLINE%d",i*sizeof(pGlobal_Flash->algalarmlinepara[0])/sizeof(UNIT_ALARMLINENOTDIR_ENTER)+j);
				xml.AddElem(cstrDirectIndexNo);
				xml.AddChildElem("XSTART",(int)pGlobal_Flash->algalarmlinepara[i].alarmLine[j].x_start);
				xml.AddChildElem("XEND",(int)pGlobal_Flash->algalarmlinepara[i].alarmLine[j].x_end);
				xml.AddChildElem("YSTART",(int)pGlobal_Flash->algalarmlinepara[i].alarmLine[j].y_start);
				xml.AddChildElem("YEND",(int)pGlobal_Flash->algalarmlinepara[i].alarmLine[j].y_end);
				//xml.AddChildElem("DIR",(int)pGlobal_Flash->algalarmlinepara[i].alarmLine[j].direction);
				xml.AddChildElem("COLOR",(int)pGlobal_Flash->algalarmlinepara[i].alarmLine[j].color);
				//xml.Add
				xml.AddChildElem("SCHEDULE");
				xml.IntoElem();
				for(m = 0;m < 7;m++)
				{
					for (n = 0;n < 8;n++)
					{
						CString cstrScheduleInfo;
						cstrScheduleInfo.Format("WEEK%d-%dTH",m,n);
						xml.AddChildElem(cstrScheduleInfo);
						xml.IntoElem();
						xml.AddChildElem("START",(unsigned short)pGlobal_Flash->algalarmlinepara[i].alarmLine[j].scheduleunit[m][n].timestart);
						xml.AddChildElem("END",(unsigned short)pGlobal_Flash->algalarmlinepara[i].alarmLine[j].scheduleunit[m][n].timeend);				
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
				xml.AddChildElem("VALID",(int)pGlobal_Flash->algalarmlinepara[i].alarmLine[j].validflag);
				xml.AddChildElem("PTZSET",(int)pGlobal_Flash->algalarmlinepara[i].alarmLine[j].preset);
				xml.AddChildElem("PTZTIME",(int)pGlobal_Flash->algalarmlinepara[i].alarmLine[j].pretime);
			}
		}
		xml.OutOfElem();
	}

	if (group.bAlgRunningPara)
	{
		xml.AddElem("AREA-Param-Setting");
		xml.IntoElem();
		for (i = 0;i < sizeof(pGlobal_Flash->algrunningpara)/sizeof(pGlobal_Flash->algrunningpara[0]);i++)
		{
			for (j = 0;j < sizeof(pGlobal_Flash->algrunningpara[0])/sizeof(CONFIG_UNIT);j++)
			{
				CString cstrAreaIndexNo;
				cstrAreaIndexNo.Format("RUNNING%d",i*sizeof(pGlobal_Flash->algrunningpara[0])/sizeof(CONFIG_UNIT)+j);
				xml.AddElem(cstrAreaIndexNo);
				xml.AddChildElem("POINT");
				for (k = 0;k< 8;k++)
				{
					xml.IntoElem();
					xml.AddChildElem("VALID",(unsigned short)pGlobal_Flash->algrunningpara[i].runningObjUnit[j].config_unit[k].valid);
					xml.AddChildElem("X",(unsigned short)pGlobal_Flash->algrunningpara[i].runningObjUnit[j].config_unit[k].x);
					xml.AddChildElem("Y",(unsigned short)pGlobal_Flash->algrunningpara[i].runningObjUnit[j].config_unit[k].y);
					xml.OutOfElem();
				}
				xml.AddChildElem("COLOR",(int)pGlobal_Flash->algrunningpara[i].runningObjUnit[j].color);
				//xml.Add
				xml.AddChildElem("SCHEDULE");
				xml.IntoElem();
				for(m = 0;m < 7;m++)
				{
					for (n = 0;n < 8;n++)
					{
						CString cstrScheduleInfo;
						cstrScheduleInfo.Format("WEEK%d-%dTH",m,n);
						xml.AddChildElem(cstrScheduleInfo);
						xml.IntoElem();
						xml.AddChildElem("START",(unsigned short)pGlobal_Flash->algrunningpara[i].runningObjUnit[j].scheduleunit[m][n].timestart);
						xml.AddChildElem("END",(unsigned short)pGlobal_Flash->algrunningpara[i].runningObjUnit[j].scheduleunit[m][n].timeend);				
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
				xml.AddChildElem("VALID",(int)pGlobal_Flash->algrunningpara[i].runningObjUnit[j].validflag);
				xml.AddChildElem("PTZSET",(int)pGlobal_Flash->algrunningpara[i].runningObjUnit[j].preset);
				xml.AddChildElem("PTZTIME",(int)pGlobal_Flash->algrunningpara[i].runningObjUnit[j].pretime);
			}
		}
		xml.OutOfElem();
	}

	if (group.bAlgSysConfigPara)
	{
		xml.AddElem("SYSCONFIG-Param-Setting");
		xml.IntoElem();
		xml.AddElem("CAMCONFIG");
		xml.AddChildElem("DEVID",CString((char *)pGlobal_Flash->sysconfigpara.camconfig.dev_id));
		xml.AddChildElem("DEVNAME",CString((char *)pGlobal_Flash->sysconfigpara.camconfig.dev_name));
		xml.AddElem("CAMSTATE",(int)pGlobal_Flash->sysconfigpara.camstate);
		xml.AddElem("CAMIPCONFIG");
		xml.AddChildElem("IPADDR",CString((char *)pGlobal_Flash->sysconfigpara.camipconfig.ipaddr));
		xml.AddChildElem("IPMASK",CString((char *)pGlobal_Flash->sysconfigpara.camipconfig.ipmask));
		xml.AddChildElem("IPGW",CString((char *)pGlobal_Flash->sysconfigpara.camipconfig.ipgw));
		xml.AddChildElem("MAC",CString((char *)pGlobal_Flash->sysconfigpara.camipconfig.macaddr));
		xml.AddChildElem("VIDEOPORT",(unsigned int)pGlobal_Flash->sysconfigpara.camipconfig.port_video);
		xml.AddElem("CENTERIPCONFIG");
		xml.AddChildElem("IPADDR",CString((char *)pGlobal_Flash->sysconfigpara.centeripconfig.ipaddr));
		xml.AddChildElem("IPMASK",CString((char *)pGlobal_Flash->sysconfigpara.centeripconfig.ipmask));
		xml.AddChildElem("IPGW",CString((char *)pGlobal_Flash->sysconfigpara.centeripconfig.ipgw));
		xml.AddChildElem("MAC",CString((char *)pGlobal_Flash->sysconfigpara.centeripconfig.macaddr));
		xml.AddChildElem("VIDEOPORT",(unsigned int)pGlobal_Flash->sysconfigpara.centeripconfig.center_port);
		xml.OutOfElem();
	}
	
	if (group.bAlgStoragePara)
	{
		xml.AddElem("STORAGE-Param-Setting");
		xml.IntoElem();
		xml.AddElem("DISTINFO");
		xml.AddChildElem("SYSTYPE",(unsigned int)pGlobal_Flash->sysstoragepara.storageconfig.systemtype);
		xml.AddChildElem("DISCNT",(unsigned int)pGlobal_Flash->sysstoragepara.storageconfig.diskcount);
		for (i = 0;i<(unsigned int)pGlobal_Flash->sysstoragepara.storageconfig.diskcount&&i<10;i++)
		{
			CString cstrDistInfo;
			cstrDistInfo.Format("DIST%d",i);
			xml.AddChildElem(cstrDistInfo);
			xml.IntoElem();
			xml.AddChildElem("TYPE",(unsigned int)pGlobal_Flash->sysstoragepara.storageconfig.diskdesc[i].drive_type);
			xml.AddChildElem("FSTYPE",(unsigned int)pGlobal_Flash->sysstoragepara.storageconfig.diskdesc[i].drive_fstype);
			xml.AddChildElem("NAME",CString((char*)pGlobal_Flash->sysstoragepara.storageconfig.diskdesc[i].disk_name));
			xml.AddChildElem("REALSIZE",(unsigned int)pGlobal_Flash->sysstoragepara.storageconfig.diskdesc[i].realsize);
			xml.AddChildElem("EMPTYSIZE",(unsigned int)pGlobal_Flash->sysstoragepara.storageconfig.diskdesc[i].emptysize);
			xml.OutOfElem();
		}
		xml.AddElem("CAMSTORAGE");
		xml.AddChildElem("STORAGELOC",CString((char*)pGlobal_Flash->sysstoragepara.camstorage.storage_position));
		xml.AddChildElem("ALARMPERCENT",(unsigned int)pGlobal_Flash->sysstoragepara.camstorage.percent_alarm);
		xml.AddChildElem("DELFLAG",(unsigned int)pGlobal_Flash->sysstoragepara.camstorage.autodelete_flag);
		xml.AddChildElem("DELPERIOD",(unsigned int)pGlobal_Flash->sysstoragepara.camstorage.delete_period);
		xml.AddElem("INFO","\r\n--SYSTYPE% <1-2:1WINDOWS_2LINUX>\r\n--TYPE% <0-6:0UNKOWN_1NOROOT_2REMOVABLE_3FIXED_4REMOTE_5CDROM_6RAMDISK>\r\n--FSTYPE <1-3:1UNKOWN_2NTFS_3FAT32>\r\n");
		xml.OutOfElem();
	}

	xml.OutOfElem();
	xml.Save(Xml_file);
	return true;
}	



