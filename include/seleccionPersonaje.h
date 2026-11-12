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
    int maxUsoHabilidad;
    int contadorHabilidad = 0;
    string rutaImagen;
    string rutaImagenTexturaIdle;
    string rutaImgTexturaDanio;
    Texture textura;
    Texture texturaIdle;
    Texture texturaDanio;
    Color vecColores[CANT_COLORES];
public:
    Personaje(const vector<string>& datos, int vida, int armadura, int maxUsoHabilidad, string rutaImagen, string rutaImagenTexturaIdle, string rutaImgTexturaDanio,const vector<Color>& colores);

    //getters
    string getTexto(TextoPersonaje tipo) const;
    int getVida() const;
    int getArmadura() const;
    const Texture& getTextura() const;
    const Texture& getTexturaIdle() const;
    const Texture& getTexturaDanio() const;

    ///setters
    void setArmadura(int arm);
    void setVida(int v);
    void setTextura(Texture nuevaTextura);
    void setContadorHabilidad(int contadorHabilidad);

    bool usarHabilidad();
    virtual void habilidad() = 0;

    void recibirDanio();

    bool cargarTexturasPersonajes();
    Color getColor(ColoresPersonaje color) const;
    virtual ~Personaje();
};


class CasperNull:public Personaje{
protected:
public:
    CasperNull();
    void habilidad() override;
    ~CasperNull();
};

class DunbarYunque:public Personaje{
protected:
public:
    DunbarYunque();
    void habilidad() override;
    ~DunbarYunque();
};

class TessKnot:public Personaje{
protected:
public:
    TessKnot();
    void habilidad() override;
    ~TessKnot();
};

Personaje* seleccionPersonaje(RenderWindow& window);

#endif
