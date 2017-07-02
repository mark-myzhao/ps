#include "ps/worker.hpp"

namespace ps {

void Worker::push(Message<double> & grad) {

}

void Worker::pull(Message<double> & weight) {

}

void Worker::push_async(Message<double> & grad) {}

void Worker::pull_async(Message<double> & weight) {}

void Worker::computeGrad(OPPTR op) {
  op();
}

}
