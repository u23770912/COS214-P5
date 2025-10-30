#include <iostream>
#include "Order.h"
#include "OrderHistory.h"

int newMain() {
    // Create history manager
    OrderHistory history;

    // Create orders
    Order order1("ORD001", "Siyabonga");
    order1.addItem("Laptop", 15000.0);
    order1.addItem("Mouse", 500.0);

    Order order2("ORD002", "Tim");
    order2.addItem("iPhone 11", 20000.0);
    order2.addItem("Cover", 200.0);

    Order order3("ORD003", "Siyabonga");
    order3.addItem("Keyboard", 1200.0);

    // Save their initial states
    history.saveOrder(&order1);
    history.saveOrder(&order2);
    history.saveOrder(&order3);

    // Display individual order
    // std::cout << "Order 1 Details:\n" << order1.getOrderDetails() << std::endl;

    // Display all orders for a specific customer
    // std::cout << order1.getOrderDetails("Siyabonga") << std::endl;
    // std::cout << order2.getOrderDetails("Tim") << std::endl;

    // Make some changes to order1
    order1.addItem("Mouse Pad", 300.0);
    order1.removeItem("Laptop");

    std::cout << "After changes to Order 1:\n" << order1.getOrderDetails() << std::endl;

    // Undo changes
    history.undo(&order1);

    std::cout << "After undo for Order 1:\n" << order1.getOrderDetails() << std::endl;

    // Display all orders for all customers
    std::cout << "All orders for all customers:\n";
    for (auto order : Order::getAllOrders()) {
        std::cout << order->getOrderDetails() << std::endl;
    }

    return 0;
}