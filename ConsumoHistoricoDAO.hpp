// ConsumoHistoricoDAO.hpp

#ifndef CONSUMO_HISTORICO_DAO_HPP
#define CONSUMO_HISTORICO_DAO_HPP

#include "DBConnection.hpp"
#include "Entidades.hpp"
#include "Logger.hpp"
#include <vector>

// Forward declaration (para evitar ciclos de inclusão, se necessário)
struct ConsumoDTO; 

/**
 * @brief Implementação do Padrão DAO para persistência das Leituras de Consumo.
 */
class ConsumoHistoricoDAO {
private:
    DBConnection* conexaoDB;

public:
    ConsumoHistoricoDAO(DBConnection* conn);

    /**
     * @brief Salva uma nova leitura de consumo no banco de dados, vinculando-a ao usuário.
     * @param idUsuario ID do usuário proprietário da leitura.
     * @param leitura DTO com o volume e o idSHA.
     */
    bool salvarLeitura(int idUsuario, const LeituraConsumo& leitura);

    /**
     * @brief Busca o histórico para consumo consolidado (simulado).
     */
    ConsumoDTO agregarConsumoPorPeriodo(int idUsuario, std::time_t inicio, std::time_t fim);
};

#endif // CONSUMO_HISTORICO_DAO_HPP