#ifndef PS_WORKER_HPP_
#define PS_WORKER_HPP_

#include <cstdio>

#include <boost/thread/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "mpi.h"

#include "ps/message.hpp"
#include "ps/node.hpp"

namespace ps {

class Psenv;

/** @brief Worker nodes of the parallel system. 
 *         It will do these things in order:
 *           1. Pull weights from the server;
 *           2. Compute gradients for current batch;
 *           3. Push gradients to the server (sync or async);
 *           4. Go back to (1.). 
 */
class Worker : public Node {
  public:
    ~Worker() {}
    void pull();
    void push() const;
    void pullAsync();
    void pushAsync();
    void setDiff(double* computedDiff);
    void wait() const { MPI_Barrier(MPI_COMM_WORLD); }
  private:
    Worker(int rank, int size, int root, int count, bool debug)
      : Node(rank, size, root, count, debug) {}

  friend class Psenv;
}; 

}
#endif
