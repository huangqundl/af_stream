#include <inttypes.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Util.hpp"
#include "Config.hpp"
#include "common.hpp"
#include "compute_thread_pcap_decoder.hpp"
#include "StFlow.hpp"

extern "C" {
#include "nids.h"
#include "ip_fragment.h"
}

// libegn
#include "egnapitype.h"
#include "egnapicommon.h"
#include "egnapiprotoid.h"
#include "egnapisspsmp.h"
#include "egnapiipflow.h"

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

//#define WX_SHOW_PACKET_RESULT   1

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

uint32_t IsWeiXinProtocol(uint32_t ulProtocolID)
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

void WeiXinStatistics(uint32_t ulProtocolID)
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
    printf("Frame:%06lu, protoID:%u, name:%s\n", g_ulFrameId, ulProtocolID, protocolName);
#endif

}

/*微信预处理*/
uint32_t WeiXinPreProcess
(
    FLOW  *pstFlow,
    EgnPacket *pstPacketInfo
)
{
    /*微信短连接*/
    if (pstFlow->ulWXProtFlag && !(pstFlow->ulWXConnFlag))
    {
        if (0 < pstPacketInfo->pstIpFrag->usLoadLen)
        {
            WeiXinStatistics(pstFlow->protoId);
        }

        return NO_NEED_INPUT_EGN;
    }

    return 0;
}

void WeiXinMachineState
(
    FLOW  *pstFlow,
    uint32_t ulProtocol
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
    FLOW  *pstFlow,
    EgnPacket *pstPacketInfo,
    EgnResult *pstInspectResult
)
{
    uint32_t ulProtocolID = pstInspectResult->ulProtoID;
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

        pstInspectResult->bIsContinue = true;

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
            pstInspectResult->bIsContinue = true;
        }
        else
        {
#if WX_SHOW_PACKET_RESULT            
            printf("Frame:%06lu, protoID:%u, name:Unkown\n", g_ulFrameId, ulProtocolID);
#endif
        }
    }
}

/*=============微信特殊处理结束，罗向龙201279  --20130627============*/

// timer group
#define VPP_OS_VER VPP_LINUX_POSIX
#include "Vpp_typedef.h"
#include "Vpp_TimerGrpAPI.h"

#define ip2str(x) \
    inet_ntoa(*((struct in_addr *)&x))

/* begin of callback functions */
EGN_VOID *EGN_MemSet
    (IN EGN_VOID * pvMemAddr, IN EGN_UCHAR ucByte, IN EGN_UINT32 ulSize) {
    return memset(pvMemAddr, ucByte, ulSize);
}

EGN_VOID *EGN_MemCpy
    (INOUT EGN_VOID * pvDestAddr,
     IN EGN_VOID * pvSrcAddr, IN EGN_UINT32 ulSize) {
    return memcpy(pvDestAddr, pvSrcAddr, ulSize);
}

EGN_UINT32 EGN_MemCmp
    (INOUT EGN_VOID * pvDestAddr,
     IN EGN_VOID * pvSrcAddr, IN EGN_UINT32 ulSize) {
    return memcmp(pvDestAddr, pvSrcAddr, ulSize);
}

EGN_UINT32 EGN_StrLen(char *pcStr)
{
    return strlen(pcStr);
}

// timer group, IDK WTF
EGN_UINT32 EGN_TimerGrpCreate
    (OUT EGN_VOID ** ppTimerGrp,
     IN EGN_UINT32 ulMaxTimerNums, IN PFEgnFuncTimeout pfTimeout) {
    return VppTimerGrpCreate(ppTimerGrp, ulMaxTimerNums, pfTimeout);
}

EGN_UINT32 EGN_TimerGrpStart
    (IN EGN_VOID * pTimerGrp,
     IN EGN_UINT32 ulTmSn,
     IN EGN_UINT32 ulTmLength, IN EGN_UINT32 ulTmName, IN EGN_UCHAR ucTmMode) {
    return VppTimerGrpStart(pTimerGrp, ulTmSn, ulTmLength, ulTmName, ucTmMode);
}

EGN_UINT32 EGN_TimerGrpStop(IN EGN_VOID * pvTimerGrp, IN EGN_UINT32 ulTmSn)
{
    return VppTimerGrpStop(pvTimerGrp, ulTmSn);
}

