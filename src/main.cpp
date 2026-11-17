#include <SFML/Graphics.hpp>
#include "menuPrincipal.h"
#include <clocale>

using namespace sf;

int main() {
    setlocale(LC_ALL, "spanish");


    RenderWindow window(VideoMode::getDesktopMode(), "Buscaminas", State::Fullscreen);

    View vistaJuego(FloatRect({0.f, 0.f}, {1920.f, 1080.f}));
    window.setView(vistaJuego);


    menuPrincipal menu;
    menu.ejecutar(window);

    return 0;
}
