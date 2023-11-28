#include "game.h"

#include <SDL2pp/SDL2pp.hh>

#define SLEEP_RATE (4.0f / 35.0f) * 50

Game::Game(ProtocoloCliente & protocolo) : cliente(protocolo) {}

void Game::run()
{
    uint32_t timeLost = 0;
    StateGame *estado;
    view.reproducir_musica();
    while (cliente.esta_conectado())
    {
        bool nuevo_estado = false;
        uint32_t ticks = SDL_GetTicks();

        std::optional<StateGame *> optional = cliente.obtener_estado();
        if (optional.has_value())
        {

            estado = optional.value();
            nuevo_estado = true;
        }

        if (not this->gameLoop(estado, nuevo_estado))
        {
            break;
        }

        uint32_t frame_ticks = SDL_GetTicks();
        uint32_t tick_diff = (frame_ticks - ticks) - timeLost;

        if (tick_diff <= SLEEP_RATE)
        {
            SDL_Delay(SLEEP_RATE - tick_diff);
            timeLost = SLEEP_RATE - tick_diff;
        }
        else
        {
            timeLost = 0;
        }
    }
}

bool Game::gameLoop(StateGame *estado, bool &nuevo_estado)
{
    view.clear();
    // estado->cambiar_render(permiso); -->para debuggear si llega algo no definido en procesar
    if (nuevo_estado)
    {
        this->procesar_estado(estado);
    }
    else
    {

        for (auto &[id, jugador] : jugadores)
        {
            jugador.stop_running();
        }
    }

    this->renderizar();
    view.mostrar();

    return this->manejarEventos();
}

bool Game::manejarEventos()
{
    Action *accion = nullptr;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return false; // Salir del juego sin necesidad de permiso
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (!permiso)
            {
                // Si no tienes permiso, solo permitir mutear el sonido
                if (event.key.keysym.sym != SDLK_m)
                {
                    continue; // Ignorar otras teclas si no hay permiso
                }
            }

            switch (event.key.keysym.sym)
            {
            case (SDLK_r):
            {
                tipo++;
                if (tipo > 10)
                {
                    tipo = 0;
                }

                accion = new Weapon(tipo);
                cliente.mandar_accion(accion);
                break;
            }
            case (SDLK_m):
            {
                view.mutear_sonidos();
                break;
            }
            case (SDLK_LEFT):
            {
                accion = new Left();
                cliente.mandar_accion(accion);
                break;
            }

            case (SDLK_RIGHT):
            {
                accion = new Right();
                cliente.mandar_accion(accion);
                break;
            }

            case (SDLK_RETURN):
            { // Enter
                view.reproducir_efecto("/sonidos/salto.WAV");
                accion = new JumpFoward();
                cliente.mandar_accion(accion);
                break;
            }
            case (SDLK_BACKSPACE):
            { // Retorno
                view.reproducir_efecto("/sonidos/salto.WAV");
                accion = new JumpBack();
                cliente.mandar_accion(accion);
                break;
            }
            case (SDLK_UP):
            {
                auto it = jugadores.find(turno);
                if (it != jugadores.end())
                {
                    // El elemento en la posición indicada por "turno" existe en el mapa
                    it->second.aumentar_angulo_arma();
                }
                break;
            }
            case (SDLK_DOWN):
            {
                // turno tiene que ser el super_id o le envio al server
                auto it = jugadores.find(turno);
                if (it != jugadores.end())
                {
                    // El elemento en la posición indicada por "turno" existe en el mapa
                    it->second.disminuir_angulo_arma();
                }
                break;
            }
            case(SDLK_SPACE):
            {
                auto it = jugadores.find(turno);
                if (it != jugadores.end())
                {
                    //si suelto o llego al maximo, disparo?
                    it->second.aumentar_potencia();
                }
                break;
            }
            case(SDLK_1):
            {
                auto it = jugadores.find(turno);
                if (it != jugadores.end())
                {
                    // El elemento en la posición indicada por "turno" existe en el mapa
                    it->second.cambiar_timer_arma(1);
                }
                break;
            }
            case(SDLK_2):
            {
                auto it = jugadores.find(turno);
                if (it != jugadores.end())
                {
                    // El elemento en la posición indicada por "turno" existe en el mapa
                    it->second.cambiar_timer_arma(2);
                }
                break;
            }
            case(SDLK_3):
            {
                auto it = jugadores.find(turno);
                if (it != jugadores.end())
                {
                    // El elemento en la posición indicada por "turno" existe en el mapa
                    it->second.cambiar_timer_arma(3);
                }
                break;
            }
            case(SDLK_4):
            {
                auto it = jugadores.find(turno);
                if (it != jugadores.end())
                {
                    // El elemento en la posición indicada por "turno" existe en el mapa
                    it->second.cambiar_timer_arma(4);
                }
                break;
            }
            case(SDLK_5):
            {
                auto it = jugadores.find(turno);
                if (it != jugadores.end())
                {
                    // El elemento en la posición indicada por "turno" existe en el mapa
                    it->second.cambiar_timer_arma(5);
                }
                break;
            }
            default:
            {
                // si llego aca, es una tecla que no uso
                std::cout << "Key=" << event.key.keysym.sym << "\n";
                break;
            }
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (!permiso)
            {
                continue; // Ignorar clics de ratón si no hay permiso
            }

            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = event.button.x;
                int mouseY = 200 - event.button.y;
                std::cout << "Clic en la posición X: " << mouseX << ", Y: " << mouseY << std::endl;
                auto it = jugadores.find(turno);
                if (it != jugadores.end())
                {
                    // El elemento en la posición indicada por "turno" existe en el mapa
                    accion = it->second.usar_arma(mouseX, mouseY);
                    cliente.mandar_accion(accion);
                    view.reproducir_efecto_arma(tipo);
                }
            }
        }
    }
    return true;
}

