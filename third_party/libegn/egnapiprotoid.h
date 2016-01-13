/*
******************************************************************************
版权所有 (C), 2008-2009, 华为技术有限公司
******************************************************************************
文 件 名   : egnapiprotoid.h
版 本 号   : 初稿
作    者   : EGN_TEAM
生成日期   : 2009年05月22日
最近修改   :
功能描述   : EGN 协议ID
函数列表   :
修改历史   :
1.日    期   : 2009年05月22日
作    者   : EGN_TEAM
修改内容   : 创建文件

******************************************************************************/
/**@file  egnapiprotoid.h
  *    EGN协议ID头文件
*******************************************************/

#ifndef __EGN_PROTO_ID_H__
#define __EGN_PROTO_ID_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define EGN_CLASS_ID_MASK      (0xFF000000)                         /* ClassId掩码 */

/*ENUM< 协议ClassID定义 >*/
typedef enum _EGN_CLASS_ID_EN
{
    EGN_CLASS_ID_BEGIN                      = EGN_EN_INVALID,
    EGN_CLASS_ID_P2P                        = 0x01,                 /* P2P传输类 */
    EGN_CLASS_ID_IM                         = 0x02,                 /* 即时通讯类 */
    EGN_CLASS_ID_VOIP                       = 0x03,                 /* VOIP类 */
    EGN_CLASS_ID_WEB_BROWSING               = 0x04,                 /* 网络浏览类 */
    EGN_CLASS_ID_GAME                       = 0x05,                 /* 游戏类 */
    EGN_CLASS_ID_STREAMING                  = 0x06,                 /* STREAMING类 */
    EGN_CLASS_ID_ATTACK                     = 0x07,                 /* 攻击类 */
    EGN_CLASS_ID_DATABASE                   = 0x08,                 /* 数据库类 */
    EGN_CLASS_ID_EMAIL                      = 0x09,                 /* 电子邮件类 */
    EGN_CLASS_ID_FILE_ACCESS                = 0x0A,                 /* 文件读取类 */
    EGN_CLASS_ID_NETWORK_ADMIN              = 0x0B,                 /* 网络管理类 */
    EGN_CLASS_ID_NETWORK_STORAGE            = 0x0C,                 /* 网络存储类 */
    EGN_CLASS_ID_NEWS_GROUP                 = 0x0D,                 /* 新闻群组类 */
    EGN_CLASS_ID_REMOTE_CONNECTIVITY        = 0x0E,                 /* 远程连接类 */
    EGN_CLASS_ID_STOCK                      = 0x0F,                 /* 股票类 */
    EGN_CLASS_ID_TUNNELLING                 = 0x10,                 /* 通道类 */
    EGN_CLASS_ID_OTHER                      = 0x11,                 /* 其他类 */
    EGN_CLASS_ID_END,

    EGN_CLASS_ID_BOTTOM = EGN_EN_BUTT
}EGN_CLASS_ID_EN;

/*ENUM< 协议AppID定义 >*/
typedef enum _EGN_APP_ID_EN
{
    EGN_APP_ID_BEGIN                      = EGN_EN_INVALID,
    EGN_APP_ID_END,

    EGN_APP_ID_BOTTOM = EGN_EN_BUTT

}EGN_APP_ID_EN;

