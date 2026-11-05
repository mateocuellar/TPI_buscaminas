#include "registroUsuario.h"
#include <cstring>
#include <iostream>

using namespace std;
using namespace sf;

registroUsuario::registroUsuario() {
    stringUsuario = "";
    stringPassword = "";
    campoSeleccionado = true;
}

bool registroUsuario::cargarRecursos() {
    return font.openFromFile("fuente.TTF");
}

void registroUsuario::inicializarComponentes(RenderWindow& window, const string& tituloPantalla) {
    Vector2u tamVentana = window.getSize();
    float centroX = tamVentana.x / 2.f;

    cajaTextoUsuario.emplace(Vector2f(300.f, 40.f));
    cajaTextoUsuario->setPosition(Vector2f(centroX - 50.f, 345.f));
    cajaTextoUsuario->setFillColor(Color(40, 50, 65));
    cajaTextoUsuario->setOutlineThickness(2.f);

    cajaTextoPassword.emplace(Vector2f(300.f, 40.f));
    cajaTextoPassword->setPosition(Vector2f(centroX - 50.f, 445.f));
    cajaTextoPassword->setFillColor(Color(40, 50, 65));
    cajaTextoPassword->setOutlineThickness(2.f);

    textoUsuario.emplace(font, "Usuario: ", 24);
    textoUsuario->setPosition(Vector2f(centroX - 250.f, 350.f));

    textoPassword.emplace(font, "Contraseña: ", 24);
    textoPassword->setPosition(Vector2f(centroX - 250.f, 450.f));

    textoTitulo.emplace(font, tituloPantalla, 44);
    textoTitulo->setPosition(Vector2f((tamVentana.x - textoTitulo->getGlobalBounds().size.x) / 2.f, 200.f));

    textoContenidoUsuario.emplace(font, "", 20);
    textoContenidoUsuario->setPosition(Vector2f(cajaTextoUsuario->getPosition().x + 10.f, 350.f));
    textoContenidoUsuario->setFillColor(Color::White);

    textoContenidoPassword.emplace(font, "", 20);
    textoContenidoPassword->setPosition(Vector2f(cajaTextoPassword->getPosition().x + 10.f, 460.f));
    textoContenidoPassword->setFillColor(Color::White);

    btnAceptar.emplace(Vector2f(180.f, 45.f));
    btnAceptar->setPosition(Vector2f(centroX - 200.f, 580.f));
    btnAceptar->setFillColor(Color(255, 107, 53));

    etiquetaAceptar.emplace(font, "Aceptar", 20);
    etiquetaAceptar->setPosition(Vector2f(
        btnAceptar->getPosition().x + (180.f - etiquetaAceptar->getGlobalBounds().size.x) / 2.f,
        btnAceptar->getPosition().y + (45.f - etiquetaAceptar->getGlobalBounds().size.y) / 2.f - etiquetaAceptar->getGlobalBounds().position.y
    ));

    btnVolver.emplace(Vector2f(180.f, 45.f));
    btnVolver->setPosition(Vector2f(centroX + 20.f, 580.f));
    btnVolver->setFillColor(Color(100, 110, 120));

    etiquetaVolver.emplace(font, "Volver", 20);
    etiquetaVolver->setPosition(Vector2f(
        btnVolver->getPosition().x + (180.f - etiquetaVolver->getGlobalBounds().size.x) / 2.f,
        btnVolver->getPosition().y + (45.f - etiquetaVolver->getGlobalBounds().size.y) / 2.f - etiquetaVolver->getGlobalBounds().position.y
    ));
}

