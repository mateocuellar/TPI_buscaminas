#ifndef PARTIDA_H
#define PARTIDA_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <optional>

class Personaje;
using namespace std;
using namespace sf;

class Partida {
private:
    mt19937 gen;


    Font font;
    Texture texturaBomba;
    Texture texturaBandera;
    Texture texturaHud;
    Texture texturaBloque;
    Texture texturaFondo;

    Personaje* personaje;
    optional<Text> textoPersonaje;

    optional<Sprite> fondo;
    optional<Sprite> hud;
    optional<Sprite> bandera;
    optional<Sprite> bombaHud;
    optional<Text> textoTimer;
    optional<Text> textoCantidadBombas;


    vector<Sprite> botones;
    vector<Sprite> bombas;
    vector<bool> bloqueVisible;
    vector<bool> banderaActiva;
    vector<int> indicesBombas;

    Clock reloj;
    int cantBombas;

    bool cargarRecursos();
    bool inicializarTablero(RenderWindow& window);

public:
    Partida();
    void ejecutar(RenderWindow& window);
    ~Partida();
};

#endif
