

#include "todo_list.hpp"
#include <chrono>
#include <thread>

int main() {
  TodoList todo;
  while (true) {
    todo.print_list();

    std::this_thread::sleep_for(std::chrono::minutes(15));
  }
  return 0;
}
