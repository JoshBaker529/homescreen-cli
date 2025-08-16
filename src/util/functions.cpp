#include "definitions.hpp"
#include <ctime>

struct tm add_days_to_date(struct tm *date, int days);
struct tm sub_days_to_date(struct tm *date, int days);

struct tm add_days_to_date(struct tm *date, int days) {
  struct tm temp = *date;
  temp.tm_mday = (temp.tm_mday + days);
  if (temp.tm_mday > days_in_month[temp.tm_mon]) {
    temp.tm_mday -= days_in_month[temp.tm_mon];
    temp.tm_mon++;
  }

  return temp;
}

struct tm sub_days_to_date(struct tm *date, int days) {
  struct tm temp = *date;
  temp.tm_mday = (temp.tm_mday - days);
  if (temp.tm_mday < 1) {
    temp.tm_mon--;
    temp.tm_mday += days_in_month[temp.tm_mon];
  }

  return temp;
}
