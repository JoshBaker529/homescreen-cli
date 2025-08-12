
#include "database_handler.hpp"
#include "sqlite3pp-master/headeronly_src/sqlite3pp.h"
#include <ctime>
#include <string>

// These are -1 to account for the first day.
// ie Sunday is 1, Saturday is 6 days ahead of that
#define DAYS_MONTH 27
#define DAYS_WEEK 6

DatabaseHandler::DatabaseHandler() {
  db = sqlite3pp::database("Something.db");
  db.execute("CREATE TABLE IF NOT EXISTS events ("
             // Generic data used for everything
             "date DATE,"
             "class TEXT,"
             "desc TEXT,"
             // Meta data used for daily info
             "schedule BOOL"
             ")");
}

DatabaseHandler::~DatabaseHandler() { db.disconnect(); }

sqlite3pp::query DatabaseHandler::get_events_month(struct tm *date) {
  struct tm start = get_start_of_week(date);
  struct tm end = add_days_to_date(&start, DAYS_MONTH);

  std::string request = "SELECT * FROM events WHERE date BETWEEN '" +
                        date_to_string(&start) + "' AND '" +
                        date_to_string(&end) + "'";

  return sqlite3pp::query(db, request.c_str());
}

std::string DatabaseHandler::date_to_string(struct tm *date) {
  return std::to_string(date->tm_year) + "-" + std::to_string(date->tm_mon) +
         "-" + std::to_string(date->tm_mday);
}

struct tm DatabaseHandler::get_start_of_week(struct tm *date) {
  struct tm updated = *date;
  updated.tm_mday -= updated.tm_wday;
  return updated;
}

struct tm DatabaseHandler::add_days_to_date(struct tm *date, int days) {
  struct tm temp = *date;
  temp.tm_mday = (temp.tm_mday + days);
  if (temp.tm_mday > days_in_month[temp.tm_mon]) {
    temp.tm_mday -= days_in_month[temp.tm_mon];
    temp.tm_mon++;
  }

  return temp;
}
