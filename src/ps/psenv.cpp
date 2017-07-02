#include "ps/psenv.hpp"

namespace ps {

Worker & Psenv::getNewWorker() {
  int curRank = workerList_.size() >= root_ ? workerList_.size() + 1 : workerList_.size();
  Worker newWorker(curRank);
  workerList_.push_back(newWorker);
  return newWorker;
}

Server & Psenv::getNewServer() {
  if (server_ == NULL) {
    server_ = new Server(root_);
  }
  return server_;
}

Worker & Psenv::getWorker(int rank) const {
  return rank < workerList_.size() ? workerList_.get(rank) : NULL;
}

Server & PSenv::getServer() const {
  return server_;
}

}
