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
    void wait() const { MPI_Barrier(MPI_COMM_WORLD); }
    // Debug only
    void show() const {
      std::cout << "Data: ";
      for (int i = 0; i < count_; ++i) {
        std::cout << data_[i] << " ";
      }
      std::cout << std::endl;
      
      std::cout << "Diff: ";
      for (int i = 0; i < count_; ++i) {
        std::cout << diff_[i] << " ";
      }
      std::cout << std::endl;
    }
  private:
    Server(int rank, int size, int root, int count) 
      : Node(rank, size, root, count) {
      if (count > 0) {
        diff_ = new double[count];
        data_ = new double[count];
        memset(diff_, 0, count);
        memset(data_, 0, count);
      }
    }
    // use the server to manage global weights and gradients
    double* data_;  // weights
    double* diff_;  // gradients

  friend class Psenv;
};

}

#endif
