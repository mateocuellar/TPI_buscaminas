#ifndef SELECCIONPERSONAJE_H
#define SELECCIONPERSONAJE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace sf;

enum TextoPersonaje { NOMBRE, HABILIDAD, PASIVA, DESCRIPCION, CANT_TEXTOS };
enum ColoresPersonaje { colorHabilidad, colorPasiva, CANT_COLORES };

class Personaje{
protected:
    string vecTextos[CANT_TEXTOS];
    int vida;
    int armadura;
    string rutaImagen;
    Texture textura;
    Color vecColores[CANT_COLORES];

public:
    Personaje(const vector<string>& datos, int vida, int armadura, string rutaImagen, const vector<Color>& colores);
    bool cargarImagen();
    const Texture& getTextura() const;

    string getTexto(TextoPersonaje tipo) const;
    int getVida() const;
    int getArmadura() const;

    Color getColor(ColoresPersonaje color) const;
    virtual ~Personaje();
};


class CasperNull:public Personaje{
protected:

public:
    CasperNull();
    ~CasperNull();
};

class DunbarYunque:public Personaje{
protected:

public:
    DunbarYunque();
    ~DunbarYunque();
};

class TessKnot:public Personaje{
protected:

public:
    TessKnot();
    ~TessKnot();
};

Personaje* seleccionPersonaje(RenderWindow& window);

#endif
