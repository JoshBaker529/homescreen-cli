#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include "events.hpp"
#include <map>
#include <string>
#include <vector>
using std::string;

typedef std::map<string, std::vector<event>> EVENT_MAP;

static int days_in_month[] = {0,  31, 28, 31, 30, 31, 30,
                              31, 31, 30, 31, 30, 31};

static std::map<string, string> foreground_color{
    {"default", "39"},      {"black", "30"},      {"red", "31"},
    {"green", "32"},        {"yellow", "33"},     {"blue", "34"},
    {"magenta", "35"},      {"cyan", "36"},       {"light gray", "37"},
    {"dark gray", "90"},    {"light red", "91"},  {"light green", "92"},
    {"light yellow", "93"}, {"light blue", "94"}, {"light magenta", "95"},
    {"light cyan", "96"},   {"white", "97"},      {"bold", "1"}};

static std::map<string, string> background_color{
    {"default", "49"},       {"black", "40"},       {"red", "41"},
    {"green", "42"},         {"yellow", "43"},      {"blue", "44"},
    {"magenta", "45"},       {"cyan", "46"},        {"light gray", "47"},
    {"dark gray", "100"},    {"light red", "101"},  {"light green", "102"},
    {"light yellow", "103"}, {"light blue", "104"}, {"light magenta", "105"},
    {"light cyan", "106"},   {"white", "107"}};

static string color_reset = "\033[39;49;0m";

static string set_bold = "\033[1m";
static string clear_bold = "\033[21m";

#endif // DEFINITIONS_HPP
