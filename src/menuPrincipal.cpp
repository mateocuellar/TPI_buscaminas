#include "MenuPrincipal.h"

using namespace std;
using namespace sf;

menuPrincipal::menuPrincipal() {

}

bool menuPrincipal::cargarRecursos() {
    if (!texturaFondo.loadFromFile("img/fondo_menu.png")) return false;
    if (!texturaHoja.loadFromFile("img/particulas/hoja1.png")) return false;
    if (!texturaPolvo.loadFromFile("img/particulas/polvo_grande.png")) return false;
    return font.openFromFile("fuente.TTF");
}

void menuPrincipal::crearParticulas(RenderWindow& window) {
    Vector2u tamVentana = window.getSize();

    random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<float> posX(0.f, static_cast<float>(tamVentana.x));
    uniform_real_distribution<float> posY(0.f, static_cast<float>(tamVentana.y));

    uniform_real_distribution<float> velX(20.f, 80.f);
    uniform_real_distribution<float> velY(10.f, 40.f);

    uniform_real_distribution<float> rotacion(-90.f, 90.f);

    uniform_real_distribution<float> escalaHoja(0.15f, 0.35f);
    uniform_real_distribution<float> escalaPolvo(0.05f, 0.12f);

    particulas.clear();

    for (int i = 0; i < 25; i++) {
        Particula p(i % 2 == 0 ? texturaHoja : texturaPolvo);

        Vector2u tamTextura = texturaHoja.getSize();

        p.sprite.setOrigin(Vector2f(
            tamTextura.x / 2.f,
            tamTextura.y / 2.f
        ));

        float escala = escalaHoja(gen);

        p.sprite.setScale(Vector2f(escala*3, escala*3));

        p.sprite.setPosition(Vector2f(
            posX(gen),
            posY(gen)
        ));

        p.velocidad = Vector2f(
            velX(gen),
            velY(gen)
        );

        p.velRotacion = rotacion(gen);

        particulas.push_back(p);
    }
}

void menuPrincipal::actualizarParticulas(float dt, RenderWindow& window) {
    Vector2u tamVentana = window.getSize();

    static random_device rd;
    static mt19937 gen(rd());

    uniform_real_distribution<float> nuevaX(0.f, static_cast<float>(tamVentana.x));

    for (size_t i = 0; i < particulas.size(); i++) {
        particulas[i].sprite.move(particulas[i].velocidad * dt);
        particulas[i].sprite.rotate(degrees(particulas[i].velRotacion * dt));

        Vector2f pos = particulas[i].sprite.getPosition();

        if (pos.x > tamVentana.x + 50.f) {
            particulas[i].sprite.setPosition(Vector2f(-50.f, pos.y));
        }
        if (pos.y > tamVentana.y + 50.f) {
            particulas[i].sprite.setPosition(Vector2f(nuevaX(gen), -50.f));
        }
    }
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

    btnSalir.emplace(Vector2f(anchoBoton/2-30, altoBoton*0.8));
    btnSalir->setPosition(Vector2f(0, 0));
    btnSalir->setFillColor(Color(200,0,0, 100));

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

    textoSalir.emplace(font, "Salir", 28);
    textoSalir->setFillColor(Color(255,255,255,200));
    FloatRect boundsSalir= textoSalir->getGlobalBounds();
    textoSalir->setPosition(Vector2f(
        (boundsSalir.size.x) / 2.f,
        btnSalir->getPosition().y + (altoBoton*0.8 - boundsSalir.size.y) / 2.f - boundsSalir.position.y
    ));
}

void menuPrincipal::ejecutar(RenderWindow& window) {
    if (!cargarRecursos()) return;
    inicializarComponentes(window);
    crearParticulas(window);

    Color colorBtn(255, 107, 53);
    Color colorHover(210, 75, 30);
    Color colorBorde(255, 180, 100);

    vector<RectangleShape*> botones;

    botones.push_back(&*btnLogin);
    botones.push_back(&*btnRegister);
    botones.push_back(&*btnPlay);
    botones.push_back(&*btnRanking);

    while (window.isOpen()) {
        float dt = relojParticulas.restart().asSeconds();
        actualizarParticulas(dt, window);

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

                    if (btnSalir->getGlobalBounds().contains(mousePos)) {
                        window.close();
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

        for (size_t i = 0; i < particulas.size(); i++) {
            window.draw(particulas[i].sprite);
        }

        window.draw(*btnLogin);
        window.draw(*btnRegister);
        window.draw(*btnPlay);
        window.draw(*btnRanking);
        window.draw(*btnSalir);

        window.draw(*sombraTitulo);
        window.draw(*sombraTitulo2);

        window.draw(*titulo);
        window.draw(*titulo2);

        window.draw(*textoLogin);
        window.draw(*textoRegister);
        window.draw(*textoNuevaPartida);
        window.draw(*textoRanking);
        window.draw(*textoSalir);

        window.display();
    }
}
