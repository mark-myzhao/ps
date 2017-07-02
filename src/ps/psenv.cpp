#include "ps/psenv.hpp"

namespace ps {

Psenv::Psenv(int root) {
  MPI_Comm_size(MPI_COMM_WORLD, &size_);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
  root_ = root > 0 ? root : 0;
}

bool Psenv::isServer() const {
  return root_ == rank_;
}

Worker* Psenv::getWorker() {
  if (isServer()) return NULL;
  if (worker_ != NULL) {
    worker_ = new Worker(getCurRank());
  }
  return worker_;
}

Server* Psenv::getServer() {
  if (!isServer()) return NULL;
  if (server_ == NULL) {
    server_ = new Server(rank_, size_, root_);
  }
  return server_;
}

}
