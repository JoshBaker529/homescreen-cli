
#include "../database_handler/database_handler.hpp"
#include <chrono>
#include <climits>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
using std::cin;
using std::cout;

#define READ_FILE 1
#define MANUAL_ADD 2
#define EDIT 3
#define DELETE 4
#define HELP 5
#define DEBUG 6

#define TABLE_EVENTS 1
#define TABLE_COLORS 2
#define TABLE_SCHEDULE 3

int main() {
  DatabaseHandler db;
  int action, table, day;
  std::string file_path, date, class_id, desc, important, fore, back, location,
      temp, start, end;
  char dummy; // for getting rid of a char at specific locations
  std::ifstream InFile;

  while (true) {
    system("clear");
    cout << "Welcome to the editor! What would you like to do?\n"
            " 1. Read from file\n"
            " 2. Manual add\n"
            " 3. Edit [not implemented]\n"
            " 4. Delete [not implemented]\n"
            " 5. Help\n"
            "\nSelection: ";
    cin >> action;

    cout << "\nWhich table will this be for?\n"
            " 1. Events\n"
            " 2. Colors\n"
            " 3. Schedule\n"
            "\nSelection: ";
    cin >> table;

    cin.ignore(INT_MAX, '\n');

    switch (action) {
    case READ_FILE: // Action
      cout << "\nElements should be tab separated, with newlines between each "
              "item."
              "\nInput file path: ";
      getline(cin, file_path, '\n');
      InFile.open(file_path);
      if (!InFile) {
        cout << "\nCould not open file.\n";
        continue;
      }
      switch (table) {
      case TABLE_EVENTS:
        while (!InFile.eof()) {
          std::getline(InFile, date, '\t');
          std::getline(InFile, class_id, '\t');
          std::getline(InFile, desc, '\t');
          std::getline(InFile, important, '\n');
          if (!InFile.eof())
            db.add_event(date, class_id, desc, important);
        }
        break;

      case TABLE_COLORS:
        while (!InFile.eof()) {
          std::getline(InFile, class_id, '\t');
          std::getline(InFile, fore, '\t');
          std::getline(InFile, back, '\n');
          if (!InFile.eof())
            db.set_color(class_id, fore, back);
        }
        break;

      case TABLE_SCHEDULE:
        while (!InFile.eof()) {
          InFile >> day;
          InFile.ignore(INT_MAX, '\t');
          std::getline(InFile, class_id, '\t');
          std::getline(InFile, location, '\t');
          std::getline(InFile, start, '\t');
          std::getline(InFile, end, '\n');
          if (!InFile.eof())
            db.add_sched_item(day, class_id, location, start, end);
        }
        break;

      } // switch table
      std::cout << "\nFile reading complete.\n\n";
      InFile.close();
      break;
    case MANUAL_ADD:
      switch (table) {
      case TABLE_EVENTS:
        cout << "\nEnter the date in YYYY-MM-DD format: ";
        std::getline(cin, date, '\n');
        cout << "\nEnter class: ";
        std::getline(cin, class_id, '\n');
        cout << "\nEnter description: ";
        std::getline(cin, desc, '\n');
        cout << "\nIs this important? [TRUE/FALSE]: ";
        std::getline(cin, important, '\n');
        db.add_event(date, class_id, desc, important);
        break;

      case TABLE_COLORS:
        cout << "\nEnter class: ";
        std::getline(cin, class_id, '\n');
        cout << "\nForeground color"
                "\nPossible choices: default, "
                "\033[30mblack\t\033[31mred\t\033[32mgreen\t\033["
                "33myellow\t\033[34mblue\t\033[35mmagenta"
                "\t\033[36mcyan\t\033[37mlight gray\t\033[90mdark "
                "gray\t\033[91mlight red\t\033[92mlight green\t\033[93mlight "
                "yellow"
                "\t\033[94mlight blue\t\033[95mlight magenta\t\033[96mlight "
                "cyan\t\033[97mwhite\n";

        cout << color_reset << "Enter color: ";
        std::getline(cin, fore, '\n');
        cout << "\nBackground color"
                "\nPossible choices: default, "
                "\033[40mblack\t\033[41;30mred\t\033[42;30mgreen\t\033["
                "43;30myellow\t\033[44;30mblue\t\033[45;30mmagenta"
                "\t\033[46;30mcyan\t\033[47;30mlight gray\t\033[100mdark "
                "gray\t\033[101;30mlight red\t\033[102;30mlight "
                "green\t\033[103;30mlight yellow"
                "\t\033[104;30mlight blue\t\033[105;30mlight "
                "magenta\t\033[106;30mlight "
                "cyan\t\033[107;30mwhite\n";
        cout << color_reset << "Enter color: ";
        std::getline(cin, back, '\n');

        db.set_color(class_id, fore, back);

        break;

      case TABLE_SCHEDULE:

        cout << "\nDay"
                "\n 0. Sunday"
                "\n 1. Monday"
                "\n 2. Tuesday"
                "\n 3. Wednesday"
                "\n 4. Thursday"
                "\n 5. Friday"
                "\n 6. Saturday"
                "\nEnter day: ";
        cin >> day;
        cin.ignore(INT_MAX, '\n');
        cout << "\nEnter class: ";
        std::getline(cin, class_id, '\n');
        cout << "\nEnter location: ";
        std::getline(cin, location, '\n');
        cout << "\nEnter start time in HH:MMXM format: ";
        std::getline(cin, start, '\n');
        cout << "\nEnter end time in HH:MMXM format: ";
        std::getline(cin, end, '\n');

        std::cout << "\nDebug: " << day << '\n'
                  << class_id << '\n'
                  << location << '\n'
                  << start << '\n'
                  << end << std::endl;

        db.add_sched_item(day, class_id, location, start, end);
        break;

      } // switch table
      break;
    case EDIT:
      switch (table) {
      case TABLE_EVENTS:
        db.edit_event();
        break;
      case TABLE_COLORS:
        db.edit_color();
        break;
      case TABLE_SCHEDULE:
        db.edit_sched();
        break;
      }
      break;
    case DELETE:
      break;
    case HELP:
      switch (table) {
      case TABLE_EVENTS:
        std::cout << "The events table is used for...";
        break;
      case TABLE_COLORS:
        std::cout << "The colors table is used for...";
        break;
      case TABLE_SCHEDULE:
        std::cout << "The schedule table is used for...";
        break;
      }
      break;
    case DEBUG:
      switch (table) {
      case TABLE_EVENTS:
        db.debug_print_db("events");
        break;
      case TABLE_COLORS:
        db.debug_colors_test();
        break;
      case TABLE_SCHEDULE:
        db.debug_print_db("sched");
        break;
      }
      break;
    } // switch action
      //

    std::cout << "\n\nDone, resetting..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(15));
  }

  // NOTE: Maybe save '?' as an input to list all possible options?

  // Full disclosure, I will not keep this 100% like this.
  // I will add a way to add bulk items
  // This is just simple code to test the calendar
  /*
  while (true) {
    cout << "add/color/list/list_color: ";
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
    if (action == "color") {
      cout << "Enter Class ID: ";
      std::getline(cin, class_id);
      cout << "Enter foreground color: ";
      std::getline(cin, fore);
      cout << "Enter background color: ";
      std::getline(cin, back);

      db.set_color(class_id, fore, back);
    }

    if (action == "list") {
      db.debug_print_db("events");
    }

    if (action == "list_color") {
      db.debug_print_db("colors");
    }
    if (action == "test_color") {
      db.debug_colors_test();
    }

    if (action == "add_sched") {
      cout << "Enter day: ";
      cin >> day;
      cin.ignore(INT_MAX, '\n');
      cout << "Enter Class ID: ";
      std::getline(cin, class_id);
      cout << "Enter location: ";
      std::getline(cin, location);
      cout << "Enter start time: ";
      std::getline(cin, start);
      cout << "Enter end time: ";
      std::getline(cin, end);

      db.add_sched_item(day, class_id, location, start, end);
    }

    if (action == "list_sched") {
      db.debug_print_db("sched");
    }
  } */
  return 0;
}
