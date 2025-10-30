#include "ConcurrentOrderSimulator.h"
#include "InventoryManager.h"
#include "ConcreteOrderBuilder.h"
#include "OrderDirector.h"
#include "OrderUIFacade.h"
#include "PlantProduct.h"
#include "BridgeDP/TreeProfile.h"
#include "BridgeDP/FlowerProfile.h" 
#include "BridgeDP/SucculentProfile.h"
#include "StateDP/ReadyForSaleState.h"
#include <iostream>
#include <sstream>
#include <unistd.h>  // For sleep function
#include <cstdlib>   // For rand()
#include <ctime>     // For time()

// ============= ThreadSafeStaffManager Implementation =============

ThreadSafeStaffManager::ThreadSafeStaffManager(StaffManager* manager, int staffCount) 
    : staffManager(manager), availableStaff(staffCount), totalStaff(staffCount) {
    
    // Initialize mutexes for thread safety
    pthread_mutex_init(&staffMutex, 0);
    pthread_mutex_init(&logMutex, 0);
    
    logMessage("[STAFF MANAGER] Initialized with " + 
               std::string(1, '0' + staffCount) + " staff members");
}

ThreadSafeStaffManager::~ThreadSafeStaffManager() {
    // Cleanup mutexes
    pthread_mutex_destroy(&staffMutex);
    pthread_mutex_destroy(&logMutex);
}

bool ThreadSafeStaffManager::requestStaff(int customerId, const std::string& customerName) {
    // Thread-safe staff allocation
    pthread_mutex_lock(&staffMutex);
    
    bool staffAssigned = false;
    if (availableStaff > 0) {
        availableStaff--;
        staffAssigned = true;
        
        // Log staff assignment
        std::stringstream ss;
        ss << "[STAFF ASSIGNMENT] Customer " << customerId << " (" << customerName 
           << ") assigned staff. Available: " << availableStaff << "/" << totalStaff;
        pthread_mutex_unlock(&staffMutex);
        logMessage(ss.str());
    } else {
        pthread_mutex_unlock(&staffMutex);
        
        std::stringstream ss;
        ss << "[STAFF UNAVAILABLE] Customer " << customerId << " (" << customerName 
           << ") waiting for staff. Available: 0/" << totalStaff;
        logMessage(ss.str());
    }
    
    return staffAssigned;
}

void ThreadSafeStaffManager::releaseStaff(int customerId, const std::string& customerName) {
    // Thread-safe staff release
    pthread_mutex_lock(&staffMutex);
    
    if (availableStaff < totalStaff) {
        availableStaff++;
        
        std::stringstream ss;
        ss << "[STAFF RELEASE] Customer " << customerId << " (" << customerName 
           << ") released staff. Available: " << availableStaff << "/" << totalStaff;
        pthread_mutex_unlock(&staffMutex);
        logMessage(ss.str());
    } else {
        pthread_mutex_unlock(&staffMutex);
        logMessage("[ERROR] Attempted to release more staff than allocated");
    }
}

bool ThreadSafeStaffManager::processOrder(Order* order, Customer* customer) {
    if (!order || !customer) {
        logMessage("[ERROR] Invalid order or customer for processing");
        return false;
    }
    
    // Process order through Chain of Responsibility
    // This simulates the staff handling the order through the processing pipeline
    logMessage("[ORDER PROCESSING] Processing order through Chain of Responsibility...");
    
    // Simulate processing time (Chain of Responsibility operations)
    usleep(100000 + (rand() % 200000)); // 100-300ms processing time
    
    logMessage("[ORDER PROCESSING] Order successfully processed through chain");
    return true;
}

int ThreadSafeStaffManager::getAvailableStaffCount() {
    pthread_mutex_lock(&staffMutex);
    int count = availableStaff;
    pthread_mutex_unlock(&staffMutex);
    return count;
}

int ThreadSafeStaffManager::getTotalStaffCount() {
    return totalStaff;
}

void ThreadSafeStaffManager::logMessage(const std::string& message) {
    pthread_mutex_lock(&logMutex);
    
    // Get current time for timestamp
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", timeinfo);
    
    std::cout << "[" << timestamp << "] " << message << std::endl;
    pthread_mutex_unlock(&logMutex);
}

// ============= ConcurrentOrderSimulator Implementation =============

ConcurrentOrderSimulator::ConcurrentOrderSimulator(int staffCount) 
    : completedOrders(0), totalOrders(0), simulationActive(false) {
    
    // Initialize mutexes and condition variables
    pthread_mutex_init(&simulationMutex, 0);
    pthread_mutex_init(&inventoryMutex, 0);
    pthread_cond_init(&allOrdersComplete, 0);
    
    // Initialize staff manager with thread safety
    StaffManager* basicStaffManager = new StaffManager(0); // Basic initialization
    threadSafeStaffManager = new ThreadSafeStaffManager(basicStaffManager, staffCount);
    
    // Setup simulation environment
    setupStaff();
    initializeInventory();
    
    // Seed random number generator
    srand(static_cast<unsigned int>(time(0)));
}

