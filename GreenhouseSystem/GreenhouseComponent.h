#ifndef GREENHOUSECOMPONENT_H
#define GREENHOUSECOMPONENT_H

class GreenhouseComponent {
public:
    GreenhouseComponent() = default;
    virtual ~GreenhouseComponent() = default;
    virtual void print() = 0;
    virtual void add(GreenhouseComponent* item) = 0;
};

#endif
