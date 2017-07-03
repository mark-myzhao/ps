#include "ps/server.hpp"
#include "ps/message.hpp"

namespace ps {

void Server::init(int count) {
  count_ = count;
  diff_ = new double[count];
  data_ = new double[count];
  memset(diff_, 0, count);
  memset(data_, 0, count);
}

void Server::recvDiffMsg() {
  memset(diff_, 0, count_);
  double* tmpBuf = new double[3 + count_ * 2];
  for (int i = 0; i < size_; ++i) {
    //  accumulate diff
    if (i != root_) {
      MPI_Recv(tmpBuf, 3 + count_ * 2, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      Message<double> recvMsg;
      recvMsg.deserialize(tmpBuf);
      double* fullArray = recvMsg.getRawArray();
      for (int j = 0; j < count_; ++j) {
        diff_[j] += fullArray[j];
      }
    }
  }
  for (int i = 0; i < count_; ++i) {
    diff_[i] /= (double) size_;
  }
  delete [] tmpBuf;
}

void Server::sendWeightMsg() const {
  Message<double> sendMsg(data_, count_, 0);
  for (int i = 0; i < size_; ++i) {
    if (i != root_) {
      MPI_Send(sendMsg.serialize(), sendMsg.getSerialSize(), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    }
  }
}

void Server::computeWeight() {
  // update weight with diff_
}

}
