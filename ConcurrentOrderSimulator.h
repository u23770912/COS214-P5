#ifndef CONCURRENT_ORDER_SIMULATOR_H
#define CONCURRENT_ORDER_SIMULATOR_H

#include <vector>
#include <pthread.h>
#include "Customer.h"
#include "StaffManager.h"
#include "Order.h"

// Forward declarations
class ConcurrentOrderSimulator;
class ThreadSafeStaffManager;

/**
 * @struct OrderThreadData
 * @brief Data structure passed to each order processing thread
 * Contains all necessary information for a thread to process a customer order
 */
struct OrderThreadData {
    int customerId;                    // Unique identifier for the customer
    std::string customerName;          // Customer's name
    std::string customerEmail;         // Customer's email
    std::vector<int> plantIndices;     // Plant indices to order
    std::vector<int> quantities;       // Quantities for each plant
    ConcurrentOrderSimulator* simulator; // Pointer to simulator for shared resources
    pthread_t threadId;                // Thread identifier
    bool orderCompleted;               // Flag indicating order completion status
    double totalAmount;                // Final order total
    std::string orderId;               // Generated order ID
    
    // Constructor for easy initialization
    OrderThreadData(int id, const std::string& name, const std::string& email,
                   const std::vector<int>& plants, const std::vector<int>& qtys,
                   ConcurrentOrderSimulator* sim)
        : customerId(id), customerName(name), customerEmail(email),
          plantIndices(plants), quantities(qtys), simulator(sim),
          threadId(0), orderCompleted(false), totalAmount(0.0) {}
};

/**
 * @class ThreadSafeStaffManager
 * @brief Thread-safe wrapper for staff management operations
 * Ensures proper synchronization when multiple threads access staff resources
 */
class ThreadSafeStaffManager {
private:
    StaffManager* staffManager;     // Underlying staff manager
    pthread_mutex_t staffMutex;     // Mutex for thread safety
    pthread_mutex_t logMutex;       // Mutex for logging operations
    int availableStaff;             // Number of available staff members
    int totalStaff;                 // Total number of staff members
    
public:
    ThreadSafeStaffManager(StaffManager* manager, int staffCount);
    ~ThreadSafeStaffManager();
    
    // Thread-safe operations
    bool requestStaff(int customerId, const std::string& customerName);
    void releaseStaff(int customerId, const std::string& customerName);
    bool processOrder(Order* order, Customer* customer);
    
    // Status queries (thread-safe)
    int getAvailableStaffCount();
    int getTotalStaffCount();
    
    // Thread-safe logging
    void logMessage(const std::string& message);
};

/**
 * @class ConcurrentOrderSimulator
 * @brief Main simulation class that orchestrates concurrent order processing
 * Manages multiple customer threads and demonstrates Chain of Responsibility under concurrency
 */
class ConcurrentOrderSimulator {
private:
    ThreadSafeStaffManager* threadSafeStaffManager;
    std::vector<OrderThreadData*> orderThreads;
    pthread_mutex_t simulationMutex;    // Mutex for simulation-wide operations
    pthread_mutex_t inventoryMutex;     // Mutex for inventory access
    pthread_cond_t allOrdersComplete;   // Condition variable for completion
    
    int completedOrders;                // Counter for completed orders
    int totalOrders;                    // Total number of orders to process
    bool simulationActive;              // Flag indicating if simulation is running
    
    // Private helper methods
    void initializeInventory();
    void setupStaff();
    void printSimulationHeader();
    void printSimulationResults();
    
public:
    ConcurrentOrderSimulator(int staffCount = 3);
    ~ConcurrentOrderSimulator();
    
    // Main simulation methods
    void addCustomerOrder(const std::string& customerName,
                         const std::string& customerEmail,
                         const std::vector<int>& plantIndices,
                         const std::vector<int>& quantities);
    
    void startSimulation();
    void waitForCompletion();
    void printResults();
    
    // Thread-safe accessors for order threads
    ThreadSafeStaffManager* getThreadSafeStaffManager() { return threadSafeStaffManager; }
    
    // Thread-safe inventory operations
    bool checkInventoryAvailability(const std::vector<int>& plantIndices,
                                  const std::vector<int>& quantities);
    void updateInventoryAfterOrder(const std::vector<int>& plantIndices,
                                 const std::vector<int>& quantities);
    
    // Thread-safe completion notification
    void notifyOrderCompletion(int customerId);
    
    // Static thread entry point
    static void* processOrderThread(void* threadData);
};

#endif // CONCURRENT_ORDER_SIMULATOR_H