#include "ps/worker.hpp"

namespace ps {

void Worker::pull() {
  if (rank_ == Node::getCurRank()) { 
    MPI_Recv(data_, count_, MPI_DOUBLE, root_, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
}

void Worker::push() const {
  if (rank_ == Node::getCurRank()) {
    MPI_Send(diff_, count_, MPI_DOUBLE, root_, 1, MPI_COMM_WORLD);
  }
}

void Worker::pullAsync() {
  mtx_.lock();
  mtx_.unlock();
}

void Worker::pushAsync() {
  mtx_.lock();
  mtx_.unlock();
}

void Worker::setDiff(double* computedDiff) {
  for (int i = 0; i < count_; ++i) {
    diff_[i] = computedDiff[i];
  }
}


}
