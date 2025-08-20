#ifndef FUNCTIONS_CPP
#define FUNCTIONS_CPP

#include "definitions.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>

// struct tm add_days_to_date(struct tm *date, int days);
// struct tm sub_days_to_date(struct tm *date, int days);

inline struct tm add_days_to_date(struct tm *date, int days) {
  struct tm temp = *date;
  temp.tm_mday = (temp.tm_mday + days);
  if (temp.tm_mday > days_in_month[temp.tm_mon]) {
    temp.tm_mday -= days_in_month[temp.tm_mon];
    temp.tm_mon++;
  }

  // std::cout << "Updated date to " << temp.tm_year << "-" << temp.tm_mon <<
  // "-"
  //         << temp.tm_mday << "\n";
  return temp;
}

inline struct tm sub_days_to_date(struct tm *date, int days) {
  struct tm temp = *date;
  temp.tm_mday = (temp.tm_mday - days);
  if (temp.tm_mday < 1) {
    temp.tm_mon--;
    temp.tm_mday += days_in_month[temp.tm_mon];
  }

  return temp;
}

inline struct tm *update_date() {
  time_t now = time(0);
  struct tm *date = localtime(&now);
  date->tm_year += 1900;
  date->tm_mon++;
  return date;
}

inline bool dates_equal(struct tm *left, struct tm *right) {
  if (left->tm_year != right->tm_year)
    return false;

  if (left->tm_mon != right->tm_mon)
    return false;

  if (left->tm_mday != right->tm_mday)
    return false;
  return true;
}

inline string get_database_date(struct tm *d) {
  std::stringstream ss;
  ss << std::setfill('0');
  ss << d->tm_year << "-" << std::setw(2) << d->tm_mon << std::setw(1) << "-"
     << std::setw(2) << d->tm_mday;

  return ss.str();
}

inline string get_month_string(struct tm *date) {
  switch (date->tm_mon) {
  case 1:
    return "January";
  case 2:
    return "February";
  case 3:
    return "March";
  case 4:
    return "April";
  case 5:
    return "May";
  case 6:
    return "June";
  case 7:
    return "July";
  case 8:
    return "August";
  case 9:
    return "September";
  case 10:
    return "October";
  case 11:
    return "November";
  case 12:
    return "December";
  default:
    return "Error";
  }
}

#endif // !FUNCTIONS_CPP
