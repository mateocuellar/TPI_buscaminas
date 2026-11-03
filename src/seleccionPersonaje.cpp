#include "seleccionPersonaje.h"
#include "partida.h"
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;
using namespace sf;

Personaje::Personaje(const vector<string>& datos, int vida, int armadura, string rutaImagen, string rutaImagenTexturaIdle,const vector<Color>& colores) {
    for (int i = 0; i < CANT_TEXTOS && i < (int)datos.size(); i++) {
        vecTextos[i] = datos[i];
    }
    for (int i = 0; i < CANT_COLORES&& i < (int)colores.size(); i++) {
        vecColores[i] = colores[i];
    }
    this->vida = vida;
    this->armadura = armadura;
    this->rutaImagen = rutaImagen;
    this->rutaImagenTexturaIdle = rutaImagenTexturaIdle;
}

Personaje::~Personaje() {
}
string Personaje::getTexto(TextoPersonaje tipo) const {
    return vecTextos[tipo];
}
int Personaje::getVida() const{
    return vida;
}
int Personaje::getArmadura() const{
    return armadura;
}


Color Personaje::getColor(ColoresPersonaje color) const {
    return vecColores[color];
}

const Texture& Personaje::getTextura() const {
    return textura;
}
const Texture& Personaje::getTexturaIdle() const {
    return texturaIdle;
}

CasperNull::CasperNull() : Personaje(
    {"Casper Null",
     "SEEK AND DESTROY",
     "DESARMADOR",
     "Filtra la informacion falsa de las Minas\n"
     "de Radiacion y muestra el numero real. \n"
     "Tiene un intento de bandera erronea por \n"
     "nivel sin penalizacion. \n\n"
     "Es fragil: 1 vida menos y una Mina de Fuego \n"
     "lo elimina al instante.\n"},
    3, 1,
    "imagenes/personaje-casper-null.png",
    "img/texturaPersonajes/casper_null_64.png",
    {Color(225, 85, 40), Color(220, 175, 60)}) {
}

CasperNull::~CasperNull() {
}
DunbarYunque::DunbarYunque() : Personaje(
    {"Dunbar Yunque",
    "MITIGAR",
    "ESCUDO TÉRMICO",
    "El escudo absorbe por completo el impacto \n"
    "de minas comunes y de fuego, \n"
    "y es inmune al hielo. \n"
    "Pero el tiempo limite se reduce 25% \n"
    "y cada bandera mal puesta le baja el \n"
    "puntaje del ranking."},
    3, 4,
    "imagenes/personaje-dunbar-yunque.png",
    "img/texturaPersonajes/dunbar_yunque_64.png",
    {Color(215, 130, 45), Color(110, 195, 235)}) {
}
DunbarYunque::~DunbarYunque() {
}
TessKnot::TessKnot() : Personaje(
    {"Tess Knot",
     "DESTREZA ANALÍTICA",
     "ESCANEO",
     "Su escaneo avisa si una ruta puede causar \n"
     "reaccion en cadena. \n"
     "No tiene defensas: una mina comun \n"
     "le resta 2 vidas, \n"
     "no es inmune a ningun estado, \n"
     "y el escaneo tiene 5 turnos de recarga."},
    4, 3,
    "imagenes/personaje-tess-knot.png",
    "img/texturaPersonajes/tess_knot_64.png",
    {Color(50, 185, 175), Color(80, 200, 110)}) {
}
TessKnot::~TessKnot() {
}

void borrarPersonajes(vector<Personaje*>& personajes) {
    for (size_t i = 0; i < personajes.size(); i++) {
        delete personajes[i];
    }
}

Personaje* seleccionPersonaje(RenderWindow& window) {
    Vector2u tamVentana = window.getSize();

    Texture texturaFondo;
    Font font, fontText;

    if (!texturaFondo.loadFromFile("img/fondo_menu.png")) return nullptr;
    if (!font.openFromFile("fuente.TTF")) return nullptr;
    if (!fontText.openFromFile("fuentetextos.TTF")) return nullptr;

    RectangleShape capaOscura;
    capaOscura.setSize(Vector2f(tamVentana.x, tamVentana.y));
    capaOscura.setFillColor(Color(0, 0, 0, 80));

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

    for (size_t i = 0; i < personajes.size(); i++) {
        if (!personajes[i]->cargarTexturasPersonajes()) {
            cout << "Error cargando recursos de " << personajes[i]->getTexto(NOMBRE) << endl;
            borrarPersonajes(personajes);
            return nullptr;
        }
    }

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

        Text desc(fontText, personajes[i]->getTexto(DESCRIPCION), 24);
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
                    borrarPersonajes(personajes);
                    return nullptr;
                }
            }

            if (const auto* mouseButton = event->getIf<Event::MouseButtonPressed>()) {
                if (mouseButton->button == Mouse::Button::Right) {
                    borrarPersonajes(personajes);
                    return nullptr;
                }
                if (mouseButton->button == Mouse::Button::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);
                    for (size_t i = 0; i < botones.size(); i++) {
                        if (botones[i].getGlobalBounds().contains(mousePos)) {
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
        window.draw(capaOscura);

        for (size_t i = 0; i < botones.size(); i++) {
            window.draw(botones[i]);
            window.draw(sprites[i]);
            window.draw(nombres[i]);
            window.draw(descripciones[i]);
        }
        window.display();
    }

    borrarPersonajes(personajes);
    return nullptr;
}
