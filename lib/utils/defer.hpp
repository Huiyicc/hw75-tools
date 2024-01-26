//
// Created by 19254 on 2024/1/22.
//

#ifndef HW_TOOLS_DEFER_HPP
#define HW_TOOLS_DEFER_HPP

#include <functional>

struct ExitCaller {
  ~ExitCaller() { functor_(); }
  ExitCaller(std::function<void()>&& functor)
      : functor_(std::move(functor)) {}
private:
  std::function<void()> functor_;
};


#define DEFER_PASTE_(x, y) x##y
#define DEFER_CAT_(x, y) DEFER_PASTE_(x, y)

#define DEFER(code) \
  ExitCaller DEFER_CAT_(defer_, __LINE__)([&](){code;}) \


#endif//HW_TOOLS_DEFER_HPP
