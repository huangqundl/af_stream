#include "DPIBoltComputeThread.hpp"

#include <inttypes.h>

#include "Util.hpp"
#include "Config.hpp"
#include "common.hpp"

std::string prot_long_str[] = { IPOQUE_PROTOCOL_LONG_STRING };
uint64_t protocol_counter[IPOQUE_MAX_SUPPORTED_PROTOCOLS + 1];
uint64_t protocol_counter_bytes[IPOQUE_MAX_SUPPORTED_PROTOCOLS + 1];


/****************************************************************************
 * Static Functions
 ****************************************************************************/

static void *malloc_wrapper(unsigned long size)
{
	return malloc(size);
}

static void free_wrapper(void *freeable)
{
	free(freeable);
}

static void debug_printf(u32 protocol, void *id_struct, ipq_log_level_t log_level, const char *format, ...) {}

u8 build_ipoque_key_from_flow(
            const FLOW* flow,
            struct ipoque_unique_flow_ipv4_and_6_struct *key_return,
            u8 * dir_return,
            u32 flags) {

    u8 swapped = 0;

    if (key_return == NULL)
        return 1;

    key_return->is_ip_v6 = 0;
    key_return->protocol = flow->field.proto;

    if (flow->field.orig_ip < flow->field.resp_ip) {
        key_return->ip.ipv4.lower_ip = flow->field.orig_ip;
        key_return->ip.ipv4.upper_ip = flow->field.resp_ip;
    } else {
        key_return->ip.ipv4.upper_ip = flow->field.orig_ip;
        key_return->ip.ipv4.lower_ip = flow->field.resp_ip;
        swapped = 1;
    }

    key_return->ip.ipv4.dummy[0] = 0;
    key_return->ip.ipv4.dummy[1] = 0;
    key_return->ip.ipv4.dummy[2] = 0;

    if (swapped == 0) {
        // qhuang: htons - compatible with OpenDPI internal
        key_return->lower_port = htons(flow->field.orig_port);
        key_return->upper_port = htons(flow->field.resp_port);
    } else {
        key_return->lower_port = htons(flow->field.resp_port);
        key_return->upper_port = htons(flow->field.orig_port);
    }

    if (dir_return != NULL) {
        *dir_return = swapped;
    }

    return 0;
}

/****************************************************************************
 * Functions for ids hash table
 ****************************************************************************/

unsigned int 
DPIBoltComputeThread::id_hash_fcn(const osdpi_id* id) {
    return HashFunc((const char*)&id->ip, sizeof(struct osdpi_ip_addr));
}

bool 
DPIBoltComputeThread::id_cmp_fcn(const osdpi_id* id1, const osdpi_id* id2) {
    if (memcmp(&id1->ip, &id2->ip, sizeof(struct osdpi_ip_addr)) == 0) {
        return true;
    }
    else {
        return false;
    }
}

void 
DPIBoltComputeThread::id_init_fcn(osdpi_id* id, void* st_arg) {
    memset(id, 0, sizeof(struct osdpi_id));
}

void 
DPIBoltComputeThread::id_free_fcn(struct osdpi_id* id, void* st_arg) {
    //LOG_MSG("Warning: free osdpi ids\n");

    memset(id, 0, sizeof(struct osdpi_id));
}

void 
DPIBoltComputeThread::id_travel_do_fcn(struct osdpi_id* f, void* context) {
}

/****************************************************************************
 * Functions for flows hash table
 ****************************************************************************/

unsigned int 
DPIBoltComputeThread::flow_hash_fcn(const osdpi_flow* flow) {
    return HashFunc((const char*)&flow->key, sizeof(struct ipoque_unique_flow_ipv4_and_6_struct));
}

bool 
DPIBoltComputeThread::flow_cmp_fcn(const osdpi_flow* flow1, const osdpi_flow* flow2) {
    if (memcmp(&flow1->key, &flow2->key, sizeof(struct ipoque_unique_flow_ipv4_and_6_struct)) == 0) {
        return true;
    }
    else {
        return false;
    }
}

