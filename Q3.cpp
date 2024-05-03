#include <iostream>
#include <thread>
#include <semaphore.h>
#include <queue>
#include <condition_variable>
#include <stdlib.h>

using namespace std;

condition_variable cv; // declare condition variable
bool ready = false; // shared flag
mutex mtx; // declare mutex

struct requestStructure {
    int request_id; 
    string ip_address;
   	string page_requested;
} rs;

queue<requestStructure> msg_queue;

void doRequest (int numThread) {
	
	while (true){
		unique_lock<mutex> lck(mtx);
		//cv.wait(lck, []{return !msg_queue.empty();});
		if (!msg_queue.empty()) {
		
			rs = msg_queue.front();
			msg_queue.pop();
			cout << rs.request_id << " ";
			cout << rs.ip_address << " ";
			cout << rs.page_requested << endl;
			cout << "Carrying Out Request " << rs.request_id << " With Thread " << numThread << endl;
			cv.wait(lck);
		}
		
		//lck.unlock();
	}	
}

void listen () {
	// Declare necessary variables
	int request_id = 0;
	string ip_address = "2009";
	int iPage = 0;
	int iSeconds = 0;
	ready = true;
	string webPages[10] = {"google.com", "yahoo.com", "youtube.com", "itch.io", 
						   "edge.com", "github.com", "vscode.com", "myWebsite.org",
						   "superSecure.com", "notTrackingYou.com"};
	while (true) {
		// Get random numbers to select sleep time and webPage out of array
		iSeconds = rand() % 3 + 1;
		iPage = rand() % 9 + 0;    	
		this_thread::sleep_for(chrono::seconds(iSeconds));   // Make the thread sleep for a random number of seconds (1-3)
		unique_lock<mutex> lck(mtx);
		// Add the required information to the requestStructure struct fields
		rs.request_id = request_id; 
		rs.ip_address = ip_address;
		rs.page_requested = webPages[iPage];
		
		// Push one struct into the queue and increment request_id
		msg_queue.push(rs);
		lck.unlock();
		request_id++;

		cv.notify_one(); // notify one waiting thread to wake up
	} 	
	cv.notify_all(); // notify all waiting threads -- shouldn't happen
}

int main () {
	int num_threads = 4;
	thread threads[num_threads];
	thread t1(listen);
	for (int i = 0; i < num_threads; i++) {
		threads[i] = thread(doRequest, i); // Declare an array of threads
	}

	t1.join();
	for (int i = 0; i < num_threads; i++) {
		threads[i].join(); // Declare an array of threads
	}
	
	
	return 0;
}