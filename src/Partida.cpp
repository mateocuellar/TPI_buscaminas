#include "Partida.h"
#include "seleccionPersonaje.h"

using namespace std;
using namespace sf;

Partida::Partida() {
    random_device rd;
    gen.seed(rd());
    personaje = nullptr;
    perdio = false;

    cantBombas = 0;
    tiempoFinal = 0;
    puntuacion = 0;

    personajeColocado = false;
    cursorCol = 0;
    cursorFila = 0;
    cantCuadradosX = 21;
    cantCuadradosY = 11;
    tamCuadrado = 75.f;
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
    if (!texturaVida.loadFromFile("img/spritesHp/corazon_lleno_256.png")) return false;
    if (!texturaArmadura.loadFromFile("img/spritesHp/armadura_llena_256.png")) return false;
    return true;
}

bool Personaje::cargarTexturasPersonajes() {
    if(!textura.loadFromFile(rutaImagen)) return false;
    if(!texturaIdle.loadFromFile(rutaImagenTexturaIdle)) return false;
    if(!texturaDanio.loadFromFile(rutaImgTexturaDanio)) return false;
    return true;
}

void Partida::destapar(int idx) {
    if (!bloqueVisible[idx]) return;
    if (banderaActiva[idx]) return;

    bool clickeoBomba = false;
    for (int j = 0; j < indicesBombas.size(); j++) {
        if (idx == indicesBombas[j]) {
            clickeoBomba = true;
            break;
        }
    }

    if (clickeoBomba) {
        personaje->recibirDanio();
        spritePersonaje->setTexture(personaje->getTexturaDanio());
        relojDanio.restart();
        mostrandoDanio = true;
        if(personaje->getVida()==0){
            perdio = true;
            tiempoFinal = reloj.getElapsedTime().asSeconds();
            for (int j = 0; j < indicesBombas.size(); j++) {
                bloqueVisible[indicesBombas[j]] = false;
            }
        }
    } else {
        puntuacion += 2;
        bloqueVisible[idx] = false;
        int offsets[] = {-12, -11, -10, -1, 0, 1, 10, 11, 12};
        for (int k = 0; k < 9; k++) {
            int vecino = idx + offsets[k];
            if (vecino < 0 || vecino >= (int)botones.size()) continue;

            bool esBomba = false;
            for (int j = 0; j < indicesBombas.size(); j++) {
                if (vecino == indicesBombas[j]) {
                    esBomba = true;
                    break;
                }
            }
            if (!esBomba) bloqueVisible[vecino] = false;
        }
    }
}

