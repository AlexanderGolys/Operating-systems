
#ifndef UNTITLED_MAIN_HPP
#define UNTITLED_MAIN_HPP

#include <semaphore.h>
#include <queue>
#include <mutex>
#include <unistd.h>
#include <thread>
#include <iostream>


using namespace std;

class Semaphore {
    sem_t sem;
public:
    Semaphore(int init_val=0);
    ~Semaphore();
    void signal();
    void wait();
};

class Message {
    static unsigned int counter;
public:
    Message();
    unsigned int id;
};

class Queues {
    queue<Message> q1;
    queue<Message> q2;
    mutex mutex_q1;
    mutex mutex_q2;
    Semaphore q1_begin;
    Semaphore q2_begin;
    Semaphore q1_end;
    Semaphore q2_end;
public:
    Queues();

    void reader();

    void producer();

    void consumer();
};

#endif //UNTITLED_MAIN_HPP
