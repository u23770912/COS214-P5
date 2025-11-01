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

void PlantGroup::print(const std::string& prefix = "", bool isLast = true) {
    // Print the current group's name (e.g., "Greenhouse", "Summer")
    std::cout << prefix << (isLast ? "└── " : "├── ") << name << std::endl; 

    // Calculate the prefix for all children
    std::string childPrefix = prefix + (isLast ? "    " : "│   "); 

    // Iterate through all children
    for (size_t i = 0; i < children.size(); ++i) {
        bool childIsLast = (i == children.size() - 1);
        
        // CRITICAL STEP: Call the child's print() method with the new prefix and last flag.
        // This ensures recursion continues down the tree.
        children[i]->print(childPrefix, childIsLast); 
    }
}

/*void PlantGroup::printRecursive(const std::string& prefix, bool isLast) {
    std::cout << prefix << (isLast ? "└── " : "├── ") << name << std::endl;

    std::string newPrefix = prefix + (isLast ? "    " : "│   ");

   
    for (size_t i = 0; i < children.size(); ++i) {
        bool childIsLast = (i == children.size() - 1);
        
        // Since PlantGroup::print() calls printRecursive, we need the logic here
        PlantGroup* childGroup = dynamic_cast<PlantGroup*>(children[i]);
        if (childGroup) {
            
            childGroup->printRecursive(newPrefix, childIsLast);
        } else {
            children[i]->print(newPrefix, childIsLast); // This will call the PlantPot::print()
        }
    }
}*/

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