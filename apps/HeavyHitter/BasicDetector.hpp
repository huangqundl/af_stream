/*
 * BasicDetector.hpp
 * - General basic class for vaious heavy hitter/changer detection
 * - Independent of platform
 * -
 * - Author: HUANG Qun
 * - qhuang@cse.cuhk.edu.hk
 * - Data: 20130728
 */


#ifndef __Detector_hpp__
#define __Detector_hpp__

#include <string>
#include "util.hpp"
#include "common.hpp"
#include "Config.hpp"

#define MB          (1024*1024)

/**
 * Class of detection algorithm
 */
class BasicDetector {

private:
    /// reset all internal data structures
    virtual void reset() = 0;

    /// retrive detected candidate from internal data structure, and fill them into the cand_list
    virtual unsigned int fill_in_cand_list(double thresh) = 0;

    /// print internal state in plaintext for debug and test
    virtual void print_internal_state(std::string output_file) = 0;

    /// print out detected keys
    //@param no_cand number of keys
    //@param outfile name of output file
    void output_cand(unsigned int no_cand, const char* outfile);

    /// calculate difference state between two adjancent time interval
    virtual void calculate_diff() = 0;

protected:

    // common fields for all detecotrs
    
    /// ID of the detector
    unsigned int id;

    /// name of detector algorithm
    std::string name;           

    /// whether it is heavy changer detector, otherwise heavy hitter
    bool is_heavy_changer;

    /// whether to print out the sketch
    bool is_print_sketch;

    /// length of key in bit, typically 32 or 64, or 104 (five tuple flow)
    unsigned int n;             

    /// maximum number of detected keys allowed
    unsigned int MAX_CAND;

    /// detected keys
    unsigned char** cand_list;

public:

    /// Construction
    BasicDetector(Config* config, unsigned int worker_id);

    /// allocate memory for internal structure
    virtual void init();

    /// print initialized parameters
    virtual void print_parameter();

    /// update when a new packet arrivies
    virtual void update(unsigned char* key, long long value) = 0;

    /// detect and output heavy keys (typically at the end of time interval)
    /// thresh is in MB, i.e. output key larger than thresh MB
    void detect(double thresh, std::string& output_file);

    /// cleaning
    virtual void clean();

    /// get the name of the detector
    std::string getName() {return name;}

};
#endif
