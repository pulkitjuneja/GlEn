#include "Logger.h"

FixedSizeQueue<LogMessage, 100> Logger::logBuffer;
HANDLE Logger::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void Logger::logInfo(string message)
{
	LogMessage logMessage("INFO", getCurrentTimeString(), message);
	logBuffer.emplace(logMessage);
	SetConsoleTextAttribute(hConsole, 10);
	cout << logMessage.toString()<<"\n";
	SetConsoleTextAttribute(hConsole, 15);
}

void Logger::logWarn(string message)
{
	LogMessage logMessage("WARN", getCurrentTimeString(), message);
	logBuffer.emplace(logMessage);
	SetConsoleTextAttribute(hConsole, 14);
	cout << logMessage.toString() << "\n";
	SetConsoleTextAttribute(hConsole, 15);
}

void Logger::logDebug(string message)
{
	LogMessage logMessage("DEBUG", getCurrentTimeString(), message);
	logBuffer.emplace(logMessage);
	SetConsoleTextAttribute(hConsole, 11);
	cout << logMessage.toString() << "\n";
	SetConsoleTextAttribute(hConsole, 15);
}

void Logger::logError(string message)
{
	LogMessage logMessage("ERROR", getCurrentTimeString(), message);
	logBuffer.emplace(logMessage);
	SetConsoleTextAttribute(hConsole, 12);
	cout << logMessage.toString() << "\n";
	SetConsoleTextAttribute(hConsole, 15);
}

void Logger::clearLogBuffer()
{
	FixedSizeQueue<LogMessage, 100> empty;
	swap(logBuffer, empty);
}

string Logger::getCurrentTimeString()
{
	auto now = chrono::system_clock::now();
	time_t now_c = chrono::system_clock::to_time_t(now);

	auto t = time(&now_c);
	auto tm = *localtime(&t);

	ostringstream oss;
	oss << put_time(&tm, "%d-%m-%Y %H-%M-%S");
	string str = oss.str();

	return str;
}
	