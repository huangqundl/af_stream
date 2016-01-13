/*
 ******************************************************************************
 版权所有 (C), 2008-2009, 华为技术有限公司
 ******************************************************************************
  文 件 名   : egnapiuserdefinerule.h
  版 本 号   : 初稿
  作    者   : EGN项目组
  生成日期   : 2012年12月6日
  最近修改   :
  功能描述   : 公共宏定义
  函数列表   :
  修改历史   :
  1.日    期   : 2012年12月6日
    作    者   : EGN项目组
    修改内容   : 创建文件

******************************************************************************/
/**@file  egnapiuserdefinerule.h
  *    公共宏定义
*******************************************************/
/**
 * @defgroup egn  EGN自定义规则模块的API
 */

#ifndef __EGN_API_USER_DEFINE_RULE_H__
#define __EGN_API_USER_DEFINE_RULE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*MACRO<  自定义单包规则支持的最大数量 >*/
#define EGN_UD_SINGLE_PKT_RULE_MAX_NUM  1024

/*MACRO<  自定义行为规则支持的最大数量 >*/
#define EGN_UD_BEHA_RULE_MAX_NUM  64

/*MACRO<  自定义等差规则支持的最大数量 >*/
#define EGN_UD_ARITH_RULE_MAX_NUM  16

/*MACRO<  自定义HTTP承载规则支持的最大数量 >*/
#define EGN_UD_HTTP_BEAR_RULE_MAX_NUM  64

/*MACRO< 自定义规则名的最大长度 >*/
#define EGN_USER_RULE_NAME_MAX_LEN      31

/*MACRO< 自定义规则支持最多可以指定的字符串特征的个数 >*/
#define EGN_USER_STRING_PATTERN_MAX_NUM      6

/*MACRO< 自定义规则支持最多可以指定的pcre特征的个数 >*/
#define EGN_USER_PCRE_PATTERN_MAX_NUM      1

/*MACRO< 自定义规则支持的pcre特征的最大长度 >*/
#define EGN_USER_PCRE_PATTERN_MAX_LEN      63

/*MACRO< 支持的自定义规则的协议、应用、大类id最小值 >*/
#define EGN_UD_ID_MIN  0x10000000

/*MACRO< 支持的自定义规则的协议、应用、大类id最大值 >*/
#define EGN_UD_ID_MAX  0x1FFFFFFF

/*MACRO< 支持的自定义字符串条件的最大偏移值 >*/
#define EGN_UD_STR_CONDITION_OFFSET_MAX  63

/*MACRO< 自定义等差规则包含模式最大数量 >*/
#define EGN_USER_ARITH_PATTERN_MAX_NUM 2

/*MACRO< 自定义等差规则等差字段的最大长度 >*/
#define EGN_USER_ARITH_FIELD_MAX_LEN    4

/*MACRO< 自定义等差规则识别中间状态包含的匹配信息节点数 >*/
#define EGN_USER_ARITH_MATCH_NODE_NUM    32

/*MACRO< 自定义等差规则模式串的最大偏移 >*/
#define EGN_USER_ARITH_PATTERN_MAX_OFFSET 1500

/*MACRO< 支持的自定义字符串条件的最小长度 >*/
#define EGN_UD_STR_CONDITION_LEN_MIN  3

/*MACRO< 支持的自定义字符串条件的最大长度 >*/
#define EGN_UD_STR_CONDITION_LEN_MAX  31

/*MACRO< 自定义单包规则字符串条件缓存区的长度 >*/
#define EGN_UD_SINGLE_PKT_STR_BUF_LEN  63

/*MACRO< 自定义HTTP承载规则字符串条件缓存区的长度 >*/
#define EGN_UD_HTTP_BEAR_STR_BUF_LEN  80

/*ENUM< 自定义字符串条件的起始位置 >*/
typedef enum
{
    EGN_UD_STRING_BASE_BEGIN          = EGN_EN_INVALID,
    EGN_UD_STRING_BASE_LOAD_BEGIN     = 0,   /* 负载起始位置 */
    EGN_UD_STRING_BASE_LOAD_END       = 1,   /* 负载结束位置 */
    EGN_UD_STRING_BASE_HTTP_FIRST     = 2,   /* 首行位置,只有HTTP承载才会有效 */
    EGN_UD_STRING_BASE_HTTP_HEADER    = 3,   /* 头域位置,只有HTTP承载才会有效 */
    EGN_UD_STRING_BASE_LOAD_BODY      = 4,   /* 消息体位置,只有HTTP承载才会有效 */
    EGN_UD_STRING_BASE_END,

    EGN_UD_STRING_BASE_BOTTOM = EGN_EN_BUTT
}EGN_UD_STRING_BASE_EN;

/*ENUM< 自定义字符串条件的大小端 >*/
typedef enum
{
    EGN_UD_BYTE_ORDER_BEGIN    = EGN_EN_INVALID,
    EGN_UD_BYTE_ORDER_UNKNOWN  = 0,   /* 未知 */
    EGN_UD_BYTE_ORDER_BIG      = 1,   /* 大端 */
    EGN_UD_BYTE_ORDER_LITTLE   = 2,   /* 小端 */
    EGN_UD_BYTE_ORDER_END,

    EGN_UD_BYTE_ORDER_BOTTOM = EGN_EN_BUTT
}EGN_UD_BYTE_ORDER_EN;

/*ENUM< 自定义字符串格式 >*/
typedef enum
{
    EGN_UD_CONTENT_TYPE_BEGIN   = EGN_EN_INVALID,
    EGN_UD_CONTENT_TYPE_STR     = 0,              /* 普通字符串，大小写不敏感  */
    EGN_UD_CONTENT_TYPE_HEX     = 1,              /* 十六进制字符串，大小写敏感 */
    EGN_UD_CONTENT_TYPE_END,

    EGN_UD_CONTENT_TYPE_BOTTOM  = EGN_EN_BUTT
}EGN_UD_CONTENT_TYPE_EN;

