// License

#ifndef _THREAD_HPP
#define _THREAD_HPP

#include <thread>
#include "thread_id.hpp"
#include "control_channel/mailbox.hpp"
#include "fault_tolerance/backup_item.hpp"

namespace afs {

    // base class for object which is executed as an individual thread
    
    enum ThreadType {t_ingress=0, t_egress, t_compute_thread};

    const char kThreadName[3][30] = {
        "Input Thread",
        "Output Thread",
        "ComputeThread Thread"
    };

    const char kCpuAffinityKey[3][40] = {
        "up_thread_affinity",
        "down_thread_affinity",
        "compute_thread_affinity"
    };

    class ThreadBase {

    public:
        void Create();
        void Destroy();

        // communicate with main thread, called by main thread
        int WaitThread(afs_zmq::command_t* cmd, bool is_block);
        void NotifyThread(afs_zmq::command_t::type_t type);

        // the following functions are deprecated
        void WaitReady();
        void NotifyStart();
        void NotifyClean();
        void WaitFinish1();
        void WaitFinish2();

        ThreadBase(ThreadType type);

        //  These are internal members. However, they would not be accessible from the main C routine of the thread if they are private.
        int get_tid() { return tid_; }
        int get_wid() { return wid_; }
        void set_tid(int id) { tid_ = id; }
        void set_wid(int wid) { wid_ = wid; }
        void thread_fun();

        void AssignCPU(int cpu) {
            cpu_ = cpu;
        }

        //bool IsRun() { return is_run_; }

    protected:
        char* get_thread_str() {
            return thread_str_;
        }

        //void SendBackup(BackupItem &item);
        void SendWorker(afs_zmq::command_t &cmd);
        void WaitWorker(afs_zmq::command_t::type_t type, bool is_block);

    private:
        ThreadType type_;
        bool is_run_;

        // zero-based [worker id] and [thread id], assigned by worker
        int wid_;
        int tid_;
        int cpu_;

        // system-view [process id] and [thread (light-weight process) id]
        int spid_;
        int sppid_;
    
        // thread information
        char thread_str_[50];

        //void SendToWorker();

        // communicate with main thread, called by thread itself
        void NotifyWorker(afs_zmq::command_t::type_t type);
        // the following functions are deprecated
        void NotifyReady();
        void WaitStart();
        void WaitClean();
        void NotifyFinish1();
        void NotifyFinish2();

        //pthread_t descriptor;
        std::thread* thread_instance_;

        // communication channel with main thread
        afs_zmq::mailbox_t thread_to_main_;
        afs_zmq::mailbox_t main_to_thread_;


        // implemented by derived classes, called in corresponing functions
        virtual void ThreadInitHandler() = 0;
        virtual void ThreadFinishHandler() = 0;
        virtual void ThreadMainHandler() = 0;
    };

} // namespace afs

#endif // _I_RUNNABLE_HPP
