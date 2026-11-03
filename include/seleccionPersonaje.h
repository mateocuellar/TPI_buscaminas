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
    string rutaImagenTexturaIdle;
    Texture textura;
    Texture texturaIdle;
    Color vecColores[CANT_COLORES];
public:
    Personaje(const vector<string>& datos, int vida, int armadura, string rutaImagen, string rutaImagenTexturaIdle, const vector<Color>& colores);

    bool cargarTexturasPersonajes();

    const Texture& getTextura() const;
    const Texture& getTexturaIdle() const;

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
