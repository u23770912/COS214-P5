#include <iostream>
#include <vector>
#include <map>
#include "WoodenPotFactory.h"
#include "MetalPotFactory.h"
#include "PlasticPotFactory.h"
#include "ClayPotFactory.h"
#include "GlassPotFactory.h"
#include "PotFactory.h"

using namespace std;

// int main() {
//     WoodenPotFactory woodenPotFactory;
//     MetalPotFactory metalPotFactory;
//     PlasticPotFactory plasticPotFactory;
//     ClayPotFactory clayPotFactory;
//     GlassPotFactory glassPotFactory;

//     Pot* pot1 = metalPotFactory.createPot("Large", "Round", true);
//     Pot* pot2 = woodenPotFactory.createPot("Medium", "Square", false);
//     Pot* pot3 = plasticPotFactory.createPot("Small", "Round", true);
//     Pot* pot4 = clayPotFactory.createPot("Tiny", "Hexagonal", false);
//     Pot* pot5 = glassPotFactory.createPot("Tiny", "Hexagonal", false);

//     std::cout << "=============== 🌿 Pots in inventory 🪴 =============== " << std::endl;

//     pot1->print();
//     pot2->print();
//     pot3->print();
//     pot4->print();
//     pot5->print();

//     delete pot1;
//     delete pot2;
//     delete pot3;
//     delete pot4;
//     delete pot5;

//     return 0;
// }

struct PotSpec
{
    string material;
    string size;
    string shape;
    bool hasDrainage;

    PotSpec(string mat, string sz, string sh, bool drain)
        : material(mat), size(sz), shape(sh), hasDrainage(drain) {}
};

vector<Pot *> createPotOrder(const vector<PotSpec> &specifications)
{
    vector<Pot *> order;

    map<string, PotFactory *> factories;
    factories["Wooden"] = new WoodenPotFactory();
    factories["Metal"] = new MetalPotFactory();
    factories["Plastic"] = new PlasticPotFactory();
    factories["Clay"] = new ClayPotFactory();
    factories["Glass"] = new GlassPotFactory();

    for (const auto &spec : specifications)
    {
        if (factories.find(spec.material) != factories.end())
        {
            order.push_back(factories[spec.material]->createPot(
                spec.size, spec.shape, spec.hasDrainage));
        }
    }

    for (auto &pair : factories)
    {
        delete pair.second;
    }

    return order;
}

int main()
{
    vector<PotSpec> orderSpecs = {
        PotSpec("Metal", "Large", "Round", true),
        PotSpec("Wooden", "Medium", "Square", false),
        PotSpec("Plastic", "Small", "Round", true),
        PotSpec("Clay", "Tiny", "Hexagonal", false),
        PotSpec("Glass", "Tiny", "Hexagonal", false)};

    vector<Pot *> myOrder = createPotOrder(orderSpecs);

    cout << "=============== 🌿 Pots in my order 🪴 =============== " << endl;

    for (Pot *pot : myOrder)
    {
        pot->print();
    }

    for (Pot *pot : myOrder)
    {
        delete pot;
    }

    return 0;
}
