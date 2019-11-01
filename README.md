# RTGraph - Runtime Graph library for C++11

RTGraph targets C++11 and above. It only relies on the C++ standard library and provides accurate runtime measurements, which are grouped by their identifier string and position in the call graph.

## Features
- Very small overhead
  - Storage for measurements is pre-allocated. Default is for 1000'000 measurements.
  - At each start / stop, only a pointer to a string literal and the current time is written to memory.
  - Dynamic strings as identifiers are supported, but have to be moved / copied.
  - Entire graph construction as done in separate processing call.
- Print of summary can be customized with a selection of statistical properties.
- JSON export of all measurements and entire graph.


## Example
```cpp
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
```

### Output:
```
Default statistic:
=================================================================================================
                  #         Total          %   Parent %        Median           Min           Max
-------------------------------------------------------------------------------------------------
Group_1           1     100.30 ms     100.00     100.00     100.30 ms     100.30 ms     100.30 ms
 |- Sleep         1     100.30 ms     100.00     100.00     100.30 ms     100.30 ms     100.30 ms

Group_2           1     619.16 ms     100.00     100.00     619.16 ms     619.16 ms     619.16 ms
 |- Once          1     100.76 ms      16.27      16.27     100.76 ms     100.76 ms     100.76 ms
 | |- Sleep       1     100.76 ms      16.27     100.00     100.76 ms     100.76 ms     100.76 ms
 |
 |- Multiple      1     518.40 ms      83.73      83.73     518.40 ms     518.40 ms     518.40 ms
 | |- Sleep       5     518.40 ms      83.73     100.00     105.05 ms     100.39 ms     105.06 ms

=================================================================================================

Custom statistic:
=================================
                  #          Mean
---------------------------------
Group_1           1     100.30 ms
 |- Sleep         1     100.30 ms

Group_2           1     619.16 ms
 |- Once          1     100.76 ms
 | |- Sleep       1     100.76 ms
 |
 |- Multiple      1     518.40 ms
 | |- Sleep       5     103.68 ms

=================================
```
