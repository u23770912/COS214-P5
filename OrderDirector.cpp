#include "OrderDirector.h"
#include "OrderBuilder.h"
#include "ConcreteOrderBuilder.h"
#include "Order.h"
#include <iostream>

OrderDirector::OrderDirector() : builder(0) {
}

OrderDirector::OrderDirector(OrderBuilder* builder) : builder(builder) {
}

OrderDirector::~OrderDirector() {
    // Note: We don't delete the builder as it's managed by the Customer
}

void OrderDirector::setBuilder(OrderBuilder* builder) {
    this->builder = builder;
}

OrderBuilder* OrderDirector::getBuilder() const {
    return builder;
}

Order* OrderDirector::construct() {
    // Default construction - create a garden starter kit
    return constructGardenStarterKit();
}

Order* OrderDirector::constructSimplePlantOrder(const std::string& plantType, int quantity) {
    if (!validateBuilder()) {
        return 0;
    }
    
    resetBuilder();
    
    // Step 1: Build the main plant
    builder->buildPlant(plantType, quantity);
    
    // Step 2: Get the final order
    return builder->getOrder();
}

Order* OrderDirector::constructPlantWithPotOrder(const std::string& plantType, 
                                                const std::string& potType, int quantity) {
    if (!validateBuilder()) {
        return 0;
    }
    
    resetBuilder();
    
    // Step 1: Build the plant
    builder->buildPlant(plantType, quantity);
    
    // Step 2: Add matching pots
    builder->buildPlantPot(potType, quantity);
    
    // Step 3: Get the final order
    return builder->getOrder();
}

Order* OrderDirector::constructBundleOrder(const std::string& bundleName,
                                          const std::vector<std::string>& plantTypes,
                                          const std::vector<int>& quantities,
                                          double discount) {
    if (!validateBuilder()) {
        return 0;
    }
    
    if (plantTypes.size() != quantities.size()) {
        std::cout << "Error: Plant types and quantities must have same size" << std::endl;
        return 0;
    }
    
    resetBuilder();
    
    // Cast to ConcreteOrderBuilder to access extended functionality
    ConcreteOrderBuilder* concreteBuilder = dynamic_cast<ConcreteOrderBuilder*>(builder);
    if (concreteBuilder) {
        // Step 1: Create the bundle
        concreteBuilder->buildCustomBundle(bundleName, "Custom Bundle", discount);
        
        // Step 2: Add plants to the bundle
        for (size_t i = 0; i < plantTypes.size(); ++i) {
            concreteBuilder->addPlantToCurrentBundle(plantTypes[i], quantities[i]);
        }
    } else {
        // Fallback for basic builder
        builder->buildBundle(bundleName, 1);
    }
    
    // Step 3: Get the final order
    return builder->getOrder();
}

Order* OrderDirector::constructGardenStarterKit() {
    if (!validateBuilder()) {
        return 0;
    }
    
    resetBuilder();
    
    // Cast to ConcreteOrderBuilder to access extended functionality
    ConcreteOrderBuilder* concreteBuilder = dynamic_cast<ConcreteOrderBuilder*>(builder);
    if (concreteBuilder) {
        // Step 1: Create a starter bundle with discount
        concreteBuilder->buildCustomBundle("Garden Starter Kit", "Beginner Package", 15.0);
        
        // Step 2: Add essential plants for beginners
        concreteBuilder->addPlantToCurrentBundle("Aloe Vera", 1, "small");
        concreteBuilder->addPlantToCurrentBundle("Snake Plant", 1, "medium");
        concreteBuilder->addPlantToCurrentBundle("Pothos", 1, "small");
        
        // Step 3: Add pots for the plants
        concreteBuilder->buildPlantPot("Ceramic", 2);
        concreteBuilder->buildPlantPot("Terracotta", 1);
        
    } else {
        // Fallback for basic builder
        builder->buildBundle("Garden Starter Kit", 1);
        builder->buildPlantPot("Basic", 3);
    }
    
    return builder->getOrder();
}

Order* OrderDirector::constructSucculentCollection() {
    if (!validateBuilder()) {
        return 0;
    }
    
    resetBuilder();
    
    // Cast to ConcreteOrderBuilder to access extended functionality
    ConcreteOrderBuilder* concreteBuilder = dynamic_cast<ConcreteOrderBuilder*>(builder);
    if (concreteBuilder) {
        // Step 1: Create succulent collection bundle
        concreteBuilder->buildCustomBundle("Succulent Paradise", "Succulent Collection", 20.0);
        
        // Step 2: Add various succulents
        concreteBuilder->addPlantToCurrentBundle("Aloe Vera", 2, "medium");
        concreteBuilder->addPlantToCurrentBundle("Jade Plant", 1, "large");
        concreteBuilder->addPlantToCurrentBundle("Echeveria", 3, "small");
        concreteBuilder->addPlantToCurrentBundle("Barrel Cactus", 1, "medium");
        
        // Step 3: Add specialized pots
        concreteBuilder->buildPlantPot("Succulent", 4);
        concreteBuilder->buildPlantPot("Drainage", 3);
        
    } else {
        // Fallback for basic builder
        builder->buildBundle("Succulent Collection", 1);
        builder->buildPlantPot("Succulent", 5);
    }
    
    return builder->getOrder();
}

bool OrderDirector::validateBuilder() const {
    if (!builder) {
        std::cout << "Error: No builder set. Please set a builder before construction." << std::endl;
        return false;
    }
    return true;
}

void OrderDirector::resetBuilder() {
    if (builder) {
        builder->reset();
    }
}
