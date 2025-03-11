#pragma once
#include <signal.h>
#include <functional>
#include <map>

std::map < int, std ::function < void() >> handlers_;

void signal_handler(int sig) { handlers_[sig](); }

struct Signal {
  // 在程序的任何地方被调用，而不需要创建 Signal 类的对象。
  static void signal(int sig, const std::function<void()> &handler) {
    handlers_[sig] = handler;
    ::signal(sig, signal_handler);
  }
};

