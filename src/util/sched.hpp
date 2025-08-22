
#include <cstddef>
#include <iostream>
#include <string>

struct sched {
  int day;
  std::string class_id;
  std::string location;
  std::string start;
  std::string end;

  bool operator<(const sched &rhs) const {
    int leftHour = std::stoi(start.substr(0, 2)) % 12;
    int leftMinute = std::stoi(start.substr(3, 2));
    int leftAmPm = start.substr(5, 2) == "AM" ? 0 : 1;

    int rightHour = std::stoi(rhs.start.substr(0, 2)) % 12;
    int rightMinute = std::stoi(rhs.start.substr(3, 2));
    int rightAmPm = rhs.start.substr(5, 2) == "AM" ? 0 : 1;

    if (leftAmPm < rightAmPm)
      return true;
    if (leftAmPm == rightAmPm) {
      if (leftHour < rightHour)
        return true;
      if (leftHour == rightHour) {
        if (leftMinute < rightMinute)
          return true;
      }
    }

    return false;
  }

  sched(int d, std::string c, std::string l, std::string s, std::string e)
      : day(d), class_id(c), location(l), start(s), end(e) {}

  friend std::ostream &operator<<(std::ostream &os, const sched &e);
};
inline std::ostream &operator<<(std::ostream &os, const sched &e) {
  os << "{ day: " << e.day << ", class: " << e.class_id
     << ", loc: " << e.location << ", start: " << e.start << ", end: " << e.end
     << " }";
  return os;
}