/*ENUM< 自定义包深度条件中的类型 >*/
typedef enum
{
    EGN_UD_DEPTH_TYPE_BEGIN          = EGN_EN_INVALID,
    EGN_UD_DEPTH_TYPE_RANGE          = 0,   /* 范围 */
    EGN_UD_DEPTH_TYPE_DISTANCE       = 1,   /* 确定的隔包 */
    EGN_UD_DEPTH_TYPE_END,

    EGN_UD_DEPTH_TYPE_BOTTOM = EGN_EN_BUTT
}EGN_UD_DEPTH_TYPE_EN;

/*ENUM< 自定义包深度条件中的类型 >*/
typedef enum
{
    EGN_UD_BEAR_SAME_PKT_BEGIN     = EGN_EN_INVALID,
    EGN_UD_BEAR_SAME_PKT_SAME      = 0,    /* 和承载协议在同一个包，取此值时，深度值ucBearDepth必须为0 */
    EGN_UD_BEAR_SAME_PKT_NOTSAME   = 1,    /* 和承载协议在不同的包，取此值时，深度值ucBearDepth必须不为0 */
    EGN_UD_BEAR_SAME_PKT_ALL       = 2,    /* 和承载协议可能在同一个包，也可能在不同的包，，取此值时，深度值ucBearDepth必须不为0 */
    EGN_UD_BEAR_SAME_PKT_END,

    EGN_UD_BEAR_SAME_PKT_BOTTOM    = EGN_EN_BUTT
}EGN_UD_BEAR_SAME_PKT_EN;

/*ENUM< 自定义规则五元组信息中的IP版本号 >*/
typedef enum
{
    EGN_UD_IP_VERSION_BEGIN         = EGN_EN_INVALID,
    EGN_UD_IP_VERSION_V4            = 0,   /* IPV4 */
    EGN_UD_IP_VERSION_V6            = 1,   /* IPV6 */
    EGN_UD_IP_VERSION_ANY           = 2,   /* any */
    EGN_UD_IP_VERSION_END,

    EGN_UD_IP_VERSION_BOTTOM = EGN_EN_BUTT
}EGN_UD_IP_VERSION_EN;

/*ENUM< 自定义规则类型 >*/
typedef enum
{
    EGN_UD_RULE_TYPE_BEGIN          = EGN_EN_INVALID,
    EGN_UD_RULE_TYPE_SINGLE_PACKET  = 0,               /* 自定义单包规则 */
    EGN_UD_RULE_TYPE_BEHA           = 1,               /* 自定义行为统计规则 */
    EGN_UD_RULE_TYPE_ARITH          = 2,               /* 自定义等差规则 */
    EGN_UD_RULE_TYPE_HTTP_BEAR      = 3,               /* 自定义承载规则 */
    EGN_UD_RULE_TYPE_ALL            = 4,               /* 所有自定义规则 */
    EGN_UD_RULE_TYPE_END,

    EGN_UD_RULE_TYPE_BOTTOM            = EGN_EN_BUTT
}EGN_UD_RULE_TYPE_EN;

/*STRUCT< 自定义规则承载信息 >*/
typedef struct _EgnUDRuleBearInfo
{
    EGN_UINT32  ulBearID;           /* 承载协议ID，对于HTTP承载规则该值无效，此id只能是系统规则协议对应的id，不能是自定义规则的协议id，为0时表示无承载关系，如果ulBearID不能识别，则自定义规则不能被识别 */
    EGN_UINT16  usBearDepth;        /* 承载的深度，取值大于等于0，小于等于65535，最大深度为65535 */
    EGN_UINT8   ucSamePacket;       /* 是否和承载协议在同一个包，EGN_UD_BEAR_SAME_PKT_EN */
    EGN_UINT8   ucReserved;         /* 自身4、8字节对齐保留字节 */
}EgnUDRuleBearInfo;

/*STRUCT< 自定义规则公共信息 >*/
typedef struct _EgnUDRuleBaseInfo
{
    EGN_UINT32          ulProtocolId;       /* 小类协议ID，与系统知识库中的协议id区分，最小取值EGN_UD_ID_MIN，最大取值EGN_UD_ID_MAX */
    EGN_UINT32          ulAppID;            /* 应用ID，最小取值EGN_UD_ID_MIN，最大取值EGN_UD_ID_MAX */
    EGN_UINT32          ulSrvID;            /* 大类协议ID，最小取值EGN_UD_ID_MIN，最大取值EGN_UD_ID_MAX */
    EGN_UINT32          ulRuleID;           /* 规则编号，与系统知识库中的规则编号区分，最小取值EGN_UD_ID_MIN，最大取值EGN_UD_ID_MAX */
    EGN_UINT8           ucWeight;           /* 规则权重，取值范围为大于等于1，小于等于99，值越大，权重越高，为0时将取默认权重13 */
    EGN_UINT8           ucL4Proto;          /* 4层协议：EGN_TRANS_TYPE_EN */
    EGN_UINT8           aucReserved[6];     /* 自身4、8字节对齐保留字节 */
    EGN_UCHAR           aucRuleName[EGN_USER_RULE_NAME_MAX_LEN + 1];  /* 规则Name，最大长度为EGN_USER_RULE_NAME_MAX_LEN，由字母、数字、'-'、'_'组合 */
}EgnUDRuleBaseInfo;

/*STRUCT< 用户pcre条件定义 >*/
typedef struct _EgnUDCondPcre
{
    EGN_UINT8   ucPcreLen;          /* PCRE长度，用户需要保证和内容对应 */
    EGN_UINT8   ucKeyNo;            /* 关键字usKeyLoc的偏移类型，EGN_UD_STRING_BASE_EN，普通单包规则里面此字段无意义，http承载规则里面此字段取首行或头域或body */
    EGN_UINT8   aucReserved[6];     /* 自身4、8字节对齐保留字节 */
    EGN_UINT8   aucPcre[EGN_USER_PCRE_PATTERN_MAX_LEN + 1];    /* PCRE规则，最大长度为63，用户需要保证和长度对应 */
}EgnUDCondPcre;

