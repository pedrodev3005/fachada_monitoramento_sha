// TemplateMonitoramento.hpp

#ifndef TEMPLATE_MONITORAMENTO_H
#define TEMPLATE_MONITORAMENTO_H

#include "LeitorImagemSHA.hpp"
#include "EstrategiasOCR.hpp"
#include "SubsistemaAlerta.hpp" // Dependência para o passo final
#include "ConsumoHistoricoDAO.hpp" // Dependência para persistência

// --- Classe Abstrata (Define o Esqueleto) ---
// EM TemplateMonitoramento.hpp (ou o seu arquivo de cabeçalho do Template)

class FluxoProcessamentoBase {
protected:
    // O Hook de verificação de limite precisa de 3 argumentos
    virtual double extrairConsumo(const Imagem& imagem) = 0;
    virtual void verificarLimite(int idUsuario, const std::string& idSHA, double volume) = 0; // CORRIGIDO

    // O passo concreto precisa de 2 argumentos
    void salvarHistorico(int idUsuario, const LeituraConsumo& leitura, ConsumoHistoricoDAO* dao) { // CORRIGIDO
        dao->salvarLeitura(idUsuario, leitura); // CORRIGIDO
        Logger::getInstance()->registrarInfo("Template", "Leitura salva no histórico.");
    }
    
public:
    // O método template precisa ser atualizado para o novo salvarHistorico
    void executarFluxo(const std::string& idSHA, int idUsuario, LeitorImagemSHA* leitor, ConsumoHistoricoDAO* dao, SubsistemaAlerta* alerta);
};

// --- Classe Concreta (Implementa os Hooks) ---
class LeituraSHAProcessador : public FluxoProcessamentoBase {
private:
    // Referências necessárias para os Hooks
    LeitorImagemSHA* leitorSHA;
    SubsistemaAlerta* subsistemaAlerta; 
    ConsumoHistoricoDAO* historicoDAO;

    // Atributos internos
    InterpretadorConsumo* interpretadorAtual = nullptr;

protected:
    double extrairConsumo(const Imagem& imagem) override;
    void verificarLimite(int idUsuario, const std::string& idSHA, double volume) override;

public:
    LeituraSHAProcessador(LeitorImagemSHA* leitor, SubsistemaAlerta* alerta, ConsumoHistoricoDAO* dao) 
        : leitorSHA(leitor), subsistemaAlerta(alerta), historicoDAO(dao) {}

    // Delega para o Template base a execução
    void iniciar(const std::string& idSHA, int idUsuario) {
        executarFluxo(idSHA, idUsuario, leitorSHA, historicoDAO, subsistemaAlerta);
    }
};

#endif // TEMPLATE_MONITORAMENTO_H