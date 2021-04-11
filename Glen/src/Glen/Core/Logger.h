#pragma once 

#ifndef LOGGER_H

#include "Glen/Utils/FixedSizeQueue.h"
#include <chrono>
#include <ctime>
#include <iomanip>

// TODO: platfrom specific code
#define NOMINMAX
#include <windows.h>

using namespace std;

class LogWindowPanel;

struct LogMessage {
	string logType;
	string time;
	string message;

	string toString() {
		return time + " " + logType + " " + message;
	}

	LogMessage(string logType, string time, string message): logType(logType),
		time(time), message(message) {}
};

class Logger {
public:
	friend class LogWindowPanel;
	static void logInfo(string message);
	static void logWarn(string message);
	static void logDebug(string message);
	static void logError(string message);
private:
	static FixedSizeQueue<LogMessage, 100> logBuffer;
	static HANDLE  hConsole;

	static void clearLogBuffer();
	static string getCurrentTimeString();
};

#endif // !LOGGER_H
