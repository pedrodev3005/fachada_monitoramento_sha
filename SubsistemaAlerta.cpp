// SubsistemaAlerta.cpp

#include "SubsistemaAlerta.hpp"

bool SubsistemaAlerta::definirLimite(int idUsuario, double limiteVolumeM3) {
    // Persiste o limite definido
    bool sucesso = limiteDAO->salvarLimite(idUsuario, limiteVolumeM3);
    if (sucesso) {
        Logger::getInstance()->registrarInfo("SubsistemaAlerta", "Limite de " + std::to_string(limiteVolumeM3) + "m3 definido para ID " + std::to_string(idUsuario));
    }
    return sucesso;
}

void SubsistemaAlerta::notificarAlerta(int idUsuario, const AlertaConsumo& alerta, CanalAlerta canal) {
    //Usa a Factory para obter o objeto de envio correto
    ServicoNotificacao* notificador = notificadorFactory->criarNotificador(canal);

    if (notificador) {
        //Chama o método de envio (polimorfismo)
        notificador->enviarAlerta(alerta);
    }
}

// Método que será chamado pelo Template Method do Subsistema de Dados
bool SubsistemaAlerta::verificarLimiteExcedido(int idUsuario, const std::string& idSHA, double volumeAtual) {
    double limite = limiteDAO->buscarLimite(idUsuario);
    
    if (volumeAtual > limite) {
        // Gera o DTO de Alerta
        AlertaConsumo alerta = {
            idUsuario, 
            "", // SHA ID seria preenchido antes
            volumeAtual, 
            "Consumo excedeu o limite de " + std::to_string(limite) + " m3.",
            CAGEPA // Alerta padrão para a concessionária
        };
        
        // Dispara o alerta para a concessionária e o usuário
        notificarAlerta(idUsuario, alerta, CAGEPA);
        notificarAlerta(idUsuario, alerta, USUARIO);
        
        return true;
    }
    return false;
}