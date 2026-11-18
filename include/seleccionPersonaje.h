#ifndef SELECCIONPERSONAJE_H
#define SELECCIONPERSONAJE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace sf;

enum TextoPersonaje { NOMBRE, HABILIDAD, PASIVA, DESCRIPCION, CANT_TEXTOS };
enum TexturasPersonaje { FOTO, IDLE, ANIMDANIO, ANIMHABILIDAD, HUD, CANT_TEXTURAS };
enum ColoresPersonaje { colorHabilidad, colorPasiva, CANT_COLORES };

class Personaje{
protected:
    string vecTextos[CANT_TEXTOS];
    int vida;
    int armadura;
    int maxUsoHabilidad;
    int contadorHabilidad = 0;

    string rutaImagenes[CANT_TEXTURAS];

    Texture textura;
    Texture texturaIdle;
    Texture texturaDanio;
    Texture texturaHabilidad;

    Color vecColores[CANT_COLORES];
public:
    Personaje(const vector<string>& datos, int vida, int armadura, int maxUsoHabilidad, const vector <string>rutaImagenes, const vector<Color>& colores);

    string getTexto(TextoPersonaje tipo) const;
    int getVida() const;
    int getArmadura() const;
    const Texture& getTextura() const;
    const Texture& getTexturaIdle() const;
    const Texture& getTexturaDanio() const;
    const Texture& getTexturaHabilidad() const;


    void setArmadura(int arm);
    void setVida(int v);
    void setTextura(Texture nuevaTextura);
    int getContadorHabilidad() const;
    bool usarHabilidad();
    virtual void habilidad() = 0;
    void recargarHabilidad();
    int getUsosRestantes() const;

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