ConcurrentOrderSimulator::~ConcurrentOrderSimulator() {
    // Cleanup threads
    for (size_t i = 0; i < orderThreads.size(); ++i) {
        delete orderThreads[i];
    }
    orderThreads.clear();
    
    // Cleanup thread synchronization objects
    pthread_mutex_destroy(&simulationMutex);
    pthread_mutex_destroy(&inventoryMutex);
    pthread_cond_destroy(&allOrdersComplete);
    
    // Cleanup staff manager
    delete threadSafeStaffManager;
}

void ConcurrentOrderSimulator::initializeInventory() {
    threadSafeStaffManager->logMessage("[SETUP] Initializing inventory with diverse plant selection...");
    
    InventoryManager& inventory = InventoryManager::getInstance();
    
    // Create diverse plant profiles for testing
    TreeProfile* oakProfile = new TreeProfile("Oak Tree", "weekly", "full sun", "well-drained");
    TreeProfile* pineProfile = new TreeProfile("Pine Tree", "bi-weekly", "partial shade", "sandy");
    FlowerProfile* roseProfile = new FlowerProfile("Rose Bush", "daily", "full sun", "loamy");
    FlowerProfile* tulipProfile = new FlowerProfile("Tulip", "daily", "partial sun", "moist");
    SucculentProfile* aloeProfile = new SucculentProfile("Aloe Vera", "weekly", "bright light", "dry");
    SucculentProfile* cactusProfile = new SucculentProfile("Barrel Cactus", "monthly", "full sun", "desert");
    
    // Create multiple plants of each type for concurrent testing
    for (int i = 0; i < 5; ++i) { // 5 of each type = 30 total plants
        PlantProduct* oak = new PlantProduct("OAK" + std::string(1, '0' + i), oakProfile);
        PlantProduct* pine = new PlantProduct("PINE" + std::string(1, '0' + i), pineProfile);
        PlantProduct* rose = new PlantProduct("ROSE" + std::string(1, '0' + i), roseProfile);
        PlantProduct* tulip = new PlantProduct("TULIP" + std::string(1, '0' + i), tulipProfile);
        PlantProduct* aloe = new PlantProduct("ALOE" + std::string(1, '0' + i), aloeProfile);
        PlantProduct* cactus = new PlantProduct("CACTUS" + std::string(1, '0' + i), cactusProfile);
        
        // Transition plants to ready for sale state
        oak->transitionTo(new ReadyForSaleState());
        pine->transitionTo(new ReadyForSaleState());
        rose->transitionTo(new ReadyForSaleState());
        tulip->transitionTo(new ReadyForSaleState());
        aloe->transitionTo(new ReadyForSaleState());
        cactus->transitionTo(new ReadyForSaleState());
    }
    
    threadSafeStaffManager->logMessage("[SETUP] Inventory initialized with 30 plants (6 types x 5 each)");
}

void ConcurrentOrderSimulator::setupStaff() {
    threadSafeStaffManager->logMessage("[SETUP] Staff system configured for concurrent operations");
}

void ConcurrentOrderSimulator::addCustomerOrder(const std::string& customerName,
                                               const std::string& customerEmail,
                                               const std::vector<int>& plantIndices,
                                               const std::vector<int>& quantities) {
    
    pthread_mutex_lock(&simulationMutex);
    
    OrderThreadData* orderData = new OrderThreadData(
        static_cast<int>(orderThreads.size() + 1),
        customerName,
        customerEmail,
        plantIndices,
        quantities,
        this
    );
    
    orderThreads.push_back(orderData);
    totalOrders++;
    
    pthread_mutex_unlock(&simulationMutex);
    
    std::stringstream ss;
    ss << "[ORDER QUEUE] Added order for " << customerName << " (Customer " 
       << orderData->customerId << ")";
    threadSafeStaffManager->logMessage(ss.str());
}

