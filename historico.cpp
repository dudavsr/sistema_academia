#include "historico.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

// Adicionar registro ao histórico (em memória)
void Historico::adicionarRegistro(const RegistroTreino& registro) {
    registros.push_back(registro);
}

// Exibir histórico completo no console
void Historico::exibirHistorico() const {
    std::cout << "=== HISTORICO DE TREINOS ===" << std::endl;
    
    if (registros.empty()) {
        std::cout << "Nenhum treino registrado ainda." << std::endl;
    } else {
        std::cout << std::left 
                  << std::setw(20) << "DATA/HORA"
                  << std::setw(20) << "FICHA"
                  << std::setw(10) << "TEMPO"
                  << std::setw(10) << "CALORIAS" 
                  << std::endl;
        std::cout << "------------------------------------------------------------" << std::endl;

        for (const auto& reg : registros) {
            std::cout << std::left 
                      << std::setw(20) << reg.dataHora
                      << std::setw(20) << reg.nomeFicha
                      << std::fixed << std::setprecision(1)
                      << reg.tempoTotal << " min   "
                      << reg.caloriasTotal << " cal"
                      << std::endl;
        }
    }
    std::cout << "============================" << std::endl;
}

// Getter de registros
const std::vector<RegistroTreino>& Historico::getRegistros() const {
    return registros;
}

// Carregar histórico do arquivo historico.txt
void Historico::carregarDeArquivo() {
    std::ifstream arquivo("historico.txt");
    
    // Se o arquivo não existir (primeira execução), não faz nada
    if (!arquivo.is_open()) {
        return;
    }

    std::string linha;
    while (std::getline(arquivo, linha)) {
        if (linha.empty()) continue;

        std::stringstream ss(linha);
        std::string segmento;
        RegistroTreino reg;

        // Formato: DATA;ID_FICHA;NOME_FICHA;TEMPO_TOTAL;CALORIAS_TOTAL
        
        // 1. Data
        if (std::getline(ss, segmento, ';')) reg.dataHora = segmento;
        
        // 2. ID Ficha
        if (std::getline(ss, segmento, ';')) reg.idFicha = std::stoi(segmento);
        
        // 3. Nome Ficha
        if (std::getline(ss, segmento, ';')) reg.nomeFicha = segmento;
        
        // 4. Tempo Total
        if (std::getline(ss, segmento, ';')) reg.tempoTotal = std::stod(segmento);
        
        // 5. Calorias Total
        if (std::getline(ss, segmento, ';')) reg.caloriasTotal = std::stod(segmento);

        registros.push_back(reg);
    }
    
    arquivo.close();
    std::cout << "Historico carregado: " << registros.size() << " registros." << std::endl;
}

// Salvar histórico no arquivo historico.txt
void Historico::salvarEmArquivo() const {
    std::ofstream arquivo("historico.txt");
    
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao salvar historico!" << std::endl;
        return;
    }

    for (const auto& reg : registros) {
        arquivo << reg.dataHora << ";"
                << reg.idFicha << ";"
                << reg.nomeFicha << ";"
                << reg.tempoTotal << ";"
                << reg.caloriasTotal << std::endl;
    }
    
    arquivo.close();
}