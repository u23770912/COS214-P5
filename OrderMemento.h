#ifndef ORDERMEMENTO_H
#define ORDERMEMENTO_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class OrderMemento {
private:
    std::string state;
    string getState() const;
public:
    OrderMemento(const string& savedState);
    
};

#endif // ORDERMEMENTO_H