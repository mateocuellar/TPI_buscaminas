#ifndef REGISTROUSUARIO_H
#define REGISTROUSUARIO_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <optional>

class registroUsuario {
private:
    sf::Font font;

    std::optional<sf::RectangleShape> cajaTextoUsuario;
    std::optional<sf::RectangleShape> cajaTextoPassword;
    std::optional<sf::RectangleShape> btnAceptar;
    std::optional<sf::RectangleShape> btnVolver;

    std::optional<sf::Text> textoTitulo;
    std::optional<sf::Text> textoUsuario;
    std::optional<sf::Text> textoPassword;
    std::optional<sf::Text> textoContenidoUsuario;
    std::optional<sf::Text> textoContenidoPassword;
    std::optional<sf::Text> etiquetaAceptar;
    std::optional<sf::Text> etiquetaVolver;

    std::string stringUsuario;
    std::string stringPassword;

    bool campoSeleccionado;

    bool cargarRecursos();
    void inicializarComponentes(sf::RenderWindow& window, const std::string& tituloPantalla);

public:
    registroUsuario();


    void mostrarLogin(sf::RenderWindow& window);
    void mostrarRegistro(sf::RenderWindow& window);
};

#endif
