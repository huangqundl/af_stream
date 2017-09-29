/*
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   *************************************************************************
   NOTE to contributors. This file comprises the principal public contract
   for AF-Stream API users. Any change to this file
   supplied in a stable release SHOULD not break existing applications.
   In practice this means that the value of constants must not change, and
   that old values may not be reused for new constants.
   *************************************************************************
*/

#ifndef __AFS_THREAD_HPP_INCLUDED__
#define __AFS_THREAD_HPP_INCLUDED__

#include <thread>

#include "thread_id.hpp"
#include "../control_channel/mailbox.hpp"

namespace afs {

    // base class for object which is executed as an individual thread
    
    enum ThreadType {t_ingress=0, t_egress, t_compute_thread};

    const char kThreadName[3][30] = {
        "Input Thread",
        "Output Thread",
        "ComputeThread Thread"
    };

    class ThreadBase {

    public:

        //  Constructor
        ThreadBase(ThreadType type);

        //  create/destroy threads
        void Create();
        void Destroy();

        //  thread rountine in C++ style
        void thread_fun();

    private:

        //  implemented by derived classes, called in thread_fun() 
        virtual void ThreadInitHandler() = 0;
        virtual void ThreadFinishHandler() = 0;
        virtual void ThreadMainHandler() = 0;

    public:

        //  communicate with main thread, called by main (worker) thread
        int64_t WaitThread(afs_zmq::command_t* cmd, bool is_block);
        void WaitThread(afs_zmq::command_t::type_t type); // block mode
        void NotifyThread(afs_zmq::command_t::type_t type);

    protected:

        // communicate with main thread, called by thread itself
        void NotifyWorker(afs_zmq::command_t &cmd);
        void NotifyWorker(afs_zmq::command_t::type_t type);
        void WaitWorker(afs_zmq::command_t::type_t type, bool is_block);

    public:

        //  Internal getters/setters.
        //  They are public so taht C-style thread routine can call them.
        int64_t get_tid() { return tid_; }
        int64_t get_wid() { return wid_; }
        void set_tid(int id) { tid_ = id; }
        void set_wid(int wid) { wid_ = wid; }
        char* get_thread_str() {
            return thread_str_;
        }

        void AssignCPU(int cpu) {
            cpu_ = cpu;
        }

    private:

        ThreadType type_;

        //  zero-based [worker id] and [thread id], assigned by worker
        //  -1 indicates unassigned
        int64_t wid_;
        int64_t tid_;

        //  CPU core
        //  -1 indicates unassigned
        int64_t cpu_;

        //  system-view [process id] and [thread (light-weight process) id]
        int64_t spid_;
        int64_t sppid_;
    
        //  thread information
        char thread_str_[50];

        //  pthread_t instance;
        std::thread* thread_instance_;

        //  communication channel with main thread
        afs_zmq::mailbox_t thread_to_main_;
        afs_zmq::mailbox_t main_to_thread_;
    };

} // namespace afs

#endif
