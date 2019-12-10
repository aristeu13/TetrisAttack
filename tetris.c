#ifndef TETRIS_C_INCLUDED
#define TETRIS_C_INCLUDED
#include "tetris.h"

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
    int c;
    Pecas N = malloc(sizeof(struct pecas));
    if(N != NULL){
        int i,j;
        for(i=0;i<10;i++){
            for(j=0;j<6;j++){
                if(i > 6) N->tipo[i][j] = -1;
                else N->tipo[i][j] = (rand() % 4);
                while ((j > 1 && N->tipo[i][j] == N->tipo[i][j-1] && N->tipo[i][j] == N->tipo[i][j-2] && N->tipo[i][j] != -1) || (i >= 2 && N->tipo[i][j] == N->tipo[i-1][j] && N->tipo[i][j] == N->tipo[i-2][j] && N->tipo[i][j] != -1)){
                    N->tipo[i][j] = (rand() % 4);
                }
            }
        }
        N->vel = 0;
        N->pontos = 0;
    }
    return N;
}

void carregaTexturas(GLuint tela_2d[],Selector selector, Pecas pecas){
    char str[50];

    sprintf(str,"assets/start.png");
    tela_2d[0] = carregaArqTextura(str);

    sprintf(str,"assets/game3a.png");
    tela_2d[1] = carregaArqTextura(str);

    sprintf(str,"assets/gameover.png");
    tela_2d[2] = carregaArqTextura(str);

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

    int i;
    for(i=0;i<10;i++){
        sprintf(str,"assets/sprite%d.png",i);
        pontos[i] = carregaArqTextura(str);
    }

}

static GLuint carregaArqTextura(char *str){
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
    for(l=0;l<10;l++){
        for(t=0;t<6;t++){
            int comp1H = pecas->tipo[l][t] == pecas->tipo[l][t+1];
            int comp2H = pecas->tipo[l][t] == pecas->tipo[l][t+2];
            int comp3H = pecas->tipo[l][t] == pecas->tipo[l][t+3];
            int comp4H = pecas->tipo[l][t] == pecas->tipo[l][t+4];
            int comp5H = pecas->tipo[l][t] == pecas->tipo[l][t+5];

            int dif0 = pecas->tipo[l-1][t+0] != -1;//evitar marca ponto antes da peca cair completamente
            int dif1 = pecas->tipo[l-1][t+1] != -1;
            int dif2 = pecas->tipo[l-1][t+2] != -1;
            int dif3 = pecas->tipo[l-1][t+3] != -1;
            int dif4 = pecas->tipo[l-1][t+4] != -1;
            int dif5 = pecas->tipo[l-1][t+5] != -1;

            int comp1V = pecas->tipo[l][t] == pecas->tipo[l+1][t];
            int comp2V = pecas->tipo[l][t] == pecas->tipo[l+2][t];
            int comp3V = pecas->tipo[l][t] == pecas->tipo[l+3][t];
            int comp4V = pecas->tipo[l][t] == pecas->tipo[l+4][t];
            int comp5V = pecas->tipo[l][t] == pecas->tipo[l+5][t];

            int difSelf = pecas->tipo[l][t] != -1;

            if(l > 0 && t < 1 && comp1H && comp2H && comp3H && comp4H && comp5H && dif0 && dif1 && dif2 && dif3 && dif4 && dif5 && difSelf){
                pecas->tipo[l][t] = -1;
                pecas->tipo[l][t+1] = -1;
                pecas->tipo[l][t+2] = -1;
                pecas->tipo[l][t+3] = -1;
                pecas->tipo[l][t+4] = -1;
                pecas->pontos += 200;
            }else if(l > 0 && l < 5  && comp1V && comp2V && comp3V && comp4V && comp5V && difSelf){
                pecas->tipo[l][t] = -1;
                pecas->tipo[l+1][t] = -1;
                pecas->tipo[l+2][t] = -1;
                pecas->tipo[l+3][t] = -1;
                pecas->tipo[l+4][t] = -1;
                pecas->tipo[l+5][t] = -1;
                pecas->pontos += 200;
            }else if(l > 0 && t < 2 && comp1H && comp2H && comp3H && comp4H && dif0 && dif1 && dif2 && dif3 && dif4 && difSelf){
                pecas->tipo[l][t] = -1;
                pecas->tipo[l][t+1] = -1;
                pecas->tipo[l][t+2] = -1;
                pecas->tipo[l][t+3] = -1;
                pecas->tipo[l][t+4] = -1;
                pecas->pontos += 150;
            }else if(l > 0 && l < 6  && comp1V && comp2V && comp3V && comp4V && difSelf){
                pecas->tipo[l][t] = -1;
                pecas->tipo[l+1][t] = -1;
                pecas->tipo[l+2][t] = -1;
                pecas->tipo[l+3][t] = -1;
                pecas->tipo[l+4][t] = -1;
                pecas->pontos += 150;
            }else if(l > 0 && t < 3 && comp1H && comp2H && comp3H && dif0 && dif1 && dif2 && dif3 && difSelf){
                pecas->tipo[l][t] = -1;
                pecas->tipo[l][t+1] = -1;
                pecas->tipo[l][t+2] = -1;
                pecas->tipo[l][t+3] = -1;
                pecas->pontos += 100;
            }else if(l > 0 && l < 7  && comp1V && comp2V && comp3V && difSelf){
                pecas->tipo[l][t] = -1;
                pecas->tipo[l+1][t] = -1;
                pecas->tipo[l+2][t] = -1;
                pecas->tipo[l+3][t] = -1;
                pecas->pontos += 100;
            }else if(l > 0 && t < 4 && comp1H && comp2H && dif0 && dif1 && dif2 && difSelf){
                pecas->tipo[l][t] = -1;
                pecas->tipo[l][t+1] = -1;
                pecas->tipo[l][t+2] = -1;
                pecas->pontos += 50;
            }else if(l > 0 && l < 8  && comp1V && comp2V && difSelf){
                pecas->tipo[l][t] = -1;
                pecas->tipo[l+1][t] = -1;
                pecas->tipo[l+2][t] = -1;
                pecas->pontos += 50;
            }
        }
    }
}

