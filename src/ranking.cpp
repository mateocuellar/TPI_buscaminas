#include "ranking.h"
#include <fstream>
#include <algorithm>
#include <cstring>
#include <iostream>

using namespace std;
using namespace sf;

ranking::ranking() : textoTitulo(font), textoCabecera(font), etiquetaVolver(font) {

}


bool ranking::cargarRecursos() {
    return font.openFromFile("fuente.TTF");
}

void ranking::leerYProcesarRanking() {
    listaPuntajes.clear();
    ifstream archivo("Ranking.dat", std::ios::binary);
    if (!archivo.is_open()) return;

    vector<RegistroRankingBinario> todosLosRegistros;
    RegistroRankingBinario aux;


    while (archivo.read(reinterpret_cast<char*>(&aux), sizeof(RegistroRankingBinario))) {
        todosLosRegistros.push_back(aux);
    }
    archivo.close();


    vector<RegistroRankingBinario> filtrados;
    for (const auto& reg : todosLosRegistros) {
        bool encontrado = false;
        for (auto& f : filtrados) {
            if (f.idUsuario == reg.idUsuario) {
                encontrado = true;

                if (reg.tiempoSegundos < f.tiempoSegundos) {
                    f = reg;
                }
                break;
            }
        }
        if (!encontrado) {
            filtrados.push_back(reg);
        }
    }



    for (size_t i = 0; i < filtrados.size(); i++) {
        for (size_t j = i + 1; j < filtrados.size(); j++) {
            if (filtrados[i].tiempoSegundos > filtrados[j].tiempoSegundos) {
                swap(filtrados[i], filtrados[j]);
            }
        }
    }


    for (size_t i = 0; i < filtrados.size() && i < 10; i++) {
        listaPuntajes.push_back(filtrados[i]);
    }
}

