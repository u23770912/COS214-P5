#ifndef ORDERMEMENTO_H
#define ORDERMEMENTO_H

#include <string>

class OrderMemento {
private:
    std::string state;

public:
    OrderMemento(const std::string& savedState);
    std::string getState() const;
};

#endif // ORDERMEMENTO_H