void 
DPIBoltComputeThread::flow_init_fcn(osdpi_flow* flow, void* st_arg) {
    memset(flow, 0, sizeof(struct osdpi_flow));
}

void 
DPIBoltComputeThread::flow_free_fcn(struct osdpi_flow* flow, void* st_arg) {
    //LOG_MSG("Warning: free osdpi flows\n");

    memset(flow, 0, sizeof(struct osdpi_flow));
}

void 
DPIBoltComputeThread::flow_travel_do_fcn(struct osdpi_flow* f, void* context) {
    if (f->detected_protocol == 0) {
        char msg[1024];
        char addr1[30];
        char addr2[30];
        sprintf(msg, "origip=%s respip=%s origport=%u respport=%u proto=%u pkt: %" PRIu64  "\n", 
            ip2a(f->key.ip.ipv4.lower_ip, addr1),
            ip2a(f->key.ip.ipv4.upper_ip, addr2),
            f->key.lower_port,
            f->key.upper_port,
            f->key.protocol,
            f->pkt_cnt); 
        printf("%s\n", msg);
    }
}

/****************************************************************************
 * Override Functions from ComputeThreads
 ****************************************************************************/

void DPIBoltComputeThread::process(FLOW& f) {
    count++;

    if (!f.field.has_export) {
        uint32_t rest_len = f.field.o2r_data_cnt + f.field.r2o_data_cnt;
        uint32_t protocol = 0;
        struct iphdr* ip = (struct iphdr*)(&f.payload);
        uint32_t cnt = f.field.o2r_pkt_cnt + f.field.r2o_pkt_cnt;
        int has_build_flow = 0;
        while (rest_len > sizeof(struct iphdr) && rest_len > ntohs(ip->tot_len)) {
            uint32_t packet_len = ntohs(ip->tot_len);
            if (packet_len > rest_len) {
                packet_len = rest_len;
                ip->tot_len = htons(packet_len);
            }
            uint32_t ret_protocol;
            int ret = new_flow_processing(0, ip, packet_len, packet_len, &ret_protocol);
            if (ret) {
                LOG_DEBUG("ret: %d, Error when processing new flow, orig cnt: %u, current cnt: %u, cutted packet_len: %u, orig packet len: %u\n", ret, f.field.o2r_pkt_cnt+f.field.r2o_pkt_cnt, cnt, packet_len, ntohs(ip->tot_len));
                //char addr1[30], addr2[30];
                LOG_DEBUG("Error DPI flow: %s:%d <-> %s:%d\n", ip2a(f.field.orig_ip, addr1), f.field.orig_port, ip2a(f.field.resp_ip, addr2), f.field.resp_port);
                break;
            }
            has_build_flow = 1;
            if (ret_protocol != protocol) {
                if (protocol == 0 ) {
                    protocol = ret_protocol;
                }
                else if (ret_protocol != 0 && // unknown packet
                            ret_protocol != 7 // general HTTP packet
                            ) {
                    protocol = ret_protocol;
                }
            }
            ip = (struct iphdr*)((char*)ip + packet_len);
            rest_len -= packet_len;
            cnt--;
        }

	    ip_packet_count += f.field.o2r_pkt_cnt + f.field.r2o_pkt_cnt - f.field.export_pkt_cnt;
	    total_bytes += f.field.o2r_byte_cnt + f.field.r2o_byte_cnt - f.field.export_byte_cnt;
	    protocol_counter[protocol] += f.field.o2r_pkt_cnt + f.field.r2o_pkt_cnt - f.field.export_pkt_cnt;
	    protocol_counter_bytes[protocol] += f.field.o2r_byte_cnt + f.field.r2o_byte_cnt - f.field.export_byte_cnt;
        DPIBoltComputeThread::osdpi_flow* oflow = get_existed_osdpi_flow(&f);
        if (oflow == NULL && has_build_flow) {
            char addr1[30], addr2[30];
            LOG_ERR("DPIBoltComputeThread: cannot get existed osdpi flow for new flow: %s:%d <-> %s:%d, count %" PRIu64 "\n", ip2a(f.field.orig_ip, addr1), f.field.orig_port, ip2a(f.field.resp_ip, addr2), f.field.resp_port, count);
        }
        if (oflow) {
            oflow->pkt_cnt += f.field.o2r_pkt_cnt + f.field.r2o_pkt_cnt - f.field.export_pkt_cnt;
            oflow->byte_cnt += f.field.o2r_byte_cnt + f.field.r2o_byte_cnt - f.field.export_byte_cnt;
        }
        /*
            if ((f.field.resp_ip == 0x7f21700a  && f.field.orig_port == 3128 && f.field.resp_port == 1672) || (f.field.orig_ip == 0x7f21700a  && f.field.resp_port == 3128 && f.field.orig_port == 1672)) {
                char addr1[30], addr2[30];
                LOG_MSG("flow: %s:%d <-> %s:%d, detected: %u\n", ip2a(f.field.orig_ip, addr1), f.field.orig_port, ip2a(f.field.resp_ip, addr2), f.field.resp_port, protocol);
            }
            */
    }
    else {
        DPIBoltComputeThread::osdpi_flow* oflow = get_existed_osdpi_flow(&f);
        if (oflow == NULL) {
            /*
            char addr1[30], addr2[30];
            LOG_WARN("DPIBoltComputeThread: cannot get existed osdpi flow for old flow: %s:%d <-> %s:%d, count %" PRIu64 "\n", ip2a(f.field.orig_ip, addr1), f.field.orig_port, ip2a(f.field.resp_ip, addr2), f.field.resp_port, count);
            */
            failed_lookup_event++;
            u32 protocol = 0;
	        ip_packet_count += f.field.o2r_pkt_cnt + f.field.r2o_pkt_cnt - f.field.export_pkt_cnt;
	        total_bytes += f.field.o2r_byte_cnt + f.field.r2o_byte_cnt - f.field.export_byte_cnt;
	        protocol_counter[protocol] += f.field.o2r_pkt_cnt + f.field.r2o_pkt_cnt - f.field.export_pkt_cnt;
	        protocol_counter_bytes[protocol] += f.field.o2r_byte_cnt + f.field.r2o_byte_cnt - f.field.export_byte_cnt;
        }
        else {
            u32 protocol = oflow->detected_protocol;
	        ip_packet_count += f.field.o2r_pkt_cnt + f.field.r2o_pkt_cnt - f.field.export_pkt_cnt;
	        total_bytes += f.field.o2r_byte_cnt + f.field.r2o_byte_cnt - f.field.export_byte_cnt;
	        protocol_counter[protocol] += f.field.o2r_pkt_cnt + f.field.r2o_pkt_cnt - f.field.export_pkt_cnt;
	        protocol_counter_bytes[protocol] += f.field.o2r_byte_cnt + f.field.r2o_byte_cnt - f.field.export_byte_cnt;
            oflow->pkt_cnt += f.field.o2r_pkt_cnt + f.field.r2o_pkt_cnt - f.field.export_pkt_cnt;
            oflow->byte_cnt += f.field.o2r_byte_cnt + f.field.r2o_byte_cnt - f.field.export_byte_cnt;
        }
    }
}

