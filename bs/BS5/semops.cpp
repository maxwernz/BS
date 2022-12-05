#include "semops.h"

my_sem::my_sem(int value) {
    count = value;
}

void my_sem::wait() {
    std::unique_lock<std::mutex> lock(mtx);
    cond.wait(lock, [&]() {return count > 0;});
    count -= 1;
}

void my_sem::post() {
    std::unique_lock<std::mutex> lock(mtx);
    count += 1;
    cond.notify_one();
}

my_sem::~my_sem() {}