#include "OrderUIFacade.h"
#include "Customer.h"
#include "Order.h"
#include "PlantProduct.h"
#include "ConcreteOrderBuilder.h"
#include "InventoryManager.h"
#include "PlantSpeciesProfile.h"
#include <iostream>
#include <iomanip>

OrderUIFacade::OrderUIFacade(Customer* customer) : customer(customer) {
    // Facade initialized with customer reference
}

OrderUIFacade::~OrderUIFacade() {
    // Note: We don't delete the customer as it's managed externally
}

void OrderUIFacade::setCustomer(Customer* customer) {
    this->customer = customer;
}

Customer* OrderUIFacade::getCustomer() const {
    return customer;
}

double OrderUIFacade::calculateAutomaticDiscount(int totalPlants) const {
    // Automatic discount logic based on order size
    if (totalPlants >= 10) {
        return 30.0; // Maximum discount for 10+ plants
    } else if (totalPlants >= 6) {
        return 15.0; // Mid-tier discount for 6-9 plants
    } else if (totalPlants >= 3) {
        return 10.0; // Basic discount for 3-5 plants
    }
    return 0.0; // No discount for orders under 3 plants
}

int OrderUIFacade::countTotalPlantsInOrder(ConcreteOrderBuilder* builder) const {
    if (!builder || !builder->hasCurrentOrder()) {
        return 0;
    }
    
    // Get temporary order to count plants
    Order* tempOrder = builder->getOrder();
    if (!tempOrder) {
        return 0;
    }
    
    int totalPlants = 0;
    std::vector<class OrderItem*> items = tempOrder->getOrderItems();
    
    // Count total plants across all order items
    // Note: This is a simplified count - in a real system you'd have more complex logic
    totalPlants = static_cast<int>(items.size());
    
    return totalPlants;
}

void OrderUIFacade::displayAvailableItems() {
    std::cout << "\n=== AVAILABLE PLANTS IN NURSERY ===" << std::endl;
    
    std::vector<PlantProduct*> availablePlants = getAvailablePlantsFromInventory();
    
    if (availablePlants.empty()) {
        std::cout << "[ERROR] Sorry! No plants are currently available for sale." << std::endl;
        std::cout << "Please check back later or contact staff." << std::endl;
        return;
    }
    
    std::cout << "We have " << availablePlants.size() << " plants ready for sale:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    for (size_t i = 0; i < availablePlants.size(); ++i) {
        displayPlantDetails(availablePlants[i], static_cast<int>(i + 1));
    }
    
    std::cout << std::string(60, '-') << std::endl;
    displayDiscountInformation();
    std::cout << "[INFO] To add a plant: Use addPlantToOrder(number, quantity)" << std::endl;
}

void OrderUIFacade::viewCurrentOrder() {
    if (!customer) {
        std::cout << "[ERROR] No customer associated with UI facade." << std::endl;
        return;
    }
    
    ConcreteOrderBuilder* builder = customer->getOrderBuilder();
    if (!builder || !builder->hasCurrentOrder()) {
        std::cout << "\n[ORDER] Your order is currently empty." << std::endl;
        std::cout << "[INFO] Use displayAvailableItems() to see what's available!" << std::endl;
        displayDiscountInformation();
        return;
    }
    
    std::cout << "\n=== YOUR CURRENT ORDER ===" << std::endl;
    
    // Get temporary order to display
    Order* tempOrder = builder->getOrder();
    if (tempOrder && !tempOrder->isEmpty()) {
        std::cout << tempOrder->getOrderSummary() << std::endl;
        
        // Show current plant count and applicable discount
        int plantCount = countTotalPlantsInOrder(builder);
        double discount = calculateAutomaticDiscount(plantCount);
        
        std::cout << "\n[DISCOUNT INFO] Total plants: " << plantCount;
        if (discount > 0.0) {
            std::cout << " | Automatic discount: " << discount << "%" << std::endl;
        } else {
            std::cout << " | No discount applied" << std::endl;
        }
        
        std::cout << "[TOTAL] Current Total: $" << std::fixed << std::setprecision(2) 
                  << tempOrder->getTotalAmount() << std::endl;
    } else {
        std::cout << "[ORDER] Order is empty." << std::endl;
    }
}