void DPIBoltComputeThread::finish() {

    printResults();
    LOG_MSG("ComputeThread %d (%d) processed %" PRIu64 "\n", subId, getpid(), count);

	ipoque_exit_detection_module(ipoque_struct, free_wrapper);

    ComputeThread<FLOW, MainStateEvent>::finish();
}

/****************************************************************************
 * Private functions
 ****************************************************************************/

void* DPIBoltComputeThread::get_id(const struct osdpi_ip_addr *ip)
{
    osdpi_id* key_id = (struct osdpi_id*)ip;    // flow that stores the key
    struct osdpi_id* id;                        // pointer to the flow state

    id = osdpi_ids->find(key_id, 0, false);
    if (id == NULL) {
        id = osdpi_ids->find(key_id, count, true);
        if (id == NULL) {
            LOG_ERR("DPIBoltComputeThread Warning: can not get osdpi id\n");
        }
        osdpi_id_count++;
        id->ip = *ip;
    }

    return &id->ipoque_id;
}

struct DPIBoltComputeThread::osdpi_flow* DPIBoltComputeThread::get_osdpi_flow(const struct iphdr *iph, u16 ipsize)
{
	const u8 *l4 = NULL;
	u16 l4_len = 0;
	u8 l4_protocol = 0;
	struct ipoque_unique_flow_ipv4_and_6_struct key;
	u8 dir = 0;

