/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : Vpp_TimerGrp.c
  版 本 号   : 初稿
  作    者   : 无
  生成日期   : 无
  最近修改   :
  功能描述   : 定时器功能函数
  函数列表   :
              Vpp_AddToBinTree
              Vpp_ExchangeValue
              Vpp_GetTime
              Vpp_InitLock
              Vpp_Lock
              Vpp_Unlock
              VppTimerGrpCreate
              VppTimerGrpCreateUnit
              VppTimerGrpDestroy
              VppTimerGrpGet
              VppTimerGrpIsRun
              VppTimerGrpIterate
              VppTimerGrpIterateFree
              VppTimerGrpIterateStop
              VppTimerGrpListAddNode
              VppTimerGrpListDeleteNode
              VppTimerGrpModuleInit
              VppTimerGrpMoudleDeInit
              VppTimerGrpReset
              VppTimerGrpResize
              VppTimerGrpSet
              VppTimerGrpStart
              VppTimerGrpStop
              VppTimerGrpTrigger
              VppTimerRestart
              VppTimerStart
              VppTimerStop
  修改历史   :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

******************************************************************************/

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
#define VPP_OS_VER VPP_LINUX_POSIX
#include "Vpp_TimerGrp.h"

/* [2007-11-01][add by c103208] begin: 增加初始化配置最大定时器数量接口 */
VPP_UINT32 g_maxtimer_num = 200000;   /* 默认最大定时器数量20万 */
/* [2007-11-01][add by c103208] end: 增加初始化配置最大定时器数量接口 */


VPP_TIMERGRP_GLOBAL_ST     g_stGlobal;

/*****************************************************************************
 函 数 名  : VppTimerSetMaxTimerNum
 功能描述  : 初始化设置可用的最大定时器数，必须大于等于20000个
             初始化后不能调用本接口，默认最大定时器数量20万。
 输入参数  : IN VPP_UINT32 ulMaxTimerNum 最大定时器数
 输出参数  : 无
 返 回 值  : VPP_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2007年11月02日
    作    者   : 李玉章 37398
    修改内容   : 新生成函数。

*****************************************************************************/
VPP_UINT32   VppTimerSetMaxTimerNum
(
    IN VPP_UINT32 ulMaxTimerNum
)
{
    if (ulMaxTimerNum < VPP_MIN_TIMER_NUM)
    {
        return VPP_FAILURE;
    }

    if(g_stGlobal.ulInitFlag == VPP_FALSE)
    {
        g_maxtimer_num = ulMaxTimerNum;
    }
    else
    {
        return VPP_ERR_REINIT;
    }

    return VPP_SUCCESS;

}

/*****************************************************************************
 函 数 名  : VppTimerGrpModuleInit
 功能描述  : 初始化定时器组模块，配置相关参数、注册回调函数指针?
 输入参数  : *pstSysCallBack
             *pstConfig
 输出参数  : 无
 返 回 值  : 正确执行返回VPP_SUCCESS，否则返回值大于零的错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerGrpModuleInit
(
    IN  VPP_TIMERGRP_SYS_CALLBACK_ST     *pstSysCallBack,
    IN  VPP_TIMERGRP_CONFIG_ST           *pstConfig
)
{
    if ((VPP_NULL_PTR == pstSysCallBack) || (VPP_NULL_PTR == pstConfig))
    {
        return VPP_ERR_INPUTNULL;
    }

    if ((VPP_NULL_PTR == pstSysCallBack->pfMalloc)
       ||(VPP_NULL_PTR == pstSysCallBack->pfFree)
       ||(VPP_NULL_PTR == pstSysCallBack->pfMemcpy)
       ||(VPP_NULL_PTR == pstSysCallBack->pfMemset))
    {
        return VPP_ERR_INPUTNULL;
    }

    if(g_stGlobal.ulInitFlag == VPP_FALSE)
    {
        /* [[SWED13909] [2009-01-15] [Z00108581]] 【业软新需求 200812270221】TimerGropu组件需要提供支持suse10 64位系统 BEGIN */
        /* Fix Review problem, InitLock Fail, then Module can't be inited again */
        if (VPP_SUCCESS != Vpp_InitLock())
        {
            return VPP_ERR_INITLOCK;
        }
        /* [SWED13909] [2009-01-15] [Z00108581]   End */

        g_stGlobal.ulInitFlag = VPP_TRUE;

        /*将回调函数注册到定时器模块,保存相应的回调函数指针到g_stGlobal*/
        g_stGlobal.stSysCallback.pfMalloc = pstSysCallBack->pfMalloc;
        g_stGlobal.stSysCallback.pfFree = pstSysCallBack->pfFree;
        g_stGlobal.stSysCallback.pfMemcpy = pstSysCallBack->pfMemcpy;
        g_stGlobal.stSysCallback.pfMemset = pstSysCallBack->pfMemset;

        /* 配置参数 */
        g_stGlobal.stConfig.ulTicksPerSecond = pstConfig->ulTicksPerSecond;

        /*初始化定时器运行队列g_stGlobal.stRunTimerList;*/
        g_stGlobal.stRunTimerList.ulCount = 0;
        g_stGlobal.stRunTimerList.pArray = VPP_NULL_PTR;

        g_stGlobal.pGrpTimerList = VPP_NULL_PTR;

        /* Fix Review problem, Free CB Num In Cache must Init to zero */
        g_stGlobal.ulFreeCount = 0;

    }
    else
    {
        return VPP_ERR_REINIT;
    }

    pstSysCallBack = pstSysCallBack;/* for pc_lint */
    pstConfig = pstConfig;/* for pc_lint */
    return VPP_SUCCESS;
}

