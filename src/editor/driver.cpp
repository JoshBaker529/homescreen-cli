
#include "../database_handler/database_handler.hpp"
#include <iostream>
#include <string>
using std::cin;
using std::cout;

int main() {
  DatabaseHandler db;
  std::string action, date, class_id, desc, important;

  // Full disclosure, I will not keep this 100% like this.
  // I will add a way to add bulk items
  // This is just simple code to test the calendar
  while (true) {
    cout << "Add/Modify/Remove: ";
    std::getline(cin, action);

    if (action == "add") {
      cout << "\nDate in YYYY-MM-DD format: ";
      std::getline(cin, date);
      cout << "\nEnter class ID: ";
      std::getline(cin, class_id);
      cout << "\nEnter Description: ";
      std::getline(cin, desc);
      cout << "\nIs this an important item? [TRUE/FALSE]: ";
      cin >> important;

      db.add_event(date, class_id, desc, important);
      // db.debug_print_db();
    }

    if (action == "list") {
      db.debug_print_db();
    }
  }
  return 0;
}
