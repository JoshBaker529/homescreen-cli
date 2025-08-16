#include "events.hpp"
#include <map>
#include <string>
#include <vector>
using std::string;

typedef std::map<string, std::vector<event>> EVENT_MAP;

static int days_in_month[] = {0,  31, 28, 31, 30, 31, 30,
                              31, 31, 30, 31, 30, 31};
