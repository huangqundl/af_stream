/*
 ******************************************************************************
 版权所有 (C), 2008-2009, 华为技术有限公司
 ******************************************************************************
  文 件 名   : egnapicommonsn.h
  版 本 号   : 初稿
  作    者   : EGN项目组
  生成日期   : 2012年06月07日
  最近修改   :
  功能描述   : 公共宏定义
  函数列表   :
  修改历史   :
  1.日    期   : 2012年06月07日
    作    者   : EGN项目组
    修改内容   : 创建文件

******************************************************************************/
/**@file  egnapicommonsn.h
  *    公共宏定义
*******************************************************/
/**
 * @defgroup session  EGN会话模块的API
 */

#ifndef __EGN_API_COMMON_SN_H__
#define __EGN_API_COMMON_SN_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*STRUCT< 识别参数 >*/
typedef struct _EgnCfgParam
{
    EGN_UINT32  ulFlowScale;            /* 最大流规模，该值不能超过10000000 */
    EGN_UINT32  ulRelScaleIPv4;         /* IPv4关联表大小，该值不能超过10000000 */
    EGN_UINT32  ulRelScaleIPv6;         /* IPv6关联表大小，该值不能超过10000000 */
#ifdef EGN_64
    EGN_UINT8   aucReserved[4];         /* 64位对齐保留字节 */
#endif
} EgnCfgParam;

/*STRUCT< 会话参数 >*/
typedef struct _EgnSnCfgParam
{
    EGN_UINT32  ulSnSwitch;             /* Session功能开关 */
    EGN_UINT32  ulSnFlowScale;          /* 需要Sn关联的流数，用于计算Sn中间状态内存，必须配 */
    EGN_UINT32  ulSnCBCntIPv4;          /* IPv4会话关联识别CB块的数目 */
    EGN_UINT32  ulSnCBCntIPv6;          /* IPv6会话关联识别CB块的数目 */
    EGN_UINT32  ulSnBodyMemCache;       /* 2k缓存 */
    EGN_UINT32  ulSnBodyMemDecompress;  /* 6k缓存 */
} EgnSnCfgParam;

/*******************************************************************************
*    Func Name: PFEgnSspDeCompress
*      Purpose: 解压缩回调原型
*  Description: 解压缩回调原型
*        Input: EGN_UINT32 ulCompressType:压缩类型<EGN_SN_DECOMPRESS_TYPE_EN>
                EGN_UCHAR *pucInData:压缩数据<非空>
                EGN_UINT32 ulInDataLen:压缩数据长度<大于零>
*        InOut: EGN_UCHAR *pucOutData:解压数据<非空>
                EGN_UINT32 *pulOutDataLen:解压数据长度<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R06C00
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
typedef EGN_UINT32 (*PFEgnSspDeCompress)
(
    IN    EGN_UINT32      ulCompressType,  /* 压缩类型 */
    IN    EGN_UCHAR      *pucInData,       /* 压缩数据 */
    IN    EGN_UINT32      ulInDataLen,     /* 压缩数据长度 */
    INOUT EGN_UCHAR      *pucOutData,      /* 解压数据 */
    INOUT EGN_UINT32     *pulOutDataLen    /* 解压数据长度 */
);
/*STRUCT< 系统服务功能的回调函数定义 >*/
typedef struct _EgnSnSspFunc
{
    PFEgnSspDeCompress  pfDeCompress;      /* 解压缩 必须 */
} EgnSnSspFunc;

/*STRUCT< SSP回调接口 >*/
typedef struct _EgnSnSspSmpGlobalInfo
{
    EgnSnSspFunc   stSnSspCallback;        /* SSP回调接口 */
} EgnSnSspSmpGlobalInfo;

