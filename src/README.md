# Structure of source code


## database_handler
This holds the code for the shared class used between the calendar and todo list. Used for reading and writing to the database.

## editor
This holds the code specific to adding, removing, and modifying items in the database. 

## calendar
This holds the code specific to the calendar CLI. Prints the calendar to terminal, populated with the events from the database_handler with a priority to important items.

## todo_list
### Not implemented yet
This holds the code specific to the todo list CLI. Prints events in order to the terminal.

## daily_info
### Not implemented yet
This holds the code specific to the daily informations CLI. Will print your schedule, todo items, and top 'n' important items.


