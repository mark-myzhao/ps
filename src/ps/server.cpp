#include "ps/server.hpp"
#include "ps/message.hpp"

namespace ps {

void Server::recvDiff() {
  for (int i = 0; i < count_; ++i) {
    diff_[i] = 0;
  }
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
    diff_[i] /= (double) (size_ - 1);
  }
  delete [] tmpBuf;
}

void Server::sendWeight() const {
  for (int i = 0; i < size_; ++i) {
    if (i != root_) {
      MPI_Send(data_, count_, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    }
  }
}

void Server::recvDiffFromAWorker(int rank) {
  mtx_.lock();
  MPI_Recv(diff_, count_, MPI_DOUBLE, rank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  mtx_.unlock();
}

void Server::sendWeightToAWorker(int rank) {
  mtx_.lock();
  MPI_Send(data_, count_, MPI_DOUBLE, rank, 0, MPI_COMM_WORLD);
  mtx_.unlock();
}

void Server::asyncOp(int op) {
  std::vector<boost::thread*> threadList;
  for (int i = 0; i < size_; ++i) {
    if (i == root_) {
      continue;
    }
    // use thread to collect diff
    if (op == Server::OP_RECV) {
      if (debug_) printf("+[Server %d](async): receive gradients from worker %d\n", Node::getCurRank(), i);
      boost::function<void (int)> recvAsync(boost::bind(&Server::recvDiffFromAWorker, this, _1));
      boost::thread* t = new boost::thread(recvAsync, i);
      threadList.push_back(t);
      if (debug_) printf("-[Server %d](async): finish receiving gradients from worker %d\n", Node::getCurRank(), i);
    } else if (op == Server::OP_SEND) {
      if (debug_) printf("+[Server %d](async): send weight to worker %d\n", Node::getCurRank(), i);
      boost::function<void (int)> sendAsync(boost::bind(&Server::sendWeightToAWorker, this, _1));
      boost::thread* t = new boost::thread(sendAsync, i);
      threadList.push_back(t);
      if (debug_) printf("-[Server %d](async): finish sending weight to worker %d\n", Node::getCurRank(), i);
    }
  }
  for (int i = 0; i < threadList.size(); ++i) {
    threadList[i]->join();
    delete threadList[i];
  }
}

void Server::computeWeight(double lr) {
  // update data_ with diff_
  for (int i = 0; i < count_; ++i) {
    data_[i] = data_[i] - lr * diff_[i];
  }
}

}
