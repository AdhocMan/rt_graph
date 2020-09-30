#include <chrono>
#include <iostream>
#include <thread>
#include "rt_graph/rt_graph.hpp"

int main() {
  rt_graph::Timer timer;

  auto sleep = [&]() {
    rt_graph::ScopedTiming scoped2("Sleep", timer);
    std::this_thread::sleep_for(std::chrono::duration<double>(0.1));
  };

  timer.start("Group_1");
  sleep();
  timer.stop("Group_1");

  {
    rt_graph::ScopedTiming scoped2("Group_2", timer);
    {
      rt_graph::ScopedTiming scoped2_1("Once", timer);
      sleep();
    }
    {
      rt_graph::ScopedTiming scoped2_2("Multiple", timer);
      for (int i = 0; i < 5; ++i) {
        sleep();
      }
    }
  }

  // process timings
  const auto result = timer.process();

  // print default statistics
  std::cout << "Default statistic:" << std::endl;
  std::cout << result.print();

  std::cout << std::endl << "Custom statistic:" << std::endl;
  // only print the number of measurements and mean
  std::cout << result.print({rt_graph::Stat::Count, rt_graph::Stat::Mean}) << std::endl;

  return 0;
}
