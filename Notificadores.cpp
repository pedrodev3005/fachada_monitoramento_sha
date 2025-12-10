// Notificadores.cpp

#include "Notificadores.hpp"
#include <iostream>
#include <sstream>

// =======================================================
// NotificadorEmail (Usa Adapter para SMTP)
// =======================================================
void NotificadorEmail::enviarAlerta(const AlertaConsumo& alerta) {
    std::string destinatario = "usuario_" + std::to_string(alerta.idUsuario) + "@email.com";
    std::string assunto = "ALERTA CAGEPA: Consumo Excedido no SHA " + alerta.idHidrometro;
    std::string corpo = "Seu consumo (" + std::to_string(alerta.volumeExcedido) + " m3) ultrapassou o limite.";

    clientSMTP.conectar("smtp.servidor.com");
    // clientSMTP.autenticar(...)
    
    if (clientSMTP.enviar(destinatario, assunto, corpo)) {
        Logger::getInstance()->registrarEventoCritico("NotificadorEmail", 
                                                "Alerta enviado por email para ID " + std::to_string(alerta.idUsuario));
    } else {
        Logger::getInstance()->registrarErro("NotificadorEmail", "Falha ao enviar e-mail via SMTP.");
    }
}

// =======================================================
// NotificadorAPIExterna
// =======================================================
void NotificadorAPIExterna::enviarAlerta(const AlertaConsumo& alerta) {
    // [Implementar chamada HTTP/cURL para API externa]
    Logger::getInstance()->registrarInfo("NotificadorAPIExterna", 
                                        "Alerta enviado para API externa de terceiros.");
}

// =======================================================
// NotificadorInterno (Para Painel CAGEPA)
// =======================================================
void NotificadorInterno::enviarAlerta(const AlertaConsumo& alerta) {
    // [Implementar: Inserir registro na tabela de notificações internas da CAGEPA]
    Logger::getInstance()->registrarEventoCritico("NotificadorInterno", 
                                                "ALERTA INTERNO CAGEPA: Volume: " + std::to_string(alerta.volumeExcedido));
}