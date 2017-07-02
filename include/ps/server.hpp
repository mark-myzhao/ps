#ifndef PS_SERVER_HPP_
#define PS_SERVER_HPP_

#include <cstring>

#include "mpi.h"

#include "ps/message.hpp"

namespace ps {

class Psenv;

/** @brief The server of parallel system.
 *         Receive gradients from workers and return updated weights to them.
 */
class Server {
  public:
    ~Server() {
      delete [] data_;
      delete [] diff_;
    }
    void init(int count) {
      count_ = count;
      diff_ = new double[count];
      data_ = new double[count];
      memset(diff_, 0, count);
      memset(data_, 0, count);
    }
    void recvDiffMsg();
    void sendWeightMsg() const;
    // Computer weight with diff, data and learning rate
    void computeWeight();
    void wait() const { MPI_Barrier(MPI_COMM_WORLD); }
    int getRank() const { return rank_; }
  private:
    Server(int rank, int size, int root=0)
      : rank_(rank), size_(size), root_(root) {}
    int rank_;  // current rank
    int size_;  // the number of nodes
    int root_;  // server rank
    int count_;
    double* data_;
    double* diff_;

  friend class Psenv;
};

}

#endif
