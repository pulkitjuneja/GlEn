#include "Logger.h"

FixedSizeQueue<LogMessage, 100> Logger::logBuffer;
HANDLE Logger::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void Logger::logInfo(std::string message)
{
	LogMessage logMessage("INFO", getCurrentTimeString(), message);
	logBuffer.emplace(logMessage);
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << logMessage.toString()<<"\n";
	SetConsoleTextAttribute(hConsole, 15);
}

void Logger::logWarn(std::string message)
{
	LogMessage logMessage("WARN", getCurrentTimeString(), message);
	logBuffer.emplace(logMessage);
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << logMessage.toString() << "\n";
	SetConsoleTextAttribute(hConsole, 15);
}

void Logger::logDebug(std::string message)
{
	LogMessage logMessage("DEBUG", getCurrentTimeString(), message);
	logBuffer.emplace(logMessage);
	SetConsoleTextAttribute(hConsole, 11);
	std::cout << logMessage.toString() << "\n";
	SetConsoleTextAttribute(hConsole, 15);
}

void Logger::logError(std::string message)
{
	LogMessage logMessage("ERROR", getCurrentTimeString(), message);
	logBuffer.emplace(logMessage);
	SetConsoleTextAttribute(hConsole, 12);
	std::cout << logMessage.toString() << "\n";
	SetConsoleTextAttribute(hConsole, 15);
}

void Logger::clearLogBuffer()
{
	FixedSizeQueue<LogMessage, 100> empty;
	swap(logBuffer, empty);
}

std::string Logger::getCurrentTimeString()
{
	auto now = std::chrono::system_clock::now();
	time_t now_c = std::chrono::system_clock::to_time_t(now);

	auto t = time(&now_c);
	auto tm = *localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
	std::string str = oss.str();

	return str;
}
	