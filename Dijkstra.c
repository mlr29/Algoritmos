#include <stdio.h>
#include <stdlib.h>

typedef struct sNo
{
    char no;
    struct sNo *proximo;
    struct sDistancia *distancias;
} TNo;

typedef struct sDistancia
{
    int distancia;
    TNo *parente;
    struct sDistancia *proximo;

} TDistancia;

typedef struct sTabelaMenorCaminho
{
    char no;
    int distancia;
    int visitado;
    char pai;
    struct sTabelaMenorCaminho *proximo;
} TTabMenorCaminho;

void inicializaNo(TNo **sent, char no)
{
    if (*sent == NULL)
    {
        *sent = malloc(sizeof(TNo));
        (*sent)->no = no;
        (*sent)->distancias = NULL;
        (*sent)->proximo = NULL;
    }
    else
    {
        TNo *proximo = *sent;
        while (proximo->proximo != NULL)
        {
            proximo = proximo->proximo;
        }
        TNo *novo = malloc(sizeof(TNo));
        novo->no = no;
        novo->proximo = NULL;
        novo->distancias = NULL;
        proximo->proximo = novo;
    }
}

void inicializaTabela(TTabMenorCaminho **tabela, char no)
{
    if (*tabela == NULL)
    {
        *tabela = malloc(sizeof(TTabMenorCaminho));
        (*tabela)->no = no;
        (*tabela)->pai = 'X';
        (*tabela)->visitado = 0;
        (*tabela)->distancia = __INT32_MAX__;
        (*tabela)->proximo = NULL;
    }
    else
    {
        TTabMenorCaminho *ultimo = *tabela;
        TTabMenorCaminho *novo = malloc(sizeof(TTabMenorCaminho));
        while (ultimo->proximo != NULL)
        {
            ultimo = ultimo->proximo;
        }

        novo->no = no;
        novo->pai = 'X';
        novo->visitado = 0;
        novo->distancia = __INT32_MAX__;
        novo->proximo = NULL;
        ultimo->proximo = novo;
    }
}

TNo *noProcurado(TNo *sent, char nome)
{
    TNo *noAtual = sent;
    while (noAtual->no != nome)
    {
        noAtual = noAtual->proximo;
    }
    return noAtual;
}

TTabMenorCaminho *noTabProcurado(TTabMenorCaminho *tab, char nome)
{
    TTabMenorCaminho *ultimoTab = tab;
    while (ultimoTab->no != nome && ultimoTab != NULL)
    {
        ultimoTab = ultimoTab->proximo;
    }
    return ultimoTab;
}

void imprimeNos(TNo *grafo)
{
    TNo *proximo = grafo;
    while (proximo != NULL)
    {
        printf("%c ", proximo->no);
        proximo = proximo->proximo;
    }
    printf("\n\n");
}

void ligaNos(TNo *sent, char nomeNoOrigem, char nomeNoDestino, int distancia)
{
    if (sent == NULL)
    {
        printf("\nLista vazia.");
    }
    else
    {
        TNo *noOrigem = sent;
        TNo *noDestino = sent;

        while (noOrigem->no != nomeNoOrigem && noOrigem != NULL)
        {
            noOrigem = noOrigem->proximo;
        }

        if (noOrigem == NULL)
        {
            printf("\nNo de origem nao existe.");
            return;
        }

        while (noDestino->no != nomeNoDestino && noDestino != NULL)
        {
            noDestino = noDestino->proximo;
        }

        if (noDestino == NULL)
        {
            printf("\nNo de destino nao existe.");
            return;
        }

        if (noOrigem->distancias == NULL)
        {

            TDistancia *distancias = malloc(sizeof(TDistancia));

            distancias->distancia = distancia;
            distancias->parente = noDestino;
            distancias->proximo = NULL;
            noOrigem->distancias = distancias;
        }
        else
        {
            TDistancia *proximo = noOrigem->distancias;
            TDistancia *novo = malloc(sizeof(TDistancia));

            while (proximo->proximo != NULL)
            {
                proximo = proximo->proximo;
            }

            novo->distancia = distancia;
            novo->parente = noDestino;
            novo->proximo = NULL;
            proximo->proximo = novo;
        }
    }
}

