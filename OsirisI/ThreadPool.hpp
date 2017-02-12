#ifndef ITHREADPOOL_HPP_
#define ITHREADPOOL_HPP_

namespace OsirisI {
	namespace Threading {
		class ThreadPool;

		class Worker {
		public:
			Worker(ThreadPool &s) : pool(s) { }
			void operator()();
		private:
			ThreadPool &pool;
		};

		class ThreadPool {
		public:
			ReturnState Initialize( unsigned int numOfThreads );
			void AddToQueue(std::function<void()> f);
			ReturnState Release();
			ThreadPool();
			~ThreadPool();

		protected:
			friend class Worker;

			// need to keep track of threads so we can join them
			std::vector< std::thread > workers;

			// the task queue
			std::queue< std::function<void()> > tasks;

			// synchronization
			std::mutex queue_mutex;
			std::condition_variable condition;
			bool stop;
		};
	}
}


#endif /* ITHREADPOOL_HPP_ */