bool registroUsuario::mostrarLogin(RenderWindow& window) {
    if (!cargarRecursos()) return false;
    inicializarComponentes(window, "INICIAR SESION");

    stringUsuario = "";
    stringPassword = "";


    Text textoAlerta(font, "", 18);
    textoAlerta.setFillColor(Color::Red);
    textoAlerta.setPosition(Vector2f(window.getSize().x / 2.f - 100.f, 530.f));

    while (window.isOpen()) {
        cajaTextoUsuario->setOutlineColor(campoSeleccionado ? Color(255, 107, 53) : Color::Transparent);
        cajaTextoPassword->setOutlineColor(!campoSeleccionado ? Color(255, 107, 53) : Color::Transparent);

        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (keyPressed->code == Keyboard::Key::Escape) return false;
                if (keyPressed->code == Keyboard::Key::Tab) campoSeleccionado = !campoSeleccionado;

                if (keyPressed->code == Keyboard::Key::Enter){
                    if (campoSeleccionado){
                        campoSeleccionado = false;
                    }
                    else {

                        if (verificarCredenciales(stringUsuario, stringPassword)) {
                            cout << "Login exitoso!" << endl;
                            return true;
                        } else {
                            textoAlerta.setString("Usuario o clave incorrectos.");
                        }
                    }
                }
            }

            if (const auto* textEvent = event->getIf<Event::TextEntered>()) {
                char caracter = static_cast<char>(textEvent->unicode);
                if (caracter >= 32 && caracter <= 126) {
                    if (campoSeleccionado && stringUsuario.size() < 15) stringUsuario += caracter;
                    else if (!campoSeleccionado && stringPassword.size() < 15) stringPassword += caracter;
                }
                else if (caracter == 8) {
                    if (campoSeleccionado && !stringUsuario.empty()) stringUsuario.pop_back();
                    else if (!campoSeleccionado && !stringPassword.empty()) stringPassword.pop_back();
                }
            }

            if (const auto* mouseButton = event->getIf<Event::MouseButtonPressed>()) {
                if (mouseButton->button == Mouse::Button::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);
                    if (cajaTextoUsuario->getGlobalBounds().contains(mousePos)) campoSeleccionado = true;
                    if (cajaTextoPassword->getGlobalBounds().contains(mousePos)) campoSeleccionado = false;
                    if (btnVolver->getGlobalBounds().contains(mousePos)) return false;

                    if (btnAceptar->getGlobalBounds().contains(mousePos)) {

                        if (verificarCredenciales(stringUsuario, stringPassword)) {
                            cout << "Login exitoso!" << endl;
                            return true;
                        } else {
                            textoAlerta.setString("Usuario o clave incorrectos.");
                        }
                    }
                }
            }
        }

        textoContenidoUsuario->setString(stringUsuario);
        string oculto(stringPassword.size(), '*');
        textoContenidoPassword->setString(oculto);

        window.clear(Color(24, 33, 43));
        window.draw(*textoTitulo);
        window.draw(*cajaTextoUsuario);
        window.draw(*cajaTextoPassword);

        window.draw(*textoUsuario);
        window.draw(*textoPassword);

        window.draw(*textoContenidoUsuario);
        window.draw(*textoContenidoPassword);

        window.draw(*btnAceptar);
        window.draw(*btnVolver);
        window.draw(*etiquetaAceptar);
        window.draw(*etiquetaVolver);
        window.draw(textoAlerta);
        window.display();
    }
}

