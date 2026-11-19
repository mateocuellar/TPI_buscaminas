#include "Juego.h"
#include "menuPrincipal.h"

using namespace std;
using namespace sf;

Juego::Juego() : mapa(21, 11) {

    random_device rd;
    gen.seed(rd());

    puntajeActual = 0;
    nivelActual = 1;
    personaje = nullptr;

    mostrandoDanio = false;
    mostrandoHabilidad = false;
    perdio = false;
    gano = false;
    personajeColocado = false;

    tiempoFinal = 0.f;
    tiempoAcumulado = 0.f;
    tiempoLimite = 300.f;
    tiempoCongelado = 0.f;

    cursorCol = 0;
    cursorFila = 0;
    tamCuadrado = 75.f;
    cantBombas = 0;

    banderasErroneasUsadas = 0;
    turnosRecargaTess = 0;
}

Juego::~Juego() {

    delete personaje;
}

bool Juego::cargarRecursos() {

    vector<string> rutasBombas = {
    "img/bomba.png",
    "img/bomba_fuego.png",
    "img/bomba_hielo.png",
    "img/bomba_radiacion.png"
    };

    texturasBombas.resize(rutasBombas.size());

    for (size_t i = 0; i < rutasBombas.size(); i++) {
        if (!texturasBombas[i].loadFromFile(rutasBombas[i])) return false;
    }
    if (!font.openFromFile("fuente.TTF")) return false;
    if (!fontNum.openFromFile("fuentenumeros.TTF")) return false;
    if (!texturaBandera.loadFromFile("img/bandera.png")) return false;
    if (!texturaHud.loadFromFile("img/hud.png")) return false;
    if (!texturaBloque.loadFromFile("img/bloque.png")) return false;
    if (!texturaFondo.loadFromFile("img/fondo.png")) return false;
    if (!texturaVida.loadFromFile("img/spritesHp/corazon_lleno_256.png")) return false;
    if (!texturaArmadura.loadFromFile("img/spritesHp/armadura_llena_256.png")) return false;
    return true;
}

