#ifndef PSENV_HPP_
#define PSENV_HPP_

#include "ps/server.hpp"
#include "ps/worker.hpp"

namespace ps {

class Worker;
class Server;
class Psenv;

/** 
  * @brief Global configuration for the whole parallel environment.
  */
class Config {
  public:
    Config(int blob_num=0, int neuron_sum=0, int* neuron_num=NULL)
      : blob_num_(blob_num), neuron_sum_(neuron_sum) {
      neuron_num_ = new int[blob_num_];
      for (int i = 0; i < blob_num_; ++i) {
        neuron_num_[i] = neuron_num[i];
      }
    }
    ~Config() {
      if (neuron_num_ != NULL)
        delete [] neuron_num_;
    }
    int blob_num_;
    int neuron_sum_;
    int* neuron_num_; 
};

/**
 * @brief A Parameter Server environment holding configurations for the whole system.
 *        The parallel system contains serveral Workers and exactly one Server,
 *        which can only be produced by this factory class.
 */
class Psenv {
  public:
    Psenv* getEnv(int root=0) {
      if (env_ == NULL) {
        env_ = new Psenv(root);
      }
      return env_;
    }
    ~Psenv() {
      delete worker_;
      delete server_;
    }
    bool isServer() const;
    Worker* getWorker();
    Server* getServer();
    int getCurRank() const {
      return rank_;
    }
  private:
    Psenv(int root=0); 
    Psenv* env_ = NULL;
    int size_;  // number of nodes
    int rank_;  // current rank
    int root_;  // rank of the server
    Worker* worker_ = NULL;
    Server* server_ = NULL;
};


}

#endif
