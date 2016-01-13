/*
 ******************************************************************************
 版权所有 (C), 2008-2009, 华为技术有限公司
 ******************************************************************************
  文 件 名   : egnapicommon.h
  版 本 号   : 初稿
  作    者   : EGN项目组
  生成日期   : 2008年12月18日
  最近修改   :
  功能描述   : 公共宏定义
  函数列表   :
  修改历史   :
  1.日    期   : 2009年2月2日
    作    者   : EGN项目组
    修改内容   : 创建文件

******************************************************************************/
/**@file  egnapicommon.h
  *    公共宏定义
*******************************************************/
/**
 * @defgroup egn  EGN协议识别模块的API
 */

#ifndef __EGN_API_COMMON_H__
#define __EGN_API_COMMON_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/** EGN软件产品版本定义 */
#define EGN_SOFTWARE_EX_VER "NSE V300R006C00SPC201"

/** EGN软件内部子版本定义 */
#define EGN_INNER_VER "B501"

/** EGN软件内部版本号 */
#define EGN_SOFTWARE_IN_VER EGN_SOFTWARE_EX_VER/**/""EGN_INNER_VER""/**/EGN_PRODUCT\
 "/"/**/EGN_OSVER"/"/**/EGN_CPU" ["__DATE__" "__TIME__"][BinChkKK]"

/** EGN软件兼容版本 */
#define EGN_COMPATI_VERSION 0002

/** EGN Rule Ver */
#define EGN_RULEINFO_VERSION 0005

/** 查询知识库信息用 */
#define EGN_UNKNOW_CLASS_ID     0xffffffff      /* 无效的ClassID */
#define EGN_UNKNOW_APP_ID       0xffffffff      /* 无效的AppID */
#define EGN_UNKNOW_PROTO_ID     0xffffffff      /* 无效的ProtocolID */

#define EGN_SYSINFO_BUF_LEN     12288           /*  获取配置信息时，保存该信息申请的内存大小 */

/** 初始化参数Magic有效定义 */
#define EGN_INIT_PUB_PARA_MAGIC_VALID   (0x6C)

/** Protocol中PPID最大个数 */
#define EGN_MAX_PARENT_PID         8

/** 内存页初始化种类个数 */
#define EGN_MEMCP_INIT_MAX_COUNT   (10)

/** 最大属性维度数目 */
#define EGN_MAX_ATTRIBUTE_NUM  (64)

/** 支持设置快速老化时间的下限限 */
#define EGN_RELATION_MIN_AGING_TIME     3   /* 关联表老化时间的最小值 */

/** 支持设置快速老化时间的上限限 */
#define EGN_RELATION_MAX_AGING_TIME     30  /* 关联表老化时间的最大值 */

/** 支持的最大同步的peer数 */
#define EGN_SYN_PEER_MAX_NUM 20

/* 统计识别处理的报文负载最大长度 */
#define EGN_STAT_MAX_PACKET_LOADLEN     1600

/* 一次匹配最多返回多少个匹配结果 */
#define EGN_HA_RETURN_PATT_MAX  256

#ifdef EGN_GREEDY_MATCH

/* 最多附加识别协议个数 */
#define EGN_MAX_APPEND_RESULT_NUM   64
#endif

/** 归一化查询维度掩码 */
#define EGN_DBG_QUERY_ENGINE_VERSION        0x01     /* 查询引擎版本号 */
#define EGN_DBG_QUERY_DETECT_THRESHOLD      0x02     /* 查询识别阀值 */
#define EGN_DBG_QUERY_RULELIB_VERSION       0x04     /* 查询知识库版本号 */
#define EGN_DBG_QUERY_PEER_INFO             0x08     /* 查询peer信息 */
#define EGN_DBG_QUERY_RELTBL_INFO           0x10     /* 查询关联表状态 */
#define EGN_DBG_QUERY_PKT_STAT_INFO         0x20     /* 查询报文统计信息 */
#define EGN_DBG_QUERY_STATICBP_INFO         0x40     /* 查询静态内存分配信息 */
#define EGN_DBG_QUERY_STATBP_INFO           0x80     /* 查询统计内存分配信息 */
#define EGN_DBG_QUERY_ALL_INFO              0xff     /* 查询以上所有信息 */

#define  EGN_MAX_DISABLED_PROTO_NUM 15000   /* 最大去使能协议个数 */

/** 系统协议ID最大值(小于,不能等于最大值) */
#define EGN_SYS_PROTO_ID_MAX    60000

/*STRUCT< 字符串结构体 >*/
typedef struct _EgnString
{
    EGN_UCHAR  *pucString;       /*  字符串指针 */
    EGN_UINT32  ulLen;           /*  字符串长度 */
#ifdef EGN_64
    EGN_UINT8   aucReserved[4];  /* 64位对齐保留字节  */
#endif
}EgnString;

/*ENUM< 地址类型定义 >*/
typedef enum
{
    EGN_ADDR_TYPE_INVALID   = EGN_EN_INVALID,
    EGN_ADDR_TYPE_IPV4      = 0,    /* IPV4 */
    EGN_ADDR_TYPE_IPV6      = 1,    /* IPV6 */
    EGN_ADDR_TYPE_ANY       = 2,    /* Any */
    EGN_ADDR_TYPE_END,

    EGN_ADDR_TYPE_BOTTOM     =  EGN_EN_BUTT
}EGN_ADDR_TYPE_EN;

/*ENUM< 报文方向 >*/
typedef enum
{
    EGN_PACKET_DIRECTION_BEGIN    = EGN_EN_INVALID,
    EGN_PACKET_DIRECTION_UP       = 0,  /* 上行的报文 */
    EGN_PACKET_DIRECTION_DOWN     = 1,  /* 下行的报文 */
    EGN_PACKET_DIRECTION_ANY      = 2,  /* UpLink and DownLink */
    EGN_PACKET_DIRECTION_END,

    EGN_PACKET_DIRECTION_BOTTOM   = EGN_EN_BUTT
}EGN_PACKET_DIRECTION_EN;

#ifdef EGN_HA
/*ENUM< HA识别结果 >*/
typedef enum
{
    EGN_HA_INPUT_PACKET_BEGIN       = EGN_EN_INVALID,
    EGN_HA_INPUT_PACKET_STOP        = 0,    /* 停止 */
    EGN_HA_INPUT_PACKET_SOFT        = 1,    /* 送软件 */
    EGN_HA_INPUT_PACKET_HARD        = 2,    /* 送硬件加速 */
    EGN_HA_INPUT_PACKET_END,
    EGN_HA_INPUT_PACKET_BOTTOM = EGN_EN_BUTT
}EGN_HA_INSPECT_PACKET_EN;
#endif

/*ENMU< 硬件加速模式串集类型定义 >*/
typedef enum
{
    EGN_HA_PATTERN_SET_TYPE_BEGIN   = EGN_EN_INVALID,
    EGN_HA_PATTERN_SET_TYPE_INVALID = 0,
    EGN_HA_PATTERN_SET_TYPE_TCP     = 1,
    EGN_HA_PATTERN_SET_TYPE_UDP     = 2,
    EGN_HA_PATTERN_SET_TYPE_HTTP    = 3,
    EGN_HA_PATTERN_SET_TYPE_SOFT    = 4,
    EGN_HA_PATTERN_SET_TYPE_END,

    EGN_HA_PATTERN_SET_TYPE_BOTTOM  = EGN_EN_BUTT
} EGN_HA_PATTERN_SET_TYPE_EN;

/*ENMU< 硬件加速模式串类型> */
typedef enum
{
    EGN_HA_PATTERN_TYPE_BEGIN           = EGN_EN_INVALID,
    EGN_HA_PATTERN_TYPE_CONTENT_STRING  = 0,
    EGN_HA_PATTERN_TYPE_PCRE            = 1,
    EGN_HA_PATTERN_TYPE_END,

    EGN_HA_PATTERN_TYPE_BOTTOM          = EGN_EN_BUTT
} EGN_HA_PATTERN_TYPE_EN;

/*MACRO< 未使用的参数或变量，消除PC-LINT告警. >*/
#define EGN_UNUSED(_x) ((_x) = (_x))

/*STRUCT< IP地址 >*/
typedef struct _EgnIpAddr
{
    EGN_UINT32   ulIpType;       /* IP类型#EGN_ADDR_TYPE_EN */
#ifdef EGN_64
    EGN_UINT8    aucReserved[4]; /* 64位对齐保留字节 */
#endif
    union
    {
        EGN_UINT32   ulIpv4Addr;        /* IPv4地址，必须为大端 */
        EGN_UINT8    aucIpv4Addr[4];    /* IPv4地址，必须为大端 */
        EGN_UINT32   aulIpv6Addr[4];    /* IPv6地址，必须为大端 */
        EGN_UINT8    aucIpv6Addr[16];   /* IPv6地址，必须为大端 */
    }u;
}EgnIpAddr;

/*STRUCT< 大类信息 >*/
typedef struct _EgnSuperClassInfo
{
    EGN_UINT32  ulSuperClassID;    /* 大类ID */
    EGN_UINT32  ulIsUserDefine;    /* EGN_SYS_DEFINE:系统协议#EGN_USER_DEFINE：自定义协议,为后续版本扩展而保留 */
    EgnString   stSuperClassName;  /* 大类名称 */
    EgnString   stSuperClassDesc;  /* 大类描述 */
}EgnSuperClassInfo;

/*STRUCT< 子类信息 >*/
typedef struct _EgnClassInfo
{
    EGN_UINT32  ulClassID;      /* 子类ID */
    EGN_UINT32  ulSuperClassID; /* 对应的大类ID */
    EGN_UINT32  ulIsUserDefine; /* EGN_SYS_DEFINE:系统协议#EGN_USER_DEFINE：自定义协议,为后续版本扩展而保留 */
#ifdef EGN_64
    EGN_UINT8   aucReserved[4]; /* 64位对齐保留字节 */
#endif
    EgnString   stClassName;    /* 子类名称 */
    EgnString   stClassDesc;    /* 子类描述 */
}EgnClassInfo;

/*STRUCT< 应用信息 >*/
typedef struct _EgnAppInfo
{
    EGN_UINT32  ulAppID;        /* 应用ID */
    EGN_UINT32  ulIsUserDefine; /* EGN_SYS_DEFINE:系统协议#EGN_USER_DEFINE：自定义协议,为后续版本扩展而保留 */
    EgnString   stAppName;      /* 应用名称 */
    EgnString   stAppDesc;      /* 应用描述 */
}EgnAppInfo;

/*STRUCT< 协议信息 >*/
typedef struct _EgnProtoInfo
{
    EGN_UINT32  ulSuperClassID;     /* 大类ID */
    EGN_UINT32  ulClassID;          /* 子类ID */
    EGN_UINT32  ulProtoID;          /* 协议ID */
    EGN_UINT32  ulAppID;            /* 应用ID */
    EGN_UINT32  ulParentPIDNum;     /* ParentID个数 */
    EGN_UINT8   ucIsUserDefine;     /* EGN_SYS_DEFINE:系统协议，#EGN_USER_DEFINE：自定义协议 */
    EGN_UINT8   aucReserved[3];     /* 字节对齐保留 */
    EgnString   stProtoName;        /* 协议名称 */
    EgnString   stProtoDesc;        /* 协议描述 */
    EGN_UINT32  aulParentPID[EGN_MAX_PARENT_PID];   /* ParentID内容 */
}EgnProtoInfo;

/*STRUCT< IP分片 >*/
typedef struct _EgnIpFrag
{
    struct _EgnIpFrag      *pstNext;            /* 下一个分片 */
    EGN_UCHAR              *pucLoadData;        /* 指向分片报文传输层载荷的指针 */
    EGN_UINT16              usLoadLen;          /* 分片报文传输层载荷长度 */
    EGN_UINT8               aucReserved[2];     /* 保留字节  */
#ifdef EGN_64
    EGN_UINT8               aucReserved2[4];    /* 64位对齐保留字节 */
#endif
} EgnIpFrag;

/*STRUCT< 识别数据包 >*/
typedef struct _EgnPacket
{
    EGN_UINT8 *pucIpData;       /* IP头指针 */
    EGN_UINT8 *pucTpData;       /* 传输层头指针 */
    EgnIpFrag *pstIpFrag;       /* IP分片传输层载荷链表 */
    EGN_UINT8  ucDirection;     /* 对应于#EGN_PACKET_DIRECTION_EN */
    EGN_UINT8  ucProto;         /* 传输层协议类型，对应于#EGN_TRANS_TYPE_EN */
    EGN_UINT8  aucReserved[2];
#ifdef EGN_64
    EGN_UINT8  aucReserved2[4]; /* 64位对齐保留字节 */
#endif
    EgnIpAddr  stSrcIp;         /* 源IP地址 */
    EgnIpAddr  stDstIp;         /* 目的IP地址 */
    EGN_UINT16 usSrcPort;       /* 源端口 */
    EGN_UINT16 usDstPort;       /* 目的端口 */
    EGN_UINT32 ulTunnelId;      /* 应用层指定的VPN ID或Tunnel ID，用来进行关联识别 */
    EGN_VOID * pvFlowHandle;    /* 数据包所属的流句柄 */
}EgnPacket;

#ifdef EGN_GREEDY_MATCH
/* 贪婪识别的识别结果 */
typedef struct _EgnGreedyResult
{
    EGN_UINT32  ulRuleID;               /* 规则ID */
    EGN_UINT32  ulProtocol;             /* 协议ID，只需要处理一层 */
}EgnGreedyResult;
#endif

/*STRUCT< 识别结果 >*/
typedef struct _EgnResult
{
    EGN_UINT32      ulResult;              /* 识别结果，参考#EGN_INSPECT_EN */
    EGN_UINT32      ulProtoID;             /* 识别协议ID，对应到发布件中"知识库-类别应用协议ID.XLS"中页面"协议_子协议关系"列sub_prot */
    EGN_BOOL        bIsContinue;           /* 是否需要继续灌包 */
    EGN_UINT32      ulCarrierNum;          /* 承载协议个数 */
    EGN_UINT32      aulCarrierProtoID[4];  /* 承载协议ID列表，0为最底层，依次往上递增 */
    EGN_VOID       *pvFlowHandle;          /* 数据流是通过哪条流关联识别出 */
    EGN_UINT32      aulAttribute[EGN_MAX_ATTRIBUTE_NUM]; /* 该流的具体属性信息 */
    #ifdef EGN_GREEDY_MATCH
    EGN_UINT32      ulFirstProtoID;         /* 首次识别的协议ID */
    EGN_UINT32      ulAppendResultCnt;      /* 附加识别协议个数 */
    EgnGreedyResult astAppendResult[EGN_MAX_APPEND_RESULT_NUM]; /* 附加识别协议信息 */
    #endif
    EGN_UINT32      ulSuperClassId;        /* 识别结果最上层的大类id */
    EGN_UINT32      ulClassId;             /* 识别结果最上层的子类id */
    EGN_UINT32      ulAppId;               /* 识别结果最上层的应用id */
#ifdef EGN_64
    EGN_UINT8       aucReserved[4];        /* 8字节对齐保留 */
#endif
}EgnResult;

#ifdef EGN_HA
/*STRUCT< 硬件识别结果 >*/
typedef struct _EgnHAResult
{
    EGN_UINT32      ulContinue;     /* 是否需要继续输入数据包, EGN_HA_INSPECT_PACKET_EN */
    EGN_UINT32      ulResult;       /* 识别结果,对应于#EGN_INSPECT_EN */
    EGN_UINT32      ulProtoID;      /* 协议ID */
#ifdef EGN_64
    EGN_UINT8       aucReserved[4]; /* 64位对齐保留字节 */
#endif
    EGN_VOID       *pvFlowHandle;   /* 关联识别数据包所属的控制流句柄  */
}EgnHAResult;
#endif

/*STRUCT< 识别扩展参数，用于后续扩展 >*/
typedef struct _EgnInspectAuxData
{
    EGN_UINT32     ulAuxData;       /* 扩展数据 */
    EGN_UINT32     ulReserved;      /* 保留字节 */
} EgnInspectAuxData;

/*STRUCT< 初始化参数配置 >*/
typedef struct _EgnInitCfgParam
{
    /* 内部使用，产品不需关注 */
    EGN_UCHAR   ucCfgValidMagic;            /* Cfg参数有效Magic,用户无需关注,请不要修改 */
    EGN_UINT8   aucReserved[3];             /* 保留字，四字节对齐。  */

    /* 所需内存相关配置参数 */
    EGN_BOOL    bIsCfgParaShared;           /* 配置信息是否共享，EGN_FALSE: 否；EGN_TRUE : 是， 默认: 是 */
    EGN_BOOL    bIsRuleLibShared;           /* 知识库是否共享，  EGN_FALSE: 否；EGN_TRUE : 是， 默认: 是 */
    EGN_BOOL    bIsMemShared;               /* 是否共享内存，  EGN_FALSE: 否；EGN_TRUE : 是， 默认: 是 */

    EGN_UINT16  usMaxInstance;              /* 最大的实例个数, 单实例时实例数为1，多实例时实例最大数32 */
    EGN_UINT8   aucReserved1[2];            /* 保留字，四字节对齐。  */
    EGN_UINT32  ulMemPolicy;                /* 内存策略，取值EGN_MEM_POLICY_EN  */
    EGN_UINT32  ulFlowScale;                /* 流规模  */
    EGN_UINT32  ulRelationCBCountIPv4;      /* IPv4关联识别CB块的数目 */
    EGN_UINT32  ulRelationCBCountIPv6;      /* IPv6关联识别CB块的数目 */
    EGN_UINT32  ulDnsRelCBCountIPv4;        /* DNS关联识别IPv4的CB块的数目 */
    EGN_UINT32  ulDnsRelCBCountIPv6;        /* DNS关联识别IPv6的CB块的数目 */

    /* 上限可配置参数 */
    EGN_UINT32  ulMaxDetectCount;           /* 识别阀值设置 */
    EGN_UINT32  ulMaxRelAgeNum;             /* 关联表老化上限 */
    EGN_BOOL    bIsNeedSuperClass;          /* 是否需要在识别结果中返回应用、子类和大类id信息，EGN_FALSE: 关；EGN_TRUE : 开， 默认: 关 */

    EGN_UINT32  ulFastRelationAgingTime;    /* 快速老化表使用的时间，单位  分钟 */
    EGN_UINT32  ulRollBackKeepTime;         /* 回退保持时长，单位秒，默认28800  */
    EGN_UINT32  ulIpFlowAgedTime;           /* 流表老化时长，单位秒，默认20，最小3秒，最大2小时 */
    EGN_UINT16  usScanInterval;             /* 老化扫描的时间间隔，单位毫秒，默认200毫秒 */
    EGN_UINT8   aucReserved4[2];            /* 保留字，四字节对齐。  */

    /* 可配置功能开关 */
    EGN_BOOL    bFastRelationSwitch;        /* 快速关联开关，EGN_FALSE: 关；EGN_TRUE : 开， 默认: 开 */
    EGN_BOOL    bUseWellKnownPortInspect;   /* 知名端口模糊匹配功能是否开启开关,EGN_FALSE: 关；EGN_TRUE : 开， 默认: 开 */
    EGN_BOOL    bHierInspectSwitch;         /* 层级识别命中非同包识别功能全局开关，EGN_FALSE: 关；EGN_TRUE : 开， 默认: 关 */
    EGN_BOOL    bBehaviorInspect;           /* 行为特征识别开关，EGN_FALSE: 关；EGN_TRUE : 开， 默认: 开 */
    EGN_BOOL    bStatSwitch;                /* 统计功能开关，EGN_FALSE: 关；EGN_TRUE : 开， 默认: 关 */
    EGN_BOOL    bIsNotFirstPKTRelInspect;   /* 是否支持非首包关联识别开关，EGN_FALSE: 关；EGN_TRUE : 开， 默认: 关 */
    EGN_BOOL    bUnsymmetricalInspect;      /* 是否支持非对称识别开关，EGN_FALSE: 关；EGN_TRUE : 开， 默认: 关 */
    EGN_BOOL    bIsStatInspectSwitch;       /* 统计识别功能开关，EGN_FALSE: 关；EGN_TRUE : 开， 默认: 关 */
    EGN_BOOL    bUDRuleSwitch;              /* 是否自定义规则开关，EGN_FALSE: 关；EGN_TRUE : 开， 默认: 关 */
    EGN_BOOL    bNgfwUserDefineSwitch;      /* 是否NGFW自定义开关，EGN_FALSE: 关；EGN_TRUE : 开， 默认: 关 */
    EGN_BOOL    bDnsRelSwitch;              /* DNS关联识别开关，EGN_FALSE: 关；EGN_TRUE : 开， 默认: 关 */
    EGN_BOOL    bFragSwitch;                /* 是否分片识别开关，EGN_FALSE: 关；EGN_TRUE : 开， 默认: 关 */
    EGN_BOOL    bIsHardwareAccelerate;      /* 是否使用硬件加速功能，EGN_FALSE: 关；EGN_TRUE : 开， 默认: 关 */
    EGN_BOOL    bWeakRelationSwitch;        /* 是否使用弱关联识别开关，EGN_FALSE: 关；EGN_TRUE : 开， 默认: 开 */
}EgnInitCfgParam;

/*STRUCT< 规则统计信息 >*/
typedef struct _EgnRuleStatRd
{
    EGN_UINT32   ulRuleId;       /* 规则id */
    EGN_UINT32   ulMatchCnt;     /* 规则匹配次数 */
    EGN_UINT32   ulSuccCnt;      /* 规则匹配成功次数 */
#ifdef EGN_64
    EGN_UINT8    ucReserved[4];  /* 为64位保留 */
#endif
}EgnRuleStatRd;

/*ENUM< 内存策略计算方式定义 >*/
typedef enum
{
    EGN_MEM_POLICY_BEGIN                 = EGN_EN_INVALID,
    EGN_MEM_POLICY_UNLIMIT               = 0,                /* 默认值; 保证EGN使用的内存 */
    EGN_MEM_POLICY_LIMIT                 = 1,                /* 限制内存大小，保证中间状态内存 */
    EGN_MEM_POLICY_ONLY_LOAD_TXT_RULELIB = 2,                /* 只加载明文库内存模式 */
    EGN_MEM_POLICY_END,
    EGN_MEM_POLICY_BOTTOM       = EGN_EN_BUTT
}EGN_MEM_POLICY_EN;

/*ENUM< 某条规则传输类型定义 >*/
typedef enum
{
    EGN_TRANS_TYPE_BEGIN = EGN_EN_INVALID,
    EGN_TRANS_TYPE_TCP   = 0x6,            /* TCP  类型 */
    EGN_TRANS_TYPE_UDP   = 0x11,           /* UDP  类型 */
    EGN_TRANS_TYPE_END,

    EGN_TRANS_TYPE_BOTTOM = EGN_EN_BUTT
}EGN_TRANS_TYPE_EN;

/*ENUM< 统计单元 >*/
typedef enum
{
    EGN_STAT_UNIT_BEGIN      = EGN_EN_INVALID,
    EGN_STAT_UNIT_INSPECT    = 0,      /* 对应统计参数:包相关统计项 */
    EGN_STAT_UNIT_RELTBL     = 1,      /* 对应统计参数:关联表相关统计项 */
    EGN_STAT_UNIT_END,

    EGN_STAT_UNIT_BOTTOM     = EGN_EN_BUTT
}EGN_STAT_UNIT_EN;

/*ENUM< 统计项 >*/
typedef enum
{
    EGN_STAT_ENTITY_BEGIN                   = EGN_EN_INVALID,
    EGN_STAT_ENTITY_PACKET_COUNT            = 0,                /* 统计所有包 */
    EGN_STAT_ENTITY_SUCCESS_COUNT           = 1,                /* 统计识别成功的包 */
    EGN_STAT_ENTITY_UNKNOWN_COUNT           = 2,                /* 统计未识别的包 */
    EGN_STAT_ENTITY_RELITEM_ADDED_COUNT     = 3,                /* 统计指定协议添加的关联表项个数 */
    EGN_STAT_ENTITY_RELITEM_DESTROYED_COUNT = 4,                /* 统计指定协议关联表项销毁个数 */
    EGN_STAT_ENTITY_RELITEM_HITED_COUNT     = 5,                /* 统计指定协议命中的关联表项个数 */
    EGN_STAT_ENTITY_RELITEM_HITED_TIMES     = 6,                /* 指定协议销毁的和被替换的表项被命中的次数之和 */
    EGN_STAT_ENTITY_RELITEM_LIFE_SUM        = 7,                /* 统计指定协议关联表项销毁时所有的时间差之和 */
    EGN_STAT_ENTITY_REL_QUERY_TIMES         = 8,                /* 统计所有协议关联表查询次数 */
    EGN_STAT_ENTITY_REL_QUERY_SUCCESS_TIMES = 9,                /* 统计所有协议关联表查询成功次数 */
    EGN_STAT_ENTITY_FINISH_COUNT            = 10,               /* 统计识别完成的包 */
    EGN_STAT_ENTITY_RELATION_COUNT          = 11,               /* 统计需要关联识别的包 */

    EGN_STAT_ENTITY_END,

    EGN_STAT_ENTITY_BOTTOM              = EGN_EN_BUTT
}EGN_STAT_ENTITY_EN;