EGN_UINT32 EGN_TimerGrpDestroy(IN EGN_VOID ** ppvTimerGrp)
{
    return VppTimerGrpDestroy(ppvTimerGrp);
    return EGN_RET_SUCCESS;
}

// lock related fucntions. use empty functions for single thread.
EGN_UINT32 EGN_SmCreate
    (IN EGN_CONST EGN_UCHAR * pucName, INOUT EGN_VOID ** ppRWLockHandle) {
    return EGN_RET_SUCCESS;
}

EGN_UINT32 EGN_SmP(IN EGN_CONST EGN_VOID * pRWLockHandle)
{
    return EGN_RET_SUCCESS;
}

EGN_UINT32 EGN_SmV(IN EGN_CONST EGN_VOID * pRWLockHandle)
{
    return EGN_RET_SUCCESS;
}

EGN_UINT32 EGN_SmDelete
    (IN EGN_CONST EGN_UCHAR * pucName, INOUT EGN_VOID ** ppRWLockHandle) {
    return EGN_RET_SUCCESS;
}

EGN_UINT32 EGN_StrNCmp
    (IN const EGN_CHAR * pcStr1,
     IN const EGN_CHAR * pcStr2, IN EGN_UINT32 ulCount) {
    return strncmp(pcStr1, pcStr2, ulCount);
}

EGN_VOID EGN_AtomicAdd(IN EGN_UINT32 ulAddEnd, INOUT EGN_UINT32 * pulSummand)
{
    *pulSummand += ulAddEnd;
}

EGN_VOID EGN_AtomicSub
    (IN EGN_UINT32 ulSubtrahend, INOUT EGN_UINT32 * pulMinuend) {
    *pulMinuend -= ulSubtrahend;
}

EGN_VOID feedWatchdog(void)
{
    return;
}

EGN_VOID *EgnUtFileOpen(IN EGN_CONST EGN_UCHAR * pucFileName,
                        IN EGN_CHAR * pcMode)
{
    return fopen((const char *)(pucFileName), pcMode);
}

EGN_INT32 EgnUtFileSeek(IN EGN_VOID * handle, IN EGN_INT32 offset,
                        IN EGN_UINT32 ulWhence)
{
    return fseek((FILE *) handle, (long)(offset), ulWhence);
}

EGN_INT32 EgnUtFileRead
    (IN EgnFileHandle iFd, IN EGN_VOID * pBuf, IN EGN_UINT32 ulCount) {
    return fread(pBuf, 1, ulCount, (FILE *) iFd);
}

EGN_INT32 EgnUtFileClose(IN EgnFileHandle iFd)
{
    return fclose((FILE *) iFd);
}

EGN_INT32 EgnUtFileTell(IN EgnFileHandle iFd)
{
    return ftell((FILE *) iFd);
}

EGN_VOID *EgnUtMemAlloc
    (IN EGN_UINT32 ulPID, IN EGN_UINT32 ulMemType, IN EGN_UINT32 ulSize) {
    return malloc(ulSize);
}

EGN_VOID EgnUtMemFree(IN EGN_UINT32 ulModId, IN EGN_VOID * pvMemAddr)
{
    free(pvMemAddr);
    pvMemAddr = NULL;
}

/* end of callback functions */

