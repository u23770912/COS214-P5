#ifndef GREENHOUSECOMPONENT_H
#define GREENHOUSECOMPONENT_H

#include <iostream>

class GreenhouseComponent {
    public: 
        GreenhouseComponent();
        virtual ~GreenhouseComponent();
        virtual void print(const std::string& prefix = "", bool isLast = true) = 0;
        virtual void add(GreenhouseComponent* item) = 0;
        virtual void display() = 0;
        virtual void getPlantCount() = 0;
        virtual void remove(GreenhouseComponent* item) = 0;
};

#endif