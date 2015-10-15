/*
 * TCP Queue.hpp
 *
 * Data Queue to connect two components within same physical machine using TCP sockets
 */

#ifndef __TCP_QUEUE_HPP__
#define __TCP_QUEUE_HPP__

#include <string>

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "BaseQueue.hpp"
#include "Util.hpp"

template <class T>
class TCPQueue : BaseQueue<T> {
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
            struct sockaddr_in server_addr;
            
            char ip[30];
            int ip1, ip2, ip3, ip4;
            uint16_t port;
            sscanf(endpoint.c_str(), "tcp://%d.%d.%d.%d:%hu", &ip1, &ip2, &ip3, &ip4, &port);
            sprintf(ip, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);
    
            sd = socket(AF_INET, SOCK_STREAM, 0);
            memset(&server_addr, 0, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = inet_addr(ip);
            server_addr.sin_port = htons(port);
    
            if (connect(sd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0) {
                LOG_MSG("connect error\n");
            }
            LOG_MSG("Connected IP:%s, Port:%d\n", (char*)inet_ntoa(server_addr.sin_addr), port);
        }
    }

    virtual ~TCPQueue() {
        close(sd);
        close(client_sd);
    }

    /*
     * Insert data.
     * Return 0 if success, otherwise return -1
     */
	int write(T* data) {
        uint32_t size = sizeof(T);
        int len;
        uint32_t already_sent = 0;
        while (1) {
            if ((len=send(client_sd, data+already_sent, size-already_sent, 0)) < 0) {
                LOG_ERR("TCP Queue: error when send event %d\n", errno);
                return -1;
            }
            already_sent += len;
            if (already_sent >= size) {
                break;
            }
        }
        return 0;
    }

    /*
     * Extract data.
     * Return 0 if success, otherwise return -1
     */
	int read(T* data) {
        uint32_t already_recv = 0;
        int len;
        while (1) {
            if ((len=recv(sd, data+already_recv, sizeof(T), 0))<0) {
                LOG_ERR("TCP Queue: error when read event %d %d\n", len, errno);
                return -1;
            }
            already_recv += len;
            if (already_recv >= sizeof(T)) {
                break;
            }
        }
        return 0;
    }
};

#endif