void DPIBoltSubComputeThread::process(Packet::TotalField &tf)
{
    count++;
    if (count % 10000 == 0)
        LOG_MSG("\rProcessing %8" PRIu64 "th packet", count);

    p.get_tf() = tf;

    if (p.get_tf().bf.status != STATUS_VALID) {
        p.get_tf().bf.appId = 0;
        emit(p.get_tf());
        return;
    }

    st_flow_->link_state(p);
    FLOW* f = st_flow_->update_state(p);
    if (f == NULL) // should not happen
        LOG_ERR("Cannot find a flow.\n");

    struct ip *ip_hdr = (struct ip *) p.get_payload();
    int ip_len = p.get_ip_dlen();
    if (ip_len > p.get_data_len()) { // bugy packets
        return;
    }

    bool need_free = false;
    switch (ip_defrag_stub(ip_hdr, &ip_hdr)) {
        case IPF_ISF:
            return;
        case IPF_NOTF:
            break;
        case IPF_NEW:
            ip_len = ntohs(ip_hdr->ip_len);
            need_free = true;
            break;
        default:
            return;
    };

    // debug
    //static int flag = 0;
    //if (flag < 0) {
        //LOG_MSG("===============packet info=========\n");
        //uint32_t src_ip = p.get_src_ip();
        //uint32_t dst_ip = p.get_dst_ip();
        //LOG_MSG("source: %s:%u\n", ip2str(src_ip), p.get_src_port());
        //LOG_MSG("dest: %s:%u\n", ip2str(dst_ip), p.get_dst_port());
        //LOG_MSG("ip length: %u\n", p.get_ip_dlen());
        //flag++;
    //}

    g_ulFrameId++;
    g_currPacketPayload = ip_len;
    g_totalPayload += g_currPacketPayload;
    packetInspect(ip_hdr, ip_len, f);
    if (need_free)
        free(ip_hdr);

    if (count % 100 == 0) {
        std::vector<FLOW>* unexported_flows = st_flow_->get_unexported_flows();
        uint32_t size = unexported_flows->size();
        for (uint32_t i=0; i<size; ++i) {
            FLOW *f = &(*unexported_flows)[i];
            f->dpi_state = kUnknown;
            for (list<Packet::TotalField>::iterator i = f->pkt_list->begin(); i != f->pkt_list->end(); i++) {
                i->bf.appId = 0;
                emit(*i);
            }
            f->pkt_list->clear();
            delete f->pkt_list;
        }
        unexported_flows->clear();
    }
}

void DPIBoltSubComputeThread::packetInspect(struct ip *ip_hdr, int ip_len, FLOW *f)
{
    EGN_UINT32 ulRet;
    EgnPacket stPacket;
    EgnIpFrag stIpFrag;
    EGN_VOID **ppvFlow = &(f->egn_flow);
    EgnResult stResult;

    stPacket.pucIpData = (EGN_UINT8 *) ip_hdr;
    stPacket.pucTpData = (EGN_UINT8 *) (ip_hdr) + (ip_hdr->ip_hl << 2);
    stPacket.pstIpFrag = &stIpFrag;
    stIpFrag.pstNext = NULL;
    if (f->field.orig_ip == p.get_src_ip()) {
        stPacket.ucDirection = EGN_PACKET_DIRECTION_UP;
    } else {
        stPacket.ucDirection = EGN_PACKET_DIRECTION_DOWN;
    }
    stPacket.ulTunnelId = 0;
    stPacket.pvFlowHandle = NULL;
    stPacket.ucProto = p.get_proto();
    stPacket.stSrcIp.u.ulIpv4Addr = p.get_src_ip();
    stPacket.stSrcIp.ulIpType = EGN_ADDR_TYPE_IPV4;
    stPacket.stDstIp.u.ulIpv4Addr = p.get_dst_ip();
    stPacket.stDstIp.ulIpType = EGN_ADDR_TYPE_IPV4;
    stPacket.usSrcPort = p.get_src_port();
    stPacket.usDstPort = p.get_dst_port();
    if (p.get_proto() == IPPROTO_TCP) {
        struct tcphdr *tcp_hdr = (struct tcphdr *)((EGN_UCHAR *)ip_hdr +
                (ip_hdr->ip_hl << 2));
        stIpFrag.pucLoadData = (EGN_UCHAR *) tcp_hdr + (tcp_hdr->doff << 2);
        stIpFrag.usLoadLen = ip_len - (ip_hdr->ip_hl << 2) -
            (tcp_hdr->doff << 2);
    } else if (ip_hdr->ip_p == IPPROTO_UDP) {
        struct udphdr *udp_hdr = (struct udphdr *)((EGN_UCHAR *)ip_hdr +
                (ip_hdr->ip_hl << 2));
        stIpFrag.pucLoadData = (EGN_UCHAR *) udp_hdr + 8;
        stIpFrag.usLoadLen = ip_len - (ip_hdr->ip_hl << 2) - 8;
    }

    if (f->is_finished == false) {
        if (stIpFrag.usLoadLen == 0) {
            f->dpi_state = kUnknown;
            return;
        }

        ulRet = EgnApiPacketInspect(inspectId, pvHandle, ppvFlow,
                &stPacket, &stResult, NULL);

        if (ulRet == EGN_RET_SUCCESS && WeiXinPreProcess(f, &stPacket) == 0) {
            WeiXinProcess(f, &stPacket, &stResult);
        }

        if (ulRet != EGN_RET_SUCCESS) {
            // debug
            //static int flag = 0;
            //if (flag < 10) {
                //flag++;
                //LOG_MSG("ulRet = %x\n", ulRet);
            //}
            EgnApiReleaseCtxData(inspectId, pvHandle, ppvFlow);
            f->is_finished = true;
            for (list<Packet::TotalField>::iterator i = f->pkt_list->begin(); i != f->pkt_list->end(); i++) {
                (*i).bf.appId = 0;
                emit((*i));
            }
            f->pkt_list->clear();
        } else if (stResult.bIsContinue == false) {
            f->is_finished = true;
            EgnApiReleaseCtxData(inspectId, pvHandle, ppvFlow);
            if (stResult.ulResult == EGN_INSPECT_UNKNOWN) {
                f->dpi_state = kUnknown;
                f->protoId = 0;
            } else {
                f->dpi_state = kOk;
                f->protoId = stResult.ulProtoID;
            }
            for (list<Packet::TotalField>::iterator i = f->pkt_list->begin(); i != f->pkt_list->end(); i++) {
                (*i).bf.appId = f->protoId;
                emit((*i));
            }
            f->pkt_list->clear();
        } else {
            f->dpi_state = kUnknown;
        }
    } else {
        p.get_tf().bf.appId = f->protoId;
        emit(p.get_tf());
        f->pkt_list->clear();
    }
}

