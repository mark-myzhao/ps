#include "ps/worker.hpp"

namespace ps {

void Worker::pull() {
  if (rank_ == Node::getCurRank()) { 
    if (debug_) printf("+[WORKER %d]: pull weight from the SERVER\n", Node::getCurRank());
    MPI_Recv(data_, count_, MPI_DOUBLE, root_, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (debug_) printf("-[WORKER %d]: finish pulling weight\n", Node::getCurRank());
  }
}

void Worker::push() const {
  if (rank_ == Node::getCurRank()) {
    if (debug_) printf("+[WORKER %d]: push gradients to the SERVER\n", Node::getCurRank());
    MPI_Send(diff_, count_, MPI_DOUBLE, root_, 1, MPI_COMM_WORLD);
    if (debug_) printf("-[WORKER %d]: finish pushing gradients\n", Node::getCurRank());
  }
}

void Worker::pullAsync() {
  if (rank_ == Node::getCurRank()) {
    if (debug_) printf("+[WORKER %d](async): pull weight from the SERVER\n", Node::getCurRank());
    mtx_.lock();
    boost::function<void (void)> pullFunc(boost::bind(&Worker::pull, this));
    boost::thread t(pullFunc);
    t.join();
    mtx_.unlock();
    if (debug_) printf("-[WORKER %d](async): finish pulling weight\n", Node::getCurRank());
  }
}

void Worker::pushAsync() {
  if (rank_ == Node::getCurRank()) {
    if (debug_) printf("+[WORKER %d](async): push gradients to the SERVER\n", Node::getCurRank());
    mtx_.lock();
    boost::function<void (void)> pushFunc(boost::bind(&Worker::push, this));
    boost::thread t(pushFunc);
    t.join();
    mtx_.unlock();
    if (debug_) printf("-[WORKER %d](async): finish pushing gradients\n", Node::getCurRank());
  }
}

void Worker::setDiff(double* computedDiff) {
  for (int i = 0; i < count_; ++i) {
    diff_[i] = computedDiff[i];
  }
}


}
