#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <iostream>
#include <string>
using std::string;

struct event {
  string date;
  string class_id;
  string description;
  bool finished;
  bool sched;
  bool important;

  bool operator>(const event &rhs) const {
    if (this->date == rhs.date) {
      if (this->important && !rhs.important)
        return true;
      return false;
    }

    return this->date.compare(rhs.date) > 0;
  }

  event(string d, string c, string desc, bool f, bool s, bool i)
      : date(d), class_id(c), description(desc), finished(f), sched(s),
        important(i) {}

  friend std::ostream &operator<<(std::ostream &os, const event &e);
};

inline std::ostream &operator<<(std::ostream &os, const event &e) {
  os << "{ date: " << e.date << ", class: " << e.class_id
     << ", desc: " << e.description << ", finished: " << e.finished
     << ", sched: " << e.sched << ", important: " << e.important << " }";
  return os;
}
#endif // !EVENTS_HPP
