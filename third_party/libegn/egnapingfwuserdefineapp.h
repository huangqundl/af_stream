/*
 ******************************************************************************
 °æÈ¨ËùÓÐ (C), 2008-2009, »ªÎª¼¼ÊõÓÐÏÞ¹«Ë¾
 ******************************************************************************
  ÎÄ ¼þ Ãû   : egnapingfwuserdefineapp.h
  °æ ±¾ ºÅ   : ³õ¸å
  ×÷    Õß   : EGNÏîÄ¿×é
  Éú³ÉÈÕÆÚ   : 2013Äê01ÔÂ10ÈÕ
  ×î½üÐÞ¸Ä   :
  ¹¦ÄÜÃèÊö   : ¹«¹²ºê¶¨Òå
  º¯ÊýÁÐ±í   :
  ÐÞ¸ÄÀúÊ·   :
  1.ÈÕ    ÆÚ   : 2013Äê01ÔÂ10ÈÕ
    ×÷    Õß   : EGNÏîÄ¿×é
    ÐÞ¸ÄÄÚÈÝ   : ´´½¨ÎÄ¼þ

******************************************************************************/
/**@file  egnapingfwuserdefineapp.h
  *    ¹«¹²ºê¶¨Òå
*******************************************************/
/**
 * @defgroup egn  EGN×Ô¶¨Òå¹æÔòÄ£¿éµÄAPI
 */

#ifndef __EGN_API_NGFW_USER_DEFINE_APP_H__
#define __EGN_API_NGFW_USER_DEFINE_APP_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*MACRO< NGFW×Ô¶¨ÒåÓ¦ÓÃÃûÖ§³ÖµÄ×î´ó³¤¶È >*/
#define EGN_NGFW_UD_APP_NAME_LEN_MAX  32

/*MACRO< NGFW×Ô¶¨ÒåÓ¦ÓÃÖ§³ÖµÄÊý¾ÝÄ£ÐÍ×î´ó³¤¶È >*/
#define EGN_NGFW_UD_MODEL_NAME_LEN_MAX  32

/*MACRO< NGFW×Ô¶¨Òå¹æÔòÃûÖ§³ÖµÄ×î´ó³¤¶È >*/
#define EGN_NGFW_UD_RULE_NAME_LEN_MAX  32

/*MACRO< NGFW×Ô¶¨Òå¹æÔòÃèÊöÖ§³ÖµÄ×î´ó³¤¶È >*/
#define EGN_NGFW_UD_RULE_DESC_LEN_MAX  128

/*MACRO< NGFW×Ô¶¨ÒåÓ¦ÓÃÃèÊöÖ§³ÖµÄ×î´ó³¤¶È >*/
#define EGN_NGFW_UD_APP_DESC_LEN_MAX  128

/*MACRO< NGFW×Ô¶¨Òå¹æÔòÖÐÖ§³ÖµÄipÌõ¼þ×î´ó¸öÊý >*/
#define EGN_NGFW_UD_RULE_IP_COND_MAX  4

/*MACRO< NGFW×Ô¶¨Òå¹æÔòÖÐÖ§³ÖµÄportÌõ¼þ×î´ó¸öÊý >*/
#define EGN_NGFW_UD_RULE_PORT_COND_MAX  4

/*MACRO< NGFW×Ô¶¨Òå¹æÔòÖÐÖ§³ÖµÄÄ£Ê½´®Ìõ¼þµÄ×îÐ¡³¤¶È >*/
#define EGN_NGFW_UD_RULE_PATTERN_COND_LEN_MIN  3

/*MACRO< NGFW×Ô¶¨Òå¹æÔòÖÐÖ§³ÖµÄÄ£Ê½´®Ìõ¼þµÄ×î´ó³¤¶È >*/
#define EGN_NGFW_UD_RULE_PATTERN_COND_LEN_MAX  128

/* Ö§³ÖµÄNgfw×Ô¶¨ÒåÓ¦ÓÃµÄAppId×îÐ¡Öµ >*/
#define EGN_NGFW_UD_APP_ID_MIN  60000

/*ENUM< NGFW×Ô¶¨Òå¹æÔòµÄ¼à²â·½Ïò >*/
typedef enum
{
    EGN_NGFW_CHECK_DIR_BEGIN = EGN_EN_INVALID,
    EGN_NGFW_CHECK_DIR_REQ   = 0,   /* ÇëÇó */
    EGN_NGFW_CHECK_DIR_RES   = 1,   /* ÏìÓ¦ */
    EGN_NGFW_CHECK_DIR_BOTH  = 2,   /* Ë«Ïò */
    EGN_NGFW_CHECK_DIR_END,

    EGN_NGFW_CHECK_DIR_BOTTOM = EGN_EN_BUTT
}EGN_NGFW_CHECK_DIR_EN;

/*ENUM< NGFW×Ô¶¨Òå¹æÔòµÄÊ¶±ðÄ£Ê½ >*/
typedef enum
{
    EGN_NGFW_INSPECT_MOD_BEGIN = EGN_EN_INVALID,
    EGN_NGFW_INSPECT_MOD_NONE   = 0,   /* Î´Öª£¬³õÊ¼×´Ì¬ */
    EGN_NGFW_INSPECT_MOD_PACKET = 1,   /* °üÊ½Ê¶±ð */
    EGN_NGFW_INSPECT_MOD_FLOW   = 2,   /* Á÷Ê½Ê¶±ð */
    EGN_NGFW_INSPECT_MOD_END,

    EGN_NGFW_INSPECT_MOD_BOTTOM = EGN_EN_BUTT
}EGN_NGFW_INSPECT_MOD_EN;

/*ENUM< NGFW×Ô¶¨Òå¹æÔòµÄÄ£Ê½´®ÀàÐÍ >*/
typedef enum
{
    EGN_NGFW_PATTERN_TYPE_BEGIN = EGN_EN_INVALID,
    EGN_NGFW_PATTERN_TYPE_NONE  = 0,   /* Î´Öª£¬³õÊ¼×´Ì¬ */
    EGN_NGFW_PATTERN_TYPE_STR   = 1,   /* ×Ö·û´® */
    EGN_NGFW_PATTERN_TYPE_PCRE  = 2,   /* ÕýÔò */
    EGN_NGFW_PATTERN_TYPE_END,

    EGN_NGFW_PATTERN_TYPE_BOTTOM = EGN_EN_BUTT
}EGN_NGFW_PATTERN_TYPE_EN;

/*ENUM< Ngfw×Ô¶¨ÒåÓ¦ÓÃµÄ×´Ì¬ >*/
typedef enum
{
    EGN_NGFW_APP_STATUS_BEGIN               = EGN_EN_INVALID,
    EGN_NGFW_APP_STATUS_INIT                = 0,   /* ³õÊ¼»¯       */
    EGN_NGFW_APP_STATUS_MODIFIED_NOTACTIVED = 1,   /* ÐÞ¸Ä,Î´¼¤»î  */
    EGN_NGFW_APP_STATUS_DELETE_NOTACTIVED   = 2,   /* É¾³ý, Î´¼¤»î */
    EGN_NGFW_APP_STATUS_ACTIVE              = 3,   /* ¼¤»î         */
    EGN_NGFW_APP_STATUS_END,
    EGN_NGFW_APP_STATUS_BOTTOM              = EGN_EN_BUTT
} EGN_NGFW_APP_STATUS_EN;

