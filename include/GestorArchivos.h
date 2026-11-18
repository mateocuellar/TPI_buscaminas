#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H

#include <string>
#include "registroUsuario.h"
#include "ranking.h"

class GestorArchivos {

    public:

        static bool guardarUsuario(const std::string& user, const std::string& pass);
        static void actualizarRanking(int idUser, const std::string& nombreUser, int puntos, int segundos, int lvl);
};

#endif
