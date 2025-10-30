#include <iostream>
#include "WoodenPotFactory.h"
#include "MetalPotFactory.h"
#include "PlasticPotFactory.h"
#include "ClayPotFactory.h"
#include "GlassPotFactory.h"

int main() {
    WoodenPotFactory woodenPotFactory;
    MetalPotFactory metalPotFactory;
    PlasticPotFactory plasticPotFactory;
    ClayPotFactory clayPotFactory;
    GlassPotFactory glassPotFactory;

    Pot* pot1 = metalPotFactory.createPot("Large", "Round", true);
    Pot* pot2 = woodenPotFactory.createPot("Medium", "Square", false);
    Pot* pot3 = plasticPotFactory.createPot("Small", "Round", true);
    Pot* pot4 = clayPotFactory.createPot("Tiny", "Hexagonal", false);
    Pot* pot5 = glassPotFactory.createPot("Tiny", "Hexagonal", false);

    
    std::cout << "=============== 🌿 Pots in inventory 🪴 =============== " << std::endl;

    pot1->print();
    pot2->print();
    pot3->print();
    pot4->print();
    pot5->print();

    delete pot1;
    delete pot2;
    delete pot3;
    delete pot4;
    delete pot5;

    return 0;
}

