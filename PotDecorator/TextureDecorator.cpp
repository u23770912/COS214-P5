#include "TextureDecorator.h"
#include <iostream>

TextureDecorator::TextureDecorator(Pot* pot, const std::string& texture) 
    : PotDecorator(pot, getTexturePrice(texture)), 
      texture(texture) {
}

void TextureDecorator::print() {
    wrappedPot->print();
    std::cout << " + " << texture << " Texture (R" << decorationPrice << ")";
}

double TextureDecorator::getTexturePrice(const std::string& texture) const {
    if (texture == "Embossed" || texture == "Woven") return 6.0;
    if (texture == "Hammered" || texture == "Ribbed") return 4.0;
    return 3.0;
}