/*****************************************************************************
 函 数 名  : VppTimerGrpCreate
 功能描述  : 申请定时器组，包含多个定时器组单元。
 输入参数  : ulMaxTimerNums  定时器组内定时器的最大数目
             pfTimeout       定时器超时处理回调函数指针
 输出参数  : ppTimerGrp      定时器组句柄指针，内存由定时器组模块分配。APP必须保存该指针
 返 回 值  : 正确执行返回VPP_SUCCESS，否则返回值大于零的错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerGrpCreate
(
    OUT  VPP_VOID**              ppTimerGrp,
    IN   VPP_UINT32              ulMaxTimerNums,
    IN   PTVppFuncTimeout        pfTimeout
)
{
    VPP_TIMERGRP_ST         *pTempTimerGrp = VPP_NULL_PTR;
    VPP_TIMERGRP_UNIT_ST    *pTimerGrpUnit = VPP_NULL_PTR;
    VPP_TIMERGRP_ST         *pTimerGrpLast = VPP_NULL_PTR;
    VPP_UINT32              ulRetVal;

    if(VPP_NULL_PTR == ppTimerGrp)
    {
        return VPP_ERR_INPUTNULL;
    }

    /* 输入参数定时器组个数为0 */
    if(0 == ulMaxTimerNums)
    {
        return VPP_ERR_TIMERNUMS;
    }

    /* 输入参数pfTimeout为空 */
    if (VPP_NULL_PTR == pfTimeout)
    {
        return VPP_ERR_INPUTNULL;
    }

    /*申请定时器组资源，给定时器组分配内存*/
    pTempTimerGrp = (VPP_TIMERGRP_ST*)(VPP_VOID*)malloc(sizeof(VPP_TIMERGRP_ST));
    if (VPP_NULL_PTR == pTempTimerGrp)
    {
        return VPP_ERR_MALLOCFAIL;
    }

    /* 初始化定时器组资源,将输入参数保存到定时器组结构中*/
    memset(pTempTimerGrp,0,sizeof(VPP_TIMERGRP_ST));

    ulRetVal = VppTimerGrpCreateUnit(&pTimerGrpUnit, ulMaxTimerNums);
    if(ulRetVal != VPP_SUCCESS)
    {
        VPP_FREE(pTempTimerGrp);
        pTempTimerGrp = VPP_NULL_PTR;
        return ulRetVal;
    }

    pTempTimerGrp->pFirstTimerGrpUnit = pTimerGrpUnit;
    pTempTimerGrp->ulMaxNum = ulMaxTimerNums;
    pTempTimerGrp->pfTimeout = pfTimeout;

    /* 输出定时器组句柄指针*/
    *ppTimerGrp = pTempTimerGrp;

    pTimerGrpLast = g_stGlobal.pGrpTimerList;
    if(pTimerGrpLast == VPP_NULL_PTR)
    {
        g_stGlobal.pGrpTimerList = pTempTimerGrp;
    }
    else
    {
        while(pTimerGrpLast->pNextGrp != VPP_NULL_PTR)
        {
            pTimerGrpLast = pTimerGrpLast->pNextGrp;
        }

        pTimerGrpLast->pNextGrp = pTempTimerGrp;
        pTempTimerGrp->pPrevGrp = pTimerGrpLast;
    }

    return VPP_SUCCESS;
}

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
)
{
    VPP_TIMERGRP_UNIT_ST        *pTimerGrpUnit;
    VPP_UINT32                  ulUnitSize;

    if((ppTimerGrpUnit == VPP_NULL_PTR)
    ||(*ppTimerGrpUnit != VPP_NULL_PTR))
    {
        return VPP_ERR_INPUTNULL;
    }

    pTimerGrpUnit = (VPP_TIMERGRP_UNIT_ST*)(VPP_VOID*)malloc(sizeof(VPP_TIMERGRP_UNIT_ST));
    if(pTimerGrpUnit == VPP_NULL_PTR)
    {
        return VPP_ERR_MALLOCFAIL;
    }

    memset(pTimerGrpUnit, 0, sizeof(VPP_TIMERGRP_UNIT_ST));

    ulUnitSize = sizeof(VPP_ADDRESS);
    pTimerGrpUnit->pData = (VPP_ADDRESS*)(VPP_VOID*)malloc(ulUnitSize * ulMaxTimerNums);
    if(pTimerGrpUnit->pData == VPP_NULL_PTR)
    {
        VPP_FREE(pTimerGrpUnit);
        pTimerGrpUnit = VPP_NULL_PTR;
        return VPP_ERR_MALLOCFAIL;
    }

    memset(pTimerGrpUnit->pData, 0, ulUnitSize * ulMaxTimerNums);

    pTimerGrpUnit->pNextUnit = VPP_NULL_PTR;
    pTimerGrpUnit->ulMaxNum = ulMaxTimerNums;

    *ppTimerGrpUnit = pTimerGrpUnit;

    return VPP_SUCCESS;
}

/*****************************************************************************
 函 数 名  : VppTimerGrpResize
 功能描述  : 扩大定时器组中定时器的个数。
 输入参数  : ulNewSize       当前定时器组需要的定时器的最大数目
             pTimerGrp       定时器组句柄指针
 输出参数  : pTimerGrp       定时器组句柄指针，新增的定时器内存由定时器组模块分配?
 返 回 值  : 正确执行返回VPP_SUCCESS，否则返回值大于零的错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerGrpResize
(
    IO  VPP_VOID*               pTimerGrp,
    IN  VPP_UINT32              ulNewSize
)
{
    VPP_TIMERGRP_ST         *pTempTimerGrp;
    VPP_TIMERGRP_UNIT_ST    *pTimerGrpUnit = VPP_NULL_PTR;
    VPP_TIMERGRP_UNIT_ST    *pTimerGrpUnitTmp = VPP_NULL_PTR;
    VPP_UINT32              ulAddSize;
    VPP_UINT32              ulRetValue;
    VPP_INT                 lockKey = 0;

    if(pTimerGrp == VPP_NULL_PTR)
    {
        return VPP_ERR_INPUTNULL;
    }

    pTempTimerGrp = (VPP_TIMERGRP_ST*)pTimerGrp;
    if (VPP_SUCCESS != Vpp_Lock(&lockKey))
    {
        return VPP_ERR_LOCK;
    }

    if(pTempTimerGrp->ulMaxNum < ulNewSize)
    {
        /* 如果要变大 */
        ulAddSize = ulNewSize - pTempTimerGrp->ulMaxNum;

        pTimerGrpUnitTmp = pTempTimerGrp->pFirstTimerGrpUnit;
        while(pTimerGrpUnitTmp->pNextUnit != VPP_NULL_PTR)
        {
            pTimerGrpUnitTmp = pTimerGrpUnitTmp->pNextUnit;
        }

        ulRetValue = VppTimerGrpCreateUnit(&pTimerGrpUnit, ulAddSize);
        if(ulRetValue != VPP_SUCCESS)
        {
            if (VPP_SUCCESS != Vpp_Unlock(lockKey))
            {
                return VPP_ERR_UNLOCK;
            }
            return ulRetValue;
        }

        pTimerGrpUnitTmp->pNextUnit = pTimerGrpUnit;
    }
    else if(pTempTimerGrp->ulMaxNum > ulNewSize)
    {
        if (VPP_SUCCESS != Vpp_Unlock(lockKey))
        {
            return VPP_ERR_UNLOCK;
        }
        /* 暂时不实现 */
        return VPP_ERR_RESIZE;
    }

    pTempTimerGrp->ulMaxNum = ulNewSize;

    if (VPP_SUCCESS != Vpp_Unlock(lockKey))
    {
        return VPP_ERR_UNLOCK;
    }

    return VPP_SUCCESS;
}

/*****************************************************************************
 函 数 名  : VppTimerGrpUnlockDestroy
 功能描述  : 没有锁的删除TimerGrp函数
 输入参数  : IO   VPP_VOID**              ppTimerGrp
 输出参数  : 无
 返 回 值  : VPP_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2007年12月11日
    作    者   : weihongkang
    修改内容   : 新生成函数

*****************************************************************************/
VPP_UINT32 VppTimerGrpUnlockDestroy
(
    IO   VPP_VOID**              ppTimerGrp
)
{
    VPP_TIMERGRP_ST         *pTempTimerGrp = VPP_NULL_PTR;
    VPP_TIMERGRP_UNIT_ST    *pTimerGrpUnit = VPP_NULL_PTR;
    VPP_TIMERGRP_UNIT_ST    *pTimerGrpUnitNext = VPP_NULL_PTR;
    VPP_UINT32              ulRetValue;

    if(VPP_NULL_PTR == ppTimerGrp)
    {
        return VPP_ERR_INPUTNULL;
    }

    pTempTimerGrp = (VPP_TIMERGRP_ST*)*ppTimerGrp;

    /* 停止所有正在运行的定时器 */
    ulRetValue = VppTimerGrpIterate(pTempTimerGrp, VppTimerGrpIterateStop);
    if(VPP_SUCCESS != ulRetValue)
    {
        return ulRetValue;
    }

    /* 释放定时器组结构中所有指针;
    清空所有数据;*/
    pTempTimerGrp->ulMaxNum = 0;
    pTempTimerGrp->pfTimeout = VPP_NULL_PTR;

    pTimerGrpUnit = pTempTimerGrp->pFirstTimerGrpUnit;
    while(pTimerGrpUnit != VPP_NULL_PTR)
    {
        pTimerGrpUnitNext = pTimerGrpUnit->pNextUnit;
        VPP_FREE(pTimerGrpUnit->pData);
        pTimerGrpUnit->pData = VPP_NULL_PTR;
        VPP_FREE(pTimerGrpUnit);
        pTimerGrpUnit = pTimerGrpUnitNext;
    }

    if(pTempTimerGrp->pPrevGrp == VPP_NULL_PTR)
    {
        if(pTempTimerGrp->pNextGrp == VPP_NULL_PTR)
        {
            g_stGlobal.pGrpTimerList = VPP_NULL_PTR;
        }
        else
        {
            pTempTimerGrp->pNextGrp->pPrevGrp = VPP_NULL_PTR;
            g_stGlobal.pGrpTimerList = pTempTimerGrp->pNextGrp;
        }
    }
    else
    {
        pTempTimerGrp->pPrevGrp->pNextGrp = pTempTimerGrp->pNextGrp;
        if(pTempTimerGrp->pNextGrp != VPP_NULL_PTR)
        {
            pTempTimerGrp->pNextGrp->pPrevGrp = pTempTimerGrp->pPrevGrp;
        }
    }

    VPP_FREE(pTempTimerGrp);
    *ppTimerGrp = VPP_NULL_PTR;

    return VPP_SUCCESS;
}

