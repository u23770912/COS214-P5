#include "OrderMemento.h"

OrderMemento::OrderMemento(const std::string& savedState)
    : state(savedState)
{}

std::string OrderMemento::getState() const
{
    return state;
}