/*ENUM< 错误统计的统计项 >*/
typedef enum
{
    EGN_STAT_ERR_BEGIN               = EGN_EN_INVALID,
    EGN_ERR_ALLOC_FAILED                 = 0,                    /* 分配内存失败 */
    EGN_ERR_MM_REPEAT_FREE               = 1,                    /* bufmem重复释放 */
    EGN_ERR_RELOAD                       = 2,                    /* 重复加载 */
    EGN_ERR_RELEASE_IS_GOING             = 3,                    /* 备知识库正在释放中 */
    EGN_ERR_CREAT_BUF_MEM_FAILED         = 4,                    /* 创建bufmem失败 */
    EGN_ERR_THREAD_ID_INCON_INSTANCE     = 5,                    /* 上层输入线程ID与实例中保存的不一致 */
    EGN_ERR_THREAD_ID_INCON_CTX          = 6,                    /* 上层输入线程ID与中间状态保存的不一致 */
    EGN_ERR_PEERLIST_IS_FULL             = 7,                    /* peerlist表满，非空 */
    EGN_ERR_PEERSET_CACHE_NOT_ENOUGH     = 8,                    /* 批量添加三元组时的cache的空间不足 */
    EGN_ERR_INSPECT_TIME_OUT             = 9,                    /* 识别超时，立即返回的错误码 */
    EGN_ERR_ALLOC_OVERWRITE_FAILED       = 10,                   /* 内存分配过程中发现写越界错误 */
    EGN_ERR_PEERLIST_FLOW_PEER_IS_MAX    = 11,                   /* 整条控制流可以添加关联表个数已经达到上限 */
    EGN_ERR_PEERLIST_MAX_COLLISION       = 12,                   /* peerlist冲突达到最大个数 */
    EGN_ERR_STAT_DETECT_ALLOC_FAILED     = 13,                   /* 统计识别内存分配失败 */
    EGN_ERR_STAT_DETECT_CREATE_BUF_FAILED = 14,                  /* 统计识别创建内存bufmem失败 */

    EGN_STAT_ERR_END,
    EGN_STAT_ERR_BOTTOM              = EGN_EN_BUTT
}EGN_STAT_ERROR_EN;

/*ENUM< 关联表项统计控制选项 >*/
typedef enum
{
    EGN_STAT_CTRL_BEGIN     = EGN_EN_INVALID,
    EGN_STAT_ONOFF_SWITCH   = 0,             /* 统计开关，输入为一个BOOL值，1表示开，0表示关 */
    EGN_STAT_PROTOID_SET    = 1,             /* 指定协议ID，输入为一个UINT32类型的协议ID */
    EGN_STAT_CTRL_END,

    EGN_STAT_CTRL_BOTTOM    = EGN_EN_BUTT
}EGN_STAT_CTRL_EN;

/*STRUCT< 关联统计项结构体 >*/
typedef struct _EgnRelStatInfo
{
    EGN_UINT32  ulRelItemAddedCnt;      /* 指定协议添加的关联表项个数 */
    EGN_UINT32  ulRelItemDestroyedCnt;  /* 指定协议关联表项销毁个数 */
    EGN_UINT32  ulRelItemHitedCnt;      /* 指定协议命中的关联表项个数 */
    EGN_UINT32  ulRelItemHitedTimes;    /* 指定协议销毁的和被替换的表项被命中的次数之和 */
    EGN_UINT32  ulRelItemLifeSum;       /* 指定协议关联表项销毁时所有的时间差之和,该统计项以关联老化定时器步长为单位 */
    EGN_UINT32  ulRelQueryTimes;        /* 所有协议关联表查询次数 */
    EGN_UINT32  ulRelQuerySuccessTimes; /* 所有协议关联表查询成功次数 */
    EGN_UINT32  ulTotalRelItemAddedCnt;      /* 普通关联表，添加的关联表项个数 */
    EGN_UINT32  ulTotalRelItemDestroyedCnt;  /* 普通关联表，销毁的关联表项个数 */
#ifdef EGN_64
    EGN_UINT8   aucReserved[4];         /* 64位对齐保留字节 */
#endif
}EgnRelStatInfo;

/*STRUCT< 统计项，内部使用 >*/
typedef struct _EgnStatInfo
{
    EGN_UINT32      ulPacketCount;      /* 识别数据包的总数 */
    EGN_UINT32      ulSuccessCount;     /* 识别成功的包的个数 */
    EGN_UINT32      ulUnknownCount;     /* 未识别的包的个数 */
    EGN_UINT32      ulFinishCnt;        /* 识别结果为finish的包个数 */
    EGN_UINT32      ulRelCnt;           /* 识别结果为relation的包个数 */
#ifdef EGN_64
    EGN_UINT8       aucReserved[4];     /* 64位对齐保留字节 */
#endif
    EgnRelStatInfo  stRelStatInfo;      /* 关联表相关统计项 */
}EgnStatInfo;

/*STRUCT< 动态内存统计项，AR使用 >*/
typedef struct _EgnDynMemStatInfo
{
    EGN_UINT32      ulMemCpAllocTimes;        /* 知识库等常驻 MemCp 申请次数 */
    EGN_UINT32      ulMemCpAllocSize;         /* 知识库等常驻 MemCp 申请大小 */
    EGN_UINT32      ulDynAllocTimes;          /* DynAlloc 申请次数 */
    EGN_UINT32      ulDynAllocDisableSize;    /* DynAlloc 去使能内存大小 */
    EGN_UINT32      ulDynAllocEnableSize;     /* DynAlloc 使能内存申请大小 */
    EGN_UINT32      ulDynAllocInsSize;        /* DynAlloc 单识别进程初始化完成所用内存 */
    EGN_UINT32      ulDynAllocInsTmpSize;     /* DynAlloc 单识别进程初始化完成所用内存临时计数 */
    EGN_UINT32      ulFlowCtxMemCpAllocTimes; /* 中间状态 MemCp 申请次数 */
    EGN_UINT32      ulFlowCtxMemCpAllocSize;  /* 中间状态 MemCp 申请大小 */
    EGN_UINT32      ulFlowCtxMemMaxSize;      /* 中间状态可使用的内存上限 */
}EgnDynMemStatInfo;

/*STRUCT<  关联表项结构 >*/
typedef struct _EgnPeerTuple
{
    EGN_UINT8     ucTransProtocol;  /* 传输层协议，具体取值见EGN_TRANS_TYPE_EN */
    EGN_UINT8     ucWeakFlag;       /* 弱关联识别标志位 */
    EGN_UINT16    usPort;           /* 端口 */
    EGN_UINT32    ulTunnelId;       /* 通道ID,不关心可以给0 */
    EgnIpAddr     stIpAddr;         /* IP地址 */
}EgnPeerTuple;

/*STRUCT<  老化时间结构 >*/
typedef struct _EgnPeerTime
{
    EGN_UINT32   ulTriggerTimeout;      /* 触发老化时间 */
    EGN_UINT32   ulPersistenceTimeout;  /* 持续老化时间 */
}EgnPeerTime;

/*STRUCT< 同步关联表的对应的关联数据流信息 >*/
typedef struct _EgnPeerDataFlowInfo
{
    EGN_VOID            *pvFlowHandle;              /* 关联识别控制流ID */
    EGN_UINT32           ulProtocol;                /* 参见EGN_PROTO_ID_EN */
#ifdef EGN_64
    EGN_UINT8           aucReserved[4];             /* 64位对齐保留字节 */
#endif
} EgnPeerDataFlowInfo;

/*STRUCT< 同步关联表时保存关联表项信息结构 >*/
typedef struct _EgnPeerInfoSetItem
{
    EgnPeerTuple            stTuple;            /* 待同步的三元组信息 */
    EgnPeerDataFlowInfo     stPeerSetDataInfo;  /* 待同步的数据信息 */
    EgnPeerTime             stTimeout;          /* 老化时间 */
}EgnPeerInfoSetItem;

/*STRUCT< 需要同步的peer信息 >*/
typedef struct _EgnPeerSet
{
    EgnPeerInfoSetItem  astPeerItem[EGN_SYN_PEER_MAX_NUM];  /* 待同步的peer */
    EGN_UINT32          ulPeerSetPeerNum;                   /* 待同步的peer个数 */
#ifdef EGN_64
    EGN_UINT8           aucReserved[4];                     /* 64位对齐保留字节 */
#endif
}EgnPeerSet;

/*STRUCT< 关联查询结果信息 >*/
typedef struct _EgnRelResult
{
    EGN_UINT32  ulLeftAgingTime;        /* 该关联表项对应的剩余老化时间，即这个表项还需要多长时间老化 */
    EGN_UINT32  ulProtocol;             /* 该关联表项对应的协议 */
    EGN_VOID    *pvFlowHandle;          /* 关联识别控制流ID */
} EgnRelResult;

/*ENUM< 识别结果 >*/
typedef enum
{
    EGN_INSPECT_BEGIN       = EGN_EN_INVALID,
    EGN_INSPECT_UNKNOWN     = 0,    /* 未知 */
    EGN_INSPECT_FINISH      = 1,    /* 识别结束 */
    EGN_INSPECT_RELATION    = 2,    /* 关联识别 */
    EGN_INSPECT_DOUBT       = 3,    /* 继续识别，可能是某种协议 */
    EGN_INSPECT_END,

    EGN_INSPECT_BOTTOM = EGN_EN_BUTT
}EGN_INSPECT_EN;

/*ENUM< 内部常使用的返回值 >*/
typedef enum
{
    EGN_RET_BEGIN                   = EGN_EN_INVALID,

    /* ----------------------- 公共域 ---------------------------------------- */
    EGN_RET_SUCCESS                                  = 0,       /* 成功 */
    EGN_RET_FAILURE                                  = 1,       /* 通常失败 */
    EGN_RET_ALLOC_FAILED                             = 2,       /* 分配内存失败 */
    EGN_RET_INVALID_PARAM                            = 3,       /* 无效的入参 */
    EGN_RET_INVALID_DATA                             = 4,       /* 无效的输入数据 */
    EGN_RET_INVALID_PROTOCOL                         = 5,       /* 无效的协议类型 */
    EGN_RET_NULL_PTR                                 = 6,       /* 空指针错误 */
    EGN_RET_MM_REPEAT_FREE                           = 7,       /* bufmem重复释放 */
    EGN_RET_CREATE_RULE_FAILED                       = 8,       /* 创建规则错误 */
    EGN_RET_ERR_BUF_MORE                             = 9,       /* 需要更大的存储空间 */
    EGN_RET_ERR_ID_NOT_EXIST                         = 10,      /* 没有ID对应的信息 */
    EGN_RET_ERR_NAME_NOT_EXIST                       = 11,      /* 没有名字对应的信息 */
    EGN_RET_REINIT                                   = 12,      /* 重复初始化 */
    EGN_RET_REDEINIT                                 = 13,      /* 重复去初始化 */
    EGN_RET_RELOAD                                   = 14,      /* 重复加载 */
    EGN_RET_PARSER_OVERFLOW                          = 15,      /* 长度不够,解析越界 */
    EGN_RET_PARSER_FAILURE                           = 16,      /* 关联识别时,解析协议失败 */
    EGN_RET_ITEM_NO_EXIST                            = 17,      /* 用于关联表，表示该记录不存在 */
    EGN_RET_RULE_CHECK_FAILURE                       = 18,      /* 规则有效性检查失败 */
    EGN_RET_RELEASE_IS_GOING                         = 19,      /* 备知识库正在释放中 */
    EGN_RET_ALREADY_INSPECTED                        = 20,      /* 一条流已经被识别出为FINISH或者UNKNOW,不允许继续识别 */
    EGN_RET_REACH_BUFF_END                           = 22,      /* 加载结束 */
    EGN_RET_AC_STATE_OVERFLOW                        = 23,      /* AC状态值溢出 */
    EGN_RET_CREAT_BUF_MEM_FAILED                     = 24,      /* 创建bufmem失败 */
    EGN_RET_MD5_FAILED                               = 25,      /* MD5相关的错误 */
    PCREX_RET_RESUME_REGEX_NOT_EXIST                 = 26,      /* resume的PCREX表达式不存在 */
    PCREX_RET_RESUME_REGEX_NOT_SUSPENDED             = 27,      /* resume的PCREX表达式没有被suspend */
    PCREX_RET_COMPILE_FAIURE                         = 28,      /* PCREX表达式编译失败 */
    PCREX_RET_RESUME_REGEX_NOT_SUSPENDED_OR_CONTINUE = 29,      /* resume的PCREX表达式没有被suspend 或者 cont */
    PCREX_RET_GET_RELATION_PARSE_REGEX_FAILED        = 30,      /* 获取关联解析的PCREX正则失败 */
    PCREX_RET_PCRE_EXEC_ERROR                        = 31,      /* PCRE执行错误 */
    PCREX_RET_REGEX_CTX_STATE_EXCEPTION              = 32,      /* PCREX正则的中间状态异常 */
    PCREX_RET_RELATION_PARSE_FAILED                  = 33,      /* PCREX表达式关联解析失败 */
    PCREX_RET_PUSHALLOC_FAILURE                      = 34,      /* PCREX压栈失败 */
    PCREX_RET_INVALID_VAR                            = 35,      /* PCREX表达式中变量非法 */
    PCREX_RET_REACH_NAME_TABLE_MAX_SIZE              = 36,      /* PCREX表达式名字达到了命名表的最大值 */
    PCREX_RET_REGEX_NAME_DUPLICATED                  = 37,      /* PCREX表达式名字重复 */
    PCREX_RET_REGEX_PCRE_COMPILE_FAILED              = 38,      /* PCRE正则compile识别 */
    PCREX_RET_REGEX_PCRE_STUDY_FAILED                = 39,      /* PCRE正则study识别 */
    PCREX_RET_REGEX_VARIABLE_MISUSED                 = 40,      /* PCREX表达式错误使用了非自己的变量 */
    PCREX_RET_REGEX_MISUSED_IN_RESUME                = 41,      /* PCREX表达式resume的使用错误 */
    PCREX_RET_REGEX_SUSPENDED_BUT_NOT_RESUMED        = 42,      /* PCREX表达式suspend 但是没有resume */
    PCREX_RET_REGEX_RESUMED_BUT_NOT_SUSPENDED        = 43,      /* PCREX表达式resume 但是没有suspend */
    PCREX_RET_VARIABLE_DECLARED_TWICE                = 44,      /* PCREX表达式变量声明了2次 */
    PCREX_RET_REGEX_SUSPEND_TWICE                    = 45,      /* PCREX表达式suspend了次 */
    PCREX_RET_REGEX_RESET_WITHOUT_RESUME             = 46,      /* PCREX表达式reset了没有resume的表达式 */
    PCREX_RET_REGEX_RESET_TWICE                      = 47,      /* PCREX表达式reset了2次 */
    PCREX_RET_REGEX_RESUME_TWICE                     = 48,      /* PCREX表达式resume了2次 */
    PCREX_RET_CALLOUT_SYNTAX_ERROR                   = 49,      /* PCREX表达式callout语法错误 */
    PCREX_RET_CLONE_VARIABLE_STACK_FAILED            = 50,      /* PCREX表达式clone 变量栈失败 */
    PCREX_RET_CREATE_TOKEN_STACK_FAILED              = 51,      /* PCREX表达式创建token栈失败 */
    PCREX_RET_CLONE_TOKEN_STACK_FAILED               = 52,      /* PCREX表达式clone token栈失败 */
    PCREX_RET_UNKNOWN_TOKEN_TYPE                     = 53,      /* PCREX表达式中unkonwn token类型 */
    PCREX_RET_GET_CAPTURE_VALUE_FAILED               = 54,      /* PCREX表达式获取捕获的value失败 */
    PCREX_RET_REGEX_PAYLOAD_FORMAT_ERROR             = 55,      /* PCREX表达式payload格式错误 */
    PCREX_RET_REGEX_PAYLOAD_INDEX_INVAILD            = 56,      /* PCREX表达式payload index错误 */
    PCREX_RET_GET_TOKEN_STACK_ELEMENT_FAILED         = 57,      /* PCREX表达式获取token元素错误 */
    PCREX_RET_CAPTURE_IP_STRING_CONVERT_FAILED       = 58,      /* PCREX表达式捕获IP类型转换错误 */
    PCREX_RET_GET_BIT_VALUE_BIT_RANGE_INVALID        = 59,      /* PCREX表达式获取bit的范围非法 */
    PCREX_RET_REGEX_MEMTABLE_INDEX_INVAILD           = 60,      /* PCREX表达式memtable index非法 */
    PCREX_RET_TOKEN_VALUE_NOT_INTEGER_COMPATIBLE     = 61,      /* PCREX表达式token value不是整型 */
    PCREX_RET_REGEX_IS_NULL                          = 62,      /* PCREX表达式是NULL */
    PCREX_RET_ARRAY_OFFSET_ERR                       = 63,      /* PCREX表达式数据OFFET错误 */
    PCREX_RET_STACK_FLIP_ARRAY                       = 64,      /* PCREX表达式stack flip to array */
    PCREX_RET_PIPELINE                               = 65,      /* 返回为正则执行PIPELINE */
    PCREX_RET_CALLOUT_NOT_SUPPORT_FRO_CUR_REGEX      = 66,      /* 当前CALLOUT在当前的正则中不支持 */
    PCREX_RET_FLOW_CACHE_IS_NULL                     = 67,      /* 流的cache为NULL */
    PCREX_RET_FLOW_CACHE_IS_NOT_ENOUGH               = 68,      /* 流的cache的空间不足 */
    PCREX_RET_CONVERT_STRING_TO_UINT32_FAILED        = 69,      /* 把字符串转换为UINT32失败 */
    PCREX_RET_REGEX_RESUME_DEAD_LOOP                 = 70,      /* resume的PCREX表达式死循环 */
    PCREX_RET_NAME_DUPLICATED                        = 71,      /* PCREX表达式名字重复 */

    EGN_RET_IPF_NO_IDLE_HASH_NODE                    = 72,      /* 没有空闲的hash节点 */
    EGN_RET_IPF_ITEM_NO_EXIST                        = 73,      /* 流表表项不存在 */
    EGN_RET_IPF_NO_IDLE_IDX_NODE                     = 74,      /* 没有空闲的流表索引节点 */
    EGN_RET_IPF_HASH_MAGIC_ERR                       = 75,      /* 流表对应的hash节点的magic值错误 */
    EGN_RET_IPF_ABN_PACKET_ERR                       = 76,      /* 流表管理，包异常错误 */

    EGN_RET_THREAD_ID_INCONSISTENT_INSTANCE          = 77,      /* 上层输入线程ID与实例中保存的不一致 */
    EGN_RET_THREAD_ID_INCONSISTENT_CTX               = 78,      /* 上层输入线程ID与中间状态保存的不一致 */
    EGN_RET_PEERLIST_TIMEOUT_VALUE_BEYOND_CAPABILITY = 79,      /* 超时时间设置超出范围 */
    EGN_RET_PEERLIST_PEER_NOT_FOUND                  = 80,      /* PEER没有找到 */
    EGN_RET_PEERLIST_IS_FULL                         = 81,      /* peerlist表满，非空 */
    EGN_RET_PEERLIST_CONFIGURE_ERROR                 = 82,      /* PeerList配置错误 */
    EGN_RET_PEERSET_CACHE_IS_NOT_ENOUGH              = 83,      /* 批量添加三元组时的cache的空间不足 */

    EGN_RET_ADAPTER_LOCK_CRT_FAILURE                 = 84,      /* 创建锁调用失败 */
    EGN_RET_ADAPTER_LOCK_ADD_FAILURE                 = 85,      /* 加写锁调用失败 */
    EGN_RET_ADAPTER_TIMERGRP_CRT_FAILURE             = 86,      /* 创建定时器数组失败 */
    EGN_RET_ADAPTER_TIMERGRP_START_FAILURE           = 87,      /* 启动定时器数组失败 */

    EGN_RET_CONTEXT_WITH_EXCEPTION                   = 88,      /* 传入的中间状态是已经出现过异常的中间状态，不予识别 */

    EGN_RET_INSPECT_TIME_OUT                         = 89,      /* 识别超时，立即返回的错误码 */
    EGN_RET_ENABLEMAGIC_INCONSISTENT_CTX             = 90,      /* 全局使能magic值与中间状态保存的不一致 */
    EGN_RET_ALLOC_OVERWRITE_FAILED                   = 91,      /* 内存分配过程中发现写越界错误 */
    EGN_RET_PEERLIST_FLOW_PEER_IS_MAX                = 92,      /* 整条控制流可以添加关联表个数已经达到上限 */
    EGN_RET_BYTE_CACHE_CTX_ABNORMAL                  = 93,      /* 取字节缓存数据异常 */
    EGN_RET_NOT_SUPPORT_PROTOCOL_CTRL_STATE          = 94,      /* 该协议不支持用户配置信息 */
    EGN_RET_FLOWCHANG_PROTOCOL_NOT_FOUND             = 95,      /* 知识库中没有该流变更协议 */
    EGN_RET_PEERLIST_NO_EXIST                        = 96,      /* 当前要使用的关联表暂时不存在 */
    EGN_RET_UD_PROTOCOL_ID_ERR                       = 97,      /* 自定义协议id错误 */
    EGN_RET_UD_RULE_ID_ERR                           = 98,      /* 自定义规则id错误 */
    EGN_RET_UD_BEAR_ID_ERR                           = 99,      /* 自定义承载id错误 */
    EGN_RET_UD_WEIGHT_ERR                            = 100,     /* 自定义规则权重错误 */
    EGN_RET_UD_TRANS_TYPE_ERR                        = 101,     /* 自定义规则传输层协议错误 */
    EGN_RET_UD_RULE_NAME_ERR                         = 102,     /* 自定义规则名称错误 */
    EGN_RET_UD_BEAR_INFO_ERR                         = 103,     /* 自定义规则承载信息错误 */
    EGN_RET_UD_COND_LESS_ERR                         = 104,     /* 自定义规则条件不足错误 */
    EGN_RET_UD_COND_NUM_ERR                          = 105,     /* 自定义规则条件个数错误 */
    EGN_RET_UD_PCRE_COND_ERR                         = 106,     /* 自定义规则pcre条件错误 */
    EGN_RET_UD_STR_COND_ERR                          = 107,     /* 自定义规则字符串条件错误 */
    EGN_RET_UD_DEPTH_COND_ERR                        = 108,     /* 自定义规则深度条件错误 */
    EGN_RET_UD_PORT_COND_ERR                         = 109,     /* 自定义规则端口错误 */
    EGN_RET_UD_IP_VER_ERR                            = 110,     /* 自定义规则ip版本号错误 */
    EGN_RET_UD_IP_COND_ERR                           = 111,     /* 自定义规则ip条件错误 */
    EGN_RET_UD_RULE_ID_REPEAT                        = 112,     /* 自定义规则重复 */
    EGN_RET_UD_RULE_FULL                             = 113,     /* 自定义规则满了 */
    EGN_RET_UD_RULE_SWITCH_OFF                       = 114,     /* 自定义规则功能的开关为关闭 */
    EGN_RET_UD_NO_RULE_NEED_ACTIVE                   = 115,     /* 自定义规则不需要激活 */
    EGN_RET_UD_RULE_NO_EXIST                         = 116,     /* 该自定义规则不存在 */
    EGN_RET_UD_RULE_BE_DELETED                       = 117,     /* 用户自定义规则已经被删除 */
    EGN_RET_UD_RULE_PATTERN_INVALID                  = 118,     /* 自定义等差规则模式数达到上限或为0 */
    EGN_RET_UD_ARITH_ENDIAN_INVALID                  = 119,     /* 自定义等差规则模式中字节序错误 */
    EGN_RET_UD_ARITH_FIELD_INVALID                   = 120,     /* 自定义等差规则模式中等差字段长度达到上限或为0 */
    EGN_RET_UD_ARITH_OFFSETBASE_INVALID              = 121,     /* 自定义等差规则模式中偏移方向错误 */
    EGN_RET_UD_ARITH_OFFSET_IS_MAX                   = 122,     /* 自定义等差规则模式中等偏移达到上限 */
    EGN_RET_UD_BEHA_RULE_PTN_PKTLENSEQ_INVALID       = 123,     /* 自定义统计规则模式中序列值无效 */
    EGN_RET_UD_PROTOCOL_ID_NO_EXIST                  = 124,     /* 自定义协议ID不存在 */
    EGN_RET_UD_BEAR_ID_NO_EXIST                      = 125,     /* 自定义承载ID不存在 */
    EGN_RET_UD_APP_ID_ERR                            = 126,     /* 自定义应用id错误 */
    EGN_RET_UD_SRV_ID_ERR                            = 127,     /* 自定义大类id错误 */
    EGN_RET_STAT_INSPECT_SWITCH_OFF                  = 128,     /* 统计识别开关为关闭 */
    EGN_RET_NGFW_UD_APP_FULL                         = 129,     /* NGFW自定义应用满了 */
    EGN_RET_NGFW_UD_APP_NOT_EXIST                    = 130,     /* NGFW自定义应用不存在 */
    EGN_RET_NGFW_UD_RULE_NAME_COMFLICT               = 131,     /* NGFW自定义规则重名 */
    EGN_RET_NGFW_UD_APP_RULE_FULL                    = 132,     /* NGFW自定义，一个应用下的规则数满 */
    EGN_RET_NGFW_UD_ALL_RULE_FULL                    = 133,     /* NGFW自定义，所有规则数满 */
    EGN_RET_NGFW_UD_APP_NAME_REPEAT                  = 134,     /* NGFW自定义应用名字有重复 */
    EGN_RET_NGFW_UD_APP_SOME_IN_USE                  = 135,     /* NGFW自定义应用被其它用户使用 */
    EGN_RET_NGFW_UD_APP_NO_RULE                      = 136,     /* NGFW自定义应用下没有规则 */
    EGN_RET_NGFW_UD_APP_NO_SAMENAME_RULE             = 137,     /* NGFW自定义应用下没有相同名称的规则 */
    EGN_RET_DNS_PROTOCOL_INFO_ERR                    = 138,     /* 输入的DNS信息错误 */
    EGN_RET_DNS_ANSWER_NOT_INSPECT                   = 139,     /* 输入的DNS信息不识别 */
    EGN_RET_DNS_IP_IS_INEXISTENCE                    = 140,     /* 处理的IP不存在 */
    EGN_RET_NGFW_UD_APP_NO_SAME_PORT                 = 141,     /* NGFW自定义应用下没有相同名称的规则 */
    EGN_RET_NGFW_UD_APP_INVALID_IPV4                 = 142,     /* NGFW自定义应用下规则的ipv4条件非法 */
    EGN_RET_NGFW_UD_APP_INVALID_IPV6                 = 143,     /* NGFW自定义应用下规则的ipv6条件非法 */
    EGN_RET_NGFW_UD_APP_INVALID_PORT                 = 144,     /* NGFW自定义应用下规则的端口条件非法 */
    EGN_RET_NGFW_UD_APP_INVALID_PATTERN              = 145,     /* NGFW自定义应用下规则的模式串条件非法 */
    EGN_RET_NGFW_UD_APP_INVALID_RULE_DESC            = 146,     /* NGFW自定义应用下规则的描述非法 */
    EGN_RET_NGFW_UD_APP_INVALID_IP_NUM               = 147,     /* NGFW自定义应用下规则的IP条件个数非法 */
    EGN_RET_NGFW_UD_APP_INVALID_PORT_NUM             = 148,     /* NGFW自定义应用下规则的Port条件个数非法 */
    EGN_RET_NGFW_UD_BEING_ACTIVATED                  = 149,     /* 在激活自定义规则后，还未激活应用，就尝试进行增/删/改操作;或者重复的激活自定义规则 */
    EGN_RET_NGFW_UD_APP_INVALID_TRANSTYPE            = 150,     /* NGFW自定义应用下规则的传输层协议非法 */
    EGN_RET_PROTO_DISABLE_CFG_FULL                   = 151,     /* 协议去使能配置已满。见EGN_MAX_DISABLED_PROTO_NUM */
    EGN_RET_PROTO_NOT_BE_UD_ID                       = 152,     /* 协议去使能配置不支持自定义协议的配置 */
    EGN_RET_PROTO_INFO_RULE_LIB_NOT_INITED           = 153,     /* 明文知识库未初始化 */
    EGN_RET_DNS_PARSE_OR_MATCH_NODE_IS_MAX           = 154,     /* 解析的DNS中的回答问题个数已经达到上限 */
    EGN_RET_FRAG_INSPECT_PACKET_FRAG                 = 155,     /* 数据包被分片 */
    EGN_RET_HA_INVALID_CONDITION_ID                  = 156,     /* 硬件加速条件出现非法条件ID */
    EGN_RET_HA_INVALID_PATTERN_ID                    = 157,     /* 硬件加速条件出现非法模式串ID */
    EGN_RET_NGFW_HA_SWITCH_OFF                       = 158,     /* 硬件加速开关没有打开 */
    EGN_RET_NGFW_UD_RULE_IPV4_REPEAT                 = 159,     /* NGFW自定义应用下有相同的ipv4条件 */
    EGN_RET_NGFW_UD_RULE_IPV4_FULL                   = 160,     /* NGFW自定义应用下的ipv4条件个数已满 */
    EGN_RET_NGFW_UD_RULE_IPV6_REPEAT                 = 161,     /* NGFW自定义应用下有相同的ipv6条件   */
    EGN_RET_NGFW_UD_RULE_IPV6_FULL                   = 162,     /* NGFW自定义应用下的ipv6条件个数已满 */
    EGN_RET_NGFW_UD_RULE_PORT_REPEAT                 = 163,     /* NGFW自定义应用下的有相同的端口条件 */
    EGN_RET_NGFW_UD_RULE_PORT_FULL                   = 164,     /* NGFW自定义应用下的端口条件个数已满 */
    EGN_RET_NGFW_HA_MATCH_NUM_INVALID                = 165,     /* 硬件加速匹配返回的硬件匹配结果个数非法 */
    EGN_RET_RULE_NUM_TO_MAX                          = 166,     /* 规则个数达到最大，包括系统和自定义的规则 */
    EGN_RET_END,

    EGN_RET_BOTTOM             = EGN_EN_BUTT
} EGN_RET_RESULT_EN;

