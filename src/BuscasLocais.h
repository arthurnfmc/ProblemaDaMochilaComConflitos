#ifndef BUSCAS_LOCAIS_H
#define BUSCAS_LOCAIS_H

#include <algorithm>
#include "Instancia.h"
#include "Solucao.h"
#include "Avaliador.h"
#include "Comparadores.h"
#include "HeuristicasConstrutivas.h"
#include "MetodosExatos.h"

using std::sort;
using std::pair;

class BuscaLocal
{
public:
    virtual Solucao aprimorarSolucao(Instancia& inst, Solucao& sol)=0;
};

class BuscaLocalDeCI // Explora estrutura de vizinhancas do CI e nao da propria solucao
{
public:
    virtual Solucao aprimorarSolucao(Instancia& inst, Solucao& sol, Solucao& CI)=0; // hc tem de ser de decomposicao!!!
};

// Pega um item e o adiciona na solucao e, entao, remove os conflitos e adequa ao pesoMax.
// Se o item pertencer ao CI da solBase, garantidamente nao eh possivel melhora. (caso do resolvedor CPLEX)
// Entretando, ja que |CI| tende a ser muito menor que a quantidade de itens total, e em uma descida esse movimento sera executado varias vezes
// Isso nao sera tao problematico.
// Nao irei adicionar verificacao sobre o CI pois estragaria a modularidade do codigo (nao eh todo metodo que decompoe o problema).
// Retorna par (novaSolucao, novoCusto)
//pair<Solucao, float> movimentoInsercaoAleatoria(Instancia& inst, Solucao& solBase, float fo);

class DescidaDestroyAndRepair : public BuscaLocalDeCI
{
private:
    int maxIter; // Maximo de iteracoes sem melhora
    float percDestruido; // Percentual do CI destruido (adaptativo)
    float percBase; // Percentual Inicial (fixo)
    float alpha; // Grau de variacao
    Solucao recomposicaoGulosa(Instancia& inst, Solucao& CI);
    Solucao recomposicaoAleatoria(Instancia& inst, Solucao& CI);

public:
    DescidaDestroyAndRepair();
    DescidaDestroyAndRepair(int maxIter, float percDestruido, float alpha);
    ~DescidaDestroyAndRepair();

    Solucao aprimorarSolucao(Instancia& inst, Solucao& sol, Solucao& CI);
};


#endif