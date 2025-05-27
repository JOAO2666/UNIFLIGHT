#include "raylib.h"

typedef enum
{
    POSE_IDLE,
    POSE_SOCO,
    POSE_CHUTE,
    POSE_PODER,
    POSE_DEFESA,
    POSE_DANO
} TipoPose;

typedef struct
{
    Vector2 posicao;
    Vector2 velocidade;
    Color cor;
    float vida;
    float vidaMaxima;
    float tamanho;
} Particula;

typedef struct
{
    Particula particulas[50];
    int numParticulas;
    bool ativo;
    float timer;
} SistemaParticulas;

typedef struct
{
    Texture2D textura;
    const char *nome;
    const char *habilidade;
    const char *historia;
    Color corHabilidade;

    // Texturas para animações de luta
    Texture2D texturaLuta;
    Texture2D texturaSoco;
    Texture2D texturaChute;
    Texture2D texturaPoder;
    Texture2D texturaDefesa;
    Texture2D texturaDano;

    // Sistema de animação
    TipoPose poseAtual;
    float timerAnimacao;
    float duracaoAnimacao;
    bool animando;

    // Stats de combate
    int vidaMaxima;
    int vidaAtual;
    int poderMaximo;
    int poderAtual;
    int danoSoco;
    int danoChute;
    int danoPoder;
} Personagem;

typedef struct
{
    const char *texto;
    int posY;
} OpcaoMenu;

typedef struct
{
    Texture2D textura;
    const char *nome;
    const char *descricao;
    Sound musicaFundo;
} Mapa;

typedef struct
{
    Sound somSoco;
    Sound somChute;
    Sound somPoder;
    Sound somVitoria;
    Sound somDerrota;
} Sons;

typedef struct
{
    Personagem *jogador1;
    Personagem *jogador2;
    Mapa *mapaAtual;
    int roundAtual;
    float tempoRound;
    bool jogador1Confirmado;
    bool jogador2Confirmado;
    bool mapaConfirmado;

    // Sistema de efeitos visuais
    SistemaParticulas particulasImpacto;
    float flashTela;
    float tremor;
} EstadoJogo;

typedef enum
{
    TELA_MENU,
    TELA_OPCOES,
    TELA_SELECAO,
    TELA_SELECAO_MAPA,
    TELA_LUTA
} Tela;

typedef struct
{
    // Controles Player 1
    int teclaPoderP1;
    int teclaSocoP1;
    int teclaChute1;
    int teclaEsquerdaP1;
    int teclaDireitaP1;
    int teclaConfirmarP1;

    // Controles Player 2
    int teclaPoderP2;
    int teclaSocoP2;
    int teclaChute2;
    int teclaEsquerdaP2;
    int teclaDireitaP2;
    int teclaConfirmarP2;

    // Volume
    float volumeMusica;
    float volumeEfeitos;
} ConfiguracaoJogo;