void imprimeDistancias(TNo *sent)
{
    TNo *proximo = sent;
    TDistancia *proximoDistancia = NULL;
    printf("\nGrafo:\n\n");
    while (proximo != NULL)
    {

        proximoDistancia = proximo->distancias;

        while (proximoDistancia != NULL)
        {
            printf("(%c, %c, %d) ", proximo->no, proximoDistancia->parente->no, proximoDistancia->distancia);

            proximoDistancia = proximoDistancia->proximo;
        }
        printf("\n");
        proximo = proximo->proximo;
    }
    printf("\n");
}

void algoritmoDijkstra(TNo *sent, TTabMenorCaminho *tab, char nomeNoOrigem)
{
    TTabMenorCaminho *ultimoTab = noTabProcurado(tab, nomeNoOrigem);
    TTabMenorCaminho *proximo, *noDestinoTab;
    TNo *noAtual = noProcurado(sent, nomeNoOrigem);
    int distanciaAtual, menorDistancia = __INT32_MAX__, i = 0;
    char nomeNoDestino;

    if (ultimoTab == NULL)
    {
        printf("\nNo nao existe.");
        return;
    }
    else
    {
        ultimoTab->distancia = 0;
    }

    while (noAtual->distancias != NULL)
    {
        TDistancia *conexoes = noAtual->distancias;
        while (conexoes != NULL)
        {
            distanciaAtual = conexoes->distancia + ultimoTab->distancia;
            nomeNoDestino = conexoes->parente->no;
            noDestinoTab = noTabProcurado(tab, nomeNoDestino);
            if (distanciaAtual <= noDestinoTab->distancia)
            {
                noDestinoTab->distancia = distanciaAtual;
                noDestinoTab->pai = ultimoTab->no;
            }
            conexoes = conexoes->proximo;
        }
        ultimoTab->visitado = 1;
        proximo = tab;

        while (1)
        {
            if (proximo->visitado == 0 && proximo->distancia < menorDistancia)
            {
                ultimoTab = proximo;
                noAtual = noProcurado(sent, ultimoTab->no);
                i++;
            }

            if (proximo->proximo == NULL)
                break;

            proximo = proximo->proximo;
        }

        if (i == 0)
        {
            return;
        }
        else
        {
            i = 0;
        }
    }
}

void imprimeTabela(TTabMenorCaminho *tab)
{
    TTabMenorCaminho *proximo = tab;

    while (proximo != NULL)
    {
        printf("\n");
        printf("   %c\t|\t\t%d\t\t|\t  %c ", proximo->no, proximo->distancia != __INT32_MAX__ ? proximo->distancia : -1, proximo->pai);
        proximo = proximo->proximo;
    }
    printf("\n");
}

int main()
{
    char nos[] = {'A', 'B', 'C', 'D', '\0'}, noEscolhido;
        char grafo[][3] = {{'A', 'B', 4}, {'A', 'C', 7}, {'B', 'D', 5}, {'B', 'C', 2}, {'C', 'D', 2}, {'D', 'C', 5}, {'C', 'B', 4}};
    TTabMenorCaminho *tabela = NULL;
    TNo *sentinela = NULL;

    printf("--------------------Algoritmo de Dijkstra--------------------\n\n");

    for (int i = 0; nos[i] != '\0'; i++)
    {
        inicializaTabela(&tabela, nos[i]);
        inicializaNo(&sentinela, nos[i]);
    }

    printf("Nos:\n");
    imprimeNos(sentinela);
    for (int i = 0; i < 7; i++)
    {
        ligaNos(sentinela, grafo[i][0], grafo[i][1], grafo[i][2]);
    }

    imprimeDistancias(sentinela);

    
    printf("\nEscolha um No para calcular distancias: ");
    scanf("%c", &noEscolhido);

    algoritmoDijkstra(sentinela, tabela, noEscolhido);

    printf("\nTabela:\n\n   No\t|\tDistancia para No %c\t|\tNo Pai", noEscolhido);
    imprimeTabela(tabela);


    return 0;
}
