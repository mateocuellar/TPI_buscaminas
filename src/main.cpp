#include <SFML/Graphics.hpp>
#include "menuPrincipal.h"

using namespace sf;

int main() {

    RenderWindow window(VideoMode::getDesktopMode(), "Buscaminas");
    menuPrincipal menu;
    menu.ejecutar(window);

    return 0;
}