void Game::procesar_estado(StateGame *estado)
{

    if (estado->type == TIPO_TURNO)
    {
        TurnoDTO *turn = dynamic_cast<TurnoDTO *>(estado);
        this->turno = turn->obtenerIdTurno();
        permiso = turn->obtenerPermiso();
        cliente.autorizar_turno(permiso);

        // Le saco el arma a todos los jugadores
        for (auto &[id, jugador] : jugadores)
        {
            jugador.cargar_armas(NO_WEAPON, 0);
        }
    }
    else if (estado->type == TIPO_SEGUNDO)
    {
        SegundosDTO *segundos = dynamic_cast<SegundosDTO *>(estado);
        this->tiempo_restante_turno = segundos->obtenerTiempo();
    }
    else if (estado->type == TIPO_PAQUETE)
    {
        PaqueteDTO *paquete = dynamic_cast<PaqueteDTO *>(estado);
        this->procesar_paquete(paquete);
    }
    else if (estado->type == TIPO_ESCENARIO)
    {
        EscenarioDTO *escenario = dynamic_cast<EscenarioDTO *>(estado);
        this->cargar_escenario(escenario);
    }
    else if (estado->type == TIPO_ARMA)
    {
        ArmaDTO *arma = dynamic_cast<ArmaDTO *>(estado);
        arma->cargar(jugadores);
    }
}

