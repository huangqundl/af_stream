#include "WorkerClient.hpp"

zhandle_t* WorkerClient::zh = NULL;
std::string WorkerClient::server_name = "127.0.0.1:2181";
clientid_t WorkerClient::myid;
set<string> WorkerClient::upstream_workers;
deque<ControlMsg> WorkerClient::remoteCmd;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of argument\n");
        return -1;
    }

    WorkerClient client(argv[1]);

    if (client.initialize() == 0) {
        srand((int)getpid());
        int work = rand() % 10;
        LOG_MSG("Work for %d seconds\n", work);
        sleep(work);

        while (!client.is_done()) {
            sleep(5);
        }
        client.finish();
    }
    else {
        fprintf(stderr, "init error\n");
    }

    return 0;
}
