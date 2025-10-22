#include "OrderHistory.h"

OrderHistory::OrderHistory()
{}

OrderHistory::~OrderHistory()
{}
void OrderHistory::addMemento(OrderMemento* memento)
{}

OrderMemento* OrderHistory::getMemento(int index) const
{}

void OrderHistory::removeMemento(int id)
{}

void OrderHistory::saveOrder(Order* order)
{}

void OrderHistory::undo(Order* order)
{}