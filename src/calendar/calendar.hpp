#ifndef CALENDAR_HPP
#define CALENDAR_HPP
#include "../database_handler/database_handler.hpp"
#include <ctime>
#include <string>
using std::string;

class Calendar {
public:
  Calendar();
  ~Calendar();
  void print_month();

private:
  DatabaseHandler database;
  struct tm *date;
  bool show_class = false; // toggles showing the class name in Calendar

  EVENT_MAP events;
};

#endif // !CALENDAR_HPP
