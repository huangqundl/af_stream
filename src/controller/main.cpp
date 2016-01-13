#include "zk_controller_client.hpp"
//#include "../Applications/FlowDPI/SpoutDispatcher.hpp"
//#include "../Applications/FlowDPI/DPIBoltAnalyzer.hpp"

zhandle_t* CoordinatorClient::zh = NULL;
//std::string CoordinatorClient::server_name = "192.168.102.19:2181";
clientid_t CoordinatorClient::myid;
int CoordinatorClient::num_of_workers = 0;

class DemoCoordinator : public CoordinatorClient {
public:
    /*
    void output_stat() {
        printf("\nStatistics\n");
        printf("==========\n");

        struct String_vector strings;
        int rc = zoo_get_children(zh, NODE_NAME_STAT, 0, &strings);
        if (rc) {
            LOG_MSG("Error %d for get child %s\n", rc, NODE_NAME_STAT);
            return;
        }

        PacketStat packet_stat;
        memset(&packet_stat, 0, sizeof(packet_stat));
        packet_stat.used_time = 1;
        DpiStat dpi_stat;
        memset(&dpi_stat, 0, sizeof(dpi_stat));

        for (int i = 0; i < strings.count; i++) {
            char path[50];
            sprintf(path, NODE_NAME_STAT "/%s", strings.data[i]);
            struct Stat stat;
            if (strncmp(strings.data[i], "FlowDPI_Flowspout", strlen("FlowDPI_Flowspout")) == 0) {
                PacketStat data;
                int len = sizeof(data);
                rc = zoo_get(zh, path, 0, (char*) (&data), &len, &stat);
                if (rc || len != sizeof(data))
                    continue;
                packet_stat.tot_pkt_cnt += data.tot_pkt_cnt;
                packet_stat.tot_byte_cnt += data.tot_byte_cnt;
                packet_stat.valid_pkt_cnt += data.valid_pkt_cnt;
                packet_stat.valid_byte_cnt += data.valid_byte_cnt;
                packet_stat.non_ip_cnt += data.non_ip_cnt;
                packet_stat.ip_not_full_cnt += data.ip_not_full_cnt;
                packet_stat.ip_ver_fail_cnt += data.ip_ver_fail_cnt;
                packet_stat.used_time = data.used_time > packet_stat.used_time ? data.used_time : packet_stat.used_time;
            }
            if (strncmp(strings.data[i], "FlowDPI_DPIbolt", strlen("FlowDPI_DPIbolt")) == 0) {
                DpiStat data;
                int len = sizeof(data);
                rc = zoo_get(zh, path, 0, (char*) (&data), &len, &stat);
                if (rc || len != sizeof(data))
                    continue;
                dpi_stat.pkt_cnt += data.pkt_cnt;
                dpi_stat.byte_cnt += data.byte_cnt;
                for (int i = 0; i <= IPOQUE_MAX_SUPPORTED_PROTOCOLS; i++) {
                    dpi_stat.protocol_counter[i] += data.protocol_counter[i];
                    dpi_stat.protocol_counter_bytes[i] += data.protocol_counter_bytes[i];
                }
            }
        }

        printf("\nDispatcher Stat\n"
               "---------------\n"
               "\tTotal packets observed: %" PRIu64 "\n"
               "\tTotal bytes observed: %" PRIu64 "\n"
               "\tValid packet count: %" PRIu64 "\n"
               "\tValid byte count: %" PRIu64 "\n"
               "\tNon-IP packet count: %" PRIu64 "\n"
               "\tIP-not-full packet count: %" PRIu64 "\n"
               "\tIP-version-failed packet count: %" PRIu64 "\n"
               "\tTCP-not-full packet count: %" PRIu64 "\n"
               "\tUsed time: %.2lf s\n"
               "\tSpeed: %.2lf Gb/s\n",
               packet_stat.tot_pkt_cnt,
               packet_stat.tot_byte_cnt,
               packet_stat.valid_pkt_cnt,
               packet_stat.valid_byte_cnt,
               packet_stat.non_ip_cnt,
               packet_stat.ip_not_full_cnt,
               packet_stat.ip_ver_fail_cnt,
               packet_stat.tcp_not_full_cnt,
               packet_stat.used_time / 1.0e6,
               packet_stat.tot_byte_cnt / 1.0e3 * 8 / packet_stat.used_time);

        std::string prot_long_str[] = { IPOQUE_PROTOCOL_LONG_STRING };

        printf("\nDPI Results\n");
        printf("-----------\n");
        printf("\tip packets:   \x1b[33m%-13" PRIu64 "\x1b[0m\n", dpi_stat.pkt_cnt);
        printf("\tip bytes:     \x1b[34m%-13" PRIu64 "\x1b[0m\n", dpi_stat.byte_cnt);

        printf("\n\ndetected protocols:\n");
        for (int i = 0; i <= IPOQUE_MAX_SUPPORTED_PROTOCOLS; i++) {
            if (dpi_stat.protocol_counter[i] > 0) {
                printf("\t\x1b[31m%-20s\x1b[0m packets: \x1b[33m%-13" PRIu64 "\x1b[0m bytes: \x1b[34m%-13" PRIu64 "\x1b[0m\n",
                       prot_long_str[i].c_str(), dpi_stat.protocol_counter[i], dpi_stat.protocol_counter_bytes[i]);
            }
        }
    }
    */
};

int main(int argc, char* argv[]) {
    DemoCoordinator client;

    if (argc != 2) {
        LOG_MSG("Usage: %s [Zookeeper server addr]\n", argv[0]);
        exit(1);
    }

    client.set_server_name(argv[1]);

    if (client.initialize() == 0) {
        char str[50];
        while (true) {
            printf("$ ");
            if (fgets(str, 50, stdin) == NULL)
                break;
            char* cmd = strtok(str, " \n");
            if (cmd == NULL)
                continue;
            if (strcmp(cmd, "quit") == 0) {
                client.finish();
                break;
            }
            /*
            else if (strcmp(cmd, "output_stat") == 0) {
                client.output_stat();
            }
            */
        }
    }
    else {
        fprintf(stderr, "init error\n");
    }

    return 0;
}
