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
}


int main() {
	sem_init(&sem, 0, 0);
    
    thread thread2(runMeSecond);
    thread thread1(runMeFirst);
    
    thread1.join();
    thread2.join();
    
	return 0;
}