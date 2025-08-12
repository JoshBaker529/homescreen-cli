// This class is a wrapper for the sqlite3pp class
// to easily get the information I need

#ifndef DATABASE_HANDLER_H
#define DATABASE_HANDLER_H

#include <ctime>
#include <sqlite3pp-master/headeronly_src/sqlite3ppext.h>

template <class T> class DatabaseHandler {
public:
  DatabaseHandler();
  ~DatabaseHandler();
  T *get_events_month(struct tm *start);

private:
  sqlite3pp::database db;
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
