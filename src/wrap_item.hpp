#ifndef __ROLL_WRAP_ITEM_HPP_INCLUDED__
#define __ROLL_WRAP_ITEM_HPP_INCLUDED__

#include <stdio.h>

namespace afs {

    enum ITEM_TYPE {
        ITEM_NORMAL,
        ITEM_FINISH,
        ITEM_TIMEOUT,
        ITEM_REVERSE,
        ITEM_ACK
    };

    template<class T>
    class WrapItem {

    public:
        WrapItem<T>() : type_(ITEM_NORMAL), seq_(0) {}

        inline ITEM_TYPE get_type() {
            return type_;
        }

        inline void set_type(ITEM_TYPE t) {
            type_ = t;
        }

        inline uint64_t get_seq() {
            return seq_;
        }

        inline void inc_seq() {
            seq_++;
        }

        inline void set_seq(uint64_t s) {
            seq_ = s;
        }

        inline void set_thr_source(int s) {
            t_source_ = s;
        }

        inline int get_thr_source() {
            return t_source_;
        }

        inline void set_worker_source(int s) {
            w_source_ = s;
        }

        inline int get_worker_source() {
            return w_source_;
        }

        inline T& data() {
            return data_;
        }

        /*
        inline void set_data(T& data) {
            data_ = data;
        }
        */

    private:

        ITEM_TYPE type_;
        uint64_t seq_;

        // set and get in analyzers
        uint32_t t_source_;

        // set by analyzer
        // get by in_callback
        uint32_t w_source_;

        T data_;
    };

    class NullClass {
    public:
        NullClass& operator= (NullClass& i) {
            return *this;
        }
    };

    typedef WrapItem<NullClass> WNullClass;
}

#endif // include 
