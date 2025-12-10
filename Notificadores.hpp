// Notificadores.hpp

#ifndef NOTIFICADORES_H
#define NOTIFICADORES_H

#include "NotificacaoFactory.hpp"
#include "Logger.hpp"

// Simulação da biblioteca externa (Mailio/SMTP)
class MailioClient {
public:
    void conectar(const std::string& servidor) { /* ... */ }
    void autenticar(const std::string& user, const std::string& pass) { /* ... */ }
    bool enviar(const std::string& destinatario, const std::string& assunto, const std::string& corpo) { return true; }
};

// --- Implementação do Notificador Email (Adapter/Produto Concreto) ---

class NotificadorEmail : public ServicoNotificacao {
private:
    MailioClient clientSMTP; // Adapter para a biblioteca externa

public:
    void enviarAlerta(const AlertaConsumo& alerta) override;
};

// --- Implementação do Notificador API Externa (Produto Concreto) ---

class NotificadorAPIExterna : public ServicoNotificacao {
public:
    void enviarAlerta(const AlertaConsumo& alerta) override;
};

// --- Implementação do Notificador Interno (Produto Concreto - CAGEPA) ---

class NotificadorInterno : public ServicoNotificacao {
public:
    void enviarAlerta(const AlertaConsumo& alerta) override;
};

#endif // NOTIFICADORES_H