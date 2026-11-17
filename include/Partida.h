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
    Clock relojAnim;
    Clock relojDanio;
    bool mostrandoDanio = false;
    bool mostrandoHabilidad = false;

    Font font;
    Font fontNum;
    Texture texturaBomba;
    Texture texturaBandera;
    Texture texturaHud;
    Texture texturaBloque;
    Texture texturaFondo;
    Texture texturaVida;
    Texture texturaArmadura;

    Personaje* personaje;
    optional<Text> textoPersonaje;
    optional<Text> textoHablidad;
    optional<Text> textoPasiva;
    optional<Text> textoStatus;
    optional<Text> textoCommands;
    vector <Text> vecTextoCommands;

    optional<Sprite> fondo;
    optional<Sprite> hud;
    optional<Sprite> bandera;
    optional<Sprite> bombaHud;
    optional<Sprite> vidaHud;
    optional<Sprite> armaduraHud;
    optional<Sprite> spritePersonaje;
    optional<Sprite> spritePersonajeHud;
    optional<Text> textoTimer;
    optional<Text> textoCantidadBombas;
    optional<Text> textoVidaActual;
    optional<Text> textoArmaduraActual;


    vector<Sprite> botones;
    vector<Sprite> bombas;
    vector<bool> bloqueVisible;
    vector<bool> banderaActiva;
    vector<int> indicesBombas;

    bool perdio;
    bool gano;
    std::optional<sf::Text> textoVictoria;
    std::string nombreJugador;

    float tiempoFinal;
    int puntuacion;

    bool personajeColocado;
    int cursorCol;
    int cursorFila;
    int cantCuadradosX;
    int cantCuadradosY;
    float tamCuadrado;

    RectangleShape capaOscura;
    optional<Text> textoGameOver;
    optional<Text> textoPuntuacion;

    vector<int> numeros;
    optional<Text> textoNumero;

    bool esBomba(int idx);
    void calcularNumeros();

    Clock reloj;
    int cantBombas;

    void destaparRecursivo(int col, int fila);
    bool cargarRecursos();
    bool inicializarTablero(RenderWindow& window);
    void destapar(int idx);

public:
    Partida();
    void ejecutar(sf::RenderWindow& window, const std::string& usuario = "Invitado");
    ~Partida();
};

#endif
