#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <algorithm>
#include <numeric>
#include <string>
using namespace std;
using namespace sf;

void partida(RenderWindow& window){
    random_device rd;
    mt19937 gen(rd());
    unsigned int resx = 1920;
    unsigned int resy = 1080;

    Font font;
    font.openFromFile("fuente.TTF");

    // cargar texturas
    Texture texturaBomba;
    if (!texturaBomba.loadFromFile("img/bomba.png")) {
        cout << "Error cargando bomba.png" << endl;
    }
    Texture texturaBandera;
    if (!texturaBandera.loadFromFile("img/bandera.png")) {
        cout << "Error cargando bandera.png" << endl;
    }
    Texture texturaHud;
    if (!texturaHud.loadFromFile("img/hud.png")) {
        cout << "Error cargando hud.png" << endl;
    }
    Texture texturaBloque;
    if (!texturaBloque.loadFromFile("img/bloque.png")) {
        cout << "Error cargando bloque.png" << endl;
        return;
    }
    Texture texturaFondo;
    if (!texturaFondo.loadFromFile("img/fondo.png")) {
        cout << "Error cargando fondo.png" << endl;
        return;
    }

    Sprite fondo(texturaFondo);
    sf::Vector2u tamFondo = texturaFondo.getSize();
    sf::Vector2u tamVentana = window.getSize();

    fondo.setScale(sf::Vector2f(
        static_cast<float>(tamVentana.x) / tamFondo.x,
        static_cast<float>(tamVentana.y) / tamFondo.y
    ));

    Clock reloj;

    Text textoTimer(font, "", 30);
    textoTimer.setPosition(Vector2f(150, resy-100));
    textoTimer.setFillColor(Color::White);

    vector<Sprite> botones;
    int cantCuadradosX = 13;
    int cantCuadradosY = 13;

    float tamCuadrado = 60.f;
    float separacion = 3.f;
    float paso = tamCuadrado + separacion;

    // Calcular el tamaño total del tablero
    float anchoTablero = cantCuadradosX * tamCuadrado
                       + (cantCuadradosX - 1) * separacion;

    float altoTablero = cantCuadradosY * tamCuadrado
                      + (cantCuadradosY - 1) * separacion;

    // Centrar el tablero horizontalmente
    float inicioX = (window.getSize().x - anchoTablero) / 2.f;

    // Centrar verticalmente en el espacio disponible arriba del HUD
    float altoDisponible = 830.f;
    float inicioY = (altoDisponible - altoTablero) / 2.f;

    for (int i = 0; i < cantCuadradosX; i++) {
        for (int j = 0; j < cantCuadradosY; j++) {

            Sprite boton(texturaBloque);

            Vector2u tam = texturaBloque.getSize();

            float escalaX = tamCuadrado / tam.x;
            float escalaY = tamCuadrado / tam.y;

            boton.setScale({escalaX, escalaY});

            boton.setPosition({
                inicioX + i * paso,
                inicioY + j * paso
            });

            botones.push_back(boton);
        }
    }
    vector<bool> bloqueVisible(botones.size(), true);

    vector<bool> banderaActiva(botones.size(), false);

    uniform_int_distribution<int> distBomb(5, 15);
    int cantBombas = distBomb(gen);

    vector<int> indices(botones.size());
    iota(indices.begin(), indices.end(), 0);
    shuffle(indices.begin(), indices.end(), gen);

    vector<int> indicesBombas(indices.begin(), indices.begin() + cantBombas);

    Vector2u tamaniotexturaBomba = texturaBomba.getSize();
    float escalaX = 60.f / tamaniotexturaBomba.x;
    float escalaY = 60.f / tamaniotexturaBomba.y;

    // crear un sprite por cada bomba, en la posición del botón correspondiente
    vector<Sprite> bombas;
    for (int idx : indicesBombas) {
        cout << idx << endl;
        Sprite bomba(texturaBomba);
        bomba.setScale({escalaX, escalaY});
        bomba.setPosition(botones[idx].getPosition());
        bombas.push_back(bomba);
    }

    // un solo sprite de bandera, reutilizado para dibujar todas
    Sprite bandera(texturaBandera);
    bandera.setScale({escalaX, escalaY});
    //sprite hud
    Sprite hud(texturaHud);

    Vector2u tamHud = texturaHud.getSize();

    float escalaHudX = 1920.f / tamHud.x;
    float escalaHudY = 250.f / tamHud.y;

    hud.setScale({escalaHudX, escalaHudY});
    hud.setPosition({0, 830.f});
    // texto
    Text texto(font, "CANTIDAD DE BOMBAS: " + to_string(cantBombas), 30);
    texto.setPosition(Vector2f(150, resy-60));
    Sprite bombaHud(texturaBomba);
    bombaHud.setScale({escalaX*0.7, escalaY*0.7});
    bombaHud.setPosition(Vector2f(550, resy-60));

    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* mouseButton = event->getIf<Event::MouseButtonPressed>()) {
                Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);

                if (mouseButton->button == Mouse::Button::Left) {
                    for (size_t i = 0; i < botones.size(); i++) {
                        if (botones[i].getGlobalBounds().contains(mousePos) && !banderaActiva[i]) {
                            cout << "Se clickeo el boton " << i << endl;

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

                                int offsets[9] = {-11, -10, -9, -1, 0, 1, 9, 10, 11};
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

                                    if (!esBomba) {
                                        bloqueVisible[idx] = false;
                                    }
                                }
                            }
                        }
                    }
                }

                if (mouseButton->button == Mouse::Button::Right) {
                    for (size_t i = 0; i < botones.size(); i++) {
                        if (botones[i].getGlobalBounds().contains(mousePos)) {
                            banderaActiva[i] = !banderaActiva[i]; // toggle
                        }
                    }
                }
            }
        }

        float segundos = reloj.getElapsedTime().asSeconds();
        textoTimer.setString("TIEMPO: " + to_string((int)segundos));

        // acá dibujás tus objetos
        window.clear(Color(24, 33, 43));
        window.draw(fondo);
        window.draw(hud);
        window.draw(textoTimer);
        window.draw(texto);
        window.draw(bombaHud);

        for (const auto& b : bombas) {
            window.draw(b);
        }
        for (size_t i = 0; i < botones.size(); i++) {
            if (bloqueVisible[i]) {
                window.draw(botones[i]);
            }
        }

        // dibujar banderas encima de los botones que la tienen activa
        for (size_t i = 0; i < botones.size(); i++) {
            if (banderaActiva[i]) {
                bandera.setPosition(botones[i].getPosition());
                window.draw(bandera);
            }
        }

        window.display();
    }
}
void gameover(){

}