/*****************************************************************************
 函 数 名  : VppTimerGrpDestroy
 功能描述  : 释放定时器组中所有定时器
 输入参数  : ppTimerGrp    定时器组句柄指针
 输出参数  : 无
 返 回 值  : 正确执行返回VPP_SUCCESS，否则返回值大于零的错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerGrpDestroy
(
    IO   VPP_VOID**  ppTimerGrp
)
{
    VPP_UINT32              ulRetValue;
    VPP_INT                 lockKey = 0;

    ppTimerGrp = ppTimerGrp;
    if(VPP_NULL_PTR == ppTimerGrp)
    {
        return VPP_ERR_INPUTNULL;
    }

    if (VPP_SUCCESS != Vpp_Lock(&lockKey))
    {
        return VPP_ERR_LOCK;
    }

    ulRetValue = VppTimerGrpUnlockDestroy(ppTimerGrp);

    if (VPP_SUCCESS != Vpp_Unlock(lockKey))
    {
        return VPP_ERR_UNLOCK;
    }

    return ulRetValue;
}

/*****************************************************************************
 函 数 名  : VppTimerGrpMoudleDeInit
 功能描述  : 注销定时器组模块，删除所有定时器组
 输出参数  : 无
 返 回 值  : 正确执行返回VPP_SUCCESS，否则返回值大于零的错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerGrpMoudleDeInit()
{
    VPP_TIMERGRP_ST     *pTimerGrp = VPP_NULL_PTR;
    VPP_UINT32          ulRetValue;

    pTimerGrp = g_stGlobal.pGrpTimerList;
    while(pTimerGrp)
    {
        ulRetValue = VppTimerGrpUnlockDestroy((VPP_VOID**)(VPP_ADDRESS)(&pTimerGrp));
        if(ulRetValue != VPP_SUCCESS)
        {
            return ulRetValue;
        }

        pTimerGrp = g_stGlobal.pGrpTimerList;
    }

    /* [[SWED12582][2008-12-17][chenhongshan]]begin: 去初始化时，释放缓冲区的定时器资源*/
    while (0 < g_stGlobal.ulFreeCount)
    {
        g_stGlobal.ulFreeCount--;
        VPP_FREE(g_stGlobal.ahTimerBuffer[g_stGlobal.ulFreeCount]);
    }
    /* [[SWED12582][2008-12-17][chenhongshan]]end. */

    /* 清空运行队列 */
    if (g_stGlobal.stRunTimerList.pArray != VPP_NULL_PTR)
    {
        VPP_FREE(g_stGlobal.stRunTimerList.pArray);
        g_stGlobal.stRunTimerList.pArray = VPP_NULL_PTR;
    }
    g_stGlobal.stRunTimerList.ulCount = 0;

    memset(&g_stGlobal, 0, sizeof(VPP_TIMERGRP_GLOBAL_ST));

    return VPP_SUCCESS;
}

/*****************************************************************************
 函 数 名  : VppTimerGrpStart
 功能描述  : 启动定时器组中的某个定时器
 输入参数  : pucTimeGrp    定时器组句柄指针
             ulTmSn        要启动的相对定时器的序号
             ulTmName      定时器名字
             ulTmLength    定时器长度，单位是毫秒
             ucTmMode      定时器的工作模式：VPP_TIMER_NOLOOP（不循环）或 VPP_TIMER_LOOP（循环）
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
VPP_UINT32 VppTimerGrpStart
(
    IN  VPP_VOID*               pTimerGrp,
    IN  VPP_UINT32              ulTmSn,
    IN  VPP_UINT32              ulTmLength,
    IN  VPP_UINT32              ulTmName,
    IN  VPP_UINT8               ucTmMode
)
{
    VPP_TIMERGRP_ST*    pTempTimerGrp = (VPP_TIMERGRP_ST*)pTimerGrp;
    PVPPTIMER*          ppTempTimer = VPP_NULL_PTR;
    VPP_UINT32          ulReturnVal   = 0;
    VPP_INT             lockKey = 0;

    if(VPP_NULL_PTR == pTempTimerGrp)
    {
        return VPP_ERR_INPUTNULL;
    }

    /*if(0 == ulTmLength)
    {
        return VPP_ERR_TIMEOUTLEN;
    }*/

    if(ulTmSn >= pTempTimerGrp->ulMaxNum)
    {
        return VPP_ERR_TIMERGRPSN;
    }

    if((VPP_TIMER_LOOP != ucTmMode)&&(VPP_TIMER_NOLOOP != ucTmMode))
    {
        return VPP_ERR_TIMERMODE;
    }

    if (VPP_SUCCESS != Vpp_Lock(&lockKey))
    {
        return VPP_ERR_LOCK;
    }
    ulReturnVal = VppTimerGrpGet(pTempTimerGrp, ulTmSn, &ppTempTimer);
    if((ulReturnVal == VPP_SUCCESS)&&(VPP_NULL_PTR != *ppTempTimer))
    {
        if (VPP_TIMER_TICKING == (*ppTempTimer)->ucTmState)
        {
            /* 停止该定时器*/
            ulReturnVal = VppTimerStop(ppTempTimer);
            if(VPP_SUCCESS != ulReturnVal)
            {
                if (VPP_SUCCESS != Vpp_Unlock(lockKey))
                {
                    return VPP_ERR_UNLOCK;
                }
                return ulReturnVal;
            }
        }
        *ppTempTimer = VPP_NULL_PTR;
    }

    /* 启动该定时器*/
    ulReturnVal = VppTimerStart(ppTempTimer,ulTmLength,ulTmName,ulTmSn,ucTmMode,pTempTimerGrp->pfTimeout);
    if(ulReturnVal != VPP_SUCCESS)
    {
        if (VPP_SUCCESS != Vpp_Unlock(lockKey))
        {
            return VPP_ERR_UNLOCK;
        }
        return ulReturnVal;
    }

    if (VPP_SUCCESS != Vpp_Unlock(lockKey))
    {
        return VPP_ERR_UNLOCK;
    }
    return VPP_SUCCESS;
}