/*ENUM< 知识库状态类型 >*/
typedef enum
{
    EGN_RULE_LIB_STATE_BEGIN          = EGN_EN_INVALID,
    EGN_RULE_LIB_STATE_INIT           = 0,                /* 知识库初始 */
    EGN_RULE_LIB_STATE_ACTIVE         = 1,                /* 激活状态 */
    EGN_RULE_LIB_STATE_FROZEN         = 2,                /* 冻结状态 */
    EGN_RULE_LIB_STATE_DEACTIVE       = 3,                /* 去激活状态 */
    EGN_RULE_LIB_STATE_END,

    EGN_RULE_LIB_STATE_BOTTOM       = EGN_EN_BUTT
}EGN_RULE_LIB_STATE_EN;

/*STRUCT< ReleaseDate发布日期 >*/
typedef struct _EgnReleaseDate
{
    EGN_UINT16  usYear;     /* 年 */
    EGN_UINT8   ucMonth;    /* 月 */
    EGN_UINT8   ucDay;      /* 日 */
    EGN_UINT8   ucHour;     /* 小时 */
    EGN_UINT8   ucMinute;   /* 分钟 */
    EGN_UINT8   ucSec;      /* 秒 */
    EGN_UINT8   ucReserved; /* 保留字节 */
} EgnReleaseDate;

/*STRUCT< 举例说明:版本号Version:01.0001.0054.01 >*/
typedef struct _EgnRuleLibVersionNum
{
    EGN_UINT16  usMajorVersion;         /* 大版本号AA,对应例子中的01 */
    EGN_UINT16  usCompatibleVersion;    /* 兼容性版本号XXXX,对应例子中的0001 */
    EGN_UINT16  usSnapShotVersion;      /* 知识库快照版本BBBB,对应例子中的0054 */
    EGN_UINT16  usSnapShotSubVersion;   /* 知识库特征码快照子版本号CC,对应例子中的01 */
} EgnRuleLibVersionNum;

/*STRUCT< 举例说明:01.0001.0054.01.20090530.14:48:50 >*/
typedef struct _EgnRuleLibVersion
{
    EgnReleaseDate          stReleaseDate;      /* 发布日期,对应例子中的20090530.14:48:50 */
    EgnRuleLibVersionNum    stLibVersionNum;    /* 版本号，具体其结构体 */
} EgnRuleLibVersion;

/*STRUCT< 协议导入配置结构 >*/
typedef struct _EgnProtoImportCfgInfo
{
    EGN_UINT32  ulProtoID;      /* 协议ID */
    EGN_BOOL    bIsImport;      /* 是否需要导入，0表示不导入，１表示导入 */
} EgnProtoImportCfgInfo;

/*STRUCT< 规则导入配置结构 >*/
typedef struct _EgnRuleImportCfgInfo
{
    EGN_UINT32  ulRuleID;      /* 规则ID */
    EGN_BOOL    bIsImport;      /* 是否需要导入，0表示不导入，１表示导入 */
} EgnRuleImportCfgInfo;

/*STRUCT< 协议关联解析配置结构 >*/
typedef struct _EgnProtoRelParseCfgInfo
{
    EGN_UINT32   ulProtoId;    /* 协议ID */
    EGN_BOOL     bIsEnable;    /* 是否需要关联解析的协议,EGN_FALSE表示已经设置关闭该协议的解析功能 */
    EGN_BOOL     bIsInCrtRuleLib;    /* 该协议是否在当前知识库中存在 */
#ifdef EGN_64
    EGN_UINT8    aucReserved[4];     /* 64位对齐保留字节 */
#endif
} EgnProtoRelParseCfgInfo;

/*ENUM< 层级非首包识别开关的状态 >*/
typedef enum
{
    EGN_HIER_DET_STATE_OFF      = 0,     /* 关闭层级非首包识别 */
    EGN_HIER_DET_STATE_DEFAULT  = 2,     /* 默认状态，以全局层级识别命中非同包识别功能全局开关(EgnInitCfgParam中的bHierInspectSwitch)为准 */
    EGN_HIER_DET_STATE_ON       = 0xffff,/* 打开层级非首包识别 */
    EGN_HIER_DET_STATE_END,

    EGN_HIER_DET_STATE_BOTTOM   = EGN_EN_BUTT
}EGN_HIER_DET_STATE;

/*STRUCT< 层级非首包识别开关状态信息结构 >*/
typedef struct _EgnHierMaxDetCntInfo
{
    EGN_UINT32  ulProtoID;             /* 待设置层级非首包识别开关的状态的协议ID */
    EGN_UINT16  usHierMaxDetCnt;        /* ulProtoID对应的层级非首包识别开关的状态，对应EGN_HIER_DET_STATE */
    EGN_UINT8   ucIsInCrtRuleLib;      /* 该协议是否在当前知识库中存在，EGN_PROTOCOL_VALIDE_STATE_EN */
    EGN_UINT8   ucReserved;            /* 对齐保留字节 */
} EgnHierMaxDetCntInfo;

/*STRUCT< 层级非首包识别开关状态结构 >*/
typedef struct _EgnHierMaxDetCnt
{
    EGN_UINT32  ulProtoID;             /* 待设置层级非首包识别开关的状态的协议ID */
    EGN_UINT16  usHierMaxDetCnt;       /* ulProtoID对应的层级非首包识别开关的状态，对应EGN_HIER_DET_STATE */
    EGN_UINT8   aucReserved[2];        /* 对齐保留字节 */
} EgnHierMaxDetCnt;

/*STRUCT< 内存块统计信息 >*/
typedef struct _EgnBufStatPoolMemInfo
{
    EGN_UINT32  ulBlockSize;     /* 块大小 */
    EGN_UINT32  ulListSize;      /* 块的总数 */
    EGN_UINT32  ulNoOfFreeElmnt; /* 块的剩余个数 */
    EGN_UINT32  ulFirstOccLoc;   /* 第一个空闲块的位置 */
    EGN_UINT32  ulLastOccLoc;    /* 最后一个空闲块的位置 */
#ifdef EGN_64
    EGN_UINT8              aucReserved[4];     /* 64位对齐保留字节 */
#endif
}EgnBufStatPoolMemInfo;

/*STRUCT< 内存块信息 >*/
typedef struct _EgnBufStatInfo
{
    EGN_UINT32             ulBufPoolNum;       /* 内存池数目 */
#ifdef EGN_64
    EGN_UINT8              aucReserved[4];     /* 64位对齐保留字节 */
#endif
    EgnBufStatPoolMemInfo  stMemInfo[EGN_MEMCP_INIT_MAX_COUNT];  /* 内存信息 */
}EgnBufStatInfo;

/*ENUM< 支持的告警阀值种类定义 >*/
typedef enum
{
    EGN_RESALARMTHRESHOLD_BEGIN       = EGN_EN_INVALID,
    EGN_RESALARMTHRESHOLD_RULELIB     = 0,                /* EGN知识库阀值设置 */
    EGN_RESALARMTHRESHOLD_CTX         = 1,                /* EGN中间状态阀值设置 */
    EGN_RESALARMTHRESHOLD_PEERLIST    = 2,                /* EGN关联表阀值设置 */
    EGN_RESALARMTHRESHOLD_END,

    EGN_RESALARMTHRESHOLD_BOTTOM      = EGN_EN_BUTT
}EGN_RESALARMTHRESHOLD_EN;

/*MACRO< 支持的告警值上限 >*/
#define EGN_RESALARMTHRESHOLD_MAX_SIZE   (95)

/*MACRO< 支持的告警值下限 >*/
#define EGN_RESALARMTHRESHOLD_MIN_SIZE   (50)

/*MACRO< 字符串最大长度 >*/
#define  EGN_UNITE_QUERY_STRING_LEN_MAX   512

/*ENUM< 获取信息的类型，根据类型分配内存 >*/
typedef enum
{
    EGN_UNITE_GET_SET_BEGIN           = EGN_EN_INVALID,
    EGN_UNITE_GET_SET_ENGINE_VERSION  = 0,                  /* 引擎版本号,只能查询，EGN_UCHAR */
    EGN_UNITE_GET_SET_RULELIB_VERSION,                      /* 知识库版本号，只能查询，EGN_UCHAR */
    EGN_UNITE_GET_SET_PEER_INFO,                            /* PEER信息，只能查询，EgnPeerInfo */
    EGN_UNITE_GET_SET_EGN_STATE,                            /* EGN状态信息，只能查询，EGN_UINT32，0:未初始化，1:使能，2:正在去使能，3:去使能，4:去初始化中 */
    EGN_UNITE_GET_SET_CTX_STATIC_INFO,                      /* 中间状态数目，只能查询，EgnCtxNumInfo */
    EGN_UNITE_GET_SET_PACKET_STATIC_INFO,                   /* 报文统计信息，只能查询，EgnPacketStatInfo */
    EGN_UNITE_GET_SET_HEALTH_INFO,                          /* 健康信息，只能查询，EGN_UCHAR */
    EGN_UNITE_GET_SET_RULELIB_BLOCK_INFO,                   /* 知识库内存块使用情况(静态模式下)，只能查询，EgnRuleLibBlockInfo */
    EGN_UNITE_GET_SET_STATIC_ERR_INFO,                      /* 错误统计信息，只能查询，EgnErrInfo */
    EGN_UNITE_GET_SET_RUEL_LIB_STATE_INFO,                  /* 知识库状态信息，只能查询，EgnRuleLibState */
    EGN_UNITE_GET_SET_DYNMEM_INFO,                          /* AR申请的内存信息，只能查询，EgnDynMemInfo */
    EGN_UNITE_GET_SET_STATIC_BLOCK_INFO,                    /* 全局静态内存管理，只能查询，EgnStaticMemBlockInfo */
    EGN_UNITE_GET_SET_CB_STATIC_INFO,                       /* CB配置信息，只能查询，EgnCBStaticInfo */
    EGN_UNITE_GET_SET_MATCHED_RULE_INFO,                    /* 当前报文命中的规则信息，只能查询，EGN_UINT32 */
    EGN_UNITE_GET_SET_SYSMEMMODE_INFO,                      /* 系统内存模式信息，只能查询，EGN_UINT32，0:静态，1:动态 */
    EGN_UNITE_GET_SET_OPTIMAL_RESULT_MODE_SWITCH_INFO,      /* 最优识别结果模式开关，只能查询，EGN_UINT32 */
    EGN_UNITE_GET_SET_RULE_STAT_INFO,                       /* 规则统计信息，只能查询，EgnGetRuleStatInfo */
    EGN_UNITE_GET_SET_STAT_INFO,                            /* 内部统计信息，只能查询，EgnStatInfo */
    EGN_UNITE_GET_SET_DETECT_THRESHOLD,                     /* 识别阀值，可查询及静态、动态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_FAST_RELSWITCH_INFO,                  /* 快速关联开关信息，可查询及静态、动态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_ROLLBACK_TIME,                        /* 回退保持时长，可查询及静态、动态设置，EGN_UINT32,单位:秒 */
    EGN_UNITE_GET_SET_WELLKNOWN_PORT_SWITCH,                /* 知名端口开关信息，可查询及静态、动态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_STATINSPECT_SWITCH,                   /* 统计识别开关信息，可查询及静态、动态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_NOTFIRSTPKT_RELINSPECT_SWITCH_INFO,   /* 非首包关联识别开关信息，可查询及静态、动态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_HIERINSPECT_SWITCH,                   /* 层级识别命中非首包开关，可查询及动态、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_UNSYMMETRICAL_INSPECT_SWITCH_INFO,    /* 非对称识别开关信息，可查询及静态、动态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_BEHAVIOR_INSPECT_STATE,               /* 行为特征识别功能开启状态，可查询及静态、动态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_IP_FLOW_AGED_TIME,                    /* 流表老化时间，可查询及静态、动态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_DNS_REL_SWTICH,                       /* DNS关联识别开关，可查询、静态、动态设置(动态设置不支持从关到开状态的设置)，EGN_UINT32 */
    EGN_UNITE_GET_SET_STAT_CONTROL,                         /* EGN统计功能，查询时对应EgnGetStatControl，静态设置对应EGN_UINT32，动态设置对应EgnSetStatControl */
    EGN_UNITE_GET_SET_MAX_INSTANCE_INFO,                    /* 最大实例个数信息，可查询及静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_SCAN_INTERVAL,                        /* 老化扫描的时间间隔，可查询、静态设置，EGN_UINT32 单位: 毫秒 */
    EGN_UNITE_GET_SET_FAST_RELAGING_TIME,                   /* 快速关联老化时间，可查询、静态，EGN_UINT32，单位:分钟  */
    EGN_UNITE_GET_SET_WEAK_RELATION_SWITCH,                 /* 弱关联识别开关，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_FLOWSCALE_INFO,                       /* 流规模信息，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_MEMPLICY_INFO,                        /* 内存配置方式信息，可查询、静态设置，EGN_UINT32，0:不受限，1:受限 */
    EGN_UNITE_GET_SET_NGFW_UD_APP_SWITCH_INFO,              /* 用户NGFW自定义控制开关信息，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_NGFW_HA_SWITCH,                       /* NGFW产品进行硬件加速的设置，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_FRAG_SWITCH_INFO,                     /* 分片控制开关信息，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_UDRULE_SWITCH_INFO,                   /* 用户自定义控制开关信息，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_PROTO_IMPORT_STATE,                   /* 协议是否导入的信息，可查询、动态设置，查询时对应EgnGetProtoImport，动态设置时对应EgnSetProtoImportOrRelParse */
    EGN_UNITE_GET_SET_PROTO_REL_PARSE_STATE,                /* 协议不关联解析的信息，可查询、动态设置，查询时对应EgnGetProtoRelParse，动态设置时对应EgnSetProtoImportOrRelParse */
    EGN_UNITE_GET_SET_PROTO_HIER_INSPECT_STATE,             /* 协议层级识别阀值的信息，可查询、动态设置，查询时对应EgnGetProtoHierInspect，动态设置时对应EgnSetProtoHierInspect */
    EGN_UNITE_GET_SET_RULE_IMPORT_STATE,                    /* 规则是否导入的信息(目前只支持BT行为识别规则)，可查询、动态设置，查询时对应EgnGetRuleImport，动态设置时对应EgnSetRuleImport */
    EGN_UNITE_GET_SET_ALARM_THRESHOLD,                      /* 告警值信息，可查询、动态设置，查询时对应EgnGetResAlarmThreshold，动态设置时对应EgnSetResAlarmThreshold */
    EGN_UNITE_GET_SET_ATTR_TYPE_CFG,                        /* 属性类型配置信息，可查询、动态设置，查询时对应EgnGetAttrTypeCfg，动态设置时对应EgnSetAttrTypeCfgInfo */
    EGN_UNITE_GET_SET_CFG_PARA_IS_SHARE,                    /* 配置信息是否共享，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_RULELIB_IS_SHARE,                     /* 知识库是否共享，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_MEM_IS_SHARE,                         /* 是否共享内存，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_REL_CB_NUM_IPV4,                      /* IPv4关联识别CB块的数目，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_REL_CB_NUM_IPV6,                      /* IPv6关联识别CB块的数目，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_MAX_RELAGE_NUM,                       /* 关联表老化上限，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_DNS_REL_CB_NUM_IPV4,                  /* DNS关联识别IPv4的CB块的数目，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_DNS_REL_CB_NUM_IPV6,                  /* DNS关联识别IPv6的CB块的数目，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_NEED_SUPER_CLASS,                     /* 识别结果中是否需要返回大类、子类和应用id信息开关，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_UD_BACK_RULE_LIB_NEED_MEM,            /* 用户自定义规则备份主备知识库所需内存，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_SN_CB_STATIC_INFO,                    /* session CB配置信息，只能查询，EgnCBStaticInfo */
    EGN_UNITE_GET_SET_SN_STATIC_BLOCK_INFO,                 /* session 全局静态内存管理，只能查询，EgnStaticMemBlockInfo */
    EGN_UNITE_GET_SET_SN_SWITCH,                            /* session开关信息，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_SN_FLOWSCALE,                         /* session流规模信息，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_SN_REL_CB_NUM_IPV4,                   /* session IPv4关联识别CB块的数目，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_SN_REL_CB_NUM_IPV6,                   /* session IPv6关联识别CB块的数目，可查询、静态设置，EGN_UINT32 */
    EGN_UNITE_GET_SET_SN_BODY_MEM_CACHE,                    /* session 缓存待处理数据的大小，可查询、静态设置，默认为2K，设置大小必须大于等于1K，EGN_UINT32 */
    EGN_UNITE_GET_SET_SN_BODY_MEM_DECOMPRESS,               /* session 缓存解压数据的大小，可查询、静态设置，默认为6K，设置大小必须大于等于1K，且大于等于session 缓存待处理数据的大小；设置时通常先设置session 缓存待处理数据的大小EGN_UINT32 */

    EGN_UNITE_GET_SET_END,

    EGN_UNITE_GET_SET_BOTTOM     = EGN_EN_BUTT
}EGN_UNITE_GET_SET_EN;

/*STRUCT< peer信息的结构 >*/
typedef struct _EgnPeerInfo
{
    EGN_UINT32   ulRelItemAddedCnt;      /* 添加的关联表项个数 */
    EGN_UINT32   ulRelItemDestroyedCnt;  /* 关联表项销毁个数 */
} EgnPeerInfo;

/*STRUCT< 中间状态信息的结构 >*/
typedef struct _EgnCtxNumInfo
{
    EGN_UINT32   ulCurCtxNum;         /* 当前中间状态的数目 */
    EGN_UINT32   ulCurCtxNumWithRel;  /* 当前关联识别中间状态的数目 */
}EgnCtxNumInfo;

/*STRUCT< 动态内存信息的结构 >*/
typedef struct _EgnDynMemInfo
{
    EGN_UINT32   ulMemCpAllocTimes;     /* MemCp申请次数 */
    EGN_UINT32   ulMemCpAllocSize;      /* MemCp申请大小 */
    EGN_UINT32   ulDynAllocTimes;       /* DynAlloc申请次数 */
    EGN_UINT32   ulDynAllocSize;        /* DynAlloc申请大小 */
    EGN_UINT32   ulRuleLibDynAllocSize; /* 知识库内存大小 */
#ifdef EGN_64
    EGN_UINT8    aucReserved[4];        /* 64位对齐保留字节 */
#endif
}EgnDynMemInfo;

