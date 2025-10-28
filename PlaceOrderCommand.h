#ifndef PLACEORDERCOMMAND_H
#define PLACEORDERCOMMAND_H

#include <string>
#include "Command.h"
#include "Order.h"
#include "Customer.h"

class StaffMember; // Forward declaration
class InventoryManager; // Forward declaration
class SinglePlant; // Forward declaration
class PlantBundle; // Forward declaration

/**
 * @brief Command class for placing orders (Command pattern)
 * Encapsulates the action of placing an order
 */

class PlaceOrderCommand : public Command {
    private:
        Order* order;
        Customer* customer;
        std::string timestamp;
        bool executed;
        StaffMember* salesStaff;

    public:
        PlaceOrderCommand(Order* order, Customer* customer);
        ~PlaceOrderCommand();
        
        // Override Command methods
        void setReceiver(PlantProduct* plant) override;
        void execute() override;
        
        // Override clone method for Prototype pattern
        Command* clone() const override {
            return new PlaceOrderCommand(*this);
        }
        
        // Command type
        std::string getType() const override {
            return "PlaceOrder";
        }
        
        // Required role to execute this command
        std::string getRequiredRole() const override {
            return "Sales";
        }
        
        // Getters
        Order* getOrder() const;
        Customer* getCustomer() const;
        std::string getTimestamp() const;
        bool isExecuted() const;
        
        // Command information
        std::string getCommandInfo() const;

    private:
        // Helper method to generate timestamp
        std::string generateTimestamp();
        
        // Inventory validation methods
        bool validateInventoryAvailability();
        bool validateOrderItem(const OrderItem* item, const std::vector<PlantProduct*>& availablePlants);
        bool validateSinglePlant(const SinglePlant* plant, const std::vector<PlantProduct*>& availablePlants);
        bool validatePlantBundle(const PlantBundle* bundle, const std::vector<PlantProduct*>& availablePlants);
        
        // Order processing methods
        void processOrderWithStaff();
        void updateInventoryAfterSale();
        void removeSoldItemsFromInventory(const OrderItem* item, InventoryManager* inventory);
};

#endif