#ifndef RANKING_H
#define RANKING_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>


struct RegistroRankingBinario {
    int idPuntaje;
    int idUsuario;
    char nombreUsuario[20];
    int puntaje;
    int tiempoSegundos;
    int nivel;

};

class ranking {
private:
    sf::Font font;
    sf::Text textoTitulo;
    sf::Text textoCabecera;
    std::vector<sf::Text> filasRanking;

    sf::RectangleShape btnVolver;
    sf::Text etiquetaVolver;

    std::vector<RegistroRankingBinario> listaPuntajes;
    std::vector<sf::RectangleShape> lineasContenedor;

    bool cargarRecursos();
    void inicializarComponentes(sf::RenderWindow& window);
    void leerYProcesarRanking();

public:
    ranking();
    void mostrar(sf::RenderWindow& window);


    static bool registrarPuntaje(int idUser, const std::string& nombreUser, int puntos, int segundos, int lvl);
};

#endif
