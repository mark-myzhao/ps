#ifndef PSENV_HPP_
#define PSENV_HPP_

#include "ps/server.hpp"
#include "ps/worker.hpp"

namespace ps {

class Worker;
class Server;

/**
 * @brief A Parameter Server environment holding configurations for the whole system.
 *        The parallel system contains serveral Workers and exactly one Server,
 *        which can only be produced by this factory class.
 */
class Psenv {
  public:
    Psenv(int root=0); 
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
    int size_;  // number of nodes
    int rank_;  // current rank
    int root_;  // rank of the server
    Worker* worker_ = NULL;
    Server* server_ = NULL;
};

}

#endif