bool OrderUIFacade::addPlantToOrderWithAutoDiscount(int plantIndex, int quantity) {
    if (!customer) {
        std::cout << "[ERROR] No customer associated with UI facade." << std::endl;
        return false;
    }
    
    std::vector<PlantProduct*> availablePlants = getAvailablePlantsFromInventory();
    
    // Validate input
    if (plantIndex < 1 || plantIndex > static_cast<int>(availablePlants.size())) {
        std::cout << "[ERROR] Invalid plant number. Please choose between 1 and " 
                  << availablePlants.size() << std::endl;
        return false;
    }
    
    if (quantity <= 0) {
        std::cout << "[ERROR] Quantity must be greater than 0." << std::endl;
        return false;
    }
    
    PlantProduct* selectedPlant = availablePlants[plantIndex - 1];
    std::string plantType = selectedPlant->getProfile()->getSpeciesName();
    
    std::cout << "\n[SUCCESS] Adding " << quantity << "x " << plantType 
              << " to your order..." << std::endl;
    
    // Notify customer observers about the interaction
    customer->notifyInteraction("PlantSelection", 
        "Customer selected " + std::to_string(quantity) + "x " + plantType);
    
    // Use customer's builder to add the plant
    ConcreteOrderBuilder* builder = customer->getOrderBuilder();
    if (builder) {
        builder->buildPlant(plantType, quantity);
        
        // Calculate and show potential discount after addition
        int newPlantCount = countTotalPlantsInOrder(builder);
        double discount = calculateAutomaticDiscount(newPlantCount);
        
        std::cout << "[COMPLETE] Plant added successfully!" << std::endl;
        std::cout << "[DISCOUNT] Total plants now: " << newPlantCount;
        if (discount > 0.0) {
            std::cout << " | Automatic discount: " << discount << "%" << std::endl;
        } else {
            std::cout << " | Add more plants for discounts!" << std::endl;
        }
        
        return true;
    }
    
    std::cout << "[ERROR] Unable to access order builder." << std::endl;
    return false;
}

bool OrderUIFacade::addBundleToOrderWithAutoDiscount(const std::string& bundleName, 
                                                     const std::vector<int>& plantIndices) {
    if (!customer) {
        std::cout << "[ERROR] No customer associated with UI facade." << std::endl;
        return false;
    }
    
    std::vector<PlantProduct*> availablePlants = getAvailablePlantsFromInventory();
    
    if (plantIndices.empty()) {
        std::cout << "[ERROR] Bundle must contain at least one plant." << std::endl;
        return false;
    }
    
    // Validate all indices
    for (size_t i = 0; i < plantIndices.size(); ++i) {
        int index = plantIndices[i];
        if (index < 1 || index > static_cast<int>(availablePlants.size())) {
            std::cout << "[ERROR] Invalid plant number " << index << ". Please choose between 1 and " 
                      << availablePlants.size() << std::endl;
            return false;
        }
    }
    
    // Calculate automatic discount based on bundle size
    double autoDiscount = calculateAutomaticDiscount(static_cast<int>(plantIndices.size()));
    
    std::cout << "\n[BUNDLE] Creating bundle: " << bundleName << " with " 
              << static_cast<int>(plantIndices.size()) << " plants" << std::endl;
    std::cout << "[AUTO-DISCOUNT] Applying " << autoDiscount << "% discount" << std::endl;
    
    // Notify customer observers about bundle creation
    customer->notifyInteraction("BundleCreation", 
        "Customer creating " + bundleName + " with " + std::to_string(plantIndices.size()) + " plants");
    
    // Use customer's builder to create bundle with automatic discount
    ConcreteOrderBuilder* builder = customer->getOrderBuilder();
    if (builder) {
        builder->buildCustomBundle(bundleName, "Mixed", autoDiscount);
        
        // Add each plant to the bundle
        for (size_t i = 0; i < plantIndices.size(); ++i) {
            int index = plantIndices[i];
            PlantProduct* plant = availablePlants[index - 1];
            std::string plantType = plant->getProfile()->getSpeciesName();
            builder->addPlantToCurrentBundle(plantType, 1);
            std::cout << "  [OK] Added " << plantType << " to bundle" << std::endl;
        }
        
        std::cout << "[BUNDLE] Bundle created successfully with automatic discount!" << std::endl;
        return true;
    }
    
    std::cout << "[ERROR] Unable to access order builder." << std::endl;
    return false;
}

