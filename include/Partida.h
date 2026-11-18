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
#include <cmath>

class Personaje;
using namespace std;
using namespace sf;

enum TipoMina { NINGUNA = 0, COMUN, FUEGO, HIELO, RADIACION };

class Partida {
private:
    mt19937 gen;
    Clock relojAnim;
    Clock relojDanio;
    Clock relojDelta;
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
    vector<Text> vecTextoCommands;

    optional<Sprite> fondo;
    optional<Sprite> hud;
    optional<Sprite> bandera;
    optional<Sprite> bombaHud;
    optional<Sprite> vidaHud;
    optional<Sprite> armaduraHud;
    optional<Sprite> spritePersonaje;
    optional<Text> textoTimer;
    optional<Text> textoCantidadBombas;
    optional<Text> textoVidaActual;
    optional<Text> textoArmaduraActual;

    vector<Sprite> botones;
    vector<Sprite> bombas;
    vector<bool> bloqueVisible;
    vector<bool> banderaActiva;
    vector<int> indicesBombas;

    vector<int> tipoMina;
    vector<int> turnosFuego;
    vector<int> numerosMostrados;
    vector<bool> tienePowerUp;
    vector<bool> powerUpRecogido;

    bool perdio;
    bool gano;
    std::optional<sf::Text> textoVictoria;
    std::string nombreJugador;

    float tiempoFinal;
    float tiempoAcumulado;
    float tiempoLimite;
    float tiempoCongelado;
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
    void generarMapaSeguro(int idxSeguro);
    void calcularNumeros();
    void aplicarRadiacion();
    void ejecutarHabilidadTactica();
    void actualizarTurnosFuego();

    Clock reloj;
    int cantBombas;

    void destaparRecursivo(int col, int fila);
    bool cargarRecursos();
    bool inicializarTablero(RenderWindow& window);
    void destapar(int idx);

    int banderasErroneasUsadas = 0;
    int turnosRecargaTess = 0;
    bool alertaReaccionCadena = false;


public:
    Partida();
    void ejecutar(sf::RenderWindow& window, const std::string& usuario = "Invitado");
    ~Partida();
};

#endif

