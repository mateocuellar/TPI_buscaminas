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

class Partida {
private:
    std::mt19937 gen;


    sf::Font font;
    sf::Texture texturaBomba;
    sf::Texture texturaBandera;
    sf::Texture texturaHud;
    sf::Texture texturaBloque;
    sf::Texture texturaFondo;


    std::optional<sf::Sprite> fondo;
    std::optional<sf::Sprite> hud;
    std::optional<sf::Sprite> bandera;
    std::optional<sf::Sprite> bombaHud;
    std::optional<sf::Text> textoTimer;
    std::optional<sf::Text> textoCantidadBombas;


    std::vector<sf::Sprite> botones;
    std::vector<sf::Sprite> bombas;
    std::vector<bool> bloqueVisible;
    std::vector<bool> banderaActiva;
    std::vector<int> indicesBombas;

    sf::Clock reloj;
    int cantBombas;

    bool cargarRecursos();
    void inicializarTablero(sf::RenderWindow& window);

public:
    Partida();
    void ejecutar(sf::RenderWindow& window);
};

#endif
