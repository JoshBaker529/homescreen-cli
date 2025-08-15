
#include "database_handler.hpp"
#include "sqlite3pp-master/headeronly_src/sqlite3pp.h"
#include <ctime>
#include <string>

#include <iostream>

// These are -1 to account for the first day.
// ie Sunday is 1, Saturday is 6 days ahead of that
#define DAYS_MONTH 27
#define DAYS_WEEK 6

DatabaseHandler::DatabaseHandler() {
  db = sqlite3pp::database("Something.db");
  sqlite3pp::command cmd(db, "CREATE TABLE IF NOT EXISTS events ("
                             // Generic data used for everything
                             "date DATE,"
                             "class TEXT,"
                             "desc TEXT,"
                             "finished BOOL,"
                             // Meta data used for daily info
                             "schedule BOOL,"
                             "important BOOL"
                             ")");
  cmd.execute();
}

DatabaseHandler::~DatabaseHandler() { db.disconnect(); }

void DatabaseHandler::debug_print_db() {
  sqlite3pp::query qry(db, "SELECT * FROM events");

  for (int i = 0; i < qry.column_count(); ++i) {
    std::cout << qry.column_name(i) << "\t";
  }

  std::cout << "\n";

  for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
    for (int j = 0; j < qry.column_count(); ++j) {
      std::cout << (*i).get<char const *>(j) << "\t";
    }
    std::cout << std::endl;
  }
}

void DatabaseHandler::add_event(string date, string class_id, string desc,
                                string sched, string important) {

  sqlite3pp::command cmd(
      db, "INSERT INTO "
          "events (date, class, desc, finished, schedule, important) "
          "VALUES (?, ?, ?, ?, ?, ?)");
  cmd.binder() << date << class_id << desc << "FALSE" << sched << important;
  cmd.execute();
}

std::string DatabaseHandler::get_events_month_string(struct tm *date) {
  struct tm start = get_start_of_week(date);
  struct tm end = add_days_to_date(&start, DAYS_MONTH);

  std::string request = "SELECT * FROM events WHERE date BETWEEN '" +
                        date_to_string(&start) + "' AND '" +
                        date_to_string(&end) + "'";

  return request;
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
