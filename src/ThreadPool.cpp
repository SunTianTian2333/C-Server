#include "ThreadPool.h"
#include <stdexcept>

ThreadPool::ThreadPool(int size):stop(false){
    for(int i=0;i<size;i++){
        threads.emplace_back(std::thread([this](){
            while(true){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(tasks_mtx);
                    cv.wait(lock,[this](){
                        return stop||!tasks.empty();
                    });
                    if(stop && tasks.empty()) return;
                    task=tasks.front();
                    tasks.pop();
                }
                task();
            }
        }));
    }
}

ThreadPool::~ThreadPool(){
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        stop=true;
    }
    cv.notify_all(); //唤醒所有沉睡线程
    for(std::thread &th:threads){ //以引用的方式访问每个线程对象，这样可以避免拷贝线程对象
        if(th.joinable())
           th.join(); //主线程等待该线程执行完毕
    }
}

// void ThreadPool::add(std::function<void()> func){
//     {
//         std::unique_lock<std::mutex> lock(tasks_mtx);
//         if(stop)
//             throw std::runtime_error("ThreadPool already stop, can't add task any more");
//         tasks.emplace(func);
//     }
//     cv.notify_one();
// }