#include <SFML/Graphics.hpp>
#include "menuPrincipal.h"

using namespace sf;

int main() {

    RenderWindow window(VideoMode::getDesktopMode(), "Buscaminas", State::Fullscreen);
    menuPrincipal menu;
    menu.ejecutar(window);

    return 0;
}
