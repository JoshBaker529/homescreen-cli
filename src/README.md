# Structure of source code


## database_handler
### Not implemented yet
This holds the code for the shared class used between the calendar and todo list. Used for reading and writing to the database.

## calendar
This holds the code specific to the calendar CLI. Prints the calendar to terminal, populated with the events from the database_handler

## todo_list
### Not implemented yet
This holds the code specific to the todo list CLI. Prints events in order to the terminal.

# Major things that still need to be done:
  - Implement the database_handler
    - A way to add/modify to the database, whether that is through the todo_list or a separate CLI.
  - Implement the todo list

