#include "stdafx.h"

using namespace OsirisI::Utilities;

namespace OsirisI {
	namespace Threading {
		void Worker::operator()() {
			std::function<void()> task;
			while(true) {
				{   // acquire lock
					std::unique_lock<std::mutex> lock(pool.queue_mutex);

					// look for a work item
					while(!pool.stop && pool.tasks.empty())
					{ // if there are none wait for notification
						pool.condition.wait(lock);
					}

					// exit if the pool is stopped
					if(pool.stop) return;

					// get the task from the queue	
					LOG_DEBUG( "Executing before front \n" );
					task = pool.tasks.front();
					LOG_DEBUG( "Executing before pop \n" );
					pool.tasks.pop();

					LOG_DEBUG( "Executing: " << task.target_type().name() << "\n" );
				}   // release lock

				// execute the task
				if(task != NULL) task();
				if(pool.stop) return; //Preventing DeadLocks
			}
		}

		ThreadPool::ThreadPool() {
			this->stop = false;
		}

		// the constructor just launches some amount of workers
		ReturnState ThreadPool::Initialize(unsigned int numOfThreads) {
			if(numOfThreads == 0) numOfThreads = std::thread::hardware_concurrency() - 1;
			for(unsigned int i = 0; i < numOfThreads; ++i) {
				Worker* w = new Worker(*this);
				std::function<void()> f([=]() {w->operator()();});
				workers.push_back(std::thread(f));
			}
			return OS_OK;
		}

		ThreadPool::~ThreadPool() {
		}


		// add new work item to the pool
		void ThreadPool::AddToQueue(std::function<void()> f) {         
			if(f == NULL) return;

			{
				this->queue_mutex.lock();
				std::lock_guard<std::mutex> lockG(this->queue_mutex, std::adopt_lock);
				tasks.push(std::function<void()>(f)); // add the task
			}

			// wake up one thread
			condition.notify_one();
		}

		ReturnState ThreadPool::Release() {
			// stop all threads
			stop = true;
			condition.notify_all();

			// join them
			for(size_t i = 0; i < workers.size(); i++) workers[i].join();

			return OS_OK;
		}
	}
}
