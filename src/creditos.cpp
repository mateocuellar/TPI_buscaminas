#include "creditos.h"
#include "menuPrincipal.h"
#include <fstream>
#include <cstring>
#include <iostream>

using namespace std;
using namespace sf;

creditos::creditos() : etiquetaVolver(font){

}

bool creditos::cargarRecursos() {
    if(!font.openFromFile("fuente.TTF")) return false;
    texturaLogo.emplace();
    if (!texturaLogo->loadFromFile("img/logo.png")) return false;
    return true;
}

const Texture& creditos::getTextura(){
    return *texturaLogo;
}
void creditos::inicializarComponentes(RenderWindow& window) {
    Vector2u tamVentana = Vector2u(window.getView().getSize());
    textoTitulo.emplace(font, "CREDITOS",50);
    textoTitulo->setPosition(Vector2f((tamVentana.x - textoTitulo->getGlobalBounds().size.x) / 2.f, 200.f));

    float inicioX = 600.f;
    textoCreditos.emplace(font,
                          "Integrantes:\n"
                          "Rodrigo Alejandro Aranda\n"
                          "Mateo Cuellar\n\n"
                          "Materia:\n"
                          "Programación II\n\n"
                          "Profesores:\n"
                          "Brian Esteban Lara Campos\n"
                          "Daniel Kloster\n"
                          , 28);
    textoCreditos->setPosition(Vector2f(inicioX, 350.f));

    textoTpi.emplace(font, U"Trabajo Pr\u00e1ctico Integrador - Programaci\u00f3n II - Universidad Tecnol\u00f3gica Nacional", 20);
    FloatRect boundsTpi = textoTpi->getGlobalBounds();
    textoTpi->setPosition(Vector2f((tamVentana.x - boundsTpi.size.x) / 2.f,tamVentana.y - boundsTpi.size.y - 25.f));

    logo.emplace(getTextura());
    float altoimg = 250.f;
    float escala = altoimg / texturaLogo->getSize().y;
    logo->setScale(Vector2f(escala, escala));
    logo->setPosition(Vector2f(inicioX + textoCreditos->getGlobalBounds().size.x +50, (tamVentana.y - altoimg)/2 - 30));


    btnVolver.setSize(Vector2f(280.f, 60.f));
    btnVolver.setPosition(Vector2f((tamVentana.x - btnVolver.getGlobalBounds().size.x )/2, tamVentana.y - 350.f));
    btnVolver.setFillColor(Color(255, 107, 53));
    etiquetaVolver.setString("Volver");
    etiquetaVolver.setCharacterSize(28);
    etiquetaVolver.setFillColor(Color::White);
    etiquetaVolver.setPosition(Vector2f(
        btnVolver.getPosition().x + (280.f - etiquetaVolver.getGlobalBounds().size.x) / 2.f,
        btnVolver.getPosition().y + (60.f - etiquetaVolver.getGlobalBounds().size.y) / 2.f - etiquetaVolver.getGlobalBounds().position.y
    ));

}

void creditos::mostrar(RenderWindow& window) {
    if (!cargarRecursos()) return;
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
        dibujarConSombra(window, *textoTitulo, 5.f);
        dibujarConSombra(window, *textoCreditos, 3.f);
        dibujarConSombra(window, *textoTpi, 3.f);

        window.draw(*logo);

        window.draw(btnVolver);
        window.draw(etiquetaVolver);
        window.display();
    }
}
