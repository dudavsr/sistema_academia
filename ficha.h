#ifndef FICHA_H
#define FICHA_H

#include <string>
#include <vector>
#include "exercicio.h"

class Ficha {
private:
    int id;
    std::string nome;
    std::vector<Exercicio*> exercicios;
    
    static int proximoId;

public:
    // Construtores
    Ficha(std::string nome);
    Ficha(int id, std::string nome);

    ~Ficha();

    void adicionarExercicio(Exercicio* exercicio);
    void exibirFicha() const;
    
    double calcularTempoTotal() const;
    double calcularCaloriasTotais() const;
    
    int getId() const;
    std::string getNome() const;
    const std::vector<Exercicio*>& getExercicios() const;
    
    static void atualizarProximoId(int maiorId);
};

#endif