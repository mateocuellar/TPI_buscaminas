#include "MenuPrincipal.h"

using namespace std;
using namespace sf;

menuPrincipal::menuPrincipal() {

}

bool menuPrincipal::cargarRecursos() {
    return font.openFromFile("fuente.TTF");
}

void menuPrincipal::inicializarComponentes(RenderWindow& window) {
    Vector2u tamVentana = window.getSize();

    float anchoBoton = 320.f;
    float altoBoton = 50.f;
    float inicioY = 300.f;
    float espaciado = 80.f;


    btnLogin.emplace(Vector2f(anchoBoton, altoBoton));
    btnLogin->setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f, inicioY));
    btnLogin->setFillColor(Color(255, 107, 53));

    btnRegister.emplace(Vector2f(anchoBoton, altoBoton));
    btnRegister->setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f, inicioY + espaciado));
    btnRegister->setFillColor(Color(255, 107, 53));

    btnPlay.emplace(Vector2f(anchoBoton, altoBoton));
    btnPlay->setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f, inicioY + (espaciado * 2)));
    btnPlay->setFillColor(Color(255, 107, 53));

    btnRanking.emplace(Vector2f(anchoBoton, altoBoton));
    btnRanking->setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f, inicioY + (espaciado * 3)));
    btnRanking->setFillColor(Color(255, 107, 53));


    titulo.emplace(font, "MINESWEEPER: ELEMENTAL TACTICS", 48);
    titulo->setPosition(Vector2f((tamVentana.x - titulo->getGlobalBounds().size.x) / 2.f, 180.f));

    textoLogin.emplace(font, "Iniciar Sesion", 24);
    FloatRect boundsLogin = textoLogin->getGlobalBounds();
    textoLogin->setPosition(Vector2f(
        (tamVentana.x - boundsLogin.size.x) / 2.f,
        btnLogin->getPosition().y + (altoBoton - boundsLogin.size.y) / 2.f - boundsLogin.position.y
    ));

    textoRegister.emplace(font, "Registrarse", 24);
    FloatRect boundsRegister = textoRegister->getGlobalBounds();
    textoRegister->setPosition(Vector2f(
        (tamVentana.x - boundsRegister.size.x) / 2.f,
        btnRegister->getPosition().y + (altoBoton - boundsRegister.size.y) / 2.f - boundsRegister.position.y
    ));

    textoNuevaPartida.emplace(font, "Iniciar Partida", 24);
    FloatRect boundsPlay = textoNuevaPartida->getGlobalBounds();
    textoNuevaPartida->setPosition(Vector2f(
        (tamVentana.x - boundsPlay.size.x) / 2.f,
        btnPlay->getPosition().y + (altoBoton - boundsPlay.size.y) / 2.f - boundsPlay.position.y
    ));

    textoRanking.emplace(font, "Ranking", 24);
    FloatRect boundsRanking = textoRanking->getGlobalBounds();
    textoRanking->setPosition(Vector2f(
        (tamVentana.x - boundsRanking.size.x) / 2.f,
        btnRanking->getPosition().y + (altoBoton - boundsRanking.size.y) / 2.f - boundsRanking.position.y
    ));
}

void menuPrincipal::ejecutar(RenderWindow& window) {
    if (!cargarRecursos()) return;
    inicializarComponentes(window);

    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (keyPressed->code == Keyboard::Key::Escape) {
                    window.close(); // Cierra el juego desde el menú
                }
            }

            if (const auto* mouseButton = event->getIf<Event::MouseButtonPressed>()) {
                if (mouseButton->button == Mouse::Button::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);

                    if (btnLogin->getGlobalBounds().contains(mousePos)) {
                        cout << "Abriendo Login..." << endl;
                        registroUsuario ui;
                        ui.mostrarLogin(window);
                    }

                    if (btnRegister->getGlobalBounds().contains(mousePos)) {
                        cout << "Abriendo Registro..." << endl;
                        registroUsuario ui;
                        ui.mostrarRegistro(window);
                    }

                    if (btnPlay->getGlobalBounds().contains(mousePos)) {
                        cout << "Iniciando partida orientada a objetos..." << endl;
                        Partida partidaActual;
                        partidaActual.ejecutar(window);
                    }

                    if (btnRanking->getGlobalBounds().contains(mousePos)) {
                        cout << "Clic en: Ranking" << endl;
                    }
                }
            }
        }

        window.clear(Color(24, 33, 43));

        window.draw(*btnLogin);
        window.draw(*btnRegister);
        window.draw(*btnPlay);
        window.draw(*btnRanking);

        window.draw(*textoLogin);
        window.draw(*textoRegister);
        window.draw(*textoNuevaPartida);
        window.draw(*textoRanking);
        window.draw(*titulo);

        window.display();
    }
}

