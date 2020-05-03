#include "main.hpp"


using namespace std;

#define MAX_INT 0xFFFFFFFF
#define S 1000
#define MAX_SLEEP 10
#define QUEUES 1


int random_sleep(int b)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> randomizer(0, b);
    return randomizer(gen);
}

Semaphore::Semaphore(int init_val) {
    if(sem_init(&sem, 0, init_val) == -1)
        throw "Semaphore initialization failed.";
}

int Semaphore::value()
{
    int ret;
    if(sem_getvalue(&sem, &ret) == -1)
        throw("Error while getting semaphore value.");
    return ret;
}

void Semaphore::print_value() {
    cout << value() << endl;
}

Semaphore::~Semaphore() {
    sem_destroy(&sem);
}

void Semaphore::signal() {
    if(sem_post(&sem) == -1)
        throw "Semaphore signal failed.";
//    cout << "signal, new value: ";

//    print_value();
}

void Semaphore::wait() {
    if(sem_wait(&sem) == -1)
        throw "Semaphore wait failed.";
//    cout << "wait, new value: ";
//    print_value();
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

    q1_r = Semaphore(0);
    q2_r = Semaphore(0);
    q1_busy = Semaphore(1);
    q2_busy = Semaphore(1);
}

[[noreturn]] void Queues::reader() {
    int q_number = random_sleep(QUEUES);
    while(true)
    {
        if(q_number == 0 && !q1.empty())
        {
            if (q1_r.value() == 0)
                q1_busy.wait();
            q1_r.signal();

            io.lock();
            cout << "q1: message " << q1.back().id << endl;
            io.unlock();

            sleep(2);
            io.lock();
            cout << "q1: reader finished work. Active readers: " << q1_r.value() - 1 << endl;
            io.unlock();


            if(q1_r.value() == 1)
                q1_busy.signal();
            q1_r.wait();
        }
        else if (!q2.empty())
        {
            if (q2_r.value() == 0)
                q2_busy.wait();
            q2_r.signal();

            io.lock();
            cout << "q2: message " << q2.back().id << endl;
            io.unlock();

            sleep(2);

            io.lock();
            cout << "q2: reader finished work. Active readers: " << q2_r.value() - 1 << endl;
            io.unlock();

            if (q2_r.value() == 1)
                q2_busy.signal();
            q2_r.wait();
        }
        sleep(random_sleep(MAX_SLEEP));
    }
}

[[noreturn]] void Queues::producer() {
    while(true){
        Message mes = Message();
        int q_number = random_sleep(QUEUES);
        if (q_number == 0)
        {
            q1_end.wait();
            q1_busy.wait();
            mutex_q1.lock();
            q1.push(mes);

            if(q1_r.value() != 0)
                throw("trying to produce on q1 with active readers!");

            io.lock();
            cout << "q1: added message " << mes.id << endl;
            io.unlock();

            mutex_q1.unlock();
            q1_begin.signal();
            q1_busy.signal();

        }
        else
        {
            q2_end.wait();
            q2_busy.wait();
            mutex_q2.lock();
            q2.push(mes);

            if(q2_r.value() != 0)
                throw("trying to produce on q2 with active readers!");

            io.lock();
            cout << "q2: added message " << mes.id << endl;
            io.unlock();

            mutex_q2.unlock();
            q2_begin.signal();
            q2_busy.signal();
        }
        sleep(random_sleep(MAX_SLEEP));
    }
}

[[noreturn]] void Queues::consumer() {
    while(true){
        int q_number = random_sleep(QUEUES);
        if (q_number == 0)
        {
            q1_begin.wait();
            q1_busy.wait();
            mutex_q1.lock();

            if(q1_r.value() != 0)
                throw("trying to consume on q1 with active readers!");

            io.lock();
            cout << "q1: removed message " << q1.front().id << endl;
            io.unlock();

            q1.pop();
            mutex_q1.unlock();
            q1_end.signal();
            q1_busy.signal();
        }
        else
        {
            q2_begin.wait();
            q2_busy.wait();
            mutex_q2.lock();

            if(q2_r.value() != 0)
                throw("trying to consume on q2 with active readers!");

            io.lock();
            cout << "q2: removed message " << q2.front().id << endl;
            io.unlock();

            q2.pop();
            mutex_q2.unlock();
            q2_end.signal();
            q2_busy.signal();
        }
        sleep(random_sleep(MAX_SLEEP));
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
    thread con[5];
    thread re[5];
    thread pro[5];
    for (int i=0; i<5; ++i) {
        con[i] = thread(consumer);
    }
    for (int i=0; i<5; ++i) {
        re[i] = thread(reader);
    }
    for (int i=0; i<5; ++i) {
        pro[i] = thread(producer);
    }
    for (int i=0; i<5; ++i) {
        con[i].join();
    }
    for (int i=0; i<5; ++i) {
        re[i].join();
    }
    for (int i=0; i<5; ++i) {
        pro[i].join();
    }

}
