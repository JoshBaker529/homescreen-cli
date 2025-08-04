
#include <ctime>
#include <iostream>

#include <thread>
#include <chrono>

#include <cstdlib>

#include <sys/ioctl.h> // Required for ioctl and winsize
#include <unistd.h>    // Required for STDOUT_FILENO

void print_monthly_calendar();

int main() {

  int i = 0;
    while (i < 10) {
    print_monthly_calendar();

    std::this_thread::sleep_for(std::chrono::seconds(5));
    i++;
  }
  return 0;

}

void print_monthly_calendar() {

  system("clear");
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
    std::cerr << "Error getting terminal size." << std::endl;
    exit(1);
  }

  std::cout << "Rows: " << w.ws_row << " Cols: " << w.ws_col << "\n";
  // Working size, for easy division
  short working_width = (w.ws_col-2)  - ((w.ws_col-2) % 7) + 1;
  short working_height = (w.ws_row-2) - ((w.ws_row-2) % 4);

  // each day box size
  short day_width = working_width / 7;
  short day_heigh = working_height / 4;

  for (int wid = 0; wid < working_width; wid++) {
    std::cout << "#";
  }
  std::cout << "\n";

  for (int week = 0; week < 4; week++) {
    for (int j = 0; j < day_heigh - 1; j++) {
      std::cout << "#";
      for (int day = 0; day < 7; day++) {
        for (int i = 0; i < day_width - 1; i++) {
          std::cout << " ";
        }
        std::cout << "#";
      }
      std::cout << "\n";
    }
  for (int wid = 0; wid < working_width; wid++) {
    std::cout << "#";
  }
  std::cout << "\n";

  }
}