/*STRUCT< 知识库内存块信息的结构 >*/
typedef struct _EgnRuleLibBlockInfo
{
    EGN_UINT32   ulMemTotalSize;      /* 知识库总的内存大小 */
    EGN_UINT32   ulMemUsedSize;       /* 使用的内存大小 */
}EgnRuleLibBlockInfo;

/*STRUCT< 错误统计信息 >*/
typedef struct _EgnErrInfo
{
    EGN_UINT32     ulAllocFailedNum;        /* 内存分配失败次数 */
    EGN_UINT32     ulMemRepeatFreeNum;      /* Bufmem重复释放次数 */
    EGN_UINT32     ulReLoadNum;             /* 重复加载次数 */
    EGN_UINT32     ulReleaseIsGoing;        /* 备知识库正在释放 */
    EGN_UINT32     ulCreatBufMemFailedNum;  /* 创建BufMem失败次数 */
    EGN_UINT32     ulThreadIdInconInstance; /* 上层输入线程ID与实例中保存的不一致 */
    EGN_UINT32     ulThreadIdInconCtx;      /* 上层输入线程ID与中间状态保存的不一致 */
    EGN_UINT32     ulPeerListIsFull;        /* peerlist表满 */
    EGN_UINT32     ulPeerCacheNotEnough;    /* 批量添加三元组时的cache的空间不足 */
    EGN_UINT32     ulInspectTimeOut;        /* 识别超时 */
    EGN_UINT32     ulAllocOverwriteFailed;  /* 内存分配过程中发现写越界错误 */
    EGN_UINT32     ulPeerListFlowIsMax;     /* 整条控制流可以添加关联表个数已经达到上限 */
    EGN_UINT32     ulPeerListMaxCollision;  /* 冲突链中peer数达到上限 */
    EGN_UINT32     ulStatAllocFailedNum;    /* 统计失败分配内存失败 */
    EGN_UINT32     ulStatCreateBufFailedNum;  /* 统计识别创建Bufmem失败 */
#ifdef EGN_64
    EGN_UINT8      aucReserved[4];          /* 64位对齐保留字节 */
#endif
}EgnErrInfo;

/*STRUCT< 报文统计信息的结构 >*/
typedef struct _EgnPacketStatInfo
{
    EGN_UINT32     ulPacketCount;       /* 进入识别的总包数 */
    EGN_UINT32     ulSuccessCount;      /* 识别成功包数 */
    EGN_UINT32     ulFailCount;         /* 识别失败包数 */
    EGN_UINT32     ulUnknownCount;      /* 未识别包数 */
    EGN_UINT32     ulFinishCnt;         /* 识别结束包数 */
    EGN_UINT32     ulRelCnt;            /* 需要关联识别包数 */
}EgnPacketStatInfo;

/*STRUCT< 知识库状态的结构 >*/
typedef struct _EgnRuleLibState
{
    EGN_UINT32     ulMasterRuleLibIdx;    /* 主知识库索引号 */
    EGN_UINT32     ulMasterRuleLibState;  /* 主知识库状态 */
    EGN_UINT32     ulSlaveRuleLibState;   /* 备知识库状态 */
#ifdef EGN_64
    EGN_UINT8      aucReserved[4];        /* 64位对齐保留字节 */
#endif
}EgnRuleLibState;

/*STRUCT< CB配置信息的结构 >*/
typedef struct _EgnCBStaticInfo
{
    EGN_UINT32     ulTotalNumIpv4;    /* IPv4总共的CB块 */
    EGN_UINT32     ulTotalNumIpv6;    /* IPv6总共的CB块 */
    EGN_UINT32     ulUsedNumIpv4;     /* IPv4使用的CB块 */
    EGN_UINT32     ulUsedNumIpv6;     /* IPv6使用的CB块 */
}EgnCBStaticInfo;

/*STRUCT< Block信息的结构 >*/
typedef struct _EgnBpBlockInfo
{
    EGN_UINT32     ulEachBlockMemSize;    /* 内存块大小 */
    EGN_UINT32     ulBockNum;             /* 块总数 */
    EGN_UINT32     ulNoOfFreeElmnt;       /* 剩余个数 */
    EGN_UINT32     ulFirstOccLoc;         /* 第一个空闲元素的位置 */
    EGN_UINT32     ulLastOccLoc;          /* 最后一个空闲元素的位置 */
#ifdef EGN_64
    EGN_UINT8      aucReserved[4];        /* 64位对齐保留字节 */
#endif
}EgnBpBlockInfo;

/*STRUCT< 静态内存信息的结构 >*/
typedef struct _EgnStaticMemBlockInfo
{
    EGN_UINT32       ulBlockTypeNum;    /* Block类型的数量 */
#ifdef EGN_64
    EGN_UINT8        aucReserved[4];    /* 64位对齐保留字节 */
#endif
    EgnBpBlockInfo   stBlockInfo[EGN_MEMCP_INIT_MAX_COUNT]; /* Block信息 */
}EgnStaticMemBlockInfo;

/*STRUCT< 查询协议是否导入的信息 >*/
typedef struct _EgnGetProtoImport
{
    EGN_UINT32               ulInProtoID;            /* 查询的协议ID。当ID不为0时，查询单个协议，当ID为0时，查询所有已配置的协议导入信息 */
    EGN_UINT32               ulInOutItemNum;         /* 作为输入时为配置信息数组个数，作为输出为配置项数量 */
    EgnProtoImportCfgInfo   *pstOutProtoImportInfo;  /* 协议导入配置信息数组，需根据ulInOutItemNum为其分配内存 */
}EgnGetProtoImport;

/*STRUCT< 查询协议不关联解析的信息 >*/
typedef struct _EgnGetProtoRelParse
{
    EGN_UINT32               ulInProtoID;             /* 查询的协议ID。当ID不为0时，查询单个协议，当ID为0时，查询所有已配置的协议关联解析信息 */
    EGN_UINT32               ulInOutItemNum;          /* 作为输入时为配置信息数组个数，作为输出为配置项数量 */
    EgnProtoRelParseCfgInfo *pstOutProtoRelParseInfo; /* 协议关联解析配置信息数组，需根据ulInOutItemNum为其分配内存 */
}EgnGetProtoRelParse;

/*STRUCT< 查询协议层级识别阀值的信息 >*/
typedef struct _EgnGetProtoHierInspect
{
    EGN_UINT32             ulInProtoID;             /* 查询的协议ID。当ID不为0时，查询单个协议，当ID为0时，查询所有已配置的协议层级识别非同包识别开关的信息 */
    EGN_UINT32             ulInOutItemNum;          /* 作为输入时为配置信息数组个数，作为输出为配置项数量 */
    EgnHierMaxDetCntInfo  *pstOutHierMaxDetCntInfo; /* 层级非首包识别开关状态配置信息数组，需根据ulInOutItemNum为其分配内存 */
}EgnGetProtoHierInspect;

/*STRUCT< 查询规则是否导入的信息 >*/
typedef struct _EgnGetRuleImport
{
    EGN_UINT32             ulInRuleID;              /* 查询的规则ID，目前只支持BT行为识别规则ID,8155 */
    EGN_UINT32             ulInOutItemNum;          /* 作为输入时为配置信息数组个数，作为输出为配置项数量 */
    EgnRuleImportCfgInfo  *pstOutRuleImportCfgInfo; /* 规则导入配置信息数组,需根据ulInOutItemNum分配相应内存 */
}EgnGetRuleImport;

/*STRUCT< 告警值查询 >*/
typedef struct _EgnGetResAlarmThreshold
{
    EGN_UINT32      ulInType;           /* 需要查询的告警值类型 */
    EGN_UINT32      ulOutThreshold;     /* 获得的具体上限告警值，该值的取值范围[50-95]，默认值85 */
}EgnGetResAlarmThreshold;

/*STRUCT< 属性类型是否识别 >*/
typedef struct _EgnGetAttrTypeCfg
{
    EGN_UINT32   ulInAttrTypeID;        /* 属性类型id */
    EGN_BOOL     bOutIsEnable;          /* 是否识别 */
}EgnGetAttrTypeCfg;

/*STRUCT< 查询规则统计信息 >*/
typedef struct _EgnGetRuleStatInfo
{
    EGN_UINT32         ulInStatRdNum;       /* 期待的统计记录数目 */
    EGN_UINT32         ulOutGetedRdNum;     /* 实际获得的统计记录数目 */
    EgnRuleStatRd     *pstOutRuleStatRd;    /* 保存查询记录的内存，需根据ulInStatRdNum为其分配内存 */
}EgnGetRuleStatInfo;

/*STRUCT< 查询EGN统计功能 >*/
typedef struct _EgnGetStatControl
{
    EGN_UINT32    ulInCommand;      /* 统计功能类型，取值范围为EGN_STAT_ONOFF_SWITCH 、EGN_STAT_PROTOID_SET */
    EGN_UINT32    ulOutContent;     /* ulInCommand为0时，表示用户配置的统计开关值，该值可取0、1；ulInCommand为1，表示用户配置的协议ID(如果不配置，默认为11) */
}EgnGetStatControl;

/*STRUCT< 查询的信息 >*/
typedef struct _EgnGetCfgInfo
{
    union
    {
        EGN_UINT32               ulOutGetValue;                             /* 获取的整型类型的值 */
        EGN_UCHAR                aucString[EGN_UNITE_QUERY_STRING_LEN_MAX]; /* 字符串类型信息 */
        EgnPeerInfo              stPeerInfo;                                /* Peer信息 */
        EgnCtxNumInfo            stCtxNumInfo;                              /* 中间状态信息 */
        EgnDynMemInfo            stDynMemInfo;                              /* 动态内存信息 */
        EgnRuleLibBlockInfo      stRuleLibBlockInfo;                        /* 知识库内存块信息 */
        EgnStaticMemBlockInfo    stStaticMemBlockInfo;                      /* 静态内存信息 */
        EgnErrInfo               stErrInfo;                                 /* 错误统计信息 */
        EgnPacketStatInfo        stPacketStatInfo;                          /* 报文统计信息 */
        EgnRuleLibState          stRuleLibState;                            /* 知识库状态 */
        EgnCBStaticInfo          stCBStaticInfo;                            /* CB配置信息 */
        EgnGetProtoImport        stGetProtoImport;                          /* 协议是否导入的信息 */
        EgnGetProtoRelParse      stGetProtoRelParse;                        /* 协议不关联解析的信息 */
        EgnGetProtoHierInspect   stGetProtoHierInspect;                     /* 协议层级识别开关状态的信息 */
        EgnGetRuleImport         stGetRuleImport;                           /* 规则是否导入的信息 */
        EgnGetResAlarmThreshold  stGetResAlarmThreshold;                    /* 告警值查询 */
        EgnGetAttrTypeCfg        stGetAttrTypeCfg;                          /* 属性类型是否识别 */
        EgnGetRuleStatInfo       stGetRuleStatInfo;                         /* 规则统计信息 */
        EgnGetStatControl        stGetStatControl;                          /* EGN统计功能 */
        EgnStatInfo              stStatInfo;                                /* 统计项 */
    }GetCfgInfoUn;
} EgnGetCfgInfo;

/*STRUCT< 设置一组协议是否导入/一组协议关联解析开关的结构 >*/
typedef struct _EgnSetProtoImportOrRelParse
{
    EGN_UINT32   ulInProtoNum;          /* 协议配置信息的个数 */
    EGN_BOOL     bInIsEnable;           /* 协议是否导入 */
    EGN_UINT32  *pulInOutProtoIdList;   /* 协议列表，失败时为回填失败的协议。作为入参时需根据ulInProtoNum个数分配相应内存 */
    EGN_UINT32   ulOutFailNum;          /* 设置失败的数目 */
#ifdef EGN_64
    EGN_UINT8    aucReserved[4];        /* 64位对齐保留字节 */
#endif
}EgnSetProtoImportOrRelParse;

/*STRUCT< 设置一组协议的层级识别包个数的阀值 >*/
typedef struct _EgnSetProtoHierInspect
{
    EGN_UINT32           ulInProtoNum;          /* 协议配置信息的个数 */
    EGN_UINT32           ulOutFailNum;          /* 设置失败的数目 */
    EgnHierMaxDetCnt    *pstInOutProtoIdList;   /* 输入:用户要配置的协议列表。输出:失败时回填失败的协议列表。作为入参时需根据ulInProtoNum分配内存 */
}EgnSetProtoHierInspect;

/*STRUCT< 设置规则是否导入 >*/
typedef struct _EgnSetRuleImport
{
    EGN_UINT32   ulInRuleNum;           /* 规则配置信息的个数 */
    EGN_BOOL     bInIsEnable;           /* 规则是否导入 */
    EGN_UINT32  *pulInOutRuleIdList;    /* 规则列表，失败时为回填失败的规则。目前只支持BT行为识别规则，作为入参时需根据ulInProtoNum个数分配相应内存 */
    EGN_UINT32   ulOutFailNum;          /* 设置失败的数目 */
#ifdef EGN_64
    EGN_UINT8    aucReserved[4];        /* 64位对齐保留字节 */
#endif
}EgnSetRuleImport;

/*STRUCT< 设置告警值 >*/
typedef struct _EgnSetResAlarmThreshold
{
    EGN_UINT32  ulInType;       /* 告警值类型 */
    EGN_UINT32  ulInThreshold;  /* 具体上限告警值，该值的取值范围[50-95]，默认值85。 */
}EgnSetResAlarmThreshold;

/*STRUCT< 设置属性类型是否识别的结构体 >*/
typedef struct _EgnSetAttrTypeCfgInfo
{
    EGN_UINT32   ulInAttrTypeId;    /* 属性类型 */
    EGN_BOOL     bInIsEnable;       /* 是否识别 */
}EgnSetAttrTypeCfgInfo;

/*STRUCT< 设置EGN统计功能 >*/
typedef struct _EgnSetStatControl
{
    EGN_UINT32    ulInCommand;  /* 统计功能类型。取值范围为EGN_STAT_ONOFF_SWITCH、EGN_STAT_PROTOID_SET */
    EGN_UINT32    ulInContent;  /* 待设置的值。ulInCommand为EGN_STAT_ONOFF_SWITCH时，ulInContent表示统计开关的值，可以取EGN_TRUE、EGN_FALSE; ulInCommand为EGN_STAT_PROTOID_SET时，ulInContent表示待统计的一个协议ID值（默认是EGN_PROTO_ID_BT_DATA_TCP) */
}EgnSetStatControl;

/*STRUCT< 待设置信息的结构体 >*/
typedef struct _EgnSetCfgParamInfo
{
    union
    {
        EGN_UINT32                  ulInSetValue;               /* 待设置的整型类型的值 */
        EgnSetProtoImportOrRelParse stProtoImportOrRelParse;    /* 待设置的协议导入或解析信息 */
        EgnSetProtoHierInspect      stProtoHierInspect;         /* 待设置的层级识别阀值 */
        EgnSetRuleImport            stRuleImport;               /* 待设置的规则导入信息 */
        EgnSetResAlarmThreshold     stResAlarmThreshold;        /* 待设置的告警信息 */
        EgnSetAttrTypeCfgInfo       stAttrTypeCfgInfo;          /* 待设置的属性识别信息 */
        EgnSetStatControl           stStatControl;              /* 待设置的EGN统计功能 */
    }CfgParamInfoUn;
} EgnSetCfgParamInfo;

/*STRUCT< IP地址条件的结构体 >*/
typedef struct _EgnIpAddrCond
{
    EGN_UINT32   ulIpType;       /* IP类型#EGN_ADDR_TYPE_EN */
    EGN_UINT32   ulIpMaskLen;    /* ipmask或前缀,ipv4取值1-32,ipv6取值1-128 */
    union
    {
        EGN_UINT32   ulIpv4Addr;        /* IPv4地址,必须为大端 */
        EGN_UINT8    aucIpv4Addr[4];    /* IPv4地址,必须为大端 */
        EGN_UINT32   aulIpv6Addr[4];    /* IPv6地址,必须为大端 */
        EGN_UINT8    aucIpv6Addr[16];   /* IPv6地址,必须为大端 */
    }IpUn;
}EgnIpAddrCond;

/*STRUCT< NGFW自定义端口条件，暂时不支持端口范围，usStartPort和usEndPort需相等 >*/
typedef struct _EgnPortCond
{
    EGN_UINT16  usStartPort;    /* 起始端口 */
    EGN_UINT16  usEndPort;      /* 结束端口 */
#ifdef EGN_64
    EGN_UCHAR   aucReserved[4];
#endif
}EgnPortCond;

/*ENUM< DNS查询类型定义 >*/
typedef enum
{
    EGN_DNS_TYPE_BEGIN           = EGN_EN_INVALID,
    EGN_DNS_TYPE_A               = 1,                /* A类型 */
    EGN_DNS_TYPE_AAAA            = 28,                /* AAAA类型 */
    EGN_DNS_TYPE_CNAME           = 5,                /* CNAME类型 */
    EGN_DNS_TYPE_OTHER           = 255,              /* 其他EGN不关心的类型 */
    EGN_DNS_TYPE_END,
    EGN_DNS_TYPE_BOTTOM          = EGN_EN_BUTT
}EGN_DNS_TYPE_EN;

/*ENUM< DNS查询类定义 >*/
typedef enum
{
    EGN_DNS_CLASS_BEGIN           = EGN_EN_INVALID,
    EGN_DNS_CLASS_IN              = 1,                /* IN类代表internet系统 */
    EGN_DNS_CLASS_CH              = 2,                /* CH类代表Chaos系统 */
    EGN_DNS_CLASS_END,
    EGN_DNS_CLASS_BOTTOM          = EGN_EN_BUTT
}EGN_DNS_CLASS_EN;

/*STRUCT< DNS非IP类型的数据结构定义 */
typedef struct _EgnDnsOtherData
{
    EGN_UINT32   ulDataLen;             /* 非IP类型的回答数据长度 */
#ifdef EGN_64
        EGN_UINT8    aucReserved[4];        /* 64位对齐保留字节 */
#endif
    EGN_UCHAR   *pucData;               /* 非IP类型的回答数据 */
}EgnDnsOtherData;

/*STRUCT< DNS回答信息结构体定义 >*/
typedef struct _EgnDnsAnswer
{
    EGN_UCHAR   *pucAnswerName;         /* 回答对应的名长度 */
    EGN_UINT32   ulAnswerNameLen;       /* 回答对应的名 */
    EGN_UINT16   usType;                /* 回答类型，具体参见EGN_DNS_TYPE_EN */
    EGN_UINT16   usClass;               /* 回答对应的系统类型，具体参见EGN_DNS_CLASS_EN */
    EGN_UINT32   ulTTL;                 /* 回答在DNS解析器的存活时间，暂时不用关注 */
#ifdef EGN_64
    EGN_UINT8    aucReserved[4];        /* 64位对齐保留字节 */
#endif
    union
    {
        EGN_UINT32      ulIpV4Data;
        EGN_UINT32      aulIpV6Data[4];
        EgnDnsOtherData stOtherData;
    }DataUn;
}EgnDnsAnswer;

/*STRUCT< DNS回答信息链表结构定义 >*/
typedef struct _EgnDnsAnswerList
{
    EgnDnsAnswer                stDnsAnswer;    /* 回答信息节点 */
    struct _EgnDnsAnswerList   *pstNext;        /* 指向下一个回答 */
}EgnDnsAnswerList;

/*STRUCT< DNS查询信息结构体定义 >*/
typedef struct _EgnDnsQuery
{
    EGN_UCHAR   *pucQueryName;          /* 查询对应的名长度 */
    EGN_UINT32   ulQueryNameLen;        /* 查询对应的名 */
    EGN_UINT16   usType;                /* 查询类型，具体参见EGN_DNS_TYPE_EN */
    EGN_UINT16   usClass;               /* 查询对应的系统类型，具体参见EGN_DNS_CLASS_EN */
}EgnDnsQuery;

/*STRUCT< DNS查询信息链表结构定义 >*/
typedef struct _EgnDnsQueryList
{
    EgnDnsQuery                stDnsQuery;    /* 查询信息节点 */
    struct _EgnDnsQueryList   *pstNext;        /* 指向下一个查询 */
}EgnDnsQueryList;

/*STRUCT< DNS协议报文信息 >*/
typedef struct _EgnDnsPacketInfo
{
    EGN_UINT16          usTransactionID;      /* 标识字段 对应RFC文档中的标志*/
    EGN_UINT16          usFlags;              /* 标志字段 对应RFC文档中的标志*/
#ifdef EGN_64
    EGN_UINT8    aucReserved[4];        /* 64位对齐保留字节 */
#endif
    EgnDnsQueryList    *pstQueryHead;         /* 问题链表 */
    EgnDnsAnswerList   *pstAnswerHead;        /* 回答链表 */
}EgnDnsPacketInfo;

/*STRUCT< DNS阻断的识别结果 >*/
typedef struct _EgnDnsRequestResult
{
    EGN_UINT32  ulProtoID;      /* 识别出来的协议ID */
#ifdef EGN_64
        EGN_UINT8    aucReserved[4];        /* 64位对齐保留字节 */
#endif
}EgnDnsRequestResult;

/*STRUCT< 关联下发关联信息 >*/
typedef struct _EgnPeerListTupleInfo
{
    EGN_UINT32     ulTunnelId;           /* Tunnel   Id*/
    EGN_UINT8      ucTransProtocol;      /* 传输层协议*/
    EGN_UINT8      ucTupleFlag;          /* 三元组状态*/
    EGN_UINT16     usPort;               /* 端口 */
    EgnIpAddr     *pstIpAddr;            /* IP */
}EgnPeerListTupleInfo;

/*STRUCT< 关联下发识别结果信息 >*/
typedef struct _EgnProtoResultInfo
{
    EGN_UINT32  ulAppId;                                     /* 应用ID */
    EGN_UINT32  ulSuperClassId;                              /* 大类ID */
    EGN_UINT32  ulClassId;                                   /* 小类ID */
    EGN_UINT32  ulLastBearID;                                /* 承载ID */
    EGN_UINT32  ulProtoId;                                   /* 协议ID */
    #ifdef EGN_64
    EGN_UINT32  ulReserved;                                  /**< 64位对齐保留字节 */
    #endif
}EgnProtoResultInfo;

/*******************************************************************************
*    Func Name: EgnApiDnsBlockInspect
*      Purpose: 将完成DNS请求包的识别。
*  Description: 用户根据EgnDnsRequestResult 中的协议ID是否为0，判断EGN是否识别出来这个DNS应用协议。
*        Input: EGN_UINT16 usThreadID:实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*               EgnDnsPacketInfo* pstDnsInfo:DNS报文信息<非空>
*        InOut: EgnDnsRequestResult* pstInspectResult:识别结果<非空>
*        Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution:  该API在以下情况不可调用:1.成功初始化EGN识别线程之前
*                                        2.使用动态申请内存方式的产品不可被调用。
*                                        3.DNS应用识别开关为关的状态
*                                        4.session模式下
*        Since: V3R6C00SPC200
*    Reference: EgnApiInspectorInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiDnsBlockInspect
(
    IN                  EGN_UINT16               usThreadID,        /* 识别进程ID */
    IN      EGN_CONST   EGN_VOID                *pvHandle,          /* 实例句柄 */
    IN      EGN_CONST   EgnDnsPacketInfo      *pstDnsInfo,        /* 待识别的DNS协议信息 */
    INOUT               EgnDnsRequestResult     *pstInspectResult   /* 识别结果 */
);