/*ENUM<Ngfw×Ô¶¨ÒåÓ¦ÓÃµÄ²éÑ¯·½Ê½> */
typedef enum
{
    EGN_NGFW_APP_QUERY_MODEL_BEGIN           = EGN_EN_INVALID,
    EGN_NGFW_APP_QUERY_MODEL_ONLY_ACTIVED    = 0,   /* Ö»²éÑ¯¼¤»îAPP         */
    EGN_NGFW_APP_QUERY_MODEL_ONLY_NOTACTIVED = 1,   /* ²éÑ¯Î´¼¤»îµÄAPP       */
    EGN_NGFW_APP_QUERY_ALL                   = 2,   /* ²éÑ¯¼¤»îºÍÎ´¼¤»îµÄAPP */
    EGN_NGFW_APP_QUERY_MODEL_END,
    EGN_NGFW_APP_QUERY_MODEL_BOTTOM          = EGN_EN_BUTT
} EGN_NGFW_APP_QUERY_MODEL_EN;

/*ENUM<Ó¦ÓÃÊôÐÔ×Ö¶ÎÀàÐÍ >*/
typedef enum
{
    EGN_NGFW_APP_ATTR_TYPE_BEGIN           = EGN_EN_INVALID,
    EGN_NGFW_APP_ATTR_TYPE_CATEGORY        = 0,   /* ×ÓÀàÀà±ð   */
    EGN_NGFW_APP_ATTR_TYPE_DATAMODEL       = 1,   /* Êý¾ÝÄ£ÐÍ   */
    EGN_NGFW_APP_ATTR_TYPE_RISK            = 2,   /* ·çÏÕµÈ¼¶   */
    EGN_NGFW_APP_ATTR_TYPE_DESC            = 3,   /* ÖÐÓ¢ÎÄÃèÊö */
    EGN_NGFW_APP_ATTR_TYPE_ALL             = 4,   /* ËùÓÐ×Ö¶Î   */
    EGN_NGFW_APP_ATTR_TYPE_END,
    EGN_NGFW_APP_ATTR_TYPE_BOTTOM          = EGN_EN_BUTT
}EGN_NGFW_APP_ATTR_TYPE_EN;

/*ENUM< NGFW×Ô¶¨Òå¹æÔòÖÐµÄ³ÉÔ±ÀàÐÍ£¬ÓÃÓÚÉ¾³ý»òÐÞ¸Ä >*/
typedef enum
{
    EGN_NGFW_UD_RULE_CONTENT_TYPE_BEGIN         = EGN_EN_INVALID,
    EGN_NGFW_UD_RULE_CONTENT_TYPE_TRANS_TYPE    = 1,
    EGN_NGFW_UD_RULE_CONTENT_TYPE_IPV4          = 2,
    EGN_NGFW_UD_RULE_CONTENT_TYPE_IPV6          = 3,
    EGN_NGFW_UD_RULE_CONTENT_TYPE_PORT          = 4,
    EGN_NGFW_UD_RULE_CONTENT_TYPE_SIGNATURE     = 5,
    EGN_NGFW_UD_RULE_CONTENT_TYPE_DESC          = 6,
    EGN_NGFW_UD_RULE_CONTENT_TYPE_IPV4_ALL      = 7,
    EGN_NGFW_UD_RULE_CONTENT_TYPE_IPV6_ALL      = 8,
    EGN_NGFW_UD_RULE_CONTENT_TYPE_PORT_ALL      = 9,
    EGN_NGFW_UD_RULE_CONTENT_TYPE_ALL           = 10,
    EGN_NGFW_UD_RULE_CONTENT_TYPE_END,
    EGN_NGFW_UD_RULE_CONTENT_TYPE_BOTTOM        = EGN_EN_BUTT
}EGN_NGFW_UD_RULE_CONTENT_TYPE_EN;

/*STRUCT< NGFW×Ô¶¨ÒåÓ¦ÓÃÐÅÏ¢ >*/
typedef struct _EgnNgfwUDApp
{
    EGN_UINT32  ulAppId;                                     /* Ó¦ÓÃID */
    EGN_UINT16  usCategoryId;                                /* ´óÀàID */
    EGN_UINT16  usSubCategoryId;                             /* Ð¡ÀàID */
    EGN_UINT16  usVfwId;                                     /* ÐéÄâ·À»ðÇ½ID */

    EGN_UINT16  usEnable:1;                                  /* Ê¹ÄÜ±êÊ¶ */
    EGN_UINT16  usIsTransportSig:1;                          /* ÊÇ·ñ³ÐÔØÓ¦ÓÃ£¬Ä¬ÈÏ£ºNO */

    EGN_UINT16  usRiskValue:5;                               /* ·çÏÕµÈ¼¶ */
    EGN_UINT16  usRiskType:7;                                /* ·çÏÕÀàÐÍ */

    EGN_UINT16  ucStatus:3;                                  /* Ó¦ÓÃ×´Ì¬£¬¼¤»î¡¢Î´¼¤»î */
    EGN_UCHAR   aucReserved1[2];

    EGN_UCHAR   aucName[EGN_NGFW_UD_APP_NAME_LEN_MAX + 1];   /* Ó¦ÓÃÃû */
    EGN_UCHAR   aucDataModel[EGN_NGFW_UD_MODEL_NAME_LEN_MAX + 1];   /* Êý¾ÝÄ£ÐÍ */

    EGN_UCHAR   aucAppDesc[EGN_NGFW_UD_APP_DESC_LEN_MAX + 1]; /* APPÃèÊö */
    EGN_UCHAR   aucReserved2[1];

#ifdef EGN_64
    EGN_UCHAR   aucReserved3[4];
#endif
} EgnNgfwUDApp;

/*STRUCT< NGFW×Ô¶¨ÒåÓ¦ÓÃÐÅÏ¢ >*/
typedef struct _EgnNgfwUDAppS
{
    EgnNgfwUDApp stNotActivedApp;    /* Î´¼¤»îµÄApp */
    EgnNgfwUDApp stActivedApp;       /* ÒÑ¼¤»îµÄApp */
}EgnNgfwUDAppS;

/*STRUCT< NGFW×Ô¶¨Òå¹æÔòÖÐµÄsignatureÐÅÏ¢ >*/
typedef struct _EgnNgfwUDSignature
{
    EGN_UCHAR  ucPatternLen;    /* ×Ö·û´®»òÕýÔò³¤¶È£¬×îÐ¡³¤¶ÈÎªEGN_NGFW_UD_RULE_PATTERN_COND_LEN_MIN, ×î´ó³¤¶ÈÎªEGN_NGFW_UD_RULE_PATTERN_COND_LEN_MAX */
    EGN_UCHAR  ucMode;          /* Ê¶±ðÄ£Ê½£¬°üÊ¶±ð¡¢Á÷Ê¶±ð¡¢Ä£Ê½²»¶¨£¬¼ûEGN_NGFW_INSPECT_MOD_EN */
    EGN_UCHAR  ucDir;           /* Ä£Ê½´®µÄ¼à²â·½Ïò£¬¶ÔÁ÷Ê½Ê¶±ðÓÐÐ§£¬ÇëÇó¡¢ÏìÓ¦¡¢ÇëÇó»òÏìÓ¦£¬¼ûEGN_NGFW_CHECK_DIR_EN */
    EGN_UCHAR  ucPatternType;   /* ×Ö·û´®»¹ÊÇÕýÔò£¬¼ûEGN_NGFW_PATTERN_TYPE_EN */
#ifdef EGN_64
    EGN_UCHAR  aucReserved[4];  /* 8×Ö½Ú¶ÔÆë±£Áô */
#endif
    EGN_UCHAR  aucPattern[EGN_NGFW_UD_RULE_PATTERN_COND_LEN_MAX]; /* Ä£Ê½´®£¬ÒÔucPatternLenÎª³¤¶È */
} EgnNgfwUDSignature;

