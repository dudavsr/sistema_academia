#include "ficha.h"
#include <iostream>
#include <iomanip>

int Ficha::proximoId = 1;

// Construtor para novos cadastros
Ficha::Ficha(std::string nome) {
    this->id = proximoId++;
    this->nome = nome;
}

// Construtor para leitura de arquivo
Ficha::Ficha(int id, std::string nome) {
    this->id = id;
    this->nome = nome;
    
    // Sincronização do contador estático
    if (id >= proximoId) {
        proximoId = id + 1;
    }
}

// Destrutor
Ficha::~Ficha() {
    exercicios.clear();
}

// Adicionar exercício à ficha
void Ficha::adicionarExercicio(Exercicio* exercicio) {
    if (exercicio != nullptr) {
        exercicios.push_back(exercicio);
    }
}

// Exibir ficha completa com exercícios e totais
void Ficha::exibirFicha() const {
    std::cout << "=== FICHA " << id << ": " << nome << " ===" << std::endl;
    
    if (exercicios.empty()) {
        std::cout << " (Ficha vazia)" << std::endl;
    } else {
        for (const auto* ex : exercicios) {
            ex->exibirDetalhes();
        }
    }
    
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "RESUMO:" << std::endl;
    std::cout << "Tempo Total Estimado: " << std::fixed << std::setprecision(1) 
              << calcularTempoTotal() << " min" << std::endl;
    std::cout << "Gasto Calorico Total: " << std::fixed << std::setprecision(1) 
              << calcularCaloriasTotais() << " cal" << std::endl;
    std::cout << "================================================" << std::endl;
}

// Calcular tempo total da ficha
double Ficha::calcularTempoTotal() const {
    double total = 0.0;
    for (const auto* ex : exercicios) {
        total += ex->calcularTempoEstimado();
    }
    return total;
}

// Calcular calorias totais da ficha
double Ficha::calcularCaloriasTotais() const {
    double total = 0.0;
    for (const auto* ex : exercicios) {
        total += ex->calcularCaloriasGastas();
    }
    return total;
}

// Getters implementation
int Ficha::getId() const { 
    return id; 
}

std::string Ficha::getNome() const { 
    return nome; 
}

const std::vector<Exercicio*>& Ficha::getExercicios() const {
    return exercicios;
}

void Ficha::atualizarProximoId(int maiorId) {
    if (maiorId >= proximoId) {
        proximoId = maiorId + 1;
    }
}