/*******************************************************************************
*    Func Name: EgnApiDnsRelParse
*      Purpose: 解析DNS响应包。
*  Description: 解析DNS响应包，并提取IP信息，进行DNS关联识别。
                    根据pulAddIpNum的值，判断EGN解析并成功添加IP的个数。
*        Input: EGN_UINT16 usThreadID:实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*               EgnDnsPacketInfo* pstDnsInfo:DNS报文信息<非空>
*        InOut:
*       Output: EGN_UINT32* pulAddIpNum:成功添加IP的个数<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution:  该API在以下情况不可调用:1.成功初始化EGN识别线程之前
*                                        2.使用动态申请内存方式的产品不可被调用。
*                                        3.DNS应用识别开关为关的状态
*                                        4.session模式下
*        Since: V3R6C00SPC200
*    Reference: EgnApiInspectorInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiDnsRelParse
(
    IN              EGN_UINT16           usThreadID,
    IN              EGN_VOID            *pvHandle,
    IN  EGN_CONST   EgnDnsPacketInfo  *pstDnsInfo,
    OUT             EGN_UINT32          *pulAddIpNum
);

/*******************************************************************************
*    Func Name: EgnApiDnsDelIpFromIpTbl
*      Purpose: 删除某IP表项。
*  Description: 根据用户输入的IP删除相应的IP表项。
*        Input: EGN_UINT16 usThreadID:线程ID
*               EGN_VOID  *pvHandle:实例句柄<非空>
*               EgnIpAddr* pstIpAddr:IP结构<非空>
*        InOut: NA
*        Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution:  该API在以下情况不可调用:1.成功初始化EGN识别线程之前
*                                        2.使用动态申请内存方式的产品不可被调用。
*                                        3.DNS应用识别开关为关的状态
*                                        4.session模式下
*        Since: V3R6C00SPC200
*    Reference: EgnApiInspectorInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiDnsDelIpFromIpTbl
(
    IN              EGN_UINT16       usThreadID,
    IN  EGN_CONST   EGN_VOID        *pvHandle,
    IN  EGN_CONST   EgnIpAddr       *pstIpAddr
);

/*******************************************************************************
*    Func Name: EgnApiDnsGetIpFromIpTbl
*      Purpose: 查询某IP表项是否存在。
*  Description: 根据用户输入的IP查询是否现在
*        Input: EGN_UINT16 usThreadID:线程ID
*               EGN_VOID *pvHandle:实例句柄<非空>
*               EgnIpAddr* pstIpAddr:IP结构<非空>
*        InOut: NA
*        Output:EGN_BOOL* pbIsExist:该表项是否存在，0:不存在，1:存在
*               EGN_UINT32* pulProtoID:获取到的协议ID,0:表示未获取
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution:  该API在以下情况不可调用:1.成功初始化EGN识别线程之前
*                                        2.使用动态申请内存方式的产品不可被调用。
*                                        3.DNS应用识别开关为关的状态
*                                        4.session模式下
*        Since: V3R6C00SPC200
*    Reference: EgnApiInspectorInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiDnsGetIpFromIpTbl
(
    IN                  EGN_UINT16       usThreadID,
    IN      EGN_CONST   EGN_VOID        *pvHandle,
    IN      EGN_CONST   EgnIpAddr       *pstIpAddr,
    OUT                 EGN_BOOL        *pbIsExist,
    OUT                 EGN_UINT32      *pulProtoID
);

/*******************************************************************************
*    Func Name: EgnApiDnsGetIpTblInfo
*      Purpose: 打印所有IP实例的ip表项添加次数与更新次数
*  Description: 用户可通过该接口获取所有实例IP表项添加及更新次数( 不包括第一次添加的 )
*        Input: EGN_UINT16  usThreadID:线程ID
*               EGN_VOID   *pvHandle:实例句柄<非空>
*               EGN_UINT32  ulIpTblInfoLen:保存IP信息的内存总长度
*        InOut: EGN_UCHAR  *pucIpTblInfo:保存IP信息的内存首地址，建议传入2K内存
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution:  该API在以下情况不可调用:1.成功初始化EGN及SESSION组件之前
*                                        2.使用动态申请内存方式的产品不可被调用。
*                                        3.DNS应用识别开关为关的状态
*                                        4.session模式下
*        Since: V3R6C00SPC200
*    Reference: EgnApiInspectorInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiDnsGetIpTblInfo
(
    IN      EGN_UINT16       usThreadID,
    IN      EGN_VOID        *pvHandle,
    IN      EGN_UINT32       ulIpTblInfoLen,
    INOUT   EGN_UCHAR       *pucIpTblInfo
);

/*******************************************************************************
*    Func Name: EgnApiPacketInspect
*      Purpose: 将完成2～4层解析后的报文传入并进行协议识别。
*  Description: 根据输入实例线程ID、实例句柄、流识别句柄和报文获取识别结果及识别扩展参数。
                用户可以根据识别结果EgnResult中的bIsContinue来控制是否继续输入包到引擎中识别。
                若bIsContinue为EGN_TRUE需要继续输入包，若bIsContinue为EGN_FALSE不需要继续输入包。
*        Input: EGN_UINT16 usThreadID:实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*               EGN_VOID** ppvFlowInspect:流识别句柄<非空>
*               EgnPacket* pstPacketInfo:报文<非空>
*        InOut: EgnResult* pstIspectResult:识别结果<非空>
*               EgnInspectAuxData* pstAuxData:识别扩展参数<预留>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 该API在成功初始化EGN识别线程关联表非共享时调用EgnApiExpandInspectorInit成功
                或关联表共享时调用EgnApiInspectorInit成功之前不可被调用。
*        Since: V100R001C01
*    Reference: EgnApiInspectorInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiPacketInspect
(
    IN                EGN_UINT16           usThreadID,
    IN                EGN_VOID            *pvHandle,
    IN                EGN_VOID           **ppvFlowInspect,
    IN     EGN_CONST  EgnPacket           *pstPacketInfo,
    INOUT             EgnResult           *pstInspectResult,
    INOUT  EGN_CONST  EgnInspectAuxData   *pstAuxData
);

/*******************************************************************************
*    Func Name: EgnApiReleaseCtxData
*      Purpose: 释放指定的流识别句柄。
*  Description: 对于非首包识别的流，识别结束后使用该接口释放流识别句柄。
*        Input: EGN_UINT16 usThreadID:调用此接口的线程ID<0~65535>
*               EGN_VOID* pvHandle:多实例句柄，不能为空<非空>
*               EGN_VOID** ppvCtxData:流识别句柄<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_VOID
*      Caution: 该API在成功初始化EGN识别线程关联表非共享时调用EgnApiExpandInspectorInit成功
                或关联表共享时调用EgnApiInspectorInit成功之前不可被调用。
*        Since: V100R001C01
*    Reference: EgnApiInspectorInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_VOID EgnApiReleaseCtxData
(
    IN              EGN_UINT16    usThreadID,
    IN              EGN_VOID     *pvHandle,
    IN              EGN_VOID    **ppvCtxData
);

/*******************************************************************************
*    Func Name: EgnApiSetFlowMaxDetectCount
*      Purpose: 设置流最大检测阀值
*  Description: 设置流最大检测阀值，取值范围受uint32数值范围限制。
                流最大检测阀值默认配置在接口EgnApiInitPubParam中设置。
*        Input: EGN_UINT32 ulMaxDetectCnt:流最大检测阀值<大于零>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V100R001C01
*    Reference: EgnApiInitPubParam
*               EgnApiGetFlowMaxDetectCount
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSetFlowMaxDetectCount
(
    IN    EGN_UINT32    ulMaxDetectCnt
);

/*******************************************************************************
*    Func Name: EgnApiGetFlowMaxDetectCount
*      Purpose: 查询流最大检测阀值。
*  Description:
*        Input: NA
*        InOut: NA
*       Output: EGN_UINT32* pulMaxDetectCnt:流最大检测阀值<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: null
*        Since: V100R001C01
*    Reference: EgnApiSetFlowMaxDetectCount
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetFlowMaxDetectCount
(
    OUT    EGN_UINT32    *pulMaxDetectCnt
);

/*******************************************************************************
*    Func Name: EgnApiInitPubParam
*      Purpose: 生成EGN的默认初始化配置参数。
*  Description: 该接口提供了默认参数配置功能，按照缺省值，生成EGN的默认初始化配置参数。
                如果有些参数您不希望进行配置，或者配置起来比较困难，可以使用该接口进行配置。
                相关的配置参数，请参见EgnInitCfgParam的参数。
*        Input: EgnInitCfgParam* pstParam:初始化参数<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 该API在使用EgnApiRegSspFunc成功注册系统服务回调函数之前可被调用。
*        Since: V100R001C01
*    Reference: EgnInitCfgParam
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiInitPubParam
(
    IN  EgnInitCfgParam    *pstParam
);

/*******************************************************************************
*    Func Name: EgnApiGetSysInfo
*      Purpose: 收集EGN配置信息。
*  Description: EGN一键式信息收集接口，根据输入的实例线程ID(usThreadID)和实例句柄pvHandle，
                将该实例的配置信息保存在首地址为ppucSysInfo的内存中，长度为pulSysInfoLen。
*        Input: EGN_UINT16 usThreadID:实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*        InOut: NA
*       Output: EGN_UCHAR** ppucSysInfo:保存配置信息内存首地址<非空>
*               EGN_UINT32* pulSysInfoLen:保存配置信息内存长度<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、在调用该接口之前要EGN组件初始化完毕，并且保证动态内存申请接口已经注册。
                2、当API在识别线程被调用时，必须输入调用线程的识别实例句柄，如果输入为空则多线程可靠性校验会失效。
                3、当API在管理线程被调用时，识别实例句柄传入必须为空，此时不会进行多线程可靠性校验。
                4、EGN在去使能状态下，通过EgnApiGetSysInfo获取信息，不显示CB块和知识库的信息。
*        Since: V200R002C01
*    Reference: EgnApiFreeSysInfo
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetSysInfo
(
    IN            EGN_UINT16   usThreadID,
    IN            EGN_VOID    *pvHandle,
    OUT           EGN_UCHAR  **ppucSysInfo,
    OUT           EGN_UINT32  *pulSysInfoLen
);

/*******************************************************************************
*    Func Name: EgnApiFreeSysInfo
*      Purpose: 释放EGN一键式信息收集的内存。
*  Description: EGN一键式信息收集的内存释放接口，根据实例线程IDusThreadID和实例句柄pvHandle，
                释放EGN一键式信息收集的内存。
*        Input: EGN_UINT16 usThreadID:实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*        InOut: NA
*       Output: EGN_UCHAR** ppucSysInfo:保存配置信息内存首地址<非空>
*       Return: 无
*      Caution: 当API在识别线程被调用时，必须输入调用线程的识别实例句柄，如果输入为空则多线程可靠性校验会失效。
                当API在管理线程被调用时，识别实例句柄传入必须为空，此时不会进行多线程可靠性校验。
*        Since: V200R002C01
*    Reference: EgnApiGetSysInfo
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_VOID EgnApiFreeSysInfo
(
    IN              EGN_UINT16  usThreadID,
    IN              EGN_VOID   *pvHandle,
    INOUT           EGN_UCHAR **ppucSysInfo
);

/*******************************************************************************
*    Func Name: EgnApiGetHealthInfo
*      Purpose: 获取一键式检查组件健康信息。
*  Description: EGN一键式健康检查接口，完成EGN组件相关信息的健康检查，
                并将对应实例的健康信息存储在首地址为ppucHealthInfo的内存中。
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
*    Reference: EgnApiFreeHealthInfo
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetHealthInfo
(
    IN            EGN_UINT16   usThreadID,
    IN            EGN_VOID    *pvHandle,
    OUT           EGN_UCHAR  **ppucHealthInfo,
    OUT           EGN_UINT32  *pulHealthInfoLen
);

/*******************************************************************************
*    Func Name: EgnApiFreeHealthInfo
*      Purpose: 释放一键式健康检查信息的内存。
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
*    Reference: EgnApiGetHealthInfo
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_VOID EgnApiFreeHealthInfo
(
    IN              EGN_UINT16    usThreadID,
    IN              EGN_VOID     *pvHandle,
    INOUT           EGN_UCHAR   **ppucHealthInfo
);

/*******************************************************************************
*    Func Name: EgnApiMngInit
*      Purpose: 初始化管理线程
*  Description: EGN管理线程初始化接口，完成EGN组件需要使用的系统资源的申请、初始化等工作。
                调用EgnApiMngInit前，需要先调用EgnApiInitPubParam初始化EGN的配置参数，按照缺省值，
                生成EGN的默认配置值。如果默认值能满足业务需求，可以直接调用EgnApiMngInit初始化。
                如果默认值不能满足业务需求，可以根据业务需求修改相应的配置值，
                再调用EgnApiMngInit初始化EGN模块的管理线程。
*        Input: EGN_UCHAR* pucGlobalBuffer:全局内存首地址<非空>
*               EGN_UINT32 ulBufferLen:全局内存长度<大于零>
*               EgnInitCfgParam* pstParam:参数信息<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、除特殊说明外，其他API应在成功初始化EGN管理线程成功调用该API之后才能被使用。
                2、在动态内存模式下不能调用该接口，否则会导致识别业务异常。
*        Since: V200R001C01
*    Reference: EgnApiInitPubParam
*               EgnApiMngDestroy
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiMngInit
(
   IN            EGN_UCHAR           *pucGlobalBuffer,
   IN            EGN_ULONG            ulBufferLen,
   IN  EGN_CONST EgnInitCfgParam     *pstParam
);

/*******************************************************************************
*    Func Name: EgnApiExpandInspectorInit
*      Purpose: 关联表非共享时，初始化识别实例。
*  Description: 关联表非共享时，引擎识别实例初始化接口。完成业务进程、线程的初始化。
                根据输入的线程IDusThreadID、共享内存池首地址pucSharedBuffer、共享内存池长度ulSharedBufferLen、
                非共享内存池首地址pucUnsharedBuffer、非共享内存池长度ulUnsharedBufferLen，
                创建识别实例句柄ppvHandle，且初始化引擎为此实例创建的句柄，外部不能改变。
*        Input: EGN_UINT16 usThreadID:调用此接口的线程ID<0~65535>
*               EGN_UCHAR* pucSharedBuffer:共享内存池首地址<非空>
*               EGN_UINT32 ulSharedBufferLen:共享内存池长度<大于零>
*               EGN_UCHAR* pucUnsharedBuffer:非共享内存池首地址<非空>
*               EGN_UINT32 ulUnsharedBufferLen:非共享内存池长度<大于零>
*        InOut: NA
*       Output: EGN_VOID** ppvHandle:实例句柄，初始化引擎为此实例创建的句柄，外部不能改变<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、除特殊说明外，其他API应在成功初始化EGN识别线程关联表非共享时，成功调用该API之后才能被使用。
                2、多实例时识别实例必须逐个的初始化。在关联表非共享的情况下，最大只支持32个实例。
*        Since: V200R002C00
*    Reference: EgnApiInspectorInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32  EgnApiExpandInspectorInit
(
   IN  EGN_UINT16           usThreadID,
   IN  EGN_UCHAR           *pucSharedBuffer,
   IN  EGN_ULONG            ulSharedBufferLen,
   IN  EGN_UCHAR           *pucUnsharedBuffer,
   IN  EGN_ULONG            ulUnsharedBufferLen,
   OUT EGN_VOID           **ppvHandle
);

/*******************************************************************************
*    Func Name: EgnApiInspectorInit
*      Purpose: 初始化识别实例。
*  Description: 引擎识别实例初始化接口，在多进程、多线程模型下调用，完成业务进程、线程的初始化，
                多实例时识别实例必须逐个的初始化。
*        Input: EGN_UINT16 usThreadID:调用此接口的线程ID<0~65535>
*               EGN_UCHAR* pucGlobalBuffer:共享线程内存池首地址<非空>
*               EGN_UINT32 ulBufferLen:共享线程内存池长度<大于零>
*        InOut: NA
*       Output: EGN_VOID** ppvHandle:实例句柄，初始化引擎为此实例创建的句柄，外部不能改变<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 除特殊说明外，其他API应在成功初始化EGN识别线程关联表共享时，成功调用该API之后才能被使用。
*        Since: V200R002C00
*    Reference: EgnApiInspectorDestroy
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiInspectorInit
(
    IN     EGN_UINT16         usThreadID,
    IN     EGN_UCHAR         *pucGlobalBuffer,
    IN     EGN_ULONG          ulBufferLen,
    OUT    EGN_VOID         **ppvHandle
);

/*******************************************************************************
*    Func Name: EgnApiMngDestroy
*      Purpose: 去初始化管理线程
*  Description: 完成管理进程、线程系统资源的释放等工作。
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、该API在成功初始化EGN管理线程成功调用EgnApiMngInit之前可被调用。
                2、多进程、多线程模型下调用，单线程模型下不能调用本接口。
                3、在动态内存模式下不能调用该接口，否则会导致识别业务异常。
*        Since: V100R001C03
*    Reference: EgnApiMngInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiMngDestroy
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiInspectorDestroy
*      Purpose: 去初始化识别实例。
*  Description: 完成业务进程、线程系统资源的释放等工作。
*        Input: EGN_UINT16 usThreadID:调用此接口的线程ID<0~65535>
*               EGN_VOID** ppvHandle:实例句柄<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、多实例时识别实例必须逐个的去初始化。
                2、在动态内存模式下不能调用该接口，否则会导致识别业务异常。
*        Since: V200R002C00
*    Reference: EgnApiInspectorInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiInspectorDestroy
(
    IN  EGN_UINT16  usThreadID,
    IN  EGN_VOID  **ppvHandle
);

/*******************************************************************************
*    Func Name: EgnApiGetNeededMemSize
*      Purpose: 计算引擎所需内存。
*  Description: 该API接口根据最大流规模ulFlowScale、关联表大小ulRelationScale、
                识别实例个数usMaxInstance和内存是否共享bIsMemShared计算出管理实例
                所需共享内存大小pulSharedMemSize和每个识别实例所需非共享内存的大小pulUnSharedMemSize，单位字节。
*        Input: EGN_UINT32 ulFlowScale:最大流规模<1000~10000000>
*               EGN_UINT32 ulRelScaleIPv4:IPv4关联表大小<0~10000000>
*               EGN_UINT32 ulRelScaleIPv6:IPv6关联表大小<0~10000000>
*               EGN_UINT16 usMaxInstance:识别实例个数<1~32>
*               EGN_BOOL bIsMemShared:是否共享内存<EGN_FALSE：不共享；EGN_TRUE：共享>
*        InOut: EGN_UINT32* pulSharedMemSize:管理实例所需共享内存大小<非空>
*               EGN_UINT32* pulUnSharedMemSize:每个识别实例所需非共享内存的大小<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、该API在使用EgnApiRegSspFunc成功注册系统服务回调函数之前可被调用。
                2、用户使用时严格按照需求传入相应参数，该API接口只能在内存机制为非受限的时候才有效。
                   即进行EGN参数配置时，将ulMemPolicy初始化为EGN_MEM_POLICY_UNLIMIT，EGN默认配置该参数为非受限。
                3、在动态内存模式下不能调用该接口，否则会导致识别业务异常。
*        Since: V100R001C03
*    Reference: EgnApiInitPubParam
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetNeededMemSize
(
    IN     EGN_UINT32   ulFlowScale,
    IN     EGN_UINT32   ulRelScaleIPv4,
    IN     EGN_UINT32   ulRelScaleIPv6,
    IN     EGN_UINT16   usMaxInstance,
    IN     EGN_BOOL     bIsMemShared,
    INOUT  EGN_ULONG   *pulSharedMemSize,
    INOUT  EGN_ULONG   *pulUnSharedMemSize
);

/*******************************************************************************
*    Func Name: EgnApiImportSysRuleLib
*      Purpose: 导入系统知识库。
*  Description: 导入方式可以是内存方式，也可以是文件方式。优先使用内存方式，当文件内存指针非空时，
                使用内存方式。当文件内存指针为空或文件内存大小为零时，使用文件方式。
*        Input: EGN_UINT8* pucRuleLibBuff:文件内存指针<非空>
*               EGN_UINT32 ulBuffLen:文件内存大小<大于零>
*               EGN_UINT8* pcRuleFilePath:文件路径<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V100R001C01
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiImportSysRuleLib
(
   IN             EGN_UINT8   *pucRuleLibBuff,
   IN             EGN_UINT32   ulBuffLen,
   IN   EGN_CONST EGN_UINT8   *pucRuleFilePath
);

/*******************************************************************************
*    Func Name: EgnApiImportUserRuleLib
*      Purpose: 导入用户定义知识库。
*  Description: 导入方式可以是内存方式，也可以是文件方式。优先使用内存方式，当文件内存指针非空时，
                使用内存方式。当文件内存指针为空或文件内存大小为零时，使用文件方式。
*        Input: EGN_UINT8* pucRuleLibBuff:文件内存指针<非空>
*               EGN_UINT32 ulBuffLen:文件内存大小<大于零>
*               EGN_UINT8* pcRuleFilePath:文件路径<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 在导入用户自定义知识库前，需要先导入系统知识库。
*        Since: V100R001C01
*    Reference: EgnApiImportSysRuleLib
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiImportUserRuleLib
(
   IN            EGN_UINT8   *pucRuleLibBuff,
   IN            EGN_UINT32   ulBuffLen,
   IN  EGN_CONST EGN_UINT8   *pucRuleFilePath
);

/*******************************************************************************
*    Func Name: EgnApiLoadRuleLib
*      Purpose: 加载知识库。
*  Description: 加载知识库。
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 在加载知识库前，需要先导入系统知识库，否则加载失败。
*        Since: V100R001C01
*    Reference: EgnApiImportSysRuleLib
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiLoadRuleLib
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiApplyRuleLib
*      Purpose: 启用知识库。
*  Description: 启用知识库。
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 在启用知识库前，需要先加载知识库，否则启用失败。
*        Since: V100R001C01
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiApplyRuleLib
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiReleaseRuleLib
*      Purpose: 释放备知识库。
*  Description: 释放备知识库。
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V100R001C01
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiReleaseRuleLib
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiRollBackRuleLib
*      Purpose: 回退知识库。
*  Description: 升级并启用新知识库后，在一定时间内可以使用该接口回退到老知识库，放弃该次的升级。
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，参见EGN_RET_RESULT_EN。
*      Caution: 回退必须在指定的时间内默认为8小时进行，超过该时间，回退失败。
*        Since: V100R001C03
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiRollBackRuleLib
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiGetRuleLibState
*      Purpose: 获取知识库状态。
*  Description: 获取当前主知识库索引号和主备知识库的状态。
*        Input: NA
*        InOut: NA
*       Output: EGN_UINT32* pulRuleLibIdx:主知识库索引<非空>
*               EGN_UINT32* pulMasterLibState:主知识库状态<非空>
*               EGN_UINT32* pulSlaveLibState:备知识库状态<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V100R001C01
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetRuleLibState
(
    OUT EGN_UINT32 *pulRuleLibIdx,
    OUT EGN_UINT32 *pulMasterLibState,
    OUT EGN_UINT32 *pulSlaveLibState
);

/*******************************************************************************
*    Func Name: EgnApiGetSuperClassInfo
*      Purpose: 按照大类ID或名称查询大类信息。
*  Description: 按照大类ulSuperClassID或大类名称pucSuperClassName查询大类信息数组pstSuperClassInfo
                    和大类数组的大小pulItemNum只能查询系统知识库信息。
*        Input: EGN_UINT32 ulSuperClassID:大类ID<0~65535>
*               EGN_UINT8* pucSuperClassName:大类名，大小写不敏感<非空>
*        InOut: EgnClassInfo* pstSuperClassInfo:大类信息数组<非空>
*               EGN_UINT32* pulItemNum:大类数组的大小<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 如果在下次调用EgnApiLoadProtoInfoLib或EgnApiReleaseProtoInfoLib。
                之后还想继续使用本次的查询结果，需要将EgnSuperClassInfo中的stSuperClassName和stSuperClassDesc拷贝出来。
                因为指针pstSuperClassInfo再次调用后已经改变。
*        Since: V300R006C00
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetSuperClassInfo
(
    IN    EGN_UINT32            ulSuperClassID,
    IN    EGN_UINT8            *pucSuperClassName,
    INOUT EgnSuperClassInfo    *pstSuperClassInfo,
    INOUT EGN_UINT32           *pulItemNum
);

/*******************************************************************************
*    Func Name: EgnApiGetClassInfo
*      Purpose: 按照子类ID或名称查询大类信息。
*  Description: 按照子类ulClassID或子类名称pucClassName查询子类信息数组pstClassInfo
                    和子类数组的大小pulItemNum只能查询系统知识库信息。
*        Input: EGN_UINT32 ulClassID:子类ID<0~65535>
*               EGN_UINT8* pucClassName:子类名，大小写不敏感<非空>
*        InOut: EgnClassInfo* pstClassInfo:子类信息数组<非空>
*               EGN_UINT32* pulItemNum:子类数组的大小<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 如果在下次调用EgnApiLoadProtoInfoLib或EgnApiReleaseProtoInfoLib。
                之后还想继续使用本次的查询结果，需要将EgnClassInfo中的stClassName和stClassDesc拷贝出来。
                因为指针pstClassInfo再次调用后已经改变。
*        Since: V100R001C01
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetClassInfo
(
    IN    EGN_UINT32      ulClassID,
    IN    EGN_UINT8      *pucClassName,
    INOUT EgnClassInfo   *pstClassInfo,
    INOUT EGN_UINT32     *pulItemNum
);

/*******************************************************************************
*    Func Name: EgnApiGetAppInfo
*      Purpose: 按照应用ID或名称查询应用信息。
*  Description: 按照应用IDulAppID或应用名称pucAppName查询应用信息数组pstAppInfo
                    和应用数组的大小pulItemNum只能查询系统知识库信息。
*        Input: EGN_UINT32 ulAppID:应用ID<需要查EgnTxt.txt>
*               EGN_UINT8* pucAppName:应用名，大小写不敏感<非空>
*        InOut: EgnAppInfo* pstAppInfo:应用信息数组<非空>
*               EGN_UINT32* pulItemNum:应用数组的大小<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 如果在下次调用EgnApiLoadProtoInfoLib或EgnApiReleaseProtoInfoLib之后还想继续使用本次的查询结果，
                需要将EgnAppInfo中的stAppName和stAppDesc拷贝出来。因为指针pstAppInfo再次调用后已经改变。
*        Since: V200R002C01
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetAppInfo
(
    IN    EGN_UINT32    ulAppID,
    IN    EGN_UINT8    *pucAppName,
    INOUT EgnAppInfo   *pstAppInfo,
    INOUT EGN_UINT32   *pulItemNum
);

/*******************************************************************************
*    Func Name: EgnApiGetProtoInfo
*      Purpose: 按照协议ID或名称查询协议信息。
*  Description: 按照协议IDulProtoID或协议名称pucProtoName查询协议信息数组pstProtoInfo
                和协议数组的大小pulItemNum只能查询系统知识库信息。
*        Input: EGN_UINT32 ulProtoID:协议ID<需要查EgnTxt.txt>
*               EGN_UINT8* pucProtoName:协议名，大小写不敏感<非空>
*        InOut: EgnProtoInfo* pstProtoInfo:协议信息数组<非空>
*               EGN_UINT32* pulItemNum:协议数组的大小<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 如果在下次调用EgnApiLoadProtoInfoLib或EgnApiReleaseProtoInfoLib之后还想继续使用本次的查询结果，
                需要将EgnProtoInfo中的stProtoName和stProtoDesc拷贝出来。因为指针pstProtoInfo再次调用后已经改变。
*        Since: V100R001C01
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetProtoInfo
(
    IN    EGN_UINT32      ulProtoID,
    IN    EGN_UINT8      *pucProtoName,
    INOUT EgnProtoInfo   *pstProtoInfo,
    INOUT EGN_UINT32     *pulItemNum
);

/*******************************************************************************
*    Func Name: EgnApiGetRuleLibVersion
*      Purpose: 获取知识库版本。
*  Description: 获取知识库的版本信息pstRuleLibVersion。
*        Input: NA
*        InOut: EgnRuleLibVersion pstRuleLibVersion:知识库版本结构<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V100R001C01
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetRuleLibVersion
(
    INOUT EgnRuleLibVersion *pstRuleLibVersion
);

/*******************************************************************************
*    Func Name: EgnApiGetRuleLibNormalVersion
*      Purpose: 获取主知识库正式版本号
*  Description: 获取主知识库的正式版本号pucRuleLibNorVer。
*        Input: NA
*        InOut: EGN_UINT32* pulRuleLibNorVerLen:输入时是缓存长度，输出时是正式版本号实际长度<非空>
*               EGN_UCHAR* pucRuleLibNorVer:存放版本号的内存，长度至少为正式版本号实际长度，建议初次设32字节<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution: 无
*        Since: V300R005C02
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetRuleLibNormalVersion
(
    INOUT EGN_UINT32   *pulRuleLibNorVerLen,
    INOUT EGN_UCHAR    *pucRuleLibNorVer
);

/*******************************************************************************
*    Func Name: EgnApiGetProductVersion
*      Purpose: 获取当前使用识别引擎组件的产品版本号。
*  Description: 获取当前使用识别引擎组件的产品版本号。
*        Input: NA
*        InOut: EGN_UINT32 pulProductVerLen:输入时缓存长度，输出是实际长度<非空>
*               EGN_CHAR* pucProductVer:存放版本号的内存，长度至少为65个字节<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V200R001C01
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetProductVersion
(
    INOUT EGN_UINT32   *pulProductVerLen,
    INOUT EGN_CHAR     *pcProductVer
);

/*******************************************************************************
*    Func Name: EgnApiGetVersion
*      Purpose: 查询识别引擎当前的版本号。
*  Description: 维护接口，查询识别引擎当前的版本号。
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_UCHAR，EGN当前的版本号，外部不能释放返回的地址
*      Caution: 该API在使用EgnApiRegSspFunc成功注册系统服务回调函数之前可被调用。
*        Since: V100R001C01
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UCHAR *EgnApiGetVersion
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiSetStatControl
*      Purpose: 控制EGN统计功能
*  Description: 该接口控制EGN统计功能开关及指定统计哪个协议。ulCommand为EGN_STAT_ONOFF_SWITCH时，ulContent可以取EGN_TRUE、EGN_FALSE;
                ulCommand为EGN_STAT_PROTOID_SET时，ulContent为一个协议ID值（默认是EGN_PROTO_ID_BT_DATA_TCP）。
*        Input: EGN_UINT32 ulCommand:统计信息控制选项<EGN_STAT_ONOFF_SWITCH、EGN_STAT_PROTOID_SET>
*               EGN_UINT32 ulContent:统计信息控制选项<EGN_TRUE/EGN_FALSE or 协议ID>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: ulCommand为EGN_STAT_ONOFF_SWITCH时，ulContent表示统计开关的值，可以取EGN_TRUE、EGN_FALSE；
                ulCommand为EGN_STAT_PROTOID_SET时，ulContent表示待统计的一个协议ID值（默认是EGN_PROTO_ID_BT_DATA_TCP），
                当进行协议ID配置后，会将上次的关联表统计结果清零。
*        Since: V200R002C02
*    Reference: EgnApiGetStatControl
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSetStatControl
(
    IN      EGN_CONST    EGN_UINT32    ulCommand,
    IN      EGN_CONST    EGN_UINT32    ulContent
);

/*******************************************************************************
*    Func Name: EgnApiGetStatControl
*      Purpose: 查询EGN统计信息
*  Description: 该接口查询EGN统计功能开关状态，查询统计的是哪个协议。
*        Input: EGN_UINT32 ulCommand:统计信息控制选项<EGN_STAT_ONOFF_SWITCH、EGN_STAT_PROTOID_SET>
*        InOut: EGN_UINT32* pulContent:统计信息<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: ulCommand为EGN_STAT_ONOFF_SWITCH时，pulContent为用户配置的统计开关值，可以取EGN_TRUE、EGN_FALSE；
                ulCommand为EGN_STAT_PROTOID_SET时，pulContent为用户配置的一个协议ID值（如不进行任何设置时，默认是EGN_PROTO_ID_BT_DATA_TCP）。
*        Since: V200R002C02
*    Reference: EgnApiSetStatControl
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetStatControl
(
    IN      EGN_CONST    EGN_UINT32    ulCommand,
    INOUT                EGN_UINT32   *pulContent
);

/*******************************************************************************
*    Func Name: EgnApiStatGet
*      Purpose: 获取EGN内部统计信息。
*  Description: 获取内部统计信息，用户外部自行申请内存，传入指针。内部统计项包括识别数据包的总数，
                识别成功的总数，未识别的总数，关联表相关统计项。
*        Input: NA
*        InOut: EgnStatInfo* pstStatRslt:输出统计数据<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V200R001C03
*    Reference: EgnApiStatClear
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiStatGet
(
    INOUT EgnStatInfo *pstStatRslt
);

/*******************************************************************************
*    Func Name: EgnApiStatClear
*      Purpose: 清空EGN内部统计信息。
*  Description: 将EGN内部的统计项全部清零，重新开始统计。内部统计项包括识别数据包的总数，
                识别成功的总数，未识别的总数，关联表相关统计项。
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 在开始进行统计前，建议先使用该接口清空以前的统计信息，否则，统计信息中包含以前的信息。
*        Since: V200R001C03
*    Reference: EgnApiStatGet
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiStatClear
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiFastRelationSet
*      Purpose: 设置快速关联识别的开关状态和老化时间
*  Description: 设置快速关联识别的开关状态和老化时间，当前版本下老化时间设置不生效。
*        Input: EGN_BOOL bSwitch:快速关联开关<EGN_FALSE:关
                                             EGN_TRUE:开
                                             默认:开>
*               EGN_UINT32 ulAgingTime:老化时间(单位:分钟)，当前版本下设置不生效<3~30>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V200R001C03
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiFastRelationSet
(
    IN  EGN_CONST  EGN_BOOL     bSwitch,
    IN  EGN_CONST  EGN_UINT32   ulAgingTime
);

/*******************************************************************************
*    Func Name: EgnApiFastRelationGet
*      Purpose: 获取快速关联识别的开关状态和老化时间。
*  Description: 获取快速关联识别的开关状态和老化时间。
*        Input: NA
*        InOut: NA
*       Output: EGN_BOOL* pbSwitch:快速关联开关，EGN_FALSE:关；EGN_TRUE:开<非空>
*               EGN_UINT32* pulAgingTime:老化时间<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V200R001C03
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiFastRelationGet
(
    OUT  EGN_BOOL      *pbSwitch,
    OUT  EGN_UINT32    *pulAgingTime
);

/*******************************************************************************
*    Func Name: EgnApiSetWellknownPortState
*      Purpose: 设置知名端口功能是否开启。
*  Description: 设置知名端口功能是否开启。
*        Input: EGN_BOOL bWellkonwnPortState:设置知名端口功能是否开启开关<EGN_FALSE:关
                                                                        EGN_TRUE:开，
                                                                        默认:关>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V200R001C03
*    Reference: EgnApiGetWellknownPortState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSetWellknownPortState
(
    IN EGN_BOOL  bWellkonwnPortState
);

/*******************************************************************************
*    Func Name: EgnApiGetWellknownPortState
*      Purpose: 获取知名端口功能开启状态。
*  Description: 获取知名端口功能开启状态。
*        Input: NA
*        InOut: NA
*       Output: EGN_BOOL pbWellkonwnPortState:取得知名端口功能开启状态<EGN_FALSE:关
                                                                     EGN_TRUE:开>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V200R001C03
*    Reference: EgnApiSetWellknownPortState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetWellknownPortState
(
    OUT EGN_BOOL  *pbWellkonwnPortState
);

/*******************************************************************************
*    Func Name: EgnApiGetIdleRelationCBCount
*      Purpose: 获取空闲的关联识别CB块的个数。
*  Description: 根据输入的实例线程ID和实例句柄，获取空闲的关联识别CB块的个数。
*        Input: EGN_UINT16 usThreadID:实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*        InOut: EGN_UINT32* pulIdleCBCountIPv4:空闲的IPv4关联识别CB块的个数<非空>
*               EGN_UINT32* pulIdleCBCountIPv6:空闲的IPv6关联识别CB块的个数<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、该API在初始化EGN识别线程调用EgnApiExpandInspectorInit或调用EgnApiInspectorInit之前不可被调用。
                2、当API在识别线程被调用时，必须输入调用线程的识别实例句柄，如果输入为空则多线程可靠性校验会失效。
                3、当API在管理线程被调用时，识别实例句柄传入必须为空，此时不会进行多线程可靠性校验。
*        Since: V200R001C03
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetIdleRelationCBCount
(
    IN      EGN_UINT16  usThreadID,
    IN      EGN_VOID   *pvHandle,
    INOUT   EGN_UINT32 *pulIdleCBCntIPv4,
    INOUT   EGN_UINT32 *pulIdleCBCntIPv6
);

/*******************************************************************************
*    Func Name: EgnApiLoadProtoInfoLib
*      Purpose: 加载知识库明文协议信息。
*  Description: 加载方式可以是内存方式，也可以是文件方式。
                优先使用内存方式，当文件内存指针非空时，使用内存方式。
                当文件内存指针为空或文件内存大小为零时，使用文件方式。
                导入明文知识库信息后，可以调用
                EgnApiGetRuleLibVersion、
                EgnApiGetClassInfo、
                EgnApiGetAppInfo、
                EgnApiGetProtoInfo、
                EgnApiGetRuleLibState
                查询知识库相关信息。
*        Input: EGN_UINT8* pucProtoInfoLibBuff:文件内存指针<非空>
*               EGN_UINT32 ulBuffLen:文件内存大小<大于零>
*               EGN_UINT8* pucProtoInfoFilePath:文件路径<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 该接口在识别初始化后只调用一次。该接口是用户导入明文的协议信息时调用。
                当知识库切换时，用户需要导入新知识库对应的明文协议信息。
*        Since: V200R002C00
*    Reference: EgnApiGetRuleLibVersion
*               EgnApiGetClassInfo
*               EgnApiGetAppInfo
*               EgnApiGetProtoInfo
*               EgnApiGetRuleLibState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiLoadProtoInfoLib
(
    IN             EGN_UINT8   *pucProtoInfoLibBuffer,
    IN             EGN_UINT32   ulBufferLen,
    IN  EGN_CONST  EGN_UINT8   *pucProtoInfoFilePath
);

/*******************************************************************************
*    Func Name: EgnApiLoadProtoInfoLibIndependently
*      Purpose: 加载知识库明文协议信息,不校验知识库的版本号以及发布日期
*  Description: 加载方式可以是内存方式，也可以是文件方式。
                优先使用内存方式，当文件内存指针非空时，使用内存方式。
                当文件内存指针为空或文件内存大小为零时，使用文件方式。
                导入明文知识库信息后，可以调用
                EgnApiGetRuleLibVersion、
                EgnApiGetClassInfo、
                EgnApiGetAppInfo、
                EgnApiGetProtoInfo、
                EgnApiGetRuleLibState
                查询知识库相关信息。
*        Input: EGN_UINT8* pucProtoInfoLibBuff:文件内存指针<非空>
*               EGN_UINT32 ulBuffLen:文件内存大小<大于零>
*               EGN_UINT8* pucProtoInfoFilePath:文件路径<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 该接口在识别初始化后只调用一次。该接口是用户导入明文的协议信息时调用。
                当知识库切换时，用户需要导入新知识库对应的明文协议信息。
*        Since: V200R002C00
*    Reference: EgnApiGetRuleLibVersion
*               EgnApiGetClassInfo
*               EgnApiGetAppInfo
*               EgnApiGetProtoInfo
*               EgnApiGetRuleLibState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiLoadProtoInfoLibIndependently
(
    IN             EGN_UINT8   *pucProtoInfoLibBuffer,
    IN             EGN_UINT32   ulBufferLen,
    IN  EGN_CONST  EGN_UINT8   *pucProtoInfoFilePath
);

/*******************************************************************************
*    Func Name: EgnApiReleaseProtoInfoLib
*      Purpose: 释放协议、Class、App查询信息内存。
*  Description: 释放协议、Class、App查询信息内存。
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: 无
*      Caution: 无
*        Since: V200R002
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_VOID EgnApiReleaseProtoInfoLib
(
    EGN_VOID
);

#ifdef EGN_HA
/*******************************************************************************
*    Func Name: EgnApiDecomposeRuleLib
*      Purpose: 分解知识库
*  Description: 硬件加速方案中使用到的软硬件结合的知识库，这时需要将知识库分解为软件知识库
                和硬件知识库本接口提供分解知识库功能，输入知识库指针和知识库长度，
                获取软件知识库长度、硬件知识库长度以及获取软件知识库、硬件知识库。
*        Input: EGN_CONST pucRuleLibBuff:EGN_UINT8。知识库Buff<非空>
*               EGN_CONST ulBuffLen:EGN_UINT32。知识库Buff长度<大于零>
*        InOut: NA
*       Output: EGN_UINT32* pulSwRuleLibLen:软件知识库Buff长度<非空>
*               EGN_UINT8** ppucSwRuleLibBuf:软件知识库Buff<非空>
*               EGN_UINT32* pulHwRuleLibLen:硬件知识库Buff长度<非空>
*               EGN_UINT8** ppucHwRuleLibBuf:硬件知识库Buff<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、该API在成功初始化EGN管理线程成功调用EgnApiMngInit之前可被调用。
                2、在动态内存模式下不能调用该接口，否则会导致识别业务异常。
*        Since: V200R002C01
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiDecomposeRuleLib
(
    IN   EGN_CONST EGN_UINT8    *pucRuleLibBuff,
    IN   EGN_CONST EGN_UINT32    ulBuffLen,
    OUT  EGN_UINT32             *pulSwRuleLibLen,
    OUT  EGN_UINT8             **ppucSwRuleLibBuf,
    OUT  EGN_UINT32             *pulHwRuleLibLen,
    OUT  EGN_UINT8             **ppucHwRuleLibBuf
);

/*******************************************************************************
*    Func Name: EgnApiHAPacketInspect
*      Purpose: 硬件加速软件识别接口。硬件加速不支持IPv6
*  Description: 通过硬件加速将完成2～4层解析的报文传入进行协议识别。
                用户根据EgnHAResult中的ulContinue来控制识别方式，EGN_HA_INPUT_PACKET_STOP停止，
                EGN_HA_INPUT_PACKET_SOFT送软件，EGN_HA_INPUT_PACKET_HARD送硬件加速。
*        Input: EGN_UINT16 usThreadID:多实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*               EGN_VOID** ppvFlowInspect:流识别句柄<非空>
*               EgnPacket* pstPacketInfo:报文<非空>
*        InOut: NA
*       Output: EGN_UINT32 ulRuleLibId:硬件知识库ID<0,1>
*               EGN_UINT32 ulRuleNum:硬件识别规则个数<1~3872>
*               EGN_UINT32* pulRuleId:硬件识别规则列表<非空>
*               EgnHAResult* pstInspectResult:识别结果<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、该API在成功初始化EGN识别线程成功调用EgnApiInspectorInit之前不可被调用。
                2、在动态内存模式下不能调用该接口，否则会导致识别业务异常。
                3、硬件规则为(128,3999),最大规则个数为3999-128+1=3872
*        Since: V200R002C01
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiHAPacketInspect
(
    IN           EGN_UINT16    usThreadID,
    IN           EGN_VOID     *pvHandle,
    IN           EGN_VOID    **ppvFlowInspect,
    IN EGN_CONST EgnPacket    *pstPacketInfo,
    IN           EGN_UINT32    ulRuleLibId,
    IN           EGN_UINT32    ulRuleNum,
    IN EGN_CONST EGN_UINT32   *pulRuleId,
    OUT          EgnHAResult  *pstInspectResult
);
#endif

/*******************************************************************************
*    Func Name: EgnApiSetBehaviorInspectState
*      Purpose: 设置行为特征识别功能是否开启。
*  Description: 行为特征识别功能开关。
*        Input: EGN_BOOL bBehaviorInspectState:<EGN_FALSE：关
                                                EGN_TRUE：开
                                                默认：开>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V200R002C02
*    Reference: EgnApiGetBehaviorInspectState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSetBehaviorInspectState
(
    IN EGN_BOOL  bBehaviorInspectState   /* 行为特征识别配置状态 */
);

