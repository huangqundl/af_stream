
/**@file  egnapisspsmp.h
  *   系统管理功能和系统服务功能定义
  *******************************************************/

#ifndef __EGN_API_SSP_SMP_H__
#define __EGN_API_SSP_SMP_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/** 全局组件模块号 */
#define EGN_COMP_GLOBAL 0

/** 分配内存类型定义 */
#define EGN_MEM_DYNAMIC 0   /* 动态内存 */
#define EGN_MEM_STATIC  1   /* 静态内存 */

/** CPU型号定义 */
#ifdef EGN_CPU
#define EGN_X86      "X86"      /* X86 */
#define EGN_PPC750   "PPC750"   /* PPC750 */
#define EGN_MIPS64   "MIPS64"   /* MIPS64 */
#else
#define EGN_CPU ""              /* NONE */
#endif

/** EGN支持的操作系统定义 */
#ifdef EGN_OSVER
#define EGN_WIN32          "WIN32"              /* WIN32 */
#define EGN_SUSE_LNX       "SUSE_LINUX"         /* SUSE_LINUX */
#define EGN_WINDRIVER_LNX  "WINDRIVER_LINUX"    /* WINDRIVER_LINUX */
#define EGN_VXWS           "VXWORKS"            /* VXWORKS */
#define EGN_VXWS55         "VXWORKS55"          /* VXWORKS55 */
#else
#define EGN_OSVER          ""                   /* NONE */
#endif

/** EGN用户定义 */
#ifdef EGN_PRODUCT
#define EGN_GEN     "GEN"   /* GEN */
#define EGN_UAP     "GGSN"  /* GGSN */
#define EGN_SCG     "SCG"   /* SCG */
#else
#define EGN_PRODUCT ""      /* NONE */
#endif

/** 文件操作位置定义 */
#ifndef EGN_FILE_OP
#define  EGN_FILE_OP                /* 文件操作位置 */
#define  EGN_SEEK_SET    0          /* 从文件头开始 */
#define  EGN_SEEK_CUR    1          /* 从文件指针当前位置开始 */
#define  EGN_SEEK_END    2          /* 从文件结尾开始 */

#define  EgnFileHandle   EGN_VOID*  /* 定义文件句柄 */
#endif  /* EGN_FILE_OP */

/*ENUM< 调试控制开关 >*/
typedef enum
{
    EGN_DEBUG_BEGIN   = EGN_EN_INVALID,
    EGN_DEBUG_ON      = 0,     /* 调试信息开关打开 */
    EGN_DEBUG_OFF     = 1,     /* 调试信息开关关闭 */
    EGN_DEBUG_END,

    EGN_DEBUG_BOTTOM  =  EGN_EN_BUTT
}EGN_DBG_SWTICH_EN;

/*ENUM< 调试信息输出级别 >*/
typedef enum
{
    EGN_DEBUG_LEVEL_INVALID = EGN_EN_INVALID,
    EGN_DEBUG_PRINT_ALL     = 0,  /* 保留未使用 */
    EGN_DEBUG_LEVEL_FATAL   = 1,  /* 输出Fatal信息 */
    EGN_DEBUG_LEVEL_ERR     = 2,  /* 输出Fatal、Error信息 */
    EGN_DEBUG_LEVEL_WARN    = 3,  /* 输出Fatal、Error、Warning信息 */
    EGN_DEBUG_LEVEL_INFO    = 4,  /* 输出Fatal、Error、Warning、Info信息 */
    EGN_DEBUG_LEVEL_END,

    EGN_DEBUG_LEVEL_BOTTOM    =  EGN_EN_BUTT
}EGN_DEBUG_LEVEL_EN;

/*ENUM< 调试信息输出控制开关 >*/
typedef enum
{
    EGN_DBGCMD_INVALID       = EGN_EN_INVALID,
    EGN_DBGCMD_ONOFF_SWITCH  = 1, /* enum EGN_DBG_SWTICH_EN */
    EGN_DBGCMD_LEVEL_SWITCH  = 2, /* enum EGN_DEBUG_LEVEL_EN */
    EGN_DBGCMD_END,

    EGN_DBGCMD_BOTTOM        = EGN_EN_BUTT
}EGN_DBG_CMD_EN;

/*ENUM< 模块号定义 >*/
typedef enum
{
    EGN_MODULE_INVALID  = EGN_EN_INVALID,
    EGN_MODULE_PARSER   = 0,    /* 解析模块 */
    EGN_MODULE_REPORT   = 1,    /* 报表模块 */
    EGN_MODULE_ENGINE   = 2,    /* 引擎模块 */
    EGN_MODULE_END,

    EGN_MODULE_BOTTOM     =  EGN_EN_BUTT
}EGN_MODULE_EN;

/*ENUM< 定时器组运行模式定义 >*/
typedef enum    EGN_TIMER_MODE_TYPE_ENUM
{
    EGN_TIMER_MODE_TYPE_BEGIN  = EGN_EN_INVALID,
    EGN_TIMER_MODE_TYPE_NOLOOP = 0,             /* 非循环模式 */
    EGN_TIMER_MODE_TYPE_LOOP   = 1,             /* 循环模式 */
    EGN_TIMER_MODE_TYPE_END,

    EGN_TIMER_MODE_TYPE_BOTTOM = EGN_EN_BUTT
}EGN_TIMER_MODE_TYPE_EN;

/*
 *#define EGN_TASK_ID  g_stCoreGlobalInfo.ulTaskId
*/

/*MACRO< TaskID >*/
#define EGN_TASK_ID 0

/*MACRO< 日志ID >*/
#define EGN_LOG_ID  (EGN_UINT32)((0xff000000 & __FILE_ID__) | __LINE__)

/*MACRO< 日志等级 >*/
#define EGN_LOG_ERR 0

