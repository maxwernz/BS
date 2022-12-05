#include "semops.h"
#include <iostream>
#include <thread>

using namespace std;


void kritisch(my_sem& s) {
  s.wait();
  cout << "Thread " << this_thread::get_id() << ": Im kritischen Programmteil" << endl;
  std::this_thread::sleep_for(2s);
  s.post();
}

void run(my_sem& s) {
  cout << "Thread " << this_thread::get_id() << " versucht kritischen Programmteil zu betreten." << endl;
  kritisch(s);
  cout << "Thread " << this_thread::get_id() << " hat kritischen Programmteil verlassen." << endl;
}

int main() {
    my_sem sem(1);
    std::thread t1(run, std::ref(sem));
    std::thread t2(run, std::ref(sem));
    std::thread t3(run, std::ref(sem));
    std::thread t4(run, std::ref(sem));
 
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

