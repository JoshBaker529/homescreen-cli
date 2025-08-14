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

static struct chars {
  string VERTICAL = "\u2503";
  string TOP_RIGHT = "\u2513";
  string BOTTOM_LEFT = "\u2517";
  string BOTTOM_VERT = "\u253B";
  string TOP_VERT = "\u2533";
  string LEFT_HORIZONTAL = "\u2523";
  string RIGHT_HORIZONTAL = "\u252B";
  string HORIZONTAL = "\u2501";
  string CROSS = "\u254B";
  string BOTTOM_RIGHT = "\u251B";
  string TOP_LEFT = "\u250F";
} Characters;

#endif // !CALENDAR_HPP