	if (ipoque_detection_get_l4((u8*)iph, ipsize, &l4, &l4_len, &l4_protocol, 0) == 0) {
		if (ipoque_detection_build_key((u8*)iph, ipsize, l4, l4_len, l4_protocol, &key, &dir, 0) == 0) {

            osdpi_flow* key_flow = (struct osdpi_flow*)&key;
            struct osdpi_flow* flow;

            flow = osdpi_flows->find(key_flow, 0, false);
            if (flow != 0) {
                return flow;
            }

            flow = osdpi_flows->find(key_flow, count, true);
            osdpi_flow_count++;
            if (flow == NULL) {
                LOG_ERR("DPIBoltComputeThread Warning: can not get new osdpi id\n");
            }
            flow->key = key;
            return flow;
		}
	}

	return NULL;
}

struct DPIBoltComputeThread::osdpi_flow* DPIBoltComputeThread::get_existed_osdpi_flow(const FLOW* flow)
{
	struct ipoque_unique_flow_ipv4_and_6_struct key;
	u8 dir = 0;

	if (build_ipoque_key_from_flow(flow, &key, &dir, 0) == 0) {

        osdpi_flow* key_flow = (struct osdpi_flow*)&key;
        struct osdpi_flow* flow;
        flow = osdpi_flows->find(key_flow, count, false);
        return flow;
    }
	return NULL;
}

struct DPIBoltComputeThread::osdpi_flow* DPIBoltComputeThread::insert_osdpi_flow(const FLOW* flow)
{
	struct ipoque_unique_flow_ipv4_and_6_struct key;
	u8 dir = 0;

	if (build_ipoque_key_from_flow(flow, &key, &dir, 0) == 0) {

        osdpi_flow* key_flow = (struct osdpi_flow*)&key;
        struct osdpi_flow* flow;
        flow = osdpi_flows->find(key_flow, 0, false);
        if (flow == NULL) {
            flow = osdpi_flows->find(key_flow, count, true);
            if (flow == NULL) {
                LOG_ERR("DPIBoltComputeThread Warning: can not get inserted osdpi flow\n");
            }
            osdpi_flow_count++;
            flow->key = key;
        }
        return flow;
    }
	return NULL;
}

