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
            if (string(f.nombreUsuario) == string(reg.nombreUsuario)) {
                encontrado = true;
                if (reg.puntaje > f.puntaje || (reg.puntaje == f.puntaje && reg.tiempoSegundos < f.tiempoSegundos)) {
                    f = reg;
                }
                break;
            }
        }
        if (!encontrado) {
            filtrados.push_back(reg);
        }
    }
    sort(filtrados.begin(), filtrados.end(), [](const RegistroRankingBinario& a, const RegistroRankingBinario& b) {
        if (a.puntaje != b.puntaje) return a.puntaje > b.puntaje;
        return a.tiempoSegundos < b.tiempoSegundos;
    });
    for (size_t i = 0; i < filtrados.size() && i < 5; i++) {
        listaPuntajes.push_back(filtrados[i]);
    }
}

void ranking::inicializarComponentes(RenderWindow& window) {
    Vector2u tamVentana = Vector2u(window.getView().getSize());
    float centroX = tamVentana.x / 2.f;
    textoTitulo.setString("TOP 5 HISTÓRICO - JUGADORES");
    textoTitulo.setCharacterSize(56);
    textoTitulo.setFillColor(Color::White);
    textoTitulo.setOutlineColor(Color::Black);
    textoTitulo.setOutlineThickness(3.f);
    textoTitulo.setPosition(Vector2f((tamVentana.x - textoTitulo.getGlobalBounds().size.x) / 2.f, 95.f));
    float anchoTabla = 1100.f;
    float inicioTablaX = centroX - (anchoTabla / 2.f);
    float inicioTablaY = 230.f;
    float altoFila = 68.f;
    float colPos[] = { 120.f, 540.f, 820.f, 1100.f };
    textoCabecera.setString("POS");
    textoCabecera.setCharacterSize(26);
    textoCabecera.setFillColor(Color(255, 107, 53));
    textoCabecera.setPosition(Vector2f(
        inicioTablaX + (colPos[0] - textoCabecera.getGlobalBounds().size.x) / 2.f,
        inicioTablaY + (altoFila - textoCabecera.getGlobalBounds().size.y) / 2.f - textoCabecera.getGlobalBounds().position.y
    ));
    Text capUsuario(font, "JUGADOR", 26);
    capUsuario.setFillColor(Color(255, 107, 53));
    capUsuario.setPosition(Vector2f(
        inicioTablaX + colPos[0] + ((colPos[1] - colPos[0]) - capUsuario.getGlobalBounds().size.x) / 2.f,
        textoCabecera.getPosition().y
    ));
    Text capTiempo(font, "TIEMPO", 26);
    capTiempo.setFillColor(Color(255, 107, 53));
    capTiempo.setPosition(Vector2f(
        inicioTablaX + colPos[1] + ((colPos[2] - colPos[1]) - capTiempo.getGlobalBounds().size.x) / 2.f,
        textoCabecera.getPosition().y
    ));
    Text capPuntaje(font, "PUNTAJE", 26);
    capPuntaje.setFillColor(Color(255, 107, 53));
    capPuntaje.setPosition(Vector2f(
        inicioTablaX + colPos[2] + ((colPos[3] - colPos[2]) - capPuntaje.getGlobalBounds().size.x) / 2.f,
        textoCabecera.getPosition().y
    ));
    filasRanking.clear();
    filasRanking.push_back(capUsuario);
    filasRanking.push_back(capTiempo);
    filasRanking.push_back(capPuntaje);
    const size_t totalFilasMostrar = 5;
    for (size_t i = 0; i < totalFilasMostrar; i++) {
        float filaY = inicioTablaY + ((i + 1) * altoFila);
        bool hayDatos = (i < listaPuntajes.size());
        Color colorFila = Color(220, 225, 230);
        if (hayDatos) {
            if (i == 0) colorFila = Color(255, 215, 0);
            else if (i == 1) colorFila = Color(200, 215, 230);
            else if (i == 2) colorFila = Color(215, 140, 75);
        } else {
            colorFila = Color(130, 140, 150);
        }
        string strPos = "#" + to_string(i + 1);
        string strUser = hayDatos ? string(listaPuntajes[i].nombreUsuario) : "---";
        string strTiempo = hayDatos ? (to_string(listaPuntajes[i].tiempoSegundos) + "s") : "---";
        string strPuntos = hayDatos ? to_string(listaPuntajes[i].puntaje) : "---";
        Text txtPos(font, strPos, 24);
        txtPos.setFillColor(colorFila);
        txtPos.setPosition(Vector2f(
            inicioTablaX + (colPos[0] - txtPos.getGlobalBounds().size.x) / 2.f,
            filaY + (altoFila - txtPos.getGlobalBounds().size.y) / 2.f - txtPos.getGlobalBounds().position.y
        ));
        filasRanking.push_back(txtPos);
        Text txtUser(font, strUser, 24);
        txtUser.setFillColor(colorFila);
        txtUser.setPosition(Vector2f(
            inicioTablaX + colPos[0] + ((colPos[1] - colPos[0]) - txtUser.getGlobalBounds().size.x) / 2.f,
            filaY + (altoFila - txtUser.getGlobalBounds().size.y) / 2.f - txtUser.getGlobalBounds().position.y
        ));
        filasRanking.push_back(txtUser);
        Text txtTiempo(font, strTiempo, 24);
        txtTiempo.setFillColor(colorFila);
        txtTiempo.setPosition(Vector2f(
            inicioTablaX + colPos[1] + ((colPos[2] - colPos[1]) - txtTiempo.getGlobalBounds().size.x) / 2.f,
            filaY + (altoFila - txtTiempo.getGlobalBounds().size.y) / 2.f - txtTiempo.getGlobalBounds().position.y
        ));
        filasRanking.push_back(txtTiempo);
        Text txtPuntos(font, strPuntos, 24);
        txtPuntos.setFillColor(colorFila);
        txtPuntos.setPosition(Vector2f(
            inicioTablaX + colPos[2] + ((colPos[3] - colPos[2]) - txtPuntos.getGlobalBounds().size.x) / 2.f,
            filaY + (altoFila - txtPuntos.getGlobalBounds().size.y) / 2.f - txtPuntos.getGlobalBounds().position.y
        ));
        filasRanking.push_back(txtPuntos);
    }
    lineasContenedor.clear();
    float altoTotalTabla = altoFila * (totalFilasMostrar + 1);
    sf::RectangleShape fondoTabla(Vector2f(anchoTabla, altoTotalTabla));
    fondoTabla.setPosition(Vector2f(inicioTablaX, inicioTablaY));
    fondoTabla.setFillColor(Color(15, 22, 30, 220));
    fondoTabla.setOutlineColor(Color(255, 107, 53, 200));
    fondoTabla.setOutlineThickness(3.f);
    lineasContenedor.push_back(fondoTabla);
    sf::RectangleShape fondoCabecera(Vector2f(anchoTabla, altoFila));
    fondoCabecera.setPosition(Vector2f(inicioTablaX, inicioTablaY));
    fondoCabecera.setFillColor(Color(255, 107, 53, 35));
    lineasContenedor.push_back(fondoCabecera);
    for (size_t i = 1; i <= totalFilasMostrar; i++) {
        sf::RectangleShape lineah(Vector2f(anchoTabla, 1.f));
        lineah.setPosition(Vector2f(inicioTablaX, inicioTablaY + (i * altoFila)));
        lineah.setFillColor(Color(255, 255, 255, 45));
        lineasContenedor.push_back(lineah);
    }
    for (int i = 0; i < 3; i++) {
        sf::RectangleShape lineav(Vector2f(1.f, altoTotalTabla));
        lineav.setPosition(Vector2f(inicioTablaX + colPos[i], inicioTablaY));
        lineav.setFillColor(Color(255, 255, 255, 45));
        lineasContenedor.push_back(lineav);
    }
    btnVolver.setSize(Vector2f(280.f, 60.f));
    btnVolver.setPosition(Vector2f(centroX - 140.f, inicioTablaY + altoTotalTabla + 75.f));
    btnVolver.setFillColor(Color(255, 107, 53));
    etiquetaVolver.setString("Volver");
    etiquetaVolver.setCharacterSize(28);
    etiquetaVolver.setFillColor(Color::White);
    etiquetaVolver.setPosition(Vector2f(
        btnVolver.getPosition().x + (280.f - etiquetaVolver.getGlobalBounds().size.x) / 2.f,
        btnVolver.getPosition().y + (60.f - etiquetaVolver.getGlobalBounds().size.y) / 2.f - etiquetaVolver.getGlobalBounds().position.y
    ));
}

