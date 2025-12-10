// SubsistemaDados.cpp

#include "SubsistemaDados.hpp"
#include "TemplateMonitoramento.hpp" // Inclui a estrutura do Template Method
#include "Logger.hpp"

// Construtor: Armazena as dependências que serão passadas para o Template Method.
SubsistemaDados::SubsistemaDados(LeitorImagemSHA* leitor, SubsistemaAlerta* alerta, ConsumoHistoricoDAO* dao)
    : leitorSHA(leitor), subsistemaAlerta(alerta), historicoDAO(dao) {
    
    Logger::getInstance()->registrarInfo("SubsistemaDados", "Subsistema inicializado e pronto para iniciar fluxos.");
}

// =========================================================================
// Método de Entrada  - Cliente do Template Method
// =========================================================================

void SubsistemaDados::iniciarProcessamento(const std::string& idSHA, int idUsuario) {
    
    Logger::getInstance()->registrarInfo("SubsistemaDados", "Acionando Template Method para SHA: " + idSHA);
    
    // 1. Cria a instância concreta do Template Method (Processador)
    LeituraSHAProcessador processor(leitorSHA, subsistemaAlerta, historicoDAO);
    
    // 2. Executa o fluxo fixo (Leitura -> Registro -> Alerta)
    // Este método é herdado de FluxoProcessamentoBase e executa a sequência imutável.
    processor.iniciar(idSHA, idUsuario); 
}

// =========================================================================
// Consulta de Histórico 
// =========================================================================

ConsumoDTO SubsistemaDados::monitorarConsumoUsuario(int idUsuario, std::time_t inicio, std::time_t fim) {
    // Esta lógica deve obter a lista de SHAs do usuário (via SubsistemaUsuarios)
    // e chamar o ConsumoHistoricoDAO para agregar os dados.
    
    Logger::getInstance()->registrarInfo("SubsistemaDados", "Consulta consolidada de consumo para ID: " + std::to_string(idUsuario));
    
    // Em um projeto real, esta linha chamaria o DAO para fazer a agregação SQL:
    // ConsumoHistoricoDAO->agregarConsumoPorUsuario(idUsuario, inicio, fim);
    
    return ConsumoDTO{150.75}; // Retorno simulado
}