/*******************************************************************************
*    Func Name: PFEgnSspMemAlloc
*      Purpose: 申请内存
*  Description: 内存申请的回调函数，为ulPID模块申请类型为ulMemType，大小为ulSize的内存。
*        Input: EGN_UINT32 ulPID:模块号<EGN_MODULE_EN>
*               EGN_UINT32 ulMemType:内存的类型<EGN_DYNAMIC/EGN_STATIC>
*               EGN_UINT32 ulSize:申请内存的大小，单位:字节<大于零>
*        InOut: NA
*       Output: NA
*       Return: HTTP_VOID，返回分配内存的地址
*      Caution: 无。
*        Since: V100R001C01
*    Reference: PFEgnSspMemFree
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID * (*PFEgnSspMemAlloc)
(
    IN EGN_UINT32   ulPID,
    IN EGN_UINT32   ulMemType,
    IN EGN_UINT32   ulSize
);

/*******************************************************************************
*    Func Name: PFEgnSspMemFree
*      Purpose: 释放内存
*  Description: 释放内存的回调函数，释放模块号为ulModId地址为pvMemAddr的内存。
*        Input: EGN_UINT32 ulModId:模块号<EGN_MODULE_EN>
*               EGN_VOID *pvMemAddr:内存的地址<非空>
*        InOut: NA
*       Output: NA
*       Return: 无。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: PFEgnSspMemAlloc
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID (*PFEgnSspMemFree)
(
    IN EGN_UINT32   ulModId,
    IN EGN_VOID    *pvMemAddr
);

/*******************************************************************************
*    Func Name: PFEgnSspMemSet
*      Purpose: 填充内存的回调函数
*  Description: 对起始地址为pvMemAddr、长度为ulSize的内存，填充字符ucByte。
*        Input: EGN_VOID *pvMemAddr:内存的地址<EGN_MODULE_EN>
*               EGN_UCHAR ucByte:填充的字符<字符值>
*               EGN_UINT32 ulSize:填充的长度，单位:字节<大于零>
*        InOut: NA
*       Output: NA
*       Return: EGN_VOID，返回被填充的目标地址。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID * (*PFEgnSspMemSet)
(
    IN EGN_VOID    *pvMemAddr,
    IN EGN_UCHAR    ucByte,
    IN EGN_UINT32   ulSize
);

/*******************************************************************************
*    Func Name: PFEgnSspMemCpy
*      Purpose: 内存复制的回调函数
*  Description: 将以源地址pvSrcAddr起始的内存中长度为ulSize的内容，复制到以目标地址pvDestAddr起始的内存中。
*        Input: EGN_VOID *pvSrcAddr:源地址<非空>
*               EGN_UINT32 ulSize:复制的长度，单位:字节<大于零>
*        InOut: EGN_VOID *pvDestAddr:目标地址<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution: 把一块内存的内容复制到另一块内在中，不会分配内存，指定的长度不能大于目标地址的长度，否则会内存越界。
*        Since: V100R001C01
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID *(*PFEgnSspMemCpy)
(
    INOUT EGN_VOID     *pvDestAddr,
    IN    EGN_VOID     *pvSrcAddr,
    IN    EGN_UINT32   ulSize
);

/*******************************************************************************
*    Func Name: PFEgnSspMemCmp
*      Purpose: 内存比较的回调函数
*  Description: 将以源地址pvSrcAddr起始的内存和以目标地址pvDestAddr起始的内存中长度为ulSize的内容比较。
*        Input: EGN_VOID *pvDestAddr:目标地址<非空>
*               EGN_VOID *pvSrcAddr:源地址<非空>
*               EGN_UINT32 ulSize:比较的长度，单位:字节<大于零>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，0表示相等，非零表示不等。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_UINT32   (*PFEgnSspMemCmp)
(
    IN  EGN_VOID    *pvDestAddr,
    IN  EGN_VOID    *pvSrcAddr,
    IN  EGN_UINT32   ulSize
);

/*******************************************************************************
*    Func Name: PFEgnSspStrLen
*      Purpose: 计算字符串的长度的回调函数。
*  Description: 计算输入字符串pcStr的长度，一直计算到字符串结束符\\0为止，不包括\\0。
*        Input: EGN_CHAR *pcStr:指向字符串的指针<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，返回字符串的长度。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_UINT32   (*PFEgnSspStrLen)
(
    IN  EGN_CHAR    *pcStr
);

/*******************************************************************************
*    Func Name: PFEgnSspStrNCmp
*      Purpose: 字符串比较的回调函数。
*  Description: 比较目标字符串pcStr1和源字符串pcStr2前ulCount个字节是否相同。
*        Input: EGN_CHAR *pcStr1:目标字符串<非空>
*               EGN_CHAR *pcStr2:源字符串<非空>
*               EGN_UINT32 ulCount:比较的长度，单位：字节<大于零>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，0为字符串相等，其他返回值为字符串不相等。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_UINT32   (*PFEgnSspStrNCmp)
(
    IN  EGN_CONST   EGN_CHAR    *pcStr1,
    IN  EGN_CONST   EGN_CHAR    *pcStr2,
    IN              EGN_UINT32   ulCnt
);

/*******************************************************************************
*    Func Name: PFEgnAtomicAdd
*      Purpose: 原子加操作。
*  Description: 输入加数ulAddEnd和指向被加数的指针pulSummand，将结果存pulSummand指向的内存中。
*        Input: EGN_UINT32 ulAddEnd:加数<大于零>
*        InOut: EGN_UINT32 *pulSummand:加数，执行后为和<非空>
*       Output: NA
*       Return: 无。
*      Caution: 在对pulSummand指向的内存做加操作的时候，要保证是原子操作。
*        Since: V100R001C03
*    Reference: PFEgnAtomicSub
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID (*PFEgnAtomicAdd)
(
    IN      EGN_UINT32  ulAddEnd,
    INOUT   EGN_UINT32 *pulSummand
);

/*******************************************************************************
*    Func Name: PFEgnAtomicSub
*      Purpose: 原子减操作。
*  Description: 输入减数ulSubtrahend和指向被减数的指针pulMinuend，将结果存pulMinuend指向的内存中。
*        Input: EGN_UINT32 ulSubtrahend:减数<大于零>
*        InOut: EGN_UINT32 *pulMinuend:被减数，执行后为差<非空>
*       Output: NA
*       Return: 无。
*      Caution: 在对pulMinuend指向的内存做减操作的时候，要保证是原子操作。
*        Since: V100R001C03
*    Reference: PFEgnAtomicAdd
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID (*PFEgnAtomicSub)
(
    IN      EGN_UINT32  ulSubtrahend,
    INOUT   EGN_UINT32 *pulMinuend
);

/*******************************************************************************
*    Func Name: PFEgnFeedWatchdog
*      Purpose: 复位看门狗。
*  Description: null
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: 无。
*      Caution: 无。
*        Since: V100R001C03
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID (*PFEgnFeedWatchdog)
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: PFEgnFuncPeerSynAdd
*      Purpose: 关联表信息同步接口。
*  Description: 当需要支持非对称识别场景时，需要调用该接口实现不同设备间的关联表项同步。
*        Input: EgnPeerSet pstPeerSet:待同步的关联表项信息<非空>
*        InOut: NA
*       Output: NA
*       Return: 无。
*      Caution: 该同步操作只有在对数据包内容进行解析添加关联表的时候才调用，对于快速关联不调用。
*        Since: V300R006C00
*    Reference: N/A
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID (*PFEgnFuncPeerSynAdd)
(
    IN  EgnPeerSet    *pstPeerSet
);

/*******************************************************************************
*    Func Name: PFEgnIsInspectTimeOut
*      Purpose: API识别时长保护。
*  Description: 是否超时由产品控制判断，注册的时长保护函数不能阻塞操作，只支持TD_RNC产品。
*        Input: EGN_UINT16 usThreadID:实例线程ID<0~65535>
*        InOut: NA
*       Output: NA
*       Return: EGN_BOOL，识别是否超时。
                1表示识别时间已经超时，需要立即返回特定错误码EGN_RET_INSPECT_TIME_OUT，EGN后续流程将此场景当作一种异常场景处理；
                0表示不需中断，可以继续识别。
*      Caution: 无。
*        Since: V200R002C02
*    Reference: N/A
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_BOOL (*PFEgnIsInspectTimeOut)
(
    IN  EGN_UINT16  usThreadID  /* 实例线程ID */
);