void ranking::mostrar(RenderWindow& window) {
    if (!cargarRecursos()) return;
    leerYProcesarRanking();
    inicializarComponentes(window);
    Texture texFondo;
    bool tieneFondo = texFondo.loadFromFile("img/fondo_menu.png");
    optional<Sprite> sprFondo;
    RectangleShape capaOscura(Vector2f(window.getView().getSize()));
    capaOscura.setFillColor(Color(10, 15, 22, 185));
    if (tieneFondo) {
        sprFondo.emplace(texFondo);
        Vector2u tamV = Vector2u(window.getView().getSize());
        Vector2u tamF = texFondo.getSize();
        sprFondo->setScale({static_cast<float>(tamV.x) / tamF.x, static_cast<float>(tamV.y) / tamF.y});
    }
    Color colorBtn(255, 107, 53);
    Color colorHover(210, 75, 30);
    Color colorBorde(255, 180, 100);
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
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
        if (btnVolver.getGlobalBounds().contains(mousePos)) {
            btnVolver.setFillColor(colorHover);
            btnVolver.setOutlineColor(colorBorde);
            btnVolver.setOutlineThickness(3.f);
        }
        else {
            btnVolver.setFillColor(colorBtn);
            btnVolver.setOutlineThickness(0.f);
        }
        window.clear(Color(24, 33, 43));
        if (tieneFondo) {
            window.draw(*sprFondo);
            window.draw(capaOscura);
        }
        for (const auto& linea : lineasContenedor) {
            window.draw(linea);
        }
        window.draw(textoTitulo);
        window.draw(textoCabecera);
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


