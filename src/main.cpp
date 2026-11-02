#include <SFML/Graphics.hpp>
#include "menuPrincipal.h"
#include <clocale>

using namespace sf;

int main() {
    setlocale(LC_ALL, "spanish");

    RenderWindow window(VideoMode::getDesktopMode(), "Buscaminas", State::Fullscreen);
    menuPrincipal menu;
    menu.ejecutar(window);

    return 0;
}
