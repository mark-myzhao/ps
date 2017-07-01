#include "ps/message.hpp"

int main() {
  int testDiff[] = { 1, 2, 3, 4 ,5 };
  ps::Message<int> message(testDiff, 5);
  return 0;
}
