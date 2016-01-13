#include "BasicDetector.hpp"

/*****************************************
 * private functions
 * **************************************/

void BasicDetector::output_cand(unsigned int no_cand, const char* outfile) {
    FILE* fp;
    fp = fopen(outfile, "w");
    for (unsigned int i = 0; i<no_cand; ++i) {
        if (n == 32) {
            char addr1[30];
            fprintf(fp, "%s\n", ip2a((*((unsigned int*)(cand_list[i]))), addr1));
        }
        else {
            char addr1[30];
            char addr2[30];
            fprintf(fp, "%s %s\n", ip2a((*((unsigned int*)(cand_list[i]))), addr1), ip2a((*((unsigned int*)(cand_list[i]+4))), addr2));
        }
    }
    fclose(fp);
}

/******************************************
 * public functions
 * called by ComputeThread.cpp
 * **************************************/

// called by init, and overriding functions
void BasicDetector::print_parameter() {
    if (is_heavy_changer) {
        LOG_MSG("%s: detect Heavy Changer\n", name.c_str());
    }
    else {
        LOG_MSG("%s: detect Heavy Hitter\n", name.c_str());
    }
    LOG_MSG("Common Setup: key length %u\n", n);
    LOG_MSG("Common Setup: maximum candidate key %u\n", MAX_CAND);
}

// called in ComputeThread::ComputeThread()
BasicDetector::BasicDetector(Config* config, unsigned int i) {
    is_heavy_changer = config->getboolean("is_heavy_changer", false);
    is_print_sketch = config->getboolean("isPrintSketch", false);
    n = config->getint("keyLength", 32);
    MAX_CAND = config->getint("maxNumCand", 100000);
    id = i;
}

// called in ComputeThread::init()
void BasicDetector::init() { 
    cand_list = (unsigned char**)calloc(MAX_CAND, sizeof(unsigned char*));
    for (unsigned int i=0; i<MAX_CAND; ++i) {
        cand_list[i] = (unsigned char*)calloc(n/8, sizeof(unsigned char));
    }
}

// called in ComputeThread::finish();
void BasicDetector::clean() {
    for (unsigned int i=0; i<MAX_CAND; ++i) {
        free(cand_list[i]);
    }
    free(cand_list);
} 

// called in ComputeThread::timeoutHander()
void BasicDetector::detect(double thresh, std::string& output_file) {
    
    // calculate diff sketch
    if (is_heavy_changer) {
        calculate_diff();
    }

    if (is_print_sketch) {
        print_internal_state(output_file);
    }
            
    // detect based on the currect/diff sketch
    //cand_list = AbsChange_Output(absc, thresh);
    unsigned int no_cand = fill_in_cand_list(thresh);
    output_cand(no_cand, ("cand_" + output_file).c_str());

    // reset
    //AbsChange_reset(absc);
    reset();
}
