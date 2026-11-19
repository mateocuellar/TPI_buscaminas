#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include "Juego.h"
#include "registroUsuario.h"

using namespace std;
using namespace sf;

void dibujarConSombra(RenderWindow& window, Text& texto, float desplazamiento);

struct Particula {
    Sprite sprite;
    Vector2f velocidad;
    float velRotacion;

    Particula(const Texture& texturaParticula) : sprite(texturaParticula) {

    }
};



class menuPrincipal {
private:
    Font font;

    optional<RectangleShape> btnLogin;
    optional<RectangleShape> btnRegister;
    optional<RectangleShape> btnIniciarPartida;
    optional<RectangleShape> btnCerrarSesion;
    optional<RectangleShape> btnRanking;
    optional<RectangleShape> btnCreditos;
    optional<RectangleShape> btnSalir;

    optional<Text> titulo;
    optional<Text> titulo2;
    optional<Text> bienvenida;
    optional<Text> textoLogin;
    optional<Text> textoRegister;
    optional<Text> textoIniciarPartida;
    optional<Text> textoCerrarSesion;
    optional<Text> textoRanking;
    optional<Text> textoCreditos;
    optional<Text> textoSalir;
    optional<Text> textoTpi;

    Texture texturaFondo;
    optional<Sprite> fondo;
    optional<Sprite> brillo;

    Texture texturaHoja;
    Texture texturaPolvo;
    Texture texturaBrillo;
    vector<Particula> particulas;

    string usuarioActual = "Invitado";
    bool sesionIniciada = false;

    Clock relojParticulas;
    void crearParticulas(RenderWindow& window);
    void actualizarParticulas(float dt, RenderWindow& window);

    bool cargarRecursos();
    void inicializarComponentes(RenderWindow& window);
    void actualizarBienvenida(RenderWindow& window);
public:
    menuPrincipal();
    void ejecutar(RenderWindow& window);
};

#endif
