#ifndef PS_SERVER_HPP_
#define PS_SERVER_HPP_

#include "mpi.h"

namespace ps {

class Server {
  public:
    Server(int rank) : rank_(rank) {}
    // Computer weight with diff, data and learning rate
    void run();
  private:
    int rank_;
    int count_;
    double* weight_;
    double* diff_;
};

}

#endif
