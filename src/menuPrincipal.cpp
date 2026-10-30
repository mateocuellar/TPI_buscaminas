#include "MenuPrincipal.h"

using namespace std;
using namespace sf;

menuPrincipal::menuPrincipal() {

}

bool menuPrincipal::cargarRecursos() {
    if (!texturaFondo.loadFromFile("img/fondo_menu.png")) return false;
    return font.openFromFile("fuente.TTF");
}


void menuPrincipal::inicializarComponentes(RenderWindow& window) {
    Vector2u tamVentana = window.getSize();

    Color colorBtn(255, 107, 53);

    float anchoBoton = 320.f;
    float altoBoton = 60.f;
    float inicioY = 400.f;
    float espaciado = 80.f;

    fondo.emplace(texturaFondo);
    Vector2u tamFondo = texturaFondo.getSize();
    fondo->setScale(Vector2f(
        static_cast<float>(tamVentana.x) / static_cast<float>(tamFondo.x),
        static_cast<float>(tamVentana.y) / static_cast<float>(tamFondo.y)
    ));
    fondo->setPosition(Vector2f(0,0));

    btnLogin.emplace(Vector2f(anchoBoton, altoBoton));
    btnLogin->setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f, inicioY));
    btnLogin->setFillColor(colorBtn);

    btnRegister.emplace(Vector2f(anchoBoton, altoBoton));
    btnRegister->setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f, inicioY + espaciado));
    btnRegister->setFillColor(colorBtn);

    btnPlay.emplace(Vector2f(anchoBoton, altoBoton));
    btnPlay->setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f, inicioY + (espaciado * 2)));
    btnPlay->setFillColor(colorBtn);

    btnRanking.emplace(Vector2f(anchoBoton, altoBoton));
    btnRanking->setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f, inicioY + (espaciado * 3)));
    btnRanking->setFillColor(colorBtn);


    titulo.emplace(font, "MINESWEEPER:", 94);
    titulo->setPosition(Vector2f((tamVentana.x - titulo->getGlobalBounds().size.x) / 2.f, 180.f));

    titulo2.emplace(font, "ELEMENTAL TACTICS", 64);
    titulo2->setPosition(Vector2f((tamVentana.x - titulo2->getGlobalBounds().size.x) / 2.f, 270.f));

    sombraTitulo.emplace(font, "MINESWEEPER:", 94);
    sombraTitulo->setFillColor(Color(0, 0, 0, 200));
    sombraTitulo->setPosition(Vector2f((tamVentana.x - titulo->getGlobalBounds().size.x) / 2.f + 5, 180.f + 5));

    sombraTitulo2.emplace(font, "ELEMENTAL TACTICS", 64);
    sombraTitulo2->setFillColor(Color(0, 0, 0, 200));
    sombraTitulo2->setPosition(Vector2f((tamVentana.x - titulo2->getGlobalBounds().size.x) / 2.f + 5, 270.f + 5));

    textoLogin.emplace(font, "Iniciar Sesion", 28);
    FloatRect boundsLogin = textoLogin->getGlobalBounds();
    textoLogin->setPosition(Vector2f(
        (tamVentana.x - boundsLogin.size.x) / 2.f,
        btnLogin->getPosition().y + (altoBoton - boundsLogin.size.y) / 2.f - boundsLogin.position.y
    ));

    textoRegister.emplace(font, "Registrarse", 28);
    FloatRect boundsRegister = textoRegister->getGlobalBounds();
    textoRegister->setPosition(Vector2f(
        (tamVentana.x - boundsRegister.size.x) / 2.f,
        btnRegister->getPosition().y + (altoBoton - boundsRegister.size.y) / 2.f - boundsRegister.position.y
    ));

    textoNuevaPartida.emplace(font, "Iniciar Partida", 28);
    FloatRect boundsPlay = textoNuevaPartida->getGlobalBounds();
    textoNuevaPartida->setPosition(Vector2f(
        (tamVentana.x - boundsPlay.size.x) / 2.f,
        btnPlay->getPosition().y + (altoBoton - boundsPlay.size.y) / 2.f - boundsPlay.position.y
    ));

    textoRanking.emplace(font, "Ranking", 28);
    FloatRect boundsRanking = textoRanking->getGlobalBounds();
    textoRanking->setPosition(Vector2f(
        (tamVentana.x - boundsRanking.size.x) / 2.f,
        btnRanking->getPosition().y + (altoBoton - boundsRanking.size.y) / 2.f - boundsRanking.position.y
    ));
}

void menuPrincipal::ejecutar(RenderWindow& window) {
    if (!cargarRecursos()) return;
    inicializarComponentes(window);

    Color colorBtn(255, 107, 53);
    Color colorHover(210, 75, 30);
    Color colorBorde(255, 180, 100);
    vector<RectangleShape*> botones;
    botones.push_back(&*btnLogin);
    botones.push_back(&*btnRegister);
    botones.push_back(&*btnPlay);
    botones.push_back(&*btnRanking);

    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (keyPressed->code == Keyboard::Key::Escape) {
                    window.close();
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
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

        for (size_t i = 0; i < botones.size(); i++) {
            if (botones[i]->getGlobalBounds().contains(mousePos)) {
                botones[i]->setFillColor(colorHover);
                botones[i]->setOutlineColor(colorBorde);
                botones[i]->setOutlineThickness(3.f);
            } else {
                botones[i]->setFillColor(colorBtn);
                botones[i]->setOutlineThickness(0.f);
            }
        }

        window.clear(Color(24, 33, 43));

        window.draw(*fondo);

        window.draw(*btnLogin);
        window.draw(*btnRegister);
        window.draw(*btnPlay);
        window.draw(*btnRanking);

        window.draw(*sombraTitulo);
        window.draw(*sombraTitulo2);

        window.draw(*titulo);
        window.draw(*titulo2);

        window.draw(*textoLogin);
        window.draw(*textoRegister);
        window.draw(*textoNuevaPartida);
        window.draw(*textoRanking);


        window.display();
    }
}

