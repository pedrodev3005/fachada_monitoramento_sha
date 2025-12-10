// UsuarioDAOImpl.cpp

#include "UsuarioDAOImpl.hpp" // Define a classe concreta
#include <sstream>
#include <stdexcept>

// =======================================================
// Implementação dos Métodos
// =======================================================

UsuarioDAOImpl::UsuarioDAOImpl(DBConnection* conn) : conexaoDB(conn) {
    if (!conn) {
        Logger::getInstance()->registrarErro("UsuarioDAOImpl", "ConexaoDB nula ao inicializar DAO.");
        throw std::runtime_error("Conexao DB nao pode ser nula.");
    }
    Logger::getInstance()->registrarInfo("UsuarioDAOImpl", "DAO de Usuários inicializado com sucesso.");
}

bool UsuarioDAOImpl::salvar(const Usuario& usuario) {
    // 1. Simulação da query SQL
    std::string sql = "INSERT INTO Usuarios (nome, cpf) VALUES ('" + usuario.nome + "', '" + usuario.cpf + "')";
    
    // 2. Execução da query
    if (conexaoDB->executarQuery(sql)) {
        Logger::getInstance()->registrarEventoCritico("UsuarioDAOImpl", "Usuario " + usuario.nome + " salvo com sucesso.");
        return true;
    }
    
    Logger::getInstance()->registrarErro("UsuarioDAOImpl", "Falha ao salvar usuário no RDB.");
    return false;
}

Usuario UsuarioDAOImpl::buscarPorID(int idUsuario) {
    // Simulação da busca no DB
    std::string sql = "SELECT nome, cpf FROM Usuarios WHERE idUsuario = " + std::to_string(idUsuario);
    conexaoDB->executarQuery(sql);
    
    // Simula o retorno de um objeto preenchido
    Usuario u{idUsuario, "Usuario Retornado", "000.000.000-00", {}};
    
    Logger::getInstance()->registrarInfo("UsuarioDAOImpl", "Busca de Usuario ID: " + std::to_string(idUsuario));
    return u;
}

bool UsuarioDAOImpl::vincularSHA(int idUsuario, const std::string& idSHA) {
    // Simulação da query para criar a relação (Chave Estrangeira)
    std::string sql = "INSERT INTO Contas (idUsuario, idSHA) VALUES (" + std::to_string(idUsuario) + ", '" + idSHA + "')";
    
    if (conexaoDB->executarQuery(sql)) {
        Logger::getInstance()->registrarInfo("UsuarioDAOImpl", "SHA " + idSHA + " vinculado.");
        return true;
    }
    
    Logger::getInstance()->registrarErro("UsuarioDAOImpl", "Falha ao vincular SHA: " + idSHA);
    return false;
}

std::vector<Conta> UsuarioDAOImpl::obterContasPorUsuario(int idUsuario) {
    // Simulação de busca de todas as contas do usuário
    std::string sql = "SELECT idSHA FROM Contas WHERE idUsuario = " + std::to_string(idUsuario);
    conexaoDB->executarQuery(sql);
    
    // Retorno simulado de SHAs
    Logger::getInstance()->registrarInfo("UsuarioDAOImpl", "Obtidas contas para ID: " + std::to_string(idUsuario));
    return {
        {"SHA123", idUsuario}, 
        {"SHA456", idUsuario}
    };
}