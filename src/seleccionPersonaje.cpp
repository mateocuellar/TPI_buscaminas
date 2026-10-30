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
    CasperNull casper;
    if (!casper.cargarImagen()) {
    cout << "Error cargando la imagen del personaje" << endl;
    }
    DunbarYunque dunbar;
    if (!dunbar.cargarImagen()) {
    cout << "Error cargando la imagen del personaje" << endl;
    }
    TessKnot tess;
    if (!tess.cargarImagen()) {
    cout << "Error cargando la imagen del personaje" << endl;
    }
    Font font;
    if (!font.openFromFile("fuente.TTF")) {
        cout << "Error cargando la fuente" << endl;
    }
    Font fontText;
    if (!fontText.openFromFile("fuentetextos.TTF")) {
        cout << "Error cargando la fuente" << endl;
    }
    Color colorNormal(255, 107, 53);
    Color colorHover(210, 75, 30);
    Color colorBorde(255, 180, 100);

    float anchoBoton = 650.f;
    float altoBoton = 250.f;
    float anchoImg = altoBoton - 30;
    float altoImg = altoBoton - 30;

    float inicioY = tamVentana.y/2 - altoBoton/2;
    float espaciado = 300.f;
    float margen = 30.f;

    /// Boton CASPER NULL
    RectangleShape btnCasper(Vector2f(anchoBoton, altoBoton));
    btnCasper.setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f, inicioY-espaciado));
    btnCasper.setFillColor(Color(255, 107, 53));

    Text textoCasper(font, casper.getTexto(NOMBRE), 24);
    textoCasper.setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f  + margen + anchoImg, inicioY - espaciado + margen/2));
    Text descCasper(fontText, casper.getTexto(DESCRIPCION), 22);
    descCasper.setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f  + margen + anchoImg, inicioY - espaciado + margen * 2));

    Sprite spriteCasper(casper.getTextura());
    Vector2u tam = casper.getTextura().getSize();
    spriteCasper.setScale(Vector2f(anchoImg / tam.x, altoImg / tam.y));
    spriteCasper.setPosition(Vector2f((tamVentana.x - anchoBoton + margen) / 2.f, inicioY - espaciado + margen/2));
    /// Boton Dunbar Yunque
    RectangleShape btnDunbar(Vector2f(anchoBoton, altoBoton));
    btnDunbar.setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f, inicioY ));
    btnDunbar.setFillColor(Color(255, 107, 53));

    Text textoDunbar(font, dunbar.getTexto(NOMBRE), 24);
    textoDunbar.setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f  + margen + anchoImg, inicioY + margen/2));
    Text descDunbar(fontText, dunbar.getTexto(DESCRIPCION), 22);
    descDunbar.setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f  + margen + anchoImg, inicioY + margen * 2));

    Sprite spriteDunbar(dunbar.getTextura());
    spriteDunbar.setScale(Vector2f(anchoImg / tam.x, altoImg/ tam.y));
    spriteDunbar.setPosition(Vector2f((tamVentana.x - anchoBoton + margen) / 2.f, inicioY + margen/2));
    /// Boton Tess Knot
    RectangleShape btnTess(Vector2f(anchoBoton, altoBoton));
    btnTess.setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f, inicioY + espaciado));
    btnTess.setFillColor(Color(255, 107, 53));

    Text textoTess(font, tess.getTexto(NOMBRE),24);
    textoTess.setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f  + margen + anchoImg, inicioY + espaciado + margen/2));
    Text descTess(fontText, tess.getTexto(DESCRIPCION), 22);
    descTess.setPosition(Vector2f((tamVentana.x - anchoBoton) / 2.f  + margen + anchoImg, inicioY + espaciado + margen * 2));

    Sprite spriteTess(tess.getTextura());
    spriteTess.setScale(Vector2f(anchoImg / tam.x, altoImg/ tam.y));
    spriteTess.setPosition(Vector2f((tamVentana.x - anchoBoton + margen) / 2.f, inicioY + espaciado + margen/2));



    while (window.isOpen()) {
        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (keyPressed->code == Keyboard::Key::Escape) {
                    return nullptr;
                }
            }

            if (const auto* mouseButton = event->getIf<Event::MouseButtonPressed>()) {
                Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);
                 if (btnCasper.getGlobalBounds().contains(mousePos)) {
                    cout << "se selecciono a Casper" << endl;
                    CasperNull* elegido = new CasperNull();
                    if (!elegido->cargarImagen()) {
                        cout << "Error cargando la imagen del personaje" << endl;
                    }
                    return elegido;
                }
                if (btnDunbar.getGlobalBounds().contains(mousePos)) {
                    cout << "se selecciono a Casper" << endl;
                    DunbarYunque* elegido = new DunbarYunque();
                    if (!elegido->cargarImagen()) {
                        cout << "Error cargando la imagen del personaje" << endl;
                    }
                    return elegido;
                }
                if (btnTess.getGlobalBounds().contains(mousePos)) {
                    cout << "se selecciono a Casper" << endl;
                    TessKnot* elegido = new TessKnot();
                    if (!elegido->cargarImagen()) {
                        cout << "Error cargando la imagen del personaje" << endl;
                    }
                    return elegido;
                }
            }
        }
        Vector2i mousePixel = Mouse::getPosition(window);
        Vector2f mousePos = window.mapPixelToCoords(mousePixel);

        // CASPER
        if (btnCasper.getGlobalBounds().contains(mousePos)) {

            btnCasper.setFillColor(colorHover);
            btnCasper.setOutlineColor(colorBorde);
            btnCasper.setOutlineThickness(3.f);

        } else {

            btnCasper.setFillColor(colorNormal);
            btnCasper.setOutlineThickness(0.f);
        }

        // DUNBAR
        if (btnDunbar.getGlobalBounds().contains(mousePos)) {

            btnDunbar.setFillColor(colorHover);
            btnDunbar.setOutlineColor(colorBorde);
            btnDunbar.setOutlineThickness(3.f);

        } else {

            btnDunbar.setFillColor(colorNormal);
            btnDunbar.setOutlineThickness(0.f);
        }

        // TESS
        if (btnTess.getGlobalBounds().contains(mousePos)) {

            btnTess.setFillColor(colorHover);
            btnTess.setOutlineColor(colorBorde);
            btnTess.setOutlineThickness(3.f);

        } else {

            btnTess.setFillColor(colorNormal);
            btnTess.setOutlineThickness(0.f);
        }

        window.clear(Color(24, 33, 43));

        window.draw(btnCasper);
        window.draw(btnDunbar);
        window.draw(btnTess);

        window.draw(textoCasper);
        window.draw(textoDunbar);
        window.draw(textoTess);
        window.draw(descCasper);
        window.draw(descDunbar);
        window.draw(descTess);

        window.draw(spriteCasper);
        window.draw(spriteDunbar);
        window.draw(spriteTess);
        window.display();
    }
    return nullptr;
}