void Game::procesar_paquete(PaqueteDTO *paquete)
{

    std::vector<JugadorDTO> jugadores_paquete = paquete->obtener_jugadores();
    std::set<int> jugadores_en_paquete;
    for (auto &jugador_ : jugadores_paquete)
    {

        JugadorDTO jugador = jugador_;

        gusanoX = jugador.posicion_x();
        gusanoY = jugador.posicion_y();

        int id = jugador.obtenerId();
        jugadores_en_paquete.insert(id); // para borrar al jugador desconectado

        if (jugadores.find(id) == jugadores.end())
        {
            // not found --> lo guardo
            jugadores.emplace(std::make_pair(id, jugador));
        }
        else
        {
            // found --> lo actualizo
            jugadores.at(id).actualizar(jugador);
        }
    }
    for (auto it = jugadores.begin(); it != jugadores.end();)
    {
        if (jugadores_en_paquete.find(it->first) == jugadores_en_paquete.end())
        {
            it = jugadores.erase(it);
        }
        else
        {
            ++it;
        }
    }

    std::vector<ObjetoDTO> objetos_paquete = paquete->obtener_objetos();

    int tamanio_paquete = static_cast<int>(objetos_paquete.size());
    int cantidad_misiles = static_cast<int>(objetos.size());

    // std::set<int> jugadores_en_paquete;

    //itero por los misiles(objetos_paquete) y me fijo si explosion es true.
    //si lo es, creo un objeto explocion en la posicion del misil
    //el objeto explocion, luego de renderizarse, se destruye
    for (auto & proyectil : objetos_paquete) {
        if(proyectil.exploto()){
            Explotion explotion(proyectil.posicion_x(), proyectil.posicion_y());
            explosiones.push_back(explotion);
            view.reproducir_sonido_explosion();
        }
    }

    if (objetos.size() > objetos_paquete.size())
    {
        int misiles_destruidos = objetos.size() - objetos_paquete.size();

        objetos.erase(objetos.begin(), objetos.begin() + misiles_destruidos);
    }

    if (tamanio_paquete > 0)
    {

        for (int i = 0; i < tamanio_paquete; ++i)
        {

            if (i >= cantidad_misiles)
            {

                // No se encontró el índice en "objetos" --> lo guardo
                objetos.push_back(objetos_paquete.at(i));
            }
            else
            {
                // Verificar que objetos.at(i) esté dentro del rango válido
                if (i < cantidad_misiles)
                {
                    objetos.at(i).actualizar(objetos_paquete.at(i));
                }
                else
                {
                    // Manejar el caso donde i es mayor que el tamanio de "objetos"
                    // Esto podría ser un error o requerir alguna lógica adicional según tus necesidades
                    std::cerr << "Error: Índice fuera de rango en 'objetos'." << std::endl;
                }
            }
        }
    }
}

void Game::cargar_escenario(EscenarioDTO *escenario)
{
    std::vector<VigaDTO> vigas_escenario = escenario->obtener_vigas();

    std::vector<ProvisionDTO> provisiones_escenario = escenario->obtener_provisiones();

    for (VigaDTO viga : vigas_escenario)
    {
        vigas.push_back(viga);
    }

    for (ProvisionDTO provision : provisiones_escenario)
    {
        provisiones.push_back(provision);
    }
}

void Game::cargar_arma(PaqueteDTO *paquete)
{
}

void Game::renderizar()
{
    // Centra la vista en la posición actual del gusano
    //view.centrarEnGusano(gusanoX, gusanoY);

    std::string time = "Tiempo restante: " + std::to_string((tiempo_restante_turno)) +
                       " Es mi turno: " + (permiso ? "true" : "false");

    view.renderizar_fondo_pantalla();
    SDL_Color color = {255, 255, 255, 255};
    view.renderizar_texto(time, 0, 0, color);
    // view.renderizar_gusano(0,0);

    if(turno >= 10) {//si el turno es de una id valida
        view.renderizar_municion(jugadores.at(turno));
    }

    for (VigaDTO viga : vigas)
    { // para mostrar las vigas
        view.renderizar_viga(viga);
    }

    for (ProvisionDTO provision : provisiones)
    {
        view.renderizar_provision(provision);
    }

    for (auto const &[id, jugador] : jugadores)
    { // para mostrar los jugadores

        view.renderizar_gusano(jugador);
    }

    this->renderizar_misiles();

    for (Explotion& explotion : explosiones)
    { // para mostrar las vigas
        view.renderizar_explocion(explotion);
    }
    //chequeo si alguna termino y la borro
    for (auto iter = explosiones.begin(); iter != explosiones.end(); ) {
        if (iter->finalizo()) {
            iter = explosiones.erase(iter);
        } else {
            ++iter;
        }
    }

    view.mostrar();
}

void Game::renderizar_misiles()
{

    for (ObjetoDTO misil : objetos)
    { // para mostrar las vigas
        view.renderizar_misil(misil);
    }
}