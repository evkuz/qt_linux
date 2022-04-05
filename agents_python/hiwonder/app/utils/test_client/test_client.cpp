#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include "SocketClient.h"

using namespace std;

int main(int argc, char *argv[]) {
  cout << "Press enter to repeat reading."<< endl;
  cout << "Enter any char to and press enter to exit." << endl;
  SocketClient client("/run/lock/my.socket");
  DetectorState state;

  std::string line;
  while (true) {
    int res = client.GetState(&state);
    if (res != 0) {
      std::cerr << "Can't read from socket server! Error code:" << res << endl;
      return -3;
    }
    if (state.isDetected)
      cout << "DETECTED: " << state.objectX << " " << state.objectY << endl;
    else
      cout << "NOT DETECTED" << endl;

    std::getline (std::cin,line);
    if (line.length() > 0)
      break;
  }

  return 0;
}