bool Juego::inicializarTablero(RenderWindow& window) {

    Vector2u tamVentana = Vector2u(window.getView().getSize());

    spritePersonaje.emplace(personaje->getTextura(IDLE));
    spritePersonaje->setScale({1.5f, 1.5f});
    spritePersonaje->setOrigin({32.f, 60.f});

    fondo.emplace(texturaFondo);
    Vector2u tamFondo = texturaFondo.getSize();
    fondo->setScale(Vector2f(static_cast<float>(tamVentana.x) / tamFondo.x, static_cast<float>(tamVentana.y) / tamFondo.y));

    textoTimer.emplace(font, "TIEMPO: 0:0", 30);
    textoTimer->setPosition(Vector2f(750, tamVentana.y - 185));
    textoTimer->setFillColor(Color::White);

    textoCantidadBombas.emplace(font, "", 30);
    textoCantidadBombas->setPosition(Vector2f(750, tamVentana.y - 140));

    capaOscura.setSize(Vector2f(tamVentana.x, tamVentana.y));
    capaOscura.setFillColor(Color(0, 0, 0, 180));

    textoPuntajeHud.emplace(font, "PUNTAJE: 0", 30);
    textoPuntajeHud->setPosition(Vector2f(750.f, tamVentana.y - 97.f));
    textoPuntajeHud->setFillColor(Color(90, 90, 90, 170));


    textoGameOver.emplace(font, "GAME OVER", 80);
    textoGameOver->setFillColor(Color::Red);
    FloatRect limites = textoGameOver->getGlobalBounds();
    textoGameOver->setOrigin(Vector2f(limites.position.x + limites.size.x / 2.f, limites.position.y + limites.size.y / 2.f));
    textoGameOver->setPosition(Vector2f(tamVentana.x / 2.f, tamVentana.y / 2.f));

    textoVictoria.emplace(font, "VICTORIA!", 80);
    textoVictoria->setFillColor(Color::Green);
    FloatRect limitesVic = textoVictoria->getGlobalBounds();
    textoVictoria->setOrigin(Vector2f(limitesVic.position.x + limitesVic.size.x / 2.f, limitesVic.position.y + limitesVic.size.y / 2.f));
    textoVictoria->setPosition(Vector2f(tamVentana.x / 2.f, tamVentana.y / 2.f));

    textoPuntuacion.emplace(font, "", 30);
    textoPuntuacion->setFillColor(Color::White);

    botonVolver.setSize({240.f, 55.f});
    botonVolver.setFillColor(Color(50, 60, 75));
    botonVolver.setOutlineColor(Color::White);
    botonVolver.setOutlineThickness(2.f);
    botonVolver.setOrigin({120.f, 27.5f});
    botonVolver.setPosition({tamVentana.x  / 2.f, tamVentana.y / 2.f + 130.f});

    textoVolver.emplace(font, "Volver al menú", 32);
    textoVolver->setFillColor(Color::White);
    FloatRect lbVolver = textoVolver->getLocalBounds();
    textoVolver->setOrigin({lbVolver.position.x + lbVolver.size.x / 2.f, lbVolver.position.y + lbVolver.size.y / 2.f});
    textoVolver->setPosition(botonVolver.getPosition());

    float anchoTablero = mapa.columnas * tamCuadrado + (mapa.columnas - 1);
    float altoTablero = mapa.filas * tamCuadrado + (mapa.filas - 1);
    float inicioX = (tamVentana.x - anchoTablero) / 2.f;
    float altoDisponible = 830.f;
    float inicioY = (altoDisponible - altoTablero) / 2.f;

    botones.clear();
    for (int i = 0; i < mapa.columnas; i++) {
        for (int j = 0; j < mapa.filas; j++) {
            Sprite boton(texturaBloque);
            Vector2u tam = texturaBloque.getSize();
            boton.setScale({tamCuadrado / tam.x, tamCuadrado / tam.y});
            boton.setPosition({inicioX + i * tamCuadrado, inicioY + j * tamCuadrado});
            botones.push_back(boton);
        }
    }

    uniform_int_distribution<int> distBomb(22, 32);
    cantBombas = distBomb(gen);
    textoCantidadBombas->setString("CANTIDAD DE BOMBAS: " + to_string(cantBombas));
    textoNumero.emplace(fontNum, "", 36);

    tiempoLimite = 300.f;
    if (dynamic_cast<Estabilizador*>(personaje) != nullptr) {
        tiempoLimite *= 0.75f;
    }

    Vector2u tamBomba = texturasBombas[0].getSize();
    float escalaX = tamCuadrado / tamBomba.x;
    float escalaY = tamCuadrado / tamBomba.y;

    bandera.emplace(texturaBandera);
    bandera->setScale({escalaX, escalaY});

    hud.emplace(texturaHud);
    Vector2u tamHud = texturaHud.getSize();
    hud->setScale({1920.f / tamHud.x, 250.f / tamHud.y});
    hud->setPosition({0, 830.f});

    spritePersonajeHud.emplace(personaje->getTextura(HUD));
    Vector2u tamPjHud = personaje->getTextura(HUD).getSize();
    float anchoMarco = 94.f;
    float altoMarco = 120.f;
    spritePersonajeHud->setScale({anchoMarco / tamPjHud.x, altoMarco / tamPjHud.y});
    spritePersonajeHud->setPosition({153.f, 829.f});

    bombaHud.emplace(texturasBombas[0]);
    bombaHud->setScale({escalaX * 0.7f, escalaY * 0.7f});
    bombaHud->setPosition(Vector2f(1150, tamVentana.y - 150));

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

    textoHablidad.emplace(font, personaje->getTexto(HABILIDAD), 28);
    textoHablidad->setPosition(Vector2f(150.f, tamVentana.y - 105.f));
    textoHablidad->setFillColor(personaje->getColor(colorHabilidad));

    textoPasiva.emplace(font, personaje->getTexto(PASIVA), 28);
    textoPasiva->setPosition(Vector2f(150.f, tamVentana.y - 60.f));
    textoPasiva->setFillColor(personaje->getColor(colorPasiva));

    textoStatus.emplace(font, "ESTADO", 36);
    textoStatus->setPosition(Vector2f(800, tamVentana.y - 240));

    textoCommands.emplace(font, "COMANDOS", 36);
    textoCommands->setPosition(Vector2f(1280, tamVentana.y - 240));

    vecTextoCommands.clear();
    string comandos[4] = {"E - Habilidad", "B - Bandera", "R - Revelar", "WASD - Mover"};
    for (int i = 0; i < 4; i++) {
        Text textocomandos(font, comandos[i], 36);
        textocomandos.setPosition(Vector2f(1280, tamVentana.y - 190 + 40 * i));
        vecTextoCommands.push_back(textocomandos);
    }
    return true;
}

