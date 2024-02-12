#include <iostream>

#include "lib/CCircularBuffer.h"
#include "lib/CCircularBufferExp.h"

int main() {
    CCircularBuffer<int> buffer(5);
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    buffer.push_back(4);
    buffer.push_back(5);

    auto it1 = buffer.begin() + 1;
    auto it2 = buffer.end() - 1;

    buffer.erase(it1, it2);

    auto begin = buffer.begin();
    auto end = buffer.end();


    while (begin != end) {
        std::cout <<*begin << ' ';
        ++begin;
    }

    return 0;
}