/*STRUCT< 用户字符串条件定义 >*/
typedef struct _EgnUDCondStr
{
    EGN_UINT16 usKeyLoc;            /* 关键字出现偏移，取值[0,63]，或等于65535；等于65535时，ucKeyNo只能为EGN_UD_STRING_BASE_LOAD_BEGIN，表示偏移为不限制 。如果本规则是HTTP承载，本字段无效，*/
    EGN_UCHAR  ucKeyNo;             /* 关键字usKeyLoc的偏移类型，EGN_UD_STRING_BASE_EN */
    EGN_UINT8  ucKeyLen;            /* 关键字长度(如果关键字是16进制则表示转换后的长度)，取值[3,31]，用户需要保证和内容相符 */
    EGN_UINT8  ucKeyType;           /* 标示关键字格式：EGN_UD_CONTENT_TYPE_EN */
    EGN_UINT8  aucReserved[3];      /* 保留字段 */
    EGN_UCHAR  aucKeyInfo[EGN_UD_SINGLE_PKT_STR_BUF_LEN];  /* 关键字，普通字符串最大长度为31，十六进制格式最大为62，用户需要保证和长度相符 */
    EGN_UINT8  aucReserved1;         /* 保留字段 */
}EgnUDCondStr;

/*STRUCT< 自定义五元组信息 >*/
typedef struct _EgnUDCondFiveTuple
{
    EGN_UINT16      usSrcPort;      /* 源端口，为0表示没有源端口条件 */
    EGN_UINT16      usDstPort;      /* 目的端口，为0表示没有目的端口条件 */
    EGN_UINT16      usSrcPortEnd;   /* 源端口范围上限，减去源端口的差值必须大于等于0，小于128，为0时表示和源端口相等 */
    EGN_UINT16      usDstPortEnd;   /* 目的端口范围上限，减去目的端口的差值必须大于等于0，小于128，为0时表示和目的端口相等 */
    EgnIpAddrCond   stSrcIp;        /* 源IP地址, 支持ipv4和ipv6，支持网段，网络序大端表示。为全0时表示没有SrcIP条件 */
    EgnIpAddrCond   stDstIp;        /* 目的IP地址, 支持ipv4和ipv6，支持网段，网络序大端表示。为全0时表示没有DstIP条件 */
}EgnUDCondFiveTuple;

/*STRUCT< 自定义包深度条件 >*/
typedef struct _EgnUDCondDepth
{
    EGN_UINT32   ulDepthType;      /* 使用距离还是范围，见EGN_UD_DEPTH_TYPE_EN */
    EGN_UINT32   ulMaxDepth;       /* 深度，最大取值254，为0表示无深度条件 */
}EgnUDCondDepth;

/*STRUCT< 自定义单包规则信息 >*/
typedef struct _EgnUDSinglePacketRule
{
    EgnUDRuleBaseInfo   stRuleBaseInfo;     /* 规则基本信息 */
    EgnUDRuleBearInfo   stBearInfo;         /* 规则承载信息 */
    EGN_UINT8           ucCondPcreNum;      /* 单包规则含有的正则式条件个数 */
    EGN_UINT8           ucCondStrNum;       /* 单包规则含有的字符串条件个数 */
    EGN_UINT8           aucReserved[6];     /* 自身4、8字节对齐保留字段 */
    EgnUDCondPcre       astCondPcre[EGN_USER_PCRE_PATTERN_MAX_NUM];   /* 正则表达式条件信息，依次填充，保证成员个数和ucCondPcreNum对应 */
    EgnUDCondStr        astCondStr[EGN_USER_STRING_PATTERN_MAX_NUM];  /* 字符串条件信息，依次填充，保证成员个数和ucCondStrNum对应 */
    EgnUDCondFiveTuple  stCondFiveTuple;    /* 五元组条件信息 */
    EgnUDCondDepth      stCondDepth;        /* 深度条件信息 */
}EgnUDSinglePacketRule;

/*MACRO< 自定义HTTP承载规则的字符串条件的最小长度 >*/
#define EGN_USER_HTTP_BEAR_RULE_STR_MIN_LEN      7

/*MACRO< 自定义HTTP承载规则的字符串条件的最大长度 >*/
#define EGN_USER_HTTP_BEAR_RULE_STR_MAX_LEN      40

/*MACRO< 自定义HTTP承载规则支持最多可以指定的字符串特征的个数 >*/
#define EGN_USER_HTTP_BEAR_STRING_PATTERN_MAX_NUM      4

/*MACRO< 自定义HTTP承载规则支持最多可以指定的pcre特征的个数 >*/
#define EGN_USER_HTTP_BEAR_PCRE_PATTERN_MAX_NUM      2

/*STRUCT< 用户HTTP承载字符串条件定义 >*/
typedef struct _EgnUDHttpBearCondStr
{
    EGN_UINT16 usKeyLoc;            /* 头域字符串中头域值忽略前面的空格后，相对于头域名的偏移，对于首行、消息体字符串不支持偏移 */
    EGN_UCHAR  ucKeyNo;             /* 关键字usKeyLoc的偏移类型，EGN_UD_STRING_BASE_EN */
    EGN_UINT8  ucKeyLen;            /* 关键字转化为普通字符串后的长度，除去格式头("URL:"、"HEADER:"、"BODY:")外，有效关键字的取值大于等于EGN_UD_STR_CONDITION_LEN_MIN，小于等于EGN_UD_STR_CONDITION_LEN_MAX */
    EGN_UINT8  ucKeyType;           /* 关键字形式：字符串、十六进制 EGN_UD_CONTENT_TYPE_EN*/
    EGN_UINT8  aucReserved[3];      /* 保留字段 */
    EGN_UCHAR  aucKeyInfo[EGN_UD_HTTP_BEAR_STR_BUF_LEN];  /* 关键字，最大长度为31,有效长度由ucKeyLen指定。这里因为HTTP承载规则有指定所属位置的说明，所以加长 */
}EgnUDHttpBearCondStr;

