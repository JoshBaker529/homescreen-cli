// This class is a wrapper for the sqlite3pp class
// to easily get the information I need

#ifndef DATABASE_HANDLER_H
#define DATABASE_HANDLER_H

#include <ctime>
#include <sqlite3pp-master/headeronly_src/sqlite3pp.h>
#include <string>

static int days_in_month[] = {0,  31, 28, 31, 30, 31, 30,
                              31, 31, 30, 31, 30, 31};

class DatabaseHandler {
public:
  DatabaseHandler();
  ~DatabaseHandler();
  sqlite3pp::query get_events_month(struct tm *start);

private:
  sqlite3pp::database db;
  std::string date_to_string(struct tm *date);
  struct tm get_start_of_week(struct tm *date);
  struct tm add_days_to_date(struct tm *date, int days);
};

// TODO:
// Main thought for updating cache:
// Use websockets to communicate updates.
// When a change is made, send a request to every other process
//    to update their cache.
// When recieving a request, re-process the query
// .
// WHICH APPARENTLY C++ DOESN'T HAVE WEBSOCKETS BUILT INTO IT!

#endif // !DATABASE_HANDLER_H
