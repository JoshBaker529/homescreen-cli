#include "day_view.hpp"
#include "../util/border_characters.hpp" // Maybe ?
#include "../util/definitions.hpp"
#include "../util/functions.hpp"
// #include "../util/sched.hpp"
#include <algorithm>
#include <iostream>

#include <sstream>
#include <sys/ioctl.h> // Required for ioctl and winsize
#include <unistd.h>    // Required for STDOUT_FILENO
#include <vector>

DayView::DayView() { date = update_date(); }

DayView::~DayView() {}

void DayView::print_view() {

  system("clear");
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
    std::cerr << "Error getting terminal size." << std::endl;
    exit(1);
  }

  int mid_point = (w.ws_col / 2);

  date = update_date();

  std::cout << Characters.TOP_LEFT;
  for (int i = 0; i < w.ws_col - 2; i++) {
    std::cout << Characters.HORIZONTAL;
  }
  std::cout << Characters.TOP_RIGHT << "\n" << Characters.VERTICAL << " ";
  string d = "Monday, August 18th 2025";
  int j;
  for (j = 0; j < w.ws_col - 2; j++) {
    if (j > d.length())
      std::cout << " ";
    else
      std::cout << d[j];
  }

  std::cout << Characters.VERTICAL << "\n" << Characters.LEFT_HORIZONTAL;
  for (int i = 0; i < w.ws_col - 2; i++) {
    std::cout << Characters.HORIZONTAL;
  }
  std::cout << Characters.RIGHT_HORIZONTAL;

  int right_size = w.ws_col - mid_point;

  string rhs = make_right_side(right_size, date->tm_wday); // NOTE: CHECK!!

  std::cout << "\n" << rhs;

  std::cout << std::flush;
}

string DayView::make_right_side(int &size, int day) {
  std::stringstream ss;

  std::vector<sched> vec = database.get_day_sched(day);
  std::sort(vec.begin(), vec.end());

  // Three boxes, minus one for right hand border
  int box_width = (size / 3) - 1;
  // Normalizing size, including left border;
  size = box_width * 3 + 1;

  for (auto i = vec.begin(); i != vec.end(); i++) {
    // std::cout << (*i) << "\n";
    ss << Characters.VERTICAL;
    std::string time = i->start + "-" + i->end;
    if (time.length() > box_width) {
      time = i->start;
    }

    ss << time;
    int buffer = box_width - time.length();
    for (; buffer > 0; buffer--) {
      ss << " ";
    }

    ss << Characters.VERTICAL << i->class_id;
    buffer = box_width - i->class_id.length();
    for (; buffer > 0; buffer--) {
      ss << " ";
    }

    ss << Characters.VERTICAL << i->location;
    buffer = box_width - i->location.length();
    for (; buffer > 0; buffer--) {
      ss << " ";
    }

    ss << Characters.VERTICAL << "\n";
    // Border
    bool bottom = (i + 1) == vec.end();
    if (bottom) {
      ss << Characters.BOTTOM_LEFT;
    } else {
      ss << Characters.LEFT_HORIZONTAL;
    }
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < box_width; j++) {
        ss << Characters.HORIZONTAL;
      }
      if (i == 2)
        ss << Characters.RIGHT_HORIZONTAL << "\n";
      else if (bottom)
        ss << Characters.BOTTOM_VERT;
      else
        ss << Characters.CROSS;
    }
  }

  return ss.str();
}