void OrderUIFacade::displayOrderCreationWelcome() {
    if (!customer) {
        std::cout << "[ERROR] No customer associated with UI facade." << std::endl;
        return;
    }
    
    std::cout << "\n=== Welcome " << customer->getName() << "! Let's create your order ===" << std::endl;
    std::cout << "New order session started!" << std::endl;
    displayOrderInstructions();
}

void OrderUIFacade::displayOrderExecutionSummary() {
    if (!customer) {
        std::cout << "[ERROR] No customer associated with UI facade." << std::endl;
        return;
    }
    
    std::cout << "\n=== EXECUTING ORDER ===" << std::endl;
    std::cout << "Customer: " << customer->getName() << " (" << customer->getEmail() << ")" << std::endl;
}

void OrderUIFacade::displayFinalOrderConfirmation() {
    std::cout << "\n=== ORDER FINALIZED ===" << std::endl;
    std::cout << "[SUCCESS] Order is ready for execution!" << std::endl;
    std::cout << "[INFO] Use executeOrder() to place your order with staff." << std::endl;
}

void OrderUIFacade::displayDiscountInformation() const {
    std::cout << "\n[AUTOMATIC DISCOUNTS AVAILABLE]" << std::endl;
    std::cout << "  3-5 plants:  10% discount automatically applied" << std::endl;
    std::cout << "  6-9 plants:  15% discount automatically applied" << std::endl;
    std::cout << "  10+ plants:  30% discount automatically applied (MAX)" << std::endl;
}

void OrderUIFacade::displayOrderInstructions() const {
    std::cout << "Use these interactive methods to build your order:" << std::endl;
    std::cout << "1. displayAvailableItems() - See what's available" << std::endl;
    std::cout << "2. addPlantToOrder(index, quantity) - Add plants with auto-discount" << std::endl;
    std::cout << "3. addBundleToOrder(name, indices) - Create bundles with auto-discount" << std::endl;
    std::cout << "4. viewCurrentOrder() - Review your order" << std::endl;
    std::cout << "5. finalizeOrder() - Complete the order" << std::endl;
}

// Private helper methods moved from Customer class

std::vector<PlantProduct*> OrderUIFacade::getAvailablePlantsFromInventory() {
    InventoryManager& inventory = InventoryManager::getInstance();
    return inventory.getReadyForSalePlants();
}

void OrderUIFacade::displayPlantDetails(const PlantProduct* plant, int index) {
    std::cout << std::setw(3) << index << ". ";
    
    if (plant && plant->getProfile()) {
        std::cout << std::setw(15) << plant->getProfile()->getSpeciesName();
        std::cout << " | State: " << std::setw(12) << plant->getCurrentStateName();
        
        // Display plant care requirements if available
        std::string waterNeeds = plant->getProfile()->getProperty("idealWater");
        std::string sunNeeds = plant->getProfile()->getProperty("idealSunlight");
        
        if (!waterNeeds.empty()) {
            std::cout << " | Water: " << waterNeeds;
        }
        if (!sunNeeds.empty()) {
            std::cout << " | Sun: " << sunNeeds;
        }
        
        std::cout << " | $15.99"; // Default price - could be enhanced
    } else {
        std::cout << "Unknown Plant";
    }
    std::cout << std::endl;
}
