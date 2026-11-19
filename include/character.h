#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Map.h"

using namespace std;
using namespace sf;

enum TextoPersonaje { NOMBRE, HABILIDAD, PASIVA, DESCRIPCION, CANT_TEXTOS };
enum TexturasPersonaje { FOTO, IDLE, ANIMDANIO, ANIMHABILIDAD, HUD, CANT_TEXTURAS };
enum ColoresPersonaje { colorHabilidad, colorPasiva, CANT_COLORES };

class Character {
protected:
    string nombre;
    int vidas;

    string vecTextos[CANT_TEXTOS];
    int armadura;
    int maxUsoHabilidad;
    int contadorHabilidad = 0;

    string rutaImagenes[CANT_TEXTURAS];
    Texture vecTexturas[CANT_TEXTURAS];
    Color vecColores[CANT_COLORES];

public:
    Character(const vector<string>& datos, int vida, int armadura, int maxUsoHabilidad, const vector<string> rutaImagenes, const vector<Color>& colores);

    string getTexto(TextoPersonaje tipo) const;
    int getVida() const { return vidas; }
    int getArmadura() const { return armadura; }
    const Texture& getTextura(TexturasPersonaje tipo) const;

    void setArmadura(int arm) { armadura = arm; }
    void setVida(int v) { vidas = v; }
    int getContadorHabilidad() const { return contadorHabilidad; }
    bool usarHabilidad();
    void recargarHabilidad();
    int getUsosRestantes() const;

    virtual void procesarImpactoMina(int tipoMina) = 0;
    virtual void habilidad() = 0;

    void recibirDanio();
    bool cargarTexturasPersonajes();
    Color getColor(ColoresPersonaje color) const;
    virtual ~Character();
};

class Desactivador : public Character {
public:
    Desactivador();
    void procesarImpactoMina(int tipoMina) override;
    void habilidad() override;
    ~Desactivador();
};

class Estabilizador : public Character {
public:
    bool escudoHermetico;
    Estabilizador();
    void procesarImpactoMina(int tipoMina) override;
    void habilidad() override;
    ~Estabilizador();
};

class Cartografa : public Character {
public:
    Cartografa();
    void procesarImpactoMina(int tipoMina) override;
    bool escanearRuta(const Map& mapa, int col, int fila) const;
    void habilidad() override;
    ~Cartografa();
};



Character* seleccionPersonaje(RenderWindow& window);

#endif

