#ifndef PSENV_HPP_
#define PSENV_HPP_

#include "mpi.h"

#include <iostream>

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
    ~Psenv() {
      if (worker_ != NULL) delete worker_;
      if (server_ != NULL) delete server_;
      if (env_ != NULL) delete env_;
    }
    // enable debug info
    void setDebug(bool debug);

    // judge if current node is server
    bool isServer() const;

    // worker and server factory
    Worker* getWorker();
    Server* getServer();

    // static methods
    static Psenv* getEnv(int root=0, int count=0, bool debug=false) {
      if (env_ == NULL) env_ = new Psenv(root, count, debug);
      return env_;
    }

    // initalize and finalize MPI environment
    static void initalize(int* argc, char*** argv) {
      MPI_Init(argc, argv);
    }
    static void finalize() {
      MPI_Finalize();
    }

    // get the rank of current node
    static int getCurRank() {
      int rank = -1;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      return rank;
    }
  private:
    Psenv(int root, int count, bool debug); 
    static Psenv* env_;
    int size_;    // number of nodes
    int count_;   // number of parameters in the net
    int root_;    // rank of the server
    bool debug_;  // determine if debug info is enabled
    Worker* worker_ = NULL;
    Server* server_ = NULL;
};


}

#endif
