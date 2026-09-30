#include <iostream>
#include <sstream>
#include <vector>
#include <future>

#include "threadfuncs.h"

int main() {
  about();

  Logger logger("output.log");

  {
    std::ostringstream oss;
    oss << "main: pid = " << getThreadID()
        << ", opened file: 'output.log'";
    logger.writeLine(oss.str());
  }

  std::vector<ThreadArgs> args(COUNT_THREADS);
  for (int i = 0; i < COUNT_THREADS; ++i) {
    std::ostringstream oss;
    oss << "T" << i;
    args[i].id  = i;
    args[i].tag = oss.str();
  }

  // async launch
  std::vector<std::future<std::string>> futures;
  futures.reserve(COUNT_THREADS);

  for (int i = 0; i < COUNT_THREADS; ++i) {
    futures.push_back(
      std::async(std::launch::async, funcThread,
                 std::cref(args[i]), std::ref(logger))
    );
  }

  for (auto& f : futures) {
    logger.writeLine("result: " + f.get());
  }

  logger.writeLine("main: all threads finished, file closed");
  return 0;
}