unsigned int DPIBoltComputeThread::new_flow_processing(const uint64_t time, const struct iphdr *iph, uint16_t ipsize, uint16_t rawsize, uint32_t* ret)
{
	struct ipoque_id_struct *src = NULL;
	struct ipoque_id_struct *dst = NULL;
	struct osdpi_flow *flow = NULL;
	struct ipoque_flow_struct *ipq_flow = NULL;
	u32 protocol = 0;

	struct osdpi_ip_addr src_ip, dst_ip;
	memset(&src_ip, 0, sizeof(struct osdpi_ip_addr));
	memset(&dst_ip, 0, sizeof(struct osdpi_ip_addr));

	if (iph->version == 6 && ipsize >= sizeof(struct ip6_hdr)) {
		struct ip6_hdr *ip6h = (struct ip6_hdr *)iph;
		src_ip.is_ip_v6 = 1;
		src_ip.ipv6 = ip6h->ip6_src;
		dst_ip.is_ip_v6 = 1;
		dst_ip.ipv6 = ip6h->ip6_dst;
	} else
	if (iph->version == 4 && ipsize >= sizeof(struct iphdr)) {
		src_ip.ipv4 = iph->saddr;
		dst_ip.ipv4 = iph->daddr;
	} else {
        LOG_DEBUG("Unknow L3 packet: version %u, size %u\n", iph->version, ipsize);
		return 1;
	}

	src = (struct ipoque_id_struct*)get_id(&src_ip);
	dst = (struct ipoque_id_struct*)get_id(&dst_ip);

    // qhuang: incomplete packet will return here
	flow = get_osdpi_flow(iph, ipsize);
	if (flow != NULL) {
		ipq_flow = (struct ipoque_flow_struct*)(flow->ipoque_flow);
	} else {
        //LOG_MSG("incomplete packet\n");
		return 2;
	}

    /*
	if (ip_packet_count % 49999 == 0) {
		printf("\rip packets scanned: \x1b[33m%-10" PRIu64 "\x1b[0m ip bytes scanned: \x1b[34m%-10" PRIu64 "\x1b[0m",
			   ip_packet_count, total_bytes);
	}
    */

	// only handle unfragmented packets
	if (iph->version == 4 && (iph->frag_off & htons(0x1FFF)) != 0) {

		static u8 frag_warning_used = 0;
		if (frag_warning_used == 0) {
			printf("\n\nWARNING: fragmented ip packets are not supported and will be skipped \n\n");
			sleep(2);
			frag_warning_used = 1;
		}
		printf("\n\nWARNING: fragmented ip packets are not supported and will be skipped \n\n");
        *ret = 0;
		return 0;

	} else {

		// here the actual detection is performed
		protocol = ipoque_detection_process_packet(ipoque_struct, ipq_flow, (uint8_t *) iph, ipsize, time, src, dst);

	}

    *ret = protocol;

	if (flow != NULL) {
		flow->detected_protocol = protocol;
	}

	return 0;
}

void DPIBoltComputeThread::printResults(void)
{
	u32 i;

    printf("%" PRIu64 " events fail to lookup the flow table\n", failed_lookup_event);
	printf("\x1b[2K\n");
	printf("pcap file contains\n");
	printf("\tip packets:   \x1b[33m%-13" PRIu64 "\x1b[0m\n", ip_packet_count);
	printf("\tip bytes:     \x1b[34m%-13" PRIu64 "\x1b[0m\n", total_bytes);
	printf("\tunique ids:   \x1b[35m%-13u\x1b[0m\n", osdpi_id_count);
	printf("\tunique flows: \x1b[36m%-13u\x1b[0m\n", osdpi_flow_count);

	printf("\n\ndetected protocols:\n");
	for (i = 0; i <= IPOQUE_MAX_SUPPORTED_PROTOCOLS; i++) {
		u32 protocol_flows = 0;

		if (protocol_counter[i] > 0) {
			printf("\t\x1b[31m%-20s\x1b[0m packets: \x1b[33m%-13" PRIu64 "\x1b[0m bytes: \x1b[34m%-13" PRIu64 "\x1b[0m "
				   "flows: \x1b[36m%-13u\x1b[0m\n",
				   prot_long_str[i].c_str(), protocol_counter[i], protocol_counter_bytes[i], protocol_flows);
		}
	}

    if (sizeof(DpiStat) <= CONTROL_MSG_MAX_SIZE) {
        DpiStat dpi_stat;
        dpi_stat.pkt_cnt = ip_packet_count;
        dpi_stat.byte_cnt = total_bytes;
        memcpy(dpi_stat.protocol_counter, protocol_counter, sizeof(protocol_counter));
        memcpy(dpi_stat.protocol_counter_bytes, protocol_counter_bytes, sizeof(protocol_counter_bytes));
        ControlMsg msg;
        msg.type = STAT_DATA;
        msg.length = sizeof(dpi_stat);
        memcpy(msg.data, &dpi_stat, sizeof(dpi_stat));
        while (this->controlQueue->write(&msg) == -1);
    }
    else {
        LOG_WARN("stat data is too large: %zd.\n", sizeof(DpiStat));
    }

    // print out all the flows
    //osdpi_flows->travel_table();
}

