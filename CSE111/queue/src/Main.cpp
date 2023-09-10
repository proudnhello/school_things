
#include <string>

#include <thread>

#include <unistd.h>
#include <memory>
#include <iostream>
#include <mutex>
#include <chrono>

#include "ThreadSafeQueue.h"
#include "TextMessage.h"

typedef ThreadSafeQueue<std::shared_ptr<TextMessage>> TextMessageQueue;
typedef std::vector<std::shared_ptr<std::thread>> ThreadVector;

std::mutex cout_guard;

void line_output(const std::string & out) {
	// We don't want to see overlapping lines of output
	// so, we synchronize this to output only whole lines at a time
	// this is a "shared resource"
	std::scoped_lock(cout_guard);
	std::cout << out << std::endl;	
}

void producer_func(TextMessageQueue * queue) {

	// "Produce" a number of messages into the queue

	for (size_t i = 0; i < 100; i++) {
		std::string msg = "Hello from : " + std::to_string(i);
		queue->push(std::make_shared<TextMessage>(msg));
	}

}

void consumer_func(TextMessageQueue * queue) {

	// many consumers will consume the messages - each consumer
	// thread runs this function

	while (true) {
		
		std::shared_ptr<TextMessage> msg = queue->pop();
		
		if (msg == nullptr) {
			// null will be returned when the queue aborts
			line_output("consumer_func terminated");	
			return;
		}

		// otherwise we output the message
		line_output("MESSAGE : " + msg->get_text());
	}

}

int main()
{

	TextMessageQueue queue;
	producer_func(&queue);

	ThreadVector threads;

	for (size_t i = 0; i < 10; i++) {
		std::shared_ptr<std::thread> thread = std::make_shared<std::thread>(consumer_func, &queue);
		threads.push_back(thread);
		std::cout << "Made thread: " << i << std::endl;
	}

	do { 
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
		line_output("Queue size is " + std::to_string(queue.size())); 
	} while (queue.size() != 0);	

	queue.abort();

	for (auto thread : threads) {
		thread->join();
	}

	return 0;
}
