
#include "database_handler.hpp"
#include "sqlite3pp-master/headeronly_src/sqlite3pp.h"
#include <ctime>

template <class T> DatabaseHandler<T>::DatabaseHandler() {
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

template <class T> DatabaseHandler<T>::~DatabaseHandler() { db.disconnect(); }

template <class T> T *DatabaseHandler<T>::get_events_month(struct tm *start) {}
