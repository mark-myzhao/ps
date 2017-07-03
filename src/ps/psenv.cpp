#include "ps/psenv.hpp"

namespace ps {

// initialize static variable
Psenv* Psenv::env_ = NULL;

Psenv::Psenv(int root, int count) {
  MPI_Comm_size(MPI_COMM_WORLD, &size_);
  root_ = root > 0 ? root : 0;
  count_ = count;
}

bool Psenv::isServer() const {
  return root_ == Psenv::getCurRank();
}

Worker* Psenv::getWorker() {
  if (isServer()) return NULL;
  if (worker_ == NULL) {
    worker_ = new Worker(getCurRank(), size_, root_, count_);
  }
  return worker_;
}

Server* Psenv::getServer() {
  if (!isServer()) return NULL;
  if (server_ == NULL) {
    server_ = new Server(getCurRank(), size_, root_, count_);
  }
  return server_;
}

}
