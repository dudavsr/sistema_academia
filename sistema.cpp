#include "sistema.h"
#include "cardio.h"
#include "forca.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <algorithm>
#include <iomanip>

Sistema::Sistema() {
    carregarDados();
}

Sistema::~Sistema() {
    salvarDados();
    
    for (Ficha* f : fichas) {
        delete f;
    }
    fichas.clear();

    for (Exercicio* e : exercicios) {
        delete e;
    }
    exercicios.clear();
}

void Sistema::carregarDados() {
    std::cout << "Carregando dados..." << std::endl;

    std::ifstream arqEx("exercicios.txt");
    if (arqEx.is_open()) {
        std::string linha;
        while (std::getline(arqEx, linha)) {
            if (linha.empty()) continue;
            std::stringstream ss(linha);
            std::string segmento;
            
            int tipo, id;
            std::string nome;
            
            if (std::getline(ss, segmento, ';')) tipo = std::stoi(segmento);
            if (std::getline(ss, segmento, ';')) id = std::stoi(segmento);
            if (std::getline(ss, segmento, ';')) nome = segmento;

            Exercicio* novoExercicio = nullptr;

            if (tipo == 1) {
                int duracao, statusInt;
                double cals;
                if (std::getline(ss, segmento, ';')) duracao = std::stoi(segmento);
                if (std::getline(ss, segmento, ';')) cals = std::stod(segmento);
                if (std::getline(ss, segmento, ';')) statusInt = std::stoi(segmento);
                novoExercicio = new Cardio(id, nome, (statusInt == 1), duracao, cals);

            } else if (tipo == 2) {
                double carga;
                int series, reps, descanso, statusInt;
                if (std::getline(ss, segmento, ';')) carga = std::stod(segmento);
                if (std::getline(ss, segmento, ';')) series = std::stoi(segmento);
                if (std::getline(ss, segmento, ';')) reps = std::stoi(segmento);
                if (std::getline(ss, segmento, ';')) descanso = std::stoi(segmento);
                if (std::getline(ss, segmento, ';')) statusInt = std::stoi(segmento);
                novoExercicio = new Forca(id, nome, (statusInt == 1), carga, series, reps, descanso);
            }

            if (novoExercicio != nullptr) {
                exercicios.push_back(novoExercicio);
                Exercicio::atualizarProximoId(id);
            }
        }
        arqEx.close();
    }

    std::ifstream arqFichas("fichas.txt");
    if (arqFichas.is_open()) {
        std::string linha;
        while (std::getline(arqFichas, linha)) {
            if (linha.empty()) continue;
            std::stringstream ss(linha);
            std::string segmento;

            int idFicha, totalExs;
            std::string nomeFicha;

            if (std::getline(ss, segmento, ';')) idFicha = std::stoi(segmento);
            if (std::getline(ss, segmento, ';')) nomeFicha = segmento;
            if (std::getline(ss, segmento, ';')) totalExs = std::stoi(segmento);

            Ficha* novaFicha = new Ficha(idFicha, nomeFicha);
            Ficha::atualizarProximoId(idFicha);

            for (int i = 0; i < totalExs; i++) {
                if (std::getline(ss, segmento, ';')) {
                    int idEx = std::stoi(segmento);
                    Exercicio* exPtr = buscarExercicioPorId(idEx);
                    if (exPtr != nullptr) {
                        novaFicha->adicionarExercicio(exPtr);
                    }
                }
            }
            fichas.push_back(novaFicha);
        }
        arqFichas.close();
    }

    historico.carregarDeArquivo();
}

void Sistema::salvarDados() {
    std::ofstream arqEx("exercicios.txt");
    if (arqEx.is_open()) {
        for (Exercicio* ex : exercicios) {
            arqEx << ex->getTipo() << ";" << ex->getId() << ";" << ex->getNome() << ";";
            if (ex->getTipo() == 1) {
                Cardio* c = dynamic_cast<Cardio*>(ex);
                if (c) arqEx << c->getDuracao() << ";" << c->getCaloriasPorMinuto() << ";" << (c->isAtivo() ? 1 : 0);
            } else if (ex->getTipo() == 2) {
                Forca* f = dynamic_cast<Forca*>(ex);
                if (f) arqEx << f->getCarga() << ";" << f->getSeries() << ";" << f->getRepeticoes() << ";" << f->getTempoDescanso() << ";" << (f->isAtivo() ? 1 : 0);
            }
            arqEx << std::endl;
        }
        arqEx.close();
    }

    std::ofstream arqFichas("fichas.txt");
    if (arqFichas.is_open()) {
        for (Ficha* f : fichas) {
            auto lista = f->getExercicios();
            arqFichas << f->getId() << ";" << f->getNome() << ";" << lista.size();
            for (Exercicio* ex : lista) arqFichas << ";" << ex->getId();
            arqFichas << std::endl;
        }
        arqFichas.close();
    }

    historico.salvarEmArquivo();
}

Exercicio* Sistema::buscarExercicioPorId(int id) {
    for (Exercicio* ex : exercicios) {
        if (ex->getId() == id) return ex;
    }
    return nullptr;
}

Ficha* Sistema::buscarFichaPorId(int id) {
    for (Ficha* f : fichas) {
        if (f->getId() == id) return f;
    }
    return nullptr;
}