void Juego::sincronizarSpritesBombas() {

    bombas.clear();
    for (int c = 0; c < mapa.columnas; c++) {
    for (int f = 0; f < mapa.filas; f++) {
        if (mapa.tablero[c][f].tipoMina != NINGUNA) {
            int idxTex = mapa.tablero[c][f].tipoMina - 1;

            Sprite sprBomba(texturasBombas[idxTex]);
            Vector2u tam = texturasBombas[idxTex].getSize();
            sprBomba.setScale({tamCuadrado / tam.x, tamCuadrado / tam.y});
            sprBomba.setPosition(botones[c * mapa.filas + f].getPosition());

            bombas.push_back(sprBomba);
        }
    }
    }
}

void Juego::destapar(int col, int fila) {

    Casilla& cas = mapa.tablero[col][fila];
    if (cas.revelada || cas.tieneBandera) return;

    if (cas.tipoMina != NINGUNA) {
        cas.revelada = true;

        if (cas.tipoMina == FUEGO) {
            for (int dc = -1; dc <= 1; dc++) {
                for (int df = -1; df <= 1; df++) {
                    int c = col + dc, f = fila + df;
                    if (c >= 0 && c < mapa.columnas && f >= 0 && f < mapa.filas) {
                        mapa.tablero[c][f].tiempoEncierro = 15.f;
                        mapa.tablero[c][f].tiempoFuego = 35.f;
                    }
                }
            }
        }
        else if (cas.tipoMina == HIELO) {
            if (dynamic_cast<Estabilizador*>(personaje) == nullptr) {
                tiempoCongelado = 10.f;
            }
        }

        personaje->procesarImpactoMina(cas.tipoMina);

        relojDanio.restart();
        mostrandoDanio = true;

        if (personaje->getVida() <= 0) {
            personaje->setVida(0);
            perdio = true;
            tiempoFinal = tiempoAcumulado;
            for (int c = 0; c < mapa.columnas; c++) {
                for (int f = 0; f < mapa.filas; f++) {
                    if (mapa.tablero[c][f].tipoMina != NINGUNA) {
                        mapa.tablero[c][f].revelada = true;
                    }
                }
            }
        }
    }
    else {

        puntajeActual += 2;
        bool recogio = false;
        mapa.destaparRecursivo(fila, col, puntajeActual, tiempoAcumulado, recogio);

        if (mapa.todasSegurasReveladas()) {
            gano = true;
            tiempoFinal = tiempoAcumulado;
            puntajeActual += 100 + static_cast<int>(max(0.f, tiempoLimite - tiempoAcumulado));
            GestorArchivos::actualizarRanking(0, nombreJugador, puntajeActual, static_cast<int>(tiempoFinal), nivelActual);
        }
    }
}

