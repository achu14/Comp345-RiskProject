#include "CommandProcessing.h"
#include <iterator>

// Assuming gameState is a std::string* and points to a string that holds the current game state.
// Add these private members to your CommandProcessor class:
std::map<std::string, std::set<std::string>> validCommands = {
        {"start",        {"tournament", "loadmap"}},
        {"maploaded",    {"validatemap", "loadmap"}},
        {"mapvalidated", {"addplayer"}},
        {"playersadded", {"gamestart",   "addplayer"}},
        {"win",          {"replay",      "quit"}}
};

std::map<std::string, std::string> nextState = {
        {"loadmap",     "maploaded"},
        {"validatemap", "mapvalidated"},
        {"addplayer",   "playersadded"},
        {"gamestart",   "assignreinforcement"},
        {"tournament",  "tournamentmode"},
        {"replay",      "start"},
        {"quit",        "exit"}
};

// Helper function to split a string by whitespace and return the parts
std::vector<std::string> splitString(const std::string &str) {
    std::istringstream iss(str);
    std::vector<std::string> parts(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
    return parts;
}

std::map<std::string, std::vector<std::string>> parseTournamentCommand(const std::vector<std::string>& parts) {
    std::map<std::string, std::vector<std::string>> parameters;
    std::string currentFlag;

    for (const auto& part : parts) {
        if (part.starts_with('-')) { // Check if the part is a flag
            currentFlag = part;
            parameters[currentFlag]; // Initialize the vector for this flag
        } else {
            if (!currentFlag.empty()) {
                parameters[currentFlag].push_back(part); // Add value to the appropriate flag
            }
        }
    }

    return parameters;
}

// Helper function to validate the tournament mode parameters
bool validateTournamentParameters(const std::map<std::string, std::vector<std::string>>& parameters) {
    // Validate Map Files (-M)
    auto mapFilesIt = parameters.find("-M");
    if (mapFilesIt == parameters.end() || mapFilesIt->second.empty() ||
        mapFilesIt->second.size() < 1 || mapFilesIt->second.size() > 5) {
        std::cout << "Invalid number of map files." << std::endl;
        return false;
    }
    // Additional checks for each map file's existence and format can be added here

    // Validate Player Strategies (-P)
    auto playerStrategiesIt = parameters.find("-P");
    if (playerStrategiesIt == parameters.end() || playerStrategiesIt->second.empty() ||
        playerStrategiesIt->second.size() < 2 || playerStrategiesIt->second.size() > 4) {
        std::cout << "Invalid number of player strategies." << std::endl;
        return false;
    }
    // Additional checks for strategy validity can be added here

    // Validate Number of Games (-G)
    auto numberOfGamesIt = parameters.find("-G");
    if (numberOfGamesIt == parameters.end() || numberOfGamesIt->second.empty()) {
        std::cout << "Number of games parameter missing." << std::endl;
        return false;
    }
    int numberOfGames;
    try {
        numberOfGames = std::stoi(numberOfGamesIt->second.front());
    } catch (const std::exception& e) {
        std::cout << "Invalid number of games format." << std::endl;
        return false;
    }
    if (numberOfGames < 1 || numberOfGames > 5) {
        std::cout << "Number of games out of range." << std::endl;
        return false;
    }

    // Validate Max Number of Turns (-D)
    auto maxNumberOfTurnsIt = parameters.find("-D");
    if (maxNumberOfTurnsIt == parameters.end() || maxNumberOfTurnsIt->second.empty()) {
        std::cout << "Max number of turns parameter missing." << std::endl;
        return false;
    }
    int maxNumberOfTurns;
    try {
        maxNumberOfTurns = std::stoi(maxNumberOfTurnsIt->second.front());
    } catch (const std::exception& e) {
        std::cout << "Invalid max number of turns format." << std::endl;
        return false;
    }
    if (maxNumberOfTurns < 10 || maxNumberOfTurns > 50) {
        std::cout << "Max number of turns out of range." << std::endl;
        return false;
    }

    return true; // All parameters are valid
}


// Command class methods
Command::Command(std::string command) : command(new std::string(std::move(command))), effect(new std::string()) {}

Command::Command(const Command &other) : command(new std::string(*other.command)),
                                         effect(new std::string(*other.effect)) {}

Command &Command::operator=(const Command &other) {
    if (this != &other) {
        delete command;
        delete effect;
        command = new std::string(*other.command);
        effect = new std::string(*other.effect);
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Command &command) {
    os << "Command: " << *command.command << ", Effect: " << *command.effect;
    return os;
}

Command::~Command() {
    delete command;
    delete effect;
}

void Command::saveEffect(std::string effect) {
    *this->effect = std::move(effect);
}

std::string Command::getCommand() const {
    return *command;
}

std::string Command::getEffect() const {
    return *effect;
}

// CommandProcessor class methods
CommandProcessor::CommandProcessor(std::string initialGameState) : commands(new std::vector<Command *>()), gameState(
        new std::string(std::move(initialGameState))) {}

CommandProcessor::CommandProcessor(const CommandProcessor &other) : commands(
        new std::vector<Command *>(*other.commands)), gameState(new std::string(*other.gameState)) {}

CommandProcessor &CommandProcessor::operator=(const CommandProcessor &other) {
    if (this != &other) {
        delete commands;
        delete gameState;
        commands = new std::vector<Command *>(*other.commands);
        gameState = new std::string(*other.gameState);
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const CommandProcessor &cp) {
    os << "Game State: " << *cp.gameState;
    return os;
}

CommandProcessor::~CommandProcessor() {
    for (Command *command: *commands) {
        delete command;
    }
    delete commands;
    delete gameState;
}

void CommandProcessor::readCommand() {
    std::string command;
    std::cout << "Enter command: ";
    std::getline(std::cin, command);
    saveCommand(command);
}

void CommandProcessor::saveCommand(const std::string &command) {
    auto *newCommand = new Command(command);
    commands->push_back(newCommand);
}


bool CommandProcessor::validate(const std::string &command) {
    auto parts = splitString(command);
    if (parts.empty()) return false;

    const auto &commandsSet = validCommands[*gameState];
    // Check if the command without the argument is in the set of valid commands
    return commandsSet.find(parts[0]) != commandsSet.end();
}

void CommandProcessor::executeCommand(const std::string &command) {
    auto parts = splitString(command);
    if (parts.empty()) return;

    if (validate(parts[0])) {
        if (parts[0] == "tournament") {
            auto tournamentParameters = parseTournamentCommand(parts);
            // Check if all required flags are present
            if (tournamentParameters.find("-M") != tournamentParameters.end() &&
                tournamentParameters.find("-P") != tournamentParameters.end() &&
                tournamentParameters.find("-G") != tournamentParameters.end() &&
                tournamentParameters.find("-D") != tournamentParameters.end()) {

                // Validate the parsed parameters
                if (validateTournamentParameters(tournamentParameters)) {
                    *gameState = nextState[parts[0]];
                    // Execute tournament logic here
                } else {
                    std::cout << "Tournament parameter parsing failed" << std::endl;
                }
            } else {
                std::cout << "Missing required tournament parameters" << std::endl;
            }
        } else if (parts[0] == "loadmap" && parts.size() > 1) {
            // Handle the loadmap command with filename
            *gameState = nextState[parts[0]];
            // Additional logic for loading map
        } else {
            // Handle other commands
            *gameState = nextState[parts[0]];
            // Additional logic for other commands
        }
    } else {
        std::cout << "Invalid command." << std::endl;
        // Handle invalid command
    }

    notifyObservers(this);
}


// Now when getting the command, you will need to check if the command is "loadmap" and handle it specially
Command CommandProcessor::getCommand() {
    readCommand();
    Command *cmd = commands->back();
    std::string commandStr = cmd->getCommand();

    auto parts = splitString(commandStr);
    if (!parts.empty() && validate(parts[0])) {
        executeCommand(commandStr);
        cmd->saveEffect("Valid command executed. New state: " + *gameState);
    } else {
        cmd->saveEffect("Invalid command. Current state: " + *gameState);
    }

    return *cmd;
}

std::string CommandProcessor::stringToLog() {
    auto lastCommand = *commands->back();

    std::stringstream s;
    s << "Command : " << lastCommand.getCommand() << std::endl;

    return s.str();
}

// FileCommandProcessorAdapter class methods
FileCommandProcessorAdapter::FileCommandProcessorAdapter(std::string initialGameState, const std::string &filename)
        : CommandProcessor(std::move(initialGameState)), file(new std::ifstream(filename)) {}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter &other)
        : CommandProcessor(other), file(new std::ifstream(*other.filename)) {}

FileCommandProcessorAdapter &FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter &other) {
    if (this != &other) {
        CommandProcessor::operator=(other);
        delete file;
        file = new std::ifstream(*other.filename);
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const FileCommandProcessorAdapter &fcpa) {
    os << static_cast<const CommandProcessor &>(fcpa) << ", File: " << fcpa.file;
    return os;
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete file;
    delete filename;
}

// You may need to add a method to check for the end of file in your FileCommandProcessorAdapter class
bool FileCommandProcessorAdapter::isEndOfFile() {
    return file->eof();
}

void FileCommandProcessorAdapter::readCommand() {
    if (!file->is_open()) {
        std::cerr << "Error: Unable to open file." << std::endl;
        return;
    }

    std::string command;
    if (std::getline(*file, command)) {
        saveCommand(command);
    } else {
        std::cerr << "Error: Unable to read command from file or end of file reached." << std::endl;
    }
}
