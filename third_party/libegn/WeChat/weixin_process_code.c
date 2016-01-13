
/*=============微信特殊处理开始，罗向龙201279  --20130627============*/

#define PROT_WX                 1837
#define PROT_WX_VOIP            1865
#define PROT_WX_LOGIN           3073
#define PROT_WX_REALTALK        3074
#define PROT_WX_VOICECALL       3075
#define PROT_WX_VIDEOCALL       3076
#define PROT_WX_TXTCHAT         3077
#define PROT_WX_SENDPIC         3078
#define PROT_WX_HEATBEAT        3079
#define PROT_WX_FRIEND          3080
#define PROT_WX_VOICEMSG        3081
#define PROT_WX_VIDEOCALL_START 3082
#define PROT_WX_VIDEOCALL_FIN   3083
#define PROT_WX_VIDEO_AUDIO_SW  3084
#define PROT_WX_REALTALK_START  3085
#define PROT_WX_REALTALK_FIN    3086
#define PROT_WX_LOGOUT          3087
#define PROT_WX_KEYDOWN_REPORT  3088

#define PROT_WX_UNKOWN          0
#define PROT_WX_MSGTYPE_CHANGE  1

#define WX_SHOW_PACKET_RESULT   1

#define PROT_HTTP               78

#define NO_NEED_INPUT_EGN       1

unsigned long g_ulFrameId = 0;  /*cap包的第几帧*/

unsigned long  g_totalPayload = 0; /*总流量*/
unsigned long  g_currPacketPayload = 0; /*当前报文流量*/

unsigned long  g_WXCommonPayload = 0; /*通用流量*/
unsigned long  g_WXLoginPayload = 0; /*登录流量*/
unsigned long  g_WXTextChatPayload = 0; /*文字聊天流量*/
unsigned long  g_WXSendPicPayload = 0; /*图片分享流量*/
unsigned long  g_WXVoiceMsgPayload = 0; /*语音留言流量*/
unsigned long  g_WXFriendPayload = 0; /*朋友圈流量*/
unsigned long  g_WXHeartbeatPayload = 0; /*心跳流量*/
unsigned long  g_WXRealTalkPayload = 0; /*实时对讲流量*/
unsigned long  g_WXVideoCallPayload = 0; /*视频通话流量*/
unsigned long  g_WXVoiceCallPayload = 0; /*语音通话流量*/
unsigned long  g_WXVoipPayload = 0; /*VOID流量*/
unsigned long  g_WXLogoutPyaload = 0; /*退出流量*/
unsigned long  g_WXKeydownReportPayload = 0; /*按键报告*/

_UL IsWeiXinProtocol(_UL ulProtocolID)
{
    if (   ulProtocolID == PROT_WX
        || ulProtocolID == PROT_WX_LOGIN
        || ulProtocolID == PROT_WX_TXTCHAT
        || ulProtocolID == PROT_WX_SENDPIC
        || ulProtocolID == PROT_WX_VOICEMSG
        || ulProtocolID == PROT_WX_FRIEND
        || ulProtocolID == PROT_WX_HEATBEAT
        || ulProtocolID == PROT_WX_REALTALK
        || ulProtocolID == PROT_WX_REALTALK_FIN
        || ulProtocolID == PROT_WX_REALTALK_START
        || ulProtocolID == PROT_WX_VIDEOCALL_FIN
        || ulProtocolID == PROT_WX_VIDEOCALL_START
        || ulProtocolID == PROT_WX_VIDEO_AUDIO_SW
        || ulProtocolID == PROT_WX_VOIP
        || ulProtocolID == PROT_WX_LOGOUT
        || ulProtocolID == PROT_WX_VOICECALL
        || ulProtocolID == PROT_WX_VIDEOCALL
        || ulProtocolID == PROT_WX_KEYDOWN_REPORT)
    {
        return 1;
    }

    return 0;
}

void WeiXinPayloadReset()
{
    g_WXCommonPayload = 0; /*通用流量*/
    g_WXLoginPayload = 0; /*登录流量*/
    g_WXTextChatPayload = 0; /*文字聊天流量*/
    g_WXSendPicPayload = 0; /*图片分享流量*/
    g_WXVoiceMsgPayload = 0; /*语音留言流量*/
    g_WXFriendPayload = 0; /*朋友圈流量*/
    g_WXHeartbeatPayload = 0; /*心跳流量*/
    g_WXRealTalkPayload = 0; /*实时对讲流量*/
    g_WXVideoCallPayload = 0; /*视频通话流量*/
    g_WXVoiceCallPayload = 0; /*语音通话流量*/
    g_WXVoipPayload = 0; /*VOID流量*/
    g_WXLogoutPyaload = 0; /*退出流量*/
    g_WXKeydownReportPayload = 0; /*按键报告*/
}

