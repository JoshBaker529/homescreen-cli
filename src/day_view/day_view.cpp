#include "day_view.hpp"
#include "../util/border_characters.hpp" // Maybe ?
#include "../util/definitions.hpp"
#include "../util/functions.hpp"
// #include "../util/sched.hpp"
#include <algorithm>
#include <iostream>

#include <sstream>
#include <string>
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
  int left_rows = w.ws_row - 3; // total number of rows left

  string rhs = make_right_side(right_col, right_rows);
  string lhs = make_left_side(w.ws_col - right_col, left_rows);

  // Printing start

  // Top bar
  std::cout << Characters.TOP_LEFT;
  for (int i = 0; i < w.ws_col - 2; i++) {
    std::cout << Characters.HORIZONTAL;
  }

  // Date
  std::cout << Characters.TOP_RIGHT << "\n" << Characters.VERTICAL << " ";
  string d = get_day_string(date) + ", " + get_month_string(date) + " " +
             std::to_string(date->tm_mday) + get_date_ending(date) + " " +
             std::to_string(date->tm_year);
  int j;
  // minus 2 for border
  for (j = 0; j < w.ws_col - 2; j++) {
    if (j > d.length())
      std::cout << " ";
    else
      std::cout << d[j];
  }
  std::cout << Characters.VERTICAL << "\n" << Characters.LEFT_HORIZONTAL;
  // border below date
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

  // Body
  int posR = 0;
  int posL = 0;
  int current_row = 0;
  // Printing common rows
  for (; current_row < right_rows && current_row < left_rows; current_row++) {
    std::cout << Characters.VERTICAL;
    // Left Side

    for (; posL < lhs.length() && lhs[posL] != '\n'; posL++) {
      std::cout << lhs[posL];
    }
    posL++; // skip '\n';

    // Right side
    for (; posR < rhs.length() && rhs[posR] != '\n'; posR++)
      std::cout << rhs[posR];
    std::cout << rhs[posR++];
  }

  // Uncommon rows
  if (left_rows > right_rows) {
    for (; current_row < left_rows; current_row++) {
      std::cout << Characters.VERTICAL;
      for (; posL < lhs.length() && lhs[posL] != '\n'; posL++) {
        std::cout << lhs[posL];
      }
      posL++; // skip '\n'
      // minus one for right border
      for (int i = 0; i < right_col - 1; i++)
        std::cout << " ";
      std::cout << Characters.VERTICAL << '\n';
    }

  } else {
    for (; current_row < right_rows; current_row++) {
      std::cout << Characters.VERTICAL;
      for (int i = 0; i < w.ws_col - right_col - 1; i++) {
        std::cout << " ";
      }

      for (; posR < rhs.length() && rhs[posR] != '\n'; posR++)
        std::cout << rhs[posR];
      std::cout << rhs[posR++];
    }
  }

  // Below printing
  // +3 for the date bar
  for (int i = current_row + 3; i < w.ws_row - 1; i++) {
    std::cout << Characters.VERTICAL;
    // -2 for border
    for (int j = 0; j < w.ws_col - 2; j++) {
      std::cout << " ";
    }

    std::cout << Characters.VERTICAL << '\n';
  }

  std::cout << Characters.BOTTOM_LEFT;

  // -2 for border
  for (int j = 0; j < w.ws_col - 2; j++) {
    std::cout << Characters.HORIZONTAL;
  }
  std::cout << Characters.BOTTOM_RIGHT;

  std::cout << std::flush;
}

string DayView::make_right_side(int &size, int &rows) {
  std::stringstream ss;

  std::vector<sched> vec = database.get_day_sched(date->tm_wday);
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

string DayView::make_left_side(int col, int &rows) {
  int working_length =
      col - 1 - 3; // minus left border and space for bullet point
  std::stringstream ss;
  // Little silly, but using the same date to grab just today's events,
  // and then grabbing the single vector out of the map
  std::vector<event> events =
      database.get_events_between(date, date)[get_database_date(date)];

  int rows_used = 0;

  std::string line = " Due Today:";
  ss << line;
  for (int i = 0; i < working_length - line.length() + 3; i++)
    ss << " ";
  ss << '\n';

  for (auto i = events.begin(); i != events.end() && rows_used < rows; i++) {
    std::string line = i->class_id + ": " + i->description;
    // Truncate line to correct length;
    if (line.length() > working_length)
      line = line.substr(0, working_length);
    ss << "  " << database.get_color(i->class_id) << line << color_reset;
    // Adding the space between text and rhs border
    for (int j = 0; j < working_length - line.length(); j++) {
      ss << " ";
    }
    ss << '\n';

    rows_used++;
  }

  rows = rows_used;
  rows++;
  return ss.str();
}
