#ifndef SEMOPS_H
#define SEMOPS_H

// Semaphor-Operationen
#include<mutex>
#include<condition_variable>
		
class my_sem {
    std::mutex mtx;
    std::condition_variable cond;
    unsigned int count;
public:
    my_sem(int value);
    my_sem(const my_sem&) = delete;
    my_sem& operator=(const my_sem&) = delete;
    void wait();
    void post();
    ~my_sem();
    //int sem_getval(); for debugging
};

#endif // SEMOPS_H
