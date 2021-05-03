#pragma once 

#ifndef LOGGER_H

#include "Glen/Utils/FixedSizeQueue.h"
#include <chrono>
#include <ctime>
#include <iomanip>

// TODO: platfrom specific code
#define NOMINMAX
#include <windows.h>

class LogWindowPanel;

struct LogMessage {
	std::string logType;
	std::string time;
	std::string message;

	std::string toString() {
		return time + " " + logType + " " + message;
	}

	LogMessage(std::string logType, std::string time, std::string message) : logType(logType),
		time(time), message(message) {}
};

class GLN_API Logger {
public:
	friend class LogWindowPanel;
	static void logInfo(std::string message);
	static void logWarn(std::string message);
	static void logDebug(std::string message);
	static void logError(std::string message);
private:
	static FixedSizeQueue<LogMessage, 100> logBuffer;
	static HANDLE  hConsole;

	static void clearLogBuffer();
	static std::string getCurrentTimeString();
};

#endif // !LOGGER_H
