#include "LoggingObserver.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unistd.h>

void LogSubject::addObserver(LogObserver *logObserver) {
    observers.push_back(logObserver);
}

void LogSubject::removeObserver(LogObserver *logObserver) {
    observers.remove(logObserver);
}

void LogSubject::notifyObservers(ILoggable *iLoggable) {
    for (LogObserver *observer: observers) {
        observer->notify(iLoggable);
    }
}

void LogSubject::notifyObservers(ILoggable *iLoggable, std::string message) {
    for (LogObserver *observer: observers) {
        observer->notify(iLoggable, message);
    }
}

LogObserver::LogObserver() : LogFile("game.txt") {

    LogFile = std::ofstream("game.txt");

    if (!LogFile.is_open()) {
        std::cerr << "Error opening log file: game.txt" << std::endl;
    } else {
        LogFile << "LogObserver created " << std::endl;
        LogFile.close();
    }
}

LogObserver::~LogObserver() {
    LogFile.close();
}

void LogObserver::notify(ILoggable *iLoggable) {
    LogFile = std::ofstream("game.txt", std::ios::out | std::ios::app);

    if (LogFile.is_open()) {
        LogFile << iLoggable->stringToLog() << std::endl;
        LogFile.flush();
    }
}

void LogObserver::notify(ILoggable *iLoggable, std::string message) {
    LogFile = std::ofstream("game.txt", std::ios::out | std::ios::app);

    if (LogFile.is_open()) {
        LogFile << message << std::endl;
        LogFile.flush();
    }
}

ILoggable::~ILoggable() {

}
