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

    tiempoFinal = 0.f;
    tiempoAcumulado = 0.f;
    tiempoLimite = 300.f;
    if (dynamic_cast<DunbarYunque*>(personaje) != nullptr) {
        tiempoLimite *= 0.75f;
    }
    tiempoCongelado = 0.f;

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
    if (tienePowerUp[idx] && !powerUpRecogido[idx]) {
        powerUpRecogido[idx] = true;
        puntuacion += 30;
        tiempoAcumulado = max(0.f, tiempoAcumulado - 15.f);
        personaje->recargarHabilidad();
        personaje->setArmadura(personaje->getArmadura() + 1);
    }
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
    for (size_t j = 0; j < indicesBombas.size(); j++) {
        if (idx == indicesBombas[j]) {
            clickeoBomba = true;
            break;
        }
    }
    if (clickeoBomba) {
        bloqueVisible[idx] = false;
        bool esCasper = (dynamic_cast<CasperNull*>(personaje) != nullptr);
        bool esDunbar = (dynamic_cast<DunbarYunque*>(personaje) != nullptr);
        bool esTess   = (dynamic_cast<TessKnot*>(personaje) != nullptr);
        if (tipoMina[idx] == FUEGO) {
            int col = idx / cantCuadradosY;
            int fila = idx % cantCuadradosY;
            for (int dc = -1; dc <= 1; dc++) {
                for (int df = -1; df <= 1; df++) {
                    int c = col + dc, f = fila + df;
                    if (c >= 0 && c < cantCuadradosX && f >= 0 && f < cantCuadradosY) {
                        turnosFuego[c * cantCuadradosY + f] = 6;
                    }
                }
            }
        }
        else if (tipoMina[idx] == HIELO) {
            if (!esDunbar) {
                tiempoCongelado = 10.f;
            }
        }
        if (esCasper && tipoMina[idx] == FUEGO) {
            personaje->setArmadura(0);
            personaje->setVida(0);
        }
        else if (esTess && tipoMina[idx] == COMUN) {
            personaje->recibirDanio();
            personaje->recibirDanio();
        }
        else {
            personaje->recibirDanio();
        }
        relojDanio.restart();
        mostrandoDanio = true;
        if (personaje->getVida() <= 0) {
            personaje->setVida(0);
            perdio = true;
            tiempoFinal = tiempoAcumulado;
            for (int bIdx : indicesBombas) bloqueVisible[bIdx] = false;
        }
        }
        else {
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
                tiempoFinal = tiempoAcumulado;
                puntuacion += 100 + static_cast<int>(max(0.f, tiempoLimite - tiempoAcumulado));
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
    fondo->setScale(Vector2f(static_cast<float>(tamVentana.x) / tamFondo.x,static_cast<float>(tamVentana.y) / tamFondo.y));
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
    textoGameOver->setOrigin(Vector2f(limites.position.x + limites.size.x / 2.f,limites.position.y + limites.size.y / 2.f));
    textoGameOver->setPosition(Vector2f(tamVentana.x / 2.f, tamVentana.y / 2.f));
    textoVictoria.emplace(font, "VICTORIA!", 80);
    textoVictoria->setFillColor(Color::Green);
    FloatRect limitesVic = textoVictoria->getGlobalBounds();
    textoVictoria->setOrigin(Vector2f(limitesVic.position.x + limitesVic.size.x / 2.f,limitesVic.position.y + limitesVic.size.y / 2.f));
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
    tipoMina.assign(botones.size(), NINGUNA);
    turnosFuego.assign(botones.size(), 0);
    numeros.assign(botones.size(), 0);
    numerosMostrados.assign(botones.size(), 0);
    tienePowerUp.assign(botones.size(), false);
    powerUpRecogido.assign(botones.size(), false);
    uniform_int_distribution<int> distBomb(22, 32);
    cantBombas = distBomb(gen);
    textoCantidadBombas->setString("CANTIDAD DE BOMBAS: " + to_string(cantBombas));
    textoNumero.emplace(fontNum, "", 36);
    tiempoLimite = 300.f;
    if (dynamic_cast<DunbarYunque*>(personaje) != nullptr) {
        tiempoLimite *= 0.75f;
    }
    Vector2u tamBomba = texturaBomba.getSize();
    float escalaX = tamCuadrado / tamBomba.x;
    float escalaY = tamCuadrado / tamBomba.y;
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
    vidaHud->setPosition(Vector2f(270, tamVentana.y - 170));
    vidaHud->setScale({escalaX * 0.3f, escalaY * 0.3f});
    textoVidaActual.emplace(font, to_string(personaje->getVida()), 30);
    textoVidaActual->setPosition(Vector2f(270 + 60, tamVentana.y - 170));
    armaduraHud.emplace(texturaArmadura);
    armaduraHud->setScale({escalaX * 0.3f, escalaY * 0.3f});
    armaduraHud->setPosition(Vector2f(400, tamVentana.y - 170));
    textoArmaduraActual.emplace(font, to_string(personaje->getArmadura()), 30);
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
    for (int i = 0; i < 4; i++) {
        Text textocomandos(font, comandos[i], 36);
        textocomandos.setPosition(Vector2f(1280, tamVentana.y - 190 + 40 * i));
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
    bool esCasper = (dynamic_cast<CasperNull*>(personaje) != nullptr);
    bool esDunbar = (dynamic_cast<DunbarYunque*>(personaje) != nullptr);
    bool esTess   = (dynamic_cast<TessKnot*>(personaje) != nullptr);
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
                    bool seMovio = false;
                    if (keyPressed->code == Keyboard::Key::W && cursorFila > 0) {
                        int dest = cursorCol * cantCuadradosY + (cursorFila - 1);
                        if (turnosFuego[dest] == 0) { cursorFila--; seMovio = true; }
                    }
                    if (keyPressed->code == Keyboard::Key::S && cursorFila < cantCuadradosY - 1) {
                        int dest = cursorCol * cantCuadradosY + (cursorFila + 1);
                        if (turnosFuego[dest] == 0) { cursorFila++; seMovio = true; }
                    }
                    if (keyPressed->code == Keyboard::Key::A && cursorCol > 0) {
                        int dest = (cursorCol - 1) * cantCuadradosY + cursorFila;
                        if (turnosFuego[dest] == 0) { cursorCol--; seMovio = true; }
                    }
                    if (keyPressed->code == Keyboard::Key::D && cursorCol < cantCuadradosX - 1) {
                        int dest = (cursorCol + 1) * cantCuadradosY + cursorFila;
                        if (turnosFuego[dest] == 0) { cursorCol++; seMovio = true; }
                    }
                    int idx = cursorCol * cantCuadradosY + cursorFila;
                    if (seMovio) {
                        actualizarTurnosFuego();
                        if (!bloqueVisible[idx] && tienePowerUp[idx] && !powerUpRecogido[idx]) {
                        powerUpRecogido[idx] = true;
                        puntuacion += 30;
                        tiempoAcumulado = max(0.f, tiempoAcumulado - 15.f);
                        personaje->recargarHabilidad();
                        personaje->setArmadura(personaje->getArmadura() + 1);
                        }

                        if (esTess) {
                            turnosRecargaTess++;
                            if (turnosRecargaTess >= 5) {
                                turnosRecargaTess = 0;
                                personaje->recargarHabilidad();
                            }
                            bool peligroCadena = false;
                            for (int dc = -1; dc <= 1; dc++) {
                                for (int df = -1; df <= 1; df++) {
                                    int c = cursorCol + dc, f = cursorFila + df;
                                    if (c >= 0 && c < cantCuadradosX && f >= 0 && f < cantCuadradosY) {
                                        int v = c * cantCuadradosY + f;
                                        if (tipoMina[v] == FUEGO || tipoMina[v] == HIELO || tipoMina[v] == RADIACION) {
                                            peligroCadena = true;
                                        }
                                    }
                                }
                            }
            if (peligroCadena) {
                textoPasiva->setString("ALERTA: CADENA ELEMENTAL");
                textoPasiva->setFillColor(Color(255, 100, 100));
            } else {
                textoPasiva->setString(personaje->getTexto(PASIVA));
                textoPasiva->setFillColor(personaje->getColor(colorPasiva));
            }
        }
    }

    if (keyPressed->code == Keyboard::Key::E) {
        int usosAntes = personaje->getContadorHabilidad();
        personaje->habilidad();
        if (personaje->getContadorHabilidad() > usosAntes) {
            ejecutarHabilidadTactica();
            relojAnim.restart();
            mostrandoHabilidad = true;
        }
    }

    if (keyPressed->code == Keyboard::Key::R) {
        destapar(idx);
    }

    if (keyPressed->code == Keyboard::Key::B && bloqueVisible[idx]) {
        banderaActiva[idx] = !banderaActiva[idx];

        if (banderaActiva[idx] && !esBomba(idx)) {
            if (esCasper && banderasErroneasUsadas == 0) {
                banderasErroneasUsadas++;
            } else if (esDunbar) {
                puntuacion = max(0, puntuacion - 15);
            }
        }
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
                            relojDelta.restart();
                            generarMapaSeguro(i);
                            destapar(i);
                            break;
                        }
                    }
                }
            }
        }


        float dt = relojDelta.restart().asSeconds();
        if (!perdio && !gano && personajeColocado) {
            float mult = 1.f;
            if (tiempoCongelado > 0.f) {
                tiempoCongelado -= dt;
                mult = 2.f;
                textoTimer->setFillColor(Color::Cyan);
            } else {
                textoTimer->setFillColor(Color::White);
            }
            tiempoAcumulado += dt * mult;

            int restante = max(0, static_cast<int>(tiempoLimite - tiempoAcumulado));
            int minutos = restante / 60;
            int segundos = restante % 60;

            textoTimer->setString("TIEMPO: " + to_string(minutos) + ":" + to_string(segundos));

            if (restante <= 0) {
                perdio = true;
                tiempoFinal = tiempoAcumulado;
            }
        }

        textoArmaduraActual->setString(to_string(personaje->getArmadura()));
        textoVidaActual->setString(to_string(personaje->getVida()));
        textoHablidad->setString(personaje->getTexto(HABILIDAD) + " (" + to_string(personaje->getUsosRestantes()) + ")");

        int bombasRestantes = cantBombas;
        for (size_t i = 0; i < botones.size(); i++) {
            if ((esBomba(i) && !bloqueVisible[i]) || banderaActiva[i]) {
                bombasRestantes--;
            }
        }
        textoCantidadBombas->setString("CANTIDAD DE BOMBAS: " + to_string(max(0, bombasRestantes)));

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

        for (size_t i = 0; i < vecTextoCommands.size(); i++) {
            window.draw(vecTextoCommands[i]);
        }

        window.draw(*textoPersonaje);
        window.draw(*textoHablidad);
        window.draw(*textoPasiva);

        for (const auto& b : bombas) window.draw(b);

        for (size_t i = 0; i < botones.size(); i++) {
            int valNum = esCasper ? numeros[i] : numerosMostrados[i];
            if (esBomba(i) || valNum == 0) continue;

            textoNumero->setString(to_string(valNum));

            if (valNum == 1) textoNumero->setFillColor(Color::Blue);
            else if (valNum == 2) textoNumero->setFillColor(Color::Green);
            else if (valNum == 3) textoNumero->setFillColor(Color::Red);
            else textoNumero->setFillColor(Color::Yellow);

            textoNumero->setOutlineColor(Color::Black);
            textoNumero->setOutlineThickness(1.5f);

            FloatRect lb = textoNumero->getLocalBounds();
            Vector2f posNum = botones[i].getPosition();
            textoNumero->setPosition(Vector2f(
                posNum.x + (tamCuadrado - lb.size.x) / 2.f - lb.position.x,
                posNum.y + (tamCuadrado - lb.size.y) / 2.f - lb.position.y
            ));

            bool estaRadiado = (numerosMostrados[i] != numeros[i]);
            if (esCasper && estaRadiado) {
                textoNumero->setOutlineColor(Color(50, 255, 100));
                textoNumero->setOutlineThickness(2.2f);
            } else if (estaRadiado) {
                uint8_t pulso = static_cast<uint8_t>(120 + 135 * sin(tiempoAcumulado * 7.f));
                textoNumero->setOutlineColor(Color(40, pulso, 40));
                textoNumero->setOutlineThickness(2.2f);
            } else {
                textoNumero->setOutlineColor(Color::Black);
                textoNumero->setOutlineThickness(1.5f);
            }

            window.draw(*textoNumero);
        }

        for (size_t i = 0; i < botones.size(); i++) {
            if (tienePowerUp[i] && !bloqueVisible[i] && !powerUpRecogido[i]) {
                CircleShape orbe(tamCuadrado * 0.22f);
                orbe.setFillColor(Color(255, 215, 0));
                orbe.setOutlineColor(Color(255, 120, 0));
                orbe.setOutlineThickness(2.f);
                Vector2f pos = botones[i].getPosition();
                orbe.setPosition({pos.x + tamCuadrado * 0.28f, pos.y + tamCuadrado * 0.28f});
                window.draw(orbe);
            }
        }

        for (size_t i = 0; i < botones.size(); i++) {
            if (turnosFuego[i] > 0) {
                botones[i].setColor(Color(255, 85, 35, 255));
                window.draw(botones[i]);
            } else if (bloqueVisible[i]) {
                botones[i].setColor(Color::White);
                window.draw(botones[i]);
            }
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
RectangleShape selector({tamCuadrado, tamCuadrado});
selector.setPosition(pos);
selector.setFillColor(Color::Transparent);
selector.setOutlineThickness(3.f);
selector.setOutlineColor(Color(255, 220, 50));
window.draw(selector);
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



void Partida::generarMapaSeguro(int idxSeguro) {
    int totalCeldas = botones.size();
    int colSegura = idxSeguro / cantCuadradosY;
    int filaSegura = idxSeguro % cantCuadradosY;

    vector<int> candidatos;
    for (int i = 0; i < totalCeldas; i++) {
        int c = i / cantCuadradosY;
        int f = i % cantCuadradosY;
        if (abs(c - colSegura) <= 1 && abs(f - filaSegura) <= 1) continue;
        candidatos.push_back(i);
    }

    shuffle(candidatos.begin(), candidatos.end(), gen);
    indicesBombas.assign(candidatos.begin(), candidatos.begin() + cantBombas);

    Vector2u tamBomba = texturaBomba.getSize();
    float escalaX = tamCuadrado / tamBomba.x;
    float escalaY = tamCuadrado / tamBomba.y;

    uniform_int_distribution<int> distTipo(1, 100);
    for (int idx : indicesBombas) {
        int roll = distTipo(gen);
        Sprite b(texturaBomba);
        b.setScale({escalaX, escalaY});
        b.setPosition(botones[idx].getPosition());

        if (roll <= 40) {
            tipoMina[idx] = COMUN;
        } else if (roll <= 60) {
            tipoMina[idx] = FUEGO;
            b.setColor(Color(255, 80, 50));
        } else if (roll <= 80) {
            tipoMina[idx] = HIELO;
            b.setColor(Color(80, 220, 255));
        } else {
            tipoMina[idx] = RADIACION;
            b.setColor(Color(100, 255, 80));
        }
        bombas.push_back(b);
    }


    for (int i = cantBombas; i < cantBombas + 5 && i < (int)candidatos.size(); i++) {
        tienePowerUp[candidatos[i]] = true;
    }

    calcularNumeros();
    aplicarRadiacion();
}

void Partida::aplicarRadiacion() {
    numerosMostrados = numeros;
    uniform_int_distribution<int> distAlterar(0, 1);

    for (int idx : indicesBombas) {
        if (tipoMina[idx] != RADIACION) continue;

        int col = idx / cantCuadradosY;
        int fila = idx % cantCuadradosY;

        for (int dc = -1; dc <= 1; dc++) {
            for (int df = -1; df <= 1; df++) {
                int c = col + dc;
                int f = fila + df;
                if (c < 0 || c >= cantCuadradosX || f < 0 || f >= cantCuadradosY) continue;

                int vecino = c * cantCuadradosY + f;
                if (esBomba(vecino) || numeros[vecino] == 0) continue;

                int delta = (distAlterar(gen) == 0) ? -1 : 1;
                int falso = numeros[vecino] + delta;
                if (falso < 1) falso = 2;
                if (falso > 8) falso = 7;
                numerosMostrados[vecino] = falso;
            }
        }
    }
}

void Partida::ejecutarHabilidadTactica() {
    if (dynamic_cast<CasperNull*>(personaje) != nullptr) {
        bool desactivoUna = false;
        for (int dc = -2; dc <= 2; dc++) {
            for (int df = -2; df <= 2; df++) {
                int c = cursorCol + dc, f = cursorFila + df;
                if (c < 0 || c >= cantCuadradosX || f < 0 || f >= cantCuadradosY) continue;
                int v = c * cantCuadradosY + f;
                numerosMostrados[v] = numeros[v];
                if (!desactivoUna && esBomba(v) && !banderaActiva[v]) {
                    banderaActiva[v] = true;
                    desactivoUna = true;
                }
            }
        }
    }

    else if (dynamic_cast<DunbarYunque*>(personaje) != nullptr) {
        for (int dc = -2; dc <= 2; dc++) {
            for (int df = -2; df <= 2; df++) {
                int c = cursorCol + dc, f = cursorFila + df;
                if (c >= 0 && c < cantCuadradosX && f >= 0 && f < cantCuadradosY) {
                    turnosFuego[c * cantCuadradosY + f] = 0;
                }
            }
        }
    }

    else if (dynamic_cast<TessKnot*>(personaje) != nullptr) {
        for (int dc = -1; dc <= 1; dc++) {
            for (int df = -1; df <= 1; df++) {
                int c = cursorCol + dc, f = cursorFila + df;
                if (c < 0 || c >= cantCuadradosX || f < 0 || f >= cantCuadradosY) continue;
                int v = c * cantCuadradosY + f;
                if (!bloqueVisible[v]) continue;

                if (esBomba(v)) banderaActiva[v] = true;
                else destaparRecursivo(c, f);
            }
        }
    }
}

void Partida::actualizarTurnosFuego() {
    for (size_t i = 0; i < turnosFuego.size(); i++) {
        if (turnosFuego[i] > 0) turnosFuego[i]--;
    }
}


