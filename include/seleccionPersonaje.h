#ifndef SELECCIONPERSONAJE_H
#define SELECCIONPERSONAJE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace sf;

enum TextoPersonaje { NOMBRE, HABILIDAD, PASIVA, DESCRIPCION, CANT_TEXTOS };

class Personaje{
protected:
    string vecTextos[CANT_TEXTOS];
    int vida;
    int resistencia;
    string rutaImagen;
    Texture textura;

public:
    Personaje(const vector<string>& datos, int vida, int resistencia, string rutaImagen);
    bool cargarImagen();
    const Texture& getTextura() const;

    string getTexto(TextoPersonaje tipo) const;

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
