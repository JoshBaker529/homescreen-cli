#include "calendar.hpp"

#include <ctime>
#include <iomanip>
#include <iostream>

#include <cstdlib>

#include <sys/ioctl.h> // Required for ioctl and winsize
#include <unistd.h>    // Required for STDOUT_FILENO

Calendar::Calendar() { update_date(); }

Calendar::~Calendar() { delete date; }

void Calendar::print_month() {

  // TODO:
  // Still need to implement the todo items, important dates, etc.
  system("clear");
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
    std::cerr << "Error getting terminal size." << std::endl;
    exit(1);
  }

  update_date();
  int start_date = date->tm_mday - date->tm_wday;
  string month = get_month_string();

  // Working size, for easy division
  short working_width = (w.ws_col - 1) - ((w.ws_col - 1) % 7);
  short working_height = (w.ws_row - 3) - ((w.ws_row - 3) % 4);

  // each day box size
  short day_width = working_width / 7;
  short day_heigh = working_height / 4;

  std::cout << date->tm_year << "\n" << month << "\n";

  for (int wid = 0; wid < working_width + 1; wid++) {
    std::cout << "#";
  }

  std::cout << std::setfill('0');
  for (int week = 0; week < 4; week++) {
    std::cout << "\n";
    std::cout << "#";

    for (int day = 0; day < 7; day++) {
      if (start_date == date->tm_mday) {
        std::cout << "\033[35m " << std::setw(2) << start_date++ << std::setw(1)
                  << "\033[0m";
      } else {
        std::cout << " " << std::setw(2) << start_date++ << std::setw(1);
      }
      for (int i = 0; i < day_width - 4; i++) {
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

void Calendar::update_date() {
  time_t now = time(0);
  date = localtime(&now);
  date->tm_year += 1900;
}

string Calendar::get_month_string() {
  switch (date->tm_mon) {
  case 0:
    return "January";
  case 1:
    return "February";
  case 2:
    return "March";
  case 3:
    return "April";
  case 4:
    return "May";
  case 5:
    return "June";
  case 6:
    return "July";
  case 7:
    return "August";
  case 8:
    return "September";
  case 9:
    return "October";
  case 10:
    return "November";
  case 11:
    return "December";
  default:
    return "Error";
  }
}
