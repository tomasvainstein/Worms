#ifndef ESCENARIO_H_
#define ESCENARIO_H_
#include <random>
#include <vector>
#include <sstream>
#include "gusano.h"
#include "data/posicion_viga.h"
#include "data/posicion_jugador.h"
#include <map>
#include <cmath>

const float_t FRAME_RATE = 4.0f / 60.0f;

struct Viga
{
    bool tipo;
    int x;
    int y;
    int angulo;
};

class MonitorJugadores;

class Escenario
{
public:
    Escenario(std::uint16_t x_size, std::uint16_t y_size, MonitorJugadores *monitor);

    void agregar_gusano(int jugador_id);

    void colocar_viga(int x, int y, bool tipo, int inclinacion);

    void avisar_desconexion(int jugador);

    void mover_gusano_derecha(int gusano, int jugador);

    void mover_gusano_izquierda(int gusano, int jugador);

    void mover_gusano_arriba_adelante(int gusano, int jugador);

    void mover_gusano_arriba_atras(int gusano, int jugador);

    ~Escenario();

private:
    std::uint16_t x_size;
    std::uint16_t y_size;
    MonitorJugadores *monitor;
    b2World *escenario;
    std::vector<PosicionViga> vigas;
    std::map<int, std::map<int, Gusano *>> gusanos;
    std::vector<std::pair<int, int>> spawns;
    int cantidad_jugadores = 0;

    std::vector<PosicionJugador> crear_paquete();
};
#endif