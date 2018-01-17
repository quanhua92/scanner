
#pragma once

#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>

namespace scanner {

template <typename T>
class Queue {
 public:
  Queue(int max_size = 4);
  Queue(Queue<T>&& o);

  int size();

  template <typename... Args>
  void emplace(Args&&... args);

  void push(T item);

  bool try_pop(T& item);

  void pop(T& item);

  void peek(T& item);

  void clear();

  void wait_until_empty();

 private:
  i32 max_size_;
  std::mutex mutex_;
  std::condition_variable empty_;
  std::condition_variable not_empty_;
  std::condition_variable not_full_;
  std::deque<T> data_;
  std::atomic<int> pop_waiters_{0};
  std::atomic<int> push_waiters_{0};
};
}

#include "queue.inl"
