#ifndef PS_WORKER_HPP_
#define PS_WORKER_HPP_

#include "mpi.h"

namespace ps {

class Worker {
  public:
    Worker(int rank) : rank_(rank) {}
    void push();
    void pull();
    void push_async();
    void pull_async();
    void computeGrad();
    void wait();
  private:
    int rank_;
}; 

}
#endif