void DPIBoltSubComputeThread::finish()
{
    LOG_MSG("SubComputeThread %d (%d) processed %" PRIu64 "\n", subId,
            getpid(), count);

    st_flow_->travel_flows();
    std::vector<FLOW>* unexported_flows = st_flow_->get_unexported_flows();
    uint32_t size = unexported_flows->size();
    for (uint32_t i=0; i<size; ++i) {
        FLOW *f = &(*unexported_flows)[i];
        f->dpi_state = kUnknown;
        for (list<Packet::TotalField>::iterator i = f->pkt_list->begin(); i != f->pkt_list->end(); i++) {
            i->bf.appId = 0;
            emit(*i);
        }
        f->pkt_list->clear();
        delete f->pkt_list;
    }

    ip_frag_exit();

    WeiXinShowStatistics();

    ComputeThread<Packet::TotalField, Packet::TotalField>::finish();
}

DPIBoltSubComputeThread::DPIBoltSubComputeThread(std::string name, int id) :
    ComputeThread<Packet::TotalField, Packet::TotalField>(name, id)
{
    Config *config = Config::getInstance();
    st_flow_ = new StFlow(
            config->getint("flowTableSize", 100000),
            config->getdouble("flowTableTimeout", 600));
    pvHandle = NULL;
    inspectId = 0;
}

