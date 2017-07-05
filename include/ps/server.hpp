#ifndef PS_SERVER_HPP_
#define PS_SERVER_HPP_

#include <cstring>
#include <cstdio>
#include <iostream>


#include <boost/thread/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

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
    static const int OP_SEND = 0;
    static const int OP_RECV = 1;
    ~Server() {}
    // recv gradients from workers, sync version
    void recvDiff();
    void sendWeight() const;
    // async version
    void asyncOp(int op);
    void sendWeightToAWorker(int rank);
    void recvDiffFromAWorker(int rank);
    // Computer weight with diff, data and learning rate
    void computeWeight(double lr=0.01);
  private:
    Server(int rank, int size, int root, int count, bool debug=false) 
      : Node(rank, size, root, count, debug) {}

  friend class Psenv;
};

}

#endif