/*******************************************************************************
*    Func Name: PFEgnSspFileOpen
*      Purpose: 打开文件。
*  Description: 以ucMode模式打开文件名称为pucFileName的文件。
*        Input: EGN_UCHAR *pucFileName:文件名称<非空>
*               EGN_CHAR ucMode:模式，参见fopen函数的mode参数<fopen函数的mode参数>
*        InOut: NA
*       Output: NA
*       Return: EgnFileHandle，返回0表示失败，返回非空，表示创建的文件句柄。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EgnFileHandle   (*PFEgnSspFileOpen)
(
    IN  EGN_CONST EGN_UCHAR   *pucFileName,
    IN            EGN_CHAR    *pcMode
);

/*******************************************************************************
*    Func Name: PFEgnSspFileSeek
*      Purpose: 输入文件句柄hFd，定位文件读写的位置为从起始位置ulWhence开始偏移ilOffset个字节。
*  Description: null
*        Input: EgnFileHandle hFd:文件句柄<非空>
*               EGN_INT32 ilOffset:偏移的距离，相对lWhence而言<任意值>
*               EGN_UINT32 ulWhence:定位的起始位置<参见EGN_FILE_OP>
*        InOut: NA
*       Output: NA
*       Return: EGN_INT32，返回EGN_RET_SUCCESS表示成功，其他表示失败。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_INT32   (*PFEgnSspFileSeek)
(
    IN  EgnFileHandle  hFd,
    IN  EGN_INT32      ilOffset,
    IN  EGN_UINT32     ulWhence
);

/*******************************************************************************
*    Func Name: PFEgnSspFileRead
*      Purpose: 读取文件内容到指定的内存空间。
*  Description: 读取文件hFd种ulCount字节数的内容到pBuf指向的内存块。
*        Input: EgnFileHandle hFd:文件句柄<非空>
*               EGN_VOID *pBuf:读入的内存区块<非空>
*               EGN_UINT32 ulCount:读入的字节数<大于零>
*        InOut: NA
*       Output: NA
*       Return: EGN_INT32，返回值>=0，表示实际读取的字节大小数目，其他表示失败。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_INT32   (*PFEgnSspFileRead)
(
    IN  EgnFileHandle    hFd,
    IN  EGN_VOID        *pBuf,
    IN  EGN_UINT32       ulCnt
);

/*******************************************************************************
*    Func Name: PFEgnSspFileClose
*      Purpose: 关闭文件句柄。
*  Description: 关闭已经打开的文件hFd。
*        Input: EgnFileHandle hFd:文件句柄<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_INT32，返回EGN_RET_SUCCESS表示成功，其他表示失败。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_INT32   (*PFEgnSspFileClose)
(
    IN  EgnFileHandle      hFd
);

/*******************************************************************************
*    Func Name: PFEgnFuncTimeout
*      Purpose: 定时器超时处理函数。
*  Description: 定时器超时处理函数，输入超时的定时器序号ulTmSn和定时器名称ulTmName，
                当定时器超时时，执行关联表老化，知识库升级。
*        Input: EGN_UINT32 ulTmSn:超时的定时器序号<定时器序号：0~n>
*               EGN_UINT32 ulTmName:超时的定时器名<回调，不关注>
*        InOut: NA
*       Output: NA
*       Return: 无。
*      Caution:
*        Since: V100R001C01
*    Reference: PFEgnTimerGrpCreate
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID (*PFEgnFuncTimeout)
(
    IN  EGN_UINT32      ulTmSn,  /* 相对定时器的序号The serial number ofthe relative timer */
    IN  EGN_UINT32      ulTmName /* 定时器名字 The timer name */
);