#ifdef LOAD_BALANCE
int DPIBoltComputeThread::get_exported_data(FLOW& f, ControlMsg* control_msg) {
    DPIBoltComputeThread::osdpi_flow* oflow = get_existed_osdpi_flow(&f);
    if (oflow == NULL) {
        LOG_WARN("DPIBoltComputeThread: cannot get existed flow for export\n");
        return -1;
    }

    if (oflow->is_in_coordinator) {
        return 1;
    }

    u32 protocol = oflow->detected_protocol;
    memcpy(control_msg->data, &f, 16);
    memcpy(control_msg->data+16, &protocol, sizeof(u32));
    control_msg->length = 16+sizeof(u32);
    get_state_key(f, control_msg);
    oflow->is_in_coordinator = 1;
    return 0;
}
    
int DPIBoltComputeThread::import_and_merge_data(char* data, uint32_t len) {
    char key_str[16];
    memcpy(key_str, data, 16);
    FLOW* f = (FLOW*)key_str;
    DPIBoltComputeThread::osdpi_flow* oflow = insert_osdpi_flow(f);
    if (oflow == NULL) {
        char msg[1024];
        char addr1[30];
        char addr2[30];
        sprintf(msg, "origip=%s respip=%s origport=%u respport=%u proto=%u, rest pkt: %u\n", 
            ip2a(f->field.orig_ip, addr1),
            ip2a(f->field.resp_ip, addr2),
            f->field.orig_port,
            f->field.resp_port,
            f->field.proto,
            f->field.o2r_pkt_cnt+f->field.r2o_pkt_cnt-f->field.export_pkt_cnt); 
        LOG_ERR("cannot get flow: %s\n", msg);
        return -1;
    }

    u32 tmp;
    memcpy(&tmp, data+16, sizeof(u32));
    if (tmp != oflow->detected_protocol) {
	    protocol_counter[oflow->detected_protocol] -= oflow->pkt_cnt;
        protocol_counter_bytes[oflow->detected_protocol] -= oflow->byte_cnt;
	    protocol_counter[tmp] += oflow->pkt_cnt;
        protocol_counter_bytes[tmp] += oflow->byte_cnt;
        LOG_DEBUG("change %llu from %u to %u\n", oflow->pkt_cnt, oflow->detected_protocol, tmp);
    }
    oflow->detected_protocol = tmp;
    oflow->is_in_coordinator = 1;
    //LOG_MSG("import protocol %u\n", oflow->detected_protocol);
    return 0;
}

void DPIBoltComputeThread::process_rebalanced_tuple(FLOW& f) {
    count++;

    if (!f.field.has_export) {
        LOG_WARN("Rebalance a unexported flow\n");
    }
    else {
        DPIBoltComputeThread::osdpi_flow* oflow = get_existed_osdpi_flow(&f);
        if (oflow == NULL) {
            oflow = insert_osdpi_flow(&f);
            if (oflow->detected_protocol == 0 && oflow->is_in_coordinator == 0) {
                request_import(f);
            }
        }
        if (oflow == NULL) {
            char addr1[30], addr2[30];
            LOG_ERR("DPIBoltComputeThread: cannot get existed osdpi flow for rebalanced flow: %s:%d <-> %s:%d, count %" PRIu64 "\n", ip2a(f.field.orig_ip, addr1), f.field.orig_port, ip2a(f.field.resp_ip, addr2), f.field.resp_port, count);
        }
        else {
            // set to unknown, wait for being merged later on
            // tricky: when no entry, it is 0 - unknown
            u32 protocol = oflow->detected_protocol;
	        ip_packet_count += f.field.o2r_pkt_cnt + f.field.r2o_pkt_cnt - f.field.export_pkt_cnt;
	        total_bytes += f.field.o2r_byte_cnt + f.field.r2o_byte_cnt - f.field.export_byte_cnt;
	        protocol_counter[protocol] += f.field.o2r_pkt_cnt + f.field.r2o_pkt_cnt - f.field.export_pkt_cnt;
	        protocol_counter_bytes[protocol] += f.field.o2r_byte_cnt + f.field.r2o_byte_cnt - f.field.export_byte_cnt;
            oflow->pkt_cnt += f.field.o2r_pkt_cnt + f.field.r2o_pkt_cnt - f.field.export_pkt_cnt;
            oflow->byte_cnt += f.field.o2r_byte_cnt + f.field.r2o_byte_cnt - f.field.export_byte_cnt;
        }
    }

}
#endif

