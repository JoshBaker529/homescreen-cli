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

  // Prepare printing

  int right_col = w.ws_col - mid_point;
  int right_rows = 0;

  string rhs = make_right_side(right_col, right_rows, date->tm_wday);

  // Printing start

  // Top bar
  std::cout << Characters.TOP_LEFT;
  for (int i = 0; i < w.ws_col - 2; i++) {
    std::cout << Characters.HORIZONTAL;
  }

  // Date
  std::cout << Characters.TOP_RIGHT << "\n" << Characters.VERTICAL << " ";
  string d = "Monday, August 18th 2025";
  int j;
  // minus 2 for border
  for (j = 0; j < w.ws_col - 2; j++) {
    if (j > d.length())
      std::cout << " ";
    else
      std::cout << d[j];
  }
  std::cout << Characters.VERTICAL << "\n" << Characters.LEFT_HORIZONTAL;

  // minus 2 for left border
  for (int i = 0; i < (w.ws_col - right_col) - 1; i++) {
    std::cout << Characters.HORIZONTAL;
  }

  for (int i = 0; i < 3; i++) {
    std::cout << Characters.TOP_VERT;
    // / 3 for number of boxes, -1 for TOP_VERT
    for (int j = 0; j < (right_col / 3) - 1; j++) {
      std::cout << Characters.HORIZONTAL;
    }
  }
  std::cout << Characters.RIGHT_HORIZONTAL << '\n';

  int pos = 0;
  for (int j = 0; j < right_rows; j++) {
    // NOTE: This will be whrere right hand side printing goes!
    for (int i = 0; i < w.ws_col - right_col; i++)
      std::cout << " ";

    for (; pos < rhs.length() && rhs[pos] != '\n'; pos++)
      std::cout << rhs[pos];
    std::cout << rhs[pos++];
  }
  std::cout << std::flush;
}

string DayView::make_right_side(int &size, int &rows, int day) {
  std::stringstream ss;

  std::vector<sched> vec = database.get_day_sched(day);
  std::sort(vec.begin(), vec.end());

  // Three boxes, minus one for right hand border
  int box_width = (size / 3) - 1;
  // Normalizing size, including borders;
  // (box_width + 1) for each right border, final +1 for left border
  size = (box_width + 1) * 3 + 1;

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
    rows++;
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
      if (i == 2) {
        ss << Characters.RIGHT_HORIZONTAL << "\n";
        rows++;
      } else if (bottom)
        ss << Characters.BOTTOM_VERT;
      else
        ss << Characters.CROSS;
    }
  }

  return ss.str();
}
