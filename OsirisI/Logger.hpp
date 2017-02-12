#ifndef LOGGER_HPP_
#define LOGGER_HPP_

namespace OsirisI {
	namespace Utilities {
		enum LoggerType { OSIRIS_LOGGER_DB,	OSIRIS_LOGGER_FILE };	
		enum LogRange { LOG_RANGE_INFO = 0, LOG_RANGE_DEBUG, LOG_RANGE_WARN, LOG_RANGE_ERROR };

		#define LOG_INFO(msg) {       \
			std::stringstream sstr;   \
			sstr << msg;              \
			OsirisI::Utilities::Logger::Info(sstr.str()); \
		}
		#define LOG_DEBUG(msg) {           \
			std::stringstream sstr;    \
            sstr << msg << " - " << __FILE__ << ":" << std::dec << __LINE__;\
            OsirisI::Utilities::Logger::Debug(sstr.str()); \
		}
		#define LOG_DEBUG_SMALL(msg) {     \
			std::stringstream sstr;    \
            sstr << msg;               \
            OsirisI::Utilities::Logger::Debug(sstr.str()); \
		}
		#define LOG_WARNING(msg) {         \
			std::stringstream sstr;        \
            sstr << msg << " - " << __FILE__ << ":" << std::dec << __LINE__;\
			OsirisI::Utilities::Logger::Warning(sstr.str());   \
		}
		#define LOG_WARNING_SMALL(msg) { \
			std::stringstream sstr;      \
			sstr << msg;                 \
			OsirisI::Utilities::Logger::Warning(sstr.str()); \
		}
		#define LOG_ERROR(msg) {       \
			std::stringstream sstr;    \
            sstr << msg << " - " << __FILE__ << ":" << std::dec << __LINE__;\
			OsirisI::Utilities::Logger::Error(sstr.str()); \
		}
		#define LOG_ERROR_SMALL(msg) { \
			std::stringstream sstr;    \
			sstr << msg;               \
			OsirisI::Utilities::Logger::Error(sstr.str()); \
		}
		#define LOG_NESTED(msg) { \
			std::stringstream sstr;     \
			sstr << msg;                \
			OsirisI::Utilities::Logger::Nested(sstr.str()); \
		}
		#define LOG_NESTED_NEW(msg) { \
			std::stringstream sstr;     \
			sstr << msg;                \
			OsirisI::Utilities::Logger::Nested(sstr.str(), 2); \
		}
		#define LOG_NESTED_DOWN(msg) { \
			std::stringstream sstr;     \
			sstr << msg;                \
			OsirisI::Utilities::Logger::Nested(sstr.str(), 0); \
		}
		#define LOG_NESTED_UP(msg) { \
			std::stringstream sstr;     \
			sstr << msg;                \
			OsirisI::Utilities::Logger::Nested(sstr.str(), 1); \
		}

		class Logger {
			protected:
				static LoggerType lType;
				static LogRange range;
				static LogRange nestedType;
				static std::string logFilePath;
				static std::fstream logFile;
				static int nestedDepth;
				Logger();

			public:
				~Logger();

				static ReturnState Initialize(LoggerType type, LogRange pRange);
				static ReturnState Recover();
				static ReturnState Release();


				static ReturnState Info(std::string msg);
				static ReturnState Debug(std::string msg);
				static ReturnState Warning(std::string msg);
				static ReturnState Error(std::string msg);
				static ReturnState Nested(std::string msg, int nestedStage = -1);
				static LoggerType GetLoggerType();
			
				static ReturnState Debug_AllocTrace(std::string msg, ...);
				static ReturnState Debug_FreeTrace(std::string msg, ...);
				static void* operator new(size_t sz);
				static void* operator new[](size_t sz);
				static void operator delete(void* ptr);
				static void operator delete[](void* ptr);


			private:
				static std::string GetDateTime();
				static ReturnState Log(std::string msg);
		};
	}
}

#endif /* LOGGER_HPP_ */