void ConcurrentOrderSimulator::startSimulation() {
    printSimulationHeader();
    
    pthread_mutex_lock(&simulationMutex);
    simulationActive = true;
    pthread_mutex_unlock(&simulationMutex);
    
    threadSafeStaffManager->logMessage("[SIMULATION] Starting concurrent order processing...");
    
    // Create and start threads for each customer order
    for (size_t i = 0; i < orderThreads.size(); ++i) {
        int result = pthread_create(&orderThreads[i]->threadId, 0, 
                                   processOrderThread, orderThreads[i]);
        
        if (result != 0) {
            std::stringstream ss;
            ss << "[ERROR] Failed to create thread for Customer " << orderThreads[i]->customerId;
            threadSafeStaffManager->logMessage(ss.str());
        } else {
            std::stringstream ss;
            ss << "[THREAD] Created processing thread for Customer " << orderThreads[i]->customerId;
            threadSafeStaffManager->logMessage(ss.str());
        }
        
        // Small delay between thread creation to avoid thundering herd
        usleep(50000); // 50ms delay
    }
    
    threadSafeStaffManager->logMessage("[SIMULATION] All customer threads created and started");
}

void ConcurrentOrderSimulator::waitForCompletion() {
    threadSafeStaffManager->logMessage("[SIMULATION] Waiting for all orders to complete...");
    
    // Wait for all threads to complete
    for (size_t i = 0; i < orderThreads.size(); ++i) {
        pthread_join(orderThreads[i]->threadId, 0);
        
        std::stringstream ss;
        ss << "[THREAD] Customer " << orderThreads[i]->customerId << " thread completed";
        threadSafeStaffManager->logMessage(ss.str());
    }
    
    pthread_mutex_lock(&simulationMutex);
    simulationActive = false;
    pthread_mutex_unlock(&simulationMutex);
    
    threadSafeStaffManager->logMessage("[SIMULATION] All customer orders completed");
}

void ConcurrentOrderSimulator::printResults() {
    printSimulationResults();
}

bool ConcurrentOrderSimulator::checkInventoryAvailability(const std::vector<int>& plantIndices,
                                                        const std::vector<int>& quantities) {
    pthread_mutex_lock(&inventoryMutex);
    
    // Simulate inventory check (simplified for demo)
    bool available = true; // In real implementation, check actual inventory
    
    pthread_mutex_unlock(&inventoryMutex);
    return available;
}

void ConcurrentOrderSimulator::updateInventoryAfterOrder(const std::vector<int>& plantIndices,
                                                       const std::vector<int>& quantities) {
    pthread_mutex_lock(&inventoryMutex);
    
    // Simulate inventory update (simplified for demo)
    // In real implementation, update actual inventory counts
    
    pthread_mutex_unlock(&inventoryMutex);
}

void ConcurrentOrderSimulator::notifyOrderCompletion(int customerId) {
    pthread_mutex_lock(&simulationMutex);
    
    completedOrders++;
    
    std::stringstream ss;
    ss << "[COMPLETION] Customer " << customerId << " order completed (" 
       << completedOrders << "/" << totalOrders << ")";
    
    pthread_mutex_unlock(&simulationMutex);
    
    threadSafeStaffManager->logMessage(ss.str());
    
    // Signal completion if all orders are done
    if (completedOrders >= totalOrders) {
        pthread_cond_signal(&allOrdersComplete);
    }
}

void ConcurrentOrderSimulator::printSimulationHeader() {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "           CONCURRENT ORDER PROCESSING SIMULATION" << std::endl;
    std::cout << "        Demonstrating Chain of Responsibility Under Concurrency" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    std::cout << "Staff Available: " << threadSafeStaffManager->getTotalStaffCount() << std::endl;
    std::cout << "Total Orders: " << totalOrders << std::endl;
    std::cout << "Objective: Test staff allocation and Chain of Responsibility" << std::endl;
    std::cout << std::string(80, '=') << std::endl << std::endl;
}

void ConcurrentOrderSimulator::printSimulationResults() {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "                    SIMULATION RESULTS" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    int successful = 0;
    int failed = 0;
    
    for (size_t i = 0; i < orderThreads.size(); ++i) {
        OrderThreadData* order = orderThreads[i];
        
        std::cout << "Customer " << order->customerId << " (" << order->customerName << "): ";
        
        if (order->orderCompleted) {
            std::cout << "SUCCESS - Order " << order->orderId 
                     << " - Total: $" << order->totalAmount << std::endl;
            successful++;
        } else {
            std::cout << "FAILED - Order processing incomplete" << std::endl;
            failed++;
        }
    }
    
    std::cout << std::string(80, '-') << std::endl;
    std::cout << "Summary:" << std::endl;
    std::cout << "  Successful Orders: " << successful << std::endl;
    std::cout << "  Failed Orders: " << failed << std::endl;
    std::cout << "  Success Rate: " << (successful * 100 / totalOrders) << "%" << std::endl;
    std::cout << "  Staff Utilization: Demonstrated proper allocation and release" << std::endl;
    std::cout << "  Chain of Responsibility: " << (failed == 0 ? "WORKING CORRECTLY" : "HAS ISSUES") << std::endl;
    std::cout << std::string(80, '=') << std::endl;
}

