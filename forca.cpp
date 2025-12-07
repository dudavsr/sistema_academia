#include "forca.h"
#include <iostream>
#include <iomanip>

// Construtor para novos cadastros
Forca::Forca(std::string nome, double carga, int series, int repeticoes, int tempoDescanso)
    : Exercicio(nome), carga(carga), series(series), repeticoes(repeticoes), tempoDescanso(tempoDescanso) {
}

// Construtor para leitura de arquivo
Forca::Forca(int id, std::string nome, bool ativo, double carga, int series, int repeticoes, int tempoDescanso)
    : Exercicio(id, nome, ativo), carga(carga), series(series), repeticoes(repeticoes), tempoDescanso(tempoDescanso) {
}

// Exibir detalhes do exercício
void Forca::exibirDetalhes() const {
    std::cout << "ID: " << getId() 
              << " | " << getNome() 
              << " (Forca)"
              << " | Carga: " << carga << "kg"
              << " | Series: " << series << "x" << repeticoes
              << " | Descanso: " << tempoDescanso << "s"
              << " | Status: " << (isAtivo() ? "Ativo" : "Inativo")
              << std::endl;
}

// Calcular tempo estimado (em minutos)
double Forca::calcularTempoEstimado() const {
// Fórmula: (Séries * Repetições * 3 seg) + (Séries * Tempo de Descanso)
// Assumimos 3 segundos por repetição
    double tempoTotalSegundos = (series * repeticoes * 3.0) + (series * tempoDescanso);
    
// Converter para minutos
    return tempoTotalSegundos / 60.0;
}

// Calcular calorias gastas
double Forca::calcularCaloriasGastas() const {
// Fórmula: Séries * Repetições * Carga * 0.15 
    return series * repeticoes * carga * 0.15;
}

// Retornar tipo do exercício (2 = Força)
int Forca::getTipo() const {
    return 2;
}

// Getters modificadores
double Forca::getCarga() const { 
    return carga; 
}

int Forca::getSeries() const { 
    return series; 
}

int Forca::getRepeticoes() const { 
    return repeticoes; 
}

int Forca::getTempoDescanso() const { 
    return tempoDescanso; 
}