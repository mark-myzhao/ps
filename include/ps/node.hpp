#ifndef NODE_HPP_
#define NODE_HPP_

#include "mpi.h"

class Node {
  public: 
    int getNodeRank() const { return rank_; }
    static int getCurRank() {
      int rank = -1;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      return rank;
    }
  protected:
    Node(int rank, int size, int root, int count)
      : rank_(rank), size_(size), root_(root), count_(count) {}
    int rank_;   // node rank
    int size_;   // the number of nodes
    int root_;   // server rank
    int count_;  // the number of parameters in the net
};

#endif