/*******************************************************************************
*    Func Name: PFEgnTimerGrpCreate
*      Purpose: 创建定时器组。
*  Description: 根据输入的定时器组超时处理函数pfTimeout和定时器个数ulMaxTimerNums创建的定时器组，
                输出创建的定时器组句柄ppTimerGrp。
*        Input: EGN_UINT32 ulMaxTimerNums:创建的定时器组的定时器个数<1~32>
*               PFEgnFuncTimeout pfTimeout:创建的定时器组超时处理函数<非空>
*        InOut: NA
*       Output: EGN_VOID** ppTimerGrp:创建的定时器组句柄<非空>
*       Return: EGN_UINT32，返回EGN_RET_SUCCESS表示成功，其他表示失败。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: PFEgnTimerGrpDestroy
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_UINT32  (*PFEgnTimerGrpCreate)
(
    OUT EGN_VOID          **ppvTimerGrp,
    IN  EGN_UINT32          ulMaxTimerNums,
    IN  PFEgnFuncTimeout    pfTimeout
);

/*******************************************************************************
*    Func Name: PFEgnTimerGrpStart
*      Purpose: 启动定时器组。
*  Description: 根据输入要启动的定时器组句柄ppTimerGrp，序号ulTmSn，名称ulTmName启动定时器组，
                并设置定时器组的超时时长ulTmLength和运行模式ucTmMode。
*        Input: EGN_VOID* ppTimerGrp:要启动的定时器组句柄<非空>
*               EGN_UINT32 ulTmSn:启动定时器组的定时器序号<0~31>
*               EGN_UINT32 ulTmLength:定时器超时时长，单位为ms<100~60000>
*               EGN_UINT32 ulTmName:启动定时器组的定时器名称<回调，不关注>
*               EGN_UINT32 ucTmMode:定时器组运行模式<EGN_TIMER_MODE_TYPE_EN>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，返回EGN_RET_SUCCESS表示成功，其他表示失败。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: PFEgnTimerGrpStop
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_UINT32  (*PFEgnTimerGrpStart)
(
    IN  EGN_VOID           *pvTimerGrp,
    IN  EGN_UINT32          ulTmSn,
    IN  EGN_UINT32          ulTmLength,
    IN  EGN_UINT32          ulTmName,
    IN  EGN_UCHAR           ucTmMode
);

/*******************************************************************************
*    Func Name: PFEgnTimerGrpStop
*      Purpose: 停止定时器组。
*  Description: 输入要停止的定时器组句柄ppTimerGrp和定时器序号ulTmSn，停止定时器组。
*        Input: EGN_VOID* ppTimerGrp:要停止的定时器组句柄<非空>
*               EGN_UINT32 ulTmSn:停止的定时器组的定时器序号<0~31>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，返回EGN_RET_SUCCESS表示成功，其他表示失败。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: PFEgnTimerGrpStart
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_UINT32  (*PFEgnTimerGrpStop)
(
    IN  EGN_VOID           *pvTimerGrp,
    IN  EGN_UINT32          ulTmSn
);

/*******************************************************************************
*    Func Name: PFEgnTimerGrpDestroy
*      Purpose: 释放定时器组。
*  Description: 输入要释放的定时器组句柄ppTimerGrp，释放定时器组。
*        Input: EGN_VOID** ppTimerGrp:要释放的定时器组句柄<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，返回EGN_RET_SUCCESS表示成功，其他表示失败。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: PFEgnTimerGrpCreate
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_UINT32  (*PFEgnTimerGrpDestroy)
(
    IN  EGN_VOID          **ppvTimerGrp
);

/*******************************************************************************
*    Func Name: PFEgnSspFileTell
*      Purpose: 获取文件指针当前位置。
*  Description: 输入文件句柄，获取文件指针当前位置。
*        Input: EGN_INT32 hFd:文件句柄<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_INT32，返回文件指针当前位置。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_INT32   (*PFEgnSspFileTell)
(
    IN  EgnFileHandle       hFd
);

/*******************************************************************************
*    Func Name: PFEgnSspFileRewind
*      Purpose: 移动文件指针到文件首。
*  Description: 输入文件句柄，移动文件指针到文件首。
*        Input: EGN_INT32 hFd:文件句柄<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_VOID
*      Caution: 无。
*        Since: V100R001C01
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID    (*PFEgnSspFileRewind)
(
    IN  EgnFileHandle       hFd
);

/*******************************************************************************
*    Func Name: PFEgnFuncAssert
*      Purpose: 输出断言信息。
*  Description: 断言输出的回调函数。用户调用接口EgnApiRegAssertFn，将断言回调函数PFEgnFuncAssert注册给组件。
                组件通过调用回调函数PFEgnFuncAssert，将完整的堆栈调用信息输出给用户。
*        Input: EGN_BOOL bCond:断言参数<EGN_FALSE/EGN_TRUE>
*        InOut: NA
*       Output: NA
*       Return: EGN_VOID
*      Caution: 无。
*        Since: V100R001C03
*    Reference: EgnApiRegAssertFn
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID (*PFEgnFuncAssert)
(
    IN    EGN_BOOL bCond
);

/*******************************************************************************
*    Func Name: PFEgnFuncLogSend
*      Purpose: 输出日志信息。
*  Description: 日志输出的回调函数。用户调用接口EgnApiRegLogSendFn，将日志信息输出回调函数
                PFEgnFuncLogSend注册给组件。组件通过调用回调函数PFEgnFuncLogSend，将日志信息输出给用户。
*        Input: EGN_UINT32 ulTaskId:TaskID，由系统初始化时配置到egn模块<EGN_TASK_ID>
*               EGN_UINT32 ulLogId:解析块内部唯一标识日志的ID，使用模块号和行号的组合<EGN_LOG_ID>
*               EGN_UINT32 ulLogLevel:日志的等级，暂未使用，预留<EGN_LOG_ERR>
*               EGN_INT8 pcFormat:格式化输出的内容<非空>
*        InOut: NA
*       Output: NA
*       Return: 无。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: EgnApiRegLogSendFn
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID (*PFEgnFuncLogSend)
(
    IN  EGN_UINT32  ulTaskId,
    IN  EGN_UINT32  ulLogId,
    IN  EGN_UINT32  ulLogLevel,
    IN  EGN_INT8   *pcFormat,
    ...
);

/*******************************************************************************
*    Func Name: PFEgnFuncDbgPrint
*      Purpose: 输出调试信息。
*  Description: 调试信息输出的回调函数。用户调用接口EgnApiRegDebugPrintFn，将调试信息输出回调函数
                PFEgnFuncDbgPrint注册给组件。组件通过调用回调函数PFEgnFuncDbgPrint，将调试信息输出给用户。
*        Input: EGN_UINT32 ulTaskId:由用户在初始化时注册<EGN_TASK_ID>
*               EGN_UINT32 ulDebugPrintLevel:调试等级<EGN_DEBUG_LEVEL_EN>
*               EGN_VOID pvDbgPara:调试参数<非空>
*               EGN_INT8 format:格式化输出的内容<非空>
*        InOut: NA
*       Output: NA
*       Return: 无。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: EgnApiRegDebugPrintFn
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID (*PFEgnFuncDbgPrint)
(
    IN  EGN_UINT32   ulTaskId,
    IN  EGN_UINT32   ulDebugPrintLevel,
    IN  EGN_VOID    *pvDbgPara,
    IN  EGN_INT8    *pcFormat,...
);

/*******************************************************************************
*    Func Name: PFEgnFuncStatSend
*      Purpose: 输出统计信息。
*  Description: 统计信息输出的回调函数。用户调用接口EgnApiRegStatSendFn，将统计信息输出回调函数
                PFEgnFuncStatSend注册给组件。组件通过调用回调函数PFEgnFuncStatSend，将统计信息输出给用户。
*        Input: EGN_UINT32 ulStatUnit:统计的类型<EGN_STAT_UNIT_EN>
*               EGN_UINT32 ulStatEntity:统计的协议类型<EGN_STAT_ENTITY_EN>
*               EGN_UINT32 ulOperateType:统计操作<EGN_OPER_ADD/EGN_OPER_DEL/EGN_OPER_SET>
*               EGN_UINT32 ulValue:数据<大于零>
*               EGN_UCHAR pucParams:参数<非空>
*        InOut: NA
*       Output: NA
*       Return: 无。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: EgnApiRegStatSendFn
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID (*PFEgnFuncStatSend)
(
    IN EGN_UINT32 ulStatUnit,
    IN EGN_UINT32 ulStatEntity,
    IN EGN_UINT32 ulOperateType,
    IN EGN_UINT32 ulValue,
    IN EGN_UCHAR  *pucParams
);

/*******************************************************************************
*    Func Name: PFEgnFuncTraceSend
*      Purpose: 输出跟踪信息。
*  Description: 跟踪信息输出的回调函数。用户调用接口EgnApiRegTraceSendFn，将跟踪信息输出回调函数
                PFEgnFuncTraceSend注册给组件。组件通过调用回调函数PFEgnFuncTraceSend，将跟踪信息输出给用户。
*        Input: EGN_UINT32 ulTraceObjectType:模块号<EGN_TASK_ID>
*               EGN_UINT32 ulTraceObj:跟踪对象<预留>
*               EGN_UINT32 ulMsgLength:消息长度<大于零>
*               EGN_UCHAR pucTraceMsg:消息报文<非空>
*               EGN_VOID pvTraceHandle:跟踪句柄<非空>
*        InOut: NA
*       Output: NA
*       Return: 无。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: EgnApiRegTraceSendFn
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID (*PFEgnFuncTraceSend)
(
    IN EGN_UINT32  ulTraceObjectType,
    IN EGN_UINT32  ulTraceObj,
    IN EGN_UINT32  ulMsgLength,
    IN EGN_UCHAR  *pucTraceMsg,
    IN EGN_VOID   *pvTraceHandle
);

/*******************************************************************************
*    Func Name: PFEgnRWLockCreate
*      Purpose: 创建读写锁。
*  Description: 创建读写锁的回调函数，输入锁的名称pucName和锁的句柄指针ppRWLockHandle创建一个读写锁。
*        Input: EGN_UCHAR pucName:锁的名字<非空>
*               EGN_VOID** ppRWLockHandle:锁的句柄指针<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，返回EGN_RET_SUCCESS表示成功，其他表示失败。
*      Caution: 不同线程，如果创建的锁的名字一样，要求返回同一个handle。
*        Since: V100R001C01
*    Reference: PFEgnRWLockDelete
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_UINT32 (*PFEgnRWLockCreate)
(
    IN EGN_CONST    EGN_UCHAR *pucName,
    INOUT           EGN_VOID** ppRWLockHandle
);

/*******************************************************************************
*    Func Name: PFEgnRWLock
*      Purpose: 加读写锁回调函数。
*  Description: 加读写锁回调函数，输入锁的句柄pRWLockHandle加锁。
*        Input: const pRWLockHandle:EGN_VOID，锁的句柄<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，返回EGN_RET_SUCCESS表示成功，其他表示失败。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: PFEgnRWUnlock
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_UINT32 (*PFEgnRWLock)
(
    IN EGN_CONST EGN_VOID   *pRWLockHandle
);

/*******************************************************************************
*    Func Name: PFEgnRWUnlock
*      Purpose: 解读写锁回调函数。
*  Description: 解读写锁回调函数，输入锁的句柄pRWLockHandle解锁。
*        Input: const pRWLockHandle:EGN_VOID。锁的句柄<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，返回EGN_RET_SUCCESS表示成功，其他表示失败。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: PFEgnRWLock
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_UINT32 (*PFEgnRWUnlock)
(
    IN EGN_CONST EGN_VOID* pRWLockHandle
);

/*******************************************************************************
*    Func Name: PFEgnRWLockDelete
*      Purpose: 删除读写锁回调函数。
*  Description: 根据输入锁的名字pucName和锁的句柄指针ppRWLockHandle删除锁。
*        Input: EGN_UCHAR pucName:锁的名字<非空>
*               const ppRWLockHandle:EGN_VOID，锁的句柄指针<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，返回EGN_RET_SUCCESS表示成功，其他表示失败。
*      Caution: 无。
*        Since: V100R001C01
*    Reference: PFEgnRWLockCreate
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_UINT32 (*PFEgnRWLockDelete)
(
    IN      EGN_CONST   EGN_UCHAR *pucName,
    INOUT               EGN_VOID **ppRWLockHandle
);

/*******************************************************************************
*    Func Name: PFEgnSspMemCreateBuf
*      Purpose: 创建链式内存管理句柄
*  Description: 创建链式内存管理句柄回调函数
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_VOID，创建的内存管理句柄。
*      Caution:
*        Since: V300R005C01
*    Reference: PFEgnMmCreateBufMem
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID * (*PFEgnSspMemCreateBuf)
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: PFEgnSspMemReleaseBuf
*      Purpose: 释放内存控制块
*  Description: 释放内存控制块回调函数
*        Input: NA
*        InOut: EGN_VOID *pvBufMemCp:待释放的的内存控制块<非空>
*               EGN_UINT32 *pulReleaseSize:释放的内存块大小<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution:
*        Since: V300R005C01
*    Reference: PFEgnSspMemReleaseBuf
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_UINT32 (*PFEgnSspMemReleaseBuf)
(
    INOUT EGN_VOID      *pvBufMemCp,
    OUT   EGN_UINT32    *pulReleaseSize
);

/*******************************************************************************
*    Func Name: PFEgnSspMemResetBuf
*      Purpose: 复位内存控制块
*  Description: 复位内存控制块回调函数
*        Input: NA
*        InOut: EGN_VOID *pvBufMemCp:待复位的的内存控制块<非空>
*               EGN_UINT32 *pulResetSize:复位的内存块大小<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution:
*        Since: V300R005C01
*    Reference: PFEgnSspMemResetBuf
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
 typedef EGN_UINT32 (*PFEgnSspMemResetBuf)
(
    INOUT EGN_VOID      *pvBufMemCp,
    OUT   EGN_UINT32    *pulResetSize
);

/*******************************************************************************
*    Func Name: PFEgnSspMemBufAlloc
*      Purpose: 创建Memcp的管理结构
*  Description: 创建Memcp的管理结构回调函数
*        Input: EGN_VOID *pBufCp:内存控制块<非空>
*               EGN_UINT32 ulSize:初始大小<大于零>
*        InOut: NA
*       Output: NA
*       Return: EGN_VOID，成功或者失败的错误码。
*      Caution:
*        Since: V300R005C01
*    Reference: PFEgnSspMemBufAlloc
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID * (*PFEgnSspMemBufAlloc)
(
   IN  EGN_VOID    *pvhandle,
   IN  EGN_UINT32   ulSize
);

/*******************************************************************************
*    Func Name: PFEgnSspMemBufFree
*      Purpose: 释放内存
*  Description: 释放Buffer内存，实际上并没有释放，仅提供接口
*        Input: EGN_VOID *pvDummy:需要释放的地址<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_VOID，成功或者失败的错误码。
*      Caution:
*        Since: V300R005C01
*    Reference: PFEgnSspMemBufFree
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_VOID (*PFEgnSspMemBufFree)
(
   IN  EGN_VOID *pvDummy
);

/*******************************************************************************
*    Func Name: PFEgnSspMemBufGetSize
*      Purpose: 查询指定内存控制块申请的内存总字节数
*  Description: 查询指定内存控制块申请的内存总字节数的回调函数
*        Input: EGN_VOID *pBufCp:内存控制块<非空>
*        InOut: NA
*       Output: EGN_UINT32 *pulSize:已经申请的内存总数(单位：Byte)<非空>
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution:
*        Since: V300R005C01
*    Reference: PFEgnSspMemBufGetSize
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_UINT32 (*PFEgnSspMemBufGetSize)
(
   IN  EGN_VOID    *pvhandle,
   OUT EGN_UINT32  *pulSize
);

 /*******************************************************************************
 *    Func Name: PFEgnAddPeerList
 *      Purpose: 添加关联下发接口
 *  Description: 下发关联信息添加的接口
 *        Input: EgnPeerListTupleInfo *pstTuple : 三元组信息与tunnelID合集<非空>
 *               EgnProtoResultInfo   *pstProtRetInfo : 识别结果，主要包括大类、小类、应用分类以及承载分类。<非空>
 *               EGN_VOID             *pvFlowHandle : 父会话信息（NGE的会话指针和会话版本）<非空>
 *               EGN_UINT16            usTimeOut : 老化时间，单位秒<若为全f，表示不需要老化。>
 *        InOut: NA
 *       Output: NA
 *       Return: EGN_VOID
 *      Caution:
 *        Since: V300R006C00
 *    Reference: PFEgnAddPeerList
 *------------------------------------------------------------------------------
 *  Modification History
 *  DATE         NAME                    DESCRIPTION
 *------------------------------------------------------------------------------
 *
*******************************************************************************/
 typedef EGN_VOID  (*PFEgnAddPeerList)
