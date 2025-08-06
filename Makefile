# Constants
CC := c++
FLAGS := -lsqlite3 -Wno-deprecated

# Output files
CALENDAR := build/calendar

# File variables
SQLITE3PP_FILES := src/database_handler/sqlite3pp-master/*
CALENDAR_FILES := src/calendar/driver.cpp src/calendar/calendar.hpp src/calendar/calendar.cpp

all: $(CALENDAR) # todo editor

$(CALENDAR): $(CALENDAR_FILES)
	$(CC) $(FLAGS) $(CALENDAR_FILES)
	mv a.out $(CALENDAR) 
	# I can't be bothered to separate building and linking so I can change the output name


.PHONEY: all

