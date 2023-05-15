#include "DelFn.hpp"

DelFn::DelFn(std::function<void()> fn) : callback(fn) {}

DelFn::DelFn(DelFn &&that) : callback(std::move(that.callback)) {}

DelFn &DelFn::operator=(DelFn &&that) noexcept {
    callback = std::move(that.callback);
    return *this;
}

DelFn::~DelFn() { if(callback) callback(); }
