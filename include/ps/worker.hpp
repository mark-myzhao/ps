#ifndef PS_WORKER_HPP_
#define PS_WORKER_HPP_

#include "mpi.h"

#include "ps/message.hpp"
#include "ps/psenv.hpp"

namespace ps {

/** @brief Worker nodes of the parallel system. 
 *         It will do these things in order:
 *           1. Pull weights from the server;
 *           2. Compute gradients for current batch;
 *           3. Push gradients to the server (sync or async);
 *           4. Go back to (1.). 
 */
class Worker {
  public:
    void push();
    void pull();
    void push_async();
    void pull_async();
    void computeGrad();
    void wait();
    int getRank() const { return rank_; }
  private:
    Worker(int rank) : rank_(rank) {}
    int rank_;

  friend class Psenv;
}; 

}
#endif
