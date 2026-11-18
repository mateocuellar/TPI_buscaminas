#include "Map.h"

using namespace std;

Map::Map(int cols, int fils) : filas(fils), columnas(cols) {
    tablero.assign(columnas, vector<Casilla>(filas));
}

void Map::generarProcedural(int colSegura, int filaSegura, int cantBombas, mt19937& gen) {
    tablero.assign(columnas, vector<Casilla>(filas));

    vector<pair<int, int>> candidatos;
    for (int c = 0; c < columnas; c++) {
        for (int f = 0; f < filas; f++) {
            if (abs(c - colSegura) <= 1 && abs(f - filaSegura) <= 1) continue;
            candidatos.push_back({c, f});
        }
    }

    shuffle(candidatos.begin(), candidatos.end(), gen);
    int totalColocar = min(cantBombas, static_cast<int>(candidatos.size()));

    uniform_int_distribution<int> distTipo(1, 100);
    for (int i = 0; i < totalColocar; i++) {
        int c = candidatos[i].first;
        int f = candidatos[i].second;
        int roll = distTipo(gen);

        if (roll <= 40) tablero[c][f].tipoMina = COMUN;
        else if (roll <= 60) tablero[c][f].tipoMina = FUEGO;
        else if (roll <= 80) tablero[c][f].tipoMina = HIELO;
        else tablero[c][f].tipoMina = RADIACION;
    }

    for (int i = totalColocar; i < totalColocar + 5 && i < static_cast<int>(candidatos.size()); i++) {
        int c = candidatos[i].first;
        int f = candidatos[i].second;
        tablero[c][f].tienePowerUp = true;
    }

    calcularProximidad();
    aplicarRadiacion(gen);
}

void Map::calcularProximidad() {
    for (int c = 0; c < columnas; c++) {
        for (int f = 0; f < filas; f++) {
            if (tablero[c][f].tipoMina != NINGUNA) continue;

            int cant = 0;
            for (int dc = -1; dc <= 1; dc++) {
                for (int df = -1; df <= 1; df++) {
                    int nc = c + dc;
                    int nf = f + df;
                    if (nc < 0 || nc >= columnas || nf < 0 || nf >= filas) continue;
                    if (tablero[nc][nf].tipoMina != NINGUNA) cant++;
                }
            }
            tablero[c][f].minasAlrededor = cant;
            tablero[c][f].valorMostrado = cant;
        }
    }
}

void Map::aplicarRadiacion(mt19937& gen) {
    uniform_int_distribution<int> distAlterar(0, 1);

    for (int c = 0; c < columnas; c++) {
        for (int f = 0; f < filas; f++) {
            if (tablero[c][f].tipoMina != RADIACION) continue;

            for (int dc = -1; dc <= 1; dc++) {
                for (int df = -1; df <= 1; df++) {
                    int nc = c + dc;
                    int nf = f + df;
                    if (nc < 0 || nc >= columnas || nf < 0 || nf >= filas) continue;
                    if (tablero[nc][nf].tipoMina != NINGUNA || tablero[nc][nf].minasAlrededor == 0) continue;

                    int delta = (distAlterar(gen) == 0) ? -1 : 1;
                    int falso = tablero[nc][nf].minasAlrededor + delta;
                    if (falso < 1) falso = 2;
                    if (falso > 8) falso = 7;
                    tablero[nc][nf].valorMostrado = falso;
                }
            }
        }
    }
}

void Map::destaparRecursivo(int f, int c, int& puntos, float& tiempoAcum, bool& recogioPowerUp) {
    if (c < 0 || c >= columnas || f < 0 || f >= filas) return;

    Casilla& cas = tablero[c][f];
    if (cas.revelada || cas.tieneBandera || cas.tipoMina != NINGUNA) return;

    cas.revelada = true;

    if (cas.minasAlrededor > 0) return;

    for (int dc = -1; dc <= 1; dc++) {
        for (int df = -1; df <= 1; df++) {
            destaparRecursivo(f + df, c + dc, puntos, tiempoAcum, recogioPowerUp);
        }
    }
}

void Map::actualizarFuego(float dt) {
    for (int c = 0; c < columnas; c++) {
        for (int f = 0; f < filas; f++) {
            if (tablero[c][f].tiempoEncierro > 0.f) {
                tablero[c][f].tiempoEncierro = max(0.f, tablero[c][f].tiempoEncierro - dt);
            }
            if (tablero[c][f].tiempoFuego > 0.f) {
                tablero[c][f].tiempoFuego = max(0.f, tablero[c][f].tiempoFuego - dt);
            }
        }
    }
}


bool Map::todasSegurasReveladas() const {
    for (int c = 0; c < columnas; c++) {
        for (int f = 0; f < filas; f++) {
            if (tablero[c][f].tipoMina == NINGUNA && !tablero[c][f].revelada) {
                return false;
            }
        }
    }
    return true;
}

int Map::contarBombasRestantes(int totalBombas) const {
    int restantes = totalBombas;
    for (int c = 0; c < columnas; c++) {
        for (int f = 0; f < filas; f++) {
            if ((tablero[c][f].tipoMina != NINGUNA && tablero[c][f].revelada) || tablero[c][f].tieneBandera) {
                restantes--;
            }
        }
    }
    return max(0, restantes);
}
