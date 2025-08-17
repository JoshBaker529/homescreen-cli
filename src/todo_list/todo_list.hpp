
#include "../database_handler/database_handler.hpp"
#include "../util/definitions.hpp"
#include <ctime>

class TodoList {
public:
  TodoList();
  ~TodoList();
  void print_list();

private:
  DatabaseHandler db;
  struct tm *date;
  EVENT_MAP events;
};
