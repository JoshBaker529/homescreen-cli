
#include "database_handler.hpp"
#include "../util/functions.hpp"
#include "sqlite3pp-master/headeronly_src/sqlite3pp.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include <iostream>
#include <vector>

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

EVENT_MAP DatabaseHandler::get_events_month(struct tm *date) {
  struct tm start = get_start_of_week(date);
  struct tm end = add_days_to_date(&start, DAYS_MONTH);

  std::string request = "SELECT * FROM events WHERE date BETWEEN '" +
                        date_to_string(&start) + "' AND '" +
                        date_to_string(&end) + "'";

  sqlite3pp::query qry(db, request.c_str());

  std::vector<event> vec;

  for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
    struct event item((*i).get<string>(0), (*i).get<string>(1),
                      (*i).get<string>(2), string_to_bool((*i).get<string>(3)),
                      string_to_bool((*i).get<string>(4)),
                      string_to_bool((*i).get<string>(5)));
    vec.push_back(item);
    /*
    for (int j = 0; j < qry.column_count(); ++j) {
      std::cout << (*i).get<char const *>(j) << "\t";
    }

    std::cout << std::endl;
    */
  }

  EVENT_MAP map = organize_events(vec);

  return map;
}

std::string DatabaseHandler::date_to_string(struct tm *date) {

  std::stringstream ss;
  ss << std::setfill('0');
  ss << date->tm_year << "-" << std::setw(2) << date->tm_mon << std::setw(1)
     << "-" << std::setw(2) << date->tm_mday;

  return ss.str();
  /*
    return std::to_string(date->tm_year) + "-" + std::to_string(date->tm_mon) +
           "-" + std::to_string(date->tm_mday);
           */
}

struct tm DatabaseHandler::get_start_of_week(struct tm *date) {
  struct tm updated = *date;
  updated.tm_mday -= updated.tm_wday;
  return updated;
}

EVENT_MAP DatabaseHandler::organize_events(std::vector<event> vec) {
  EVENT_MAP map;

  for (auto i = vec.begin(); i != vec.end(); i++) {
    map[i->date].push_back(*i);
    // std::cout << "Added " << (*i) << " to date " << i->date << "\n";
  }

  return map;
}

bool DatabaseHandler::string_to_bool(string s) {
  if (s == "TRUE")
    return true;
  return false;
}

string DatabaseHandler::bool_to_string(bool b) {
  if (b)
    return "TRUE";
  return "FALSE";
}