/*ENUM< Ä³ÌõNGFW×Ô¶¨Òå¹æÔòµÄ´«ÊäÀàÐÍ¶¨Òå >*/
typedef enum
{
    EGN_NGFW_TRANS_TYPE_BEGIN = EGN_EN_INVALID,
    EGN_NGFW_TRANS_TYPE_ANY   = 0X0,    /* Î´Ö¸¶¨ÀàÐÍ£¬TCP»òUDP */
    EGN_NGFW_TRANS_TYPE_TCP   = 0x6,    /* TCP  ÀàÐÍ */
    EGN_NGFW_TRANS_TYPE_UDP   = 0x11,   /* UDP  ÀàÐÍ */
    EGN_NGFW_TRANS_TYPE_END,

    EGN_NGFW_TRANS_TYPE_BOTTOM = EGN_EN_BUTT
}EGN_NGFW_TRANS_TYPE_EN;

/*ENUM< NGFW×Ô¶¨Òå¹æÔòµÄ·çÏÕµÈ¼¶¶¨Òå >*/
typedef enum
{
    EGN_NGFW_APP_RISK_TYPE_VALUE_BEGIN           =  EGN_EN_INVALID,
    EGN_NGFW_APP_RISK_TYPE_VALUE_UNKNOWN         =  0x0000U,
    EGN_NGFW_APP_RISK_TYPE_VALUE_EXPLOITABLE     =  0x0001U,
    EGN_NGFW_APP_RISK_TYPE_VALUE_MAL_VEHICLE     =  0x0002U,
    EGN_NGFW_APP_RISK_TYPE_VALUE_PROD_LOSS       =  0x0004U,
    EGN_NGFW_APP_RISK_TYPE_VALUE_DATA_LEAK       =  0x0008U,
    EGN_NGFW_APP_RISK_TYPE_VALUE_BANDWIDTH       =  0x0010U,
    EGN_NGFW_APP_RISK_TYPE_VALUE_EVASIVE         =  0x0020U,
    EGN_NGFW_APP_RISK_TYPE_VALUE_TUNNELING       =  0x0040U,
    EGN_NGFW_APP_RISK_TYPE_VALUE_END,
    EGN_NGFW_APP_RISK_TYPE_VALUE_BOTTOM          = EGN_EN_BUTT
}EGN_NGFW_APP_RISK_TYPE_VALUE_EN;

/*STRUCT< NGFW×Ô¶¨Òå¹æÔòÐÅÏ¢ >*/
typedef struct _EgnNgfwUDRule
{
    EGN_UCHAR  aucName[EGN_NGFW_UD_RULE_NAME_LEN_MAX + 4];    /* ¹æÔòÃû³Æ£¬ÓÐÐ§³¤¶ÈÎª32£¬ÒÔ\0×÷Îª½áÊø·û */
    EGN_UINT32 ulAppID;           /* Ó¦ÓÃID */
    EGN_UINT32 ulRuleId;          /* ¹æÔòID */
    EGN_UINT16 usVfwId;           /* ÐéÄâ·À»ðÇ½ID */
    EGN_UCHAR  ucTransType;       /* ´«Êä²ãÐ­Òé£¬¶ÔÓ¦EGN_NGFW_TRANS_TYPE_EN */
    EGN_UCHAR  ucDescLen;         /* ¹æÔòÃèÊö³¤¶È£¬×î´ó³¤¶ÈÎªEGN_NGFW_UD_RULE_DESC_LEN_MAX */
    EGN_UCHAR  ucIPNum;           /* IP¸öÊý£¬×î´ó¸öÊýÎªEGN_NGFW_UD_RULE_IP_COND_MAX */
    EGN_UCHAR  ucPortNum;         /* ¶Ë¿Ú¸öÊý£¬×î´ó¸öÊýÎªEGN_NGFW_UD_RULE_PORT_COND_MAX */
    EGN_UCHAR  ucNeedAddPeerList; /* ÊÇ·ñ±ØÒª¹ØÁªÏÂ·¢£¬0²»±ØÒª£¬1±ØÒª*/
    EGN_UCHAR  ucReserved;        /* ×Ö½Ú¶ÔÆë */
#ifdef EGN_64
    EGN_UCHAR  aucReserved2[4];   /* ×Ö½Ú¶ÔÆë */
#endif
    EgnNgfwUDSignature stSignature; /* signatureÐÅÏ¢ */
    EgnIpAddrCond      astIPCond[EGN_NGFW_UD_RULE_IP_COND_MAX];      /* ipÌõ¼þÁÐ±í */
    EgnPortCond        astPortCond[EGN_NGFW_UD_RULE_PORT_COND_MAX];  /* ¶Ë¿ÚÌõ¼þÁÐ±í¡£×¢Òâ:Ö»ÄÜÊÇÄ¿µÄ¶Ë¿Ú */
    EGN_UCHAR          aucDesc[EGN_NGFW_UD_RULE_DESC_LEN_MAX];       /* ÃèÊö£¬ÒÔucDescLenÎª³¤¶È£¬×î´ó³¤¶ÈÎªEGN_NGFW_UD_RULE_DESC_LEN_MAX */
} EgnNgfwUDRule;

