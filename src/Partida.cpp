#include "Partida.h"
#include "seleccionPersonaje.h"

using namespace std;
using namespace sf;

Partida::Partida() {
    random_device rd;
    gen.seed(rd());
    cantBombas = 0;
    personaje = nullptr;
    perdio = false;
    tiempoFinal = 0;
}

Partida::~Partida() {
    delete personaje;
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

bool Partida::inicializarTablero(RenderWindow& window) {
    Vector2u tamVentana = window.getSize();

    puntuacion = 0;

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

    ///para gamerover
    capaOscura.setSize(Vector2f(tamVentana.x, tamVentana.y));
    capaOscura.setFillColor(Color(0, 0, 0, 180));

    textoGameOver.emplace(font, "GAME OVER", 80);
    textoGameOver->setFillColor(Color::Red);
    FloatRect limites = textoGameOver->getLocalBounds();
    textoGameOver->setOrigin(Vector2f(
        limites.position.x + limites.size.x / 2.f,
        limites.position.y + limites.size.y / 2.f
    ));
    textoGameOver->setPosition(Vector2f(tamVentana.x / 2.f, tamVentana.y / 2.f));

    textoPuntuacion.emplace(font, "", 30);
    textoPuntuacion->setFillColor(Color::White);

    int cantCuadradosX = 20;
    float tamCuadrado = 75.f;
    int cantCuadradosY = 10;
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
    float escalaX = tamCuadrado / tamBomba.x;
    float escalaY = tamCuadrado / tamBomba.y;

    for (int idx : indicesBombas) {
        Sprite b(texturaBomba);
        b.setScale({escalaX, escalaY});
        b.setPosition(botones[idx].getPosition());
        bombas.push_back(b);
    }

    bandera.emplace(texturaBandera);
    bandera->setScale({escalaX, escalaY});

    hud.emplace(texturaHud);
    Vector2u tamHud = texturaHud.getSize();
    hud->setScale({1920.f / tamHud.x, 250.f / tamHud.y});
    hud->setPosition({0, 830.f});

    bombaHud.emplace(texturaBomba);
    bombaHud->setScale({escalaX * 0.7f, escalaY * 0.7f});
    bombaHud->setPosition(Vector2f(550, tamVentana.y - 60));

    textoPersonaje.emplace(font, personaje->getTexto(NOMBRE), 36);
    textoPersonaje->setPosition(Vector2f(270, tamVentana.y - 230));

    return true;
}

void Partida::ejecutar(RenderWindow& window) {
    if (!cargarRecursos()) return;

    personaje = seleccionPersonaje(window);
    if (personaje == nullptr) return;

    if (!inicializarTablero(window)) return;
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

                if (perdio) {
                    return;
                }

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
                                perdio = true;
                                tiempoFinal = reloj.getElapsedTime().asSeconds();
                                for (int j = 0; j < indicesBombas.size(); j++) {
                                    bloqueVisible[indicesBombas[j]] = false;
                                }
                            } else {
                                puntuacion += 2;
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

        if (!perdio) {
            float segundos = reloj.getElapsedTime().asSeconds();
            textoTimer->setString("TIEMPO: " + to_string((int)segundos));
        }

        window.clear(Color(24, 33, 43));

        window.draw(*fondo);
        window.draw(*hud);
        window.draw(*textoTimer);
        window.draw(*textoCantidadBombas);
        window.draw(*bombaHud);
        window.draw(*textoPersonaje);

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

        if (perdio) {

            textoPuntuacion->setString("Puntuacion total: " + to_string(puntuacion));
            textoPuntuacion->setPosition(Vector2f((window.getSize().x - textoPuntuacion->getLocalBounds().size.x) / 2.f, window.getSize().y / 2.f + 50));

            window.draw(capaOscura);
            window.draw(*textoGameOver);
            window.draw(*textoTimer);
            window.draw(*textoPuntuacion);
        }

        window.display();
    }
}
