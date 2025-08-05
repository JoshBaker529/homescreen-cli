#ifndef CALENDAR_HPP
#define CALENDAR_HPP
#include <ctime>
#include <string>
using std::string;

class Calendar {
public:
  Calendar();
  ~Calendar();
  void print_month();

private:
  struct tm *date;
  void update_date();
  string get_month_string();
};
#endif // !CALENDAR_H