/*STRUCT< 自定义HTTP承载规则信息 >*/
typedef struct _EgnUDHTTPBearRule
{
    EgnUDRuleBaseInfo       stRuleBaseInfo;     /* 规则基本信息 */
    EgnUDRuleBearInfo       stBearInfo;         /* 规则承载信息 */
    EGN_UINT8               ucCondPcreNum;      /* 单包规则含有的正则式条件个数 */
    EGN_UINT8               ucCondStrNum;       /* 单包规则含有的字符串条件个数 */
    EGN_UINT8               aucReserved[6];     /* 保留字段 */
    EgnUDCondPcre           astCondPcre[EGN_USER_HTTP_BEAR_PCRE_PATTERN_MAX_NUM];     /* 正则表达式条件信息 */
    EgnUDHttpBearCondStr    astCondStr[EGN_USER_HTTP_BEAR_STRING_PATTERN_MAX_NUM];    /* 字符串条件信息 */
    EgnUDCondFiveTuple      stCondFiveTuple;    /* 五元组条件信息 */
    EgnUDCondDepth          stCondDepth;        /* 深度条件信息 */
}EgnUDHTTPBearRule;

/*MACRO< 自定义行为统计规则支持最多可以指定pattern个数 >*/
#define EGN_USER_BEHA_PATTERN_MAX_NUM   8

/*MACRO< 自定义行为统计规则支持包个数最大数量  >*/
#define EGN_USER_BEHA_PKT_MAX_NUM 64

/*MACRO< 自定义行为统计规则支持包个数最小数量  >*/
#define EGN_USER_BEHA_PKT_MIN_NUM 1

/* 模式统计值最大值: 目前占用14 位*/
#define EGN_STAT_MATCH_VALUE_MAX    EGN_STAT_MAX_PACKET_LOADLEN

/*STRUCT< 自定义行为统计范围区间的定义 >*/
typedef struct _EgnRange
{
    EGN_UINT16  usStart;        /* 下限，起始值 */
    EGN_UINT16  usEnd;          /* 上限，终止值 */
    EGN_UINT8   aucReserved[4]; /* 保留 */
} EgnRange;

/*ENUM< 自定义行为规则规则模式串类型 >*/
typedef enum
{
    EGN_UD_BEHA_PTN_TYPE_BEGIN      = EGN_EN_INVALID,
    EGN_UD_BEHA_PTN_TYPE_AVG        = 0,   /* 包长平均值 */
    EGN_UD_BEHA_PTN_TYPE_RATIO      = 1,   /* 包长收发比 */
    EGN_UD_BEHA_PTN_TYPE_NUM        = 2,   /* 包长在某个范围内的包个数 */
    EGN_UD_BEHA_PTN_TYPE_SEQ        = 3,   /* 包长序列 */
    EGN_UD_BEHA_PTN_TYPE_END,

    EGN_UD_BEHA_PTN_TYPE_BOTTOM = EGN_EN_BUTT
}EGN_UD_BEHA_PTN_TYPE_EN;

/*ENUM< 自定义行为规则规则模式串检查的方向 >*/
typedef enum
{
    EGN_UD_DIR_BEGIN         = EGN_EN_INVALID,
    EGN_UD_DIR_BI            = 0,   /* 双向 */
    EGN_UD_DIR_UP            = 1,   /* 上行 */
    EGN_UD_DIR_DOWN          = 2,   /* 下行 */
    EGN_UD_DIR_END,

    EGN_UD_DIR_BOTTOM = EGN_EN_BUTT
}EGN_UD_DIR_EN;

/*STRUCT< 自定义行为统计规则模式串 >*/
typedef struct _EgnUDBehaPattern
{
    EGN_UINT8   ucType;             /* 模式类型:EGN_UD_BEHA_PTN_TYPE_EN */
    EGN_UINT8   ucDirection;        /* 方向，如果是收发比模式必须是双向的：EGN_UD_DIR_EN */

    /* 要统计的数据包位置范围，取值范围1-64 */
    EGN_UINT8   ucPKTStartPos;      /* 要统计的数据包的起始位置。取值范围1-64 */
    EGN_UINT8   ucPKTEndPos;        /* 要统计的数据包的结束位置。取值范围1-64 */
    EGN_UINT8   aucReserved[4];     /* 保留 */

    /* 值的范围：平均值、收发比、包计数使用此字段 */
    EgnRange stValueRange;  /*值域范围在[1,EGN_STAT_MATCH_VALUE_MAX]*/

    /* 包计数模式需要用到的包长范围 */
    EgnRange stPKTLenRange; /*值域范围对于EGN_UD_BEHA_PTN_TYPE_NUM模式在[1, EgnUDBehaPattern]在[1,EGN_STAT_MATCH_VALUE_MAX]，*/

    /* 包长序列使用此字段，最多64个，小于64个的情况下以0表示序列的结束（包长不允许为0） */
    EGN_UINT16   ausPKTLenSeq[EGN_USER_BEHA_PKT_MAX_NUM]; /* 包长序列的值域最大值为EGN_STAT_MATCH_VALUE_MAX，依次填充 */
} EgnUDBehaPattern;

