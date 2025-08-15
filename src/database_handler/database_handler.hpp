// This class is a wrapper for the sqlite3pp class
// to easily get the information I need

#ifndef DATABASE_HANDLER_H
#define DATABASE_HANDLER_H

#include <ctime>
#include <sqlite3pp-master/headeronly_src/sqlite3pp.h>
#include <string>
using std::string;

static int days_in_month[] = {0,  31, 28, 31, 30, 31, 30,
                              31, 31, 30, 31, 30, 31};

class DatabaseHandler {
public:
  DatabaseHandler();
  ~DatabaseHandler();
  void debug_print_db();
  void add_event(string date, string class_id, string desc, string sched,
                 string important);
  std::string get_events_month_string(struct tm *start);

private:
  sqlite3pp::database db;
  std::string date_to_string(struct tm *date);
  struct tm get_start_of_week(struct tm *date);
  struct tm add_days_to_date(struct tm *date, int days);
};

// TODO:
// Implement a way to add to the database
//  -> Send update
//  -> -> Also, need to add some defines for each port
#endif // !DATABASE_HANDLER_H
