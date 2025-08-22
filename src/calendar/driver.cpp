
// #include <ctime>

#include <chrono> // Required for time duration for sleep_for
#include <thread> // Required for sleep_for

// #include <cstdlib>

// #include <sys/ioctl.h> // Required for ioctl and winsize
// #include <unistd.h>    // Required for STDOUT_FILENO
#include "calendar.hpp"

int main() {
  Calendar cal;
  int i = 0;
  // temporarily commented
  while (true) {
    cal.print_month();

    std::this_thread::sleep_for(std::chrono::minutes(1));
  }
  return 0;
}
