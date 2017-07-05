#ifndef NODE_HPP_
#define NODE_HPP_

#include "mpi.h"

class Node {
  public: 
    ~Node() {
      delete [] data_;
      delete [] diff_;
    }
    int getNodeRank() const { return rank_; }
    static int getCurRank() {
      int rank = -1;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      return rank;
    }
    void wait() const { MPI_Barrier(MPI_COMM_WORLD); }
    // debug only
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
    double* getData() { return data_; }
    double* getDiff() { return diff_; }
  protected:
    Node(int rank, int size, int root, int count)
      : rank_(rank), size_(size), root_(root), count_(count) {
      if (count > 0) {
        diff_ = new double[count];
        data_ = new double[count];
        for (int i = 0; i < count; ++i) {
          diff_[i] = 0;
          data_[i] = 0;
        }
      }
    }
    boost::mutex mtx_;  // used to lock thread
    int rank_;          // node rank
    int size_;          // the number of nodes
    int root_;          // server rank
    int count_;         // the number of parameters in the net
    double* data_;      // store the weights
    double* diff_;      // store the gradients
};

#endif
