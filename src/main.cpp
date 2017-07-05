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

void asyncTest() {

}

void hello() {
  for (int i = 0; i < 10; ++i) {
    cout << i << endl;
  }
}

void hello2() {
  std::cout << boost::this_thread::get_id() << std::endl;
}

int main(int argc, char** argv) {
  // syncTest();
  thread t(hello);
  thread t2(hello2);
  for (int i = 0; i < 100; ++i) {

  }
  t2.join();
  return 0;
}
