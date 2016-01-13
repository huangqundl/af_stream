/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : Vpp_TimerGrp.h
  版 本 号   : 初稿
  作    者   : 无
  生成日期   : 无
  最近修改   :
  功能描述   : 定时器的数据结构，函数声明
  函数列表   :
  修改历史   :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

******************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

#ifndef __VPP_TimerGrp_H__
#define __VPP_TimerGrp_H__

#include "Vpp_typedef.h"

#define VPP_WIN32           1
#define VPP_SOLARIS_POSIX   2
#define VPP_AIX             3
#define VPP_VXWORKS         4
#define VPP_LINUX_POSIX     5
#define VPP_HP              6

#ifndef VPP_OS_VER
#error "must define VPP_OS_VER: VPP_WIN32, VPP_SOLARIS_POSIX, VPP_AIX, VPP_VXWORKS, VPP_LINUX_POSIX"
#endif


#ifdef VPP_OS_VER
#if ( VPP_OS_VER == VPP_WIN32 )
#include <windows.h>
#elif (VPP_OS_VER == VPP_SOLARIS_POSIX)
#include "pthread.h"
#elif (VPP_OS_VER == VPP_LINUX_POSIX)
#include "pthread.h"
#include "sys/time.h"
#elif (VPP_OS_VER == VPP_AIX)
#include "pthread.h"
#elif (VPP_OS_VER == VPP_HP)
#include "pthread.h"
#elif (VPP_OS_VER == VPP_VXWORKS)
#include "Time.h"
#include "intLib.h"
struct timeval
{
    long tv_sec;
    long tv_usec;
};
#endif
#endif /* VPP_OS_VER */


#ifdef VPP_OS_VER
#if ( VPP_OS_VER == VPP_WIN32 )
RTL_CRITICAL_SECTION VPP_CriticalSection;
#elif ( VPP_OS_VER == VPP_SOLARIS_POSIX || VPP_OS_VER == VPP_AIX || VPP_OS_VER == VPP_HP || VPP_OS_VER == VPP_LINUX_POSIX)
pthread_mutex_t VPP_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
#endif /* VPP_OS_VER */

#define G_SYSCALLBACK     g_stGlobal.stSysCallback;

#define G_RUNTIMERLIST    g_stGlobal.stRunTimerList;

#define VPP_MALLOC (*g_stGlobal.stSysCallback.pfMalloc)
#define VPP_FREE   (*g_stGlobal.stSysCallback.pfFree)
#define VPP_MEMCPY (*g_stGlobal.stSysCallback.pfMemcpy)
#define VPP_MEMSET (*g_stGlobal.stSysCallback.pfMemset)

/* [[SWED12582][2008-12-17][chenhongshan 103208]] Added begin: */
#ifdef VPP_DEBUG
extern int printf(const char *, ...);
#define VPP_DEBUG_HELP          0
#define VPP_DEBUG_TIMER_LIST    1
#define VPP_LOG    printf
#endif
/* [[SWED12582][2008-12-17][chenhongshan 103208]] Added end. */

/* [2007-11-01][add by c103208] begin: 增加初始化配置最大定时器数量接口 */
#define VPP_MIN_TIMER_NUM    20000           /* 最小定时器数量：2万 */
#define VPP_MAXRUNTIMER_NUM  g_maxtimer_num  /* 全局变量，保存最大定时器数 */
/* [2007-11-01][add by c103208] end: 增加初始化配置最大定时器数量接口 */

/* [[SWED12582][2008-12-17][chenhongshan]]  begin:TimerGrp 新建缓冲区管理，最大缓冲区大小*/
#define VPP_MAX_TIMEER_CACHE   512
/* [[SWED12582][2008-12-17][chenhongshan]]  end. */


typedef VPP_VOID (*PTVppFuncIterate)
(
    IN    PVPPTIMER*      ppTimer
);

typedef enum VPP_TIMER_STATE_E
{
    VPP_TIMER_IDLE,
    VPP_TIMER_TICKING,
    VPP_TIMER_STATEBUTT
} VPP_TIMER_STATE_EN;


typedef struct VPP_TIMERGRP_NODE_ST
{
    VPP_UINT32          ulTmIndex;
    VPP_UINT8           ucTmState;
    VPP_UINT8           ucTmMode;
    VPP_UINT16          usReserved;
    VPP_UINT32          ulTmLength;
    VPP_UINT32          ulTmName;
    VPP_UINT32          ulParam;
    VPP_UINT32          ulRunIndex;
    struct timeval      stNextTime; /* 下一次超时时间 */
    PTVppFuncTimeout    pfTimeout;
    PVPPTIMER*          ppTimer;     /* 记录本定时器在定时器组中的位置 */
}VPP_TIMERGRP_NODE_ST;

typedef struct VPP_TIMERGRP_UNIT_S
{
    VPP_UINT32                  ulMaxNum;
    VPP_ADDRESS                  *pData;
    struct VPP_TIMERGRP_UNIT_S  *pNextUnit;
}VPP_TIMERGRP_UNIT_ST;

