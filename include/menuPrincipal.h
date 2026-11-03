#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include "Partida.h"
#include "registroUsuario.h"

struct Particula {
    sf::Sprite sprite;
    sf::Vector2f velocidad;
    float velRotacion;

    Particula(const sf::Texture& texturaParticula) : sprite(texturaParticula) {

    }
};

class menuPrincipal {
private:
    sf::Font font;


    std::optional<sf::RectangleShape> btnLogin;
    std::optional<sf::RectangleShape> btnRegister;
    std::optional<sf::RectangleShape> btnPlay;
    std::optional<sf::RectangleShape> btnRanking;
    std::optional<sf::RectangleShape> btnSalir;

    std::optional<sf::Text> sombraTitulo;
    std::optional<sf::Text> sombraTitulo2;
    std::optional<sf::Text> titulo;
    std::optional<sf::Text> titulo2;
    std::optional<sf::Text> textoLogin;
    std::optional<sf::Text> textoRegister;
    std::optional<sf::Text> textoNuevaPartida;
    std::optional<sf::Text> textoRanking;
    std::optional<sf::Text> textoSalir;

    sf::Texture texturaFondo;
    std::optional<sf::Sprite> fondo;

    sf::Texture texturaHoja;
    sf::Texture texturaPolvo;
    std::vector<Particula> particulas;
    sf::Clock relojParticulas;

    void crearParticulas(sf::RenderWindow& window);
    void actualizarParticulas(float dt, sf::RenderWindow& window);

    bool cargarRecursos();
    void inicializarComponentes(sf::RenderWindow& window);

public:
    menuPrincipal();
    void ejecutar(sf::RenderWindow& window);
};

#endif