void WeiXinShowStatistics()
{
    printf("total:\t\t\t%ld\t(%.3f)\n", g_totalPayload, g_totalPayload*100.0/g_totalPayload);
    printf("common:\t\t\t%ld\t(%.3f)\n", g_WXCommonPayload, g_WXCommonPayload*100.0/g_totalPayload);
    printf("login:\t\t\t%ld\t(%.3f)\n", g_WXLoginPayload, g_WXLoginPayload*100.0/g_totalPayload);
    printf("text chat:\t\t%ld\t(%.3f)\n", g_WXTextChatPayload, g_WXTextChatPayload*100.0/g_totalPayload);
    printf("send pic:\t\t%ld\t(%.3f)\n", g_WXSendPicPayload, g_WXSendPicPayload*100.0/g_totalPayload);
    printf("voice msg:\t\t%ld\t(%.3f)\n", g_WXVoiceMsgPayload, g_WXVoiceMsgPayload*100.0/g_totalPayload);
    printf("friend circle:\t\t%ld\t(%.3f)\n", g_WXFriendPayload, g_WXFriendPayload*100.0/g_totalPayload);
    printf("heartbeat:\t\t%ld\t(%.3f)\n", g_WXHeartbeatPayload, g_WXHeartbeatPayload*100.0/g_totalPayload);
    printf("real talk:\t\t%ld\t(%.3f)\n", g_WXRealTalkPayload, g_WXRealTalkPayload*100.0/g_totalPayload);
    printf("video call:\t\t%ld\t(%.3f)\n", g_WXVideoCallPayload, g_WXVideoCallPayload*100.0/g_totalPayload);
    printf("voice call:\t\t%ld\t(%.3f)\n", g_WXVoiceCallPayload, g_WXVoiceCallPayload*100.0/g_totalPayload);
    printf("VoIP:\t\t\t%ld\t(%.3f)\n", g_WXVoipPayload, g_WXVoipPayload*100.0/g_totalPayload);
    printf("logout:\t\t\t%ld\t(%.3f)\n", g_WXLogoutPyaload, g_WXLogoutPyaload*100.0/g_totalPayload);
    printf("keydown report:\t\t%ld\t(%.3f)\n", g_WXKeydownReportPayload, g_WXKeydownReportPayload*100.0/g_totalPayload);
}

void WeiXinStatistics(_UL ulProtocolID)
{
    char protocolName[128] = "";
    if (ulProtocolID == PROT_WX)
    {
        strcpy(protocolName, "WeiXin_Common");
        g_WXCommonPayload += g_currPacketPayload;
    }
    else if(ulProtocolID == PROT_WX_LOGIN)
    {
        strcpy(protocolName, "WeiXin_Login");
        g_WXLoginPayload += g_currPacketPayload;
    }
    else if (ulProtocolID == PROT_WX_TXTCHAT)
    {
        strcpy(protocolName, "WeiXin_TextChat");
        g_WXTextChatPayload += g_currPacketPayload;
    }
    else if (ulProtocolID == PROT_WX_SENDPIC)
    {
        strcpy(protocolName, "WeiXin_SendPicture");
        g_WXSendPicPayload += g_currPacketPayload;
    }
    else if (ulProtocolID == PROT_WX_VOICEMSG)
    {
        strcpy(protocolName, "WeiXin_VoiceMessage");
        g_WXVoiceMsgPayload += g_currPacketPayload;
    }
    else if (ulProtocolID == PROT_WX_FRIEND)
    {
        strcpy(protocolName, "WeiXin_FriendCircle");
        g_WXFriendPayload += g_currPacketPayload;
    }
    else if (ulProtocolID == PROT_WX_HEATBEAT)
    {
        strcpy(protocolName, "WeiXin_HeartBeat");
        g_WXHeartbeatPayload += g_currPacketPayload;
    }
    else if (ulProtocolID == PROT_WX_REALTALK
        || ulProtocolID == PROT_WX_REALTALK_FIN
        || ulProtocolID == PROT_WX_REALTALK_START)
    {
        strcpy(protocolName, "WeiXin_RealTalk");
        g_WXRealTalkPayload += g_currPacketPayload;
    }
    else if (ulProtocolID == PROT_WX_VIDEOCALL_FIN
        || ulProtocolID == PROT_WX_VIDEOCALL_START
        || ulProtocolID == PROT_WX_VIDEOCALL
        || ulProtocolID == PROT_WX_VIDEO_AUDIO_SW)
    {
        strcpy(protocolName, "WeiXin_VideoCall");
        g_WXVideoCallPayload += g_currPacketPayload;
    }
    else if (ulProtocolID == PROT_WX_VOICECALL)
    {
        strcpy(protocolName, "WeiXin_VoiceCall");
        g_WXVoiceCallPayload += g_currPacketPayload;
    }
    else if (ulProtocolID == PROT_WX_VOIP)
    {
        strcpy(protocolName, "WeiXin_VOIP");
        g_WXVoipPayload += g_currPacketPayload;
    }
    else if (ulProtocolID == PROT_WX_LOGOUT)
    {
        strcpy(protocolName, "WeiXin_Logout");
        g_WXLogoutPyaload += g_currPacketPayload;
    }
    else if (ulProtocolID == PROT_WX_KEYDOWN_REPORT)
    {
        strcpy(protocolName, "WeiXin_KeydownReport");
        g_WXKeydownReportPayload += g_currPacketPayload;
    }
    else
    {
        strcpy(protocolName, "Unknow");
    }
    
#if WX_SHOW_PACKET_RESULT
    printf("Frame:%06d, protoID:%d, name:%s\n", g_ulFrameId, ulProtocolID, protocolName);
#endif

}