/*******************************************************************************
*    Func Name: EgnApiGetBehaviorInspectState
*      Purpose: 获取行为特征识别功能状态
*  Description: 获取行为特征识别功能状态
*        Input: NA
*        InOut: NA
*       Output: EGN_BOOL pbBehaviorInspectState:<EGN_FALSE:关
                                                 EGN_TRUE:开>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V200R002C02
*    Reference: EgnApiSetBehaviorInspectState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetBehaviorInspectState
(
    OUT EGN_BOOL  *pbBehaviorInspectState   /* 行为特征识别配置状态 */
);

/*******************************************************************************
*    Func Name: EgnApiSetStatInspectState
*      Purpose: 设置统计识别功能是否开启
*  Description: 设置统计识别功能是否开启
*        Input: EGN_BOOL bStatInspectSwitch:设置统计识别功能是否开启开关<EGN_FALSE: 关
                                                                       EGN_TRUE:开
                                                                       默认: 开>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006C01
*    Reference: EgnApiGetStatInspectState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSetStatInspectState
(
    IN EGN_BOOL  bStatInspectSwitch   /* 统计识别配置状态 */
);

/*******************************************************************************
*    Func Name: EgnApiGetStatInspectState
*      Purpose: 获取统计识别功能状态
*  Description: 获取统计识别功能状态
*        Input: NA
*        InOut: NA
*       Output: EGN_BOOL pbStatInspectSwitch:取得统计识别功能开启状态< EGN_FALSE: 关
                                                                     EGN_TRUE : 开 >
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006C01
*    Reference: EgnApiSetStatInspectState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetStatInspectState
(
    OUT EGN_BOOL  *pbStatInspectSwitch   /* 统计识别配置状态 */
);