/*ENUM< Session解压缩类型 >*/
typedef enum
{
    EGN_SN_DECOMPRESS_TYPE_BEGIN            = EGN_EN_INVALID,
    EGN_SN_DECOMPRESS_TYPE_GZIP             = 0,              /* Gzip解压缩 */
    EGN_SN_DECOMPRESS_TYPE_END,

    EGN_SN_DECOMPRESS_TYPE_BOTTOM           = EGN_EN_BUTT
}EGN_SN_DECOMPRESS_TYPE_EN;

/*******************************************************************************
*    Func Name: EgnApiSnPacketInspect
*      Purpose: 将完成2～4层解析后的报文传入并进行协议识别。
*  Description: 根据输入实例线程ID、实例句柄、流识别句柄和报文获取识别结果及识别扩展参数。
                用户可以根据识别结果EgnResult中的bIsContinue来控制是否继续输入包到引擎中识别。
                若bIsContinue为EGN_TRUE需要继续输入包，若bIsContinue为EGN_FALSE不需要继续输入包。
*        Input: EGN_UINT16 usThreadID:实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*               EGN_VOID** ppvFlowInspect:流识别句柄<非空>
*               EgnPacket* pstPacketInfo:报文<非空>
*        InOut: EgnResult* pstIspectResult:识别结果<非空>
*               EgnInspectAuxData* pstAuxData:识别扩展参数<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 该API在成功初始化EGN识别线程EgnApiSnInspectorInit成功之前不可被调用。
*        Since: V300R06C00
*    Reference: EgnApiSnInspectorInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnPacketInspect
(
    IN                EGN_UINT16           usThreadID,
    IN                EGN_VOID            *pvHandle,
    IN                EGN_VOID           **ppvFlowInspect,
    IN     EGN_CONST  EgnPacket           *pstPacketInfo,
    INOUT             EgnResult           *pstInspectResult,
    INOUT  EGN_CONST  EgnInspectAuxData   *pstAuxData
);

/*******************************************************************************
*    Func Name: EgnApiSnReleaseCtxData
*      Purpose: 释放指定的流识别句柄。
*  Description: 对于非首包识别的流，识别结束后使用该接口释放流识别句柄。
*        Input: EGN_UINT16 usThreadID:调用此接口的线程ID<0~65535>
*               EGN_VOID* pvHandle:多实例句柄，不能为空<非空>
*               EGN_VOID** ppvCtxData:流识别句柄<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_VOID
*      Caution: 该API在成功初始化EGN识别线程EgnApiSnInspectorInit成功之前不可被调用。
*        Since: V300R06C00
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_VOID EgnApiSnReleaseCtxData
(
    IN              EGN_UINT16    usThreadID,
    IN              EGN_VOID     *pvHandle,
    IN              EGN_VOID    **ppvCtxData
);

/*******************************************************************************
*    Func Name: EgnApiSnInitPubParam
*      Purpose: 生成EGN的默认初始化配置参数。
*  Description: 该接口提供了默认参数配置功能，按照缺省值，生成EGN的默认初始化配置参数。
                如果有些参数您不希望进行配置，或者配置起来比较困难，可以使用该接口进行配置。
                相关的配置参数，请参见_EgnInitCfgParam的参数。
*        Input: EgnInitCfgParam* pstParam:初始化参数<非空>
*               EgnSnCfgParam* pstParamSn:初始化参数<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 该API在使用EgnApiRegSspFunc成功注册系统服务回调函数之前可被调用。
*        Since: V300R06C00
*    Reference: EgnInitCfgParam
*               EgnSnCfgParam
                EgnApiSnMngInit
                EgnApiSnGetNeededMemSize
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnInitPubParam
(
    IN  EgnInitCfgParam    *pstParam,
    IN  EgnSnCfgParam      *pstParamSn
);

/*******************************************************************************
*    Func Name: EgnApiSnMngInit
*      Purpose: 初始化管理线程
*  Description: EGN管理线程初始化接口，完成EGN组件需要使用的系统资源的申请、初始化等工作。
                调用EgnApiSnMngInit前，需要先调用EgnApiSnInitPubParam初始化EGN的配置参数，
                按照缺省值，生成EGN的默认配置值。如果默认值能满足业务需求，可以直接调用EgnApiSnMngInit初始化。
                如果默认值不能满足业务需求，可以根据业务需求修改相应的配置值，再调用EgnApiSnMngInit初始化EGN模块的管理线程。
*        Input: EGN_UCHAR* pucGlobalBuffer:全局内存首地址<非空>
*               EGN_UINT32 ulBufferLen:全局内存长度<大于零>
*               EgnInitCfgParam* pstParam:参数信息<非空>
*               EgnSnCfgParam* pstParamSn:参数信息<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、除特殊说明外，其他API应在成功初始化EGN管理线程成功调用该API之后才能被使用。
                2、在动态内存模式下不能调用该接口，否则会导致识别业务异常。
*        Since: V300R06C00
*    Reference: EgnApiSnInitPubParam
*               EgnApiSnMngDestroy
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnMngInit
(
   IN            EGN_UCHAR           *pucGlobalBuffer,
   IN            EGN_ULONG            ulBufferLen,
   IN  EGN_CONST EgnInitCfgParam     *pstParam,
   IN  EGN_CONST EgnSnCfgParam       *pstParamSn
);

/*******************************************************************************
*    Func Name: EgnApiSnInspectorInit
*      Purpose: 初始化识别实例。
*  Description: 引擎识别实例初始化接口，在多进程、多线程模型下调用，完成业务进程、线程的初始化，
                多实例时识别实例必须逐个的初始化。
*        Input: EGN_UINT16 usThreadID:调用此接口的线程ID<0~65535>
*               EGN_UCHAR* pucGlobalBuffer:共享线程内存池首地址<非空>
*               EGN_UINT32 ulBufferLen:共享线程内存池长度<大于零>
*        InOut: NA
*       Output: EGN_VOID** ppvHandle:实例句柄，初始化引擎为此实例创建的句柄，外部不能改变< >
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、除特殊说明外，其他API应在成功初始化EGN识别线程关联表共享时，成功调用该API之后才能被使用。
                2、在动态内存模式下不能调用该接口，否则会导致识别业务异常。
*        Since: V300R06C00
*    Reference: EgnApiSnInspectorDestroy
                EgnApiSnPacketInspect
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnInspectorInit
(
    IN     EGN_UINT16         usThreadID,
    IN     EGN_UCHAR         *pucGlobalBuffer,
    IN     EGN_ULONG          ulBufferLen,
    OUT    EGN_VOID         **ppvHandle
);

/*******************************************************************************
*    Func Name: EgnApiSnMngDestroy
*      Purpose: 去初始化管理线程
*  Description: 完成管理进程、线程系统资源的释放等工作。
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、该API在成功初始化EGN管理线程成功调用EgnApiSnMngInit之前可被调用。
                2、多进程、多线程模型下调用，单线程模型下不能调用本接口。
                3、在动态内存模式下不能调用该接口，否则会导致识别业务异常。
*        Since: V300R06C00
*    Reference: EgnApiSnMngInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnMngDestroy
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiSnInspectorDestroy
*      Purpose: 去初始化识别实例。
*  Description: 完成业务进程、线程系统资源的释放等工作。
*        Input: EGN_UINT16 usThreadID:调用此接口的线程ID<0~65535>
*               EGN_VOID** ppvHandle:实例句柄<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、多实例时识别实例必须逐个的去初始化。
                2、在动态内存模式下不能调用该接口，否则会导致识别业务异常。
*        Since: V300R06C00
*    Reference: EgnApiSnInspectorInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnInspectorDestroy
(
    IN  EGN_UINT16  usThreadID,
    IN  EGN_VOID  **ppvHandle
);

/*******************************************************************************
*    Func Name: EgnApiSnGetNeededMemSize
*      Purpose: 计算引擎所需内存。
*  Description: 该API接口根据最大流规模ulFlowScale、关联表大小ulRelationScale、识别实例个数usMaxInstance
                和内存是否共享bIsMemShared计算出管理实例所需共享内存大小pulSharedMemSize和每个识别实例所需
                非共享内存的大小pulUnSharedMemSize，单位字节。
*        Input: EgnInitCfgParam pstParam:流数，关联表相关<非空>
*               EgnSnCfgParam pstSnParam:Session功能专用<非空>
*               EGN_UINT16 usMaxInstance:识别实例个数，该值不能超过32<1~32>
*               EGN_BOOL bIsMemShared:是否共享内存<EGN_FALSE：不共享
                                                  EGN_TRUE：共享>
*        InOut: EGN_UINT32* pulSharedMemSize:管理实例所需共享内存大小<非空>
*               EGN_UINT32* pulUnSharedMemSize:每个识别实例所需非共享内存的大小<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、该API在使用EgnApiRegSspFunc成功注册系统服务回调函数之前可被调用。
                2、用户使用时严格按照需求传入相应参数，该API接口只能在内存机制为非受限的时候才有效。
                   即进行EGN参数配置时，将ulMemPolicy初始化为EGN_MEM_POLICY_UNLIMIT，EGN默认配置该参数为非受限。
                3、在动态内存模式下不能调用该接口，否则会导致识别业务异常。
*        Since: V300R06C00
*    Reference: EgnApiSnInitPubParam
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnGetNeededMemSize
(
    IN     EGN_CONST EgnInitCfgParam *pstParam,
    IN     EGN_CONST EgnSnCfgParam   *pstSnParam,
    IN               EGN_UINT16       usMaxInstance,
    IN               EGN_BOOL         bIsMemShared,
    INOUT            EGN_ULONG       *pulSharedMemSize,
    INOUT            EGN_ULONG       *pulUnSharedMemSize
);

/*******************************************************************************
*    Func Name: EgnApiSnRegDeCompressFn
*      Purpose: 注册解压缩的回调函数。
*  Description: 注册解压缩的回调函数。
*        Input: PFEgnSspDeCompress pfDeCompress:解压函数<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 该API在使用EgnApiRegSspFunc成功注册系统服务回调函数之前可被调用。
*        Since: V300R06C00
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnRegDeCompressFn
(
    IN  PFEgnSspDeCompress  pfDeCompress
);

/*******************************************************************************
*    Func Name: EgnApiSnGetIdleRelationCBCount
*      Purpose: 获取空闲的关联识别CB块的个数。
*  Description: 根据输入的实例线程ID和实例句柄，获取空闲的关联识别CB块的个数。
*        Input: EGN_UINT16 usThreadID:实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*        InOut: EGN_UINT32* pulIdleCBCountIPv4:Egn空闲的IPv4关联识别CB块的个数<非空>
*               EGN_UINT32* pulIdleCBCountIPv6:Egn空闲的IPv6关联识别CB块的个数<非空>
*               EGN_UINT32* pulIdleCBCountIPv4Sn:Session空闲的IPv4关联识别CB块的个数<非空>
*               EGN_UINT32* pulIdleCBCountIPv6Sn:Session空闲的IPv6关联识别CB块的个数<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、该API在初始化EGN识别线程（调用EgnApiExpandInspectorInit或调用EgnApiInspectorInit或调用EgnApiSnInspectorInit）之前不可被调用。
                2、当API在识别线程被调用时，必须输入调用线程的识别实例句柄，如果输入为空则多线程可靠性校验会失效。
                3、当API在管理线程被调用时，识别实例句柄传入必须为空，此时不会进行多线程可靠性校验。
*        Since: V300R06C00
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnGetIdleRelationCBCount
(
    IN      EGN_UINT16      usThreadID,
    IN      EGN_VOID       *pvHandle,
    INOUT   EGN_UINT32     *pulIdleCBCntIPv4,
    INOUT   EGN_UINT32     *pulIdleCBCntIPv6,
    INOUT   EGN_UINT32     *pulIdleCBCntIPv4Sn,
    INOUT   EGN_UINT32     *pulIdleCBCntIPv6Sn
);

/*******************************************************************************
*    Func Name: EgnApiSnGetSysInfoExt
*      Purpose: 收集SESSION配置信息。
*  Description: EGN一键式信息收集接口，根据输入的实例线程IDusThreadID和实例句柄pvHandle，
                将该实例的配置信息保存在首地址为pucSysInfo的内存中。
*        Input: EGN_UINT16 usThreadID:实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*               EGN_UINT32 ulSysInfoLen:保存配置信息内存总长度<大于零>
*        InOut: NA
*       Output: EGN_UCHAR* pucSysInfo:保存配置信息内存首地址，建议传入12k内存<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、在调用该接口之前要EGN与SESSION组件初始化完毕。
                2、当API在识别线程被调用时，必须输入调用线程的识别实例句柄，如果输入为空则多线程可靠性校验会失效。
                3、当API在管理线程被调用时，识别实例句柄传入必须为空，此时不会进行多线程可靠性校验。
                4、session在去使能状态下，通过EgnApiGetSysInfoExt获取信息，不显示CB块和知识库的信息。
                5、外部通过打印函数显示SESSION的配置信息
*        Since: V300R005C02
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnGetSysInfoExt
(
    IN            EGN_UINT16   usThreadID,
    IN            EGN_VOID    *pvHandle,
    OUT           EGN_UCHAR   *pucSysInfo,
    IN            EGN_UINT32   ulSysInfoLen
);

/*******************************************************************************
*    Func Name: EgnApiSnGetHealthInfo
*      Purpose: SN获取一键式检查组件健康信息。
*  Description: EGN一键式健康检查接口，完成EGN组件相关信息的健康检查，并将对应实例的健康信息
                存储在首地址为ppucHealthInfo的内存中。
*        Input: EGN_UINT16 usThreadID:实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*        InOut: NA
*       Output: EGN_UCHAR** ppucHealthInfo:保存健康检查信息内存首地址<非空>
*               EGN_UINT32* pulHealthInfoLen:保存健康检查信息内存长度<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、当API在识别线程被调用时，必须输入调用线程的识别实例句柄，如果输入为空则多线程可靠性校验会失效。
                2、当API在管理线程被调用时，识别实例句柄传入必须为空，此时不会进行多线程可靠性校验。
                3、在调用该接口之前要EGN组件初始化完毕，并且保证动态内存申请接口已经注册。
                4、在动态内存模式下不能调用该接口，否则会导致识别业务异常。
*        Since: V200R002C01
*    Reference: EgnApiSnFreeHealthInfo
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnGetHealthInfo
(
    IN            EGN_UINT16   usThreadID,
    IN            EGN_VOID    *pvHandle,
    OUT           EGN_UCHAR  **ppucHealthInfo,
    OUT           EGN_UINT32  *pulHealthInfoLen
);

/*******************************************************************************
*    Func Name: EgnApiSnFreeHealthInfo
*      Purpose: Sn释放一键式健康检查信息的内存。
*  Description: 通过输入实例线程ID和实例句柄，释放该实例健康检查内存ppucHealthInfo中的信息。
*        Input: EGN_UINT16 usThreadID:实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*        InOut: NA
*       Output: EGN_UCHAR** ppucHealthInfo:保存健康检查信息内存首地址<非空>
*       Return: EGN_VOID，无
*      Caution: 1、当API在识别线程被调用时，必须输入调用线程的识别实例句柄，如果输入为空则多线程可靠性校验会失效。
                2、当API在管理线程被调用时，识别实例句柄传入必须为空，此时不会进行多线程可靠性校验。
                3、在动态内存模式下不能调用该接口，否则会导致识别业务异常。
*        Since: V200R002C01
*    Reference: EgnApiSnGetHealthInfo
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_VOID EgnApiSnFreeHealthInfo
(
    IN              EGN_UINT16    usThreadID,
    IN              EGN_VOID     *pvHandle,
    INOUT           EGN_UCHAR   **ppucHealthInfo
);

/*******************************************************************************
*    Func Name: EgnApiSnGetMngInfo
*      Purpose: session组件接口使用风格统一，管理面信息查询接口
*  Description: 1.在调用该接口之前要session及EGN组件初始化完毕
*               2.外部可根据获取信息的类型明确获取信息的数据结构，具体的数据结构在结构体EgnGetCfgInfo中,
*                该结构体中的共用体对应各种数据结构，数据结构中统一命名方式:入参，含有In;入出参，含有InOut;出参，含有Out。对于指针类型的变量需用户根据其传入的个数分配内存。
*                (具体类型与返回结果的对应关系见EGN_UNITE_GET_SET_EN)
*        Input: EGN_UINT32      ulInfoEnum:查询类型<EGN_UNITE_GET_SET_EN
*                (ulInfoEnum的范围"EGN_UNITE_GET_SET_ENGINE_VERSION~EGN_UNITE_GET_SET_SN_BODY_MEM_DECOMPRESS"，除去EGN_UNITE_GET_SET_MATCHED_RULE_INFO。
*                 其中EGN_UNITE_GET_SET_STATIC_BLOCK_INFO、EGN_UNITE_GET_SET_CB_STATIC_INFO、EGN_UNITE_GET_SET_SN_CB_STATIC_INFO、EGN_UNITE_GET_SET_SN_STATIC_BLOCK_INFO类型的信息只有内存为共享模式时，才能通过该接口获取)>
*        InOut: EgnGetCfgInfo*    pstGetCfgInfo:获取到的信息首地址<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006C00
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnGetMngInfo
(
    IN     EGN_UINT32       ulInfoEnum,
    INOUT  EgnGetCfgInfo   *pstGetCfgInfo
);

/*******************************************************************************
*    Func Name: EgnApiSnGetInspectorInfo
*      Purpose: session组件接口使用风格统一，业务面信息查询接口
*  Description: 1.在调用该接口之前要session及EGN组件初始化完毕
*               2.外部可根据获取信息的类型明确获取信息的数据结构，具体的数据结构在结构体EgnGetCfgInfo中,
*                该结构体中的共用体对应各种数据结构，数据结构中统一命名方式:入参，含有In;入出参，含有InOut;出参，含有Out。对于指针类型的变量需用户根据其传入的个数分配内存。
*                (具体类型与返回结果的对应关系见EGN_UNITE_GET_SET_EN)
*               3.该API不能在管理线程调用，只能在识别线程调用
*               4.必须输入调用线程的识别实例句柄和对应的线程id
*        Input: EGN_UINT32  ulInfoEnum:查询类型<EGN_UNITE_GET_SET_EN
*               (ulInfoEnum的范围"EGN_UNITE_GET_SET_STATIC_BLOCK_INFO, EGN_UNITE_GET_SET_MATCHED_RULE_INFO",
                "EGN_UNITE_GET_SET_SN_CB_STATIC_INFO, EGN_UNITE_GET_SET_SN_STATIC_BLOCK_INFO"(session相关配置信息)>
*               EGN_UINT16  usThreadID:实例线程ID<0~65535>
*               EGN_VOID*    pvHandle:实例handle<非空>
*        InOut: EgnGetCfgInfo*  pstGetCfgInfo:获取到的信息首地址<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006C00
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnGetInspectorInfo
(
    IN        EGN_UINT32      ulInfoEnum,
    IN        EGN_UINT16      usThreadID,
    IN        EGN_VOID       *pvHandle,
    INOUT     EgnGetCfgInfo  *pstGetCfgInfo
);

/*******************************************************************************
*    Func Name: EgnApiSnSetCfgParamInfo
*      Purpose: session组件接口使用风格统一，统一设置配置信息，分为静态设置与动态设置
*  Description: 1.静态设置:调用时机是在初始化之前，且注册了回调函数之后，否则返回失败。
*                 如果在初始化默认参数之后调用，即在调用EgnApiSnInitPubParam之后调用该接口，设置的配置信息无效，但返回成功。
*                 静态设置的参数成功且设置有效，该接口的调用时机是在初始化默认参数之前，注册了回调函数之后，且系统未初始化
*               2.动态设置:EGN组件初始化完毕,否则返回失败
*               3.外部可根据设置信息的类型明确设置信息的数据结构，具体的数据结构在结构体EgnSetCfgParamInfo中,
*                该结构体中的共用体对应各种数据结构，数据结构中统一命名方式:入参，含有In;入出参，含有InOut;出参，含有Out。对于指针类型的变量需用户根据其传入的个数分配内存。
*                (具体类型与返回结果的对应关系见EGN_UNITE_GET_SET_EN)。
*               4.该接口可设置session相关配置信息及EGN相关配置信息。
*        Input: EGN_UINT32      ulSetType:设置的类型<0:静态，1:动态>
*               EGN_UINT32      ulEnumNum:查询类型<EGN_UNITE_GET_SET_EN
*                 (ulInfoEnum的范围，静态设置:"EGN_UNITE_GET_SET_DETECT_THRESHOLD, EGN_UNITE_GET_SET_UDRULE_SWITCH_INFO",
                  "EGN_UNITE_GET_SET_CFG_PARA_IS_SHARE, EGN_UNITE_GET_SET_UD_BACK_RULE_LIB_NEED_MEM"
*                  "EGN_UNITE_GET_SET_SN_SWITCH, EGN_UNITE_GET_SET_SN_BODY_MEM_DECOMPRESS"(session相关配置信息)
*                 动态设置:"EGN_UNITE_GET_SET_DETECT_THRESHOLD, EGN_UNITE_GET_SET_IP_FLOW_AGED_TIME", "EGN_UNITE_GET_SET_PROTO_IMPORT_STATE, EGN_UNITE_GET_SET_STAT_CONTROL")>
*               EGN_VOID*    pvHandle:实例handle<非空>InOut: EgnSetCfgParamInfo*    pstSetCfgParamInfo:待设置的信息
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006C00
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnSetCfgParamInfo
(
    IN      EGN_UINT32              ulSetType,
    IN      EGN_UINT32              ulEnumNum,
    INOUT   EgnSetCfgParamInfo     *pstSetCfgParamInfo
);

/*******************************************************************************
*    Func Name: EgnApiSnExpendInspectorInit
*      Purpose: 会话关联模式下初始化识别实例。
*  Description: 引擎识别实例初始化接口，在多进程、多线程模型下调用，完成业务进程、线程的初始化，多实例时识别实例必须逐个的初始化。
*        Input: EGN_UINT16  usThreadID：调用此接口的线程ID<0~65535>
                EGN_UCHAR*  pucSharedBuffer：共享线程内存池首地址<非空>
                EGN_ULONG   ulBufferLen：共享线程内存池长度<大于零>
                EGN_UCHAR*  pucUnsharedBuffer：非共享线程内存池首地址<非空>
                EGN_ULONG   ulUnsharedBufferLen：非共享线程内存池长度<大于零>
*        InOut: NA
*       Output: EGN_VOID**  ppvHandle：实例句柄，初始化引擎为此实例创建的句柄，外部不能改变<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006C00
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32  EgnApiSnExpendInspectorInit
(
   IN  EGN_UINT16           usThreadID,
   IN  EGN_UCHAR           *pucSharedBuffer,
   IN  EGN_ULONG            ulSharedBufferLen,
   IN  EGN_UCHAR           *pucUnsharedBuffer,
   IN  EGN_ULONG            ulUnsharedBufferLen,
   OUT EGN_VOID           **ppvHandle
);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EGN_API_COMMON_H__ */

