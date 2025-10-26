#include  <iostream>
#include "Customer.h"
#include "ConcreteOrderBuilder.h"
#include "PlaceOrderCommand.h"

int main() {
    // Create a customer
    Customer customer("John Doe", "123-456-7890");
    
    // Create an order builder
    ConcreteOrderBuilder* builder = new ConcreteOrderBuilder("John Doe");
    
    // Build an order with different items
    builder->buildPlant("Rose", 2);  // 2 roses
    builder->buildPlantPot("Ceramic Pot", 1);  // 1 ceramic pot
    
    // Create a bundle
    builder->buildCustomBundle("Spring Bundle", "seasonal", 10.0);  // 10% discount
    builder->addPlantToCurrentBundle("Tulip", 3);
    builder->addPlantToCurrentBundle("Daffodil", 2);
    
    // Get the completed order
    Order* order = builder->getOrder();
    
    // Create and execute the place order command
    PlaceOrderCommand* placeCommand = new PlaceOrderCommand(order, &customer);
    placeCommand->execute();
    
    // Cleanup
    delete placeCommand;
    delete builder;
    
    return 0;
}


