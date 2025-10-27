#include "OrderHistory.h"
#include "OrderMemento.h"
#include "Order.h"

#include <stdexcept>

OrderHistory::OrderHistory()
{}

OrderHistory::~OrderHistory()
{
    for (auto m : history)
    {
        delete m;
    }

    history.clear();
}

void OrderHistory::addMemento(OrderMemento* memento)
{
    if (memento == nullptr)
    {
        return;
    }
    history.push_back(memento);
}

void OrderHistory::removeMemento(std::size_t index)
{
    if (index >= history.size())
    {
        throw std::out_of_range("Invalid index for removing memento");
    }
    delete history[index];
    history.erase(history.begin() + static_cast<std::ptrdiff_t>(index));
}

OrderMemento* OrderHistory::getMemento(std::size_t index) const
{
    if (index >= history.size())
    {
        return nullptr;
    }
    return history[index];
}

void OrderHistory::saveOrder(Order* order)
{
    if (order == nullptr)
    {
        return;
    }
    OrderMemento* m = order->createMemento();
    addMemento(m);
}

void OrderHistory::undo(Order* order)
{
    if (order == nullptr) return;
    if (history.empty()) return;

    OrderMemento* m = history.back();
    order->restoreState(m);
    removeMemento(history.size() - 1);
}