/*STRUCT< 自定义行为统计规则信息 >*/
typedef struct _EgnUDBehaRule
{
    EgnUDRuleBaseInfo   stRuleBaseInfo;     /* 规则基本信息 */
    EGN_UINT8           ucUDBehaPtnNum;     /* 行为统计模式串数量 */
    EGN_UINT8           ucReserved[3];      /* 保留字段 */
    EGN_UINT16          usPort;             /* 端口下限,如果指定端口，则不可以为0，但区分是源的还是目的*/
    EGN_UINT16          usPortEnd;          /* 端口上限,大于等于端口下限,端口下限值为0时可以为0 */
    EgnUDBehaPattern    astBehaPtn[EGN_USER_BEHA_PATTERN_MAX_NUM]; /* 模式串信息，依次填充，保证成员个数和ucUDBehaPtnNum对应 */
} EgnUDBehaRule;

/*STRUCT< 自定义等差模式串信息 >*/
typedef struct _EgnUDArithPattern
{
    EGN_UINT32  ulDelta;        /* 等差数值，如果差值未知则填写0xFFFFFFFF，不支持等差值为负的情况(比如-2翻转为0xFFFFFFFE)，使用时请注意此约束 */
    EGN_UINT16  usOffsetLoc;    /* 等差字段偏移位置，取值大于等于0，小于等于1500 */
    EGN_UINT8   ucOffsetDir;    /* 等差字段偏移方向：只能取值负载头EGN_UD_STRING_BASE_LOAD_BEGIN或负载尾EGN_UD_STRING_BASE_LOAD_END */
    EGN_UINT8   ucByteOrder;    /* 等差字段字节序：只能取值EGN_UD_BYTE_ORDER_BIG或EGN_UD_BYTE_ORDER_LITTLE */
    EGN_UINT8   ucLen;          /* 等差字段长度，取值大于等于1小于等于4 */
    EGN_UINT8   aucReserved[7]; /* 保留 */
}EgnUDArithPattern;

/*STRUCT< 自定义等差规则信息 >*/
typedef struct _EgnUDArithRule
{
    EgnUDRuleBaseInfo   stRuleBaseInfo;   /* 规则基本信息 */
    EGN_UINT8           ucUDArithPtnNum;  /* 等差模式串数量，只能取值1或者2 */
    EGN_UINT8           ucReserved[3];    /* 保留字段 */
    EGN_UINT16          usPort;           /* 端口, 不指定是源的还是目的，为0表示没有端口条件 */
    EGN_UINT16          usPortEnd;        /* 端口范围上限，减去源端口的差值必须大于等于0，为0时表示和端口相等，为固定端口 */
    EgnUDArithPattern   astArithPtn[EGN_USER_ARITH_PATTERN_MAX_NUM];  /* 模式串信息，依次填充值，保证成员个数和ucUDArithPtnNum对应 */
}EgnUDArithRule;

