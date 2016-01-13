#ifndef __DCHOICE__hpp__
#define __DCHOICE__hpp__

#include <stdint.h>

#include "Dispatcher/Router/Router.hpp"
#include "common.hpp"
#include "../Packet.hpp"

/**
 * For the distributed model of LD-Sketch,
 * the d-choice strategy to assign packets to workers.
 *
 * The set of workers for key x is determined using base and step.
 * Specifically, the d workers for key x are indexed by:
 * base%q,
 * (base+step)%q,
 * (base+2*step)%q,
 * ...,
 * (base+(d-1)*step)%q.
 *
 */
class DChoice : public Router <Packet::BasicField> {
    /**
     * number of workers
     */
    unsigned int q;

    /**
     * number of choices for each key
     */
    unsigned int d;
    
    /**
     * set of step values, step must satisfy (step, q)=1
     */
    unsigned int* step_set;

    /**
     * size of the set of step values
     */
    unsigned int num_step;

private:
    /**
     * calculate base value by hashing the key
     * @param data binary of key
     * @param n length of the binary
     */
    unsigned int base(const unsigned char* data, unsigned int n) {
        unsigned int hash = 244853197;
        unsigned int scale = 497302969;
        unsigned int hardener  = 867824724;
        while (n) {
            hash *= scale;
            hash += *data++;
            n--;
        }
        return (hash ^ hardener) % q;
    }

    /**
     * calculate step value by hashing the key
     * @param data binary of key
     * @param n length of the binary
     */
    unsigned int step(const unsigned char* data, unsigned int n) {
        unsigned int hash = 3595152499;
        unsigned int scale = 187951579;
        unsigned int hardener  = 945798103;
        while (n) {
            hash *= scale;
            hash += *data++;
            n--;
        }
        unsigned int index = (hash ^ hardener) % num_step;
        return step_set[index];
    }

    /**
     * select one from the d workers by hashing the key
     * @param data binary of key
     * @param n length of the binary
     */
    unsigned int index(const unsigned char* data, unsigned int n) {
        unsigned int hash = 79201883;
        unsigned int scale = 932901581;
        unsigned int hardener  = 387943509;
        while (n) {
            hash *= scale;
            hash += *data++;
            n--;
        }
        return (hash ^ hardener) % d;
    }

    /**
     * the greatest commond dividors
     * @param a value a
     * @param b value b
     */
    unsigned int gcd(unsigned int a, unsigned int b) {
        if (a % b == 0)
            return b;
        else
            return gcd(b, a % b);
    }

public:
    /**
     * Constructor
     * @param q_input number of workers
     * @param d_input number of choices
     */
    DChoice(unsigned int q_input, unsigned int d_input): q(q_input), d(d_input) {
        step_set = NULL;
    }

    ~DChoice(){
        if (step_set != NULL)
            free(step_set);
    }

    virtual void initialize(void* context) {
        num_step = 0;
        step_set = (unsigned int*)calloc(q, sizeof(unsigned int));
        LOG_MSG("DChoice: select %u of %u workers\n", d, q);

        for (unsigned int i=1; i<q; ++i) {
            if (gcd(q, i) == 1) {
                step_set[num_step] = i;
                num_step++;
            }
        }

        for (unsigned int i=0; i<num_step; ++i) {
            LOG_MSG("%u ", step_set[i]);
        }
        LOG_MSG("\n");
    }

	int balance(Packet::BasicField* bf, int* flags=NULL) {
        if (q == 1)
            return 0;

        unsigned char key[16];
        memcpy(key, &bf->src_ip, 4);
        memcpy(key+4, &bf->dst_ip, 4);
        unsigned int base_value = base(key, 8);
        unsigned int step_value = step(key, 8);

        unsigned int index_value = 0;
        if (bf->pre_assign != -1) {
            index_value = bf->pre_assign;
        }
        else {
            memcpy(key+8, &bf->seq, 8);
            index_value = index(key, 16);
        }

        unsigned int ret = (base_value + step_value * index_value) % q;
        return ret;
    }
};

#endif