/*****************************************************************************
 函 数 名  : VppTimerGrpIsRun
 功能描述  : 查询对应的定时器是否在运行
 输入参数  : pucTimeGrp    定时器组句柄指针
             ulTmSn        要启动的相对定时器的序号

 输出参数  : pRunFlag      当前是否在运行的标识
 返 回 值  : 正确执行返回VPP_SUCCESS，否则返回值大于零的错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerGrpIsRun
(
    IN  VPP_VOID*               pTimerGrp,
    IN  VPP_UINT32              ulTmSn,
    OUT VPP_UINT32*             pRunFlag
)
{
    VPP_TIMERGRP_ST*    pTempTimerGrp = (VPP_TIMERGRP_ST*)pTimerGrp;
    PVPPTIMER*          ppTempTimer = VPP_NULL_PTR;
    VPP_UINT32          ulReturnVal   = 0;
    VPP_INT             lockKey = 0;

    if((VPP_NULL_PTR == pTempTimerGrp)||(VPP_NULL_PTR == pRunFlag))
    {
        return VPP_ERR_INPUTNULL;
    }

    if(ulTmSn >= pTempTimerGrp->ulMaxNum)
    {
        return VPP_ERR_TIMERGRPSN;
    }

    if (VPP_SUCCESS != Vpp_Lock(&lockKey))
    {
       return VPP_ERR_LOCK;
    }

    *pRunFlag = VPP_FALSE;
    ulReturnVal = VppTimerGrpGet(pTempTimerGrp, ulTmSn, &ppTempTimer);
    if((ulReturnVal == VPP_SUCCESS)&&(VPP_NULL_PTR != *ppTempTimer))
    {
        if (VPP_TIMER_TICKING == (*ppTempTimer)->ucTmState)
        {
            *pRunFlag = VPP_TRUE;
        }
    }

    if (VPP_SUCCESS != Vpp_Unlock(lockKey))
    {
        return VPP_ERR_UNLOCK;
    }
    return VPP_SUCCESS;
}

/*****************************************************************************
 函 数 名  : VppTimerRestart
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
VPP_UINT32 VppTimerRestart
(
    IN  VPP_VOID**   ppstTimer
)
{
    VPP_UINT32       ulReturnVal = 0;
    VPP_INT          lockKey = 0;

    if (VPP_NULL_PTR == ppstTimer)
    {
        return VPP_ERR_INPUTNULL;
    }

    /* [SWED10833][2007-09-06][chenhongshan 103208]: add begin: 加锁保护 */
    if (VPP_SUCCESS != Vpp_Lock(&lockKey))
    {
        return VPP_ERR_LOCK;
    }
    /* [SWED10833][2007-09-06][chenhongshan 103208]: add end: 加锁保护 */

    /*调用VppTimerReset()重启该定时器*/
    ulReturnVal = VppTimerReset((PVPPTIMER *)ppstTimer);

    /* [SWED10833][2007-09-06][chenhongshan 103208]: add begin: 加锁保护 */
    if (VPP_SUCCESS != Vpp_Unlock(lockKey))
    {
        return VPP_ERR_UNLOCK;
    }
    /* [SWED10833][2007-09-06][chenhongshan 103208]: add end: 加锁保护 */

    return ulReturnVal;

}

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
)
{
    PVPPTIMER  pTempTimer = VPP_NULL_PTR;
    VPP_UINT32 ulTmLength = 0;
    VPP_UINT32 ulTmParam = 0;
    VPP_UINT32 ulTmName = 0;
    VPP_UINT8  ucTmMode = 0;
    PTVppFuncTimeout pfTimeout   = VPP_NULL_PTR;
    VPP_UINT32       ulReturnVal = 0;

    if (VPP_NULL_PTR == ppstTimer)
    {
        return VPP_ERR_INPUTNULL;
    }

    if (VPP_NULL_PTR == *ppstTimer)
    {
        return VPP_ERR_INPUTNULL;
    }

    pTempTimer = *ppstTimer;

    /* 先保存数据，停止定时器会把原来的数据删除 */
    ulTmLength = pTempTimer->ulTmLength;
    ulTmName = pTempTimer->ulTmName;
    ucTmMode = pTempTimer->ucTmMode;
    ulTmParam = pTempTimer->ulParam;
    pfTimeout = pTempTimer->pfTimeout;
    if(VPP_TIMER_TICKING == pTempTimer->ucTmState)
    {
        /*调用VppTimerStop()停止该定时器*/
        ulReturnVal = VppTimerStop(ppstTimer);
        if(VPP_SUCCESS != ulReturnVal)
        {
            return ulReturnVal;
        }
    }

    /*调用VppTimerStart()启动该定时器*/
    ulReturnVal = VppTimerStart(ppstTimer,
                                ulTmLength,
                                ulTmName,
                                ulTmParam,
                                ucTmMode,
                                pfTimeout);

    return ulReturnVal;

}

/*****************************************************************************
 函 数 名  : VppTimerRelStart
 功能描述  : 启动一个相对定时器
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
VPP_UINT32 VppTimerRelStart
(
    OUT  VPP_VOID**              ppstTimer,
    IN   VPP_UINT32              ulTmLength,
    IN   VPP_UINT32              ulTmName,
    IN   VPP_UINT32              ulTmParam,
    IN   VPP_UINT8               ucTmMode,
    IN   PTVppFuncTimeout        pfTimeout
)
{
    VPP_UINT32       ulReturnVal   = 0;
    VPP_INT          lockKey = 0;

    if(VPP_NULL_PTR == ppstTimer)
    {
        return VPP_ERR_INPUTNULL;
    }

    if(VPP_NULL_PTR == pfTimeout)
    {
        return VPP_ERR_INPUTNULL;
    }

    if (VPP_SUCCESS != Vpp_Lock(&lockKey))
    {
        return VPP_ERR_LOCK;
    }

    ulReturnVal = VppTimerStart((PVPPTIMER *)ppstTimer,
                                ulTmLength,
                                ulTmName,
                                ulTmParam,
                                ucTmMode,
                                pfTimeout);

    if (VPP_SUCCESS != Vpp_Unlock(lockKey))
    {
        return VPP_ERR_UNLOCK;
    }

    return ulReturnVal;
}





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
)
{
    PVPPTIMER   pstTempTimer = VPP_NULL_PTR;
    VPP_UINT32  ulReturnVal = 0;

    if(VPP_NULL_PTR == ppstTimer)
    {
        return VPP_ERR_INPUTNULL;
    }

    if (VPP_NULL_PTR == pfTimeout)
    {
        return VPP_ERR_INPUTNULL;
    }

    /*给本定时器*ppstTimer分配内存*/
    /* [[SWED12582][2008-12-17][chenhongshan]]  begin: 给定时器分配内存*/
    /* /pstTempTimer = (PVPPTIMER)(VPP_VOID*)malloc(sizeof(struct VPP_TIMERGRP_NODE_ST)); */
    VppTimerGrpAllocTimer(&pstTempTimer);
    /* [[SWED12582][2008-12-17][chenhongshan]]  end. */
    if(VPP_NULL_PTR == pstTempTimer)
    {
        return VPP_ERR_MALLOCFAIL;
    }

    /*填写定时器内容*/
    pstTempTimer->ulTmIndex = ulTmParam;
    pstTempTimer->ucTmState = VPP_TIMER_IDLE;
    pstTempTimer->ucTmMode = ucTmMode;
    pstTempTimer->ulTmLength = ulTmLength;
    pstTempTimer->ulTmName = ulTmName;
    pstTempTimer->ulParam = ulTmParam;
    Vpp_GetTime(&(pstTempTimer->stNextTime));
    pstTempTimer->stNextTime.tv_sec += (long)(ulTmLength / 1000);
    pstTempTimer->stNextTime.tv_usec += (long)((ulTmLength % 1000) * 1000);
    if (pstTempTimer->stNextTime.tv_usec >= 1000000)
    {
        pstTempTimer->stNextTime.tv_sec++;
        pstTempTimer->stNextTime.tv_usec -= 1000000;
    }
    pstTempTimer->pfTimeout = pfTimeout;
    pstTempTimer->ppTimer = ppstTimer;
    pstTempTimer->ulRunIndex = 0;

    /* 将该节点插入到运行队列中 */
    ulReturnVal = VppTimerGrpListAddNode(pstTempTimer);
    if(VPP_SUCCESS != ulReturnVal)
    {
        /* VPP_FREE(pstTempTimer); */
        /* Fix Review problem, Timer CB should be put to cache, it will be better */
        VppTimerGrpFreeTimer(&pstTempTimer);
        pstTempTimer = VPP_NULL_PTR;
        return ulReturnVal;
    }

    *ppstTimer = pstTempTimer;

    return VPP_SUCCESS;
}

