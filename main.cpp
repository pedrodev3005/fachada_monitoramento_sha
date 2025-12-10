// main.cpp - VERSÃO CLI INTERATIVA

#include "MonitoramentoFacade.hpp"
#include "DBConnection.hpp"
#include "UsuarioDAOImpl.hpp" // Implementação do DAO
#include "SubsistemaUsuarios.hpp"
#include "NotificacaoFactory.hpp"
#include "SubsistemaAlerta.hpp"
#include "LeitorImagemSHA.hpp"
#include "ConsumoHistoricoDAO.hpp" 
#include "SubsistemaDados.hpp"
#include <iostream>
#include <limits> // Para limpar o buffer
#include "sqlite3.h"

// Funções para a Interface de Usuário
void exibirMenu() {
    std::cout << "\n=============================================" << std::endl;
    std::cout << "        PAINEL DE MONITORAMENTO (CLI)" << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << "1. Criar Novo Usuario (Nome e CPF)" << std::endl;
    std::cout << "2. Definir Limite de Alerta para um ID" << std::endl;
    std::cout << "3. PROCESSAR LEITURA (ID do Usuario e SHA)" << std::endl;
    std::cout << "4. Simular Consulta Consolidada (RF 2.3)" << std::endl;
    std::cout << "5. VISUALIZAR DADOS BRUTOS (SQLITE)" << std::endl; // <-- NOVA OPÇÃO
    std::cout << "0. Sair" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "Digite sua opcao: ";
}

// Função de Callback para exibir os resultados da query
static int callback(void *data, int argc, char **argv, char **azColName) {
    // data: string opcional que você passa
    // argc: número de colunas
    // argv: array com os valores da linha (como strings)
    // azColName: array com os nomes das colunas

    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
    }
    std::cout << "\n";
    return 0;
}

// Função de acesso direto ao DB
void visualizarDadosBrutos(const char* dbPath) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open(dbPath, &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Erro ao abrir DB: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // A query para visualização:
    const char* sql = 
        "SELECT idUsuario, nome, cpf FROM Usuarios;"
        "SELECT idSHA, volume, dataHora FROM Historico;"
        "SELECT idUsuario, limiteVolumeM3 FROM Limites;";
    
    std::cout << "\n--- DADOS BRUTOS DO BANCO (" << dbPath << ") ---" << std::endl;
    
    // ATENÇÃO: sqlite3_exec pode não ser ideal para múltiplos SELECTs. 
    // Vamos simplificar para consultar apenas a tabela USUARIOS e HISTORICO.

    std::cout << "\n[Tabela USUARIOS]" << std::endl;
    const char* sql_users = "SELECT idUsuario, nome, cpf FROM Usuarios;";
    rc = sqlite3_exec(db, sql_users, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL erro (USUARIOS): " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
    
    std::cout << "\n[Tabela HISTORICO (Leituras)]" << std::endl;
    const char* sql_historico = "SELECT idSHA, volume FROM Historico;";
    rc = sqlite3_exec(db, sql_historico, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL erro (HISTORICO): " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
    
    sqlite3_close(db);
    std::cout << "-----------------------------------------------" << std::endl;
}


void processarComando(int comando, MonitoramentoFacade& fachada) {
    int idUsuario;
    std::string nome, cpf, idSHA;
    double limite;

    switch (comando) {
        case 1: {
            std::cout << "-> ID do novo usuario (ex: 102): ";
            std::cin >> idUsuario;
            std::cout << "-> Nome do usuario: ";
            // Usa std::ws para consumir espaços em branco antes de ler a string completa
            std::cin >> std::ws; 
            std::getline(std::cin, nome);
            std::cout << "-> CPF (sem formatacao): ";
            std::cin >> cpf;
            
            Usuario novoUser = {idUsuario, nome, cpf, {}};
            fachada.criarUsuario(novoUser);
            break;
        }
        case 2: {
            std::cout << "-> ID do usuario para limite: ";
            std::cin >> idUsuario;
            std::cout << "-> Limite de consumo (m3, ex: 80.0): ";
            std::cin >> limite;
            fachada.definirLimiteAlerta(idUsuario, limite);
            break;
        }
        case 3: {
            std::cout << "-> ID do usuario para monitorar: ";
            std::cin >> idUsuario;
            std::cout << "-> ID do SHA (ex: SHA-DIG-456): ";
            std::cin >> idSHA;
            
            std::cout << "Executando Template Method de Monitoramento..." << std::endl;
            fachada.processarLeituraDiaria(idSHA, idUsuario); // Inicia o fluxo fixo
            break;
        }
        case 4: {
            std::cout << "-> ID do usuario para consulta: ";
            std::cin >> idUsuario;
            
            // Simula consulta de historico (datas fixas para simplificar o input)
            ConsumoDTO c = fachada.monitorarConsumoUsuario(idUsuario, 0, 0); 
            std::cout << "-> Resultado da Consulta Consolidada: " << c.totalConsumido << " m3." << std::endl;
            break;
        }
        case 5: { // <-- NOVO CASO
            // Passa o caminho do arquivo DB. Assumimos que a conexão é "monitoramento.db"
            visualizarDadosBrutos("monitoramento.db"); 
            break;
        }
        
        case 0:
            std::cout << "Encerrando sistema." << std::endl;
            break;
        default:
            std::cout << "Opcao invalida. Tente novamente." << std::endl;
    }
}
void inicializarSistema() {
    // --- 1. Inicializa dependências ---
    DBConnection conn;
    ConsumoHistoricoDAO cDao(&conn);
    LimiteAlertaDAO lDao;
    
    // 2. Inicializa Subsistemas
    UsuarioDAOImpl uDaoImpl(&conn);
    SubsistemaUsuarios subsUsuarios(&uDaoImpl);
    
    NotificadorFactory notifFactory;
    SubsistemaAlerta subsAlerta(&notifFactory, &lDao);
    
    LeitorImagemSHA leitor;
    SubsistemaDados subsDados(&leitor, &subsAlerta, &cDao);
    
    // 3. Cria a Fachada
    MonitoramentoFacade fachada(&subsUsuarios, &subsAlerta, &subsDados);
    
    Logger::getInstance()->registrarInfo("MAIN", "Arquitetura pronta. CLI ativada.");

    // --- 4. Loop Interativo ---
    int comando = -1;
    while (comando != 0) {
        exibirMenu();
        if (!(std::cin >> comando)) {
            // Trata input não numérico
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            comando = -1;
        }
        if (comando != 0) {
            processarComando(comando, fachada);
        }
    }
}

int main() {
    inicializarSistema();
    return 0;
}