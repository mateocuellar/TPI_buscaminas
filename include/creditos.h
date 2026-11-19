#ifndef CREDITOS_H
#define CREDITOS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace sf;

class creditos {
private:
    Font font;

    optional<Text> textoTitulo;
    optional<Text> textoCreditos;

    optional<Text> textoTpi;

    optional<Texture> texturaLogo;
    optional<Sprite> logo;
    RectangleShape btnVolver;
    Text etiquetaVolver;

    bool cargarRecursos();
    void inicializarComponentes(RenderWindow& window);

public:
    creditos();
    const Texture& getTextura();
    void mostrar(RenderWindow& window);

};

#endif