void registroUsuario::mostrarRegistro(RenderWindow& window) {
    if (!cargarRecursos()) return;
    inicializarComponentes(window, "REGISTRAR USUARIO NUEVO");

    stringUsuario = "";
    stringPassword = "";

    Text textoAlerta(font, "", 18);
    textoAlerta.setFillColor(Color::Red);
    textoAlerta.setPosition(Vector2f(window.getSize().x / 2.f - 120.f, 530.f));

    while (window.isOpen()) {
        cajaTextoUsuario->setOutlineColor(campoSeleccionado ? Color(255, 107, 53) : Color::Transparent);
        cajaTextoPassword->setOutlineColor(!campoSeleccionado ? Color(255, 107, 53) : Color::Transparent);

        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (keyPressed->code == Keyboard::Key::Escape) return;
                if (keyPressed->code == Keyboard::Key::Tab) campoSeleccionado = !campoSeleccionado;

                if (keyPressed->code == Keyboard::Key::Enter){
                    if (campoSeleccionado){
                        campoSeleccionado = false;
                    }
                    else {

                        if (guardarUsuarioNuevo(stringUsuario, stringPassword)) {
                            cout << "Usuario registrado con exito!" << endl;
                            return;
                        } else {
                            textoAlerta.setString("Error: El usuario ya existe o campos vacios.");
                        }
                    }
                }
            }

            if (const auto* textEvent = event->getIf<Event::TextEntered>()) {
                char caracter = static_cast<char>(textEvent->unicode);
                if (caracter >= 32 && caracter <= 126) {
                    if (campoSeleccionado && stringUsuario.size() < 15) stringUsuario += caracter;
                    else if (!campoSeleccionado && stringPassword.size() < 15) stringPassword += caracter;
                }
                else if (caracter == 8) {
                    if (campoSeleccionado && !stringUsuario.empty()) stringUsuario.pop_back();
                    else if (!campoSeleccionado && !stringPassword.empty()) stringPassword.pop_back();
                }
            }

            if (const auto* mouseButton = event->getIf<Event::MouseButtonPressed>()) {
                if (mouseButton->button == Mouse::Button::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);
                    if (cajaTextoUsuario->getGlobalBounds().contains(mousePos)) campoSeleccionado = true;
                    if (cajaTextoPassword->getGlobalBounds().contains(mousePos)) campoSeleccionado = false;

                    if (btnVolver->getGlobalBounds().contains(mousePos)) return;
                    if (btnAceptar->getGlobalBounds().contains(mousePos)) {

                    if (guardarUsuarioNuevo(stringUsuario, stringPassword)) {
                        cout << "Usuario registrado con exito!" << endl;
                        return;
                    } else {
                        textoAlerta.setString("Error: El usuario ya existe o campos vacios.");
                    }
                    }
                    }
                    }
                    }

    textoContenidoUsuario->setString(stringUsuario);
    string oculto(stringPassword.size(), '*');
    textoContenidoPassword->setString(oculto);
    window.clear(Color(24, 33, 43));
    window.draw(*textoTitulo);
    window.draw(*cajaTextoUsuario);
    window.draw(*cajaTextoPassword);
    window.draw(*textoUsuario);
    window.draw(*textoPassword);
    window.draw(*textoContenidoUsuario);
    window.draw(*textoContenidoPassword);
    window.draw(*btnAceptar);
    window.draw(*btnVolver);
    window.draw(*etiquetaAceptar);
    window.draw(*etiquetaVolver);
    window.draw(textoAlerta);
    window.display();
    }
    }
    bool registroUsuario::guardarUsuarioNuevo(const std::string& user, const std::string& pass) {
        if (user.empty() || pass.empty() || user.size() >= 20 || pass.size() >= 20) return false;

            std::ifstream archivoLectura("Usuarios.dat", std::ios::binary);
        if (archivoLectura.is_open()) {
            RegistroUsuarioBinario registroLector;
        while (archivoLectura.read(reinterpret_cast<char*>(&registroLector), sizeof(RegistroUsuarioBinario))) {
            if (std::string(registroLector.nombre) == user) {
                archivoLectura.close();
                return false;
                }
            }
            archivoLectura.close();
        }

    RegistroUsuarioBinario nuevoUsuario;
    std::memset(&nuevoUsuario, 0, sizeof(RegistroUsuarioBinario));
    std::strncpy(nuevoUsuario.nombre, user.c_str(), 19);
    std::strncpy(nuevoUsuario.password, pass.c_str(), 19);

    std::ofstream archivoEscritura("Usuarios.dat", std::ios::binary | std::ios::app);
        if (archivoEscritura.is_open()) {
            archivoEscritura.write(reinterpret_cast<const char*>(&nuevoUsuario), sizeof(RegistroUsuarioBinario));
            archivoEscritura.close();
            return true;
        }
    return false;
    }
    bool registroUsuario::verificarCredenciales(const std::string& user, const std::string& pass) {
    std::ifstream archivo("Usuarios.dat", std::ios::binary);
        if (!archivo.is_open()) return false;
            RegistroUsuarioBinario registroLector;
            while (archivo.read(reinterpret_cast<char*>(&registroLector), sizeof(RegistroUsuarioBinario))) {
        if (std::string(registroLector.nombre) == user && std::string(registroLector.password) == pass) {
            archivo.close();
            return true;
        }
        }
    archivo.close();
    return false;
    }