/*******************************************************************************
*    Func Name: EgnApiUDAddSinglePktRule
*      Purpose: 添加自定义单包规则
*  Description: 按照结构体方式添加自定义单包规则，待激活后才能生效。该接口必须在初始化时开启自定义规则功能开关
                    （设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE），且初始化成功后，才能添加规则。
*        Input: EgnUDSinglePacketRule  *pstUDSinglePacketRule:单包规则结构体指针<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、自定义单包规则的父只能为协议，且只能为系统规则
                2、协议ID和规则ID:[0x10000000, 0x1FFFFFFF]
                3、支持的自定义单包规则最大数量为1024
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDAddSinglePktRule
(
   IN     EgnUDSinglePacketRule  *pstUDSinglePacketRule
);

/*******************************************************************************
*    Func Name: EgnApiUDAddArithRule
*      Purpose: 添加自定义等差规则
*  Description: 按照结构体方式添加自定义等差规则，待激活后才能生效。该接口必须在初始化时开启自定义规则功能开关
                    （设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE），且初始化成功后，才能添加规则。
*        Input: EgnUDArithRule *pstUDArithRule:等差规则结构指针<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、自定义等差、自定义行为识别不支持承载识别（作为父、子都不支持）
                2、协议ID和规则ID:[0x10000000, 0x1FFFFFFF]
                3、支持的用户自定义等差规则最大数量为16
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDAddArithRule
(
   IN     EgnUDArithRule *pstUDArithRule
);

/*******************************************************************************
*    Func Name: EgnApiUDAddBehaRule
*      Purpose: 添加自定义统计规则
*  Description: 按照结构体方式添加自定义等差规则，待激活后才能生效。该接口必须在初始化时开启自定义规则功能开关和统计识别开关
                    （设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE;EgnInitCfgParam.bStatInspectSwitch为EGN_TRUE），
                    且初始化成功后，才能添加规则。
*        Input: EgnUDBehaRule *pstUDBehaRule:统计规则结构指针<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 自定义统计规则最多支持64条
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDAddBehaRule
(
    IN  EgnUDBehaRule   *pstUDBehaRule
);

/*******************************************************************************
*    Func Name: EgnApiUDActiveRule
*      Purpose: 激活自定义规则
*  Description: 在添加或删除自定义规则后，调用本接口激活，使添加或删除操作生效。
                调用一次相当于对知识库做一次升级/回退，当系统知识库正在升级或者回退过程不能进行自定义规则激活。
                不能连续调用，需要等待主备知识库切换完成后才能再次调用。
*        Input: NA
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDActiveRule
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiUDAddHTTPBearRule
*      Purpose: 添加自定义HTTP承载规则
*  Description: 按照结构体方式添加自定义HTTP承载规则，待激活后才能生效。该接口必须在初始化时开启自定义规则功能开关
                    （设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE），且初始化成功后，才能添加规则。
*        Input: EgnUDHTTPBearRule  *pstUDHTTPBearRule:文件内存指针<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 1、HTTP承载规则在规则头上不需要再指定承载ID，即使指定也无效
                2、HTTP承载规则的字符串offset不做限制，因为本身就是一个精度匹配
                3、HTTP承载规则的字符串条件、PCRE字符串中的ucKeyNo字符段表示的含义是，指定该字符串需要匹配的位置是
                   首行、头域还是消息体，并且这个字符串必须和字符串指定的信息相符，否则添加失败
                   例如：字符串：URL：get*，ucKeyNo的值应该为首行
                4、HTTP承载字符串中，必须保证有效的特征串大于等于3且小于等于31，
                   即url：、body：后的字符串忽略前后空格后，剩余的长度必须大于等于3且小于等于31；
                   header:头域名：后的头域值部分去前后空格后，剩余的长度必须大于等于3且小于等于31。
                5、说明字符串的位置的"url:"、"body:"、"header:"的字符部分不区分大小写，但是字符和":"是一体的，中间不能有空格

*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDAddHTTPBearRule
(
    IN   EgnUDHTTPBearRule  *pstUDHTTPBearRule
);

/*******************************************************************************
*    Func Name: EgnApiUDGetHTTPBearRuleById
*      Purpose: 查询自定义HTTP承载规则
*  Description: 根据协议ID和规则ID, 查询自定义HTTP承载规则。该接口必须在初始化时开启自定义规则功能开关
                    （设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE），且初始化成功后才能查询规则。
*        Input: EGN_UINT32  ulProtolId:待查询的规则对应的协议ID:[0x10000000, 0x1FFFFFFF]或者 0.
                                        为零时表示按ulRuleId查找.
*               EGN_UINT32  ulRuleId:待查询的规则对应的规则ID:[0x10000000, 0x1FFFFFFF]或者 0.
                                        为零时表示按ulProtolId查找.
*        InOut: EGN_UINT32               *pulItemNum:入参:保存查询结果的数组的大小<非零>
                                                     出参: 系统中符合查询条件规则的实际个数
*       Output: EgnUDHTTPBearRule  *pstHTTPBearRule:保存查询出来的自定义规则信息<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: ulProtolId和ulRuleId不能同时为零
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDGetHTTPBearRuleById
(
    IN    EGN_UINT32          ulProtolId,
    IN    EGN_UINT32          ulRuleId,
    OUT   EgnUDHTTPBearRule  *pstHTTPBearRule,
    INOUT EGN_UINT32         *pulItemNum
);

/*******************************************************************************
*    Func Name: EgnApiUDDelHTTPBearRuleById
*      Purpose: 删除自定义HTTP承载规则
*  Description: 根据协议ID和规则ID, 删除自定义HTTP承载规则。该接口必须在初始化时开启自定义规则功能开关
                    （设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE），且初始化成功后才能删除规则。
*        Input: EGN_UINT32  ulProtolId:待删除的规则对应的协议ID:[0x10000000, 0x1FFFFFFF]
*               EGN_UINT32  ulRuleId:待删除的规则对应的规则ID:[0x10000000, 0x1FFFFFFF]
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDDelHTTPBearRuleById
(
    IN  EGN_UINT32          ulProtolId,
    IN  EGN_UINT32          ulRuleId
);

/*******************************************************************************
*    Func Name: EgnApiUDGetSinglePKTRuleById
*      Purpose: 查询自定义单包规则
*  Description: 根据协议ID和规则ID, 查询自定义单包规则。该接口必须在初始化时开启自定义规则功能开关
                    （设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE），且初始化成功后才能查询规则。
*        Input: EGN_UINT32  ulProtolId:待查询的规则对应的协议ID:[0x10000000, 0x1FFFFFFF]或者 0.
                                        为零时表示按ulRuleId查找.
*               EGN_UINT32  ulRuleId:待查询的规则对应的规则ID:[0x10000000, 0x1FFFFFFF]或者 0.
                                        为零时表示按ulProtolId查找.
*        InOut: EGN_UINT32               *pulItemNum:入参:保存查询结果的数组的大小<非零>
                                                     出参: 系统中符合查询条件规则的实际个数
*       Output: EgnUDSinglePacketRule  *pstSinglePKTRule:保存查询出来的自定义规则信息<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: ulProtolId和ulRuleId不能同时为零
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDGetSinglePKTRuleById
(
    IN    EGN_UINT32               ulProtolId,
    IN    EGN_UINT32               ulRuleId,
    OUT   EgnUDSinglePacketRule    *pstSinglePKTRule,
    INOUT EGN_UINT32               *pulItemNum
);

/*******************************************************************************
*    Func Name: EgnApiUDDelSinglePKTRuleById
*      Purpose: 删除自定义单包规则
*  Description: 根据协议ID和规则ID, 删除自定义单包规则。该接口必须在初始化时开启自定义规则功能开关
                    （设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE），且初始化成功后才能删除规则。
*        Input: EGN_UINT32  ulProtolId:待删除的规则对应的协议ID:[0x10000000, 0x1FFFFFFF]
*               EGN_UINT32  ulRuleId:待删除的规则对应的规则ID:[0x10000000, 0x1FFFFFFF]
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32  EgnApiUDDelSinglePKTRuleById
(
    IN  EGN_UINT32               ulProtolId,
    IN  EGN_UINT32               ulRuleId
);

/*******************************************************************************
*    Func Name: EgnApiUDGetArithRuleById
*      Purpose: 查询自定义等差规则
*  Description: 根据协议ID和规则ID, 查询自定义等差规则。该接口必须在初始化时开启自定义规则功能开关
                    （设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE），且初始化成功后才能查询规则。
*        Input: EGN_UINT32  ulProtolId:待查询的规则对应的协议ID:[0x10000000, 0x1FFFFFFF]或者 0.
                                        为零时表示按ulRuleId查找.
*               EGN_UINT32  ulRuleId:待查询的规则对应的规则ID:[0x10000000, 0x1FFFFFFF]或者 0.
                                        为零时表示按ulProtolId查找.
*        InOut: EGN_UINT32              *pulItemNum:入参:保存查询结果的数组的大小<非零>.
                                                    出参: 系统中符合查询条件规则的实际个数
*       Output: EgnUDArithRule pstArithRule：查询出来的自定义规则信息<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: ulProtolId和ulRuleId不能同时为零
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDGetArithRuleById
(
    IN      EGN_UINT32               ulProtolId,
    IN      EGN_UINT32               ulRuleId,
    OUT     EgnUDArithRule          *pstArithRule,
    INOUT   EGN_UINT32              *pulItemNum
);

/*******************************************************************************
*    Func Name: EgnApiUDDelArithRuleById
*      Purpose: 删除自定义等差规则
*  Description: 根据协议ID和规则ID, 删除自定义等差规则。该接口必须在初始化时开启自定义规则功能开关
                    （设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE），且初始化成功后才能删除规则。
*        Input: EGN_UINT32  ulProtolId:待删除的规则对应的协议ID:[0x10000000, 0x1FFFFFFF]
*               EGN_UINT32  ulRuleId:待删除的规则对应的规则ID:[0x10000000, 0x1FFFFFFF]
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32  EgnApiUDDelArithRuleById
(
    IN  EGN_UINT32               ulProtolId,
    IN  EGN_UINT32               ulRuleId
);

/*******************************************************************************
*    Func Name: EgnApiUDGetSinglePKTRules
*      Purpose: 查询一条或多条自定义单包规则。
*  Description: 1、查询一条或多条自定义单包规则,该查询方式必须在开启自定义规则功能开关
                 （设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE），并且初始化后才能有效
                2、入参pulItemNum代表的整形数为1024时，查询所有单包规则
                3、入参pulItemNum代表的整形数为0时，查询失败
*        Input: NA
*        InOut: EgnUDSinglePacketRule   *pstUdRuleList:保存查询结果的规则数组<非空>
                EGN_UINT32              *pulItemNum:输入为规则数组的大小，输出为系统中自定义单包规则总数<非空>
*       Output: NA
*       Return: EGN_UINT32，若数组大小不够，返回错误码为EGN_RET_ERR_BUF_MORE。其余请参见EGN_RET_RESULT_EN。
*      Caution: NA
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDGetSinglePKTRules
(
    INOUT   EgnUDSinglePacketRule   *pstUdRuleList,
    INOUT   EGN_UINT32              *pulItemNum
);

/*******************************************************************************
*    Func Name: EgnApiUDGetHttpBearRules
*      Purpose: 查询一条或多条自定义HTTP承载规则。
*  Description: 1、查询一条或多条自定义HTTP承载规则,该查询方式必须在开启自定义规则功能开关
                 （设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE），并且初始化后才能有效
                2、入参pulItemNum代表的整形数为64时，查询所有HTTP承载规则
                3、入参pulItemNum代表的整形数为0时，查询失败
*        Input: NA
*        InOut: EgnUDHTTPBearRule   *pstUdRuleList:保存查询结果的规则数组<非空>
                EGN_UINT32     *pulItemNum:输入为规则数组的大小，输出为系统中自定义HTTP承载规则总数<非空>
*       Output: NA
*       Return: EGN_UINT32，若数组大小不够，返回错误码为EGN_RET_ERR_BUF_MORE。其余请参见EGN_RET_RESULT_EN。
*      Caution: NA
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDGetHttpBearRules
(
    INOUT   EgnUDHTTPBearRule   *pstUdRuleList,
    INOUT   EGN_UINT32          *pulItemNum
);

/*******************************************************************************
*    Func Name: EgnApiUDGetArithRules
*      Purpose: 查询一条或多条自定义等差规则。
*  Description: 1、查询一条或多条自定义等差规则,该查询方式必须在开启自定义规则功能开关
                 （设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE），并且初始化后才能有效
                2、入参pulItemNum代表的整形数为16时，查询所有等差规则
                3、入参pulItemNum代表的整形数为0时，查询失败
*        Input: NA
*        InOut: EgnUDArithRule   *pstUdRuleList:保存查询结果的规则数组<非空>
                EGN_UINT32       *pulItemNum:输入为规则数组的大小，输出为系统中自定义等差规则总数<非空>
*       Output: NA
*       Return: EGN_UINT32，若数组大小不够，返回错误码为EGN_RET_ERR_BUF_MORE。其余请参见EGN_RET_RESULT_EN。
*      Caution: NA
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDGetArithRules
(
    INOUT   EgnUDArithRule      *pstUdRuleList,
    INOUT   EGN_UINT32          *pulItemNum
);

/*******************************************************************************
*    Func Name: EgnApiUDGetBehaRules
*      Purpose: 查询一条或多条自定义行为统计规则。
*  Description: 1、查询一条或多条自定义行为统计规则,该查询方式必须在开启自定义规则功能开关
                 （设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE），并且初始化后才能有效
                2、入参pulItemNum代表的整形数为64时，查询所有行为统计规则
                3、入参pulItemNum代表的整形数为0时，查询失败
*        Input: NA
*        InOut: EgnUDArithRule   *pstUdRuleList:保存查询结果的规则数组<非空>
                EGN_UINT32       *pulItemNum:输入为规则数组的大小，输出为系统中自定义行为统计规则总数<非空>
*       Output: NA
*       Return: EGN_UINT32，若数组大小不够，返回错误码为EGN_RET_ERR_BUF_MORE。其余请参见EGN_RET_RESULT_EN。
*      Caution: NA
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDGetBehaRules
(
    INOUT   EgnUDBehaRule       *pstUdRuleList,
    INOUT   EGN_UINT32          *pulItemNum
);

/*******************************************************************************
*    Func Name: EgnApiUDGetRuleNumByType
*      Purpose: 根据自定义规则类型，查询出该类型下的规则个数。
*  Description: 1、根据自定义规则类型，查询出该类型下的规则个数，必须在开启自定义规则功能开关，并且初始化后才能有效
                2、规则类型:EGN_UD_RULE_TYPE_ALL表示查询所有的自定义规则的个数
*        Input: EGN_UINT32     ulUdRuleType:需要查询的自定义规则类型<EGN_UD_RULE_TYPE_SINGLE_PACKET:自定义单包规则
                                                                     EGN_UD_RULE_TYPE_BEHA：自定义统计规则
                                                                     EGN_UD_RULE_TYPE_HTTP_BEAR：自定义HTTP承载规则
                                                                     EGN_UD_RULE_TYPE_ARITH：自定义等差规则
                                                                     EGN_UD_RULE_TYPE_ALL：所有自定义规则>
*        InOut: EGN_UINT32     *pulRuleNum:该类型下的规则个数<非空>
*       Output: NA
*       Return: EGN_UINT32，查询成功或失败，请参见EGN_RET_RESULT_EN。
*      Caution: NA
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDGetRuleNumByType
(
    IN     EGN_UINT32     ulUdRuleType,
    INOUT  EGN_UINT32     *pulRuleNum
);

/*******************************************************************************
*    Func Name: EgnApiUDIsNeedActiveRule
*      Purpose: 查询自定义规则是否需要激活
*  Description: 查询自定义规则是否需要激活，必须在开启自定义规则功能开关，并且初始化后才能有效
                在需要激活时，需要满足激活条件，否则仍然会激活失败
*        Input: EGN_BOOL    *pbIsNeedActive:是否需要激活，EGN_TRUE需要激活；EGN_FALSE不需要激活<非空>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution:
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDIsNeedActiveRule
(
    OUT EGN_BOOL    *pbIsNeedActive
);

/*******************************************************************************
*    Func Name: EgnApiUDDelRuleByType
*      Purpose: 根据自定义规则类型，删除该类型下的所有规则。
*  Description: 1、根据自定义规则类型，删除该类型下的所有规则，必须在开启自定义规则功能开关，并且初始化后才能有效。
                2、规则类型为EGN_UD_RULE_TYPE_ALL时，删除所有自定义规则。
                3、删除规则后需要重新激活才能在识别过程中生效
*        Input: EGN_UINT32     ulUdRuleType:需要删除的自定义规则的类型<EGN_UD_RULE_TYPE_SINGLE_PACKET:自定义单包规则
                                                                       EGN_UD_RULE_TYPE_BEHA：自定义统计规则
                                                                       EGN_UD_RULE_TYPE_HTTP_BEAR：自定义HTTP承载规则
                                                                       EGN_UD_RULE_TYPE_ARITH：自定义等差规则
                                                                       EGN_UD_RULE_TYPE_ALL：所有自定义规则>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，查询成功或失败，请参见EGN_RET_RESULT_EN。
*      Caution: NA
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDDelRuleByType
(
    IN     EGN_UINT32     ulUdRuleType
);

/*******************************************************************************
*    Func Name: EgnApiUDDelRuleByProtolID
*      Purpose: 删除指定协议下的所有自定义规则
*  Description: 1、根据指定的协议ID，查找所有的自定义规则的协议集合，如果查找到，
                    将该协议下的规则标记为需要删除，如果找不到，则通知用户该协议不存在。
                2、该接口必须在初始化时开启自定义规则功能开关（设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE），
                    且初始化成功后才能删除规则。
*        Input: EGN_UINT32  ulProtolId:待删除协议的ID:[0x10000000, 0x1FFFFFFF]
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，协议不存在返回EGN_RET_UD_PROTOCOL_ID_NO_EXIST，
                其他具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32  EgnApiUDDelRuleByProtolID
(
    IN  EGN_UINT32               ulProtolId
);

/*******************************************************************************
*    Func Name: EgnApiUDGetBehaRuleById
*      Purpose: 查询自定义统计规则
*  Description: 根据协议ID和规则ID, 查询自定义统计规则。该接口必须在初始化时开启自定义规则功能开关和统计识别开关
                    （设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE;EgnInitCfgParam.bStatInspectSwitch为EGN_TRUE），
                    且初始化成功后才能查询规则。
*        Input: EGN_UINT32  ulProtolId:待查询的规则对应的协议ID:[0x10000000, 0x1FFFFFFF]或者 0.
                                        为零时表示按ulRuleId查找.
*               EGN_UINT32  ulRuleId:待查询的规则对应的规则ID:[0x10000000, 0x1FFFFFFF]或者 0.
                                        为零时表示按ulRuleId查找.
*        InOut: EGN_UINT32  *pulItemNum:入参:保存查询结果的数组的大小<非零>
                                        出参: 系统中符合查询条件规则的实际个数
*       Output: EgnUDBehaRule pstBehaRule：查询出来的自定义规则信息<非空>
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: ulProtolId和ulRuleId不能同时为零
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiUDGetBehaRuleById
(
    IN      EGN_UINT32       ulProtolId,
    IN      EGN_UINT32       ulRuleId,
    OUT     EgnUDBehaRule   *pstBehaRule,
    INOUT   EGN_UINT32      *pulItemNum
);

/*******************************************************************************
*    Func Name: EgnApiUDDelBehaRuleById
*      Purpose: 删除自定义统计规则
*  Description: 根据协议ID和规则ID, 删除自定义统计规则。该接口必须在初始化时开启自定义规则功能开关和统计识别开关
                    （设置EgnInitCfgParam.bUDRuleSwitch为EGN_TRUE;EgnInitCfgParam.bStatInspectSwitch为EGN_TRUE），
                    且初始化成功后才能删除规则。
*        Input: EGN_UINT32  ulProtolId:待删除的规则对应的协议ID:[0x10000000, 0x1FFFFFFF]
*               EGN_UINT32  ulRuleId:待删除的规则对应的规则ID:[0x10000000, 0x1FFFFFFF]
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32，成功或者失败的错误码，具体请参见EGN_RET_RESULT_EN。
*      Caution: 无
*        Since: V300R006
*    Reference: 无
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32  EgnApiUDDelBehaRuleById
(
    IN  EGN_UINT32               ulProtolId,
    IN  EGN_UINT32               ulRuleId
);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EGN_API_USER_DEFINE_RULE_H__ */