void ranking::inicializarComponentes(RenderWindow& window) {
    Vector2u tamVentana = window.getSize();
    float centroX = tamVentana.x / 2.f;


    textoTitulo.setString("MEJORES TIEMPOS - RANKING");
    textoTitulo.setCharacterSize(40);
    textoTitulo.setPosition(Vector2f((tamVentana.x - textoTitulo.getGlobalBounds().size.x) / 2.f, 60.f));


    float anchoTabla = 700.f;
    float inicioTablaX = centroX - (anchoTabla / 2.f);
    float inicioTablaY = 160.f;
    float altoFila = 40.f;

    float colPos[] = { 60.f, 320.f, 520.f, 700.f };


    textoCabecera.setString("POS");
    textoCabecera.setCharacterSize(20);
    textoCabecera.setFillColor(Color(255, 107, 53));

    textoCabecera.setPosition(Vector2f(
        inicioTablaX + (colPos[0] - textoCabecera.getGlobalBounds().size.x) / 2.f,
        inicioTablaY + (altoFila - textoCabecera.getGlobalBounds().size.y) / 2.f - textoCabecera.getGlobalBounds().position.y
    ));


    static Text capUsuario(font, "USUARIO", 20);
    capUsuario.setFillColor(Color(255, 107, 53));
    capUsuario.setPosition(Vector2f(
        inicioTablaX + colPos[0] + ((colPos[1] - colPos[0]) - capUsuario.getGlobalBounds().size.x) / 2.f,
        textoCabecera.getPosition().y
    ));

    static Text capTiempo(font, "TIEMPO (SEG)", 20);
    capTiempo.setFillColor(Color(255, 107, 53));
    capTiempo.setPosition(Vector2f(
        inicioTablaX + colPos[1] + ((colPos[2] - colPos[1]) - capTiempo.getGlobalBounds().size.x) / 2.f,
        textoCabecera.getPosition().y
    ));

    static Text capPuntaje(font, "PUNTAJE", 20);
    capPuntaje.setFillColor(Color(255, 107, 53));
    capPuntaje.setPosition(Vector2f(
        inicioTablaX + colPos[2] + ((colPos[3] - colPos[2]) - capPuntaje.getGlobalBounds().size.x) / 2.f,
        textoCabecera.getPosition().y
    ));


    filasRanking.clear();


    filasRanking.push_back(capUsuario);
    filasRanking.push_back(capTiempo);
    filasRanking.push_back(capPuntaje);

    size_t cantidadFilas = listaPuntajes.size();

    for (size_t i = 0; i < cantidadFilas; i++) {
        float filaY = inicioTablaY + ((i + 1) * altoFila);


        Text txtPos(font, "#" + to_string(i + 1), 18);
        txtPos.setFillColor(Color::White);
        txtPos.setPosition(Vector2f(
            inicioTablaX + (colPos[0] - txtPos.getGlobalBounds().size.x) / 2.f,
            filaY + (altoFila - txtPos.getGlobalBounds().size.y) / 2.f - txtPos.getGlobalBounds().position.y
        ));
        filasRanking.push_back(txtPos);


        Text txtUser(font, string(listaPuntajes[i].nombreUsuario), 18);
        txtUser.setFillColor(Color::White);
        txtUser.setPosition(Vector2f(
            inicioTablaX + colPos[0] + ((colPos[1] - colPos[0]) - txtUser.getGlobalBounds().size.x) / 2.f,
            filaY + (altoFila - txtUser.getGlobalBounds().size.y) / 2.f - txtUser.getGlobalBounds().position.y
        ));
        filasRanking.push_back(txtUser);


        Text txtTiempo(font, to_string(listaPuntajes[i].tiempoSegundos) + "s", 18);
        txtTiempo.setFillColor(Color::White);
        txtTiempo.setPosition(Vector2f(
            inicioTablaX + colPos[1] + ((colPos[2] - colPos[1]) - txtTiempo.getGlobalBounds().size.x) / 2.f,
            filaY + (altoFila - txtTiempo.getGlobalBounds().size.y) / 2.f - txtTiempo.getGlobalBounds().position.y
        ));
        filasRanking.push_back(txtTiempo);


        Text txtPuntos(font, to_string(listaPuntajes[i].puntaje), 18);
        txtPuntos.setFillColor(Color::White);
        txtPuntos.setPosition(Vector2f(
            inicioTablaX + colPos[2] + ((colPos[3] - colPos[2]) - txtPuntos.getGlobalBounds().size.x) / 2.f,
            filaY + (altoFila - txtPuntos.getGlobalBounds().size.y) / 2.f - txtPuntos.getGlobalBounds().position.y
        ));
        filasRanking.push_back(txtPuntos);
    }


    lineasContenedor.clear();
    float altoTotalTabla = altoFila * (cantidadFilas + 1);


    sf::RectangleShape marco(Vector2f(anchoTabla, altoTotalTabla));
    marco.setPosition(Vector2f(inicioTablaX, inicioTablaY));
    marco.setFillColor(Color::Transparent);
    marco.setOutlineColor(Color(100, 110, 120, 150));
    marco.setOutlineThickness(2.f);
    lineasContenedor.push_back(marco);



    for (size_t i = 1; i <= cantidadFilas; i++) {
        sf::RectangleShape lineah(Vector2f(anchoTabla, 1.f));
        lineah.setPosition(Vector2f(inicioTablaX, inicioTablaY + (i * altoFila)));
        lineah.setFillColor(Color(100, 110, 120, 100));
        lineasContenedor.push_back(lineah);
    }




    for (int i = 0; i < 3; i++) {
        sf::RectangleShape lineav(Vector2f(1.f, altoTotalTabla));
        lineav.setPosition(Vector2f(inicioTablaX + colPos[i], inicioTablaY));
        lineav.setFillColor(Color(100, 110, 120, 100));
        lineasContenedor.push_back(lineav);
    }


    btnVolver.setSize(Vector2f(180.f, 45.f));
    btnVolver.setPosition(Vector2f(centroX - 90.f, 580.f));
    btnVolver.setFillColor(Color(100, 110, 120));

    etiquetaVolver.setString("Volver");
    etiquetaVolver.setCharacterSize(20);
    etiquetaVolver.setPosition(Vector2f(
    btnVolver.getPosition().x + (180.f - etiquetaVolver.getGlobalBounds().size.x) / 2.f,
    btnVolver.getPosition().y + (45.f - etiquetaVolver.getGlobalBounds().size.y) / 2.f - etiquetaVolver.getGlobalBounds().position.y
    ));
}


void ranking::mostrar(RenderWindow& window) {
    if (!cargarRecursos()) return;


    leerYProcesarRanking();
    inicializarComponentes(window);

    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (keyPressed->code == Keyboard::Key::Escape) return;
            }

            if (const auto* mouseButton = event->getIf<Event::MouseButtonPressed>()) {
                if (mouseButton->button == Mouse::Button::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);
                    if (btnVolver.getGlobalBounds().contains(mousePos)) {
                        return;
                    }
                }
            }
        }

        window.clear(Color(24, 33, 43));
        window.draw(textoTitulo);
        window.draw(textoCabecera);


        for (const auto& linea : lineasContenedor) {
            window.draw(linea);
        }

        for (const auto& fila : filasRanking) {
            window.draw(fila);
        }

        window.draw(btnVolver);
        window.draw(etiquetaVolver);
        window.display();
    }
}


bool ranking::registrarPuntaje(int idUser, const string& nombreUser, int puntos, int segundos, int lvl) {
    RegistroRankingBinario nuevoRegistro;
    memset(&nuevoRegistro, 0, sizeof(RegistroRankingBinario));

    nuevoRegistro.idUsuario = idUser;
    strncpy(nuevoRegistro.nombreUsuario, nombreUser.c_str(), 19);
    nuevoRegistro.puntaje = puntos;
    nuevoRegistro.tiempoSegundos = segundos;
    nuevoRegistro.nivel = lvl;

    ofstream archivo("Ranking.dat", std::ios::binary | std::ios::app);
    if (archivo.is_open()) {
        archivo.write(reinterpret_cast<const char*>(&nuevoRegistro), sizeof(RegistroRankingBinario));
        archivo.close();
        return true;
    }
    return false;
}

