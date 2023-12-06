#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <utility>
#include <map>
#include <set>
#include <string>
#include "LoggingObserver.h"


// Utility functions
std::vector<std::string> splitString(const std::string &str);
std::map<std::string, std::vector<std::string>> parseTournamentCommand(const std::vector<std::string>& parts);



class Command {
private:
    std::string *command;
    std::string *effect;
public:
    Command(std::string command);

    Command(const Command &other);  // Copy constructor
    Command &operator=(const Command &other);  // Assignment operator
    friend std::ostream &operator<<(std::ostream &os, const Command &command);  // Stream insertion operator
    ~Command();  // Destructor
    void saveEffect(std::string effect);

    std::string getCommand() const;

    std::string getEffect() const;


};

class CommandProcessor : public ILoggable, public LogSubject {
protected:
    std::vector<Command *> *commands;
    std::string *gameState;

    virtual void readCommand();

    void saveCommand(const std::string &command);

    bool validate(const std::string &command);

    void executeCommand(const std::string &command);

public:
    CommandProcessor(std::string initialGameState);

    CommandProcessor(const CommandProcessor &other);  // Copy constructor
    CommandProcessor &operator=(const CommandProcessor &other);  // Assignment operator
    friend std::ostream &operator<<(std::ostream &os, const CommandProcessor &cp);  // Stream insertion operator
    ~CommandProcessor();  // Destructor
    Command getCommand();

    std::string stringToLog() override;
};

class FileCommandProcessorAdapter : public CommandProcessor {
private:
    std::ifstream *file;
    std::string *filename;
protected:
    virtual void readCommand() override;

public:
    FileCommandProcessorAdapter(std::string initialGameState, const std::string &filename);

    FileCommandProcessorAdapter(const FileCommandProcessorAdapter &other);  // Copy constructor
    FileCommandProcessorAdapter &operator=(const FileCommandProcessorAdapter &other);  // Assignment operator
    friend std::ostream &
    operator<<(std::ostream &os, const FileCommandProcessorAdapter &fcpa);  // Stream insertion operator
    bool isEndOfFile();

    ~FileCommandProcessorAdapter();  // Destructor
};

#endif // COMMANDPROCESSING_H