typedef struct VPP_TIMERGRP_S
{
    VPP_UINT32                  ulMaxNum;
    PTVppFuncTimeout            pfTimeout;
    VPP_TIMERGRP_UNIT_ST        *pFirstTimerGrpUnit; /* 定时器组单元队列头指针 */
    struct VPP_TIMERGRP_S       *pNextGrp;
    struct VPP_TIMERGRP_S       *pPrevGrp;
}VPP_TIMERGRP_ST,*P_VPP_TIMERGRP_ST;


typedef struct VPP_TIMERGRP_LIST_S
{
    VPP_UINT32                  ulCount;
    VPP_ADDRESS                 *pArray;
} VPP_TIMERGRP_LIST_ST;

typedef struct VPP_TIMERGRP_GLOBAL_S
{
    VPP_UINT32                   ulInitFlag;
    VPP_TIMERGRP_SYS_CALLBACK_ST stSysCallback;
    VPP_TIMERGRP_CONFIG_ST       stConfig;
    VPP_TIMERGRP_LIST_ST         stRunTimerList;
    VPP_TIMERGRP_ST             *pGrpTimerList;  /*定时器组队列，指向定时器组队列头指针*/

    /* [[SWED12582][2008-12-17][chenhongshan]]  begin: TimerGrp缓冲区管理*/
    PVPPTIMER                    ahTimerBuffer[VPP_MAX_TIMEER_CACHE];/*定义缓冲区*/
    VPP_UINT32                   ulFreeCount;                        /*定义目前缓冲区中有效空闲块数量*/
    /* [[SWED12582][2008-12-17][chenhongshan]]  end. */

} VPP_TIMERGRP_GLOBAL_ST;

/*****************************************************************************
 函 数 名  : VppTimerGrpCreateUnit
 功能描述  : 申请定时器组单元。
 输入参数  : ulMaxTimerNums  当前定时器组单元内定时器的最大数目
 输出参数  : ppTimerGrpUnit  定时器组单元句柄指针，内存由定时器组模块分配。
 返 回 值  : 正确执行返回VPP_SUCCESS，否则返回值大于零的错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerGrpCreateUnit
(
    OUT   VPP_TIMERGRP_UNIT_ST**    ppTimerGrpUnit,
    IN    VPP_UINT32                ulMaxTimerNums
);

/*****************************************************************************
 函 数 名  : VppTimerStart
 功能描述  : 启动某个定时器
 输入参数  : ulTmName     定时器名字
             ulTmLength   定时器长度，单位是毫秒
             ucTmMode     定时器的工作模式：VPP_TIMER_NOLOOP（不循环）或 VPP_TIMER_LOOP（循环）
             pfTimeout    定时器对应的超时处理函数
 输出参数  : ppstTimer    定时器指针
 返 回 值  : 正确执行返回VPP_SUCCESS，否则返回值大于零的错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerStart
(
    OUT  PVPPTIMER*              ppstTimer,
    IN   VPP_UINT32              ulTmLength,
    IN   VPP_UINT32              ulTmName,
    IN   VPP_UINT32              ulTmParam,
    IN   VPP_UINT8               ucTmMode,
    IN   PTVppFuncTimeout        pfTimeout
);

/*****************************************************************************
 函 数 名  : VppTimerStop
 功能描述  : 停止某个定时器
 输入参数  : ppstTimer    定时器句柄指针
 输出参数  : 无
 返 回 值  : 正确执行返回VPP_SUCCESS，否则返回值大于零的错误码
 调用函数  : VppTimerGrpListDeleteNode
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerStop
(
    IN  PVPPTIMER*   ppstTimer
);

/*****************************************************************************
 函 数 名  : VppTimerGrpListAddNode
 功能描述  : 将一个定时器插入运行定时器队列中
 输入参数  : pstTimerNode    定时器句柄指针
 输出参数  : 无
 返 回 值  : 正确执行返回VPP_SUCCESS，否则返回值大于零的错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerGrpListAddNode
(
    IN  PVPPTIMER    pstTimerNode
);

/*****************************************************************************
 函 数 名  : VppTimerGrpListDeleteNode
 功能描述  : 从定时器队列中删除一个定时器
 输入参数  : pstTimerNode    定时器句柄指针
 输出参数  : 无
 返 回 值  : 正确执行返回VPP_SUCCESS，否则返回值大于零的错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerGrpListDeleteNode
(
    IN  PVPPTIMER    pstTimerNode
);

/*****************************************************************************
 函 数 名  : Vpp_InitLock
 功能描述  : 初始化信号量
 输入参数  : void
 输出参数  : 无
 返 回 值  : VPP_INT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_INT32 Vpp_InitLock( void );

/*****************************************************************************
 函 数 名  : Vpp_Lock
 功能描述  : 申请信号量
 输入参数  : VPP_INT *pLockKey
 输出参数  : 无
 返 回 值  : VPP_INT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_INT32 Vpp_Lock(VPP_INT *pLockKey);

/*****************************************************************************
 函 数 名  : Vpp_Unlock
 功能描述  : 释放信号量
 输入参数  : VPP_INT lockKey
 输出参数  : 无
 返 回 值  : VPP_INT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_INT32 Vpp_Unlock(VPP_INT lockKey);

/*****************************************************************************
 函 数 名  : Vpp_GetTime
 功能描述  : 获取当前时间
 输入参数  : struct timeval *pstTime
 输出参数  : 无
 返 回 值  : VPP_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检
*****************************************************************************/
VPP_VOID Vpp_GetTime(struct timeval *pstTime);

