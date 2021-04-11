#pragma once 

#ifndef FIXED_SIZE_QUEUE_H

#include "Glen/Globals.h"

template<typename T, int MaxLen, typename Container = std::deque<T>>
class FixedSizeQueue : public std::queue<T, Container>
{
public:
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;

    void push(const T& value) {
        if (this->size() == MaxLen) {
            this->c.pop_front();
        }
        std::queue<T, Container>::push(value);
    }

    iterator begin() { return this->c.begin(); }
    iterator end() { return this->c.end(); }
    const_iterator begin() const { return this->c.begin(); }
    const_iterator end() const { return this->c.end(); }
};

#endif // !FIXED_SIZE_QUEUE_H
