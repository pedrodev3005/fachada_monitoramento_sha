// NotificacaoFactory.hpp

#ifndef NOTIFICACAO_FACTORY_H
#define NOTIFICACAO_FACTORY_H

#include "Entidades.hpp" // Inclui AlertaConsumo e CanalAlerta
#include "Logger.hpp"

// --- I. Interface do Produto (Contrato de Envio) ---

/**
 * @brief Define a interface comum para todos os canais de notificação.
 * (Produto Abstrato do Factory Method).
 */
class ServicoNotificacao {
public:
    virtual ~ServicoNotificacao() = default;
    
    /**
     * @brief Envia o alerta de consumo para o destino específico do canal.
     */
    virtual void enviarAlerta(const AlertaConsumo& alerta) = 0;
};


// --- II. Classe Fábrica (Criador) ---

/**
 * @brief Implementa o Padrão Factory Method para criar instâncias de ServicoNotificacao.
 */
class NotificadorFactory {
public:
    /**
     * @brief Cria uma instância do notificador concreto baseado no tipo de canal.
     * @param tipoCanal O destino desejado (USUARIO, CAGEPA, EXTERNO).
     * @return Ponteiro para o objeto ServicoNotificacao criado.
     */
    ServicoNotificacao* criarNotificador(CanalAlerta tipoCanal);
};

#endif // NOTIFICACAO_FACTORY_H