void Juego::ejecutarHabilidadTactica() {
    if (dynamic_cast<Desactivador*>(personaje) != nullptr) {
        bool desactivoUna = false;
        for (int dc = -2; dc <= 2; dc++) {
            for (int df = -2; df <= 2; df++) {
                int c = cursorCol + dc, f = cursorFila + df;
                if (c < 0 || c >= mapa.columnas || f < 0 || f >= mapa.filas) continue;
                Casilla& cas = mapa.tablero[c][f];
                cas.valorMostrado = cas.minasAlrededor;
                if (!desactivoUna && cas.tipoMina != NINGUNA && !cas.tieneBandera) {
                    cas.tieneBandera = true;
                    desactivoUna = true;
                }
            }
        }
    }
    else if (dynamic_cast<Estabilizador*>(personaje) != nullptr) {
        for (int dc = -2; dc <= 2; dc++) {
            for (int df = -2; df <= 2; df++) {
                int c = cursorCol + dc, f = cursorFila + df;
                if (c >= 0 && c < mapa.columnas && f >= 0 && f < mapa.filas) {
                    mapa.tablero[c][f].tiempoEncierro = 0.f;
                    mapa.tablero[c][f].tiempoFuego = 0.f;
                }
            }
        }
    }
    else if (dynamic_cast<Cartografa*>(personaje) != nullptr) {
        for (int dc = -1; dc <= 1; dc++) {
            for (int df = -1; df <= 1; df++) {
                int c = cursorCol + dc, f = cursorFila + df;
                if (c < 0 || c >= mapa.columnas || f < 0 || f >= mapa.filas) continue;
                Casilla& cas = mapa.tablero[c][f];
                if (cas.revelada) continue;
                if (cas.tipoMina != NINGUNA) {
                    cas.tieneBandera = true;
                }
                else {
                    bool dummy = false;
                    mapa.destaparRecursivo(f, c, puntajeActual, tiempoAcumulado, dummy);
                }
            }
        }
    }
}

void Juego::iniciarPartida(RenderWindow& window, const string& usuario) {
    nombreJugador = usuario;
    if (!cargarRecursos()) return;
    personaje = seleccionPersonaje(window);
    if (personaje == nullptr) return;
    if (!inicializarTablero(window)) return;
    gameLoop(window);
}

