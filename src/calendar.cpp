#include "calendar.hpp"

#include <ctime>
#include <iomanip>
#include <iostream>

#include <cstdlib>

#include <sys/ioctl.h> // Required for ioctl and winsize
#include <unistd.h>    // Required for STDOUT_FILENO

Calendar::Calendar() {}

Calendar::~Calendar() {}

void Calendar::print_month() {

  // TODO:
  // Need to add in ability to print month and year at top.
  //    Add padding
  // Still need to implement the todo items, important dates, etc.
  system("clear");
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
    std::cerr << "Error getting terminal size." << std::endl;
    exit(1);
  }

  short date = 1; // FIX: Update to correct starting date

  // Working size, for easy division
  short working_width = (w.ws_col - 1) - ((w.ws_col - 1) % 7);
  short working_height = (w.ws_row - 3) - ((w.ws_row - 3) % 4);

  // each day box size
  short day_width = working_width / 7;
  short day_heigh = working_height / 4;

  std::cout << "MONTH\nSomething?\n";

  for (int wid = 0; wid < working_width + 1; wid++) {
    std::cout << "#";
  }

  std::cout << std::setfill('0');
  for (int week = 0; week < 4; week++) {
    std::cout << "\n";
    std::cout << "#";

    for (int day = 0; day < 7; day++) {
      std::cout << std::setw(2) << date++ << std::setw(1);
      for (int i = 0; i < day_width - 3; i++) {
        std::cout << " ";
      }
      std::cout << "#";
    }
    std::cout << "\n";
    for (int j = 0; j < day_heigh - 2; j++) {
      std::cout << "#";
      for (int day = 0; day < 7; day++) {
        // std::cout << std::setw(2) << date++ << std::setw(1);
        for (int i = 0; i < day_width - 1; i++) {
          std::cout << " ";
        }
        std::cout << "#";
      }
      std::cout << "\n";
    }
    for (int wid = 0; wid < working_width + 1; wid++) {
      std::cout << "#";
    }
    std::cout << std::flush;
  }
}
