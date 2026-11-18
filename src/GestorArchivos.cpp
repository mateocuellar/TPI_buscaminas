#include "GestorArchivos.h"
#include <fstream>
#include <cstring>

using namespace std;

bool GestorArchivos::guardarUsuario(const string& user, const string& pass) {

    RegistroUsuarioBinario nuevo;
    memset(&nuevo, 0, sizeof(RegistroUsuarioBinario));
    strncpy(nuevo.nombre, user.c_str(), 19);
    strncpy(nuevo.password, pass.c_str(), 19);

    ofstream archivo("Usuarios.dat", ios::binary | ios::app);
    if (!archivo.is_open()) return false;
    archivo.write(reinterpret_cast<const char*>(&nuevo), sizeof(RegistroUsuarioBinario));
    archivo.close();
    return true;
}

void GestorArchivos::actualizarRanking(int idUser, const string& nombreUser, int puntos, int segundos, int lvl) {

    ranking::registrarPuntaje(idUser, nombreUser, puntos, segundos, lvl);
}