/*****************************************************************************
 函 数 名  : VppTimerGrpGet
 功能描述  : 获取定时器组中的一个定时器
 输入参数  : IN VPP_TIMERGRP_ST*         pTimerGrp
             IN VPP_UINT32               ulTmSn
             OUT PVPPTIMER**             pppTimer
 输出参数  : 无
 返 回 值  : VPP_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerGrpGet
(
    IN VPP_TIMERGRP_ST*         pTimerGrp,
    IN VPP_UINT32               ulTmSn,
    OUT PVPPTIMER**             ppTimer
);

/*****************************************************************************
 函 数 名  : VppTimerGrpSet
 功能描述  : 设置定时器组中一个定时器的属性
 输入参数  : IN VPP_TIMERGRP_ST*         pTimerGrp
             IN VPP_UINT32               ulTmSn
             OUT VPP_VOID*               pTimer
 输出参数  : 无
 返 回 值  : VPP_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerGrpSet
(
    IN VPP_TIMERGRP_ST*         pTimerGrp,
    IN VPP_UINT32               ulTmSn,
    OUT VPP_VOID*               pTimer
);

/*****************************************************************************
 函 数 名  : VppTimerGrpIterateStop
 功能描述  : 停止定时器
 输入参数  : PVPPTIMER       *ppTimer
 输出参数  : 无
 返 回 值  : VPP_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_VOID VppTimerGrpIterateStop
(
    PVPPTIMER       *ppTimer
);

/*****************************************************************************
 函 数 名  : VppTimerGrpIterateFree
 功能描述  : 释放定时器
 输入参数  : PVPPTIMER       *ppTimer
 输出参数  : 无
 返 回 值  : VPP_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_VOID VppTimerGrpIterateFree
(
    PVPPTIMER       *ppTimer
);

/*****************************************************************************
 函 数 名  : VppTimerGrpIterate
 功能描述  : 使用回调函数处理定时器组
 输入参数  : pTimerGrp  要处理的定时器组
             pfIterate  回调函数指针
 输出参数  : 无
 返 回 值  : VPP_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerGrpIterate
(
    IN VPP_TIMERGRP_ST*        pTimerGrp,
    IN PTVppFuncIterate        pfIterate
);

/* [[SWED13909] [2009-01-12] [Z00108581]] 【业软新需求 200812270221】TimerGropu组件需要提供支持suse10 64位系统 BEGIN */
#define Vpp_ExchangeValue(a, b, type) \
{\
    type tmp;\
    tmp = a;\
    a = b;\
    b = tmp;\
}
/* [SWED13909] [2009-01-12] [Z00108581]   End */

/*****************************************************************************
 函 数 名  : Vpp_AddToBinTree
 功能描述  : 将定时器接结点插入Bin树
 输入参数  : PVPPTIMER  pstTimerNode
             VPP_UINT32 ulIndex
             VPP_UINT32 *pTreeArray
 输出参数  : 无
 返 回 值  : VPP_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_VOID Vpp_AddToBinTree
(
    IN  PVPPTIMER  pstTimerNode,
    IN  VPP_UINT32 ulIndex,
    IO  VPP_ADDRESS *pTreeArray
);


/*****************************************************************************
 函 数 名  : VppTimerReset
 功能描述  : 重新启动某个定时器
 输入参数  : ppstTimer       定时器句柄指针
 输出参数  : 无
 返 回 值  : 正确执行返回VPP_SUCCESS，否则返回值大于零的错误码
 调用函数  : VppTimerStart
             VppTimerStop
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerReset
(
    IN  PVPPTIMER*   ppstTimer
);

/*****************************************************************************
 函 数 名  : VppTimerGrpAllocTimer
 功能描述  : 从缓冲区中取出空闲的一个节点，或直接申请内存
 输入参数  : PVPPTIMER  pstTimer
 输出参数  : 无
 返 回 值  : VPP_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2008年12月17日
    作    者   : 陈宏山 00103208
    修改内容   : 新增函数

*****************************************************************************/
VPP_VOID VppTimerGrpAllocTimer
(
    PVPPTIMER  *pstTimer
);

/*****************************************************************************
 函 数 名  : VppTimerGrpFreeTimer
 功能描述  : 将一个定时器放回缓冲区中，或直接释放内存
 输入参数  : PVPPTIMER  pstTimer
 输出参数  : 无
 返 回 值  : VPP_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2008年12月17日
    作    者   : 陈宏山 00103208
    修改内容   : 新增函数

*****************************************************************************/
VPP_VOID VppTimerGrpFreeTimer
(
    PVPPTIMER  *pstTimer
);


#endif /*__VPP_TimerGrp_H__*/

#ifdef __cplusplus
}
#endif
