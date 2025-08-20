#include "../database_handler/database_handler.hpp"
#include <ctime>

class DayView {
public:
  DayView();
  ~DayView();
  void print_view();

private:
  DatabaseHandler database;
  struct tm *date;
  string make_right_side(int &size, int &rows);
  string make_left_side(int col, int &rows);
};