/*******************************************************************************
*    Func Name: EgnApiMemBufStatGet
*      Purpose: 获取内存使用统计信息。
*  Description: 输入实例线程IDusThreadID、实例句柄pvHandle和数组pstStatRslt，获取该实例使用的内存统计信息并存储在pstStatRslt内。
*        Input: EGN_UINT16 usThreadID:多实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*        InOut: EgnBufStatInfo* pstStatRslt:输入的参数为大小为10的数组指针，输出统计数据<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: API内部不会动态申请内存，pstStatRslt需要外部产品申请内存传入。
*        Since: V200R002C02
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiMemBufStatGet
(
    IN    EGN_UINT16      usThreadID,
    IN    EGN_VOID       *pvHandle,
    INOUT EgnBufStatInfo *pstStatRslt
);

/*******************************************************************************
*    Func Name: EgnApiGetProtoImportState
*      Purpose: 查询协议是否导入的信息。
*  Description: 查询协议是否导入的信息，当ulProtoID不为0时，查询单个协议的信息。
                当ulProtoID为0时，查询所有已配置的协议导入信息。
*        Input: EGN_UINT32 ulProtoID:查询的协议ID<需要查EgnTxt.txt>
*        InOut: EgnProtoImportCfgInfo* pstProtoInfo:协议导入配置信息数组<非空>
*               EGN_UINT32* pulItemNum:输入为配置信息数组个数，输出为配置项数量<非空>
*       Output: NA
*       Return: EGN_UINT32，若数组大小不够，返回错误码为EGN_RET_ERR_BUF_MORE。其余请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V200R001C03
*    Reference: EgnApiSetProtoImportState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetProtoImportState
(
    IN    EGN_UINT32              ulProtoID,
    INOUT EgnProtoImportCfgInfo  *pstProtoInfo,
    INOUT EGN_UINT32             *pulItemNum
);

/*******************************************************************************
*    Func Name: EgnApiSetProtoImportState
*      Purpose: 设置一组协议是否导入。
*  Description: 设置一组协议是否导入的配置信息。失败时回填所有设置失败的协议和失败的个数。
                    设置后需要重新加载知识库才能使设置内容生效。
*        Input: EGN_UINT32 ulProtoNum:协议配置信息的个数<大于零>
*               EGN_BOOL bIsEnable:协议是否导入<EGN_FALSE/EGN_TRUE>
*        InOut: EGN_UINT32* pulProtoIdList:协议列表。失败时回填失败的协议<非空>
*       Output: EGN_UINT32* pulFailNum:设置失败的数目<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 不支持设置自定义协议
*        Since: V200R001C03
*    Reference: EgnApiGetProtoImportState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSetProtoImportState
(
    IN    EGN_UINT32   ulProtoNum,
    IN    EGN_BOOL     bIsEnable,
    INOUT EGN_UINT32  *pulProtoIdList,
    OUT   EGN_UINT32  *pulFailNum
);

/*******************************************************************************
*    Func Name: EgnApiGetProtoDetectState
*      Purpose: 查询一个协议的使能状态。
*  Description: 查询一个协议的使能状态。(只能查询系统规则协议)
*        Input: EGN_UINT32 ulProtoID:需查询的协议ID<大于零>
*        InOut: 无
*       Output: EGN_BOOL* pbIsEnable:使能状态<EGN_FALSE/EGN_TRUE>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006C00SPC200
*    Reference: EgnApiSetProtoDetectState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetProtoDetectState
(
    IN  EGN_UINT32   ulProtoID,
    OUT EGN_BOOL    *pbIsEnable
);

/*******************************************************************************
*    Func Name: EgnApiGetAllDisableDetectProto
*      Purpose: 查询所有被设置为非使能的协议。
*  Description: 查询所有被设置为非使能的协议。
*        Input: 无
*        InOut: EGN_UINT32* pulItemNum:输入为数组长度，输出为非使能的协议数量<非空>
*       Output: EGN_UINT32* pulProtoID:协议ID数组<非空>
*       Return: EGN_UINT32，若数组大小不够，返回错误码为EGN_RET_ERR_BUF_MORE。其余请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006C00
*    Reference: EgnApiSetProtoDetectState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetAllDisableDetectProto
(
    INOUT   EGN_UINT32      *pulItemNum,
    OUT     EGN_UINT32      *pulProtoID
);

/*******************************************************************************
*    Func Name: EgnApiSetProtoDetectState
*      Purpose: 设置一组协议是使能。
*  Description: 设置一组协议是否使能的配置信息。从传入的协议ID数组的第一个开
*                 始，遇到第一个设置失败则立即返回，并输出设置成功的数量。
*                 如果调用此接口时尚未加载知识库，则所有设置都只是暂时保留，
*                 等到加载完知识库以后，只有知识库中存在的协议ID的设置才会生效。
                不支持自定义协议的设置，对于会话关联规则,如果当前流正在识别中，
                   此时设置协议为非使能，使能状态将在下条流的识别中生效。
*        Input: EGN_UINT32* pulProtoID:协议ID数组首地址
*               EGN_UINT32  ulProtoNum:协议ID数组个数个数<大于零>
*               EGN_BOOL    bIsEnable:配置目标状态<EGN_FALSE/EGN_TRUE>
*       Output: EGN_UINT32* pulSuccessNum:设置成功的数目<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006C00SPC200
*    Reference: EgnApiGetProtoDetectState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSetProtoDetectState
(
    IN   EGN_CONST  EGN_UINT32 *pulProtoID,
    IN              EGN_UINT32  ulProtoNum,
    IN              EGN_BOOL    bIsEnable,
    OUT             EGN_UINT32 *pulSuccessNum
);

/*******************************************************************************
*    Func Name: EgnApiGetRuleImportState
*      Purpose: 查询规则是否导入的信息。
*  Description: 根据ulRuleID查询单个规则是否导入的信息，并将查询结果存储在pstRuleInfo中。
*        Input: EGN_UINT32 ulRuleID:查询的规则ID，只能是8155(BT协议的一个规则)<8155>
*        InOut: EgnRuleImportCfgInfo* pstRuleInfo:规则导入配置信息数组<非空>
*               EGN_UINT32* pulItemNum:输入为配置信息数组个数，输出为配置项数量<非空>
*       Output: NA
*       Return: EGN_UINT32，若数组大小不够，返回错误码为EGN_RET_ERR_BUF_MORE。其余请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R005C03
*    Reference: EgnApiSetRuleImportState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetRuleImportState
(
    IN     EGN_UINT32                ulRuleID,
    INOUT  EgnRuleImportCfgInfo     *pstRuleInfo,
    INOUT  EGN_UINT32               *pulItemNum
);

/*******************************************************************************
*    Func Name: EgnApiSetRuleImportState
*      Purpose: 设置一组规则是否导入。
*  Description: 设置一组规则是否导入的配置信息，失败时回填所有设置失败的规则和失败的个数。
                    设置后需要重新加载知识库才能使设置内容生效。
*        Input: EGN_UINT32 ulRuleNum:规则配置信息的个数，只能是1<1>
*               EGN_BOOL bIsEnable:规则是否导入<EGN_FALSE：否
                                               EGN_TRUE：否>
*        InOut: EGN_UINT32* pulRuleIdList:规则列表，里面的RuleId只能是8155。失败时回填失败的规则<非空>
*       Output: EGN_UINT32* pulFailNum:设置失败的数目<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R005C03
*    Reference: EgnApiGetRuleImportState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSetRuleImportState
(
    IN     EGN_UINT32   ulRuleNum,
    IN     EGN_BOOL     bIsEnable,
    INOUT  EGN_UINT32  *pulRuleIdList,
    OUT    EGN_UINT32  *pulFailNum
);

/*******************************************************************************
*    Func Name: EgnApiSetProtoRelParseState
*      Purpose: 设置一组协议关联解析开关
*  Description: 1、内部配置表只保存配置为关联解析用户配置为关的协议信息，
                   所以通过EgnApiGetProtoRelParseState只能查到用户配置的配置为关的协议。
                2、配置关联解析的协议id必须是存在当前保存的配置表或是在知识库中的协议id，如果传入其他协议id，返回失败。
*        Input: EGN_UINT32 ulProtoNum:协议配置信息的个数<大于零>
*               EGN_BOOL bIsEnable:协议是否关联解析<EGN_FALSE：否
                                                   EGN_TRUE：是>
*        InOut: EGN_UINT32* pulProtoIdList:输入:用户要配置的协议列表。输出:失败时回填失败的协议列表<非空>
*       Output: EGN_UINT32* pulFailNum:设置失败的数目<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 不支持设置自定义协议
*        Since: V300R005C02
*    Reference: EgnApiGetProtoRelParseState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSetProtoRelParseState
(
    IN    EGN_UINT32   ulProtoNum,
    IN    EGN_BOOL     bIsEnable,
    INOUT EGN_UINT32  *pulProtoIdList,
    OUT   EGN_UINT32  *pulFailNum
);

/*******************************************************************************
*    Func Name: EgnApiGetProtoRelParseState
*      Purpose: 查询协议不关联解析的信息。当ulProtoID不为0时，查询单个协议不关联解析的信息。
*  Description: 1、若ulProtoID为0是，查询所有已配置的协议解析开的信息。
                2、通过EgnApiGetProtoRelParseState只能查到用户配置的配置为关联的协议。
                3、查询单个协议的关联解析开关时，只能查询到在用户配置表中的协议。如果查询不在用户配置的表中的协议，
                   将会返回失败，错误码显示该协议不在用户配置的表中，查询不到。
*        Input: EGN_UINT32 ulProtoID:查询的协议ID<0或查EgnTxt.txt>
*        InOut: EgnProtoRelParseCfgInfo* pstProtoInfo:协议关联解析配置信息数组<非空>
*               EGN_UINT32* pulItemNum:输入为配置信息数组个数，输出为配置项数量，当ulProtoID为0时，输入建议值256个<非空>
*       Output: NA
*       Return: EGN_UINT32，若数组大小不够，返回错误码为EGN_RET_ERR_BUF_MORE。其余请参见EGN_RET_RESULT_EN。
*      Caution: 不支持查询自定义协议
*        Since: V300R005C02
*    Reference: EgnApiSetProtoRelParseState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetProtoRelParseState
(
    IN      EGN_UINT32               ulProtoID,
    INOUT   EgnProtoRelParseCfgInfo *pstProtoInfo,
    INOUT   EGN_UINT32              *pulItemNum
);

/*******************************************************************************
*    Func Name: EgnApiSetProtoHierInspectState
*      Purpose: 设置一组协议的层级非首包识别开关的状态
*  Description: 根据输入的协议列表pstProtoIdList设置协议的层级识别开关，并将这些协议的层级识别阀值状态记录在EGN内部层级识别配置表中。
                出于节约内存的考虑，EGN不记录所有协议的层级识别阀值状态，而会根据层级识别命中非同包识别功能的总开关(EgnInitCfgParam结构体中的bHierInspectSwitch)的状态
                和输入的usHierMaxDetCnt值，选择性的记录输入协议列表的层级识别状态。
                下面两种情况下，EGN不将协议的层级识别信息记录在内部层级识别配置表中：
                1、当bHierInspectSwitch为EGN_FALSE时，且设置协议的层级识别阀值为0(即关闭层级识别命中非同包识别功能)时，
                   内部配置表不保存该协议配置，所以使用EgnApiGetProtoHierInspectState查不到该协议配置信息。
                2、当bHierInspectSwitch为EGN_TRUE时，且设置协议的层级识别阀值为0XFFFF(即打开层级识别，并使用知识库默认阀值)，
                   内部配置表不保存该协议配置，所以使用EgnApiGetProtoHierInspectState查不到该协议配置信息。
*        Input: EGN_UINT32 ulProtoNum:协议配置信息的个数<大于零>
*        InOut: EgnHierMaxDetCnt* pstProtoIdList:输入:用户要配置的协议列表。输出:失败时回填失败的协议列表<非空>
*       Output: EGN_UINT32* pulFailNum:设置失败的数目<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 不支持设置自定义协议
*        Since: V300R006
*    Reference: EgnApiGetProtoHierInspectState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSetProtoHierInspectState
(
    IN    EGN_UINT32             ulProtoNum,
    INOUT EgnHierMaxDetCnt      *pstProtoIdList,
    OUT   EGN_UINT32            *pulFailNum
);

/*******************************************************************************
*    Func Name: EgnApiCleanUpProtoHierInspectState
*      Purpose: 清除内部层级识别配置表中非层级识别协议的配置项
*  Description: 由于在知识库升级回退过程中可能出现协议删除或修改，而不会删除配置表中的对应配置项，
                  以备知识库回退后保持原有的设置。
                当该类协议数量较多时，可能导致配置表满，可以通过EgnApiCleanUpProtoHierInspectState接口
                  清除掉内部层级识别配置表中非层级识别协议的配置项。
                  但使用该接口后，无法恢复，即知识库回退之后不会保留这些配置项。
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: NA
*        Since: V300R006C00SPC200
*    Reference: NA
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiCleanUpProtoHierInspectState
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiSetNotFirstPKTRelInspectSwitch
*      Purpose: 设置是否支持非首包识别功能
*  Description: 设置是否支持非首包识别功能,必须初始化成功后，才能正常设置。
*        Input: bIsNotFirstPktRelInspect:EGN_BOOL。是否支持非首包识别<EGN_TRUE:表示支持
                                                                      EGN_FALSE:表示不支持
                                                                      默认：不支持>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006C00
*    Reference: EgnApiGetNotFirstPKTRelInspectSwitch
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSetNotFirstPKTRelInspectSwitch
(
    IN  EGN_BOOL    bIsNotFirstPKTRelInspect
);

/*******************************************************************************
*    Func Name: EgnApiGetNotFirstPKTRelInspectSwitch
*      Purpose: 查询是否支持非首包识别功能
*  Description: 查询是否支持非首包识别功能,必须初始化成功后，才能正常查询。
*        Input: NA
*        InOut: NA
*       Output: EGN_BOOL* pbIsNotFirstPktRelInspect:是否支持非首包识别<非空>
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution: 无
*        Since: V100R001C01
*    Reference: EgnApiSetNotFirstPKTRelInspectSwitch
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetNotFirstPKTRelInspectSwitch
(
    OUT    EGN_BOOL    *pbIsNotFirstPKTRelInspect
);

/*******************************************************************************
*    Func Name: EgnApiGetProtoHierInspectState
*      Purpose: 查询协议层级识别阀值的信息。
*  Description: 1、若ulProtoID为0时，查询所有已配置的协议层级识别非同包识别开关的信息。
                2、当ulProtoID不为0时，查询单个协议，如果不是层级识别协议，返回失败，否则返回该协议层级识别状态。
                   如果查询的协议不在用户配置的层级识别协议表中，则返回查询失败。
                   如果输入的数组大小不够，将会返回失败，并只拷贝数组所能容纳的配置信息。
*        Input: EGN_UINT32 ulProtoID:查询的协议ID<大于零>
*        InOut: EGN_UINT32* pulItemNum:输入为配置信息数组个数，输出为配置项数量，当ulProtoID为0时，建议输入值128个<非空>
*       Output: EgnHierMaxDetCntInfo* pstProtoInfo:层级非同包识别开关状态配置信息数组<非空>
*       Return: EGN_UINT32，若数组大小不够，返回错误码为EGN_RET_ERR_BUF_MORE。其余请参见EGN_RET_RESULT_EN。
*      Caution: 不支持查询自定义协议
*        Since: V300R006C00
*    Reference: EgnApiSetProtoHierInspectState
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetProtoHierInspectState
(
    IN      EGN_UINT32               ulProtoID,
    INOUT   EGN_UINT32              *pulItemNum,
    OUT     EgnHierMaxDetCntInfo    *pstProtoInfo
);

/*******************************************************************************
*    Func Name: EgnApiSetUnsymmetricalInspectFlag
*      Purpose: 设置是否支持非对称识别功能
*  Description: 设置是否支持非对称识别功能，需要在初始化时注册EgnSspFunc.pfPeerSynAdd
                   （原型PFEgnFuncPeerSynAdd），且初始化成功后才能设置。
*        Input: EGN_BOOL bIsUnsymmetricalInspect:是否支持非对称识别<EGN_TRUE:支持
                                                                    EGN_FALSE:不支持
                                                                    默认：不支持>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006C01
*    Reference: EgnApiGetUnsymmetricalInspectFlag
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSetUnsymmetricalInspectFlag
(
    IN     EGN_BOOL     bIsUnsymmetricalInspect
);

/*******************************************************************************
*    Func Name: EgnApiGetUnsymmetricalInspectFlag
*      Purpose: 查询是否支持非对称识别功能
*  Description: 查询是否支持非对称识别功能，必须初始化成功后，才能正常启动该查询功能。
*        Input: NA
*        InOut: NA
*       Output: EGN_BOOL pbIsUnsymmetricalInspect:是否支持非对称识别<EGN_TRUE:支持
                                                                     EGN_FALSE:不支持>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006C01
*    Reference: EgnApiSetUnsymmetricalInspectFlag
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetUnsymmetricalInspectFlag
(
    OUT EGN_BOOL    *pbIsUnsymmetricalInspect
);

/*******************************************************************************
*    Func Name: EgnApiDynMemMngDestroy
*      Purpose: 管理线程去初始化接口（AR动态内存需求）。
*  Description: 完成管理进程、线程系统资源的释放等工作，
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 该API在使用EgnApiDynMemMngInit成功初始化EGN管理线程之前可被调用。
*        Since: V300R005C01
*    Reference: EgnApiDynMemMngInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiDynMemMngDestroy
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiDynMemInspectorDestroy
*      Purpose: 引擎识别实例去初始化接口（AR动态内存需求）。
*  Description: 完成业务进程、线程系统资源的释放等工作。
*        Input: EGN_UINT16 usThreadID:调用此接口的线程ID<0~65535>
*               EGN_VOID** ppvHandle:实例句柄<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 多实例时识别实例必须逐个的去初始化。
*        Since: V300R005C01
*    Reference: EgnApiDynMemInspectorInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiDynMemInspectorDestroy
(
    IN  EGN_UINT16  usThreadID,
    IN  EGN_VOID  **ppvHandle
);

/*******************************************************************************
*    Func Name: EgnApiDynMemMngInit
*      Purpose: EGN管理线程初始化接口（AR动态内存需求）。
*  Description: 完成EGN组件需要使用的系统资源的申请、初始化等工作。
                用户需要先调用EgnApiInitPubParam生成EGN的缺省配置，然后将EgnInitCfgParam中ulRelationCBCountIpv6设置为0，
                即IPv6关联识别CB块的数目为0；将ulMemPolicy设置为EGN_MEM_POLICY_LIMIT，即内存策略为受限；
                其余参数可根据自身需要设置。设置完毕后再调用EgnApiDynMemMngInit初始化。
*        Input: EgnInitCfgParam* pstParam:参数信息<非空>
*        InOut: EGN_VOID** ppvShareInfoAddr:保存globalspace的首地址，用于多线程之间共用同一块<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、若ulRelationCBCountIpv6不设置为0，ulMemPolicy不设置为EGN_MEM_POLICY_LIMIT，则初始化失败。
                2、除特殊说明外，其他API应在成功初始化EGN管理线程（成功调用该API）之后才能被使用。
*        Since: V300R005C01
*    Reference: EgnApiDynMemMngDestroy
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiDynMemMngInit
(
   IN  EGN_CONST EgnInitCfgParam     *pstParam,
   INOUT         EGN_VOID           **ppvShareInfoAddr
);

/*******************************************************************************
*    Func Name: EgnApiDynMemInspectorInit
*      Purpose: 初始化识别实例（AR动态内存需求）。
*  Description: 引擎识别实例初始化接口，在多进程、多线程模型下调用，完成业务进程、线程的初始化，
                多实例时识别实例必须逐个的初始化。
*        Input: EGN_UINT16 usThreadID:调用此接口的线程ID<0~65535>
*               EGN_VOID* pvShareInfoAddr:保存globalspace的首地址，用于多线程之间共用同一块<非空>
*        InOut: NA
*       Output: EGN_VOID** ppvHandle:实例句柄，初始化引擎为此实例创建的句柄，外部不能改变<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 除特殊说明外，其他API应在成功初始化EGN识别线程关联表共享时，成功调用该API之后才能被使用。
*        Since: V300R005C01
*    Reference: EgnApiDynMemInspectorDestroy
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiDynMemInspectorInit
(
    IN     EGN_UINT16         usThreadID,
    IN     EGN_VOID          *pvShareInfoAddr,
    OUT    EGN_VOID         **ppvHandle
);

/*******************************************************************************
*    Func Name: EgnApiDynMemMngEnable
*      Purpose: 使能EGN（AR动态内存需求）。
*  Description: 设置识别引擎为使能状态，当识别引擎处于去使能状态时，调用该接口将回复使能状态，
                重新导入知识库后将，重新启动识别功能。
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 该接口必须在管理线程上调用。
*        Since: V300R005C01
*    Reference: EgnApiDynMemMngDisable
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiDynMemMngEnable
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiDynMemMngDisable
*      Purpose: 去使能EGN（AR动态内存需求）。
*  Description: 设置识别引擎为去使能状态，此时识别引擎将释放知识库，关闭识别功能。
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 该接口必须在管理线程上调用。
*        Since: V300R005C01
*    Reference: EgnApiDynMemMngEnable
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiDynMemMngDisable
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiSetResAlarmThreshold
*      Purpose: 告警值设置接口。
*  Description: 管理进程初始化完成后，才能调用该接口，进行设置知识库、关联表、中间状态的上限告警值。
*        Input: EGN_UINT32 ulType:需要设置的告警值类型<EGN_RESALARMTHRESHOLD_EN>
*               EGN_UINT32 ulThreshold:上限告警值<大于零>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution:
*        Since: V300R005C01
*    Reference: EgnApiGetResAlarmThreshold
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSetResAlarmThreshold
(
    IN  EGN_UINT32  ulType,
    IN  EGN_UINT32  ulThreshold
);

/*******************************************************************************
*    Func Name: EgnApiGetResAlarmThreshold
*      Purpose: 告警值查询接口。
*  Description: 管理进程初始化完成后，才能调用该接口，进行获得知识库、关联表、中间状态的上限告警值。
*        Input: EGN_UINT32 ulType:需要查询的告警值类型<EGN_RESALARMTHRESHOLD_EN>
*        InOut: NA
*       Output: EGN_UINT32 *pulThreshold:上限告警值<非空>
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution:
*        Since: V300R005C01
*    Reference: EgnApiSetResAlarmThreshold
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetResAlarmThreshold
(
    IN  EGN_UINT32  ulType,
    OUT EGN_UINT32 *pulThreshold
);

/*******************************************************************************
*    Func Name: EgnApiGetStatErrInfo
*      Purpose: 获取错误统计信息。
*  Description: 获取错误统计信息，外部传入一块内存，egn将错误统计信息写入内存，外部通过打印函数显示错误统计信息。
                错误码请参考EGN_STAT_ERROR_EN。
*        Input: EGN_UINT32 ulStatBuffLen:传入的内存大小，建议5k<大于零>
*        InOut: NA
*       Output: EGN_UINT8* pucStatBuff:错误统计结果显示所需要的内存<非空>
*       Return: EGN_UINT32，成功或者失败的错误码。
*      Caution:
*        Since: V300R005C02
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32  EgnApiGetStatErrInfo
(
    OUT EGN_UINT8    *pucStatBuff,
    IN  EGN_UINT32    ulStatBuffLen
);

/*******************************************************************************
*    Func Name: EgnApiStatErrClear
*      Purpose: 清除错误统计信息
*  Description: 清除错误统计信息
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: 无
*      Caution: 无
*        Since: V300R005C02
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_VOID  EgnApiStatErrClear
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiGetSysInfoExt
*      Purpose: 收集EGN配置信息。
*  Description: EGN一键式信息收集接口，根据输入的实例线程IDusThreadID和实例句柄pvHandle，
                将该实例的配置信息保存在首地址为pucSysInfo的内存中
*        Input: EGN_UINT16 usThreadID:实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*               EGN_UINT32 ulSysInfoLen:保存配置信息内存总长度<大于零>
*        InOut: NA
*       Output: EGN_UCHAR* pucSysInfo:保存配置信息内存首地址，建议传入12k内存<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、在调用该接口之前要EGN组件初始化完毕。
                2、当API在识别线程被调用时，必须输入调用线程的识别实例句柄，如果输入为空则多线程可靠性校验会失效。
                3、当API在管理线程被调用时，识别实例句柄传入必须为空，此时不会进行多线程可靠性校验。
                4、EGN在去使能状态下，通过EgnApiGetSysInfoExt获取信息，不显示CB块和知识库的信息。
                5、外部通过打印函数显示egn的配置信息
*        Since: V300R005C02
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetSysInfoExt
(
    IN            EGN_UINT16   usThreadID,
    IN            EGN_VOID    *pvHandle,
    OUT           EGN_UCHAR   *pucSysInfo,
    IN            EGN_UINT32   ulSysInfoLen
);

/*******************************************************************************
*    Func Name: EgnApiGetHealthInfoExt
*      Purpose: 获取一键式检查组件健康信息。
*  Description: EGN一键式健康检查接口，完成EGN组件相关信息的健康检查，
                并将对应实例的健康信息存储在首地址为pucHealthInfo的内存中。
*        Input: EGN_UINT16 usThreadID:实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*               EGN_UINT32 ulHealthInfoLen:保存健康检查信息内存总长度<大于零>
*        InOut: NA
*       Output: EGN_UCHAR* pucHealthInfo:保存健康检查信息内存首地址<非>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、当API在识别线程被调用时，必须输入调用线程的识别实例句柄，如果输入为空则多线程可靠性校验会失效。
                2、当API在管理线程被调用时，识别实例句柄传入必须为空，此时不会进行多线程可靠性校验。
                3、在动态内存模式下不能调用该接口，否则会导致识别业务异常。
*        Since: V300R005C02
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetHealthInfoExt
(
    IN            EGN_UINT16   usThreadID,
    IN            EGN_VOID    *pvHandle,
    OUT           EGN_UCHAR   *pucHealthInfo,
    IN            EGN_UINT32   ulHealthInfoLen
);

/*******************************************************************************
*    Func Name: EgnApiSetAttrTypeCfgInfo
*      Purpose: 设置属性类型是否识别。
*  Description: 设置属性类型开关，每次设置一个属性类型，当外部需要关注新的属性类型情况时，
                    可以通过该接口进行设置。设置后需要重新加载知识库才能使设置内容生效。
*        Input: EGN_UINT32 ulAttrTypeId:属性类型id<0~63>
*               EGN_BOOL bIsEnable:是否识别<EGN_TRUE：识别
                                            EGN_FALSE：不识别>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution:
*        Since: V300R006C00
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSetAttrTypeCfgInfo
(
    IN    EGN_UINT32   ulAttrTypeId,
    IN    EGN_BOOL     bIsEnable
);

/*******************************************************************************
*    Func Name: EgnApiGetAttrTypeCfgInfo
*      Purpose: 查询属性类型配置信息。
*  Description: 查询属性类型是否配置了识别。
*        Input: EGN_UINT32 ulAttrTypeId:属性类型id<0~63>
*        InOut: NA
*       Output: EGN_BOOL* pbIsEnable:是否识别<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution:
*        Since: V300R006C00
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetAttrTypeCfgInfo
(
    IN    EGN_UINT32   ulAttrTypeID,
    OUT   EGN_BOOL    *pbIsEnable
);

/*******************************************************************************
*    Func Name: EgnApiPeerListAddPeer
*      Purpose: 添加关联表项
*  Description: 添加关联表项接口，并且只有同步操作时可以调用，
                其他场景调用不保证关联识别的正确性或者直接添加失败。
*        Input: usThreadID usThreadID:进程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*               EgnPeerTuple* pstPeerTuple:待添加的关联表项信息<非空>
*               EgnPeerTime* pstPeerTime:该表项对应的老化时间<非空>
*               EGN_UINT32 ulProtocol:对应协议ID<查EgnTxt.txt>
*               EGN_VOID* pvFlowHandle:数据包所属的流句柄<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006C00
*    Reference: EgnApiPeerListDelPeer
                EgnApiPeerListSearchPeer
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiPeerListAddPeer
(
    IN  EGN_UINT16       usThreadID,
    IN  EGN_VOID        *pvHandle,
    IN  EgnPeerTuple    *pstPeerTuple,
    IN  EgnPeerTime     *pstPeerTime,
    IN  EGN_UINT32       ulProtocol,
    IN  EGN_VOID        *pvFlowHandle
);

/*******************************************************************************
*    Func Name: EgnApiPeerListSearchPeer
*      Purpose: 查询关联表项
*  Description: 查询关联表项接口，并且只有同步操作时可以调用，其他场景调用不保证关联识别的正确性。
*        Input: usThreadID usThreadID:进程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*               EgnPeerTuple* pstPeerTuple:待添加的关联表项信息<非空>
*        InOut: NA
*       Output: EgnRelResult* pstRelResult:查询结果<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006C00
*    Reference: EgnApiPeerListAddPeer
                EgnApiPeerListDelPeer
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiPeerListSearchPeer
(
    IN  EGN_UINT16        usThreadID,
    IN   EGN_VOID        *pvHandle,
    IN   EgnPeerTuple    *pstPeerTuple,
    OUT  EgnRelResult    *pstRelResult
);

/*******************************************************************************
*    Func Name: EgnApiPeerListDelPeer
*      Purpose: 删除关联表项
*  Description: 删除关联表项接口，并且只有同步操作时可以调用，其他场景调用不保证关联识别的正确性。
*        Input: usThreadID usThreadID:进程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*               EgnPeerTuple* pstPeerTuple:待添加的关联表项信息<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006C00
*    Reference: EgnApiPeerListAddPeer
                EgnApiPeerListSearchPeer
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiPeerListDelPeer
(
    IN  EGN_UINT16        usThreadID,
    IN   EGN_VOID        *pvHandle,
    IN   EgnPeerTuple    *pstPeerTuple
);

/*******************************************************************************
*    Func Name: EgnApiGetRuleStatInfo
*      Purpose: 查询规则统计信息。
*  Description: 1、获取到的不同的记录可能有相同的规则，累加后为实际规则统计信息。
                2、规则有父子关系的，会多份统计信息，主要看父子关系的个数。
                3、ulStatRecordNum个数为规则实际个数+父子关系的个数。
*        Input: EGN_UINT32 ulStatRecordNum:规则实际个数+父子关系的个数<大于零>
*        InOut: EgnRuleStatRd* pstRuleStatRd:保存查询记录的内存<非空>
*       Output: EGN_UINT32* pulRuleNum:实际获得的统计记录数目<非空>
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
EGN_UINT32 EgnApiGetRuleStatInfo
(
    INOUT EgnRuleStatRd   *pstRuleStatRd,
    IN    EGN_UINT32       ulStatRecordNum,
    OUT   EGN_UINT32      *pulRuleNum
);

/*******************************************************************************
*    Func Name: EgnApiClearRuleStatInfo
*      Purpose: 清除规则统计信息。
*  Description: 清除规则统计信息。
*        Input: NA
*        InOut: NA
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
EGN_UINT32 EgnApiClearRuleStatInfo
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiGetMatchedRule
*      Purpose: 获取当前报文匹配到的规则id
*  Description: 获取当前报文匹配到的规则id
*        Input: EGN_UINT32   ulThreadId:线程ID<0~65535>
                EGN_VOID pvHandle:实例句柄<非空>
*        InOut: NA
*       Output: EGN_UINT32* pulRuleId:规则id<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006C00
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetMatchedRule
(
    IN   EGN_UINT32   ulThreadId,
    IN   EGN_VOID    *pvHandle,
    OUT  EGN_UINT32  *pulRuleId
);

/*******************************************************************************
*    Func Name: EgnApiGetMaintainInfo
*      Purpose: 归一化查询接口
*  Description: 获取EGN维护配置信息
*        Input: EGN_UINT32 ulInfoMask:查询掩码<EGN_DBG_QUERY_ENGINE_VERSION       0x01    查询引擎版本号
                                              EGN_DBG_QUERY_DETECT_THRESHOLD      0x02    查询识别阀值
                                              EGN_DBG_QUERY_RULELIB_VERSION       0x04    查询知识库版本号
                                              EGN_DBG_QUERY_PEER_INFO             0x08    查询peer信息
                                              EGN_DBG_QUERY_RELTBL_INFO           0x10    查询关联表状态
                                              EGN_DBG_QUERY_PKT_STAT_INFO         0x20    查询报文统计信息
                                              EGN_DBG_QUERY_STATICBP_INFO         0x40    查询静态内存分配信息
                                              EGN_DBG_QUERY_ALL_INFO              0x7f    查询以上所有信息 >
*               EGN_UINT16 usThreadID:实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例handle<非空>
*               EGN_UINT32 ulBufLen:外部传入内存的大小<大于零>
*        InOut: EGN_UINT32* pucBuffer:外部传入的内存首地址<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、在调用该接口之前要EGN组件初始化完毕
                2、当API在识别线程被调用时，必须输入调用线程的识别实例句柄和对应的线程id。
                3、当API在管理线程被调用时，识别实例句柄传入必须为空。
                4、外部通过打印函数显示egn的配置信息。
*        Since: V300R006C00
*    Reference: 无。
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetMaintainInfo
(
    IN    EGN_UINT32   ulInfoMask,
    IN    EGN_UINT16   usThreadID,
    IN    EGN_VOID    *pvHandle,
    INOUT EGN_UCHAR   *pucBuffer,
    IN    EGN_UINT32   ulBufLen
);

/*******************************************************************************
*    Func Name: EgnApiGetMngInfo
*      Purpose: 组件接口使用风格统一，管理面信息查询接口
*  Description: 1.在调用该接口之前要EGN组件初始化完毕
*               2.外部可根据获取信息的类型明确获取信息的数据结构，具体的数据结构在结构体EgnGetCfgInfo中,
*                该结构体中的共用体对应各种数据结构，数据结构中统一命名方式:入参，含有In;入出参，含有InOut;出参，含有Out。对于指针类型的变量需用户根据其传入的个数分配内存。
*                (具体类型与返回结果的对应关系见EGN_UNITE_GET_SET_EN)
*        Input: EGN_UINT32      ulInfoEnum:查询类型<EGN_UNITE_GET_SET_EN
*                 (ulInfoEnum的范围:"EGN_UNITE_GET_SET_ENGINE_VERSION~EGN_UNITE_GET_SET_UD_BACK_RULE_LIB_NEED_MEM"，除去EGN_UNITE_GET_SET_MATCHED_RULE_INFO。
*                 其中EGN_UNITE_GET_SET_STATIC_BLOCK_INFO、EGN_UNITE_GET_SET_CB_STATIC_INFO类型的信息只有内存为共享模式时，才能通过该接口获取）>
*        InOut: EgnGetCfgInfo    pstGetCfgInfo:获取到的信息首地址<非空>。当查询健康信息时，若内存异常信息超过两条，由于长度限制，仅返回前两条内存记录信息。若要查询更多信息，请使用
*                 EgnApiGetHealthInfo获取。
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
EGN_UINT32 EgnApiGetMngInfo
(
    IN     EGN_UINT32       ulInfoEnum,
    INOUT  EgnGetCfgInfo   *pstGetCfgInfo
);

/*******************************************************************************
*    Func Name: EgnApiGetInspectorInfo
*      Purpose: 组件接口使用风格统一，业务面信息查询接口
*  Description: 1.在调用该接口之前要EGN组件初始化完毕
*               2.外部可根据获取信息的类型明确获取信息的数据结构，具体的数据结构在结构体EgnGetCfgInfo中,
*                该结构体中的共用体对应各种数据结构，数据结构中统一命名方式:入参，含有In;入出参，含有InOut;出参，含有Out。对于指针类型的变量需用户根据其传入的个数分配内存。
*                (具体类型与返回结果的对应关系见EGN_UNITE_GET_SET_EN)
*               3.该API不能在管理线程调用，只能在识别线程调用
*        Input: EGN_UINT32  ulInfoEnum:查询类型<EGN_UNITE_GET_SET_EN
*                  (ulInfoEnum的范围:"EGN_UNITE_GET_SET_STATIC_BLOCK_INFO~EGN_UNITE_GET_SET_MATCHED_RULE_INFO）>
*               EGN_UINT16  usThreadID:实例线程ID<0~65535>
*               EGN_VOID    pvHandle:实例handle<非空>
*        InOut: EgnGetCfgInfo  pstGetCfgInfo:获取到的信息首地址<非空>
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
EGN_UINT32 EgnApiGetInspectorInfo
(
    IN        EGN_UINT32      ulInfoEnum,
    IN        EGN_UINT16      usThreadID,
    IN        EGN_VOID       *pvHandle,
    INOUT     EgnGetCfgInfo  *pstGetCfgInfo
);

/*******************************************************************************
*    Func Name: EgnApiSetCfgParamInfo
*      Purpose: 组件接口使用风格统一，统一设置配置信息，分为静态设置与动态设置
*  Description: 1.静态设置:调用时机是在初始化之前，且注册了回调函数之后，否则返回失败。
*                 如果在初始化默认参数之后调用，即在调用EgnApiInitPubParam之后调用该接口，设置的配置信息无效，但返回成功。
*                 静态设置的参数成功且设置有效，该接口的调用时机是在初始化默认参数之前，注册了回调函数之后，且系统未初始化
*               2.动态设置:EGN组件初始化完毕,否则返回失败
*               3.外部可根据设置信息的类型明确设置信息的数据结构，具体的数据结构在结构体EgnSetCfgParamInfo中,
*                该结构体中的共用体对应各种数据结构，数据结构中统一命名方式:入参，含有In;入出参，含有InOut;出参，含有Out。对于指针类型的变量需用户根据其传入的个数分配内存。
*                (具体类型与返回结果的对应关系见EGN_UNITE_GET_SET_EN)。
*        Input: EGN_UINT32      ulSetType:设置的类型<0:静态，1:动态>
*               EGN_UINT32      ulEnumNum:设置类型<EGN_UNITE_GET_SET_EN
                     静态设置:"EGN_UNITE_GET_SET_DETECT_THRESHOLD~EGN_UNITE_GET_SET_UDRULE_SWITCH_INFO",
                              "EGN_UNITE_GET_SET_CFG_PARA_IS_SHARE~EGN_UNITE_GET_SET_UD_BACK_RULE_LIB_NEED_MEM"
                     动态设置:"EGN_UNITE_GET_SET_DETECT_THRESHOLD~EGN_UNITE_GET_SET_IP_FLOW_AGED_TIME",
                              "EGN_UNITE_GET_SET_PROTO_IMPORT_STATE~EGN_UNITE_GET_SET_STAT_CONTROL" >
*        InOut: EgnSetCfgParamInfo    pstSetCfgParamInfo:待设置的信息<非空>
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
EGN_UINT32 EgnApiSetCfgParamInfo
(
    IN      EGN_UINT32              ulSetType,
    IN      EGN_UINT32              ulEnumNum,
    INOUT   EgnSetCfgParamInfo     *pstSetCfgParamInfo
);

/*******************************************************************************
*    Func Name: EgnApiSnDnsBlockInspect
*      Purpose: 将完成DNS请求包的识别。
*  Description: 用户根据EgnDnsRequestResult 中的协议ID是否为0，判断EGN是否识别出来这个DNS应用协议。
*        Input: EGN_UINT16 usThreadID:实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*               EgnDnsPacketInfo* pstDnsInfo:DNS报文信息<非空>
*        InOut: EgnDnsRequestResult* pstInspectResult:识别结果<非空>
*        Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution:  该API在成功初始化EGN及session组件之前或者使用动态申请内存方式的产品不可被调用，并且只有session模式才能使用该接口。
*        Since: V3R6C00SPC200
*    Reference: EgnApiSnInspectorInit
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnDnsBlockInspect
(
    IN                  EGN_UINT16               usThreadID,        /* 识别进程ID */
    IN      EGN_CONST   EGN_VOID                *pvHandle,          /* 实例句柄 */
    IN      EGN_CONST   EgnDnsPacketInfo      *pstDnsInfo,        /* 待识别的DNS协议信息 */
    INOUT               EgnDnsRequestResult     *pstInspectResult   /* 识别结果 */
);

