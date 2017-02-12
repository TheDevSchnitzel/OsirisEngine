#ifndef TASKMANAGER_HPP_
#define TASKMANAGER_HPP_

namespace OsirisI {
    namespace Threading {
        class ThreadPool;
    }
    namespace Manager {
		class TaskManager {
		protected:
			static OsirisI::Threading::ThreadPool* threadPool;

		public:
			static ReturnState Initialize();

			static void AddTask(std::function<void()> func);

			static ReturnState Release();

            static std::thread StartThread(std::function<void()> func);
		};
	}
}

#endif /* TASKMANAGER_HPP */
