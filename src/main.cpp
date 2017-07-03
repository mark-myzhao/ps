#include <iostream>
#include <cassert>

#include "ps/message.hpp"
#include "ps/psenv.hpp"

using std::cout;
using std::endl;
using ps::Psenv;
using ps::Server;
using ps::Worker;

void printArr(double* arr, int count) {
  for (int i = 0; i < count; ++i) {
    cout << arr[i] << " ";
  }
  cout << endl;
}

int main(int argc, char** argv) {
  // MPI_Init(&argc, &argv);
  // This is a test program.
  // Initialized data: [0, 0, 0, 0, 0]
  // Manually run some iteration and see the result
  int count = 5;
  double* data = new double[count];
  double* preComputedDiff = new double[count];
  for (int i = 0; i < count; ++i) {
    data[i] = 0.0;
    preComputedDiff[i] = 0.0;
  }
  Psenv::initalize(&argc, &argv);
  Psenv* env = Psenv::getEnv(0, count);
  Server* server = env->getServer();
  Worker* worker = env->getWorker();
  if (env->isServer()) {
    // compute weight
    server->computeWeight(0.01);
    server->show();
    server->sendWeight();
    server->recvDiff();
  } else {
    cout << "this is worker " << Psenv::getCurRank() << endl;
    worker->pull();
    worker->setDiff(preComputedDiff);
    worker->push();
  }
  delete [] data;
  delete [] preComputedDiff;
  return 0;
}
