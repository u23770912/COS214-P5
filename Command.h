#ifndef COMMAND_H
#define COMMAND_H

/**
 * @brief Abstract Command class (Command pattern)
 * Defines the interface for executing and undoing commands
 */

class Command {
public:
    virtual ~Command() = default;

    // Command interface
    virtual void execute() = 0;
    virtual void undo();
};

#endif