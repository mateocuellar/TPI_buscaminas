#include "Partida.h"
#include "ranking.h"
#include "seleccionPersonaje.h"

using namespace std;
using namespace sf;

Partida::Partida() {
    random_device rd;
    gen.seed(rd());
    personaje = nullptr;
    perdio = false;
    gano=false;
    mostrandoDanio = false;
    mostrandoHabilidad = false;

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
    if (!fontNum.openFromFile("fuentenumeros.TTF")) return false;
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
    if(!textura.loadFromFile(rutaImagenes[FOTO])) return false;
    if(!texturaIdle.loadFromFile(rutaImagenes[IDLE])) return false;
    if(!texturaDanio.loadFromFile(rutaImagenes[ANIMDANIO])) return false;
    if(!texturaHabilidad.loadFromFile(rutaImagenes[ANIMHABILIDAD])) return false;
    return true;
}

bool Partida::esBomba(int idx){
    for (int j = 0; j < indicesBombas.size(); j++) {
        if (idx == indicesBombas[j]) {
            return true;
        }
    }
    return false;
}

void Partida::calcularNumeros() {
    numeros.assign(botones.size(), 0);

    for (int col = 0; col < cantCuadradosX; col++) {
        for (int fila = 0; fila < cantCuadradosY; fila++) {
            int idx = col * cantCuadradosY + fila;
            if (esBomba(idx)) continue;

            int cant = 0;
            for (int dc = -1; dc <= 1; dc++) {
                for (int df = -1; df <= 1; df++) {
                    int c = col + dc;
                    int f = fila + df;

                    if (c < 0 || c >= cantCuadradosX) continue;
                    if (f < 0 || f >= cantCuadradosY) continue;

                    if (esBomba(c * cantCuadradosY + f)) {
                        cant++;
                    }
                }
            }
            numeros[idx] = cant;
        }
    }
}
void Partida::destaparRecursivo(int col, int fila) {
    if (col < 0 || col >= cantCuadradosX) return;
    if (fila < 0 || fila >= cantCuadradosY) return;

    int idx = col * cantCuadradosY + fila;

    if (!bloqueVisible[idx]) return;
    if (banderaActiva[idx]) return;
    if (esBomba(idx)) return;

    bloqueVisible[idx] = false;

    if (numeros[idx] > 0) return;

    for (int dc = -1; dc <= 1; dc++) {
        for (int df = -1; df <= 1; df++) {
            destaparRecursivo(col + dc, fila + df);
        }
    }
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
        destaparRecursivo(idx / cantCuadradosY, idx % cantCuadradosY);

        bool todasDestapadas = true;
        for (size_t i = 0; i < botones.size(); i++) {
            if (!esBomba(i) && bloqueVisible[i]) {
                todasDestapadas = false;
                break;
            }
        }

        if (todasDestapadas) {
    gano = true;
    tiempoFinal = reloj.getElapsedTime().asSeconds();
    puntuacion += 100;
    ranking::registrarPuntaje(0, nombreJugador, puntuacion, static_cast<int>(tiempoFinal), 1);
}

    }
}

bool Partida::inicializarTablero(RenderWindow& window) {

    Vector2u tamVentana = Vector2u(window.getView().getSize());

    spritePersonaje.emplace(personaje->getTexturaIdle());
    spritePersonaje->setScale({1.5f, 1.5f});
    spritePersonaje->setOrigin({32.f, 60.f});
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

    textoVictoria.emplace(font, "VICTORIA!", 80);
    textoVictoria->setFillColor(Color::Green);
    FloatRect limitesVic = textoVictoria->getGlobalBounds();
    textoVictoria->setOrigin(Vector2f(
    limitesVic.position.x + limitesVic.size.x / 2.f,
    limitesVic.position.y + limitesVic.size.y / 2.f
    ));
    textoVictoria->setPosition(Vector2f(tamVentana.x / 2.f, tamVentana.y / 2.f));


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

    int cantMinBombas = 20;
    int cantMaxBombas = 30;

    uniform_int_distribution<int> distBomb(cantMinBombas, cantMaxBombas);
    cantBombas = distBomb(gen);
    textoCantidadBombas->setString("CANTIDAD DE BOMBAS: " + to_string(cantBombas));

    vector<int> indices(botones.size());
    iota(indices.begin(), indices.end(), 0);
    shuffle(indices.begin(), indices.end(), gen);
    indicesBombas.assign(indices.begin(), indices.begin() + cantBombas);

    calcularNumeros();
    textoNumero.emplace(fontNum, "", 36);

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

    string comandos[4] = {"E - Habilidad", "B - Bandera", "R - Revelar", "WASD - Mover"};

    for(int i = 0; i < 4; i++){
        Text textocomandos(font, comandos[i], 36);
        textocomandos.setPosition(Vector2f(1280, tamVentana.y - 190 + 40*i));
        vecTextoCommands.push_back(textocomandos);
    }

    return true;

}

