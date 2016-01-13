#include <string>
#include <unordered_map>
#include <random>
#include <sstream>

#include "compute_thread.hpp"
#include "raw_item.hpp"
#include "operator/hashmap_l1norm.hpp"

class ComputeThreadThread : public afs::ComputeThread<afs::RawItem, afs::NullClass, afs::NullClass, afs::RawItem> {
public:
    ComputeThreadThread(int num_out_queue, afs::RouterBase* r) :
        afs::ComputeThread<afs::RawItem, afs::NullClass, afs::NullClass, afs::RawItem>(num_out_queue, r) {}

private:

    //unordered_map<int,double> weights;
    afs::HashMapL1* weights;
    unordered_map<size_t, double> total_l1;
    random_device rd;

    // constant
    double alpha;
    double l1;
    double eps;

    // updated in learning
    double mu;
    double norm;

    // stat
    size_t n;
    double tot_err;
    double predict_err;

    unsigned long long int max_train;

    FILE* output_convergence;
    unordered_map<size_t, double> old_weights;

    void ComputeThreadInit() {
        Config* config = Config::getInstance();

        alpha = config->getdouble("alpha", 0);
        afs_assert(alpha, "learning rate alpha is not set\n");

        l1 = config->getdouble("l1", 0);
        afs_assert(l1, "l1 is not set\n");

        eps = config->getdouble("eps", 0);
        afs_assert(eps, "eps is not set\n");

        mu = 0.0;
        norm = 1.0;

        n = 0;
        tot_err = 0;
        predict_err = 0;

        char* filename_str = config->getstring("convergence_file", NULL);
        afs_assert(filename_str, "convergence_file is not set\n");
        std::string filename = filename_str;
        filename += "_" + std::to_string(get_tid());
        output_convergence = fopen(filename.c_str(), "w");

        double max_state_err = config->getdouble("max_state_err", 0);
        weights = new afs::HashMapL1(NULL, max_state_err);

        max_train = config->get_ull("logistic_regression.max_train", 0);
        LOG_MSG("max train %llu\n", max_train);
    }

    void ComputeThreadFinish() {
        fprintf(output_convergence, "stop at %lu\n", n);
        fclose(output_convergence);

        Config* config = Config::getInstance();
        char* filename_str = config->getstring("model_file", NULL);
        afs_assert(filename_str, "model_file is not set\n");
        std::string filename = filename_str;
        filename += "_" + std::to_string(get_tid());
        FILE* model_file = fopen(filename.c_str(), "w");
        std::unordered_map<size_t, double> model = weights->get_entire();
        for(auto it = model.begin(); it != model.end(); it++){
            fprintf(model_file, "%lu %lf\n", it->first, it->second);
        }
        fclose(model_file);
    }

    void ComputeThreadRecovery() {}

    void ProcessRecord(afs::RawItem& tuple, uint64_t seq) {
        //LOG_MSG("%s\n", (char*)tuple.raw_data);

        // TODO: parse
        unordered_map<size_t,double> data;
        unordered_map<size_t,double> model;

        std::string line = tuple.raw_data;
        if(line.length()){
            if(line[0] != '#' && line[0] != ' '){
                vector<string> tokens = split(line,' ');
                if(atoi(tokens[0].c_str()) == 1){
                    data[0] = 1;
                }else{
                    data[0] = 0;
                }
               for(unsigned int i = 1; i < tokens.size(); i++){
                   vector<string> feat_val = split(tokens[i],':');
                    if(feat_val.size() == 2){
                        int index = atoi(feat_val[0].c_str());
                        data[index] = atof(feat_val[1].c_str());
                        if (!weights->HasKey(index)) {
                            // init
                            //double val = -1.0+2.0*(double)rd()/rd.max();
                            double val = 1;
                            weights->UpdateF(index, val, false);
                            total_l1[index] = val;
                        }
                    }
               }
            }    
        }

        //unsigned int n = 0;
        //vector<int> index(data.size());
        //iota(index.begin(),index.end(),0);

        if (norm>eps && n<max_train) {
        //if (1) {
            mu += (l1*alpha);
            int label = data[0];
            double predicted = classify(data, weights->get_entire());
            for (auto it=data.begin(); it!=data.end(); it++) {
                if(it->first != 0) {
                    double old_val = weights->GetValue(it->first);
                    double new_val = old_val + alpha * (label - predicted) * it->second;
                    if(l1) {
                        // Cumulative L1-regularization
                        // Tsuruoka, Y., Tsujii, J., and Ananiadou, S., 2009
                        // http://aclweb.org/anthology/P/P09/P09-1054.pdf
                        double z = new_val;
                        if(new_val > 0.0){
                            new_val = max(0.0, (double)(new_val - (mu + total_l1[it->first])));
                        }else if(new_val < 0.0){
                            new_val = min(0.0, (double)(new_val + (mu - total_l1[it->first])));
                        }
                        weights->UpdateF(it->first, new_val, true);
                        total_l1[it->first] += (new_val - z);
                    }    
                }
            }

            n++;
            tot_err += abs(label - predicted);
            if ((label==0 && predicted>0.5) || (label==1 && predicted<0.5)) {
                predict_err = predict_err + 1;
            }
            fprintf(output_convergence, "result: %d predicted: %.02lf, average err: %.03lf, average predict_err: %.03lf\n", label, predicted, tot_err/n, predict_err/n);
            if (n % 10000 == 0) {
                LOG_MSG("progress %lu messages\n", n);
            }

            if (n % 100 ==0) {
                norm = vecnorm(weights->get_entire(), old_weights);
                old_weights = weights->get_entire();
            }
            // double l1n = l1norm(weights);
        }
        else {
            if (n == max_train) {
                LOG_MSG("training terminate average err: %.03lf, average predict_err: %.03lf\n", tot_err/n, predict_err/n);
            }
            n++;
            if (n % 10000 == 0) {
                LOG_MSG("progress %lu messages\n", n);
            }
        }
        
    }

    void ProcessTimeout() {}

    vector<string> split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

double vecnorm(unordered_map<size_t,double>& w1, unordered_map<size_t,double>& w2){

    double sum = 0.0;
    for(auto it = w1.begin(); it != w1.end(); it++){
        double minus = w1[it->first] - w2[it->first];
        double r = minus * minus;
        sum += r;
    }
    return sqrt(sum);
}

double classify(unordered_map<size_t,double>& features, unordered_map<size_t,double>& weights){

    double logit = 0.0;
    for(auto it = features.begin(); it != features.end(); it++){
        if(it->first != 0){
            logit += it->second * weights[it->first];
        }
    }
    return sigmoid(logit);
}


double sigmoid(double x){

    static double overflow = 20.0;
    if (x > overflow) x = overflow;
    if (x < -overflow) x = -overflow;

    return 1.0/(1.0 + exp(-x));
}

};
