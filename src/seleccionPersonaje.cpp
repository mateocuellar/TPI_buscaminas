#include "seleccionPersonaje.h"
#include "partida.h"
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;
using namespace sf;

Personaje::Personaje(const vector<string>& datos, int vida, int resistencia, string rutaImagen) {
    for (int i = 0; i < CANT_TEXTOS && i < (int)datos.size(); i++) {
        vecTextos[i] = datos[i];
    }
    this->vida = vida;
    this->resistencia = resistencia;
    this->rutaImagen = rutaImagen;
}

Personaje::~Personaje() {
}
string Personaje::getTexto(TextoPersonaje tipo) const {
    return vecTextos[tipo];
}

bool Personaje::cargarImagen() {
    return textura.loadFromFile(rutaImagen);
}
const Texture& Personaje::getTextura() const {
    return textura;
}

CasperNull::CasperNull() : Personaje(
    {"Casper Null",
     "Seek & destroy",
     "Desarmador",
     "Filtra la informacion falsa de las Minas\n"
     "de Radiacion y muestra el numero real. \n"
     "Tiene un intento de bandera erronea por \n"
     "nivel sin penalizacion. \n\n"
     "Es fragil: 1 vida menos y una Mina de Fuego \n"
     "lo elimina al instante.\n"},
    100, 10, "imagenes/personaje-casper-null.png") {
}

CasperNull::~CasperNull() {
}
DunbarYunque::DunbarYunque() : Personaje(
    {"Dunbar Yunque",
    "Mitigar",
    "Escudo termico",
    "El escudo absorbe por completo el impacto \n"
    "de minas comunes y de fuego, \n"
    "y es inmune al hielo. \n"
    "Pero el tiempo limite se reduce 25% \n"
    "y cada bandera mal puesta le baja el \n"
    "puntaje del ranking."},
    100, 20, "imagenes/personaje-dunbar-yunque.png"){
}

DunbarYunque::~DunbarYunque() {
}
TessKnot::TessKnot() : Personaje(
    {"Tess Knot",
     "Destreza analitica",
     "Escaneo",
     "Su escaneo avisa si una ruta puede causar \n"
     "reaccion en cadena. \n"
     "No tiene defensas: una mina comun \n"
     "le resta 2 vidas, \n"
     "no es inmune a ningun estado, \n"
     "y el escaneo tiene 5 turnos de recarga."},
    100, 0, "imagenes/personaje-tess-knot.png") {
}

TessKnot::~TessKnot() {
}
Personaje* seleccionPersonaje(RenderWindow& window) {
    Vector2u tamVentana = window.getSize();
    Texture texturaFondo;
    texturaFondo.loadFromFile("img/fondo_menu.png");

    Sprite fondo(texturaFondo);
    Vector2u tamFondo = texturaFondo.getSize();
    fondo.setScale(Vector2f(
        static_cast<float>(tamVentana.x) / static_cast<float>(tamFondo.x),
        static_cast<float>(tamVentana.y) / static_cast<float>(tamFondo.y)
    ));
    fondo.setPosition(Vector2f(0,0));
    vector<Personaje*> personajes = {
        new CasperNull(),
        new DunbarYunque(),
        new TessKnot()
    };

    for (Personaje* p : personajes) {
        if (!p->cargarImagen())
            cout << "Error cargando la imagen de " << p->getTexto(NOMBRE) << endl;
    }

    Font font, fontText;
    font.openFromFile("fuente.TTF");
    fontText.openFromFile("fuentetextos.TTF");

    Color colorNormal(255, 107, 53);
    Color colorHover(210, 75, 30);
    Color colorBorde(255, 180, 100);

    float anchoBoton = 850.f, altoBoton = 300.f;
    float anchoImg = altoBoton - 30, altoImg = altoBoton - 30;
    float inicioY = tamVentana.y / 2 - altoBoton / 2;
    float espaciado = 350.f;
    float margen = 30.f;
    float primeraY = inicioY - espaciado;

    vector<RectangleShape> botones;
    vector<Text> nombres, descripciones;
    vector<Sprite> sprites;

    for (size_t i = 0; i < personajes.size(); i++) {
        float y = primeraY + i * espaciado;
        float xBoton = (tamVentana.x - anchoBoton) / 2.f;

        RectangleShape boton(Vector2f(anchoBoton, altoBoton));
        boton.setPosition(Vector2f(xBoton, y));
        boton.setFillColor(colorNormal);
        botones.push_back(boton);

        Text nombre(font, personajes[i]->getTexto(NOMBRE), 30);
        nombre.setPosition(Vector2f(xBoton + margen + anchoImg, y + margen / 2));
        nombres.push_back(nombre);

        Text desc(fontText, personajes[i]->getTexto(DESCRIPCION), 28);
        desc.setPosition(Vector2f(xBoton + margen + anchoImg, y + margen * 2));
        descripciones.push_back(desc);

        Sprite sprite(personajes[i]->getTextura());
        Vector2u tam = personajes[i]->getTextura().getSize();
        sprite.setScale(Vector2f(anchoImg / tam.x, altoImg / tam.y));
        sprite.setPosition(Vector2f((tamVentana.x - anchoBoton + margen) / 2.f, y + margen / 2));
        sprites.push_back(sprite);
    }

    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (keyPressed->code == Keyboard::Key::Escape) {
                    for (Personaje* p : personajes) delete p;
                    return nullptr;
                }
            }

            if (const auto* mouseButton = event->getIf<Event::MouseButtonPressed>()) {
                if (mouseButton->button == Mouse::Button::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);
                    for (size_t i = 0; i < botones.size(); i++) {
                        if (botones[i].getGlobalBounds().contains(mousePos)) {
                            // se devuelve el personaje ya creado y cargado
                            for (Personaje* p : personajes)
                                if (p != personajes[i]) delete p;
                            cout << "se selecciono a " << personajes[i]->getTexto(NOMBRE) << endl;
                            return personajes[i];
                        }
                    }
                }
            }
        }

        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
        for (size_t i = 0; i < botones.size(); i++) {
            if (botones[i].getGlobalBounds().contains(mousePos)) {
                botones[i].setFillColor(colorHover);
                botones[i].setOutlineColor(colorBorde);
                botones[i].setOutlineThickness(3.f);
            } else {
                botones[i].setFillColor(colorNormal);
                botones[i].setOutlineThickness(0.f);
            }
        }

        window.clear(Color(24, 33, 43));
        window.draw(fondo);
        for (size_t i = 0; i < botones.size(); i++) {
            window.draw(botones[i]);
            window.draw(sprites[i]);
            window.draw(nombres[i]);
            window.draw(descripciones[i]);
        }
        window.display();
    }

    for (Personaje* p : personajes) delete p;
    return nullptr;
}