/*****************************************************************************
 函 数 名  : VppTimerGrpStop
 功能描述  : 停止定时器组中的某个定时器
 输入参数  : pTimerGrp    定时器组句柄指针
             ulTmSn       要停止的相对定时器的序号
 输出参数  : 无
 返 回 值  : 正确执行返回VPP_SUCCESS，否则返回值大于零的错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerGrpStop
(
    IN  VPP_VOID*               pTimerGrp,
    IN  VPP_UINT32              ulTmSn
)
{
    VPP_TIMERGRP_ST*     pTempTimerGrp = VPP_NULL_PTR;
    PVPPTIMER*           ppTempTimer   = VPP_NULL_PTR;
    VPP_UINT32           ulReturnVal   = 0;
    VPP_INT              lockKey = 0;

    if(VPP_NULL_PTR == pTimerGrp)
    {
        return VPP_ERR_INPUTNULL;
    }

    pTempTimerGrp = (VPP_TIMERGRP_ST*)pTimerGrp;
    if(ulTmSn >= pTempTimerGrp->ulMaxNum)
    {
        return VPP_ERR_TIMERGRPSN;
    }

    if (VPP_SUCCESS != Vpp_Lock(&lockKey))
    {
        return VPP_ERR_LOCK;
    }
    ulReturnVal = VppTimerGrpGet(pTempTimerGrp, ulTmSn, &ppTempTimer);
    if((ulReturnVal != VPP_SUCCESS )||(VPP_NULL_PTR == *ppTempTimer))
    {
        if (VPP_SUCCESS != Vpp_Unlock(lockKey))
        {
            return VPP_ERR_UNLOCK;
        }
        return VPP_ERR_TMSNINVALUD;
    }

    /* 该定时器正在运行 */
    if((*ppTempTimer)->ucTmState == VPP_TIMER_TICKING)
    {
        ulReturnVal = VppTimerStop(ppTempTimer);
        if (VPP_SUCCESS != ulReturnVal)
        {
            if (VPP_SUCCESS != Vpp_Unlock(lockKey))
            {
                return VPP_ERR_UNLOCK;
            }
            return ulReturnVal;
        }
    }

    if (VPP_SUCCESS != Vpp_Unlock(lockKey))
    {
        return VPP_ERR_UNLOCK;
    }
    return VPP_SUCCESS;
}


/*****************************************************************************
 函 数 名  : VppTimerRelStop
 功能描述  : 停止一个相对定时器
 输入参数  : pTimerGrp    定时器组句柄指针
             ulTmSn       要停止的相对定时器的序号
 输出参数  : 无
 返 回 值  : 正确执行返回VPP_SUCCESS，否则返回值大于零的错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2007年9月6日
    作    者   : 陈宏山 103208
    修改内容   : 开放单个定时器接口

*****************************************************************************/
VPP_UINT32 VppTimerRelStop
(
    IN  VPP_VOID**   ppstTimer
)
{
    VPP_UINT32       ulReturnVal   = 0;
    VPP_INT          lockKey = 0;

    if(VPP_NULL_PTR == ppstTimer)
    {
        return VPP_ERR_INPUTNULL;
    }

    if (VPP_SUCCESS != Vpp_Lock(&lockKey))
    {
        return VPP_ERR_LOCK;
    }

    ulReturnVal = VppTimerStop((PVPPTIMER *)ppstTimer);

    if (VPP_SUCCESS != Vpp_Unlock(lockKey))
    {
        return VPP_ERR_UNLOCK;
    }

    return ulReturnVal;
}

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
)
{
    VPP_UINT32  ulReturnVal = 0;

    if(VPP_NULL_PTR == ppstTimer)
    {
        return VPP_ERR_INPUTNULL;
    }

    /*将该节点从运行队列中删除*/
    ulReturnVal = VppTimerGrpListDeleteNode(*ppstTimer);
    if(VPP_SUCCESS != ulReturnVal)
    {
        return ulReturnVal;
    }

    ppstTimer = ppstTimer;
    return VPP_SUCCESS;
}

/*****************************************************************************
 函 数 名  : VppTimerGrpReset
 功能描述  : 重置定时器组，恢复到初始状态
 输入参数  : pucTimeGrp    定时器组句柄指针
 输出参数  : 无
 返 回 值  : 正确执行返回VPP_SUCCESS，否则返回值大于零的错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_UINT32 VppTimerGrpReset
(
    IN  VPP_VOID*               pTimerGrp
)
{
    VPP_TIMERGRP_ST    *pTempTimerGrp = (VPP_TIMERGRP_ST*)pTimerGrp;
    VPP_UINT32          ulRetValue;
    VPP_INT             lockKey = 0;

    if(VPP_NULL_PTR == pTimerGrp)
    {
        return VPP_ERR_INPUTNULL;
    }

    /* [SWED10833][2007-09-06][chenhongshan 103208]: add begin: 加锁保护 */
    if (VPP_SUCCESS != Vpp_Lock(&lockKey))
    {
        return VPP_ERR_LOCK;
    }
    /* [SWED10833][2007-09-06][chenhongshan 103208]: add end: 加锁保护 */

    /*将定时器资源池中的所有定时器状态置为空闲态*/
    ulRetValue = VppTimerGrpIterate(pTempTimerGrp, VppTimerGrpIterateFree);
    if(VPP_SUCCESS != ulRetValue)
    {
        /* [SWED10833][2007-09-06][chenhongshan 103208]: add begin: 加锁保护 */
        if (VPP_SUCCESS != Vpp_Unlock(lockKey))
        {
            return VPP_ERR_UNLOCK;
        }
        /* [SWED10833][2007-09-06][chenhongshan 103208]: add end: 加锁保护 */
        return ulRetValue;
    }

    /*清空运行队列*/
    g_stGlobal.stRunTimerList.ulCount = 0;
    if (g_stGlobal.stRunTimerList.pArray != VPP_NULL_PTR)
    {
        memset(g_stGlobal.stRunTimerList.pArray, 0, VPP_MAXRUNTIMER_NUM * sizeof(VPP_ADDRESS));
    }

    /* [SWED10833][2007-09-06][chenhongshan 103208]: add begin: 加锁保护 */
    if (VPP_SUCCESS != Vpp_Unlock(lockKey))
    {
        return VPP_ERR_UNLOCK;
    }
    /* [SWED10833][2007-09-06][chenhongshan 103208]: add end: 加锁保护 */

    return VPP_SUCCESS;
}