bool Partida::inicializarTablero(RenderWindow& window) {
    Vector2u tamVentana = window.getSize();

    spritePersonaje.emplace(personaje->getTexturaIdle());
    Vector2u tamPj = personaje->getTexturaIdle().getSize();
    spritePersonaje->setScale({tamCuadrado / tamPj.x, tamCuadrado / tamPj.y});
    fondo.emplace(texturaFondo);

    Vector2u tamFondo = texturaFondo.getSize();

    fondo->setScale(Vector2f(
        static_cast<float>(tamVentana.x) / tamFondo.x,
        static_cast<float>(tamVentana.y) / tamFondo.y
    ));

    textoTimer.emplace(font, "TIEMPO: 0:0", 30);
    textoTimer->setPosition(Vector2f(150, tamVentana.y - 100));
    textoTimer->setFillColor(Color::White);

    textoCantidadBombas.emplace(font, "", 30);
    textoCantidadBombas->setPosition(Vector2f(150, tamVentana.y - 60));

    ///para gameover
    capaOscura.setSize(Vector2f(tamVentana.x, tamVentana.y));
    capaOscura.setFillColor(Color(0, 0, 0, 180));

    textoGameOver.emplace(font, "GAME OVER", 80);
    textoGameOver->setFillColor(Color::Red);
    FloatRect limites = textoGameOver->getGlobalBounds();
    textoGameOver->setOrigin(Vector2f(
        limites.position.x + limites.size.x / 2.f,
        limites.position.y + limites.size.y / 2.f
    ));
    textoGameOver->setPosition(Vector2f(tamVentana.x / 2.f, tamVentana.y / 2.f));

    textoPuntuacion.emplace(font, "", 30);
    textoPuntuacion->setFillColor(Color::White);

    float anchoTablero = cantCuadradosX * tamCuadrado + (cantCuadradosX - 1);
    float altoTablero = cantCuadradosY * tamCuadrado + (cantCuadradosY - 1);
    float inicioX = (tamVentana.x - anchoTablero) / 2.f;
    float altoDisponible = 830.f;
    float inicioY = (altoDisponible - altoTablero) / 2.f;

    for (int i = 0; i < cantCuadradosX; i++) {
        for (int j = 0; j < cantCuadradosY; j++) {
            Sprite boton(texturaBloque);
            Vector2u tam = texturaBloque.getSize();
            boton.setScale({tamCuadrado / tam.x, tamCuadrado / tam.y});
            boton.setPosition({inicioX + i * tamCuadrado, inicioY + j * tamCuadrado});
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
    bombaHud->setPosition(Vector2f(550, tamVentana.y - 70));

    vidaHud.emplace(texturaVida);
    Vector2u tamVidaHud = texturaVida.getSize();
    vidaHud->setPosition(Vector2f(270, tamVentana.y - 170));
    vidaHud->setScale({escalaX * 0.3f, escalaY * 0.3f});

    textoVidaActual.emplace(font, to_string(personaje->getVida()), 30);
    textoVidaActual->setPosition(Vector2f(270 + 60, tamVentana.y - 170));

    armaduraHud.emplace(texturaArmadura);
    armaduraHud->setScale({escalaX * 0.3f, escalaY * 0.3f});
    armaduraHud->setPosition(Vector2f(400, tamVentana.y - 170));

    textoArmaduraActual.emplace(font, to_string(personaje->getArmadura()),30);
    textoArmaduraActual->setPosition(Vector2f(400 + 60, tamVentana.y - 170));

    textoPersonaje.emplace(font, personaje->getTexto(NOMBRE), 36);
    textoPersonaje->setPosition(Vector2f(270, tamVentana.y - 230));

    textoHablidad.emplace(font, personaje->getTexto(HABILIDAD), 36);
    textoHablidad->setPosition(Vector2f(750, tamVentana.y - 190));
    textoHablidad->setFillColor(personaje->getColor(colorHabilidad));

    textoPasiva.emplace(font, personaje->getTexto(PASIVA), 36);
    textoPasiva->setPosition(Vector2f(750, tamVentana.y - 150));
    textoPasiva->setFillColor(personaje->getColor(colorPasiva));

    textoStatus.emplace(font, "ESTADO", 36);
    textoStatus->setPosition(Vector2f(800, tamVentana.y - 240));

    textoCommands.emplace(font, "COMANDOS", 36);
    textoCommands->setPosition(Vector2f(1280, tamVentana.y - 240));

    string comandos[4] = {"E - Habilidad", "F - Bandera", "Q - Detonar", "R - Adyacente"};

    for(int i = 0;i < 4; i++){
        Text textocomandos(font, comandos[i], 36);
        textocomandos.setPosition(Vector2f(1280, tamVentana.y - 190 + 40*i));
        vecTextoCommands.push_back(textocomandos);
    }

    return true;
}

void Partida::ejecutar(RenderWindow& window) {
    if (!cargarRecursos()) return;
    Vector2u tamVentana = window.getSize();

    personaje = seleccionPersonaje(window);
    if (personaje == nullptr) return;

    if (!inicializarTablero(window)) return;

    while (window.isOpen()) {
                while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (keyPressed->code == Keyboard::Key::Escape || perdio) {
                    window.setMouseCursorVisible(true);
                    return;
                }

                if (personajeColocado) {
                    if (keyPressed->code == Keyboard::Key::W && cursorFila > 0){
                        cursorFila--;
                    }
                    if (keyPressed->code == Keyboard::Key::S && cursorFila < cantCuadradosY - 1){
                        cursorFila++;
                    }
                    if (keyPressed->code == Keyboard::Key::A && cursorCol > 0){
                        cursorCol--;
                    }
                    if (keyPressed->code == Keyboard::Key::D && cursorCol < cantCuadradosX - 1){
                        cursorCol++;
                    }
                    if (keyPressed->code == Keyboard::Key::E){
                        personaje->habilidad();
                    }

                    int idx = cursorCol * cantCuadradosY + cursorFila;

                    if (keyPressed->code == Keyboard::Key::Space)
                        destapar(idx);
                    if (keyPressed->code == Keyboard::Key::F)
                        banderaActiva[idx] = !banderaActiva[idx];
                }
            }

            if (const auto* mouseButton = event->getIf<Event::MouseButtonPressed>()) {
                if (perdio) {
                    window.setMouseCursorVisible(true);
                    return;
                }

                if (mouseButton->button == Mouse::Button::Left && !personajeColocado) {
                    Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);

                    for (size_t i = 0; i < botones.size(); i++) {
                        if (botones[i].getGlobalBounds().contains(mousePos)) {
                            cursorCol = i / cantCuadradosY;
                            cursorFila = i % cantCuadradosY;
                            personajeColocado = true;
                            window.setMouseCursorVisible(false);
                            destapar(i);
                            reloj.restart();
                            break;
                        }
                    }
                }
            }
        }

        if (!perdio && personajeColocado) {
            int tiempo = reloj.getElapsedTime().asSeconds();

            int minutos = tiempo/60;
            int segundos = tiempo%60;

            textoTimer->setString("TIEMPO: " + to_string((int)minutos) +":"+ to_string((int)segundos));
        }
        textoArmaduraActual->setString(to_string(personaje->getArmadura()));
        textoVidaActual->setString(to_string(personaje->getVida()));

        window.clear(Color(24, 33, 43));

        window.draw(*fondo);

        window.draw(*hud);
        window.draw(*textoTimer);
        window.draw(*bombaHud);
        window.draw(*textoCantidadBombas);
        window.draw(*vidaHud);
        window.draw(*textoVidaActual);
        window.draw(*armaduraHud);
        window.draw(*textoArmaduraActual);
        window.draw(*textoStatus);
        window.draw(*textoCommands);

        for(int i = 0;i < vecTextoCommands.size(); i++){
            window.draw(vecTextoCommands[i]);
        }

        window.draw(*textoPersonaje);
        window.draw(*textoHablidad);
        window.draw(*textoPasiva);

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
        if (mostrandoDanio && relojDanio.getElapsedTime().asSeconds() >= 1.f && !perdio) {
        spritePersonaje->setTexture(personaje->getTexturaIdle());
        mostrandoDanio = false;
}
        if (personajeColocado) {
            int idxPj = cursorCol * cantCuadradosY + cursorFila;
            spritePersonaje->setPosition(botones[idxPj].getPosition());
            window.draw(*spritePersonaje);
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
