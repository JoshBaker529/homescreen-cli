// This class is a wrapper for the sqlite3pp class
// to easily get the information I need

#ifndef DATABASE_HANDLER_H
#define DATABASE_HANDLER_H

#include <ctime>
#include <iostream>
#include <map>
#include <sqlite3pp-master/headeronly_src/sqlite3pp.h>
#include <string>
#include <vector>
using std::string;

static int days_in_month[] = {0,  31, 28, 31, 30, 31, 30,
                              31, 31, 30, 31, 30, 31};

struct event {
  string date;
  string class_id;
  string description;
  bool finished;
  bool sched;
  bool important;

  bool operator>(const event &rhs) const {
    if (this->date == rhs.date) {
      if (this->important && !rhs.important)
        return true;
      return false;
    }

    return this->date.compare(rhs.date) > 0;
  }

  event(string d, string c, string desc, bool f, bool s, bool i)
      : date(d), class_id(c), description(desc), finished(f), sched(s),
        important(i) {}

  friend std::ostream &operator<<(std::ostream &os, const event &e);
};

inline std::ostream &operator<<(std::ostream &os, const event &e) {
  os << "{ date: " << e.date << ", class: " << e.class_id
     << ", desc: " << e.description << ", finished: " << e.finished
     << ", sched: " << e.sched << ", important: " << e.important << " }";
  return os;
}

struct tm add_days_to_date(struct tm *date, int days);
struct tm sub_days_to_date(struct tm *date, int days);

typedef std::map<string, std::vector<event>> EVENT_MAP;

class DatabaseHandler {
public:
  DatabaseHandler();
  ~DatabaseHandler();
  void debug_print_db();
  void add_event(string date, string class_id, string desc, string sched,
                 string important);
  EVENT_MAP get_events_month(struct tm *start);

private:
  sqlite3pp::database db;
  std::string date_to_string(struct tm *date);
  struct tm get_start_of_week(struct tm *date);
  EVENT_MAP organize_events(std::vector<event> list);
  string bool_to_string(bool b);
  bool string_to_bool(string s);
};

// TODO:
// Implement a way to add to the database
//  -> Send update
//  -> -> Also, need to add some defines for each port
#endif // !DATABASE_HANDLER_H
