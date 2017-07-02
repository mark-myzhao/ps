#ifndef PSENV_HPP_
#define PSENV_HPP_

#include <vector>

#include "ps/worker.hpp"
#include "ps/server.hpp"

namespace ps {

/**
 * @brief A Parameter Server environment holding configurations for the whole system.
 *        The parallel system contains serveral Workers and exactly one Server,
 *        which can only be produced by this factory class.
 */
class Psenv {
  public:
    Psenv(int root=0) { root_ = root > 0 ? root : 0; }
    Worker & getNewWorker();
    Server & getNewServer();
    Worker & getWorker(int rank) const;
    Server & getServer() const;
  private:
    int root_;  // rank of the server
    vector<Worker> workerList_;
    Server* server_ = NULL;
};

}

#endif
