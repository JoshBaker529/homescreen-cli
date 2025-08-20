
#include "day_view.hpp"
#include <chrono> // Required for time duration for sleep_for
#include <thread> // Required for sleep_for

int main() {
  DayView day;
  while (true) {
    day.print_view();
    std::this_thread::sleep_for(std::chrono::minutes(1));
  }
  return 0;
}
