#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <string>

class OrderBuilder;      // Forward declaration
class Order;             // Forward declaration
class PlaceOrderCommand; // Forward declaration

class Customer
{
private:
    std::string name;
    std::string cellPhone;

    OrderBuilder *orderBuilder;
    Order *orderProduct;
    PlaceOrderCommand *placeOrderCommand;

public:
    Customer(const std::string &name, const std::string &cellPhone);
    ~Customer();

    std::string getName() const;
    std::string getCellPhone() const;

    Order createOrder(); // builds order using OrderBuilder and executes PlaceOrderCommand
    void executeOrder(); // executes the PlaceOrderCommand (placeOrderCommand->execute());
};

#endif