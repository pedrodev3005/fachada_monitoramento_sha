// EstrategiasOCR.hpp

#ifndef ESTRATEGIAS_OCR_H
#define ESTRATEGIAS_OCR_H

#include "InterpretadorConsumo.hpp"
#include "Logger.hpp"

// --- Implementação para Medidores Digitais ---
class InterpretadorDigital : public InterpretadorConsumo {
public:
    double interpretar(const Imagem& imagem) override {
        // Lógica OCR: Processar dígitos digitais (ex: 000.00)
        Logger::getInstance()->registrarInfo("Strategy:Digital", "Processando leitura digital...");
        return 95.5; // Retorna valor simulado
    }
};

// --- Implementação para Medidores Analógicos ---
class InterpretadorAnalogico : public InterpretadorConsumo {
private:
    std::string modeloOCR; // Atributo específico para o modelo de Visão Computacional

public:
    InterpretadorAnalogico() : modeloOCR("Modelo P1.0") {}
    
    double interpretar(const Imagem& imagem) override {
        // Lógica OCR: Processar ponteiros de relógio 
        Logger::getInstance()->registrarInfo("Strategy:Analogico", "Processando leitura analógica com modelo: " + modeloOCR);
        return 120.8; // Retorna valor simulado
    }
};

#endif // ESTRATEGIAS_OCR_H