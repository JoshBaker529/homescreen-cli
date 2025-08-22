# Constants
CC := c++
FLAGS := -lsqlite3 -Wno-deprecated

# Output files
CALENDAR := build/calendar
EDITOR := build/editor
TODO := build/todo_list
DAY := build/day_view

# File variables
UTIL_FILES := src/util/border_characters.hpp src/util/definitions.hpp src/util/events.hpp src/util/functions.cpp
DATABASE_HANDLER_FILES := src/database_handler/database_handler.hpp src/database_handler/database_handler.cpp 
CALENDAR_FILES := src/calendar/driver.cpp src/calendar/calendar.hpp src/calendar/calendar.cpp $(DATABASE_HANDLER_FILES)
EDITOR_FILES := src/editor/driver.cpp $(DATABASE_HANDLER_FILES)
TODO_FILES := src/todo_list/driver.cpp  src/todo_list/todo_list.hpp src/todo_list/todo_list.cpp $(DATABASE_HANDLER_FILES)
DAY_FILES := src/day_view/driver.cpp src/day_view/day_view.hpp src/day_view/day_view.cpp $(DATABASE_HANDLER_FILES)

all: $(CALENDAR) $(EDITOR) $(TODO) $(DAY)

# I can't be bothered to separate building and linking so I can change the output name

$(CALENDAR): $(CALENDAR_FILES)
	$(CC) $(FLAGS) $(CALENDAR_FILES)
	mv a.out $(CALENDAR) 

$(EDITOR): $(EDITOR_FILES)
	$(CC) $(FLAGS) $(EDITOR_FILES)
	mv a.out $(EDITOR)

$(TODO): $(TODO_FILES)
	$(CC) $(FLAGS) $(TODO_FILES)
	mv a.out $(TODO)

$(DAY): $(DAY_FILES)
	$(CC) $(FLAGS) $(DAY_FILES)
	mv a.out $(DAY)

.PHONEY: all

