#ifndef PS_SERVER_HPP_
#define PS_SERVER_HPP_

#include "mpi.h"

#include "ps/message.hpp"
#include "ps/psenv.hpp"

namespace ps {

/** @brief The server of parallel system.
 *         Receive gradients from workers and return updated weights to them.
 */
class Server {
  public:
    // Computer weight with diff, data and learning rate
    void run();
    int getRank() const { return rank_; }
  private:
    Server(int rank) : rank_(rank) {}
    int rank_;
    int count_;
    double* weight_;
    double* diff_;

  friend class Psenv;
};

}

#endif
