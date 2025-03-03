#pragma once
#include <cassert>
#include <stdexcept>
#define OS_LINUX

// 禁用拷贝和移动
#define DISALLOW_COPY(cname)                             \
  cname(const cname &) = delete;            /* NOLINT */ \
  cname &operator=(const cname &) = delete; /* NOLINT */

#define DISALLOW_MOVE(cname)                        \
  cname(cname &&) = delete;            /* NOLINT */ \
  cname &operator=(cname &&) = delete; /* NOLINT */

#define DISALLOW_COPY_AND_MOVE(cname) \
  DISALLOW_COPY(cname);               \
  DISALLOW_MOVE(cname);

// DEBUG环境下使用assert
#define ASSERT(expr, message) assert((expr) && (message))

// 处理不应该执行的代码路径，如果代码执行到这里，就抛出异常。
#define UNREACHABLE(message) throw std::logic_error(message)
