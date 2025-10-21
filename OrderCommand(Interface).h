#ifndef ORDERCOMMAND_H
#define ORDERCOMMAND_H

class OrderCommand {
    public:
        virtual ~OrderCommand() {}
        virtual void execute() = 0;
};

#endif