/*******************************************************************************
*    Func Name: EgnApiSnDnsRelParse
*      Purpose: 解析DNS响应包。
*  Description: 解析DNS响应包，并提取IP信息，进行DNS关联识别。
                    根据pulAddIpNum的值，判断EGN解析并成功添加IP的个数。
*        Input: EGN_UINT16 usThreadID:实例线程ID<0~65535>
*               EGN_VOID* pvHandle:实例句柄<非空>
*               EgnDnsPacketInfo* pstDnsInfo:DNS报文信息<非空>
*        InOut: EGN_UINT32* pulAddIpNum:成功添加IP的个数<非空>
*        Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution:  该API在成功初始化EGN及SESSION组件之前或者使用动态申请内存方式的产品或非session模式下不可被调用。
*        Since: V3R6C00SPC200
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnDnsRelParse
(
    IN              EGN_UINT16           usThreadID,
    IN              EGN_VOID            *pvHandle,
    IN  EGN_CONST   EgnDnsPacketInfo  *pstDnsInfo,
    OUT             EGN_UINT32          *pulAddIpNum
);

/*******************************************************************************
*    Func Name: EgnApiSnDnsDelIpFromIpTbl
*      Purpose: 删除某IP表项。
*  Description: 根据用户输入的IP删除相应的IP表项。
*        Input: EGN_UINT16 usThreadID:线程ID
*               EGN_VOID  *pvHandle:实例句柄<非空>
*               EgnIpAddr* pstIpAddr:IP结构<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 该API在以下情况不可调用:
                1.成功初始化EGN及SESSION组件之前
*               2.使用动态申请内存方式的产品不可被调用。
*               3.DNS应用识别开关为关的状态
*               4.非session模式下
*        Since: V3R6C00SPC200
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnDnsDelIpFromIpTbl
(
    IN              EGN_UINT16       usThreadID,
    IN  EGN_CONST   EGN_VOID        *pvHandle,
    IN  EGN_CONST   EgnIpAddr       *pstIpAddr
);

/*******************************************************************************
*    Func Name: EgnApiSnDnsGetIpFromIpTbl
*      Purpose: 查询某IP表项是否存在。
*  Description: 根据用户输入的IP查询是否现在
*        Input: EGN_UINT16 usThreadID:线程ID
*               EGN_VOID *pvHandle:实例句柄<非空>
*               EgnIpAddr* pstIpAddr:IP结构<非空>
*        InOut: NA
*       Output: EGN_BOOL* pbIsExist:该表项是否存在<0:不存在
                                                   1:存在>
*               EGN_UINT32* pulProtoID:获取到的协议ID，0表示未获取
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 该API在以下情况不可调用:
                1.成功初始化EGN及SESSION组件之前
*               2.使用动态申请内存方式的产品不可被调用。
*               3.DNS应用识别开关为关的状态
*               4.非session模式下
*        Since: V3R6C00SPC200
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnDnsGetIpFromIpTbl
(
    IN                  EGN_UINT16       usThreadID,
    IN      EGN_CONST   EGN_VOID        *pvHandle,
    IN      EGN_CONST   EgnIpAddr       *pstIpAddr,
    OUT                 EGN_BOOL        *pbIsExist,
    OUT                 EGN_UINT32      *pulProtoID
);

/*******************************************************************************
*    Func Name: EgnApiSnDnsGetIpTblInfo
*      Purpose: 打印所有IP实例的ip表项添加次数与更新次数
*  Description: 用户可通过该接口获取所有实例IP表项添加及更新次数( 不包括第一次添加的 )
*        Input: EGN_UINT16  usThreadID:线程ID
*               EGN_VOID   *pvHandle:实例句柄<非空>
*               EGN_UINT32  ulIpTblInfoLen:保存IP信息的内存总长度
*        InOut: EGN_UCHAR  *pucIpTblInfo:保存IP信息的内存首地址，建议传入2K内存<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution:  该API在以下情况不可调用:1.成功初始化EGN及SESSION组件之前
*                                        2.使用动态申请内存方式的产品不可被调用。
*                                        3.DNS应用识别开关为关的状态
*                                        4.非session模式下
*        Since: V3R6C00SPC200
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiSnDnsGetIpTblInfo
(
    IN      EGN_UINT16       usThreadID,
    IN      EGN_VOID        *pvHandle,
    IN      EGN_UINT32       ulIpTblInfoLen,
    INOUT   EGN_UCHAR       *pucIpTblInfo
);

/*******************************************************************************
*    Func Name: EgnApiQueryBearedProtocols
*      Purpose: 查询承载协议
*  Description: 查询承载协议.导入知识库时，需要导入明文知识库.
*        Input: EGN_UINT32 ulParentProtoID:被查询的父协议ID<非零>
*        InOut: EGN_UINT32* pulProtoIdListLen:输入：数组的长度。输出：实际承载协议个数。<非空
                    如果输入的长度不足，则返回错误码EGN_RET_ERR_BUF_MORE，同时该出参输出所需的最小长度。>
                EGN_UINT32* pulProtoIdList:被承载的子协议ID列表数组(不包括父协议本身)<非空>
*       Output:
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 查询仅针对系统协议，不支持查询自定义协议
*        Since: V300R006C00SPC200
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiQueryBearedProtocols
(
    IN          EGN_UINT32      ulParentProtoID,
    INOUT       EGN_UINT32     *pulProtoIdList,
    INOUT       EGN_UINT32     *pulProtoIdListLen
);

/************************************************************************************************/
/* 以下为与硬件加速相关的API */

/*STRUCT<保存命中信息的结构体，若调用者在调用 Search 时送入该结构体，则模式匹配会填充之后返回> */
typedef struct _EgnHAMatchInfo
{
    EGN_UINT32     ulPatternID;             /* 模式串匹配命中时，唯一标志这条模式串的ID */
    EGN_INT32      lCurPacketOffset;        /* 匹配的模式串起始位置相对于当前包负载头的偏移 */
    EGN_UINT32     ulFlowOffset;            /* 匹配的模式串起始位置相对于流当前方向负载头的总偏移 */
    EGN_UINT16     usPatternLen;            /* 模式串长度 */
    EGN_UINT8      aucReserved[2];          /* 保留字节，64位对齐 */
} EgnHAMatchInfo;

/*STRUCT<硬件命中的模式串结构链表>*/
typedef struct _EgnHAMatchInfoList
{
    EGN_UINT16  usMatchInfoNum;             /* 命中模式串个数 */
    EGN_UINT8   ucIsFull;                   /* 是否匹配超额 */
    EGN_UINT8   aucReserved[5];             /* 保留字节，64位对齐 */
    EgnHAMatchInfo  astMatchInfo[EGN_HA_RETURN_PATT_MAX];  /* 硬件命中的模式串结构数组 */
} EgnHAMatchInfoList;

/*STRUCT<会话信息 >*/
typedef struct _EgnHASessionInfo
{
    EGN_VOID        *pvNgeSessionAddr;   /* NGE会话的地址 */
    EGN_UINT32       ulDpSessVersion;    /* 会话版本号 */
    EGN_UINT16       usVrfID;            /* 虚拟防火墙ID */
    EGN_UINT8        ucHAPatternSetID;   /* 硬件加速模式串组的ID，相当与原GPM_PATTSETKEY_ST结构中的uiMLable */
    EGN_UINT8        ucRuleLibID;        /* 当前会话使用的知识库ID，相当与原GPM_PATTSETKEY_ST结构中的uiSLable */
} EgnHASessionInfo;

/*STRUCT<模式串属性数据结构 >*/
typedef struct _EgnPatternAtt
{
    EGN_UINT8   ucNoCase;           /* 是否大小写敏感，仅对AC算法有效 */
    EGN_UINT8   ucType;             /* 是否为正则表达式，如果是正则表达式要做一下正则表达式的检查 0:普通字符串,1:正则表达式 */
    EGN_UINT8   aucReserved[6];     /* 保留字节，64位对齐 */
} EgnPatternAtt;

/*STRUCT<关联表信息 >*/
typedef struct _EgnNgfwRel
{
    EGN_UINT32  ulType;         /* 关联表的动作 */
    EGN_UINT32  ulDpSessVer;    /* 转发面会话版本号 */
    EgnPeerInfoSetItem  stPeerItem;
    EGN_VOID    *pvNgeSess;     /* NGE会话指针 */
} EgnNgfwRel;

/*STRUCT<组件给适配同步消息的结构，目前只有同步关联表这一种> */
typedef struct _EgnSynMsg
{
    EGN_UINT32  ulMsgType;          /* 消息类型 */
    EGN_UINT8   aucReserved[4];
    union
    {
        EgnNgfwRel  stNgfwRel;      /* 提供给NGFW的 */
    }u;
} EgnSynMsg;

/*******************************************************************************
*    Func Name: EgnApiNgfwHASelectHardwareHandle
*      Purpose: 选择硬件加速的状态机
*  Description: 硬件加速方案中硬件中会存在多个状态机，每次报文进入硬件匹配前，需要先
                  调此接口来选择对应的硬件匹配状态机。
                若EgnHASessionInfo结构中的ucHAPatternSetID返回EGN_HA_PATTERN_SET_TYPE_INVALID，
                  则当前报文不必再送硬件匹配，直接送软件匹配。
*        Input: EGN_CONST EgnPacket*    pstPacketInfo   :报文<非空>*
*        InOut: EgnHASessionInfo*       pstSessInfo     :会话信息<非空>
*               EGN_VOID**              ppvFlowInspect  :流识别句柄<非空>
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: NA
*        Since: V300R006C00SPC200
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwHASelectHardwareHandle
(
    IN EGN_CONST EgnPacket          *pstPacketInfo,
    INOUT        EgnHASessionInfo   *pstSessInfo,
    INOUT        EGN_VOID          **ppvFlowInspect
);

/*******************************************************************************
*    Func Name: EgnApiNgfwHAPacketInspect
*      Purpose: 硬件加速软件识别接口。
*  Description: 通过硬件加速将完成2～4层解析的报文传入进行协议识别。
*        Input: EGN_UINT16          usThreadID          :多实例线程ID<0~65535>
*               EGN_VOID*           pvMulInstanceHandle :实例句柄<非空>
*               EGN_VOID**          ppvFlowInspect      :流识别句柄<非空>
*               EgnPacket*          pstPacketInfo       :报文<非空>
*               EgnHAMatchInfoList* pstMatchList        :硬件匹配信息<非空>
*               EgnHASessionInfo*   pstSessInfo         :会话信息<非空>
*        InOut: NA
*       Output: EgnResult*          pstInspectResult    :识别结果<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 该API在成功初始化EGN识别线程成功调用EgnApiInspectorInit之前不可被调用。
*        Since: V300R006C00SPC200
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwHAPacketInspect
(
    IN           EGN_UINT16          usThreadID,
    IN           EGN_VOID           *pvMulInstanceHandle,
    IN           EGN_VOID          **ppvFlowInspect,
    IN EGN_CONST EgnPacket          *pstPacketInfo,
    IN           EgnHAMatchInfoList *pstMatchList,
    IN           EgnHASessionInfo   *pstSessInfo,
    OUT          EgnResult          *pstInspectResult
);

/*******************************************************************************
*    Func Name: EgnApiGetMemSizeOnlyLoadRuleLib
*      Purpose: 在只加载明文知识库的情况下，计算引擎所需内存。
*  Description: 该API只计算在只加载明文知识库的情况下，所需要的内存大小。
*                  此时计算出的内存是按流规模为1、关联表为0、实例个数为1的配置来计算
*        Input: NA
*        InOut: NA
*       Output: EGN_ULONG pulmemSize:所需内存大小
*       Return: EGN_UINT32，成功或失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、该API在使用EgnApiRegSspFunc成功注册系统服务回调函数之前可被调用。
*               2、该API是在用户设定了ulMemPolicy为EGN_MEM_POLICY_ONLY_LOAD_TXT_RULELIB的模式下调用
*               3、在动态内存模式下不能调用该接口，否则会导致识别业务异常。
*        Since: V3R6C02
*    Reference: EgnApiInitPubParam
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiGetMemSizeOnlyLoadRuleLib
(
    OUT  EGN_ULONG  *pulmemSize
);

/*******************************************************************************
*    Func Name: EgnApiGetProtoInfoRuleLibVersion
*      Purpose: 获取知识库版本
*  Description: 和EgnApiGetRuleLibVersion的函数区别在于，支持在不加载密文库的情况下
*                  查询出知识库版本号
*        Input: NA
*        InOut: EgnRuleLibVersion pstRuleLibVersion:知识库版本结构<非空>
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
EGN_UINT32 EgnApiGetProtoInfoRuleLibVersion
(
    INOUT EgnRuleLibVersion *pstRuleLibVersion
);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EGN_API_COMMON_H__ */