int main() {
    random_device rd;
    mt19937 gen(rd());

    unsigned int resx = 1920;
    unsigned int resy = 1080;

    RenderWindow window(VideoMode::getDesktopMode(), "Buscaminas", State::Fullscreen);
    Font font;
    font.openFromFile("fuente.TTF");

    RectangleShape rectangle(Vector2f(250.f, 50.f));
    rectangle.setPosition(Vector2f((resx - 250) / 2.f, 300.f));
    rectangle.setFillColor(Color(255, 107, 53));

    RectangleShape rectangle2(Vector2f(250.f, 50.f));
    rectangle2.setPosition(Vector2f((resx - 250) / 2.f, 390.f));
    rectangle2.setFillColor(Color(255, 107, 53));

    Text titulo(font, "MINESWEEPER: ELEMENTAL TACTICS", 48);
    titulo.setPosition(Vector2f(
        (resx - titulo.getLocalBounds().size.x) / 2.f,
        210.f
    ));

    Text textoNuevaPartida(font, "Nueva partida", 24);
    textoNuevaPartida.setPosition(Vector2f(
        (resx - textoNuevaPartida.getLocalBounds().size.x) / 2.f,
        310.f
    ));
    Text textoRanking(font, "Ranking", 24);
    textoRanking.setPosition(Vector2f(
        (resx - textoRanking.getLocalBounds().size.x) / 2.f,
        300.f + 100
    ));


    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* mouseButton = event->getIf<Event::MouseButtonPressed>()) {
                if (mouseButton->button == Mouse::Button::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);
                    if (rectangle.getGlobalBounds().contains(mousePos)) {
                        cout << "jugar" << endl;
                        partida(window);
                    }
                }
            }
        }

        window.clear(Color(24, 33, 43));
        window.draw(rectangle);
        window.draw(rectangle2);
        window.draw(textoNuevaPartida);
        window.draw(textoRanking);
        window.draw(titulo);
        window.display();
    }

    return 0;
}
