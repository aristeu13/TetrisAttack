typedef struct selector* Selector;
typedef struct pecas* Pecas;

int movLaterais;
int movVerticais;
int selectd;


static GLuint carregaArqTextura(char str[]);
void carregaTexturas(GLuint *tela, Selector selector, Pecas pecas);
void renderiza(float xPos,float xNeg,float yPos,float yNeg,GLuint img);
Selector init_selector();
void desenhaSelector(Selector selector);
void movimentaSelector(Selector selector, float vel);
Pecas init_pecas();
void movimentoPecas();
void desenhaPecas(Pecas pecas, float vel);
void trocar(Pecas pecas, Selector selector);
