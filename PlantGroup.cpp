#include "PlantGroup.h"

PlantGroup::PlantGroup(std::string name): name(name){

}


PlantGroup::~PlantGroup(){

    std::vector<GreenhouseComponent*>::iterator it;

    for (it = this->children.begin(); it != this->children.end(); ++it){
        delete *it;
    }

}


std::string PlantGroup::getName(){
    return this->name;
}


void PlantGroup::setName(std::string name){
    this->name = name;
}

void PlantGroup::add(GreenhouseComponent* item){
    children.push_back(item);
}

// void PlantGroup::print(){
//     std::cout << "[" << this->getName() << ": (";

//     for (size_t i = 0; i < children.size(); ++i) {
//         children[i]->print();

//         if (i != children.size() - 1){
//             std::cout << ", ";
//         }
//     }

//     std::cout << ")]";
// }

void PlantGroup::print() {
    printRecursive("", true);  // start recursion
}

void PlantGroup::printRecursive(const std::string& prefix, bool isLast) {
    std::cout << prefix;
    if (!prefix.empty()) {
        std::cout << (isLast ? "└── " : "├── ");
    }
    std::cout << name << std::endl;

    for (size_t i = 0; i < children.size(); ++i) {
        bool lastChild = (i == children.size() - 1);
        std::string newPrefix = prefix + (isLast ? "    " : "│   ");
        
        // Try to see if the child is another group
        PlantGroup* childGroup = dynamic_cast<PlantGroup*>(children[i]);
        if (childGroup) {
            childGroup->printRecursive(newPrefix, lastChild);
        } else {
            // Just call the component’s own print method
            std::cout << newPrefix << (lastChild ? "└── " : "├── ");
            children[i]->print();
        }
    }
}

void PlantGroup::display() {
    print();
}

void PlantGroup::getPlantCount() {
    std::cout << "Group '" << name << "' contains " << children.size() << " items." << std::endl;
}

void PlantGroup::remove(GreenhouseComponent* item) {
    auto it = std::find(children.begin(), children.end(), item);
    if (it != children.end()) {
        delete *it;
        children.erase(it);
    }
}