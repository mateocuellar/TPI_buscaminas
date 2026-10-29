#include "Partida.h"

using namespace std;
using namespace sf;

Partida::Partida() {
    random_device rd;
    gen.seed(rd());
    cantBombas = 0;
}

bool Partida::cargarRecursos() {
    if (!font.openFromFile("fuente.TTF")) return false;
    if (!texturaBomba.loadFromFile("img/bomba.png")) return false;
    if (!texturaBandera.loadFromFile("img/bandera.png")) return false;
    if (!texturaHud.loadFromFile("img/hud.png")) return false;
    if (!texturaBloque.loadFromFile("img/bloque.png")) return false;
    if (!texturaFondo.loadFromFile("img/fondo.png")) return false;
    return true;
}

void Partida::inicializarTablero(RenderWindow& window) {
    Vector2u tamVentana = window.getSize();


    fondo.emplace(texturaFondo);
    Vector2u tamFondo = texturaFondo.getSize();
    fondo->setScale(Vector2f(
        static_cast<float>(tamVentana.x) / tamFondo.x,
        static_cast<float>(tamVentana.y) / tamFondo.y
    ));


    textoTimer.emplace(font, "", 30);
    textoTimer->setPosition(Vector2f(150, tamVentana.y - 100));
    textoTimer->setFillColor(Color::White);

    textoCantidadBombas.emplace(font, "", 30);
    textoCantidadBombas->setPosition(Vector2f(150, tamVentana.y - 60));


    int cantCuadradosX = 13;
    int cantCuadradosY = 13;
    float tamCuadrado = 60.f;
    float separacion = 3.f;
    float paso = tamCuadrado + separacion;

    float anchoTablero = cantCuadradosX * tamCuadrado + (cantCuadradosX - 1) * separacion;
    float altoTablero = cantCuadradosY * tamCuadrado + (cantCuadradosY - 1) * separacion;
    float inicioX = (tamVentana.x - anchoTablero) / 2.f;
    float altoDisponible = 830.f;
    float inicioY = (altoDisponible - altoTablero) / 2.f;

    for (int i = 0; i < cantCuadradosX; i++) {
        for (int j = 0; j < cantCuadradosY; j++) {
            Sprite boton(texturaBloque);
            Vector2u tam = texturaBloque.getSize();
            boton.setScale({tamCuadrado / tam.x, tamCuadrado / tam.y});
            boton.setPosition({inicioX + i * paso, inicioY + j * paso});
            botones.push_back(boton);
        }
    }

    bloqueVisible.assign(botones.size(), true);
    banderaActiva.assign(botones.size(), false);


    uniform_int_distribution<int> distBomb(5, 15);
    cantBombas = distBomb(gen);
    textoCantidadBombas->setString("CANTIDAD DE BOMBAS: " + to_string(cantBombas));

    vector<int> indices(botones.size());
    iota(indices.begin(), indices.end(), 0);
    shuffle(indices.begin(), indices.end(), gen);
    indicesBombas.assign(indices.begin(), indices.begin() + cantBombas);

    Vector2u tamBomba = texturaBomba.getSize();
    float escalaX = 60.f / tamBomba.x;
    float escalaY = 60.f / tamBomba.y;

    for (int idx : indicesBombas) {
        Sprite b(texturaBomba);
        b.setScale({escalaX, escalaY});
        b.setPosition(botones[idx].getPosition());
        bombas.push_back(b);
    }

    bandera.emplace(texturaBandera);
    bandera->setScale({escalaX, escalaY});

    // HUD
    hud.emplace(texturaHud);
    Vector2u tamHud = texturaHud.getSize();
    hud->setScale({1920.f / tamHud.x, 250.f / tamHud.y});
    hud->setPosition({0, 830.f});

    bombaHud.emplace(texturaBomba);
    bombaHud->setScale({escalaX * 0.7f, escalaY * 0.7f});
    bombaHud->setPosition(Vector2f(550, tamVentana.y - 60));
}

void Partida::ejecutar(RenderWindow& window) {
    if (!cargarRecursos()) return;
    inicializarTablero(window);
    reloj.restart();

    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (keyPressed->code == Keyboard::Key::Escape) {
                    return;
                }
            }

            if (const auto* mouseButton = event->getIf<Event::MouseButtonPressed>()) {
                Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);

                if (mouseButton->button == Mouse::Button::Left) {
                    for (size_t i = 0; i < botones.size(); i++) {
                        if (botones[i].getGlobalBounds().contains(mousePos) && !banderaActiva[i]) {
                            bool clickeoBomba = false;
                            for (int j = 0; j < indicesBombas.size(); j++) {
                                if ((int)i == indicesBombas[j]) {
                                    clickeoBomba = true;
                                    break;
                                }
                            }

                            if (clickeoBomba) {
                                cout << "game over" << endl;
                                bloqueVisible[i] = false;
                            } else {
                                bloqueVisible[i] = false;
                                int offsets[] = {-11, -10, -9, -1, 0, 1, 9, 10, 11};
                                for (int k = 0; k < 9; k++) {
                                    int idx = i + offsets[k];
                                    if (idx < 0 || idx >= (int)botones.size()) continue;

                                    bool esBomba = false;
                                    for (int j = 0; j < indicesBombas.size(); j++) {
                                        if (idx == indicesBombas[j]) {
                                            esBomba = true;
                                            break;
                                        }
                                    }
                                    if (!esBomba) bloqueVisible[idx] = false;
                                }
                            }
                        }
                    }
                }

                if (mouseButton->button == Mouse::Button::Right) {
                    for (size_t i = 0; i < botones.size(); i++) {
                        if (botones[i].getGlobalBounds().contains(mousePos)) {
                            banderaActiva[i] = !banderaActiva[i];
                        }
                    }
                }
            }
        }

        float segundos = reloj.getElapsedTime().asSeconds();
        textoTimer->setString("TIEMPO: " + to_string((int)segundos));

        window.clear(Color(24, 33, 43));


        window.draw(*fondo);
        window.draw(*hud);
        window.draw(*textoTimer);
        window.draw(*textoCantidadBombas);
        window.draw(*bombaHud);

        for (const auto& b : bombas) window.draw(b);
        for (size_t i = 0; i < botones.size(); i++) {
            if (bloqueVisible[i]) window.draw(botones[i]);
        }
        for (size_t i = 0; i < botones.size(); i++) {
            if (banderaActiva[i]) {
                bandera->setPosition(botones[i].getPosition());
                window.draw(*bandera);
            }
        }
        window.display();
    }
}