void Partida::ejecutar(RenderWindow& window, const string& usuario) {
    nombreJugador = usuario;
    if (!cargarRecursos()) return;
    Vector2u tamVentana = Vector2u(window.getView().getSize());

    personaje = seleccionPersonaje(window);
    if (personaje == nullptr) return;

    if (!inicializarTablero(window)) return;

    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (keyPressed->code == Keyboard::Key::Escape || perdio || gano) {

                    window.setMouseCursorVisible(true);
                    return;
                }

                if (personajeColocado) {
                    if (keyPressed->code == Keyboard::Key::W && cursorFila > 0) {
                        cursorFila--;
                    }
                    if (keyPressed->code == Keyboard::Key::S && cursorFila < cantCuadradosY - 1) {
                        cursorFila++;
                    }
                    if (keyPressed->code == Keyboard::Key::A && cursorCol > 0) {
                        cursorCol--;
                    }
                    if (keyPressed->code == Keyboard::Key::D && cursorCol < cantCuadradosX - 1) {
                        cursorCol++;
                    }
                    if (keyPressed->code == Keyboard::Key::E) {
                        int usosAntes = personaje->getContadorHabilidad();
                        personaje->habilidad();
                        if (personaje->getContadorHabilidad() > usosAntes) {
                            relojAnim.restart();
                            mostrandoHabilidad = true;
                        }
                    }

                    int idx = cursorCol * cantCuadradosY + cursorFila;

                    if (keyPressed->code == Keyboard::Key::R) {
                        destapar(idx);
                    }

                    if (keyPressed->code == Keyboard::Key::B && bloqueVisible[idx]) {
                        banderaActiva[idx] = !banderaActiva[idx];
                    }
                }
            }

            if (const auto* mouseButton = event->getIf<Event::MouseButtonPressed>()) {
                if (perdio || gano) {
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
                            reloj.restart();
                            destapar(i);
                            break;
                        }
                    }
                }
            }
        }

        if (!perdio && !gano && personajeColocado) {
            int tiempo = reloj.getElapsedTime().asSeconds();

            int minutos = tiempo / 60;
            int segundos = tiempo % 60;

            textoTimer->setString("TIEMPO: " + to_string((int)minutos) + ":" + to_string((int)segundos));
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

        for (int i = 0; i < vecTextoCommands.size(); i++) {
            window.draw(vecTextoCommands[i]);
        }

        window.draw(*textoPersonaje);
        window.draw(*textoHablidad);
        window.draw(*textoPasiva);

        for (const auto& b : bombas) window.draw(b);

        for (size_t i = 0; i < botones.size(); i++) {
            if (esBomba(i) || numeros[i] == 0) continue;

            textoNumero->setString(to_string(numeros[i]));

            if (numeros[i] == 1) textoNumero->setFillColor(Color::Blue);
            else if (numeros[i] == 2) textoNumero->setFillColor(Color::Green);
            else if (numeros[i] == 3) textoNumero->setFillColor(Color::Red);
            else textoNumero->setFillColor(Color::Yellow);

            textoNumero->setOutlineColor(Color::Black);
            textoNumero->setOutlineThickness(1.5f);

            FloatRect lb = textoNumero->getLocalBounds();
            Vector2f posNum = botones[i].getPosition();
            textoNumero->setPosition(Vector2f(
                posNum.x + (tamCuadrado - lb.size.x) / 2.f - lb.position.x,
                posNum.y + (tamCuadrado - lb.size.y) / 2.f - lb.position.y
            ));

            window.draw(*textoNumero);
        }

        for (size_t i = 0; i < botones.size(); i++) {
            if (bloqueVisible[i]) window.draw(botones[i]);
        }
        for (size_t i = 0; i < botones.size(); i++) {
            if (banderaActiva[i]) {
                bandera->setPosition(botones[i].getPosition());
                window.draw(*bandera);
            }
        }

        int frame = 0;

        if (mostrandoDanio) {
            frame = relojDanio.getElapsedTime().asSeconds() / 0.12f;
            if (frame >= 4) {
                if (perdio) frame = 3;
                else mostrandoDanio = false;
            }
        }

        if (mostrandoHabilidad && !mostrandoDanio) {
            frame = relojAnim.getElapsedTime().asSeconds() / 0.15f;
            if (frame >= 5) mostrandoHabilidad = false;
        }

        if (mostrandoDanio) {
            spritePersonaje->setTexture(personaje->getTexturaDanio());
            spritePersonaje->setTextureRect(IntRect({frame * 64, 0}, {64, 64}));
            spritePersonaje->setOrigin({32.f, 60.f});
        } else if (mostrandoHabilidad) {
            spritePersonaje->setTexture(personaje->getTexturaHabilidad());
            spritePersonaje->setTextureRect(IntRect({frame * 96, 0}, {96, 96}));
            spritePersonaje->setOrigin({48.f, 84.f});
        } else {
            frame = (int)(relojAnim.getElapsedTime().asSeconds() / 0.5f) % 2;
            spritePersonaje->setTexture(personaje->getTexturaIdle());
            spritePersonaje->setTextureRect(IntRect({frame * 64, 0}, {64, 64}));
            spritePersonaje->setOrigin({32.f, 60.f});
        }

        if (personajeColocado) {
            int idxPj = cursorCol * cantCuadradosY + cursorFila;
            Vector2f pos = botones[idxPj].getPosition();
            spritePersonaje->setPosition({pos.x + tamCuadrado / 2.f, pos.y + tamCuadrado});
            window.draw(*spritePersonaje);
        }

        if (perdio) {
            textoPuntuacion->setString("Puntuacion total: " + to_string(puntuacion));
            textoPuntuacion->setPosition(Vector2f(
                (window.getView().getSize().x - textoPuntuacion->getLocalBounds().size.x) / 2.f,
                window.getView().getSize().y / 2.f + 50
            ));

            window.draw(capaOscura);
            window.draw(*textoGameOver);
            window.draw(*textoTimer);
            window.draw(*textoPuntuacion);
        }

        if (gano) {
    textoPuntuacion->setString("Puntuacion total: " + to_string(puntuacion));
    textoPuntuacion->setPosition(Vector2f(
        (window.getView().getSize().x - textoPuntuacion->getLocalBounds().size.x) / 2.f,
        window.getView().getSize().y / 2.f + 50
    ));

    window.draw(capaOscura);
    window.draw(*textoVictoria);
    window.draw(*textoTimer);
    window.draw(*textoPuntuacion);
}

        window.display();
    }
}
