#ifndef TETRIS_C_INCLUDED
#define TETRIS_C_INCLUDED
#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "SOIL.h"
#include "tetris.h"


struct selector{
    float posX, posY, height, width;
    GLuint img;
};

struct pecas{
    int tipo[10][6];
    float vel;
    GLuint img[6];
};

Selector init_selector(){
    Selector N = malloc(sizeof(struct selector));
    if(N != NULL){
        N->posX = -0.068;
        N->posY = 0.2;
        N->height = 0.2;
        N->width = 0.13;
    }
    return N;
}

Pecas init_pecas(){
    Pecas N = malloc(sizeof(struct pecas));
    if(N != NULL){
        int i,j;
        for(i=0;i<10;i++){
            for(j=0;j<6;j++){
                if(i > 6) N->tipo[i][j] = -1;
                else N->tipo[i][j] = (rand() % 4);
                if(j > 1 && N->tipo[i][j] == N->tipo[i][j-1] && N->tipo[i][j] == N->tipo[i][j-2] && N->tipo[i][j] != -1) {
                    while (1 == 1){
                        int randon = (rand() % 4);
                        if( randon != N->tipo[i][j]){
                            N->tipo[i][j] = randon;
                            break;
                        }
                    }
                }

            }
        }
        N->vel = 0;
    }
    return N;
}

void carregaTexturas(GLuint tela_2d[],Selector selector, Pecas pecas){
    char str[50];
    int i;

    sprintf(str,"assets/start.png");
    tela_2d[0] = carregaArqTextura(str);

    sprintf(str,"assets/game2.png");
    tela_2d[1] = carregaArqTextura(str);

    sprintf(str,"assets/grade.png");
    selector->img = carregaArqTextura(str);

    sprintf(str,"assets/verde.png");
    pecas->img[0] = carregaArqTextura(str);

    sprintf(str,"assets/vermelho.png");
    pecas->img[1] = carregaArqTextura(str);

    sprintf(str,"assets/amarelo.png");
    pecas->img[2] = carregaArqTextura(str);

    sprintf(str,"assets/azul.png");
    pecas->img[3] = carregaArqTextura(str);

    sprintf(str,"assets/roxo.png");
    pecas->img[4] = carregaArqTextura(str);

}

static GLuint carregaArqTextura(char *str){
    // http://www.lonesock.net/soil.html
    GLuint tex = SOIL_load_OGL_texture
        (
            str,
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y |
            SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
        );

    if(0 == tex){
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }

    return tex;
}

void renderiza(float xPos,float xNeg,float yPos,float yNeg,GLuint img_2d){
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, img_2d);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glColor3f(0,0,1);
        glBegin(GL_QUADS);
            glTexCoord2f(1.0, 1.0);
            glVertex2f(xPos,yPos);//canto superior direito
            glTexCoord2f(1.0, 0.0);
            glVertex2f(xPos,yNeg);//cato inferior direiro
            glTexCoord2f(0.0, 0.0);
            glVertex2f(xNeg,yNeg);//canto inferior esquerdo
            glTexCoord2f(0.0, 1.0);
            glVertex2f(xNeg,yPos);//canto superior direito
        glEnd();
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void trocar(Pecas pecas, Selector selector){
    if(selectd){
        float posY = (selector->posY + selector->height + 1 - pecas->vel)*5;
        float posX = (selector->posX + selector->width + 0.20)/0.13;
        
        int aux = pecas->tipo[(int)roundf(posY)][(int)roundf(posX)];
        pecas->tipo[(int)roundf(posY)][(int)roundf(posX)] = pecas->tipo[(int)roundf(posY)][(int)roundf(posX) + 1];
        pecas->tipo[(int)roundf(posY)][(int)roundf(posX) + 1] = aux;
 
        selectd = 0;
    }
}

void gravidade(Pecas pecas){
    int l,t;
    for(l=0;l<9;l++){
        for(t=0;t<6;t++){
            if(pecas->tipo[l][t] == -1){
                pecas->tipo[l][t] = pecas->tipo[l+1][t];
                pecas->tipo[l+1][t] = -1;
            }
        }
    }
}

