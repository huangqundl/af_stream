/*
 ******************************************************************************
 版权所有 (C)， 2008-2010， 华为技术有限公司
 ******************************************************************************
  文 件 名   : egnapiipflow.h
  版 本 号   : 初稿
  作    者   : EGN项目组
  生成日期   : 2010年12月3日
  最近修改   :
  功能描述   : 流表接口定义
  函数列表   :
******************************************************************************/
/**
 *@file  egnapiipflow.h
 *流表管理头文件
*/
#ifndef  __EGN_API_IPF_
#define __EGN_API_IPF_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*STRUCT< 流表信息 >*/
typedef struct _EgnIpFlowTblInfo
{
    EGN_UINT32  ulTotalFlow;     /* 流结点总数 */
    EGN_UINT32  ulIdleFlow;      /* 空闲流结点个数 */
} EgnIpFlowTblInfo;

/*STRUCT< 统计项 >*/
typedef struct _EgnIpFlowStatInfo
{
    EGN_UINT32  ulReceivedCnt;    /* 接收到的报文总数 */
    EGN_UINT32  ulSeqRejectCnt;   /* 序列号错误的丢弃包的个数 */
    EGN_UINT32  ulOtherRejectCnt; /* 其他错误丢弃的包的个数 */
    EGN_UINT32  ulSuccessCnt;     /* 识别成功的包的个数 */
    EGN_UINT32  ulUnknownCnt;     /* 未识别出结果的包的个数 */
    EGN_UINT32  ulErrorCnt;       /* 识别返回错误的包的个数 */
}EgnIpFlowStatInfo;

/*STRUCT< 五元组 >*/
typedef struct _EgnIpFlowFiveTuple
{
    EGN_UINT32  ulSrcIp;        /* IPv4 source address */
    EGN_UINT32  ulDstIp;        /* IPv4 destination address */
    EGN_UINT16  usSrcPort;      /* Source Port */
    EGN_UINT16  usDstPort;      /* Destination Port */
    EGN_UINT8   ucProto;        /* 传输层协议类型，对应于#EGN_TRANS_TYPE_EN */
    EGN_UINT8   aucReserved[3];
} EgnIpFlowFiveTuple;

/*STRUCT< 流表初始化配置参数 >*/
typedef struct _EgnIpFlowTblCfgParam
{
    EGN_UINT32  ulSize;            /* 流的规模 */
    EGN_UINT8   aucReserved[4];    /* 保留字节，64位对齐 */
} EgnIpFlowTblCfgParam;

/*STRUCT< 扩展参数，用于后续扩展 >*/
typedef struct _EgnIpFlowAuxData
{
    EGN_UINT32 ulAuxData;          /* 扩展数据 */
    EGN_UINT32 ulReserved;         /* 保留字节，64位对齐 */
}EgnIpFlowAuxData;

/*STRUCT< 初始化静态内存Buffer >*/
typedef struct _EgnIpFlowInitStaticBuf
{
    EGN_UCHAR  *pucIpFlowBuf;     /* EgnIpFlow内存起始地址 */
    EGN_UCHAR  *pucSharedBuf;     /* 共享内存起始地址 */
    EGN_UCHAR  *pucUnsharedBuf;   /* 非共享内存起始地址 */
    EGN_ULONG   ulIpFlowBufLen;   /* EgnIpFlow内存长度 */
    EGN_ULONG   ulSharedBufLen;   /* 共享内存长度 */
    EGN_ULONG   ulUnsharedBufLen; /* 非共享内存长度 */
}EgnIpFlowInitStaticBuf;

/*ENUM< 流删除原因 >*/
typedef enum
{
    EGN_IP_FLOW_DEL_BEGIN         = EGN_EN_INVALID,
    EGN_IP_FLOW_DEL_FIN_INSPECT   = 0x0,            /* 识别结束 */
    EGN_IP_FLOW_DEL_CHAIN_BREAK   = 0x1,            /* 断链 */
    EGN_IP_FLOW_DEL_AGED          = 0x2,            /* 老化 */
    EGN_IP_FLOW_DEL_END,
    EGN_IP_FLOW_DEL_BOTTOM = EGN_EN_BUTT
}EGN_IP_FLOW_DEL_EN;

