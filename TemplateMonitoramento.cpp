// TemplateMonitoramento.cpp

#include "TemplateMonitoramento.hpp"
#include <stdexcept>

// =======================================================
// Implementação da Classe Base (Fluxo Fixo)
// =======================================================

// método Template é implementado aqui
void FluxoProcessamentoBase::executarFluxo(const std::string& idSHA, int idUsuario, 
                                          LeitorImagemSHA* leitor, ConsumoHistoricoDAO* dao, 
                                          SubsistemaAlerta* alerta) {
    
    // 1. Obter Imagem 
    Logger::getInstance()->registrarInfo("Template", "INICIANDO FLUXO: " + idSHA);
    
    // O Adaptador é usado para buscar a imagem (cumpre a restrição SHA)
    Imagem imagem = leitor->obterImagem(idSHA);
    
    // 2. Extrair Consumo (Hook Abstrato - Implementado pela Strategy)
   double volume = extrairConsumo(imagem);
    try {
        volume = extrairConsumo(imagem); // Chama o Hook da subclasse
    } catch (const std::exception& e) {
        Logger::getInstance()->registrarErro("Template", "Falha na extração (OCR): " + std::string(e.what()));
        return; // Falha na leitura, aborta o fluxo
    }

    // 3. Salvar Histórico 
    LeituraConsumo leitura = {idSHA, volume, std::time(nullptr)};
    salvarHistorico(idUsuario, leitura, dao); // CORRIGIDO: Passando idUsuario

    // 4. Verificar Alerta (Hook Abstrato - Implementado pelo Subsistema Alerta)
    verificarLimite(idUsuario, idSHA, volume);
    
    Logger::getInstance()->registrarInfo("Template", "FLUXO CONCLUÍDO: Volume: " + std::to_string(volume) + " m3.");
}

// =======================================================
// Implementação da Classe Concreta (LeituraSHAProcessador)
// =======================================================

// Hook 1: Lógica de Extração de Consumo (usa o Padrão Strategy)
double LeituraSHAProcessador::extrairConsumo(const Imagem& imagem) {
    // 1. Determina qual Strategy usar (lógica de seleção)
    TipoMedidor tipo = leitorSHA->determinarTipoMedidor(imagem.caminhoArquivo);
    
    if (interpretadorAtual) {
        delete interpretadorAtual; // Limpa a estratégia anterior, se houver
        interpretadorAtual = nullptr;
    }
    
    // 2. Cria a Strategy correta
    if (tipo == TipoMedidor::DIGITAL) {
        interpretadorAtual = new InterpretadorDigital();
    } else {
        interpretadorAtual = new InterpretadorAnalogico();
    }
    
    // 3. Executa a Strategy
    return interpretadorAtual->interpretar(imagem);
}

// Hook 2: Lógica de Verificação de Limite (usa o SubsistemaAlerta)
void LeituraSHAProcessador::verificarLimite(int idUsuario, const std::string& idSHA, double volume) {
    // O Template delega a responsabilidade de verificação e notificação para o SubsistemaAlerta
    subsistemaAlerta->verificarLimiteExcedido(idUsuario, idSHA, volume);
}
