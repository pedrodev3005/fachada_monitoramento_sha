// NotificacaoFactory.cpp

#include "NotificacaoFactory.hpp"
#include "Notificadores.hpp" // Inclui as classes concretas de notificação

ServicoNotificacao* NotificadorFactory::criarNotificador(CanalAlerta tipoCanal) {
    Logger::getInstance()->registrarInfo("NotificacaoFactory", "Criando notificador para canal: " + std::to_string(tipoCanal));
    
    switch (tipoCanal) {
        case USUARIO:
            // Produto Concreto: Notificação via E-mail/SMS (usa Mailio, um Adapter)
            return new NotificadorEmail();
            
        case EXTERNO:
            // Produto Concreto: Notificação via API HTTP
            return new NotificadorAPIExterna();
            
        case CAGEPA:
            // Produto Concreto: Notificação interna (para o painel da concessionária)
            return new NotificadorInterno(); 
            
        default:
            Logger::getInstance()->registrarErro("NotificacaoFactory", "Tipo de canal desconhecido.");
            return nullptr; 
    }
}