void Juego::gameLoop(RenderWindow& window) {
    bool esDesactivador  = (dynamic_cast<Desactivador*>(personaje) != nullptr);
    bool esEstabilizador = (dynamic_cast<Estabilizador*>(personaje) != nullptr);
    Cartografa* cartografaPtr = dynamic_cast<Cartografa*>(personaje);
    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (perdio || gano) {
                    continue;
                }

                if (keyPressed->code == Keyboard::Key::Escape) {
                    window.setMouseCursorVisible(true);
                    return;
                }

                if (personajeColocado) {
                    bool seMovio = false;
                    Casilla& casActual = mapa.tablero[cursorCol][cursorFila];
                    bool atrapadoEnFuego = (casActual.tiempoEncierro > 0.f);
                    if (!atrapadoEnFuego) {
                        if (keyPressed->code == Keyboard::Key::W && cursorFila > 0) {
                            if (mapa.tablero[cursorCol][cursorFila - 1].tiempoFuego <= 0.f) { cursorFila--; seMovio = true; }
                        }
                        if (keyPressed->code == Keyboard::Key::S && cursorFila < mapa.filas - 1) {
                            if (mapa.tablero[cursorCol][cursorFila + 1].tiempoFuego <= 0.f) { cursorFila++; seMovio = true; }
                        }
                        if (keyPressed->code == Keyboard::Key::A && cursorCol > 0) {
                            if (mapa.tablero[cursorCol - 1][cursorFila].tiempoFuego <= 0.f) { cursorCol--; seMovio = true; }
                        }
                        if (keyPressed->code == Keyboard::Key::D && cursorCol < mapa.columnas - 1) {
                            if (mapa.tablero[cursorCol + 1][cursorFila].tiempoFuego <= 0.f) { cursorCol++; seMovio = true; }
                        }
                    }
                    Casilla& nuevaCasilla = mapa.tablero[cursorCol][cursorFila];
                    if (seMovio) {
                        if (nuevaCasilla.revelada && nuevaCasilla.tienePowerUp && !nuevaCasilla.powerUpRecogido) {
                            nuevaCasilla.powerUpRecogido = true;
                            puntajeActual += 30;
                            tiempoAcumulado = max(0.f, tiempoAcumulado - 15.f);
                            personaje->recargarHabilidad();
                            personaje->setArmadura(personaje->getArmadura() + 1);
                        }
                        if (cartografaPtr != nullptr) {
                            turnosRecargaTess++;
                            if (turnosRecargaTess >= 5) {
                                turnosRecargaTess = 0;
                                personaje->recargarHabilidad();
                            }
                            if (cartografaPtr->escanearRuta(mapa, cursorCol, cursorFila)) {
                                textoPasiva->setString("ALERTA: CADENA CERCA");
                                textoPasiva->setFillColor(Color(255, 100, 100));
                            }
                            else {
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
                    if (keyPressed->code == Keyboard::Key::R && nuevaCasilla.tiempoFuego <= 0.f) {
                        destapar(cursorCol, cursorFila);
                    }
                    if (keyPressed->code == Keyboard::Key::B && !nuevaCasilla.revelada && nuevaCasilla.tiempoFuego <= 0.f) {
                        nuevaCasilla.tieneBandera = !nuevaCasilla.tieneBandera;
                        if (nuevaCasilla.tieneBandera && nuevaCasilla.tipoMina == NINGUNA) {
                            if (esDesactivador && banderasErroneasUsadas == 0) {
                                banderasErroneasUsadas++;
                            }
                            else if (esEstabilizador) {
                                puntajeActual = max(0, puntajeActual - 15);
                            }
                        }
                    }
                }
            }

            if (const auto* mouseButton = event->getIf<Event::MouseButtonPressed>()) {
                if (perdio || gano) {
                    if (mouseButton->button == Mouse::Button::Left) {
                        Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);
                        if (botonVolver.getGlobalBounds().contains(mousePos)) {
                            window.setMouseCursorVisible(true);
                            return;
                        }
                    }
                    continue;
                }

                if (mouseButton->button == Mouse::Button::Left && !personajeColocado) {
                    Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);
                    for (size_t i = 0; i < botones.size(); i++) {
                        if (botones[i].getGlobalBounds().contains(mousePos)) {
                            cursorCol = i / mapa.filas;
                            cursorFila = i % mapa.filas;
                            personajeColocado = true;
                            window.setMouseCursorVisible(false);
                            reloj.restart();
                            relojDelta.restart();
                            mapa.generarProcedural(cursorCol, cursorFila, cantBombas, gen);
                            sincronizarSpritesBombas();
                            destapar(cursorCol, cursorFila);
                            break;
                        }
                    }
                }
            }
        }

        float dt = relojDelta.restart().asSeconds();
        if (!perdio && !gano && personajeColocado) {
            mapa.actualizarFuego(dt);
            float mult = 1.f;
            if (tiempoCongelado > 0.f) {
                tiempoCongelado -= dt;
                mult = 2.f;
                textoTimer->setFillColor(Color::Cyan);
            }
            else {
                textoTimer->setFillColor(Color::White);
            }
            tiempoAcumulado += dt * mult;
            int restante = max(0, static_cast<int>(tiempoLimite - tiempoAcumulado));
            int minutos = restante / 60;
            int segundos = restante % 60;
            if(segundos<10){
                textoTimer->setString("TIEMPO: " + to_string(minutos) + ":0" + to_string(segundos));
            } else {
                textoTimer->setString("TIEMPO: " + to_string(minutos) + ":" + to_string(segundos));
            }
            if (restante <= 0) {
                perdio = true;
                tiempoFinal = tiempoAcumulado;
            }
        }

        if (perdio || gano) {
            window.setMouseCursorVisible(true);
            Vector2f posMouse = window.mapPixelToCoords(Mouse::getPosition(window));
            if (botonVolver.getGlobalBounds().contains(posMouse)) {
                botonVolver.setFillColor(Color(85, 100, 125));
            } else {
                botonVolver.setFillColor(Color(50, 60, 75));
            }
        }
        textoArmaduraActual->setString(to_string(personaje->getArmadura()));
        textoVidaActual->setString(to_string(personaje->getVida()));
        textoHablidad->setString(personaje->getTexto(HABILIDAD) + " (" + to_string(personaje->getUsosRestantes()) + ")");
        textoCantidadBombas->setString("CANTIDAD DE BOMBAS: " + to_string(mapa.contarBombasRestantes(cantBombas)));
        textoPuntajeHud->setString("PUNTAJE: " + to_string(puntajeActual));
        int nivelColor = min(5, puntajeActual / 100);
        uint8_t brillo = static_cast<uint8_t>(90 + nivelColor * 33);
        uint8_t opacidad = static_cast<uint8_t>(170 + nivelColor * 17);
        textoPuntajeHud->setFillColor(Color(brillo, brillo, brillo, opacidad));

        window.clear(Color(24, 33, 43));
        window.draw(*fondo);
        window.draw(*hud);
        window.draw(*spritePersonajeHud);
        window.draw(*bombaHud);
        dibujarConSombra(window, *textoTimer, 3.f);
        dibujarConSombra(window, *textoCantidadBombas, 3.f);
        dibujarConSombra(window, *textoPuntajeHud, 3.f);
        window.draw(*vidaHud);
        dibujarConSombra(window, *textoVidaActual, 3.f);
        dibujarConSombra(window, *textoStatus, 3.f);
        dibujarConSombra(window, *textoCommands, 3.f);
        dibujarConSombra(window, *textoArmaduraActual, 3.f);
        window.draw(*armaduraHud);

        for (size_t i = 0; i < vecTextoCommands.size(); i++) {
            window.draw(vecTextoCommands[i]);
        }
        dibujarConSombra(window, *textoPersonaje, 3.f);
        dibujarConSombra(window, *textoHablidad, 3.f);
        dibujarConSombra(window, *textoPasiva, 3.f);
        for (const auto& b : bombas) window.draw(b);
        for (int c = 0; c < mapa.columnas; c++) {
            for (int f = 0; f < mapa.filas; f++) {
                const Casilla& cas = mapa.tablero[c][f];
                int valNum = esDesactivador ? cas.minasAlrededor : cas.valorMostrado;
                if (cas.tipoMina != NINGUNA || valNum == 0) continue;
                    int idx = c * mapa.filas + f;
                    textoNumero->setString(to_string(valNum));
                    if (valNum == 1) textoNumero->setFillColor(Color::Blue);
                    else if (valNum == 2) textoNumero->setFillColor(Color::Green);
                    else if (valNum == 3) textoNumero->setFillColor(Color::Red);
                    else textoNumero->setFillColor(Color::Yellow);
                    FloatRect lb = textoNumero->getLocalBounds();
                    Vector2f posNum = botones[idx].getPosition();
                    textoNumero->setPosition(Vector2f(
                    posNum.x + (tamCuadrado - lb.size.x) / 2.f - lb.position.x,
                    posNum.y + (tamCuadrado - lb.size.y) / 2.f - lb.position.y));
                    bool estaRadiado = (cas.valorMostrado != cas.minasAlrededor);
                    if (esDesactivador && estaRadiado) {
                        textoNumero->setOutlineColor(Color(50, 255, 100));
                        textoNumero->setOutlineThickness(2.2f);
                    }
                    else if (estaRadiado) {
                        uint8_t pulso = static_cast<uint8_t>(120 + 135 * sin(tiempoAcumulado * 7.f));
                        textoNumero->setOutlineColor(Color(40, pulso, 40));
                        textoNumero->setOutlineThickness(2.2f);
                    }
                    else {
                        textoNumero->setOutlineColor(Color::Black);
                        textoNumero->setOutlineThickness(1.5f);
                    }
                    window.draw(*textoNumero);
            }
        }
        for (int c = 0; c < mapa.columnas; c++) {
            for (int f = 0; f < mapa.filas; f++) {
                const Casilla& cas = mapa.tablero[c][f];
                int idx = c * mapa.filas + f;
                if (cas.tienePowerUp && cas.revelada && !cas.powerUpRecogido) {
                    CircleShape orbe(tamCuadrado * 0.22f);
                    orbe.setFillColor(Color(255, 215, 0));
                    orbe.setOutlineColor(Color(255, 120, 0));
                    orbe.setOutlineThickness(2.f);
                    Vector2f pos = botones[idx].getPosition();
                    orbe.setPosition({pos.x + tamCuadrado * 0.28f, pos.y + tamCuadrado * 0.28f});
                    window.draw(orbe);
                }
                if (cas.tiempoFuego > 0.f) {
                    botones[idx].setColor(Color(255, 65, 30, 255));
                    window.draw(botones[idx]);
                }
                else if (!cas.revelada) {
                    botones[idx].setColor(Color::White);
                    window.draw(botones[idx]);
                }
                if (cas.tieneBandera) {
                    bandera->setPosition(botones[idx].getPosition());
                    window.draw(*bandera);
                }
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
            spritePersonaje->setTexture(personaje->getTextura(ANIMDANIO));
            spritePersonaje->setTextureRect(IntRect({frame * 64, 0}, {64, 64}));
            spritePersonaje->setOrigin({32.f, 60.f});
        }
        else if (mostrandoHabilidad) {
            spritePersonaje->setTexture(personaje->getTextura(ANIMHABILIDAD));
            spritePersonaje->setTextureRect(IntRect({frame * 96, 0}, {96, 96}));
            spritePersonaje->setOrigin({48.f, 84.f});
        }
        else {
            frame = (int)(relojAnim.getElapsedTime().asSeconds() / 0.5f) % 2;
            spritePersonaje->setTexture(personaje->getTextura(IDLE));
            spritePersonaje->setTextureRect(IntRect({frame * 64, 0}, {64, 64}));
            spritePersonaje->setOrigin({32.f, 60.f});
        }
        if (personajeColocado) {
            int idxPj = cursorCol * mapa.filas + cursorFila;
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
            textoPuntuacion->setString("Puntuacion total: " + to_string(puntajeActual));
            textoPuntuacion->setPosition(Vector2f(
            (window.getView().getSize().x - textoPuntuacion->getLocalBounds().size.x) / 2.f,window.getView().getSize().y / 2.f + 50));
            window.draw(capaOscura);
            window.draw(*textoGameOver);
            window.draw(*textoTimer);
            window.draw(*textoPuntuacion);
            window.draw(botonVolver);
            window.draw(*textoVolver);
        }
        if (gano) {
            textoPuntuacion->setString("Puntuacion total: " + to_string(puntajeActual));
            textoPuntuacion->setPosition(Vector2f(
            (window.getView().getSize().x - textoPuntuacion->getLocalBounds().size.x) / 2.f,
            window.getView().getSize().y / 2.f + 50));
            window.draw(capaOscura);
            window.draw(*textoVictoria);
            window.draw(*textoTimer);
            window.draw(*textoPuntuacion);
            window.draw(botonVolver);
            window.draw(*textoVolver);
        }
        window.display();
    }
}
