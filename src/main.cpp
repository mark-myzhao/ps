#include "ps/message.hpp"

#include <iostream>
#include <cassert>

void testMessage() {
  std::cout << "Test Constructor" << std::endl;
  int testDiff[] = { 1, 2, 3, 4 ,5 };
  ps::Message<int> msg(testDiff, 5, 5);
  msg.show();
  std::cout << msg.getSerialSize() << std::endl;
  int* serialBuff = msg.serialize();
  for (int i = 0; i < msg.getSerialSize(); ++i) {
    std::cout << serialBuff[i] << " ";
  }
  std::cout << std::endl;
  std::cout << "-------------" << std::endl;

  ps::Message<int> msg2;
  msg2.deserialize(serialBuff);
  msg2.show();
  std::cout << "-------------" << std::endl;

  int testDiff2[] = {0, 1, 9, 0, 4, 0, 6, 8};
  ps::Message<int> msg3(testDiff2, 8);
  msg3.show();
  int* serialBuff2 = msg3.serialize();
  for (int i = 0; i < msg3.getSerialSize(); ++i) {
    std::cout << serialBuff2[i] << " ";
  }
  std::cout << std::endl;
  std::cout << "-------------" << std::endl;

  delete [] serialBuff;
  delete [] serialBuff2;
}

int main() {
  testMessage();
  return 0;
}