/*ENUM< 协议ProtoID定义 >*/
typedef enum _EGN_PROTO_ID_EN
{
    EGN_PROTO_ID_BEGIN                      = EGN_EN_INVALID,

    EGN_PROTO_ID_INVALID                    = 0x00000000,

    /** DNS相关协议ID */
    EGN_PROTO_ID_DNS                        = 0x0000004F,       /* DNS协议ID */
    EGN_PROTO_ID_MDNS                       = 0x00000108,       /* MDNS协议ID */

    /** RTP相关协议ID */
    EGN_PROTO_ID_RTP                        = 0x000000F9,       /* RTP协议ID */
    EGN_PROTO_ID_RTCP                       = 0x000000FA,       /* RTCP协议ID */

    /** UUCall相关协议ID */
    EGN_PROTO_ID_UUCALL_PCCALL              = 0x000000AE,       /* UUCall协议PCCALL ID */
    EGN_PROTO_ID_UUCALL_TRANSFER            = 0x000000AD,       /* UUCall协议TRANSFER ID */

    /** GoogleTalk相关协议ID */
    EGN_PROTO_ID_GTALK_CONTROL              = 0x00000028,       /* GoogleTalk协议控制流ID */
    EGN_PROTO_ID_GTALK_DATA                 = 0x00000029,       /* GoogleTalk协议数据流ID */

    /** BT相关协议ID */
    EGN_PROTO_ID_BT_TRACKER                 = 0x0000000D,       /* BT协议TRACKER ID */
    EGN_PROTO_ID_BT_REG                     = 0x00000013,       /* BT协议REG ID */
    EGN_PROTO_ID_BT_LOGIN                   = 0x00000014,       /* BT协议LOGIN ID */
    EGN_PROTO_ID_BT_TRACKER_UDP             = 0x0000000C,       /* BT协议TRACKER_UDP ID */
    EGN_PROTO_ID_BT_DHT                     = 0x0000000E,       /* BT协议DHT ID */
    EGN_PROTO_ID_BT_DATA_TCP                = 0x0000000B,       /* BT协议DATA_TCP ID */
    EGN_PROTO_ID_BT_DATA_UDP                = 0x00000010,       /* BT协议DATA_UDP ID */
    EGN_PROTO_ID_BT_DATA_TCP_ENCREPT        = 0x00000012,       /* BT协议DATA_TCP_ENCREPT ID */

    /** RTSP相关协议ID */
    EGN_PROTO_ID_RTSP_CONTROL               = 0x00000045,       /* RTSP协议CONTROL ID */
    EGN_PROTO_ID_RTSP_DATA_RTP              = 0x00000046,       /* RTSP协议DATA_RTP ID */
    EGN_PROTO_ID_RTSP_DATA_RTCP             = 0x00000047,       /* RTSP协议DATA_RTCP ID */

    /** HTTP相关协议ID */
    EGN_PROTO_ID_HTTP                       = 0x0000004E,       /* HTTP协议HTTP ID */
    EGN_PROTO_ID_HTTP_CMCC                  = 1410,             /* HTTP协议HTTP_CMCC ID */

    /** FTP相关协议ID */
    EGN_PROTO_ID_FTP_CONTROL                = 0x0000004D,       /* FTP协议CONTROL ID */
    EGN_PROTO_ID_FTP_DATA                   = 0x0000004C,       /* FTP协议DATA ID */

    /** TFTP相关协议ID */
    EGN_PROTO_ID_TFTP_CONTROL               = 0x00000061,       /* TFTP协议CONTROL ID */
    EGN_PROTO_ID_TFTP_DATA                  = 0x00000062,       /* TFTP协议DATA ID */

    /** eDonkey相关协议ID */
    EGN_PROTO_ID_EDONKEY_CONTROL_TCP        = 0x00000017,       /* eDonkey协议CONTROL_TCP ID */
    EGN_PROTO_ID_EDONKEY_CONTROL_UDP        = 0x00000018,       /* eDonkey协议CONTROL_UDP ID */
    EGN_PROTO_ID_EDONKEY_DATA_TCP           = 0x00000019,       /* eDonkey协议DATA_TCP ID */
    EGN_PROTO_ID_EDONKEY_DATA_UDP           = 0x0000001A,       /* eDonkey协议DATA_UDP ID */

    /** emule加密协议ID */
    EGN_PROTO_ID_EMULE_ENCRYPTED            = 0x0000001B,       /* emule协议ENCRYPTED ID */

    /** THUNDER协议ID */
    EGN_PROTO_ID_THUNDER                    = 0x0000006A,       /* THUNDER协议ID */

    /** MMS相关协议ID */
    EGN_PROTO_ID_MMS_CONTROL_DATA           = 0x00000050,       /* MMS协议CONTROL_DATA ID */
    EGN_PROTO_ID_MMS_DATA_TCP               = 0x00000051,       /* MMS协议DATA_TCP ID */
    EGN_PROTO_ID_MMS_DATA_UDP               = 0x00000052,       /* MMS协议DATA_UDP ID */

    /** YahooMSG相关协议ID */
    EGN_PROTO_ID_YAHOOMSG_CONTROL           = 0x0000003C,       /* YahooMSG协议CONTROL ID */
    EGN_PROTO_ID_YAHOOMSG_DATA_TRANSFER     = 0x0000003D,       /* YahooMSG协议DATA_TRANSFER ID */
    EGN_PROTO_ID_YAHOOMSG_AUDIO_DATA        = 0x00000040,       /* YahooMSG协议AUDIO_DATA ID */
    EGN_PROTO_ID_YAHOOMSG_AUDIO_CONTROL     = 0x0000003F,       /* YahooMSG协议AUDIO_CONTROL ID */

    /** MSN相关协议ID */
    EGN_PROTO_ID_MSN_OTHER                  = 0x00000034,       /* MSN协议OTHER ID */
    EGN_PROTO_ID_MSN_CONTORL                = 0x00000032,       /* MSN协议CONTORL ID */
    EGN_PROTO_ID_MSN_AUDIO_RTP              = 0x0000002F,       /* MSN协议AUDIO_RTP ID */
    EGN_PROTO_ID_MSN_VIDEO_RTP              = 0x00000031,       /* MSN协议VIDEO_RTP ID */
    EGN_PROTO_ID_MSN_TRANSFER_DATA          = 0x00000030,       /* MSN协议TRANSFER_DATA ID */
    EGN_PROTO_ID_MSN_SIP                    = 0x00000037,       /* MSN协议SIP ID */
    EGN_PROTO_ID_MSN_AUDIO_RTCP             = 0x00000039,       /* MSN协议AUDIO_RTCP ID */
    EGN_PROTO_ID_MSN_VIDEO_RTCP             = 0x00000038,       /* MSN协议VIDEO_RTCP ID */

    /** SKYPE相关协议ID */
    EGN_PROTO_ID_SKYPE_HTTP                 = 0x00000007,       /* SKYPE协议HTTP ID */
    EGN_PROTO_ID_SKYPE_IM                   = 0x00000004,       /* SKYPE协议IM ID */
    EGN_PROTO_ID_SKYPE_LOGIN                = 0x00000006,       /* SKYPE协议LOGIN ID */
    EGN_PROTO_ID_SKYPE_PCTOPHONE            = 0x00000005,       /* SKYPE协议PCTOPHONE ID */

    /** IMAP4相关协议ID */
    EGN_PROTO_ID_IMAP4                      = 0x0000004A,       /* IMAP4协议ID */

    /** SIP相关的协议ID */
    EGN_PROTO_ID_SIP                        = 0x000000AF,       /* SIP协议ID */

    /** QVOD相关协议ID */
    EGN_PROTO_ID_QVOD_CONTROL               = 0x000000A7,       /* QVOD协议CONTROL ID */
    EGN_PROTO_ID_QVOD_DATA                  = 0x000000A9,       /* QVOD协议DATA ID */

    /** WAP1相关协议ID */
    EGN_PROTO_ID_WAP1_CONN                  = 0x0000005C,       /* WAP1协议CONN ID */
    EGN_PROTO_ID_WAP1_CONNLESS              = 0x0000005D,       /* WAP1协议CONNLESS ID */

    /** QingYL_UDP协议编号 */
    EGN_PROTO_ID_QINGYL_UDP                 = 0x00000098,       /* QingYL_UDP协议ID */

    /** PPFILM协议编号 */
    EGN_PROTO_ID_PPFILM                     = 0x0000008C,       /* PPFILM协议ID */

    /** RTSP_RDT协议编号 */
    EGN_PROTO_ID_RTSP_RDT                   = 0x00000048,       /* RTSP_RDT协议ID */

    /** Sipgate协议编号 */
    EGN_PROTO_ID_SIPGATE_CONTORL            = 0x000000DF,       /* Sipgate协议CONTORL ID */
    EGN_PROTO_ID_SIPGATE_AUDIO              = 0x000000E0,       /* Sipgate协议AUDIO ID */

    /** PPVA协议编号 */
    EGN_PROTO_ID_PPVA                       = 0x00000069,       /* PPVA协议ID */

    /** SOPCAST协议编号 */
    EGN_PROTO_ID_SOPCAST                    = 0x0000008A,       /* SOPCAST协议ID */

    /** FlashGet协议编号 */
    EGN_PROTO_ID_FLASHGET                   = 0x0000008F,       /* FlashGet协议ID */

    /** Socks协议编号 */
    EGN_PROTO_ID_SOCKS5_UDP_ASSOCIATE       = 0x000000FC,       /* Socks协议SOCKS5_UDP_ASSOCIATE ID */
    EGN_PROTO_ID_SOCKS5_UDP                 = 0x000000FD,       /* Socks协议SOCKS5_UDP ID */
    EGN_PROTO_ID_SOCKS5_TCP                 = 0x000000FB,       /* Socks协议SOCKS5_TCP ID */
    EGN_PROTO_ID_SOCKS4                     = 0x000000FE,       /* Socks协议SOCKS4 ID */
    EGN_PROTO_ID_SOCKS4A                    = 0x000000FF,       /* Socks协议SOCKS4A ID */

    /** ZOIPER协议编号 */
    EGN_PROTO_ID_ZOIPER_SIP                 = 0x00000109,       /* ZOIPER协议ZOIPER_SIP ID */

    /** IAX2协议编号 */
    EGN_PROTO_ID_IAX2                       = 0x000000E7,       /* IAX2硬编码 */

    /** ARES相关协议ID */
    EGN_PROTO_ID_ARES_DOWNLOAD_DATA         = 0x00000150,       /* ARES协议DOWNLOAD_DATA ID */

    /** SSL相关协议ID */
    EGN_PROTO_ID_SSL                        = 0x00000201,       /* SSL硬编码 */

    /** H323相关协议ID */
    EGN_PROTO_ID_H323_H225_SIGNAL           = 0x000000ED,       /* H323协议H225_SIGNAL ID */
    EGN_PROTO_ID_H323_H245_CONTROL          = 0x000000EE,       /* H323协议H245_CONTROL ID */
    EGN_PROTO_ID_H323_MEDIA                 = 0x000000EF,       /* H323协议MEDIA ID */
    EGN_PROTO_ID_H323_MEDIA_AUDIO           = 0x000000F0,       /* H323协议MEDIA_AUDIO ID */
    EGN_PROTO_ID_H323_MEDIA_VIDEO           = 0x000000F1,       /* H323协议MEDIA_VIDEO ID */

    /** RTMP相关协议ID */
    EGN_PROTO_ID_RTMP                       = 0x00000241,       /* RTMP硬编码 */

    /** Icecast相关协议ID */
    EGN_PROTO_ID_ICECAST                    = 0x0000025E,       /* Icecast硬编码 */
    EGN_PROTO_ID_SHOUTCAST                  = 0x0000025F,       /* Icecast协议SHOUTCAST ID */

    /** FTPS相关协议ID */
    EGN_PROTO_ID_FTPS_CONTROL               = 0x000002D1,       /* FTPS协议CONTROL ID */
    EGN_PROTO_ID_FTPS_DATA                  = 0x000002D2,       /* FTPS协议DATA ID */

    /** Citrix相关协议ID */
    EGN_PROTO_ID_CITRIX_SSL                 = 0x000002EA,       /* Citrix协议SSL ID */
    EGN_PROTO_ID_HTTPS                      = 0x000001AE,       /* Citrix协议HTTPS ID */

    /** Vbuzzer协议 */
    EGN_PROTO_ID_VBUZZER_SSL                = 0x000002DC,       /* Vbuzzer协议SSL ID */

    /** Nimbuzz相关协议ID */
    EGN_PROTO_ID_NIMBUZZ_CONTROL            = 0x000002DD,       /* Nimbuzz协议CONTROL ID */
    EGN_PROTO_ID_NIMBUZZ_AUDIO_DATA         = 0x000002E6,       /* Nimbuzz协议DATA ID */

    /** Globe7相关协议ID */
    EGN_PROTO_ID_GLOBE7_CONTROL             = 0x00000165,       /* Globe7协议CONTROL ID */
    EGN_PROTO_ID_GLOBE7_DATA                = 0x00000164,       /* Globe7协议DATA ID */

    /** Alicall相关协议ID */
    EGN_PROTO_ID_ALICALL_CONTROL            = 0x000000C4,       /* Alicall协议CONTROL ID */
    EGN_PROTO_ID_ALICALL_DATA               = 0x000002F8,       /* Alicall协议DATA ID */

    /** Net2Phone相关协议ID */
    EGN_PROTO_ID_NET2PHONE_CONTROL          = 0x000002E3,       /* Net2Phone协议CONTROL ID */
    EGN_PROTO_ID_NET2PHONE_DATA             = 0x000002F7,       /* Net2Phone协议DATA ID */

    /** BlackBerry */
    EGN_PROTO_ID_BLACKBERRY_MSG             =   0x000002E9,  /* Blackberry message */
    EGN_PROTO_ID_BLACKBERRY_MALI_OUT        =   0x000002E8,  /* Blackberry mai */
    EGN_PROTO_ID_BLACKBERRY_BROWSING        =   0x000002E7,  /* Blackberry browsing */

    EGN_PROTO_ID_END,

    EGN_PROTO_ID_BOTTOM = EGN_EN_BUTT
}EGN_PROTO_ID_EN;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EGN_PROTO_ID_H__ */

