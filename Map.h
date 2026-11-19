#ifndef MAP_H
#define MAP_H

#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>

enum TipoMina { NINGUNA = 0, COMUN, FUEGO, HIELO, RADIACION };

class Casilla {
public:
    int tipoMina;
    int minasAlrededor;
    bool revelada;
    bool tieneBandera;

    float tiempoEncierro;
    float tiempoFuego;
    int valorMostrado;
    bool tienePowerUp;
    bool powerUpRecogido;

    Casilla() : tipoMina(NINGUNA), tiempoEncierro(0.f), tiempoFuego(0.f), minasAlrededor(0), revelada(false),
                tieneBandera(false), valorMostrado(0),
                tienePowerUp(false), powerUpRecogido(false) {}
};

class Map {
public:
    int filas;
    int columnas;
    std::vector<std::vector<Casilla>> tablero;

    Map(int cols = 21, int fils = 11);

    void generarProcedural(int colSegura, int filaSegura, int cantBombas, std::mt19937& gen);
    void calcularProximidad();
    void destaparRecursivo(int f, int c, int& puntos, float& tiempoAcum, bool& recogioPowerUp);

    void aplicarRadiacion(std::mt19937& gen);
    void actualizarFuego(float dt);
    bool todasSegurasReveladas() const;
    int contarBombasRestantes(int totalBombas) const;
};

#endif
