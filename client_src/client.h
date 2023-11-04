

#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include <map>
#include <vector>
#include "../common_src/common_socket.h"
#include "client_protocol.h"
#include "client_sender.h"
#include "DTO/common_state_game.h"
#include <optional>

class Cliente
{
private:
    const std::string hostname;
    const std::string servname;
    ProtocoloCliente protocolo;
    bool conectado = true;
    uint8_t id;
    Queue<uint8_t> queue_sender;
    Queue<StateGame *> queue_receiver;
    ClienteLanzador lanzador;

public:
    Cliente(const std::string &hostname, const std::string &servname);
    ~Cliente();
    //void run();

    void cargar_id_jugador(const uint8_t id);
    uint8_t obtener_id_jugador();
    void procesar_mensaje();
    
    bool esta_conectado();

    std::optional<StateGame*> obtener_estado();
    void autorizar_turno(bool permiso);

    /*
        Puedo hacer que sea uno solo
    */
    void mover_izquierda();
    void mover_derecha();
    void saltar_hacia_delante();
    void saltar_hacia_atras();

    Cliente(const Cliente &) = delete;
    Cliente &operator=(const Cliente &) = delete;
};

#endif //  CLIENT_H_