/*****************************************************************************
 函 数 名  : VppTimerGrpTrigger
 功能描述  : 扫描定时器组，应用模块应该在tick事件发生时调用本函数。
 输入参数  : ultick
 输出参数  : 无
 返 回 值  : VPP_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月29日
    作    者   : 陶庆欢 60015914
    修改内容   : 代码飞检

*****************************************************************************/
VPP_VOID VppTimerGrpTrigger
(
    IN  VPP_UINT32              ultick
)
{
    VPP_INT32       rc = 0 ; /*for PC_lint;  */
    VPP_INT         lockKey = 0;
    /* 获取锁 */
    if (VPP_SUCCESS != Vpp_Lock(&lockKey)) /* 这里判断返回值意义不大 */
    {
        return;
    }

    while (g_stGlobal.stRunTimerList.ulCount != 0)
    {
        struct timeval now;
        struct timeval nextTime;
        PVPPTIMER      headTimer = (PVPPTIMER)g_stGlobal.stRunTimerList.pArray[1];

        /*取运行队列中的头节点的回调时间*/
        nextTime.tv_sec = headTimer->stNextTime.tv_sec;
        nextTime.tv_usec = headTimer->stNextTime.tv_usec;

        Vpp_GetTime(&now);

        /* 判断是否超时 */
        if (
            (nextTime.tv_sec < now.tv_sec) ||
            ((nextTime.tv_sec == now.tv_sec) && (nextTime.tv_usec <= now.tv_usec))
            )
        {
            PTVppFuncTimeout pShouldExecute = headTimer->pfTimeout;
            VPP_UINT32 index = headTimer->ulTmIndex;
            VPP_UINT32 name = headTimer->ulTmName;

            /* 定时器为循环定时器 */
            if(VPP_TIMER_LOOP == headTimer->ucTmMode)
            {
                /* [SWED10833][2007-09-06][chenhongshan 103208]: add begin: 重新封装重启定时器内部接口 */
                /*调用VppTimerReset()重启该定时器*/
                rc = (VPP_INT32)VppTimerReset(headTimer->ppTimer); /* 这里判断返回值意义不大 */
                /* [SWED10833][2007-09-06][chenhongshan 103208]: add end: 重新封装重启定时器内部接口 */
            }
            else
            {
                /*调用VppTimerStop(),停止该定时器*/
                rc = (VPP_INT32)VppTimerStop(&headTimer); /* 这里判断返回值意义不大 */
            }

            /* 调用回调函数                          */
            /* 注意: 此时没有释放锁，如果用户回调中  */
            /*       处理不当，可能造成死锁。        */
            pShouldExecute(index, name);
        }
        else
        {
            break;
        }
    }

    /* 释放锁 */
    rc = Vpp_Unlock(lockKey); /* 这里判断返回值意义不大 */

    rc = rc; /*for PC_lint;  */
    ultick = ultick;
    return;
}

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
    OUT PVPPTIMER**             pppTimer
)
{
    VPP_TIMERGRP_UNIT_ST       *pTimerGrpUnit;

    if(pTimerGrp == VPP_NULL_PTR)
    {
        return VPP_ERR_INPUTNULL;
    }

    if(ulTmSn >= pTimerGrp->ulMaxNum)
    {
        return VPP_ERR_TIMERNUMS;
    }

    if((pppTimer == VPP_NULL_PTR)||(*pppTimer != VPP_NULL_PTR))
    {
        return VPP_ERR_INPUTNULL;
    }

    pTimerGrp = pTimerGrp;
    pTimerGrpUnit = pTimerGrp->pFirstTimerGrpUnit;
    while(pTimerGrpUnit != VPP_NULL_PTR)
    {
        if(ulTmSn < pTimerGrpUnit->ulMaxNum)
        {
            *pppTimer = (PVPPTIMER*)(&pTimerGrpUnit->pData[ulTmSn]);
            return VPP_SUCCESS;
        }
        else
        {
            ulTmSn -= pTimerGrpUnit->ulMaxNum;

            pTimerGrpUnit = pTimerGrpUnit->pNextUnit;
        }
    }

    return VPP_ERR_TMNODEINVALID;
}

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
)
{
    VPP_TIMERGRP_UNIT_ST       *pTimerGrpUnit;

    if(pTimerGrp == VPP_NULL_PTR)
    {
        return VPP_ERR_INPUTNULL;
    }

    if(ulTmSn >= pTimerGrp->ulMaxNum)
    {
        return VPP_ERR_TIMERNUMS;
    }

    if(pTimer == VPP_NULL_PTR)
    {
        return VPP_ERR_INPUTNULL;
    }

    pTimerGrp = pTimerGrp;
    pTimerGrpUnit = pTimerGrp->pFirstTimerGrpUnit;
    while(pTimerGrpUnit != VPP_NULL_PTR)
    {
        if(ulTmSn < pTimerGrpUnit->ulMaxNum)
        {
            pTimerGrpUnit->pData[ulTmSn] = (VPP_ADDRESS)pTimer;
            return VPP_SUCCESS;
        }
        else
        {
            ulTmSn -= pTimerGrpUnit->ulMaxNum;

            pTimerGrpUnit = pTimerGrpUnit->pNextUnit;
        }
    }

    return VPP_ERR_TMNODEINVALID;
}

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
)
{
    VPP_INT32 rc;

    if(ppTimer == VPP_NULL_PTR)
    {
        return ;
    }

    rc = (VPP_INT32)VppTimerStop(ppTimer);  /* 这里判断返回值意义不大 */

    rc = rc; /*for PC_lint;  */
}

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
)
{
    if(ppTimer == VPP_NULL_PTR)
    {
        return ;
    }

    VPP_FREE(*ppTimer);
    *ppTimer = VPP_NULL_PTR;
}

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
)
{
    VPP_TIMERGRP_UNIT_ST       *pTimerGrpUnit;
    VPP_UINT32                  ulLoop;

    if((pTimerGrp == VPP_NULL_PTR)||(pfIterate == VPP_NULL_PTR))
    {
        return VPP_ERR_INPUTNULL;
    }

    pTimerGrpUnit = pTimerGrp->pFirstTimerGrpUnit;
    while(pTimerGrpUnit != VPP_NULL_PTR)
    {
        for(ulLoop = 0 ; ulLoop < pTimerGrpUnit->ulMaxNum; ulLoop ++)
        {
            if(pTimerGrpUnit->pData[ulLoop] != VPP_NULL_PTR)
            {
                pfIterate(((PVPPTIMER*)&pTimerGrpUnit->pData[ulLoop]));
            }
        }

        pTimerGrpUnit = pTimerGrpUnit->pNextUnit;
    }

    pTimerGrp = pTimerGrp;
    return VPP_SUCCESS;
}

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
)
{
    VPP_UINT32  ulIndex  = 0;
    VPP_ADDRESS *pArray  = VPP_NULL_PTR;

    if(VPP_NULL_PTR == pstTimerNode)
    {
        return VPP_ERR_INPUTNULL;
    }

    if (g_stGlobal.stRunTimerList.pArray == VPP_NULL_PTR)
    {
        g_stGlobal.stRunTimerList.pArray = (VPP_ADDRESS *)(VPP_VOID*)malloc(VPP_MAXRUNTIMER_NUM*sizeof(VPP_ADDRESS));
        if (g_stGlobal.stRunTimerList.pArray == VPP_NULL_PTR)
        {
            return VPP_ERR_MALLOCFAIL;
        }
        memset(g_stGlobal.stRunTimerList.pArray,0,VPP_MAXRUNTIMER_NUM*sizeof(VPP_ADDRESS));
    }
    /* 队列中定时器个数为0 */
    if(0 == g_stGlobal.stRunTimerList.ulCount)
    {
        /*pstTimerNode插入队列头*/
        g_stGlobal.stRunTimerList.pArray[1] = (VPP_ADDRESS)pstTimerNode;
        pstTimerNode->ulRunIndex = 1;
        pstTimerNode->ucTmState = VPP_TIMER_TICKING;

        /*队列定时器个数加1;*/
        g_stGlobal.stRunTimerList.ulCount++;
        return VPP_SUCCESS;
    }

/* BEGIN modify: [[SWED06479] [2006/8/1] [陶庆欢 60015914]]
   修改原因: 差1错导致内存越界 */
    if (g_stGlobal.stRunTimerList.ulCount >= VPP_MAXRUNTIMER_NUM - 1)
    {
        return VPP_ERR_RUNNINGTIMER;
    }
/* END   modify: [[SWED06479] [2006/8/1] [陶庆欢 60015914]] */
    ulIndex = g_stGlobal.stRunTimerList.ulCount + 1;
    pArray = g_stGlobal.stRunTimerList.pArray;
    pArray[ulIndex] = (VPP_ADDRESS)pstTimerNode;/* 先放到最后一个 */
    pstTimerNode->ulRunIndex = ulIndex;
    /*二叉树查找*/
    Vpp_AddToBinTree(pstTimerNode,ulIndex,pArray);

    /*该定时器状态改为计时状态;*/
    pstTimerNode->ucTmState = VPP_TIMER_TICKING;
    /*队列定时器个数加1*/
    g_stGlobal.stRunTimerList.ulCount++;

    return VPP_SUCCESS;
}

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
)
{
    PVPPTIMER*  ppTemp = VPP_NULL_PTR;
    PVPPTIMER   pstTempTNode1 = VPP_NULL_PTR;
    PVPPTIMER   pstTempTNode2 = VPP_NULL_PTR;
    VPP_UINT32  ulIndex  = 0;
    VPP_ADDRESS *pArray  = VPP_NULL_PTR;

    if(VPP_NULL_PTR == pstTimerNode)
    {
        return VPP_ERR_INPUTNULL;
    }

    if ((pstTimerNode->ulRunIndex == 0)
        || (pstTimerNode->ulRunIndex > g_stGlobal.stRunTimerList.ulCount))
    {
        return VPP_ERR_TMNODEINVALID;
    }
    /* 该定时器的状态为计时状态 */
    if (VPP_TIMER_TICKING == pstTimerNode->ucTmState)
    {
        ulIndex = pstTimerNode->ulRunIndex;
        pArray = g_stGlobal.stRunTimerList.pArray;

        /* while(1) */
        for(;;)
        {
            if (ulIndex*2 >= VPP_MAXRUNTIMER_NUM)
            {
                pstTempTNode1 = VPP_NULL_PTR;
                pstTempTNode2 = VPP_NULL_PTR;
            }
            else if ((ulIndex*2 + 1) >= VPP_MAXRUNTIMER_NUM)
            {
                pstTempTNode1 = (PVPPTIMER)pArray[ulIndex*2];
                pstTempTNode2 = VPP_NULL_PTR;
            }
            else
            {
                pstTempTNode1 = (PVPPTIMER)pArray[ulIndex*2];
                pstTempTNode2 = (PVPPTIMER)pArray[ulIndex*2 +1];
            }

            if (pstTempTNode1 == VPP_NULL_PTR)
            {/* 子节点均为空，和最后一个交换 */
                if (ulIndex == g_stGlobal.stRunTimerList.ulCount)
                {/* 刚好是最后一个 */
                    pArray[g_stGlobal.stRunTimerList.ulCount] = 0;
                    break;
                }
                else
                {
                    Vpp_ExchangeValue(((PVPPTIMER )pArray[ulIndex])->ulRunIndex,((PVPPTIMER)pArray[g_stGlobal.stRunTimerList.ulCount])->ulRunIndex, VPP_UINT32);
                    Vpp_ExchangeValue(pArray[ulIndex],pArray[g_stGlobal.stRunTimerList.ulCount], VPP_ADDRESS);
                    pArray[g_stGlobal.stRunTimerList.ulCount] = 0;
                    /* 比较当前节点是否符合规则，大于父节点 */
                    Vpp_AddToBinTree((PVPPTIMER)pArray[ulIndex],ulIndex,pArray);
                    break;
                }
            }
            else if (pstTempTNode2 == VPP_NULL_PTR)
            {/* 右子节点为空 */
                Vpp_ExchangeValue(((PVPPTIMER)pArray[ulIndex])->ulRunIndex,((PVPPTIMER)pArray[ulIndex*2])->ulRunIndex, VPP_UINT32);
                Vpp_ExchangeValue(pArray[ulIndex],pArray[ulIndex*2], VPP_ADDRESS);
                pArray[g_stGlobal.stRunTimerList.ulCount] = 0;
                break;
            }
            else
            {/* 子节点都不为空 */

                /* 总是与较小的子节点交换 */
                if (
                    (pstTempTNode1->stNextTime.tv_sec < pstTempTNode2->stNextTime.tv_sec) ||
                    ((pstTempTNode1->stNextTime.tv_sec == pstTempTNode2->stNextTime.tv_sec)
                    && (pstTempTNode1->stNextTime.tv_usec <= pstTempTNode2->stNextTime.tv_usec))
                    )/*  */
                {/* 左子节点的超时时间小于等于右子节点的超时时间 */
                    Vpp_ExchangeValue(((PVPPTIMER)pArray[ulIndex])->ulRunIndex, ((PVPPTIMER )pArray[ulIndex*2])->ulRunIndex, VPP_UINT32);
                    Vpp_ExchangeValue(pArray[ulIndex], pArray[ulIndex*2], VPP_ADDRESS);
                    ulIndex = ulIndex*2;
                }
                else
                {
                    Vpp_ExchangeValue(((PVPPTIMER )pArray[ulIndex])->ulRunIndex, ((PVPPTIMER)pArray[ulIndex*2 +1])->ulRunIndex, VPP_UINT32);
                    Vpp_ExchangeValue(pArray[ulIndex], pArray[ulIndex*2 +1], VPP_ADDRESS);
                    ulIndex = ulIndex*2 + 1;
                }
            }
        }

        /* 释放定时器内存 */
        ppTemp = pstTimerNode->ppTimer;

        /* [[SWED12582][2008-12-17][chenhongshan]]  begin:释放缓冲区定时器内存*/
        /* VPP_FREE(*ppTemp); */
        VppTimerGrpFreeTimer(ppTemp);
        /* [[SWED12582][2008-12-17][chenhongshan]]  end. */

        *ppTemp = VPP_NULL_PTR;

        /*运行队列定时器个数减1*/
        g_stGlobal.stRunTimerList.ulCount--;
        if (g_stGlobal.stRunTimerList.ulCount == 0)
        {
            g_stGlobal.stRunTimerList.pArray[1] = VPP_NULL_PTR;
        }
        return VPP_SUCCESS;
    }

    pstTimerNode = pstTimerNode;
    return VPP_FAILURE;
}