int DPIBoltSubComputeThread::APPEGNRegInit(const char *rule_lib)
{
    LOG_MSG("DPI Engine Initialization...\n");
    unsigned int g_ulBufferLen;
    EGN_UCHAR *g_pucGlobalBuffer;

    EgnInitCfgParam stInitPub;

    EGN_UINT32 ulRet;
    EgnSspFunc stEgnSspFunc;

    // initialize the callback function structure
    ulRet = EgnApiInitSspFuncParam(&stEgnSspFunc);
    if (ulRet != EGN_RET_SUCCESS) {
        LOG_MSG("EgnApiInitSspFuncParam failed!\n");
        return EGN_RET_FAILURE;
    }
    // register callback functions
    stEgnSspFunc.pfMemAlloc = EgnUtMemAlloc;
    stEgnSspFunc.pfMemFree = EgnUtMemFree;
    stEgnSspFunc.pfMemSet = EGN_MemSet;
    stEgnSspFunc.pfMemCpy = EGN_MemCpy;
    stEgnSspFunc.pfMemCmp = EGN_MemCmp;
    stEgnSspFunc.pfStrNCmp = EGN_StrNCmp;
    stEgnSspFunc.pfStrLen = EGN_StrLen;

    stEgnSspFunc.pfFopen = EgnUtFileOpen;
    stEgnSspFunc.pfFseek = EgnUtFileSeek;
    stEgnSspFunc.pfFclose = EgnUtFileClose;
    stEgnSspFunc.pfFread = EgnUtFileRead;
    stEgnSspFunc.pfFtell = EgnUtFileTell;

    stEgnSspFunc.pfTimerGrpCreate = EGN_TimerGrpCreate;
    stEgnSspFunc.pfTimerGrpStart = EGN_TimerGrpStart;
    stEgnSspFunc.pfTimerGrpStop = EGN_TimerGrpStop;
    stEgnSspFunc.pfTimerGrpDestroy = EGN_TimerGrpDestroy;

    stEgnSspFunc.pfRWLCreate = EGN_SmCreate;
    stEgnSspFunc.pfRWLWLock = EGN_SmP;
    stEgnSspFunc.pfRWLWUnlock = EGN_SmV;
    stEgnSspFunc.pfRWLRLock = EGN_SmP;
    stEgnSspFunc.pfRWLRUnlock = EGN_SmV;
    stEgnSspFunc.pfRWLockDelete = EGN_SmDelete;

    stEgnSspFunc.pfAtomicAdd = EGN_AtomicAdd;
    stEgnSspFunc.pfAtomicSub = EGN_AtomicSub;
    stEgnSspFunc.pfFeedWatchdog = feedWatchdog;

    // register the callback function structure in EGN
    ulRet = EgnApiRegSspFunc(&stEgnSspFunc);
    if (ulRet != EGN_RET_SUCCESS) {
        LOG_MSG("EgnApiRegSspFunc failed!\n");
        return EGN_RET_FAILURE;
    }

    ulRet = EgnApiInitPubParam(&stInitPub);
    if (EGN_RET_SUCCESS != ulRet) {
        LOG_MSG("EgnApiInitPubParam return error\n");
        return ulRet;
    }
    // Instance No.
    stInitPub.usMaxInstance = 1;
    // the number of CB count; usually 1/2 of the number of cached flows
    stInitPub.ulRelationCBCountIPv4 = 1500000;

    // 800 MB buffer
    g_ulBufferLen = 800 * 1024 * 1024;
    g_pucGlobalBuffer = (EGN_UCHAR *) malloc(g_ulBufferLen);
    if (g_pucGlobalBuffer == NULL)
        LOG_ERR("memory is not enough: g_pucGlobalBuffer\n");

    ulRet = EgnApiMngInit(g_pucGlobalBuffer, g_ulBufferLen, &stInitPub);
    if (EGN_RET_SUCCESS != ulRet) {
        LOG_MSG("EgnApiInit return error\n");
        if (g_pucGlobalBuffer != NULL) {
            free(g_pucGlobalBuffer);
            g_pucGlobalBuffer = NULL;
        }
        return ulRet;
    } else {
        LOG_MSG("ApimngInit successfull\n");
    }

    // import the rule lib from file
    ulRet = EgnApiImportSysRuleLib(NULL, 0, (EGN_CONST EGN_UINT8 *) rule_lib);

    if (EGN_RET_SUCCESS != ulRet) {
        LOG_MSG("EgnApiImportSysRuleLib failed!\n");
        return ulRet;
    } else {
        LOG_MSG("EgnApiImportSysRuleLib Successful\n");
    }

    // load the rule lib
    ulRet = EgnApiLoadRuleLib();
    if (EGN_RET_SUCCESS != ulRet) {
        LOG_MSG("EgnApiLoadRuleLib failed!\n");
        return ulRet;
    } else {
        LOG_MSG("EgnApiLoadRuleLib Successful\n");
    }

    // apply the rule lib
    ulRet = EgnApiApplyRuleLib();
    if (EGN_RET_SUCCESS != ulRet) {
        LOG_MSG("EgnApiApplyRuleLib failed!\n");
        return ulRet;
    } else {
        LOG_MSG("EgnApiApplyRuleLib Successful\n");
    }

    ulRet = EgnApiInspectorInit(inspectId, g_pucGlobalBuffer,
            g_ulBufferLen, &pvHandle);

    if (ulRet != EGN_RET_SUCCESS) {
        LOG_MSG("EgnApiInspectorInit failed!\n");
        (EGN_VOID) EgnApiInspectorDestroy(inspectId, &pvHandle);
        return ulRet;
    } else {
        LOG_MSG("EgnApiInspectorInit Successful\n");
    }

    EgnApiSetWellknownPortState(EGN_TRUE);

    return EGN_RET_SUCCESS;
}

void DPIBoltSubComputeThread::initialize()
{
    ip_frag_init(nids_params.n_hosts);
    st_flow_->create_table();
    Config *config = Config::getInstance();
    if (APPEGNRegInit(config->getstring("ruleLib", NULL)) != EGN_RET_SUCCESS)
        LOG_ERR("EGN Init fail\n");

    ComputeThread<Packet::TotalField, Packet::TotalField >::initialize();
}