void desenhaPecas(Pecas pecas, float vel){
    float i, j=0;
    for(i=0;i<10;i+=1){
        for(j=0;j<6;j+=1){
            if(pecas->tipo[(int)i][(int)j] > -1){
                renderiza(-0.33 + j*0.13,-0.20 + j*0.13,(-1 + i/5 + pecas->vel),(-1.2 + i/5 + pecas->vel), pecas->img[pecas->tipo[(int)i][(int)j]]);
                if((-1 + i/5 + pecas->vel) >= 0.74) tela = 2;
            }
        }
    }
    if(pecas->vel < 0.2) {
        pecas->vel += vel;
    }else {
        int l,t;
        for(l=9;l>0;l--){
            for(t=0;t<6;t++){
                pecas->tipo[l][t] = pecas->tipo[l-1][t];
            }
        }
        for(t=0;t<6;t++){
            pecas->tipo[0][t] = (rand() % 4);
            while((t > 1 && pecas->tipo[0][t] == pecas->tipo[0][t-1] && pecas->tipo[0][t] == pecas->tipo[0][t-2] && pecas->tipo[0][t] != -1) || (pecas->tipo[0][t] == pecas->tipo[1][t] && pecas->tipo[0][t] == pecas->tipo[2][t] && pecas->tipo[0][t] != -1)){
                pecas->tipo[0][t] = (rand() % 4);
            }
        }
        pecas->vel = 0;
    }
}

void movimentaSelector(Selector selector,Pecas pecas , float vel){
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
        int posY = (int)roundf((selector->posY + selector->height + 1 - pecas->vel)*5);
        selector->posY = -0.8 + posY/5 + pecas->vel;
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

    float xPos = selector->width + selector->posX;
    float xNeg = selector->posX;
    float yPos = selector->height + selector->posY;
    float yNeg = selector->posY;

    renderiza(xPos, xNeg, yPos, yNeg, selector->img);
    renderiza(xPos + selector->width, xNeg + selector->width, yPos, yNeg, selector->img);
}

void desenhaPontos(Pecas pecas, float *vel){
    int total = pecas->pontos;
    int u = total%10;
    int d = (total%100)/10;
    int c = (total%1000)/100;
    int m = (total%10000)/1000;
    int dm = (total%10000)/10000;
    // printf("%d %d %d %d %d\n",u,d,c,m,dm);
    renderiza(-0.78,-0.88,0.66,0.56,pontos[dm]);
    renderiza(-0.7,-0.80,0.66,0.56,pontos[m]);
    renderiza(-0.62,-0.72,0.66,0.56,pontos[c]);
    renderiza(-0.54,-0.64,0.66,0.56,pontos[d]);
    renderiza(-0.46,-0.56,0.66,0.56,pontos[u]);

    if(total>=500 && *vel >= 0.00015 && *vel <= 0.00025){
        *vel = *vel + 0.0002;
    }else if(total>=1000 && *vel >= 0.00035 && *vel <= 0.00045){
        *vel = *vel + 0.0002;
    }else if(total>=1500 && *vel >= 0.00055 && *vel <= 0.00065){
        *vel = *vel + 0.0002;
    }else if(total>=2000 && *vel >= 0.00075 && *vel <= 0.00085){
        *vel = *vel + 0.0002;
    }
}

#endif // TETRIS_C_INCLUDED
