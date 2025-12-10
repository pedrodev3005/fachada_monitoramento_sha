// SubsistemaAlerta.hpp

#ifndef SUBSISTEMA_ALERTA_H
#define SUBSISTEMA_ALERTA_H

#include "NotificacaoFactory.hpp"
#include "Entidades.hpp"
#include "Logger.hpp"

// Simulação do DAO de limites
class LimiteAlertaDAO {
public:
    bool salvarLimite(int idUsuario, double limite) { return true; }
    double buscarLimite(int idUsuario) { return 70.0; } // Valor padrão
};

/**
 * @brief Gerencia os limites de consumo e dispara notificações utilizando o Factory Method.
 */
class SubsistemaAlerta {
private:
    NotificadorFactory* notificadorFactory;
    LimiteAlertaDAO* limiteDAO; 

public:
    SubsistemaAlerta(NotificadorFactory* factory, LimiteAlertaDAO* dao) 
        : notificadorFactory(factory), limiteDAO(dao) {
        Logger::getInstance()->registrarInfo("SubsistemaAlerta", "Subsistema inicializado.");
    }

    bool definirLimite(int idUsuario, double limiteVolumeM3);
    void notificarAlerta(int idUsuario, const AlertaConsumo& alerta, CanalAlerta canal);
    bool verificarLimiteExcedido(int idUsuario, const std::string& idSHA, double volumeAtual);
};

#endif // SUBSISTEMA_ALERTA_H