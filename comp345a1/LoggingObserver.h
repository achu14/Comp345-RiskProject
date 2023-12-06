#ifndef COMP345A1_LOGGINGOBSERVER_H
#define COMP345A1_LOGGINGOBSERVER_H

#include <list>
#include <iostream>
#include <fstream>

class ILoggable {


public:
    virtual ~ILoggable();

    virtual std::string stringToLog() = 0;

};


class Observer {
public:
    virtual ~Observer() {}

    virtual void notify(ILoggable *iLoggable) = 0;
};

class LogObserver : public Observer {
public:
    LogObserver();

    ~LogObserver();

    void notify(ILoggable *loggable);

    void notify(ILoggable *loggable, std::string message);


private:
    std::ofstream LogFile;
};


class LogSubject {

public:

    void addObserver(LogObserver *logObserver);

    void removeObserver(LogObserver *logObserver);

    void notifyObservers(ILoggable *iLoggable);

    void notifyObservers(ILoggable *iLoggable, std::string message);


    std::list<LogObserver *> observers;
};


#endif //COMP345A1_LOGGINGOBSERVER_H