(
     IN EGN_CONST EgnPeerListTupleInfo *pstTuple,
     IN           EgnProtoResultInfo   *pstProtRetInfo,
     IN           EGN_VOID             *pvFlowHandle,
     IN           EGN_UINT16            usTimeOut
);

 /*******************************************************************************
 *    Func Name: PFEgnDelPeerList
 *      Purpose:  删除关联信息下发接口
 *  Description: 当ucIPNum与ucPortNum的值都为0时，删除所有协议ID为ulProtID的关联表项；
 *               当ucIPNum与ucPortNum都大于0时，删除协议ID为ulProtID，并且三元组中的ip和端口在分别的列表中的表项；
 *               当ucIPNum与ucPortNum其中一项大于0时，删除协议ID为ulProtID，并且三元组中对应ip或端口在列表中的表项。
 *        Input: EGN_UINT32     ulProtID    : 待删除的关联信息的分类结果。
                 EGN_UINT32     ulTunnelID  : Tunnel ID
                 EGN_UINT8      ucPortNum   : 端口条件数量<若为0，删除时不关注端口条件>
                 EgnPortCond   *pstPortList : 端口条件列表<若ucPortNum为0，删除时不关注端口条件。>
                 EGN_UINT8      ucIPNum     : IP条件数量<若为0，删除时不关注IP条件。>
                 EgnIpAddrCond *pstIPAddrList : IP条件列表<若ucIPNum为0，删除时不关注IP条件。>
 *        InOut: NA
 *       Output: NA
 *       Return: EGN_VOID
 *      Caution:
 *        Since: V300R006C00
 *    Reference: PFEgnDelPeerList
 *------------------------------------------------------------------------------
 *  Modification History
 *  DATE         NAME                    DESCRIPTION
 *------------------------------------------------------------------------------
 *
*******************************************************************************/
 typedef EGN_VOID  (*PFEgnDelPeerList)
