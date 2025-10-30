#ifndef ORDERMEMENTO_H
#define ORDERMEMENTO_H

#include <string>

/**
 * @brief Memento object for storing Order state snapshots
 *
 * This class holds an opaque string representation of an Order's state
 * so the caretaker can save and restore order snapshots without exposing
 * the internals of the Order class.
 */
class OrderMemento {
private:
    std::string state;

public:
    /**
     * @brief Create an OrderMemento with a serialized state
     * @param savedState Serialized representation of the order state
     */
    OrderMemento(const std::string& savedState);

    /**
     * @brief Retrieve the saved state
     * @return The serialized state string
     */
    std::string getState() const;
};

#endif // ORDERMEMENTO_H
