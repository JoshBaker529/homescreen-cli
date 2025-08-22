
#include "database_handler.hpp"
#include "../util/functions.hpp"
// #include "../util/sched.hpp"
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
  db = sqlite3pp::database("Homescreen.db");
  sqlite3pp::command cmd(db, "CREATE TABLE IF NOT EXISTS events ("
                             // Generic data used for everything
                             "date DATE,"
                             "class TEXT,"
                             "desc TEXT,"
                             "important BOOL"
                             ");"
                             // Colors table
                             "CREATE TABLE IF NOT EXISTS colors ("
                             "tag TEXT PRIMARY KEY,"
                             "foreground TEXT,"
                             "background TEXT"
                             ");"
                             // Weekly Schedule table
                             "CREATE TABLE IF NOT EXISTS sched ("
                             "day INTEGER,"
                             "class TEXT,"
                             "location TEXT,"
                             "start TEXT,"
                             "end TEXT"
                             ")"

  );
  cmd.execute_all();
}

DatabaseHandler::~DatabaseHandler() { db.disconnect(); }

void DatabaseHandler::debug_print_db(string table) {
  string request = "SELECT * FROM " + table;
  sqlite3pp::query qry(db, request.c_str());

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

void DatabaseHandler::debug_colors_test() {
  string request = "SELECT * FROM colors";
  sqlite3pp::query qry(db, request.c_str());

  for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
    string cl = (*i).get<string>(0);
    string color = get_color(cl);
    std::cout << color << cl << color_reset;
    std::cout << std::endl;
  }
}

void DatabaseHandler::add_event(string date, string class_id, string desc,
                                string important) {

  sqlite3pp::command cmd(db, "INSERT INTO "
                             "events (date, class, desc, important) "
                             "VALUES (?, ?, ?, ?)");
  cmd.binder() << date << class_id << desc << important;
  cmd.execute();
}

EVENT_MAP DatabaseHandler::get_events_month(struct tm *date) {
  struct tm start = get_start_of_week(date);
  struct tm end = add_days_to_date(&start, DAYS_MONTH);

  return get_events_between(&start, &end);
}

EVENT_MAP DatabaseHandler::get_events_between(struct tm *start,
                                              struct tm *end) {
  std::string request = "SELECT * FROM events WHERE date BETWEEN '" +
                        date_to_string(start) + "' AND '" +
                        date_to_string(end) + "'";

  sqlite3pp::query qry(db, request.c_str());

  std::vector<event> vec;

  for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
    struct event item((*i).get<string>(0), (*i).get<string>(1),
                      (*i).get<string>(2), string_to_bool((*i).get<string>(3)));
    vec.push_back(item);
  }

  EVENT_MAP map = organize_events(vec);

  return map;
}

string DatabaseHandler::get_latest_date() {
  sqlite3pp::query qry(db, "SELECT MAX(date) FROM events");
  sqlite3pp::query::iterator i = qry.begin();
  return (*i).get<string>(0);
}

void DatabaseHandler::set_color(string class_id, string foreground,
                                string background) {
  sqlite3pp::command cmd(db, "INSERT OR REPLACE INTO colors (tag, foreground, "
                             "background) VALUES (?, ?, ?)");
  cmd.binder() << class_id << foreground << background;
  cmd.execute();
}

string DatabaseHandler::get_color(string class_id) {
  string request = "SELECT * FROM colors WHERE tag = '" + class_id + "'";
  sqlite3pp::query qry(db, request.c_str());

  // Only should have a single entry
  sqlite3pp::query::iterator i = qry.begin();
  string fore = (*i).get<string>(1);
  string back = (*i).get<string>(2);
  string modifier = "";

  string ret = "\033[" + foreground_color[fore] + ";" + background_color[back] +
               modifier + "m";
  return ret;
}

std::vector<sched> DatabaseHandler::get_day_sched(int day) {
  std::string request = "SELECT * FROM sched WHERE day = ";
  request.append(std::to_string(day));
  sqlite3pp::query qry(db, request.c_str());
  std::vector<sched> vec;

  for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
    struct sched item((*i).get<int>(0), (*i).get<string>(1),
                      (*i).get<string>(2), (*i).get<string>(3),
                      (*i).get<string>(4));
    vec.push_back(item);
  }

  return vec;
}

void DatabaseHandler::add_sched_item(int day, string class_id, string location,
                                     string start, string end) {

  sqlite3pp::command cmd(db, "INSERT INTO sched (day, class, location, start, "
                             "end) VALUES (?, ?, ?, ?, ?)");
  cmd.binder() << day << class_id << location << start << end;
  cmd.execute();
}

void DatabaseHandler::edit_event() {
  std::cout
      << "Starting search for event\nEnter date of item you wish to modify: ";
  string date, request;
  std::getline(std::cin, date, '\n');
  request = "SELECT * FROM events WHERE date = '" + date + "'";

  sqlite3pp::query qry(db, request.c_str());

  std::cout << "Found:\n";
  int pos = 1, choice;
  std::vector<event> evec;
  for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
    evec.push_back(event((*i).get<string>(0), (*i).get<string>(1),
                         (*i).get<string>(2),
                         ((*i).get<string>(3) == "TRUE") ? true : false));

    std::cout << pos++ << ". " << (*i).get<string>(0) << '\t'
              << (*i).get<string>(1) << '\t' << (*i).get<string>(2) << '\t'
              << (*i).get<string>(3) << '\n';
  }
  std::cout << "Item to edit [0 to go back]: ";
  std::cin >> choice;
  if (choice == 0)
    return;
  event e = evec[--choice];

  std::cout << "\nFields\n"
               "0. Done\n"
               "1. Date\n"
               "2. Class\n"
               "3. Description\n"
               "4. Important\n"
               "Enter field you wish to change: ";
  std::cin >> choice;

  std::string column, old;
  switch (choice) {
  case 0:
    return;
    break;
  case 1:
    column = "date";
    old = e.date;
    break;
  case 2:
    column = "class";
    old = e.class_id;
    break;
  case 3:
    column = "desc";
    old = e.description;
    break;
  case 4:
    column = "important";
    if (e.important)
      old = "TRUE";
    else
      old = "FALSE";
    break;
  }

  std::cout << "New value: ";
  std::cin.ignore(1);
  string updated;
  std::getline(std::cin, updated, '\n');

  request = "UPDATE events "
            "SET " +
            column + " = REPLACE(" + column + ", '" + old + "', '" + updated +
            "') "
            "WHERE "
            "date = '" +
            e.date +
            "' AND "
            "class = '" +
            e.class_id +
            "' AND "
            "desc = '" +
            e.description +
            "' AND "
            "important = '" +
            (e.important ? "TRUE" : "FALSE") + "'";

  sqlite3pp::command cmd(db, request.c_str());

  cmd.execute();
}

void DatabaseHandler::edit_color() {
  std::cout << "Starting search for class\nEnter class you wish to modify: ";
  string class_id, request;
  std::getline(std::cin, class_id, '\n');
  // std::cin.ignore(1);
  //  request = "SELECT * FROM colors WHERE class = '" + class_id + "'";

  // sqlite3pp::query qry(db, request.c_str());

  std::cout << "Foreground color: ";
  string fore, back;
  std::getline(std::cin, fore, '\n');
  // std::cin.ignore(1);
  std::cout << "Background color: ";
  std::getline(std::cin, back, '\n');
  // std::cin.ignore(1);
  set_color(class_id, fore, back);
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