(
    IN           EGN_UINT32   ulProtoID,
    IN           EGN_UINT32   ulTunnelID,
    IN           EGN_UINT8    ucPortNum,
    IN EGN_CONST EgnPortCond *pstPortList,
    IN           EGN_UINT8    ucIPNum,
    IN EGN_CONST EgnIpAddrCond   *pstIPAddrList
);

/* 需要NGFW产品注册的回调函数 */

/*******************************************************************************
 *    Func Name: PFEgnHAAddPattern
 *      Purpose: 添加模式串到硬件状态机中
 *  Description: 添加模式串到硬件状态机中
 *        Input: EGN_UINT8      ucHAPattSetID : 当前模式串要添加到的硬件模式串组ID
                 EGN_UINT8      ucRuleLibID   : 当前模式串所在的知识库ID
                 EGN_UCHAR     *pucPattern    : 当前要添加的模式串内容<非空>
                 EGN_UINT16     usPatternLen  : 模式串长度
                 EGN_UINT32     ulPatternID   : 模式串ID
                 EgnPatternAtt *pstPatternAtt : 模式串属性<非空>
 *        InOut: NA
 *       Output: NA
 *       Return: EGN_VOID
 *      Caution:
 *        Since: V300R006C00
 *    Reference:
 *------------------------------------------------------------------------------
 *  Modification History
 *  DATE         NAME                    DESCRIPTION
 *------------------------------------------------------------------------------
 *
*******************************************************************************/
typedef EGN_UINT32 (*PFEgnHAAddPattern)
(
    IN      EGN_UINT8       ucHAPattSetID,      /* 当前模式串要添加到的硬件模式串组ID */
    IN      EGN_UINT8       ucRuleLibID,        /* 当前模式串所在的知识库ID */
    IN      EGN_UCHAR      *pucPattern,         /* 当前要添加的模式串内容 */
    IN      EGN_UINT16      usPatternLen,       /* 模式串长度 */
    IN      EGN_UINT32      ulPatternID,        /* 模式串ID */
    IN      EgnPatternAtt  *pstPatternAtt       /* 模式串属性 */
);

/*******************************************************************************
 *    Func Name: PFEgnHADeletePatternAll
 *      Purpose: 删除指定模式串组中的所有模式串
 *  Description: 删除指定模式串组中的所有模式串
 *        Input: EGN_UINT8      ucHAPattSetID : 当前模式串要添加到的硬件模式串组ID
                 EGN_UINT8      ucRuleLibID   : 当前模式串所在的知识库ID
 *        InOut: NA
 *       Output: NA
 *       Return: EGN_VOID
 *      Caution:
 *        Since: V300R006C00
 *    Reference:
 *------------------------------------------------------------------------------
 *  Modification History
 *  DATE         NAME                    DESCRIPTION
 *------------------------------------------------------------------------------
 *
*******************************************************************************/
typedef EGN_UINT32 (*PFEgnHADeletePatternAll)
(
    IN      EGN_UINT8       ucHAPattSetID,      /* 当前模式串要添加到的硬件模式串组ID */
    IN      EGN_UINT8       ucRuleLibID         /* 当前模式串所在的知识库ID */
);

