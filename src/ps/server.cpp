#include "ps/server.hpp"
#include "ps/message.hpp"

namespace ps {

void Server::recvDiff() {
  memset(diff_, 0, count_);
  double* tmpBuf = new double[count_];
  for (int i = 0; i < size_; ++i) {
    //  accumulate diff
    if (i != root_) {
      MPI_Recv(tmpBuf, count_, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      for (int j = 0; j < count_; ++j) {
        diff_[j] += tmpBuf[j];
      }
    }
  }
  for (int i = 0; i < count_; ++i) {
    diff_[i] /= (double) size_;
  }
  delete [] tmpBuf;
}

void Server::sendWeight() {
  for (int i = 0; i < size_; ++i) {
    if (i != root_) {
      MPI_Send(data_, count_, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    }
  }
}

void Server::computeWeight(double lr) {
  // update data_ with diff_
  for (int i = 0; i < count_; ++i) {
    data_[i] = data_[i] - lr * diff_[i];
  }
}

}
