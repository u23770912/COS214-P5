#include <iostream>
#include <vector>
#include "Customer.h"
#include "Order.h"
#include "OrderDirector.h"
#include "ConcreteOrderBuilder.h"

/**
 * @brief Demonstration of the Builder-Director Pattern in the Customer Order System
 * 
 * This program shows how the Customer uses the Director to construct different types of orders
 * without directly interacting with the Builder. The Director encapsulates all construction logic.
 */

void printOrderSummary(Order* order) {
    if (order) {
        std::cout << "\n--- Order Summary ---" << std::endl;
        std::cout << "Order ID: " << order->getOrderId() << std::endl;
        std::cout << "Customer: " << order->getCustomerName() << std::endl;
        std::cout << "Items: " << order->getOrderItems().size() << std::endl;
        std::cout << "Status: " << order->getStatus() << std::endl;
        std::cout << "Total: $" << order->calculateTotalAmount() << std::endl;
        std::cout << "---------------------\n" << std::endl;
    } else {
        std::cout << "No order to display." << std::endl;
    }
}

int main() {
    std::cout << "======================================================" << std::endl;
    std::cout << "    BUILDER-DIRECTOR PATTERN DEMONSTRATION" << std::endl;
    std::cout << "======================================================" << std::endl;
    
    // Create a customer
    Customer* customer = new Customer("Alice Garden", "alice@email.com", "123-456-7890");
    
    std::cout << "\n1. SIMPLE CONSTRUCTION (Director Default)" << std::endl;
    std::cout << "Customer calls: construct()" << std::endl;
    std::cout << "Director builds: Garden Starter Kit (default)" << std::endl;
    Order* order1 = customer->construct();
    printOrderSummary(order1);
    
    std::cout << "\n2. SIMPLE PLANT ORDER" << std::endl;
    std::cout << "Customer calls: constructSimplePlantOrder('Rose', 3)" << std::endl;
    Order* order2 = customer->constructSimplePlantOrder("Rose", 3);
    printOrderSummary(order2);
    
    std::cout << "\n3. PLANT WITH POT ORDER" << std::endl;
    std::cout << "Customer calls: constructPlantWithPotOrder('Cactus', 'Ceramic', 2)" << std::endl;
    Order* order3 = customer->constructPlantWithPotOrder("Cactus", "Ceramic", 2);
    printOrderSummary(order3);
    
    std::cout << "\n4. CUSTOM BUNDLE ORDER" << std::endl;
    std::cout << "Customer calls: constructBundleOrder(...)" << std::endl;
    std::vector<std::string> plants;
    std::vector<int> quantities;
    plants.push_back("Lavender");
    plants.push_back("Rosemary");
    plants.push_back("Basil");
    quantities.push_back(2);
    quantities.push_back(1);
    quantities.push_back(3);
    
    Order* order4 = customer->constructBundleOrder("Herb Garden", plants, quantities, 25.0);
    printOrderSummary(order4);
    
    std::cout << "\n5. GARDEN STARTER KIT" << std::endl;
    std::cout << "Customer calls: constructGardenStarterKit()" << std::endl;
    Order* order5 = customer->constructGardenStarterKit();
    printOrderSummary(order5);
    
    std::cout << "\n6. SUCCULENT COLLECTION" << std::endl;
    std::cout << "Customer calls: constructSucculentCollection()" << std::endl;
    Order* order6 = customer->constructSucculentCollection();
    printOrderSummary(order6);
    
    std::cout << "\n======================================================" << std::endl;
    std::cout << "                  PATTERN BENEFITS" << std::endl;
    std::cout << "======================================================" << std::endl;
    std::cout << "1. CLEAN CUSTOMER CODE: Customer only calls construct() methods" << std::endl;
    std::cout << "2. ENCAPSULATED LOGIC: All building steps are in the Director" << std::endl;
    std::cout << "3. FLEXIBLE CONSTRUCTION: Multiple predefined order types" << std::endl;
    std::cout << "4. REUSABLE COMPONENTS: Same Builder, different Directors possible" << std::endl;
    std::cout << "5. MAINTAINABLE: Changes to construction logic isolated in Director" << std::endl;
    std::cout << "======================================================" << std::endl;
    
    // Cleanup
    delete customer;
    
    return 0;
}