/*STRUCT< 系统服务功能的回调函数定义 >*/
typedef struct _EgnSspFunc
{
    EGN_UINT32              ulMagic;            /* SSP CBack magic,用户无需关注,请不要修改 */
#ifdef EGN_64
    EGN_UINT8               aucReserved[4];     /* 64位对齐保留字节 */
#endif
    PFEgnSspMemAlloc        pfMemAlloc;         /* 内存分配回调函数，pfMemAlloc和pfMemFree两个参数要么都注册，要么都不注册 可选 */
    PFEgnSspMemFree         pfMemFree;          /* 内存释放回调函数，pfMemAlloc和pfMemFree两个参数要么都注册，要么都不注册 可选 */
    PFEgnSspMemCreateBuf    pfMemCreateBuf;     /* 创建内存控制块函数 AR必须注册 */
    PFEgnSspMemReleaseBuf   pfMemReleaseBuf;    /* 释放内存控制块函数 AR必须注册 */
    PFEgnSspMemResetBuf     pfMemResetBuf;      /* 复位内存控制块函数 AR必须注册 */
    PFEgnSspMemBufAlloc     pfMemBufAlloc;      /* 创建Memcp的管理结构函数 AR必须注册 */
    PFEgnSspMemBufFree      pfMemBufFree;       /* 释放Buffer内存函数 AR必须注册 */
    PFEgnSspMemBufGetSize   pfMemBufGetSize;    /* 查询申请内存总数 AR必须注册 */
    PFEgnSspMemSet          pfMemSet;           /* 内存memset回调函数 必须 */
    PFEgnSspMemCpy          pfMemCpy;           /* 内存复制回调函数 必须 */
    PFEgnSspMemCmp          pfMemCmp;           /* 内存比较回调函数 必须 */
    PFEgnSspStrLen          pfStrLen;           /* 字符串长度计算回调函数 必须 */
    PFEgnSspFileOpen        pfFopen;            /* 打开文件回调函数 可选 */
    PFEgnSspFileSeek        pfFseek;            /* 移动文件指针回调函数 可选 */
    PFEgnSspFileRead        pfFread;            /* 读取文件回调函数 可选 */
    PFEgnSspFileTell        pfFtell;            /* 获取文件指针位置回调函数 可选 */
    PFEgnSspFileClose       pfFclose;           /* 关闭文件回调函数 可选 */
    PFEgnTimerGrpCreate     pfTimerGrpCreate;   /* 创建定时器组回调函数 必须 */
    PFEgnTimerGrpStart      pfTimerGrpStart;    /* 启动定时器回调函数 必须 */
    PFEgnTimerGrpStop       pfTimerGrpStop;     /* 停止定时器回调函数 必须 */
    PFEgnTimerGrpDestroy    pfTimerGrpDestroy;  /* 释放定时器组回调函数 必须 */
    PFEgnRWLockCreate       pfRWLCreate;        /* 创建锁回调函数 必须 */
    PFEgnRWLock             pfRWLWLock;         /* 加写锁回调函数 必须 */
    PFEgnRWUnlock           pfRWLWUnlock;       /* 解写锁回调函数 必须 */
    PFEgnRWLock             pfRWLRLock;         /* 加读锁回调函数 必须 */
    PFEgnRWUnlock           pfRWLRUnlock;       /* 解读锁回调函数 必须 */
    PFEgnRWLockDelete       pfRWLockDelete;     /* 删除读写锁回调函数 必须 */
    PFEgnSspStrNCmp         pfStrNCmp;          /* 字符串比较 必须 */
    PFEgnFuncPeerSynAdd     pfPeerSynAdd;       /* 关联表同步回调函数 可选 */

    PFEgnHAAddPattern       pfHAAddPattern;     /* 仅支持NGFW产品 硬件加速添加硬件模式串 */
    PFEgnHADeletePatternAll pfHADeletePatternAll; /* 仅支持NGFW产品 硬件加速删除全部模式串 */

    PFEgnAtomicAdd          pfAtomicAdd;        /* 原子加 必须 */
    PFEgnAtomicSub          pfAtomicSub;        /* 原子减 必须 */
    PFEgnFeedWatchdog       pfFeedWatchdog;     /* 看门狗复位 可选 */
    PFEgnIsInspectTimeOut   pfIsInspectTimeOut; /* 仅支持TD_RNC产品，识别时间是否超时 可选，默认必须为空 */
} EgnSspFunc;

/*STRUCT< 事件处理回调函数 >*/
typedef struct _EgnEventFunc
{
     EGN_UINT32              ulMagic;            /* event magic,用户无需关注,请不要修改 */
#ifdef EGN_64
     EGN_UINT8               aucReserved[4];     /* 64位对齐保留字节 */
#endif
    PFEgnAddPeerList      pfAddPeerList;    /* 添加关联下发处理 */
    PFEgnDelPeerList      pfDelPeerList;    /* 删除关联下发处理 */
} EgnEventFunc;

