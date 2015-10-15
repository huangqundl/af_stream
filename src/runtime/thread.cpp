// License

#include <syscall.h>
#include <signal.h>

#include "thread.hpp"
#include "util.hpp"
#include "thread_id.hpp"

extern "C"
{
    static void *thread_routine (void *arg_)
    {
        //  Following code will guarantee more predictable latencies as it'll
        //  disallow any signal handling in the I/O thread.
        sigset_t signal_set;
        int rc = sigfillset (&signal_set);
        errno_assert (rc == 0);
        rc = pthread_sigmask (SIG_BLOCK, &signal_set, NULL);
        posix_assert (rc);
        
        // CPU Pin here

        afs::ThreadBase *self = (afs::ThreadBase*) arg_;   
        afs_assert(self->get_wid()!=-1, "worker id is not set\n");
        afs_assert(self->get_tid()!=-1, "thread id is not set\n");
        set_thr_id(self->get_tid());
        self->thread_fun();
        return NULL;
    }
}

void afs::ThreadBase::Create () {
    thread_instance_ = new std::thread(thread_routine, this);
}

void afs::ThreadBase::NotifyReady () {
    zmq::command_t cmd;
    cmd.type = zmq::command_t::ready;
    thread_to_main_.send(cmd);
}

void afs::ThreadBase::WaitReady () {
    zmq::command_t cmd;

    int rc = thread_to_main_.recv(&cmd, -1);
    afs_assert(rc==0, "%s calls WaitReady() error %d\n",
            thread_str_, rc);

    afs_assert(cmd.type == zmq::command_t::ready,
            "%s receive non-ready \n",
            thread_str_);
}

void afs::ThreadBase::NotifyStart () {
    zmq::command_t cmd;
    cmd.type = zmq::command_t::ready;
    main_to_thread_.send(cmd);
}

void afs::ThreadBase::WaitStart () {
    zmq::command_t cmd;

    LOG_MSG(INDENT "ready\n");
    int rc = main_to_thread_.recv(&cmd, -1);
    afs_assert(rc==0, "%s calls WaitReady() error %d\n",
            thread_str_, rc);

    afs_assert(cmd.type == zmq::command_t::ready,
            "%s receive non-ready\n",
            thread_str_);
}

void afs::ThreadBase::NotifyClean () {
    zmq::command_t cmd;
    cmd.type = zmq::command_t::clean;
    main_to_thread_.send(cmd);
}

void afs::ThreadBase::WaitClean () {
    zmq::command_t cmd;

    int rc = main_to_thread_.recv(&cmd, -1);
    afs_assert(rc==0, "%s calls WaitClean() error %d\n",
            thread_str_, rc);

    afs_assert(cmd.type == zmq::command_t::clean,
            "%s receive non-clean \n",
            thread_str_);
}

void afs::ThreadBase::NotifyFinish1 () {
    zmq::command_t cmd;
    cmd.type = zmq::command_t::finish;
    thread_to_main_.send(cmd);
}

void afs::ThreadBase::WaitFinish1 () {
    zmq::command_t cmd;

    int rc = thread_to_main_.recv(&cmd, -1);
    afs_assert(rc==0, "%s calls WaitFinish() error %d\n",
            thread_str_, rc);

    afs_assert(cmd.type == zmq::command_t::finish,
            "%s receive non-finish\n",
            thread_str_);
}

void afs::ThreadBase::NotifyFinish2 () {
    zmq::command_t cmd;
    cmd.type = zmq::command_t::finish;
    thread_to_main_.send(cmd);
}

void afs::ThreadBase::WaitFinish2 () {
    zmq::command_t cmd;

    int rc = thread_to_main_.recv(&cmd, -1);
    afs_assert(rc==0, "%s calls WaitFinish() error %d\n",
            thread_str_, rc);

    afs_assert(cmd.type == zmq::command_t::finish,
            "%s receive non-finish\n",
            thread_str_);
}

void afs::ThreadBase::Destroy () {
    thread_instance_->join();
}

void afs::ThreadBase::thread_fun() {
    // init
    spid_ = syscall(SYS_getpid);
    sppid_ = syscall(SYS_gettid);
    sprintf(thread_str_, "%s (%d,%d: %d,%d)", kThreadName[type_], wid_, tid_, spid_, sppid_);
    LOG_MSG("\n%s created\n", thread_str_);
    if (cpu_ >= 0) {
        pin_to_cpu(cpu_);
    }

    ThreadInitHandler();
    NotifyReady();

    WaitStart();
    ThreadMainHandler();

    NotifyFinish1();
    WaitClean();

    LOG_MSG("\n%s cleaning\n", thread_str_);
    ThreadFinishHandler();
    LOG_MSG("%s finished\n", thread_str_);

    NotifyFinish2();
}

/*
zmq::mailbox_t* afs::ThreadBase::get_mailbox() {
    return &mailbox_;
}
*/

afs::ThreadBase::ThreadBase(afs::ThreadType type) : 
    type_(type), wid_(-1), tid_(-1), cpu_(-1) {}