/****************************************************************************
 * Public functions
 ****************************************************************************/
/*
 * Constructor
 */
DPIBoltComputeThread::DPIBoltComputeThread(std::string name, int id) : ComputeThread<FLOW, MainStateEvent>(name, id) {
    count = 0;
    ipoque_struct = NULL;
    detection_tick_resolution = 1000;

    ip_packet_count = 0;
    total_bytes = 0;

    size_id_struct = 0;
    osdpi_id_count = 0;

    size_flow_struct = 0;
    osdpi_flow_count = 0;

    Config *config = Config::getInstance();

    id_table_size = config->getint("osdpiIdTableSize", MAX_OSDPI_IDS);
    id_table_timeout = 10*id_table_size;

    flow_table_size = config->getint("osdpiFlowTableSize", MAX_OSDPI_FLOWS);
    flow_table_timeout = 10*id_table_size;

    failed_lookup_event = 0;
}
    
void DPIBoltComputeThread::initialize() {
	IPOQUE_PROTOCOL_BITMASK all;

	// init global detection structure
	ipoque_struct = ipoque_init_detection_module(detection_tick_resolution, malloc_wrapper, debug_printf);
	if (ipoque_struct == NULL) {
		LOG_ERR("ERROR: global structure initialization failed\n");
	}
	// enable all protocols
	IPOQUE_BITMASK_SET_ALL(all);
	ipoque_set_protocol_detection_bitmask2(ipoque_struct, &all);

	// allocate memory for id and flow tracking
	size_id_struct = ipoque_detection_get_sizeof_ipoque_id_struct();
	size_flow_struct = ipoque_detection_get_sizeof_ipoque_flow_struct();
    LOG_MSG("DPIBoltComputeThread: id size %u\n", size_id_struct);
    LOG_MSG("DPIBoltComputeThread: flow size %u\n", size_flow_struct);
    LOG_MSG("DPIBoltComputeThread: id table size %u\n", id_table_size);
    LOG_MSG("DPIBoltComputeThread: flow table size %u\n", flow_table_size);

    // allocate memory to store osdpi_ids
    osdpi_ids = new HashTable<struct osdpi_id>("osdpi_ids", 
            id_table_size, id_table_timeout,
            id_hash_fcn, id_cmp_fcn, 
            id_init_fcn, id_free_fcn, id_travel_do_fcn, this);
    if (osdpi_ids == NULL) {
        fprintf(stderr, "ERR: out of memory - create osdpi_ids table\n");
        exit(-1);
    }

    osdpi_flows = new HashTable<struct osdpi_flow>("osdpi_flows", 
            flow_table_size, flow_table_timeout,
            flow_hash_fcn, flow_cmp_fcn, 
            flow_init_fcn, flow_free_fcn, flow_travel_do_fcn, this);
	if (osdpi_flows == NULL) {
		LOG_ERR("ERROR: malloc for osdpi_flows failed\n");
	}

	// clear memory for results
	memset(protocol_counter, 0, (IPOQUE_MAX_SUPPORTED_PROTOCOLS + 1) * sizeof(uint64_t));
	memset(protocol_counter_bytes, 0, (IPOQUE_MAX_SUPPORTED_PROTOCOLS + 1) * sizeof(uint64_t));

    ComputeThread<FLOW, MainStateEvent>::initialize();
}