/*ENUM< 包的方向 >*/
typedef enum
{
    EGN_IP_FLOW_DIRECT_BEGIN  = EGN_EN_INVALID,
    EGN_IP_FLOW_DIRECT_UP     = 0x0,              /* 上行方向标记 */
    EGN_IP_FLOW_DIRECT_DOWN   = 0x1,              /* 下行方向标记 */
    EGN_IP_FLOW_DIRECT_END,
    EGN_IP_FLOW_DIRECT_BOTTOM = EGN_EN_BUTT
}EGN_IP_FLOW_DIRECT_EN;

/*******************************************************************************
*    Func Name: EgnApiIpFlowGetNeededMemSize
*      Purpose: 计算流表所需内存。
*  Description: 计算流表所需内存，单位字节。
*        Input: EGN_UINT32 ulSize:流的规模<1000~120000>
*        InOut: EGN_UINT32* pulMemSize:所需内存的大小<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 该API在EgnApiIpFlowTblInit之前调用。传入的流的规模ulSize值必须要和
                EgnApiIpFlowTblInit、EgnApiGetNeededMemSize、_EgnInitCfgParam中的ulFlowScale相同。
*        Since: V200R002C02
*    Reference: EgnApiIpFlowTblInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiIpFlowGetNeededMemSize
(
    IN     EGN_UINT32   ulSize,
    INOUT  EGN_UINT32  *pulMemSize
);

/*******************************************************************************
*    Func Name: EgnApiIpFlowTblInit
*      Purpose: 初始化流表业务实例。
*  Description: 初始化流表的内存，配置流表参数。
*        Input: EgnIpFlowInitStaticBuf* pstInitBuf:初始化内存Buffer<非空>
*               EgnIpFlowTblCfgParam pstCfgParam:流表配置参数<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 必须先调用EgnApiIpFlowGetNeededMemSize计算流表所需的内存大小。
                当前流表功能只支持关联表非共享的情况，最大只支持32个实例。
*        Since: V200R002C02
*    Reference: EgnApiIpFlowGetNeededMemSize
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32  EgnApiIpFlowTblInit
(
    IN  EgnIpFlowInitStaticBuf *pstInitBuf,
    IN  EgnIpFlowTblCfgParam *pstCfgParam
);

/*******************************************************************************
*    Func Name: EgnApiIpFlowTblDestroy
*      Purpose: 流表业务实例去初始化。
*  Description: 在启动流表功能时流表实例去初始化。
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_VOID，无。
*      Caution: 无。
*        Since: V200R002C02
*    Reference: N/A
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_VOID  EgnApiIpFlowTblDestroy
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiIpFlowInspect
*      Purpose: 将IP原始数据包传入进行识别。
*  Description: 通过输入IP原始数据包、数据包长度、包方向及包序列号，对数据包进行识别，输出识别结果。
*        Input: EGN_UINT8* pucPacket:IP原始数据包<非空>
*               EGN_UINT16 usPacketLen:数据包长度<21~65535>
*               EGN_UINT8 ucDirect:包方向，对应于EGN_IP_FLOW_DIRECT_EN<EGN_IP_FLOW_DIRECT_EN>
*               EGN_UINT32 ulSequence:包的序号，采用自然计数，第一包序号为1<大于0>
*        InOut: EgnResult pstInspectResult:识别结果<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 目前仅支持多进程环境下使用，不支持多线程。该接口不支持IPv6。
*        Since: V200R002C02
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32  EgnApiIpFlowInspect
(
    IN     EGN_CONST  EGN_UINT8    *pucPket,
    IN                EGN_UINT16    usPketlen,
    IN                EGN_UINT8     ucDirect,
    IN                EGN_UINT32    ulSequence,
    INOUT             EgnResult    *pstInspectResult
);

/*******************************************************************************
*    Func Name: EgnApiIpFlowDelete
*      Purpose: 删除一条流。
*  Description: 在启动流表功能时，根据输入的五元组信息和删除原因，删除一条流
*        Input: EgnIpFlowFiveTuple* pstFiveTuple:五元组<非空>
*               EGN_UINT8 ucDelReason:删除原因，对应于EGN_IP_FLOW_DEL_EN<EGN_IP_FLOW_DEL_EN>
*        InOut: EgnIpFlowTblCfgParam pstAuxData:扩展参数<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无。
*        Since: V200R002C02
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32  EgnApiIpFlowDelete
(
    IN  EGN_CONST  EgnIpFlowFiveTuple  *pstFiveTuple,
    IN             EGN_UINT8            ucDelReason,
    INOUT          EgnIpFlowAuxData    *pstAuxData
);

/*******************************************************************************
*    Func Name: EgnApiIpFlowQueryTableInfo
*      Purpose: 查询流表信息。
*  Description: 启动流表功能时，检查流表信息。
*        Input: NA
*        InOut: EgnIpFlowTblInfo* pstTblInfo:流表信息<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无。
*        Since: V200R002C02
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32  EgnApiIpFlowQueryTableInfo
(
    INOUT  EgnIpFlowTblInfo  *pstTblInfo
);

/*******************************************************************************
*    Func Name: EgnApiIpFlowGetHandle
*      Purpose: 获取实例handle。
*  Description: 在启动流表功能时，获取实例handle。
*        Input: NA
*        InOut: NA
*       Output: EGN_VOID** ppvHandle:实例handle<非空>
*       Return: EGN_UINT32，成功或失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无。
*        Since: V200R002C02
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiIpFlowGetHandle
(
    OUT    EGN_VOID         **ppvHandle
);

/*******************************************************************************
*    Func Name: EgnApiIpFlowStatGet
*      Purpose: 获取流表统计信息。
*  Description: 获取流表统计信息，用户需要从外部自行申请内存，传入指针。
*        Input: NA
*        InOut: EgnIpFlowStatInfo* pstStatRslt:输出统计数据<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 在开始进行统计前，先使用EgnApiIpFlowStatClear清空以前的统计信息。
*        Since: V200R002C02
*    Reference: EgnApiIpFlowStatClear
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiIpFlowStatGet
(
    INOUT EgnIpFlowStatInfo *pstStatRslt
);

/*******************************************************************************
*    Func Name: EgnApiIpFlowStatClear
*      Purpose: 清空流表统计信息。
*  Description: 清空流表统计信息。
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 在开始进行统计前，先使用该接口清空以前的统计信息。
*        Since: V200R002C02
*    Reference: EgnApiIpFlowStatGet
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiIpFlowStatClear
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiIpFlowMngInit
*      Purpose: 初始化管理线程
*  Description: EGN管理线程初始化接口，选择使用流表功能时使用该API，否则使用EgnApiMngInit。
                完成EGN组件需要使用的系统资源的申请、初始化等工作。调用EgnApiIpFlowMngInit前，
                需要先调用EgnApiInitPubParam初始化EGN的配置参数，按照缺省值，生成EGN的默认配置值。
                如果默认值能满足业务需求，可以直接调用EgnApiIpFlowMngInit初始化。
                如果默认值不能满足业务需求，可以根据业务需求修改相应的配置值，
                再调用EgnApiIpFlowMngInit初始化EGN模块的管理线程。
*        Input: EGN_UCHAR* pucGlobalBuffer:全局内存首地址，32位平台要求4字节对齐，64位平台要求8字节对齐<非空>
*               EGN_UINT32 ulBufferLen:全局内存长度，不能为零<大于零>
*               EgnInitCfgParam* pstParam:参数信息<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 除特殊说明外，其他API应在成功初始化EGN管理线程成功调用该API之后才能被使用。
*        Since: V200R002C02
*    Reference: EgnApiInitPubParam
*               EgnApiMngInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiIpFlowMngInit
(
   IN            EGN_UCHAR           *pucGlobalBuffer,
   IN            EGN_UINT32           ulBufferLen,
   IN  EGN_CONST EgnInitCfgParam     *pstParam
);

/*******************************************************************************
*    Func Name: EgnApiIpFlowMngDestroy
*      Purpose: 管理线程去初始化。
*  Description: 完成管理进程、线程系统资源的释放等工作。
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 流表管理实例去初始化必须在识别业务实例去初始化之后进行。　
*        Since: V200R002C02
*    Reference: EgnApiIpFlowMngDestroy
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiIpFlowMngDestroy
(
    EGN_VOID
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __EGN_API_IPF_ */

