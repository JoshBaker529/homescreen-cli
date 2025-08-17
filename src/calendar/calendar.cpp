#include "calendar.hpp"
#include "../util/border_characters.hpp"
#include "../util/functions.hpp"

#include <ctime>
#include <iomanip>
#include <iostream>

#include <cstdlib>

#include <sys/ioctl.h> // Required for ioctl and winsize
#include <unistd.h>    // Required for STDOUT_FILENO

Calendar::Calendar() {
  // date = new tm;
  date = update_date();
  update = true;
}

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

  date = update_date();

  // Update to calendar's own get months member function
  // To prevent reading database every time.

  if (update) {
    events = database.get_events_month(date);
    update = false;
  }
  struct tm current_date = *date;
  current_date.tm_mday = date->tm_mday - date->tm_wday;
  string month = get_month_string();

  // Working size, for easy division
  short working_width = (w.ws_col - 1) - ((w.ws_col - 1) % 7);
  short working_height = (w.ws_row - 3) - ((w.ws_row - 3) % 4);

  // each day box size
  short day_width = working_width / 7;
  short day_heigh = working_height / 4;

  std::cout << date->tm_year << "\n" << month << "\n";

  // Top border
  std::cout << Characters.TOP_LEFT;

  for (int wid = 1; wid < working_width; wid++) {
    if (wid % day_width == 0) {
      std::cout << Characters.TOP_VERT;
    } else {
      std::cout << Characters.HORIZONTAL;
    }
  }

  std::cout << Characters.TOP_RIGHT;

  // Innards
  std::cout << std::setfill('0');
  for (int week = 0; week < 4; week++) {
    // Left side
    std::cout << "\n" << Characters.VERTICAL;

    // First line of the box
    for (int day = 0; day < 7; day++) {
      if (current_date.tm_mday > days_in_month[date->tm_mon]) {
        // days_in_month from database_handler.hpp
        current_date.tm_mday = 1;
        current_date.tm_mon += 1;
        if (current_date.tm_mon > 12)
          current_date.tm_mon = 1;
      }
      if (current_date.tm_mday == date->tm_mday) {
        std::cout << "\033[35m " << std::setw(2) << current_date.tm_mday++
                  << std::setw(1) << "\033[0m";
      } else {
        std::cout << " " << std::setw(2) << current_date.tm_mday++
                  << std::setw(1);
      }
      for (int i = 0; i < day_width - 4; i++) {
        std::cout << " ";
      }
      std::cout << Characters.VERTICAL;
    }
    std::cout << "\n";

    // Rest of the lines of the box
    for (int j = 0; j < day_heigh - 2; j++) {
      current_date = sub_days_to_date(&current_date, 7);
      std::cout << Characters.VERTICAL;
      for (int day = 0; day < 7; day++) {

        string db_date = get_database_date(&current_date);
        // std::cout << db_date;
        string print = "                                             ";
        if (events.count(db_date) > 0) {
          // std::cout << "Somehting";
          if (events[db_date].size() > j) {
            print = events[db_date][j].class_id;
            print.append(" ");
            print.append(events[db_date][j].description);
            while (print.length() < day_width) {
              print.append(" ");
            }
          }
        }
        for (int i = 0; i < day_width - 1; i++) {
          // NOTE: INSERT EVENTS HERE
          std::cout << print[i];
        }
        std::cout << Characters.VERTICAL;
        current_date = add_days_to_date(&current_date, 1);
      }
      std::cout << "\n";
    }

    // If Last week, print bottom border
    if (week == 3) {
      std::cout << Characters.BOTTOM_LEFT;
      for (int wid = 1; wid < working_width; wid++) {
        if (wid % day_width == 0) {
          std::cout << Characters.BOTTOM_VERT;
        } else {
          std::cout << Characters.HORIZONTAL;
        }
      }
      std::cout << Characters.BOTTOM_RIGHT;
    } else {
      // Innard horizontal border
      std::cout << Characters.LEFT_HORIZONTAL;
      for (int wid = 1; wid < working_width; wid++) {
        if (wid % day_width == 0) {
          std::cout << Characters.CROSS;
        } else {
          std::cout << Characters.HORIZONTAL;
        }
      }
      std::cout << Characters.RIGHT_HORIZONTAL;
    }
  }

  // Force print the buffer
  std::cout << std::flush;
}

string Calendar::get_month_string() {
  switch (date->tm_mon) {
  case 1:
    return "January";
  case 2:
    return "February";
  case 3:
    return "March";
  case 4:
    return "April";
  case 5:
    return "May";
  case 6:
    return "June";
  case 7:
    return "July";
  case 8:
    return "August";
  case 9:
    return "September";
  case 10:
    return "October";
  case 11:
    return "November";
  case 12:
    return "December";
  default:
    return "Error";
  }
}
