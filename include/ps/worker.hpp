#ifndef PS_WORKER_HPP_
#define PS_WORKER_HPP_

#include "mpi.h"

#include "ps/message.hpp"

namespace ps {

typedef void (*OPPTR)(void);

class Psenv;

/** @brief Worker nodes of the parallel system. 
 *         It will do these things in order:
 *           1. Pull weights from the server;
 *           2. Compute gradients for current batch;
 *           3. Push gradients to the server (sync or async);
 *           4. Go back to (1.). 
 */
class Worker {
  public:
    ~Worker() {}
    void push(Message<double> & grad);
    void pull(Message<double> & weight);
    void push_async(Message<double> & grad);
    void pull_async(Message<double> & weight);
    void computeGrad(OPPTR op);
    void wait() const { MPI_Barrier(MPI_COMM_WORLD); }
    int getRank() const { return rank_; }
  private:
    Worker(int rank) : rank_(rank) {}
    int rank_;

  friend class Psenv;
}; 

}
#endif