/*****************************************************************************
 函 数 名  : VppTimerGrpAllocTimer
 功能描述  : 从缓冲区中取出空闲的一个节点，缓冲区中无，则直接申请内存
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
VPP_VOID VppTimerGrpAllocTimer(OUT PVPPTIMER  *pstTimer )
{
    /* 若缓冲区有空闲控制块，则直接使用空闲块，否则分配新控制块 */
    if (g_stGlobal.ulFreeCount > 0)
    {
        g_stGlobal.ulFreeCount--;
        *pstTimer = (g_stGlobal.ahTimerBuffer[g_stGlobal.ulFreeCount]);
    }
    else
    {
        /*lint -save -e826 */
        *pstTimer =(PVPPTIMER)malloc(sizeof(struct VPP_TIMERGRP_NODE_ST));
    }
}

/*****************************************************************************
 函 数 名  : VppTimerGrpFreeTimer
 功能描述  : 将一个定时器放回缓冲区中，缓冲区满则直接释放内存
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
VPP_VOID VppTimerGrpFreeTimer(IN PVPPTIMER  *pstTimer )
{
    /* 若缓冲区未满，则将释放的控制块放在缓冲区中。*/
    if (g_stGlobal.ulFreeCount < VPP_MAX_TIMEER_CACHE)
    {
        g_stGlobal.ahTimerBuffer[g_stGlobal.ulFreeCount] = *pstTimer;
        g_stGlobal.ulFreeCount++;
    }
    else
    {
        VPP_FREE(*pstTimer);
    }

    pstTimer = pstTimer;
}



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
VPP_INT32 Vpp_InitLock( void )
{
    VPP_INT32 nRet = VPP_FAILURE;

#ifdef VPP_OS_VER
#if ( VPP_OS_VER == VPP_WIN32 )
{
    InitializeCriticalSection( &VPP_CriticalSection );
    nRet = VPP_SUCCESS;
}
#elif ( VPP_OS_VER == VPP_SOLARIS_POSIX || VPP_OS_VER == VPP_AIX || VPP_OS_VER == VPP_HP)
{
    pthread_mutexattr_t  attr;
    nRet = pthread_mutexattr_init(&attr);
    if (nRet != 0)
        return nRet;
    nRet = pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
    if (nRet != 0)
        return nRet;
    nRet = pthread_mutex_init(&VPP_mutex, &attr);
    if (nRet != 0)
        return nRet;
}
#elif ( VPP_OS_VER == VPP_LINUX_POSIX)
{
    pthread_mutexattr_t  attr;
    nRet = pthread_mutexattr_init(&attr);
    if (nRet != 0)
        return nRet;
    nRet = pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE_NP);
    if (nRet != 0)
        return nRet;
    nRet = pthread_mutex_init(&VPP_mutex, &attr);
    if (nRet != 0)
        return nRet;
}
#elif (VPP_OS_VER == VPP_VXWORKS)
    nRet = VPP_SUCCESS;