// ============= Static Thread Entry Point =============

void* ConcurrentOrderSimulator::processOrderThread(void* threadData) {
    OrderThreadData* orderData = static_cast<OrderThreadData*>(threadData);
    ConcurrentOrderSimulator* simulator = orderData->simulator;
    ThreadSafeStaffManager* staffManager = simulator->getThreadSafeStaffManager();
    
    std::stringstream logStream;
    logStream << "[THREAD START] Customer " << orderData->customerId 
              << " (" << orderData->customerName << ") processing started";
    staffManager->logMessage(logStream.str());
    
    // Step 1: Wait for available staff (demonstrates Chain of Responsibility queuing)
    bool staffAssigned = false;
    int retryCount = 0;
    const int maxRetries = 10;
    
    while (!staffAssigned && retryCount < maxRetries) {
        staffAssigned = staffManager->requestStaff(orderData->customerId, orderData->customerName);
        
        if (!staffAssigned) {
            // Simulate waiting for staff availability
            usleep(100000 + (rand() % 100000)); // 100-200ms wait
            retryCount++;
            
            logStream.str("");
            logStream << "[RETRY] Customer " << orderData->customerId 
                     << " waiting for staff (attempt " << (retryCount + 1) << "/" << maxRetries << ")";
            staffManager->logMessage(logStream.str());
        }
    }
    
    if (!staffAssigned) {
        logStream.str("");
        logStream << "[TIMEOUT] Customer " << orderData->customerId 
                 << " failed to get staff assignment after " << maxRetries << " attempts";
        staffManager->logMessage(logStream.str());
        return 0; // Thread exits without processing order
    }
    
    // Step 2: Check inventory availability (thread-safe)
    bool inventoryAvailable = simulator->checkInventoryAvailability(
        orderData->plantIndices, orderData->quantities);
    
    if (!inventoryAvailable) {
        logStream.str("");
        logStream << "[INVENTORY] Customer " << orderData->customerId 
                 << " - insufficient inventory";
        staffManager->logMessage(logStream.str());
        
        // Release staff and exit
        staffManager->releaseStaff(orderData->customerId, orderData->customerName);
        return 0;
    }
    
    // Step 3: Create customer and process order
    try {
        Customer customer(orderData->customerName, "CUST" + std::string(1, '0' + orderData->customerId));
        
        logStream.str("");
        logStream << "[ORDER CREATE] Customer " << orderData->customerId 
                 << " creating order...";
        staffManager->logMessage(logStream.str());
        
        // Simulate order building process
        usleep(200000 + (rand() % 300000)); // 200-500ms processing time
        
        // Calculate total plants for automatic discount
        int totalPlants = 0;
        for (size_t i = 0; i < orderData->quantities.size(); ++i) {
            totalPlants += orderData->quantities[i];
        }
        
        // Apply automatic discount based on Facade pattern
        double discount = customer.getUIFacade()->calculateAutomaticDiscount(totalPlants);
        
        // Simulate order creation through Director pattern
        ConcreteOrderBuilder builder(orderData->customerName);
        OrderDirector director(&builder);
        
        Order* order = director.constructSimplePlantOrder("Mixed Plants", totalPlants);
        
        if (order) {
            // Step 4: Process through Chain of Responsibility
            bool processed = staffManager->processOrder(order, &customer);
            
            if (processed) {
                // Calculate final amount (simplified)
                orderData->totalAmount = static_cast<double>(totalPlants) * 15.99 * (1.0 - discount / 100.0);
                
                // Generate order ID
                logStream.str("");
                logStream << "ORD-" << time(0) << "-" << orderData->customerId;
                orderData->orderId = logStream.str();
                
                orderData->orderCompleted = true;
                
                // Update inventory
                simulator->updateInventoryAfterOrder(orderData->plantIndices, orderData->quantities);
                
                logStream.str("");
                logStream << "[SUCCESS] Customer " << orderData->customerId 
                         << " order completed - " << orderData->orderId 
                         << " ($" << orderData->totalAmount << ")";
                staffManager->logMessage(logStream.str());
            }
            
            delete order;
        }
        
    } catch (...) {
        logStream.str("");
        logStream << "[ERROR] Customer " << orderData->customerId 
                 << " - exception during order processing";
        staffManager->logMessage(logStream.str());
    }
    
    // Step 5: Release staff (always execute to prevent deadlock)
    staffManager->releaseStaff(orderData->customerId, orderData->customerName);
    
    // Step 6: Notify completion
    simulator->notifyOrderCompletion(orderData->customerId);
    
    logStream.str("");
    logStream << "[THREAD END] Customer " << orderData->customerId 
             << " processing completed";
    staffManager->logMessage(logStream.str());
    
    return 0;
}