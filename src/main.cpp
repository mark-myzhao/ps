#include <iostream>
#include <cassert>

#include <boost/thread/thread.hpp>

#include "ps/message.hpp"
#include "ps/psenv.hpp"

using std::cout;
using std::endl;
using boost::thread;
using ps::Psenv;
using ps::Server;
using ps::Worker;

void printArr(double* arr, int count) {
  for (int i = 0; i < count; ++i) {
    cout << arr[i] << " ";
  }
  cout << endl;
}

void syncTest(int* argc, char*** argv) {
  // This is a test program.
  // Initialized data: [0, 0, 0, 0, 0]
  // Manually run some iteration and see the result
  int count = 5;
  double* data = new double[count];
  double* preComputedDiff = new double[count];
  for (int i = 0; i < count; ++i) {
    data[i] = 0.0;
    preComputedDiff[i] = 1.0;
  }
  Psenv::initalize(argc, argv);
  Psenv* env = Psenv::getEnv(0, count);
  Server* server = env->getServer();
  Worker* worker = env->getWorker();
  double* tmpBuf = new double[5];
  for (int i = 0; i < 10; ++i) {
    if (env->isServer()) {
      if (server != NULL) {  
        // compute weight
        server->computeWeight(0.01);
        server->sendWeight();
        server->recvDiff();
        server->show();
      }
    } else {
      if (worker != NULL) {
        worker->pull();
        // compute gradients here
        worker->setDiff(preComputedDiff);
        worker->push();
      }
    }
  }
  delete [] data;
  delete [] preComputedDiff;
  Psenv::finalize();
}

void asyncTest(int* argc, char*** argv) {
  // This is a async test program.
  // Initialized data: [0, 0, 0, 0, 0]
  // Manually run some iteration and see the result
  int count = 5;
  double* data = new double[count];
  double* preComputedDiff = new double[count];
  for (int i = 0; i < count; ++i) {
    data[i] = 0.0;
    preComputedDiff[i] = 1.0;
  }
  Psenv::initalize(argc, argv);
  Psenv* env = Psenv::getEnv(0, count);
  env->setDebug(true);
  Server* server = env->getServer();
  Worker* worker = env->getWorker();
  double* tmpBuf = new double[5];
  for (int i = 0; i < 5; ++i) {
    if (env->isServer()) {
      if (server != NULL) {  
        // compute weight
        server->computeWeight(0.01);
        server->asyncOp(Server::OP_SEND);
        server->asyncOp(Server::OP_RECV);
        // server->show();
      }
    } else {
      if (worker != NULL) {
        worker->pullAsync();
        // compute gradients here
        worker->setDiff(preComputedDiff);
        worker->pushAsync();
      }
    }
  }
  delete [] data;
  delete [] preComputedDiff;
  Psenv::finalize();
}

int main(int argc, char** argv) {
  // syncTest(&argc, &argv);
  asyncTest(&argc, &argv);
  return 0;
}
