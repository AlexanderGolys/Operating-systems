
#ifndef UNTITLED_MAIN_HPP
#define UNTITLED_MAIN_HPP

#include <semaphore.h>
#include <queue>
#include <mutex>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <random>


using namespace std;

mutex io;

class Semaphore {
    sem_t sem;
public:
    Semaphore(int init_val=0);
    ~Semaphore();
    void signal();
    void wait();
    int value();
    void print_value();
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
    Semaphore q1_busy;
    Semaphore q1_r;
    Semaphore q2_busy;
    Semaphore q2_r;
    Semaphore q1_begin;
    Semaphore q2_begin;
    Semaphore q1_end;
    Semaphore q2_end;
public:
    Queues();

    [[noreturn]] void reader();

    [[noreturn]] void producer();

    [[noreturn]] void consumer();
};

#endif //UNTITLED_MAIN_HPP