/*微信预处理*/
_UL WeiXinPreProcess
(
    FlowTbl  *pstFlow,
    EgnPacket *pstPacketInfo
)
{
    /*微信短连接*/
    if (pstFlow->ulWXProtFlag && !(pstFlow->ulWXConnFlag))
    {
        if (0 < pstPacketInfo->pstIpFrag->usLoadLen)
        {
            WeiXinStatistics(pstFlow->pstIdentifyInfo->ulIdentifyProtocolID);
        }

        return NO_NEED_INPUT_EGN;
    }

    return 0;
}

void WeiXinMachineState
(
    FlowTbl  *pstFlow,
    _UL ulProtocol
)
{
    /*处于视频通话或者语音通话，只有结束通话，才能结束状态*/
    if (PROT_WX_VIDEOCALL_START == pstFlow->ulWXState)
    {
        if (PROT_WX_VIDEOCALL_FIN == ulProtocol)
        {
            pstFlow->ulWXState = PROT_WX_UNKOWN;
        }

        return;
    }
    
    /*处于实时对讲，只有结束实时对讲，才能结束状态*/
    if (PROT_WX_REALTALK_START == pstFlow->ulWXState)
    {
        if (PROT_WX_REALTALK_FIN== ulProtocol)
        {
            pstFlow->ulWXState = PROT_WX_UNKOWN;
        }

        return;
    }

    if (PROT_WX_MSGTYPE_CHANGE == ulProtocol)
    {
        pstFlow->ulWXState = PROT_WX_UNKOWN;
        return;
    }

    pstFlow->ulWXState = ulProtocol;
    
}


void WeiXinProcess
(
    FlowTbl  *pstFlow,
    EgnPacket *pstPacketInfo,
    EgnResult *pstInspectResult
)
{
    _UL ulProtocolID = pstInspectResult->ulProtoID;
/*        
    pstPacketInfo->pstIpFrag->usLoadLen; //流量
    //匹配的包个数
    
    pstPacketInfo->pstIpFrag->pucLoadData; //传输层净荷数据
*/
    if (IsWeiXinProtocol(ulProtocolID))
    {
        pstFlow->ulWXProtFlag = 1;
        //if (pstFlow->pstIdentifyInfo->aulCarrierProtoID[0] != PROT_HTTP)
        if (pstInspectResult->aulCarrierProtoID[0] != PROT_HTTP)
        {
            pstFlow->ulWXConnFlag = 1;
        }

        pstInspectResult->bIsContinue = TRUE;

        WeiXinMachineState(pstFlow, ulProtocolID);

        WeiXinStatistics(ulProtocolID);
    }
    else
    {
        /*微信长连接*/
        if (pstFlow->ulWXProtFlag && pstFlow->ulWXConnFlag)
        {
            /*判断是否为消息类型变更*/
            if('\x01' == pstPacketInfo->pstIpFrag->pucLoadData[7]
                && '\x00' == pstPacketInfo->pstIpFrag->pucLoadData[8]
                && '\x00' == pstPacketInfo->pstIpFrag->pucLoadData[9]
                && '\x00' == pstPacketInfo->pstIpFrag->pucLoadData[10])
            {
                WeiXinMachineState(pstFlow, PROT_WX_MSGTYPE_CHANGE);
            }

            if (PROT_WX_UNKOWN != pstFlow->ulWXState)
            {
                WeiXinStatistics(pstFlow->ulWXState);
            }
            else
            {
                /*长连接上的其他报文，全部识别为WX协议*/
                WeiXinStatistics(PROT_WX);
            }

            /*报文继续进来*/
            pstInspectResult->bIsContinue = TRUE;
        }
        else
        {
#if WX_SHOW_PACKET_RESULT            
            printf("Frame:%06d, WeiXin ID:Unkown, protoID:%d\n", g_ulFrameId, ulProtocolID);
#endif
        }
    }
}

/*=============微信特殊处理结束，罗向龙201279  --20130627============*/
