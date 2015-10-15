// License

#ifndef _THREAD_HPP
#define _THREAD_HPP

#include <thread>
#include "control_channel/mailbox.hpp"

namespace afs {

    // base class for object which is executed as an individual thread
    
    enum ThreadType {t_ingress=0, t_egress, t_analyzer};

    const char kThreadName[3][20] = {
        "Input Thread",
        "Output Thread",
        "Analyzer Thread"
    };

    const char kCpuAffinityKey[3][20] = {
        "in_thread_affinity",
        "out_thread_affinity",
        "analyzer_affinity"
    };

    class ThreadBase {

    public:
        void Create();
        void Destroy();

        // communicate with main thread, called by main thread
        void WaitReady();
        void NotifyStart();
        void NotifyClean();
        void WaitFinish1();
        void WaitFinish2();

        //  These are internal members. They should be private, however then
        //  they would not be accessible from the main C routine of the thread.
        void thread_fun();
        //thread_fn *tfn;
        //void *arg;

        ThreadBase(ThreadType type);

        int get_tid() {
            return tid_;
        }

        void set_tid(int id) {
            tid_ = id;
        }

        int get_wid() {
            return wid_;
        }

        void set_wid(int wid) {
            wid_ = wid;
        }

        void AssignCPU(int cpu) {
            cpu_ = cpu;
        }

    protected:
        char* get_thread_str() {
            return thread_str_;
        }

    private:
        ThreadType type_;

        // zero-based [worker id] and [thread id], assigned by worker
        int wid_;
        int tid_;
        int cpu_;

        // system-view [process id] and [thread (light-weight process) id]
        int spid_;
        int sppid_;
    
        // thread information
        char thread_str_[50];

        // communicate with main thread, called by thread itself
        void NotifyReady();
        void WaitStart();
        void WaitClean();
        void NotifyFinish1();
        void NotifyFinish2();

        //pthread_t descriptor;
        std::thread* thread_instance_;

        // communication channel with main thread
        zmq::mailbox_t thread_to_main_;
        zmq::mailbox_t main_to_thread_;


        // implemented by derived classes, called in corresponing functions
        virtual void ThreadInitHandler() = 0;
        virtual void ThreadFinishHandler() = 0;
        virtual void ThreadMainHandler() = 0;
    };

} // namespace afs

#endif // _I_RUNNABLE_HPP
