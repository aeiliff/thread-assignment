#include <iostream>
#include <thread>
#include <semaphore.h>

using namespace std;

sem_t sem;

void runMeFirst() {
	cout << "Run me first" << endl;
	sem_post(&sem); 
}

void runMeSecond() {
	sem_wait(&sem);
	cout << "Run me second" << endl;
	sem_post(&sem);
}

void runMeThird() {
	sem_wait(&sem);
	//sem_post(&sem);
	this_thread::sleep_for(chrono::seconds(1)); // replace this with the other two commented out lines, and this function will still run third
	//sem_wait(&sem);
	cout << "Run me Third" << endl;
}

int main() {
	sem_init(&sem, 0, 0);
    thread thread2(runMeSecond);
    thread thread3(runMeThird);
    thread thread1(runMeFirst);
    
    
    thread1.join();
    thread2.join();
    thread3.join();
    
	return 0;
}