/*******************************************************************************
*    Func Name: EgnApiInitSspFuncParam
*      Purpose: 初始化指向系统服务回调函数的结构体的指针。
*  Description: 函数EgnApiInitSspFuncParam将结构体pstSspParam中每个回调函数的指针都赋值为空，
                避免某个回调函数的指针没有赋值，导致不可预知的错误。
*        Input: NA
*        InOut: EgnSspFunc* pstSspParam:系统服务参数<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution: 输入指针pstSspParam不能为空，否则初始化错误。
*        Since: V100R001C01
*    Reference: EgnApiRegSspFunc
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiInitSspFuncParam
(
    INOUT  EgnSspFunc *pstSspParam
);

/*******************************************************************************
*    Func Name: EgnApiRegSspFunc
*      Purpose: 注册系统服务回调函数到组件。
*  Description: 系统服务主要提供分配内存、释放内存、文件操作、定时器、线程的加解锁等功能。
                组件在需要使用时，调用外部提供的函数，完成基本的功能。例如操作文件时，调用外部提供的文件操作函数。
*        Input: EgnSspFunc* pstSspParam:系统服务回调函数<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution: 除特殊说明外，其他API应在成功调用该API之后才能被使用。
                调用EgnApiRegSspFunc注册系统服务回调函数前，先调用EgnApiInitSspFuncParam，
                使结构体pstSspParam中每个回调函数的指针都赋值为空，避免某个回调函数的指针没有赋值，
                导致不可预知的错误。
*        Since: V100R001C01
*    Reference: EgnApiInitSspFuncParam
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiRegSspFunc
(
    IN EgnSspFunc *pstSspParam
);

/*******************************************************************************
*    Func Name: EgnApiRegEventFunc
*      Purpose: 注册event回调函数到组件。
*  Description: Event主要包括关联添加、关联删除两个。
*        Input: EgnEventFunc* pstEventParam:Event回调函数<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution: 必须识别实例初始化之后才能调用。
*        Since: V300R006C00
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiRegEventFunc
(
    IN EgnEventFunc *pstEventParam
);

/*******************************************************************************
*    Func Name: EgnApiRegAssertFn
*      Purpose: 注册断言的回调函数。
*  Description: 注册断言回调函数后，可以通过断言输出回调函数查询完整的堆栈信息。
*        Input: PFEgnFuncAssert pCallbackFunc:<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution: 该API在使用EgnApiRegSspFunc成功注册系统服务回调函数之前可被调用。
*        Since: V100R001C01
*    Reference: PFEgnFuncAssert
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiRegAssertFn
(
    IN PFEgnFuncAssert  pfCallbackFunc
);

/*******************************************************************************
*    Func Name: EgnApiRegTraceSendFn
*      Purpose: 注册跟踪信息输出的回调函数。
*  Description: 注册跟踪信息输出回调函数后，可以通过跟踪信息输出回调函数打印跟踪信息。
*        Input: PFEgnFuncTraceSend pCallbackFunc:<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution: 该API在使用EgnApiRegSspFunc成功注册系统服务回调函数之前可被调用。
*        Since: V100R001C01
*    Reference: PFEgnFuncTraceSend
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiRegTraceSendFn
(
    IN PFEgnFuncTraceSend pfCallbackFunc
);

/*******************************************************************************
*    Func Name: EgnApiRegStatSendFn
*      Purpose: 注册统计信息输出的回调函数。
*  Description: 注册统计信息输出回调函数后，可以通过统计信息输出回调函数打印统计信息。
*        Input: PFEgnFuncStatSend pCallbackFunc:<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution: 该API在使用EgnApiRegSspFunc成功注册系统服务回调函数之前可被调用。
*        Since: V100R001C01
*    Reference: PFEgnFuncStatSend
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiRegStatSendFn
(
    IN PFEgnFuncStatSend pfCallbackFunc
);

/*******************************************************************************
*    Func Name: EgnApiRegLogSendFn
*      Purpose: 注册日志信息输出回调函数
*  Description: 注册日志信息输出回调函数后，可以通过日志输出回调函数打印日志信息。
*        Input: PFEgnFuncLogSend pCallbackFunc:日志回调函数<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution: 该API在使用EgnApiRegSspFunc成功注册系统服务回调函数之前可被调用。
*        Since: V100R001C01
*    Reference: PFEgnFuncLogSend
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiRegLogSendFn
(
    IN  PFEgnFuncLogSend  pfCallbackFunc
);

/*******************************************************************************
*    Func Name: EgnApiRegDebugPrintFn
*      Purpose: 注册调试信息输出的回调函数
*  Description: 注册调试信息输出的回调函数后，可以通过调试信息输出回调函数打印调试信息。
*        Input: PFEgnFuncDbgPrint pCallbackFunc:<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution: 该API在使用EgnApiRegSspFunc成功注册系统服务回调函数之前可被调用。
*        Since: V100R001C01
*    Reference: PFEgnFuncDbgPrint
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiRegDebugPrintFn
(
    IN  PFEgnFuncDbgPrint pfCallbackFunc
);

/*******************************************************************************
*    Func Name: EgnApiDebugControl
*      Purpose: 控制调试信息的输出
*  Description: 该接口根据调试信息输出控制开关ulCommand，控制调试信息是否输出以及输出信息的级别。
*        Input: EGN_UINT32 ulCommand:调试信息输出控制开关<EGN_DBGCMD_ONOFF_SWITCH(DEBUG开关)，EGN_DBGCMD_LEVEL_SWITCH(调试信息输出级别)>
*               EGN_UINT8* pucContent:对应开关值<ulCommand为EGN_DBGCMD_ONOFF_SWITCH时取EGN_DEBUG_ON/EGN_DEBUG_OFF
                                                ulCommand为EGN_DBGCMD_LEVEL_SWITCH时取EGN_DEBUG_LEVEL_FATAL~EGN_DEBUG_LEVEL_INFO>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution: 该API在使用管理线程初始化之后才可被调用。
*        Since: V200R002C01
*    Reference: EgnApiGetDebugControl
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiDebugControl
(
   IN               EGN_UINT32    ulCommand,
   IN  EGN_CONST    EGN_UINT8    *pucContent
);

/*******************************************************************************
*    Func Name: EgnApiGetDebugControl
*      Purpose: 查询调试信息是否输出以及输出信息的级别。
*  Description: 该接口根据调试信息输出控制开关ulCommand，查询调试信息是否输出以及输出信息的级别。
*        Input: EGN_UINT32 ulCommand:调试信息输出控制开关<EGN_DBGCMD_ONOFF_SWITCH(DEBUG开关)，EGN_DBGCMD_LEVEL_SWITCH(调试信息输出级别)>
*        InOut: NA
*       Output: EGN_UINT32* pucContent:调试信息<非空>
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution: 该API在使用管理线程初始化之后才可被调用。
*        Since: V100R001C01
*    Reference: EgnApiDebugControl
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetDebugControl
(
    IN    EGN_UINT32    ulCommand,
    OUT   EGN_UINT32   *pulContent
);

/*STRUCT< 维护接口定义 >*/
typedef struct _EgnSmpFunc
{
    PFEgnFuncAssert     pfEgnAssert;     /* 断言接口 */
    PFEgnFuncLogSend    pfEgnLogSend;    /* 输出日志接口 */
    PFEgnFuncStatSend   pfEgnStatSend;   /* 打点统计日志接口 */
    PFEgnFuncTraceSend  pfEgnTraceSend;  /* 输出跟踪接口 */
    PFEgnFuncDbgPrint   pfEgnDbgSend;    /* 输出调试信息接口 */
} EgnSmpFunc;

/*STRUCT< SMP参数配置定义 >*/
typedef struct _EgnSmpParam
{
    EGN_UINT32          ulDebugSwtich;   /* 调试开关,具体请参见#EGN_DBG_SWTICH_EN */
    EGN_UINT32          ulDebugLevel;    /* 调试等级，具体请参见#EGN_DEBUG_LEVEL_EN */
}EgnSmpParam;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EGN_API_SSP_SMP_H__ */

