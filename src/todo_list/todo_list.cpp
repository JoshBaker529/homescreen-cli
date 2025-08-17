
#include "todo_list.hpp"
#include "../util/functions.hpp"
#include <iostream>
#include <ostream>
#include <sstream>
#include <sys/ioctl.h> // Required for ioctl and winsize
#include <unistd.h>    // Required for STDOUT_FILENO

TodoList::TodoList() { date = update_date(); }

TodoList::~TodoList() { delete date; }

void TodoList::print_list() {

  system("clear");
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
    std::cerr << "Error getting terminal size." << std::endl;
    exit(1);
  }

  std::stringstream ss(db.get_latest_date());
  int year, month, day;
  char dummy;
  ss >> year >> dummy >> month >> dummy >> day;

  date = update_date();
  struct tm start = *date;
  struct tm end = start;

  end.tm_year = year;
  end.tm_mon = month;
  end.tm_mday = day;

  // +1 to include in condition start != end
  end = add_days_to_date(&end, 1);
  events = db.get_events_between(&start, &end);

  for (; !dates_equal(&start, &end); start = add_days_to_date(&start, 1)) {
    string db_date = get_database_date(&start);
    if (events.count(db_date) > 0) {
      std::cout << db_date << "\n";
      for (auto i = events[db_date].begin(); i != events[db_date].end(); i++) {
        string print = "  ";
        print.append(i->class_id);
        print.append(": ");
        print.append(i->description);
        while (print.length() < w.ws_col) {
          print.append(" ");
        }
        for (int i = 0; i < w.ws_col; i++) {
          std::cout << print[i];
        }
        std::cout << "\n";
      }
    }
  }
  std::cout << std::flush;
}
