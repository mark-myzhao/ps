#ifndef PS_SERVER_HPP_
#define PS_SERVER_HPP_

#include <cstring>
#include <iostream>

#include "mpi.h"

#include "ps/message.hpp"
#include "ps/node.hpp"

namespace ps {

class Psenv;

/** @brief The server of parallel system.
 *         Receive gradients from workers and return updated weights to them.
 */
class Server : public Node {
  public:
    ~Server() {
      delete [] data_;
      delete [] diff_;
    }
    // recv gradients from workers, sync version
    void recvDiff();
    void sendWeight();
    // Computer weight with diff, data and learning rate
    void computeWeight(double lr=0.01);
  private:
    Server(int rank, int size, int root, int count) 
      : Node(rank, size, root, count) {}

  friend class Psenv;
};

}

#endif
