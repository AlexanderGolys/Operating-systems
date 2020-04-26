#include "main.hpp"


using namespace std;

#define MAX_INT 0xFFFFFFFF
#define S 1000
#define MAX_SLEEP 10

Semaphore::Semaphore(int init_val) {
    if(sem_init(&sem, 0, init_val) == -1)
        throw "Semaphore initialization failed.";
}

Semaphore::~Semaphore() {
    sem_destroy(&sem);
}

void Semaphore::signal() {
    if(sem_post(&sem) == -1)
        throw "Semaphore signal failed.";
}

void Semaphore::wait() {
    if(sem_wait(&sem) == -1)
        throw "Semaphore wait failed.";
}

unsigned int Message::counter = 0;
Message::Message() {
    counter = (counter + 1) % (MAX_INT - 1);
    id = counter;
}

Queues::Queues() {
    q1_begin = Semaphore(0);
    q2_begin = Semaphore(0);
    q1_end = Semaphore(S);
    q2_end = Semaphore(S);
}

void Queues::reader() {
    int q_number = rand() % 2;
    while(true)
    {
        if(q_number == 0 && !q1.empty())
        {
            mutex_q1.lock();
            cout << "message " << q1.back().id << endl;
            mutex_q1.unlock();
        }
        else if (!q1.empty())
        {
            mutex_q2.lock();
            cout << "message " << q2.back().id << endl;
            mutex_q2.unlock();
        }
        sleep(rand() % (MAX_SLEEP + 1));
    }
}

 void Queues::producer() {
    while(true){
        Message mes = Message();
        int q_number = rand() % 2;
        if (q_number == 0)
        {
            q1_end.wait();
            mutex_q1.lock();
            q1.push(mes);
            cout << "added message " << mes.id << " to q1." << endl;
            mutex_q1.unlock();
            q1_begin.signal();
        }
        else
        {
            q2_end.wait();
            mutex_q2.lock();
            q2.push(mes);
            cout << "added message " << mes.id << " to q1." << endl;
            mutex_q2.unlock();
            q2_begin.signal();
        }
        sleep(rand() % (MAX_SLEEP + 1));
    }
}

void Queues::consumer() {
    while(true){
        int q_number = rand() % 2;
        if (q_number == 0)
        {
            q1_begin.wait();
            mutex_q1.lock();
            cout << "removed message " << q1.back().id << " from q1." << endl;
            q1.pop();
            mutex_q1.unlock();
            q1_end.signal();
        }
        else
        {
            q2_begin.wait();
            mutex_q2.lock();
            q2.pop();
            cout << "removed message " << q2.back().id << " from q2." << endl;
            mutex_q2.unlock();
            q2_end.signal();
        }
        sleep(rand() % (MAX_SLEEP + 1));
    }
}

Queues q;

void consumer()
{
    q.consumer();
}

void reader()
{
    q.reader();
}

void producer()
{
    q.producer();
}
int main() {
    thread con[10];
    thread re[10];
    thread pro[10];
    for (int i=0; i<10; ++i) {
        con[i] = thread(consumer);
        re[i] = thread(reader);
        pro[i] = thread(producer);
        con[i].join();
        re[i].join();
        pro[i].join();

    }

}