void verifica(Pecas pecas){
    int l,t;
    for(l=1;l<10;l++){
        for(t=0;t<4;t++){
            int comp1 = pecas->tipo[l][t] == pecas->tipo[l][t+1];
            int comp2 = pecas->tipo[l][t] == pecas->tipo[l][t+2];
            int comp3 = pecas->tipo[l][t] == pecas->tipo[l][t+3];
            int comp4 = pecas->tipo[l][t] == pecas->tipo[l][t+4];
            int comp5 = pecas->tipo[l][t] == pecas->tipo[l][t+5];

            int dif0 = pecas->tipo[l-1][t+0] != -1;
            int dif1 = pecas->tipo[l-1][t+1] != -1;
            int dif2 = pecas->tipo[l-1][t+2] != -1;
            int dif3 = pecas->tipo[l-1][t+3] != -1;
            int dif4 = pecas->tipo[l-1][t+4] != -1;
            int dif5 = pecas->tipo[l-1][t+5] != -1;

            int difSelf = pecas->tipo[l][t] != -1;

            if( t < 1 && comp1 && comp2 && comp3 && comp4 && comp5 &&
                dif0 && dif1 && dif2 && dif3 && dif4 && comp5 && difSelf){
                    
                pecas->tipo[l][t] = -1;
                pecas->tipo[l][t+1] = -1;
                pecas->tipo[l][t+2] = -1;
                pecas->tipo[l][t+3] = -1;
                pecas->tipo[l][t+4] = -1;
                printf("\n 4 pts");
            }else if( t < 2 && comp1 && comp2 && comp3 && comp4 &&
                dif0 && dif1 && dif2 && dif3 && dif4 && difSelf){
                    
                pecas->tipo[l][t] = -1;
                pecas->tipo[l][t+1] = -1;
                pecas->tipo[l][t+2] = -1;
                pecas->tipo[l][t+3] = -1;
                pecas->tipo[l][t+4] = -1;
                printf("\n 4 pts");
            }else if( t < 3 && comp1 && comp2 && comp3 &&
                dif0 && dif1 && dif2 && dif3 && difSelf){

                pecas->tipo[l][t] = -1;
                pecas->tipo[l][t+1] = -1;
                pecas->tipo[l][t+2] = -1;
                pecas->tipo[l][t+3] = -1;
                printf("\n 4 pts");
            }else if( comp1 && comp2 &&
                dif0 && dif1 && dif2 && difSelf){

                pecas->tipo[l][t] = -1;
                pecas->tipo[l][t+1] = -1;
                pecas->tipo[l][t+2] = -1;
                printf("\n 3 pts");
            }
        }
    }
    l=0;
    for(t=0;t<4;t++){
        if(pecas->tipo[l][t] == pecas->tipo[l][t+1] && pecas->tipo[l][t] == pecas->tipo[l][t+2] && pecas->tipo[l-1][t] != -1 && pecas->tipo[l-1][t+1] != -1 && pecas->tipo[l-1][t+2] != -1){
            pecas->tipo[l][t+1] = (rand() % 4);
        }
    }
}

void desenhaPecas(Pecas pecas, float vel){
    float i, j=0;
    for(i=0;i<10;i+=1){
        for(j=0;j<6;j+=1){
            if(pecas->tipo[(int)i][(int)j] > -1) renderiza(-0.33 + j*0.13,-0.20 + j*0.13,(-1 + i/5 + pecas->vel),(-1.2 + i/5 + pecas->vel), pecas->img[pecas->tipo[(int)i][(int)j]]);
        }
    }
    if(pecas->vel < 0.2) {
        pecas->vel += vel;
    }
    else {
        int l,t;
        for(l=9;l>0;l--){
            for(t=0;t<6;t++){
                pecas->tipo[l][t] = pecas->tipo[l-1][t];
            }
        }
        for(t=0;t<6;t++){
            pecas->tipo[0][t] = (rand() % 4);
        }
        pecas->vel = 0;
    }
}

void movimentaSelector(Selector selector, float vel){
    if(movLaterais == 1 && selector->posX <= 0.07) {
        selector->posX = selector->posX + selector->width;
        movLaterais = 0;
    }
    if(movLaterais == -1 && selector->posX >= -0.21) {
        selector->posX = selector->posX - selector->width;
        movLaterais = 0;
    }

    if(selector->posY < 0.54) {
        selector->posY = selector->posY + vel;
    }else{
        selector->posY = selector->posY + vel - selector->height;
    }

    if(movVerticais == 1 && selector->posY < 0.34) {
        selector->posY = selector->posY + selector->height;
        movVerticais = 0;
    }
    if(movVerticais == -1 && selector->posY > -0.69) {
        selector->posY = selector->posY - selector->height;
        movVerticais = 0;
    }
}

void desenhaSelector(Selector selector){

    //printf("%d",press);

    float xPos = selector->width + selector->posX;
    float xNeg = selector->posX;
    float yPos = selector->height + selector->posY;
    float yNeg = selector->posY;

    renderiza(xPos, xNeg, yPos, yNeg, selector->img);
    renderiza(xPos + selector->width, xNeg + selector->width, yPos, yNeg, selector->img);

    /*renderiza(xPos, xNeg,selector->height +0.4, 0.4, selector->img);
    renderiza(xPos, xNeg,selector->height +0.5, 0.5, selector->img);

    renderiza(xPos, xNeg,selector->height -0.0, -0.0, selector->img);
    renderiza(xPos, xNeg,selector->height -0.2, - 0.2, selector->img);
    renderiza(xPos, xNeg,selector->height -0.4, -0.4, selector->img);
    renderiza(xPos, xNeg,selector->height -0.6, -0.6, selector->img);
    renderiza(xPos, xNeg,selector->height -0.8, -0.8, selector->img);*/

    //renderiza(xPos, xNeg, yPos, yNeg, selector->img);
    /*renderiza(-0.33, -0.20, 0.1, -0.1, selector->img);
    renderiza(-0.20, -0.07, 0.1, -0.1, selector->img);
    renderiza(-0.07, 0.06, 0.1, -0.1, selector->img);
    renderiza(0.06, 0.19, 0.1, -0.1, selector->img);
    renderiza(0.19, 0.32, 0.1, -0.1, selector->img);
    renderiza(0.32, 0.45, 0.1, -0.1, selector->img);*/
}


#endif // TETRIS_C_INCLUDED
