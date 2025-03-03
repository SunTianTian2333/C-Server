#include "ThreadPool.h"
#include <stdexcept>


ThreadPool::ThreadPool(unsigned int size) {
  for (unsigned int i = 0; i < size; ++i) {
    workers_.emplace_back(std::thread([this]() {
      while (true) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(queue_mutex_);
          condition_variable_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });
          if (stop_ && tasks_.empty()) {
            return;
          }
          task = tasks_.front();
          tasks_.pop();
        }
        task();
      }
    }));
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop_ = true;
  }
  condition_variable_.notify_all();
  for (std::thread &th : workers_) {
    if (th.joinable()) {
      th.join();
    }
  }
}

// void ThreadPool::add(std::function<void()> func){
//     {
//         std::unique_lock<std::mutex> lock(tasks_mtx);
//         if(stop)
//             throw std::runtime_error("ThreadPool already stop, can't add task
//             any more");
//         tasks.emplace(func);
//     }
//     cv.notify_one();
// }
