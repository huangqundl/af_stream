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

#include "thread.hpp"

#include <syscall.h>
#include <signal.h>

#include "util.hpp"

extern "C"
{
    //  Thread rountine in C style
    static void *thread_routine (void *arg_)
    {
        //  Following code will guarantee more predictable latencies as it'll
        //  disallow any signal handling in the I/O thread.
        sigset_t signal_set;
        int rc = sigfillset (&signal_set);
        errno_assert (rc == 0);
        rc = pthread_sigmask (SIG_BLOCK, &signal_set, NULL);
        posix_assert (rc);

        afs::ThreadBase *self = (afs::ThreadBase*) arg_;   
        afs_assert(self->get_wid()!=-1, "worker id is not set\n");
        afs_assert(self->get_tid()!=-1, "thread id is not set\n");
        afs::set_thr_id(self->get_tid());

        //  Invoke C++ style thread routine
        self->thread_fun();

        return NULL;
    }
}

afs::ThreadBase::ThreadBase(afs::ThreadType type) : 
    type_(type), wid_(-1), tid_(-1), cpu_(-1) {}

void afs::ThreadBase::Create () {
    thread_instance_ = new std::thread(thread_routine, this);
}

void afs::ThreadBase::Destroy () {
    thread_instance_->join();
}

int64_t afs::ThreadBase::WaitThread (afs_zmq::command_t* cmd, bool is_block) {

    int64_t rc = -1;
    if (is_block) {
        rc = thread_to_main_.recv(cmd, -1);
        afs_assert(rc==0, "%s calls WaitThread() error %ld\n",
                thread_str_, rc);
    }
    else {
        rc = thread_to_main_.recv(cmd, 0);
        if (rc != 0) {
            if (errno == EAGAIN || errno == EINTR) {
                return -1;
            }
            afs_assert(0, "%s WaitThread unknown error\n",
                thread_str_);
        }
    }
        
    return cmd->type;
}

void afs::ThreadBase::WaitThread (afs_zmq::command_t::type_t type) {

    afs_zmq::command_t cmd;

    int rc = thread_to_main_.recv(&cmd, -1);
    afs_assert(rc==0, "%s calls WaitThread() error %d\n",
            thread_str_, rc);

    afs_assert(cmd.type == type,
            "%s receive wrong message type \n",
            thread_str_);
}

void afs::ThreadBase::NotifyThread (afs_zmq::command_t::type_t type) {
    afs_zmq::command_t cmd;
    cmd.type = type;
    main_to_thread_.send(cmd);
}

void afs::ThreadBase::WaitWorker (afs_zmq::command_t::type_t type, bool is_block) {
    afs_zmq::command_t cmd;

    int rc = -1;
    if (is_block) {
        rc = main_to_thread_.recv(&cmd, -1);
        afs_assert(cmd.type == type, "%s WaitWorker wrong type\n", thread_str_);
    }
    else {
        rc = main_to_thread_.recv(&cmd, 0);
    }
    afs_assert(rc==0, "%s calls WaitWorker() error %d\n",
            thread_str_, rc);
}

void afs::ThreadBase::NotifyWorker(afs_zmq::command_t &cmd) {
    thread_to_main_.send(cmd);
}

void afs::ThreadBase::NotifyWorker(afs_zmq::command_t::type_t type) {
    afs_zmq::command_t cmd;
    cmd.type = type;
    thread_to_main_.send(cmd);
}

void afs::ThreadBase::thread_fun() {
    // init
    spid_ = syscall(SYS_getpid);
    sppid_ = syscall(SYS_gettid);
    sprintf(thread_str_, "%s (%ld,%ld: %ld,%ld)", kThreadName[type_], wid_, tid_, spid_, sppid_);
    LOG_MSG("\n%s created\n", thread_str_);
    if (cpu_ >= 0) {
        pin_to_cpu(cpu_);
    }

    ThreadInitHandler();
    NotifyWorker(afs_zmq::command_t::ready);

    WaitWorker(afs_zmq::command_t::ready, true);
    ThreadMainHandler();

    NotifyWorker(afs_zmq::command_t::finish);
    WaitWorker(afs_zmq::command_t::clean, true);

    LOG_MSG("\n%s cleaning\n", thread_str_);
    ThreadFinishHandler();
    LOG_MSG("%s finished\n", thread_str_);

    NotifyWorker(afs_zmq::command_t::finish);
}
