// SubsistemaDados.h

#ifndef SUBSISTEMA_DADOS_H
#define SUBSISTEMA_DADOS_H

#include "LeitorImagemSHA.hpp"    // Adapter
#include "SubsistemaAlerta.hpp"   // Subsistema para Alerta
#include "ConsumoHistoricoDAO.hpp" // DAO para Persistência

// Forward declarations de classes de fluxo
class LeituraSHAProcessador; 
struct ConsumoDTO; // Struct para retorno consolidado

class SubsistemaDados {
private:
    // Referências necessárias para injetar no Template Method:
    LeitorImagemSHA* leitorSHA;
    SubsistemaAlerta* subsistemaAlerta; 
    ConsumoHistoricoDAO* historicoDAO;

public:
    // Construtor que recebe as dependências
    SubsistemaDados(LeitorImagemSHA* leitor, SubsistemaAlerta* alerta, ConsumoHistoricoDAO* dao);

    // Método de Entrada para a Fachada (RF 2.1)
    void iniciarProcessamento(const std::string& idSHA, int idUsuario);
    
    // Método para Consulta Consolidada (RF 2.3)
    ConsumoDTO monitorarConsumoUsuario(int idUsuario, std::time_t inicio, std::time_t fim);
};

#endif // SUBSISTEMA_DADOS_H