#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "SOIL.h"

struct selector{
    float posX, posY, height, width;
    GLuint img;
};

struct pecas{
    int tipo[10][6];
    float vel;
    int pontos;
    GLuint img[6];
};


typedef struct selector* Selector;
typedef struct pecas* Pecas;

int movLaterais;
int movVerticais;
int selectd;
int tela;
GLuint pontos[10];

static GLuint carregaArqTextura(char str[]);
void carregaTexturas(GLuint *tela, Selector selector, Pecas pecas);
void renderiza(float xPos,float xNeg,float yPos,float yNeg,GLuint img);
Selector init_selector();
void desenhaSelector(Selector selector);
void movimentaSelector(Selector selector, Pecas pecas, float vel);
Pecas init_pecas();
void movimentoPecas();
void desenhaPecas(Pecas pecas, float vel);
void trocar(Pecas pecas, Selector selector);
void gravidade(Pecas pecas);
void verifica(Pecas pecas);
void desenhaPontos(Pecas pecas, float *vel);