#pragma once

#include "DelFn.hpp"

#include <vector>
#include <atomic>

template<typename T>
class QueueBWNR {
    std::vector<T> buf;
    std::atomic_size_t read_position{0};
    std::atomic_size_t write_position{0};
    std::atomic_bool flushing{false};
public:
    QueueBWNR(std::size_t size) : buf(size) {}

    bool pop(T& val) {
        if(flushing.load())
            return false;
        if(read_position == write_position)
            return false;
        val = std::move(buf.at(read_position));
        read_position = (read_position.load() + 1) % buf.size();
        read_position.notify_one();
        return true;
    }
    void push(T val) {
        if(flushing.load())
            return;
        auto next_pos = (write_position.load() + 1) % buf.size();
        read_position.wait(next_pos);
        buf.at(write_position) = std::move(val);
        write_position = next_pos;
    }

    /**
     *  Sets the queue empty
     *  Unblocks a push() if one is blocked
     *  Puts the queue in a flushing state, until the return object is destroyed
     *  pop() will fail and push() will return immedeately without storing the
     *  value during flushing
     *
     *  Usage example:
     *
     *  if(auto flushing = queue.flush(); true) { // queue is in a flushing state
     *      // synchronize with the writing thread here
     *      ...
     *  }
     *  // queue is out of the flushing state
     *  ...
     *
     *  @param clean - if true, overwrites objects in the queue with default-initialized
     *  @return object ends flushing state upon being destroyed
     */

    DelFn flush(bool clean = false) {
        flushing = true;
        read_position = write_position.load();
        read_position.notify_one();
        read_position = 0;
        write_position = 0;
        std::fill(buf.begin(), buf.end(), {});
        return [this]{flushing = false; };
    }
};
