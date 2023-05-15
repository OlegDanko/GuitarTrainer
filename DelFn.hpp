#pragma once

#include <functional>
class DelFn {
    std::function<void()> callback;
public:
    DelFn(std::function<void()> fn);
    DelFn(const DelFn&) = delete;
    DelFn(DelFn&& that);
    DelFn& operator=(const DelFn&) = delete;
    DelFn& operator=(DelFn&& that) noexcept;
    ~DelFn();
};
