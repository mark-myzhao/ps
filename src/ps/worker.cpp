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
  if (rank_ == Node::getCurRank()) {
    if (debug_) printf("+[Worker %d](async): pull weight from the server\n", Node::getCurRank());
    mtx_.lock();
    boost::function<void (void)> pullFunc(boost::bind(&Worker::pull, this));
    boost::thread t(pullFunc);
    t.join();
    mtx_.unlock();
    if (debug_) printf("-[Worker %d](async): finish pull weight\n", Node::getCurRank());
  }
}

void Worker::pushAsync() {
  if (rank_ == Node::getCurRank()) {
    if (debug_) printf("+[Worker %d](async): push gradients to the server\n", Node::getCurRank());
    mtx_.lock();
    boost::function<void (void)> pushFunc(boost::bind(&Worker::push, this));
    boost::thread t(pushFunc);
    t.join();
    mtx_.unlock();
    if (debug_) printf("-[Worker %d](async): finish push gradients\n", Node::getCurRank());
  }
}

void Worker::setDiff(double* computedDiff) {
  for (int i = 0; i < count_; ++i) {
    diff_[i] = computedDiff[i];
  }
}


}
