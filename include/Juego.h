#ifndef JUEGO_H
#define JUEGO_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <optional>
#include <cmath>
#include "Map.h"
#include "Character.h"
#include "GestorArchivos.h"

using namespace std;
using namespace sf;

class Juego {
private:
    int puntajeActual;
    int nivelActual;
    Map mapa;
    Character* personaje;

    mt19937 gen;
    Clock relojAnim;
    Clock relojDanio;
    Clock relojDelta;
    Clock reloj;

    bool mostrandoDanio;
    bool mostrandoHabilidad;
    bool perdio;
    bool gano;
    bool personajeColocado;

    string nombreJugador;
    float tiempoFinal;
    float tiempoAcumulado;
    float tiempoLimite;
    float tiempoCongelado;

    int cursorCol;
    int cursorFila;
    float tamCuadrado;
    int cantBombas;

    int banderasErroneasUsadas;
    int turnosRecargaTess;

    Font font;
    Font fontNum;
    std::vector<sf::Texture> texturasBombas;
    Texture texturaBandera;
    Texture texturaHud;
    Texture texturaBloque;
    Texture texturaFondo;
    Texture texturaVida;
    Texture texturaArmadura;

    optional<Sprite> fondo;
    optional<Sprite> hud;
    optional<Sprite> spritePersonajeHud;
    optional<Sprite> bandera;
    optional<Sprite> bombaHud;
    optional<Sprite> vidaHud;
    optional<Sprite> armaduraHud;
    optional<Sprite> spritePersonaje;

    optional<Text> textoPersonaje;
    optional<Text> textoHablidad;
    optional<Text> textoPasiva;
    optional<Text> textoStatus;
    optional<Text> textoCommands;
    vector<Text> vecTextoCommands;
    optional<Text> textoTimer;
    optional<Text> textoCantidadBombas;
    optional<Text> textoVidaActual;
    optional<Text> textoArmaduraActual;
    optional<Text> textoNumero;
    optional<Text> textoGameOver;
    optional<Text> textoVictoria;
    optional<Text> textoPuntuacion;

    RectangleShape capaOscura;
    vector<Sprite> botones;
    vector<Sprite> bombas;

    bool cargarRecursos();
    bool inicializarTablero(RenderWindow& window);
    void sincronizarSpritesBombas();
    void destapar(int col, int fila);
    void ejecutarHabilidadTactica();

public:
    Juego();
    ~Juego();


    void iniciarPartida(RenderWindow& window, const string& usuario);
    void gameLoop(RenderWindow& window);
    void ejecutar(RenderWindow& window, const string& usuario) { iniciarPartida(window, usuario); }
};

#endif
