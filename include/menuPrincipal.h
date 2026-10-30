#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include "Partida.h"
#include "registroUsuario.h"

class menuPrincipal {
private:
    sf::Font font;


    std::optional<sf::RectangleShape> btnLogin;
    std::optional<sf::RectangleShape> btnRegister;
    std::optional<sf::RectangleShape> btnPlay;
    std::optional<sf::RectangleShape> btnRanking;

    std::optional<sf::Text> sombraTitulo;
    std::optional<sf::Text> sombraTitulo2;
    std::optional<sf::Text> titulo;
    std::optional<sf::Text> titulo2;
    std::optional<sf::Text> textoLogin;
    std::optional<sf::Text> textoRegister;
    std::optional<sf::Text> textoNuevaPartida;
    std::optional<sf::Text> textoRanking;

    sf::Texture texturaFondo;
    std::optional<sf::Sprite> fondo;

    bool cargarRecursos();
    void inicializarComponentes(sf::RenderWindow& window);

public:
    menuPrincipal();
    void ejecutar(sf::RenderWindow& window);
};

#endif