#endif /* VPP_OS_VER */
#endif /* VPP_OS_VER */

    return nRet;
}

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
VPP_INT32 Vpp_Lock(VPP_INT *pLockKey)
{
    VPP_INT32 nRet = VPP_FAILURE;

    pLockKey = pLockKey;
#ifdef VPP_OS_VER
#if ( VPP_OS_VER == VPP_WIN32 )
{
    EnterCriticalSection( &VPP_CriticalSection );
    nRet = VPP_SUCCESS;
}
#elif ( VPP_OS_VER == VPP_SOLARIS_POSIX || VPP_OS_VER == VPP_AIX || VPP_OS_VER == VPP_HP || VPP_OS_VER == VPP_LINUX_POSIX)
{
    nRet = pthread_mutex_lock(&VPP_mutex);
}
#elif (VPP_OS_VER == VPP_VXWORKS)
{
    *pLockKey = intLock();
    nRet = VPP_SUCCESS;
}
#endif /* VPP_OS_VER */
#endif /* VPP_OS_VER */

    return nRet;
}

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
VPP_INT32 Vpp_Unlock(VPP_INT lockKey)
{
    VPP_INT32 nRet = VPP_FAILURE;

    lockKey = lockKey;
#ifdef VPP_OS_VER
#if ( VPP_OS_VER == VPP_WIN32 )
{
    LeaveCriticalSection ( &VPP_CriticalSection );
    nRet = VPP_SUCCESS;
}
#elif ( VPP_OS_VER == VPP_SOLARIS_POSIX || VPP_OS_VER == VPP_AIX || VPP_OS_VER == VPP_HP || VPP_OS_VER == VPP_LINUX_POSIX)
{
    nRet = pthread_mutex_unlock(&VPP_mutex);

}
#elif (VPP_OS_VER == VPP_VXWORKS)
{
    intUnlock(lockKey);
    nRet = VPP_SUCCESS;
}
#endif /* VPP_OS_VER */
#endif /* VPP_OS_VER ==*/

    return nRet;
}

#ifdef VPP_OS_DOPRA
typedef struct TIME_STRU
{
    VPP_UINT8 ucHour;  /* 时 */
    VPP_UINT8 ucMinute;/* 分 */
    VPP_UINT8 ucSecond;/* 秒 */
    VPP_UINT8 ucPadding;
} TIME_T;

typedef struct DATE_STRU
{
    VPP_UINT16 uwYear; /* 年 */
    VPP_UINT8 ucMonth; /* 月 */
    VPP_UINT8 ucDate;  /* 日 */
} DATE_T;
#endif

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
VPP_VOID Vpp_GetTime(struct timeval *pstTime)
{
    #if ( VPP_OS_VER == VPP_WIN32 )
    {
        /* FIX: GetTickCount() 返回32bit的毫秒数，*/
        /* 系统运行49.7天后会出现问题             */
        VPP_UINT32 millis = GetTickCount();
        pstTime->tv_sec = (long)(millis / 1000);
        pstTime->tv_usec =  (millis % 1000) * 1000;
    }
    #elif ( VPP_OS_VER == VPP_SOLARIS_POSIX || VPP_OS_VER == VPP_AIX || VPP_OS_VER == VPP_HP || VPP_OS_VER == VPP_LINUX_POSIX)
    {
        VPP_UINT32    ulValue = 0;

        #ifdef  VPP_OS_DOPRA
        {
            DATE_T        stDate;
            TIME_T        stTime;
            extern VPP_UINT32 VOS_TmSince1970( VPP_UINT32 *pulTimeInSecHigh,
                                                     VPP_UINT32 *pulTimeInSecLow );
            extern  VOS_TmGet( DATE_T     *pDate,
                                 TIME_T     *pTime,
                                 VPP_UINT32 *pulMillSecs );

            VOS_TmGet(&stDate, &stTime,&ulValue);
            pstTime->tv_usec = ulValue*1000;
            VOS_TmSince1970(&ulValue, (VPP_UINT32*)&pstTime->tv_sec);
        }
        #else
        {
            /* get time since 1970 in microsec */
            gettimeofday(pstTime, 0);
        }
        #endif


    }
    #elif (VPP_OS_VER == VPP_VXWORKS)
    {
        clock_gettime(0, (struct timespec *)pstTime);
        pstTime->tv_usec /= 1000;
    }
    #endif
}


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
VPP_VOID Vpp_AddToBinTree(PVPPTIMER  pstTimerNode, VPP_UINT32 ulIndex,VPP_ADDRESS *pTreeArray)
{
    PVPPTIMER   pstTempTNode = VPP_NULL_PTR;

    pstTempTNode = (PVPPTIMER)pTreeArray[ulIndex/2];
    for(;
        pstTempTNode != VPP_NULL_PTR;)
    {
        pstTempTNode = (PVPPTIMER)pTreeArray[ulIndex/2];
        /* 插入节点的超时时间小于当前节点的超时时间 */
        if (
            (pstTimerNode->stNextTime.tv_sec < pstTempTNode->stNextTime.tv_sec) ||
            ((pstTimerNode->stNextTime.tv_sec == pstTempTNode->stNextTime.tv_sec)
            && (pstTimerNode->stNextTime.tv_usec < pstTempTNode->stNextTime.tv_usec))
            )/*  */
        {
            Vpp_ExchangeValue(pTreeArray[ulIndex/2], pTreeArray[ulIndex], VPP_ADDRESS);
            Vpp_ExchangeValue(((PVPPTIMER )pTreeArray[ulIndex/2])->ulRunIndex, ((PVPPTIMER)pTreeArray[ulIndex])->ulRunIndex, VPP_UINT32);
            ulIndex = ulIndex/2;
            if (ulIndex == 1)
            {/* 到根节点了 */
                break;
            }
        }
        else
        {
            break;
        }
    }
    pstTimerNode = pstTimerNode;

}

/*****************************************************************************
 函 数 名  : VppTimerGrpDebugControl
 功能描述  : 调试控制接口，内部测试使用，不在头文件中出现。
 输入参数  : IN VPP_UINT32 ulCommand,调试命令字
             IN VPP_UINT8 *pucParam, 调试命令执行需要的参数
 输出参数  : 无
 返 回 值  : VPP_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2008年12月17日
    作    者   : 陈宏山 00103208
    修改内容   : 新生成函数

*****************************************************************************/
#ifdef VPP_DEBUG
VPP_VOID VppTimerGrpDebugControl
(
    IN VPP_UINT32 ulCommand,
    IN VPP_UINT8 *pucParam
)
{
    switch(ulCommand)
    {
        case VPP_DEBUG_HELP:
            VPP_LOG("\r\nVPP TIMERGRP Support Following Debug Command:\r\n");
            VPP_LOG("\t [0] show current free timer count\r\n");
            break;

        case VPP_DEBUG_TIMER_LIST:
            VPP_LOG("\t FreeTimerCount = %d\r\n", g_stGlobal.ulFreeCount);
            break;

        default:
            break;
    }
}
#endif

/*****************************************************************************
 函 数 名  : VppTimerGrpGetVersion
 功能描述  : 获取版本号
 输入参数  : VPP_VOID
 输出参数  : 无
 返 回 值  : 定时器的版本号
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2006年7月31日
    作    者   : 陶庆欢 60015914
    修改内容   : 新生成函数

*****************************************************************************/
const VPP_CHAR *VppTimerGrpGetVersion(VPP_VOID)
{
#ifdef VPP_DEBUG
    return "VPP TimerGrp Library debug version 1.3.0.13\r\n\
            Copyright(C) 2003 - 2008 Huawei Technologies Co., Ltd.All rights reserved.\r\n";
#else
    return "VPP TimerGrp Library release version 1.3.0.13\r\n\
            Copyright(C) 2003 - 2008 Huawei Technologies Co., Ltd.All rights reserved.\r\n";
#endif
}

#ifdef  __cplusplus
}
#endif