/*******************************************************************************
*    Func Name: EgnApiNgfwUDAddApp
*      Purpose: Ìí¼ÓNGFW×Ô¶¨ÒåÓ¦ÓÃ
*  Description: Ìí¼ÓNGFW×Ô¶¨ÒåÓ¦ÓÃ£¬°´ÕÕ½á¹¹Ìå·½Ê½Ìí¼Ó×Ô¶¨ÒåÓ¦ÓÃ£¬´ý¼¤»îºó²ÅÄÜÉúÐ§¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜÌí¼ÓÓ¦ÓÃ¡£
*        Input: NA
*        InOut: EgnNgfwUDApp  *pstNgfwUDApp:×Ô¶¨ÒåÓ¦ÓÃ½á¹¹ÌåÖ¸Õë<·Ç¿Õ>
*       Output: NA
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution: 1¡¢EGNÄÚ²¿Îª×Ô¶¨ÒåÓ¦ÓÃ·ÖÅäID£¬Í¨¹ýÐÞ¸Ä²ÎÊýÀïÃæµÄulAppId´ø³ö£¬AppID·¶Î§:[0x10000000, 0x1FFFFFFF]
                2¡¢Ö§³ÖµÄ×Ô¶¨ÒåÓ¦ÓÃ×î´óÊýÁ¿ 1024
                3¡¢Ö§³ÖµÄ×Ô¶¨ÒåÓ¦ÓÃÃûµÄ×î´ó³¤¶ÈÎª39
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDAddApp
(
   INOUT     EgnNgfwUDApp  *pstNgfwUDApp
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDGetAppState
*      Purpose: ²éÑ¯NGFW×Ô¶¨ÒåÓ¦ÓÃ×´Ì¬
*  Description: ¸ù¾ÝAppId²éÑ¯¶ÔÓ¦×Ô¶¨ÒåÓ¦ÓÃµÄ×´Ì¬¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ²éÑ¯Ó¦ÓÃ×´Ì¬¡£
*        Input: EGN_UINT32   ulAppId:   AppId
*        InOut:
*       Output: EGN_UINT16  *pusStatus: Òª²éÑ¯µÄÓ¦ÓÃµÄ×´Ì¬<·Ç¿Õ>
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution: Ó¦ÓÃ×´Ì¬ÖµµÄº¬ÒåÇë²Î¼ûEGN_NGFW_APP_STATUS_EN
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDGetAppState
(
   IN   EGN_UINT32  ulAppId,
   OUT  EGN_UINT16 *pusStatus
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDGetAppIDByName
*      Purpose: ²éÑ¯×Ô¶¨ÒåÓ¦ÓÃµÄAppid
*  Description: ¸ù¾ÝÓ¦ÓÃÃûÒÔ¼°ÐéÄâ·À»ðÇ½ID²éÑ¯¶ÔÓ¦×Ô¶¨ÒåÓ¦ÓÃµÄAppId¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼²éÑ¯
*        Input: EGN_UCHAR   *pucAppName:   ×Ô¶¨ÒåÓ¦ÓÃÃû
                EGN_UINT16   usVfwId:      ÐéÄâ·À»ðÇ½ID
*        InOut: EGN_UINT32  *pulAppId:     ²éÑ¯½á¹û:AppId
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDGetAppIDByName
(
    IN    EGN_UCHAR          *pucAppName,      /* ×Ô¶¨ÒåÓ¦ÓÃÃû  */
    IN    EGN_UINT16          usVfwId,         /* ÐéÄâ·À»ðÇ½ID  */
    INOUT EGN_UINT32         *pulAppId         /* ²éÑ¯½á¹ûAppId */
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDGetCommitedAppIDByName
*      Purpose: ²éÑ¯ÒÑÉúÐ§µÄ×Ô¶¨ÒåÓ¦ÓÃµÄAppid
*  Description: ¸ù¾ÝÓ¦ÓÃÃûÒÔ¼°ÐéÄâ·À»ðÇ½ID²éÑ¯ÒÑÉúÐ§µÄ×Ô¶¨ÒåÓ¦ÓÃµÄAppId¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼²éÑ¯¡£
*        Input:   EGN_UCHAR   *pucAppName:   ×Ô¶¨ÒåÓ¦ÓÃÃû<·Ç¿Õ>
                  EGN_UINT16   usVfwId:      ÐéÄâ·À»ðÇ½ID