void Sistema::cadastrarExercicio() {
    std::cout << "--- NOVO EXERCICIO ---" << std::endl;
    std::cout << "Tipo (1-Cardio, 2-Forca): ";
    int tipo;
    std::cin >> tipo;
    std::cin.ignore(); 

    std::cout << "Nome: ";
    std::string nome;
    std::getline(std::cin, nome);

    Exercicio* novo = nullptr;

    if (tipo == 1) {
        int duracao;
        double cals;
        std::cout << "Duracao (min): ";
        std::cin >> duracao;
        std::cout << "Calorias/min: ";
        std::cin >> cals;
        novo = new Cardio(nome, duracao, cals);
    } else if (tipo == 2) {
        double carga;
        int series, reps, descanso;
        std::cout << "Carga (kg): ";
        std::cin >> carga;
        std::cout << "Series: ";
        std::cin >> series;
        std::cout << "Repeticoes: ";
        std::cin >> reps;
        std::cout << "Descanso (s): ";
        std::cin >> descanso;
        novo = new Forca(nome, carga, series, reps, descanso);
    } else {
        std::cout << "Tipo invalido!" << std::endl;
        pausar();
        return;
    }

    if (novo) {
        exercicios.push_back(novo);
        std::cout << "Exercicio cadastrado com Sucesso! ID: " << novo->getId() << std::endl;
    }
    pausar();
}

void Sistema::listarExercicios() {
    std::cout << "--- LISTA DE EXERCICIOS ATIVOS ---" << std::endl;
    bool encontrou = false;
    for (const auto* ex : exercicios) {
        if (ex->isAtivo()) {
            ex->exibirDetalhes();
            encontrou = true;
        }
    }
    if (!encontrou) std::cout << "Nenhum exercicio ativo cadastrado." << std::endl;
    pausar();
}

void Sistema::excluirExercicio() {
    listarExercicios();
    std::cout << "\nInforme o ID para excluir: ";
    int id;
    std::cin >> id;

    Exercicio* ex = buscarExercicioPorId(id);
    if (ex && ex->isAtivo()) {
        ex->desativar();
        std::cout << "Exercicio desativado com sucesso!" << std::endl;
    } else {
        std::cout << "Exercicio nao encontrado ou ja inativo." << std::endl;
    }
    pausar();
}

void Sistema::criarFicha() {
    std::cin.ignore();
    std::cout << "Nome da Nova Ficha: ";
    std::string nome;
    std::getline(std::cin, nome);

    Ficha* nova = new Ficha(nome);
    fichas.push_back(nova);
    std::cout << "Ficha '" << nome << "' criada com ID " << nova->getId() << std::endl;
    pausar();
}

void Sistema::adicionarExercicioFicha() {
    if (fichas.empty()) {
        std::cout << "Nenhuma ficha cadastrada." << std::endl;
        pausar();
        return;
    }

    std::cout << "--- SELECIONE A FICHA ---" << std::endl;
    for (const auto* f : fichas) {
        std::cout << "ID: " << f->getId() << " | Nome: " << f->getNome() << std::endl;
    }
    std::cout << "ID da Ficha: ";
    int idFicha;
    std::cin >> idFicha;

    Ficha* ficha = buscarFichaPorId(idFicha);
    if (!ficha) {
        std::cout << "Ficha nao encontrada." << std::endl;
        pausar();
        return;
    }

    limparTela();
    listarExercicios(); 
    std::cout << "\nID do Exercicio para adicionar na ficha '" << ficha->getNome() << "': ";
    int idEx;
    std::cin >> idEx;

    Exercicio* ex = buscarExercicioPorId(idEx);
    if (ex && ex->isAtivo()) {
        ficha->adicionarExercicio(ex);
        std::cout << "Exercicio adicionado com sucesso!" << std::endl;
    } else {
        std::cout << "Exercicio invalido." << std::endl;
    }
    pausar();
}

void Sistema::listarFichas() {
    if (fichas.empty()) {
        std::cout << "Nenhuma ficha cadastrada" << std::endl;
    } else {
        for (const auto* f : fichas) {
            f->exibirFicha();
            std::cout << std::endl;
        }
    }
    pausar();
}

void Sistema::registrarTreino() {
    if (fichas.empty()) {
        std::cout << "Crie uma ficha antes de treinar." << std::endl;
        pausar();
        return;
    }

    std::cout << "--- REGISTRAR TREINO ---" << std::endl;
    for (const auto* f : fichas) {
        std::cout << "ID: " << f->getId() << " | Nome: " << f->getNome() << std::endl;
    }
    std::cout << "Qual ficha voce executou (ID)? ";
    int id;
    std::cin >> id;

    Ficha* ficha = buscarFichaPorId(id);
    if (ficha) {
        RegistroTreino reg;
        reg.dataHora = getDataHoraAtual();
        reg.idFicha = ficha->getId();
        reg.nomeFicha = ficha->getNome();
        reg.tempoTotal = ficha->calcularTempoTotal();
        reg.caloriasTotal = ficha->calcularCaloriasTotais();

        historico.adicionarRegistro(reg);

        std::cout << "\nTreino registrado com sucesso!" << std::endl;
        std::cout << "Data: " << reg.dataHora << std::endl;
        std::cout << "Tempo Estimado: " << reg.tempoTotal << " min" << std::endl;
        std::cout << "Gasto Calorico: " << reg.caloriasTotal << " cal" << std::endl;
    } else {
        std::cout << "Ficha nao encontrada." << std::endl;
    }
    pausar();
}

void Sistema::exibirHistorico() {
    historico.exibirHistorico();
    pausar();
}