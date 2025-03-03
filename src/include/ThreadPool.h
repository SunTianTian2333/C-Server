#pragma once
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

class ThreadPool {
 private:
  std::vector<std::thread> threads;
  std::queue<std::function<void()>> tasks;
  std::mutex tasks_mtx;
  std::condition_variable cv;
  bool stop;

 public:
  explicit ThreadPool(int size = 10);
  ~ThreadPool();

  // void add(std::function<void()>);

  template <class F, class... Args>
  auto Add(F &&f, Args &&... args) -> std::future<typename std::invoke_result<F, Args...>::type>;

};

// c++不支持模板分离编译
template <class F, class... Args>
auto ThreadPool::Add(F &&f, Args &&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
  using return_type = typename std::invoke_result<F, Args...>::type;

  auto task = std::make_shared<std::packaged_task<return_type()>>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(tasks_mtx);

    // don't allow enqueueing after stopping the pool
    if (stop) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }

    tasks.push([task]() { (*task)(); });
  }
  cv.notify_one();
  return res;
}