*        InOut:   EGN_UINT32  *pulAppId:     ²éÑ¯½á¹û:AppId<·Ç¿Õ>
*       Output:
*       Return:   EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDGetCommitedAppIDByName
(
    IN    EGN_UCHAR          *pucAppName,      /* ×Ô¶¨ÒåÓ¦ÓÃÃû  */
    IN    EGN_UINT16          usVfwId,         /* ÐéÄâ·À»ðÇ½ID  */
    INOUT EGN_UINT32         *pulAppId         /* ²éÑ¯½á¹ûAppId */
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDGetAppCount
*      Purpose: ²éÑ¯×Ô¶¨ÒåÓ¦ÓÃÊýÁ¿
*  Description: ¸ù¾ÝÐéÄâ·À»ðÇ½ID²éÑ¯Î´¼¤»îµÄ×Ô¶¨ÒåÓ¦ÓÃÊýÁ¿ÒÔ¼°ÒÑ¼¤»îµÄ×Ô¶¨ÒåÓ¦ÓÃÊýÁ¿¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼²éÑ¯¡£
*        Input: EGN_UINT16   usVfwId:              ÐéÄâ·À»ðÇ½ID
*       Output: EGN_UINT32  *pulAppNum:            ËùÓÐAppÊýÁ¿<·Ç¿Õ>
                EGN_UINT32  *pulNotActivedAppNum:  Î´¼¤»îµÄAppÊýÁ¿<·Ç¿Õ>
                EGN_UINT32  *pulActivedAppNum:     ÒÑ¾­¼¤»îµÄAppÊýÁ¿<·Ç¿Õ>
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDGetAppCount
(
    IN   EGN_UINT16          usVfwId,               /* ÐéÄâ·À»ðÇ½ID      */
    OUT  EGN_UINT32         *pulAppNum,             /* ËùÓÐAppÊýÁ¿       */
    OUT  EGN_UINT32         *pulNotActivedAppNum,   /* Î´¼¤»îµÄAppÊýÁ¿   */
    OUT  EGN_UINT32         *pulActivedAppNum       /* ÒÑ¾­¼¤»îµÄAppÊýÁ¿ */
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDGetAppInfoByID
*      Purpose: Í¨¹ýAppId²éÕÒ×Ô¶¨ÒåAppÐÅÏ¢
*  Description: Í¨¹ýAppId²éÕÒ×Ô¶¨ÒåAppÐÅÏ¢
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNGFWUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼²éÑ¯¡£
*        Input: EGN_UINT32    ulAppId:       AppId
*       Output: EgnNgfwUDApp  *pstAppInfo:    AppÐÅÏ¢<·Ç¿Õ>
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDGetAppInfoByID
(
    IN     EGN_UINT32         ulAppId,      /* AppId   */
    INOUT  EgnNgfwUDApp      *pstAppInfo    /* AppÐÅÏ¢ */
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDGetCommitedAppInfoByID
*      Purpose: Í¨¹ýAppId²éÕÒ¼¤»î×´Ì¬ÏÂµÄ×Ô¶¨ÒåAppÐÅÏ¢
*  Description: Í¨¹ýAppId²éÕÒ¼¤»î×´Ì¬ÏÂµÄ×Ô¶¨ÒåAppÐÅÏ¢¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNGFWUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼²éÑ¯¡£
*        Input: EGN_UINT32    ulAppId:        AppId
*       Output: EgnNgfwUDApp  *pstAppInfo:    AppÐÅÏ¢<·Ç¿Õ>
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDGetCommitedAppInfoByID
(
    IN     EGN_UINT32         ulAppId,      /* AppId */
    INOUT  EgnNgfwUDApp      *pstAppInfo    /* AppÐÅÏ¢ */
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDGetAppInfoByName
*      Purpose: Í¨¹ýÓ¦ÓÃÃûÒÔ¼°·À»ðÇ½IDÀ´²éÕÒ×Ô¶¨ÒåAppÐÅÏ¢
*  Description: Í¨¹ýÓ¦ÓÃÃûÒÔ¼°·À»ðÇ½IDÀ´²éÕÒ×Ô¶¨ÒåAppÐÅÏ¢¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼²éÑ¯¡£
*        Input: EGN_UCHAR    *pucAppName  :    ×Ô¶¨ÒåÓ¦ÓÃÃû<·Ç¿Õ>
                EGN_UINT16    usVfwId     :    ÐéÄâ·À»ðÇ½ID
*        InOut: EgnNgfwUDApp *pstAppInfo  :    AppÐÅÏ¢<·Ç¿Õ>
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDGetAppInfoByName
(
    IN     EGN_UCHAR     *pucAppName,  /* ×Ô¶¨ÒåÓ¦ÓÃÃû */
    IN     EGN_UINT16     usVfwId,     /* ÐéÄâ·À»ðÇ½ID */
    INOUT  EgnNgfwUDApp  *pstAppInfo   /* AppÐÅÏ¢      */
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDGetAllAppInfo
*      Purpose: ²éÕÒÍ¬Ò»·À»ðÇ½ÏÂµÄËùÓÐ×Ô¶¨ÒåÓ¦ÓÃÐÅÏ¢
*  Description: ²éÕÒÍ¬Ò»·À»ðÇ½ÏÂµÄËùÓÐ×Ô¶¨ÒåÓ¦ÓÃÐÅÏ¢£¬·µ»Ø½á¹ûÓÐÎ´¼¤»îµÄAppÐÅÏ¢»òÕßÒÑ¼¤»îµÄAppÐÅÏ¢¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼²éÑ¯¡£
*        Input: EGN_UINT16                     usVfwId             :  ÐéÄâ·À»ðÇ½ID
                EGN_UINT32                     ulQueryFlag         :  ²éÑ¯Ä£Ê½<EGN_NGFW_APP_QUERY_MODEL_EN>
*        InOut: EgnNgfwUDAppS                 *pstUserDefineAppS   :  AppÐÅÏ¢Êý×é<·Ç¿Õ>
                EGN_UINT32                    *pulItemNum          :  AppÐÅÏ¢Êý×é´óÐ¡<·Ç¿Õ>
                EGN_UINT32                    *pulNotActivedAppNum :  Î´¼¤»îµÄAppÊýÁ¿<·Ç¿Õ>
                EGN_UINT32                    *pulActivedAppNum    :  ÒÑ¾­¼¤»îµÄAppÊýÁ¿<·Ç¿Õ>
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDGetAllAppInfo
(
    IN    EGN_UINT16      usVfwId,              /* ÐéÄâ·À»ðÇ½ID */
    IN    EGN_UINT32      ulQueryFlag,          /* ²éÑ¯Ä£Ê½£¬ÊÇ²é¼¤»îµÄ»òÕßÎ´¼¤»îµÄ»òÕßÁ½Õß¶¼²é */
    INOUT EgnNgfwUDAppS  *pstUserDefineAppS,    /* AppÐÅÏ¢Êý×é */
    INOUT EGN_UINT32     *pulItemNum,           /* AppÐÅÏ¢Êý×é´óÐ¡ */
    INOUT EGN_UINT32     *pulNotActivedAppNum,  /* Î´¼¤»îµÄAppÊýÁ¿  */
    INOUT EGN_UINT32     *pulActivedAppNum      /* ÒÑ¾­¼¤»îµÄAppÊýÁ¿ */
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDModifyApp
*      Purpose: ÐÞ¸Ä×Ô¶¨ÒåÓ¦ÓÃµÄÐÅÏ¢(Ó¦ÓÃÃû³ýÍâ)
*  Description: ÐÞ¸ÄÍêºó£¬Òª¼¤»î²ÅÄÜÉúÐ§
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼ÐÞ¸Ä¡£
*        Input: EgnNgfwUDApp    *pstNgfwUDApp       :      ÒªÐÞ¸ÄµÄÓ¦ÓÃ<·Ç¿Õ>
                EGN_UINT32      ulModifyAppType     :      ÐÞ¸ÄÓ¦ÓÃµÄ×Ö¶Î<¶ÔÓ¦EGN_NGFW_APP_ATTR_TYPE_ENÃ¶¾Ù>
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDModifyApp
(
    IN    EgnNgfwUDApp   *pstNgfwUDApp,
    IN    EGN_UINT32      ulModifyAppType
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDModifyAppName
*      Purpose: ÐÞ¸Ä×Ô¶¨ÒåÓ¦ÓÃÃû
*  Description: ÐÞ¸Ä×Ô¶¨ÒåÓ¦ÓÃÃû£¬ÐÞ¸ÄÍêºó£¬Òª¼¤»î²ÅÄÜÉúÐ§¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼ÐÞ¸Ä¡£
*        Input: EGN_UINT32    ulAppId          :    ÒªÐÞ¸ÄµÄÓ¦ÓÃµÄID
                EGN_UCHAR    *pucNewAppName    :    Ó¦ÓÃÃû<·Ç¿Õ>
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDModifyAppName
(
    IN     EGN_UINT32 ulAppId,         /* AppId */
    IN     EGN_UCHAR *pucNewAppName    /* ÐÂµÄ×Ô¶¨ÒåÓ¦ÓÃÃû  */
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDDelAllApps
*      Purpose: É¾³ýÍ¬Ò»·À»ðÇ½IDÏÂµÄËùÓÐ×Ô¶¨ÒåÓ¦ÓÃ
*  Description: É¾³ýÍ¬Ò»·À»ðÇ½IDÏÂµÄËùÓÐ×Ô¶¨ÒåÓ¦ÓÃ£¬É¾³ýºó£¬Òª¼¤»î²ÅÄÜÉúÐ§¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼É¾³ý¡£
*        Input: EGN_UINT16      usVfwId : ÐéÄâ·À»ðÇ½ID
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDDelAllApps
(
    IN     EGN_UINT16 usVfwId   /* ÐéÄâ·À»ðÇ½ID  */
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDDeleteApp
*      Purpose: É¾³ýÒ»¸ö×Ô¶¨ÒåÓ¦ÓÃ»òÕßÉ¾³ýÒ»¸ö×Ô¶¨ÒåÓ¦ÓÃµÄÄ³¸ö×Ö¶Î(ÐÞ¸ÄÎªÄ¬ÈÏÖµ)
*  Description: É¾³ýÒ»¸ö×Ô¶¨ÒåÓ¦ÓÃ»òÕßÉ¾³ýÒ»¸ö×Ô¶¨ÒåÓ¦ÓÃµÄÄ³¸ö×Ö¶Î£¬É¾³ýºó£¬Òª¼¤»î²ÅÄÜÉúÐ§¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼É¾³ý¡£
*        Input: EgnNgfwUDApp   *pstNgfwUDApp        :      ÒªÉ¾³ýµÄÓ¦ÓÃ<·Ç¿Õ>
                EGN_UINT32      ulDelAttrAppType    :      ÒªÉ¾³ýÓ¦ÓÃµÄ×Ö¶Î<¶ÔÓ¦EGN_NGFW_APP_ATTR_TYPE_ENÃ¶¾Ù>
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDDeleteApp
(
    IN EGN_CONST EgnNgfwUDApp   *pstNgfwUDApp,
    IN           EGN_UINT32      ulDelAttrAppType
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDMngIncAppRefCount
*      Purpose: Ôö¼Ó×Ô¶¨ÒåÓ¦ÓÃµÄÒýÓÃ¼ÆÊý(¹ÜÀíÃæÊ¹ÓÃ)
*  Description: Ôö¼Ó×Ô¶¨ÒåÓ¦ÓÃµÄÒýÓÃ¼ÆÊý¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼ÐÞ¸Ä¡£
*        Input: EGN_UCHAR   *pucModuleName  : µ÷ÓÃÄ£¿éÃû<·Ç¿Õ>
                EGN_UINT32   ulAppId        : AppId
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDMngIncAppRefCount
(
    IN EGN_CONST EGN_UCHAR     *pucModuleName,    /* µ÷ÓÃÄ£¿éÃû */
    IN           EGN_UINT32     ulAppId           /* AppId */
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDNgeIncAppRefCount
*      Purpose: Ôö¼Ó×Ô¶¨ÒåÓ¦ÓÃµÄÒýÓÃ¼ÆÊý(NGEÆ½ÃæÊ¹ÓÃ)
*  Description: Ôö¼Ó×Ô¶¨ÒåÓ¦ÓÃµÄÒýÓÃ¼ÆÊý¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼ÐÞ¸Ä
*        Input: EGN_UCHAR   *pucModuleName  : µ÷ÓÃÄ£¿éÃû<·Ç¿Õ>
                EGN_UINT32   ulAppId        : AppId
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDNgeIncAppRefCount
(
    IN EGN_CONST EGN_UCHAR  *pucModuleName,    /* µ÷ÓÃÄ£¿éÃû */
    IN           EGN_UINT32  ulAppId           /* AppId */
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDMngDecAppRefCount
*      Purpose: ¼õÉÙ×Ô¶¨ÒåÓ¦ÓÃµÄÒýÓÃ¼ÆÊý(¹ÜÀíÆ½ÃæÊ¹ÓÃ)
*  Description: ¼õÉÙ×Ô¶¨ÒåÓ¦ÓÃµÄÒýÓÃ¼ÆÊý¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼ÐÞ¸Ä
*        Input: EGN_UCHAR   *pucModuleName :  µ÷ÓÃÄ£¿éÃû<·Ç¿Õ>
                EGN_UINT32  ulAppId        :  AppId
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDMngDecAppRefCount
(
    IN EGN_CONST EGN_UCHAR  *pucModuleName,    /* µ÷ÓÃÄ£¿éÃû */
    IN           EGN_UINT32  ulAppId           /* AppId */
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDNgeDecAppRefCount
*      Purpose: ¼õÉÙ×Ô¶¨ÒåÓ¦ÓÃµÄÒýÓÃ¼ÆÊý(NgeÆ½ÃæÊ¹ÓÃ)
*  Description: ¼õÉÙ×Ô¶¨ÒåÓ¦ÓÃµÄÒýÓÃ¼ÆÊý¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼ÐÞ¸Ä
*        Input: EGN_UCHAR   *pucModuleName  : µ÷ÓÃÄ£¿éÃû<·Ç¿Õ>
                EGN_UINT32  ulAppId         : AppId
*       Output:
*       Return:   EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDNgeDecAppRefCount
(
    IN EGN_CONST EGN_UCHAR     *pucModuleName,    /* µ÷ÓÃÄ£¿éÃû */
    IN           EGN_UINT32     ulAppId           /* AppId */
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDNeedCommit
*      Purpose: ÅÐ¶Ï·À»ðÇ½ÏÂÊÇ·ñÓÐÐèÒªÌá½»µÄ×Ô¶¨ÒåÓ¦ÓÃ
*  Description: ¸ù¾ÝÐéÄâ·À»ðÇ½ID£¬ÅÐ¶Ï¸Ã·À»ðÇ½ÏÂÊÇ·ñÓÐÐèÒªÌá½»µÄ×Ô¶¨ÒåÓ¦ÓÃ¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼²éÑ¯¡£
*        Input: EGN_UINT16   usVfwId      : ÐéÄâ·À»ðÇ½ID
*        InOut: EGN_BOOL    *bIsNeedCommit: ÊÇ·ñÐèÒªÌá½» <1:ÐèÒªÌá½»
                                                          0:²»ÐèÒªÌá½»>
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDNeedCommit
(
    IN    EGN_UINT16  usVfwId,        /* ÐéÄâ·À»ðÇ½ID */
    INOUT EGN_BOOL   *pbIsNeedCommit  /* ÊÇ·ñÐèÒªÌá½» */
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDNeedReCompile
*      Purpose: ÅÐ¶ÏËùÓÐ·À»ðÇ½ÏÂÊÇ·ñÓÐÐèÒªÖØÐÂ±àÒëµÄ×Ô¶¨ÒåÓ¦ÓÃ
*  Description: ÅÐ¶ÏËùÓÐ·À»ðÇ½ÏÂÊÇ·ñÓÐÐèÒªÖØÐÂ±àÒëµÄ×Ô¶¨ÒåÓ¦ÓÃ,µ±ÓÐÐÂµÄ×Ô¶¨ÒåÓ¦ÓÃÐèÒª¼¤»îÊ±£¬ÐèÒªÖØÐÂ±àÒë¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼²éÑ¯¡£
*        InOut: EGN_BOOL  *pbIsNeedReCompile       ÊÇ·ñÐèÒª±àÒë<·Ç¿Õ>
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDNeedReCompile
(
    INOUT EGN_BOOL *pbIsNeedReCompile  /* ÊÇ·ñÐèÒª±àÒë */
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDSetCompileFlag
*      Purpose: ÉèÖÃÐéÄâ·À»ðÇ½ÊÇ·ñÌá½»±àÒë±êÖ¾
*  Description: ÉèÖÃÐéÄâ·À»ðÇ½ÊÇ·ñÌá½»±àÒë±êÖ¾¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜ¿ªÊ¼ÐÞ¸Ä
*        Input: EGN_UCHAR  *paucCompFlagArray : ±àÒë±ê¼ÇÊý×é<·Ç¿Õ>
                EGN_UINT32  ulArraySize       : Êý×é´óÐ¡
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDSetCompileFlag
(
    IN  EGN_UCHAR  *paucCompFlagArray,  /* ±àÒë±ê¼ÇÊý×é */
    IN  EGN_UINT32  ulArraySize         /* Êý×é´óÐ¡ */
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDAddAppForceID
*      Purpose: Ìí¼Ó×Ô¶¨ÒåÓ¦ÓÃÐÅÏ¢£¬Ç¿ÖÆÖ¸¶¨Ó¦ÓÃID
*  Description: °´ÕÕ½á¹¹Ìå·½Ê½Ìí¼Ó×Ô¶¨ÒåÓ¦ÓÃ£¬´ý¼¤»îºó²ÅÄÜÉúÐ§¡£¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜÌí¼ÓÓ¦ÓÃ
                    ¸Ã½Ó¿Ú½ö¹©ÏµÍ³ÖØÆô¹ý³ÌÖÐÊ¹ÓÃ¡£
*        Input: EgnNgfwUDApp  *pstNgfwUDApp:×Ô¶¨ÒåÓ¦ÓÃ½á¹¹ÌåÖ¸Õë<·Ç¿Õ>
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution: 1¡¢AppID·¶Î§:[0x10000000, 0x1FFFFFFF]
                2¡¢Ö§³ÖµÄ×Ô¶¨ÒåÓ¦ÓÃ×î´óÊýÁ¿ 1024
                3¡¢Ö§³ÖµÄ×Ô¶¨ÒåÓ¦ÓÃÃûµÄ×î´ó³¤¶ÈÎª39
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDAddAppForceID
(
   IN EgnNgfwUDApp *pstNgfwUDApp
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDNgeActiveAllApps
*      Purpose: ¼¤»îËùÓÐ×Ô¶¨ÒåÓ¦ÓÃ( ½öNgeÃæÊ¹ÓÃ)
*  Description: ¼¤»îËùÓÐ×Ô¶¨ÒåÓ¦ÓÃ¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜÌí¼ÓÓ¦ÓÃ¡£
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDNgeActiveAllApps
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDMngActiveAllApps
*      Purpose: ¼¤»îËùÓÐ×Ô¶¨ÒåÓ¦ÓÃ( ½ö¹©¹ÜÀíÃæÊ¹ÓÃ)
*  Description: ¼¤»îËùÓÐ×Ô¶¨ÒåÓ¦ÓÃ
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜÌí¼ÓÓ¦ÓÃ¡£
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDMngActiveAllApps
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDNgeActiveRule
*      Purpose: ¼¤»îËùÓÐ×Ô¶¨ÒåÓ¦ÓÃÏÂµÄ¹æÔò(NgeÆ½ÃæÊ¹ÓÃ)
*  Description: ¼¤»îËùÓÐ×Ô¶¨ÒåÓ¦ÓÃÏÂµÄ¹æÔò¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜÌí¼ÓÓ¦ÓÃ¡£
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDNgeActiveRule
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDMngActiveRule
*      Purpose: ¼¤»îËùÓÐ×Ô¶¨ÒåÓ¦ÓÃÏÂµÄ¹æÔò(¹ÜÀíÆ½ÃæÊ¹ÓÃ)
*  Description: ¼¤»îËùÓÐ×Ô¶¨ÒåÓ¦ÓÃÏÂµÄ¹æÔò¡£
                ¸Ã½Ó¿Ú±ØÐëÔÚ³õÊ¼»¯Ê±¿ªÆôNGFW×Ô¶¨Òå¹¦ÄÜ¿ª¹Ø
                    £¨ÉèÖÃEgnInitCfgParam.bNgfwUserDefineSwitchÎªEGN_TRUE£©£¬ÇÒ³õÊ¼»¯³É¹¦ºó£¬²ÅÄÜÌí¼ÓÓ¦ÓÃ¡£
*        InOut: NA
*       Output: NA
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDMngActiveRule
(
    EGN_VOID
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDAddRule
*      Purpose: Ìí¼Ó×Ô¶¨Òå¹æÔò
*  Description: ¸ù¾Ý×Ô¶¨ÒåÓ¦ÓÃID£¬Ìí¼Ó×Ô¶¨Òå¹æÔò¡£
                ÒýÇæ³õÊ¼»¯Íê³É£¬ÇÒÖªÊ¶¿âÃ÷ÎÄÐÅÏ¢µ¼Èëºó£¬²ÅÄÜµ÷ÓÃ¡£
*        Input: EGN_UINT32      ulAppId        :  ´ýÌí¼ÓµÄ×Ô¶¨Òå¹æÔò¶ÔÓ¦µÄ×Ô¶¨ÒåÓ¦ÓÃ
                EgnNgfwUDRule  *pstUdRule      :  ´ýÌí¼ÓµÄ×Ô¶¨Òå¹æÔò<·Ç¿Õ>
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDAddRule
(
    IN   EGN_UINT32     ulAppId,
    IN   EgnNgfwUDRule *pstUdRule
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDModifyRule
*      Purpose: ÐÞ¸Ä×Ô¶¨Òå¹æÔò
*  Description: ¸ù¾Ý×Ô¶¨ÒåÓ¦ÓÃID¡¢×Ô¶¨Òå¹æÔò¡¢ÐÞ¸ÄÀàÐÍ£¬ÐÞ¸ÄÏàÓ¦µÄ×Ô¶¨Òå¹æÔò¡£
                ÒýÇæ³õÊ¼»¯Íê³É£¬ÇÒÖªÊ¶¿âÃ÷ÎÄÐÅÏ¢µ¼Èëºó£¬²ÅÄÜµ÷ÓÃ¡£
*        Input: EGN_UINT32     ulAppId      : ´ýÐÞ¸ÄµÄ×Ô¶¨Òå¹æÔò¶ÔÓ¦µÄ×Ô¶¨ÒåÓ¦ÓÃ
                EgnNgfwUDRule  *pstUdRule   : ´ýÐÞ¸ÄµÄ×Ô¶¨Òå¹æÔò<·Ç¿Õ>
                EGN_UINT32     ulModifyType : ÐÞ¸ÄµÄÀàÐÍ<EGN_NGFW_UD_RULE_CONTENT_TYPE_EN>
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDModifyRule
(
    IN   EGN_UINT32     ulAppId,
    IN   EgnNgfwUDRule *pstUdRule,
    IN   EGN_UINT32     ulModifyType
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDModifyRuleName
*      Purpose: ÐÞ¸Ä¹æÔòÃû×Ö
*  Description: ¸ù¾Ý×Ô¶¨ÒåÓ¦ÓÃID¡¢×Ô¶¨Òå¹æÔòµÄÃû×Ö£¬ÐÞ¸Ä×Ô¶¨Òå¹æÔòÃûÎªÐÂµÄÃû×Ö¡£
                ÒýÇæ³õÊ¼»¯Íê³É£¬ÇÒÖªÊ¶¿âÃ÷ÎÄÐÅÏ¢µ¼Èëºó£¬²ÅÄÜµ÷ÓÃ¡£
*        Input: EGN_UINT32     ulAppId    : ×Ô¶¨ÒåÓ¦ÓÃID
                EGN_UCHAR     *pucOldName : ´ýÐÞ¸ÄµÄ×Ô¶¨Òå¹æÔòµÄÃû×Ö<·Ç¿Õ>
                EGN_UCHAR     *pucNewName : ÐÞ¸ÄºóµÄ×Ô¶¨Òå¹æÔòµÄÃû×Ö<·Ç¿Õ>
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDModifyRuleName
(
    IN   EGN_UINT32     ulAppId,
    IN   EGN_UCHAR     *pucOldName,
    IN   EGN_UCHAR     *pucNewName
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDDelAllRule
*      Purpose: É¾³ýËùÓÐ×Ô¶¨Òå¹æÔò
*  Description: ¸ù¾Ý×Ô¶¨ÒåÓ¦ÓÃID£¬É¾³ýÆä¶ÔÓ¦µÄËùÓÐ×Ô¶¨Òå¹æÔò¡£
                ÒýÇæ³õÊ¼»¯Íê³É£¬ÇÒÖªÊ¶¿âÃ÷ÎÄÐÅÏ¢µ¼Èëºó£¬²ÅÄÜµ÷ÓÃ¡£
*        Input: EGN_UINT32     ulAppId   :  ×Ô¶¨ÒåÓ¦ÓÃID
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDDelAllRule
(
    IN   EGN_UINT32     ulAppId
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDDelOneRule
*      Purpose: É¾³ý×Ô¶¨Òå¹æÔò
*  Description: ¸ù¾Ý×Ô¶¨ÒåÓ¦ÓÃID£¬×Ô¶¨Òå¹æÔò£¬¸ù¾ÝÉ¾³ýÀàÐÍ£¬É¾³ýÆä¶ÔÓ¦µÄ×Ô¶¨Òå¹æÔò¡£
                ÒýÇæ³õÊ¼»¯Íê³É£¬ÇÒÖªÊ¶¿âÃ÷ÎÄÐÅÏ¢µ¼Èëºó£¬²ÅÄÜµ÷ÓÃ
*        Input: EGN_UINT32      ulAppId        :  ´ýÉ¾³ýµÄ×Ô¶¨Òå¹æÔò¶ÔÓ¦µÄ×Ô¶¨ÒåÓ¦ÓÃID
*               EgnNgfwUDRule  *pstUdRule      :  ´ýÉ¾³ýµÄ×Ô¶¨Òå¹æÔò<·Ç¿Õ>
*               EGN_UINT32      ulDeleteType   :  É¾³ýµÄÀàÐÍ<·Ç¿Õ>
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDDelOneRule
(
    IN   EGN_UINT32     ulAppId,
    IN   EgnNgfwUDRule *pstUdRule,
    IN   EGN_UINT32     ulDeleteType
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDGetRuleNum
*      Purpose: ²éÕÒ¹æÔò¸öÊý
*  Description: ¸ù¾Ý×Ô¶¨ÒåÓ¦ÓÃID£¬²éÕÒ¶ÔÓ¦µÄ¹æÔò¸öÊý¡£
                ÒýÇæ³õÊ¼»¯Íê³É£¬ÇÒÖªÊ¶¿âÃ÷ÎÄÐÅÏ¢µ¼Èëºó£¬²ÅÄÜµ÷ÓÃ¡£
*        Input: EGN_UINT32     ulAppId       :    ´ý²éÑ¯µÄ×Ô¶¨ÒåÓ¦ÓÃID
*       Output: EGN_UINT32    *pulRuleNum    :    ×Ô¶¨Òå¹æÔò¸öÊý<·Ç¿Õ>
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDGetRuleNum
(
    IN   EGN_UINT32     ulAppId,
    OUT  EGN_UINT32    *pulRuleNum
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDGetRuleInfo
*      Purpose: ²éÕÒ¹æÔòÐÅÏ¢ºÍ¸öÊý
*  Description: ¸ù¾Ý×Ô¶¨ÒåÓ¦ÓÃID£¬²éÕÒ¶ÔÓ¦µÄ¹æÔòÐÅÏ¢ºÍ¸öÊý¡£
                ÒýÇæ³õÊ¼»¯Íê³É£¬ÇÒÖªÊ¶¿âÃ÷ÎÄÐÅÏ¢µ¼Èëºó£¬²ÅÄÜµ÷ÓÃ¡£
*        Input: EGN_UINT32     ulAppId         :  ´ý²éÑ¯µÄ×Ô¶¨ÒåÓ¦ÓÃID
*        Inout: EGN_UINT32    *pulRuleNum      :  ×Ô¶¨Òå¹æÔòÐÅÏ¢Êý×é<·Ç¿Õ>
*               EgnNgfwUDRule *pstUdRuleArray  :  ×Ô¶¨Òå¹æÔò¸öÊý<·Ç¿Õ>
*       Output:
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDGetRuleInfo
(
    IN     EGN_UINT32     ulAppId,
    INOUT  EGN_UINT32    *pulRuleNum,
    INOUT  EgnNgfwUDRule *pstUdRuleArray
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDGetOneRule
*      Purpose: ²éÕÒ¹æÔòÐÅÏ¢
*  Description: ¸ù¾Ý×Ô¶¨ÒåÓ¦ÓÃIDºÍ¹æÔòÃû£¬²éÕÒ¶ÔÓ¦µÄ¹æÔòÐÅÏ¢¡£
                ÒýÇæ³õÊ¼»¯Íê³É£¬ÇÒÖªÊ¶¿âÃ÷ÎÄÐÅÏ¢µ¼Èëºó£¬²ÅÄÜµ÷ÓÃ¡£
*        Input: EGN_UINT32      ulAppId       :   ´ý²éÑ¯µÄ×Ô¶¨ÒåÓ¦ÓÃID
                EGN_UCHAR      *pucRuleName   :   ×Ô¶¨Òå¹æÔòÃû<·Ç¿Õ>
*       Output: EgnNgfwUDRule  *pstUdRule     :   ×Ô¶¨Òå¹æÔòÐÅÏ¢<·Ç¿Õ>
*       Return: EGN_UINT32£¬³É¹¦»òÕßÊ§°ÜµÄ´íÎóÂë£¬¾ßÌåÇë²Î¼ûEGN_RET_RESULT_EN¡£
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT32 EgnApiNgfwUDGetOneRule
(
    IN   EGN_UINT32     ulAppId,
    IN   EGN_UCHAR     *pucRuleName,
    OUT  EgnNgfwUDRule *pstUdRule
);

/*******************************************************************************
*    Func Name: EgnApiNgfwUDGetRiskValue
*      Purpose: ¸ù¾ÝÓ¦ÓÃµÄ·çÏÕ£¬¼ÆËãÆä·çÏÕ¼¶±ð¡£
*  Description: ¸ÃAPI½Ó¿Ú»á¸ù¾Ý·çÏÕÀà±ð¸öÊýÀ´¼ÆËã³ö¶ÔÓ¦µÄ·çÏÕµÈ¼¶£¬
                   ·çÏÕÀàÐÍ¸öÊýÄ¿Ç°×î¶àÊÇ7¸ö£¬¼´ÓÃ»§×î¶à¿ÉÒÔÉèÖÃ7¸ö·çÏÕÀà±ð¡£
*               ¼ÆËãÔ­ÔòÈçÏÂ:
*               ·çÏÕÀàÐÍ¸öÊý     ·çÏÕµÈ¼¶
*               6 or 7       ->     5
*               5            ->     4
*               4 or 3       ->     3
*               2            ->     2
*               1 or 0       ->     1
*        Input: EGN_UINT16      usRiskType  :    ·çÏÕÀàÐÍ£¬µÍ7Î»ÓÐÐ§£¬ÓÃµÍ7Î»À´±êÖ¾7ÖÖ²»Í¬µÄ·çÏÕÀàÐÍ
*       Output:
*       Return: EGN_UINT16      ¶ÔÓ¦µÄ·çÏÕ¼¶±ð
*      Caution:
*        Since: V300R006C00SPC200
*    Reference: ÎÞ
*------------------------------------------------------------------------------
*  Modification History
*  DATE         NAME                    DESCRIPTION
*------------------------------------------------------------------------------
*
*******************************************************************************/
EGN_UINT16 EgnApiNgfwUDGetRiskValue
(
    IN EGN_UINT16 usRiskType
);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EGN_API_USER_DEFINE_RULE_H__ */

