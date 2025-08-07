// This class is a wrapper for the sqlite3pp class
// to easily get the information I need

#ifndef DATABASE_HANDLER_H
#define DATABASE_HANDLER_H

#include "sqlite3pp-master/headeronly_src/sqlite3pp.h"
#include <ctime>

template <class T> class DatabaseHandler {
public:
  DatabaseHandler();
  ~DatabaseHandler();
  T *get_events_month(struct tm *start);

private:
  sqlite3pp::database db;
};

#endif // !DATABASE_HANDLER_H
