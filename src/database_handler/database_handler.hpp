// This class is a wrapper for the sqlite3pp class
// to easily get the information I need

#ifndef DATABASE_HANDLER_H
#define DATABASE_HANDLER_H

#include "../util/definitions.hpp"
#include "../util/sched.hpp"
#include <ctime>
#include <sqlite3pp-master/headeronly_src/sqlite3pp.h>
#include <string>
#include <vector>
using std::string;

class DatabaseHandler {
public:
  DatabaseHandler();
  ~DatabaseHandler();
  void debug_print_db(string table);
  void debug_colors_test();
  void add_event(string date, string class_id, string desc, string important);
  EVENT_MAP get_events_month(struct tm *start);
  EVENT_MAP get_events_between(struct tm *start, struct tm *end);
  string get_latest_date();
  void set_color(string class_id, string foreground, string background);
  string get_color(string class_id);
  std::vector<sched> get_day_sched(int day);
  void add_sched_item(int day, string class_id, string location, string start,
                      string end);
  void edit_event();
  void edit_color();

private:
  sqlite3pp::database db;
  std::string date_to_string(struct tm *date);
  struct tm get_start_of_week(struct tm *date);
  EVENT_MAP organize_events(std::vector<event> list);
  string bool_to_string(bool b);
  bool string_to_bool(string s);
};

// TODO:
// Add color to class_id ?
// Modify colors of baseline calendar.
// Modfy editor to allow for bulk addition
// .
// Maybe add a separate table for schedule? Sun-Sat.
// Separate table for colors?
#endif // !DATABASE_HANDLER_H
