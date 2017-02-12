#include "stdafx.h"

namespace OsirisI {
	namespace Utilities {
		LogRange Logger::range;
		LogRange Logger::nestedType;
		LoggerType Logger::lType;
		std::string Logger::logFilePath;
		std::fstream Logger::logFile;
		int Logger::nestedDepth = 1;

		std::string Logger::GetDateTime() {
			time_t t = time(0);
			tm now;
			localtime_s(&now, &t);
			std::stringstream ss;

			ss << (1900 + now.tm_year) << "_";
			ss << (((now.tm_mon + 1) < 10) ? "0" : "") << now.tm_mon + 1 << "_";
			ss << ((now.tm_mday < 10) ? "0" : "") << now.tm_mday << " ";

			ss << ((now.tm_hour < 10) ? "0" : "") << now.tm_hour << ":";
			ss << ((now.tm_min < 10) ? "0" : "") << now.tm_min << ":";
			ss << ((now.tm_sec < 10) ? "0" : "") << now.tm_sec;
			return ss.str();
		}

		ReturnState Logger::Initialize(LoggerType type, LogRange pRange) {
			range = LOG_RANGE_INFO;
			lType = type;
			if (type == OSIRIS_LOGGER_FILE) {
				std::string dateTime = GetDateTime();
				std::replace(dateTime.begin(), dateTime.end(), ':', '_');
				logFilePath = "LOG/" + dateTime + "_Osiris.log";
				_mkdir("LOG");

				Info(std::string("Log opened: " + logFilePath));
				Info(std::string("Logging on state: " + pRange));
				range = pRange;
			}
			else if (type == OSIRIS_LOGGER_DB) {

			}
			else {
				return OS_LOGGERTYPE_UNKNOWN;
			}
			return OS_OK;
		}

		ReturnState Logger::Log(std::string msg) {
			switch (lType) {
				case OSIRIS_LOGGER_FILE:
				{
					logFile.open(logFilePath, std::ios_base::in | std::ios_base::out | std::ios_base::app);
					if (logFile.is_open()) {
						logFile << msg << "\n";
						logFile.close();
					}
					else {
						std::cout << "Error opening Logfile!" << std::endl;
					}

#ifdef DEBUG
					OutputDebugStringA((msg + "\n").c_str());
#endif
					break;
				}
				case OSIRIS_LOGGER_DB:
				{
					break;
				}
				default:
				{
					return 0x10;
					break;
				}
			}
			return OS_OK;
		}

		ReturnState Logger::Recover() {
			return OS_OK;
		}

		ReturnState Logger::Release() {
			range = LOG_RANGE_INFO;
			Info("Log closed");
			switch (lType) {
				case OSIRIS_LOGGER_FILE:
				{
					break;
				}
				case OSIRIS_LOGGER_DB:
				{
					break;
				}
				default:
				{
					return 0x10;
					break;
				}
			}
			return OS_OK;
		}

		ReturnState Logger::Info(std::string msg) {
			if (range > LogRange::LOG_RANGE_INFO) return false;
			nestedType = LogRange::LOG_RANGE_INFO;
			return Log("[INFO] " + GetDateTime() + " " + msg);
		}
		ReturnState Logger::Debug(std::string msg) {
			if (range > LogRange::LOG_RANGE_DEBUG) return false;
			nestedType = LogRange::LOG_RANGE_DEBUG;
			return Log("[DEBG] " + GetDateTime() + " " + msg);
		}
		ReturnState Logger::Warning(std::string msg) {
			if (range > LogRange::LOG_RANGE_WARN) return false;
			nestedType = LogRange::LOG_RANGE_WARN;
			return Log("[WARN] " + GetDateTime() + " " + msg);
		}
		ReturnState Logger::Error(std::string msg) {
			if (range > LogRange::LOG_RANGE_ERROR) return false;
			nestedType = LogRange::LOG_RANGE_ERROR;
			return Log("[ERRO] " + GetDateTime() + " " + msg);
		}
		ReturnState Logger::Nested(std::string msg, int nestedStage) {
			if (nestedStage == 0) {
				nestedDepth++;
			}
			else if (nestedStage == 1) {
				nestedDepth--;
			}
			else if (nestedStage == 2) {
				nestedDepth = 1;
			}

			std::string nestedBuffer = "";
			for (int i = 0; i < nestedDepth; i++) {
				nestedBuffer += "    ";
			}

			std::string type = ((nestedType == LogRange::LOG_RANGE_INFO) ? "[INFO]" : (((nestedType == LogRange::LOG_RANGE_WARN) ? "[WARN]" : (((nestedType == LogRange::LOG_RANGE_DEBUG) ? "[DEBG]" : (((nestedType == LogRange::LOG_RANGE_ERROR) ? "[ERRO]" : "")))))));
			return Log(type + " " + GetDateTime() + nestedBuffer + msg);
		}

		LoggerType Logger::GetLoggerType() {
			return lType;
		}

		void* Logger::operator new(size_t size) {
			if (size == 0) {
				throw std::bad_alloc();
			}

			void* p = malloc(size);
			if (p == 0) {
				short tryCount = 0;
				while (p == 0 && tryCount < 10) {
					std::this_thread::sleep_for(std::chrono::microseconds(500));
					p = malloc(size);
					tryCount++;
				}
				if (p == 0) {
					throw std::bad_alloc();
				}
			}
			return p;
		}

		void* Logger::operator new[](size_t size) {
			if (size == 0) {
				throw std::bad_alloc();
			}

			void* p = malloc(size);
			if (p == 0) {
				short tryCount = 0;
				while (p == 0 && tryCount < 10) {
					std::this_thread::sleep_for(std::chrono::microseconds(500));
					p = malloc(size);
					tryCount++;
				}
				if (p == 0) {
					throw std::bad_alloc();
				}
			}
			return p;
		}
			void Logger::operator delete(void* ptr) {
			free(ptr);
		}
		void Logger::operator delete[](void* ptr) {
			free(ptr);
		}
	}
}
