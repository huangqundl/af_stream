/*
 * TCP Queue.hpp
 *
 * Data Queue to connect two components within same physical machine using TCP sockets
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

/*
class TCPQueue {
private:
    int sd;
    int client_sd;

public:
    TCPQueue(std::string endpoint, bool sender) {
        if (sender) {
            struct sockaddr_in server_addr;
            struct sockaddr_in c_addr;
            
            uint16_t port;
            sscanf(endpoint.c_str(), "tcp://*:%hu", &port);

            sd = socket(AF_INET, SOCK_STREAM, 0);
            long val=1;
            if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(long)) ==-1) {
                LOG_MSG("set socket error\n");
                exit(-1);
            }

            memset(&server_addr, 0, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
            server_addr.sin_port = htons(port);

            if (bind(sd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
                LOG_ERR("bind error\n");
                exit(-1);
            }
    
            if (listen(sd, 1) < 0) {
                LOG_ERR("listen error\n");
                exit(-1);
            }

            uint32_t addr_len = sizeof(c_addr);
            if ((client_sd=accept(sd, (struct sockaddr*)&c_addr, &addr_len))<0) {
                LOG_ERR("accept error\n");
                exit(-1);
            }
            LOG_MSG("Get request from IP: %s, Port: %d\n", (char*)inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
        }
        else {
        }
    }

    virtual ~TCPQueue() {
        close(sd);
        close(client_sd);
    }
};
*/

void send_data(int fd, char* data, int size) {
    int len;
    uint32_t already_sent = 0;
    while (1) {
        if ((len=send(fd, data+already_sent, size-already_sent, 0)) < 0) {
            fprintf(stderr, ("TCP Queue: error when send event\n"));
            return;
        }
        already_sent += len;
        if (already_sent >= size) {
            break;
        }
    }
}

void recv_data(int fd, char* data, int size) {
    uint32_t already_recv = 0;
    int len;
    while (1) {
        if ((len=recv(fd, data+already_recv, size, 0))<0) {
            fprintf(stderr, "TCP Queue: error when read event %d\n", len);
            return;
        }
        already_recv += len;
        if (already_recv >= size) {
            break;
        }
    }
}

int main (int argc, char *argv [])
{
    if (argc != 2) {
        printf ("usage: %s <address>\n", argv[0]);
        return 1;
    }

            
    char ip[30];
    int ip1, ip2, ip3, ip4;
    uint16_t port;
    sscanf(argv[1], "tcp://%d.%d.%d.%d:%hu", &ip1, &ip2, &ip3, &ip4, &port);
    sprintf(ip, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);
    
    int sd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);
    
    if (connect(sd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0) {
        fprintf(stderr, "connect error\n");
    }
    else {
        fprintf(stderr, "Connected IP:%s, Port:%d\n",
                (char*)inet_ntoa(server_addr.sin_addr),
                port);
    }

    close(sd);
    return 0;
}
