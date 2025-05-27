#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "structs.c"

#define LARGURA_TELA 1920
#define ALTURA_TELA 1080
#define MAX_PERSONAGENS 6
#define OPCOES_MENU 3
#define OPCOES_CONFIG 8
#define MAX_MAPAS 6
#define VIDA_MAXIMA 100
#define PODER_MAXIMO 100
#define TEMPO_ROUND 60.0f
#define MAX_ROUNDS 3

// -------------------------------- INICIO - PARTE QUE DESENHA A TELA DE SELEÇÃO DOS PERSONAGENS (DE UM PLAYER SÓ) ---------------------------------

void DesenharSelecaoPersonagem(Personagem personagens[], int selecionado, int inicioX, int inicioY, int larguraArea, int alturaArea, Color corDestaque)
{
    int espacamento = 35;
    int larguraPers = 150;
    int alturaPers = 150;

    int colunas = 3;
    int linhas = 2;

    int larguraTotal = colunas * larguraPers + (colunas - 1) * espacamento;
    int alturaTotal = linhas * alturaPers + (linhas - 1) * espacamento;

    int posXInicial = inicioX + (larguraArea - larguraTotal) / 2;
    int posYInicial = inicioY + (alturaArea - alturaTotal) / 2;

    for (int i = 0; i < MAX_PERSONAGENS; i++)
    {
        int linha = i / colunas;
        int coluna = i % colunas;

        int x = posXInicial + coluna * (larguraPers + espacamento);
        int y = posYInicial + linha * (alturaPers + espacamento);

        Rectangle destino = {(float)x, (float)y, (float)larguraPers, (float)alturaPers};
        Color cor = WHITE;

        if (i == selecionado)
        {
            destino.width *= 1.2f;
            destino.height *= 1.2f;
            DrawRectangleLines(x - 5, y - 5, (int)(larguraPers * 1.2f) + 10, (int)(alturaPers * 1.2f) + 10, corDestaque);
        }

        DrawTexturePro(personagens[i].textura,
                       (Rectangle){0, 0, (float)personagens[i].textura.width, (float)personagens[i].textura.height},
                       destino,
                       (Vector2){0, 0},
                       0.0f,
                       cor);
    }
}

// -------------------------------- FIM - PARTE QUE DESENHA A TELA DE SELEÇÃO DOS PERSONAGENS (DE UM PLAYER SÓ) ------------------------------------

// -------------------------------- INICIO - FUNÇÃO PARA DESENHAR INFORMAÇÕES DO PERSONAGEM ---------------------------------

void DesenharInfoPersonagem(Personagem personagem, int x, int y, int largura, int altura, Font fonte)
{
    // Fundo para as informações
    DrawRectangleRounded((Rectangle){x, y, largura, altura}, 0.1f, 10, Fade(BLACK, 0.8f));
    DrawRectangleRoundedLines((Rectangle){x, y, largura, altura}, 0.1f, 10, personagem.corHabilidade);

    int posY = y + 20;
    int espacamento = 35;

    // Nome do personagem
    DrawTextEx(fonte, personagem.nome, (Vector2){x + 20, posY}, 32, 1, WHITE);
    posY += espacamento + 10;

    // Habilidade
    const char *habilidadeTexto = TextFormat("Habilidade: %s", personagem.habilidade);
    DrawTextEx(fonte, habilidadeTexto, (Vector2){x + 20, posY}, 24, 1, personagem.corHabilidade);
    posY += espacamento;

    // História (quebrada em linhas)
    const char *historiaTexto = "História:";
    DrawTextEx(fonte, historiaTexto, (Vector2){x + 20, posY}, 20, 1, LIGHTGRAY);
    posY += 25;

    // Quebrar texto da história em múltiplas linhas
    int larguraTexto = largura - 40;
    const char *historia = personagem.historia;
    int tamanhoFonte = 16;

    // Simular quebra de linha manual
    char linha[200];
    int posicao = 0;
    int caracteresPorLinha = (larguraTexto / (tamanhoFonte * 0.6f));

    if (caracteresPorLinha <= 0)
        caracteresPorLinha = 20; // Valor mínimo seguro

    while (historia[posicao] != '\0' && posY < y + altura - 30)
    {
        int fim = posicao + caracteresPorLinha;
        int tamanhoHistoria = strlen(historia);

        if (fim >= tamanhoHistoria)
        {
            fim = tamanhoHistoria;
        }
        else
        {
            // Procurar o último espaço para quebra de palavra
            while (fim > posicao && historia[fim] != ' ')
            {
                fim--;
            }
        }

        int tamanho = fim - posicao;
        if (tamanho > 0 && tamanho < 199) // Verificar limites do array
        {
            strncpy(linha, &historia[posicao], tamanho);
            linha[tamanho] = '\0';
            DrawTextEx(fonte, linha, (Vector2){x + 20, posY}, tamanhoFonte, 1, WHITE);
            posY += 20;
            posicao = (fim < tamanhoHistoria && historia[fim] == ' ') ? fim + 1 : fim;
        }
        else
        {
            break;
        }
    }
}

// -------------------------------- FIM - FUNÇÃO PARA DESENHAR INFORMAÇÕES DO PERSONAGEM ------------------------------------

// -------------------------------- INICIO - FUNÇÕES DO SISTEMA DE LUTA ---------------------------------

void DesenharBarraVida(int x, int y, int largura, int altura, int vidaAtual, int vidaMaxima, Color cor)
{
    // Fundo da barra
    DrawRectangle(x, y, largura, altura, DARKGRAY);

    // Barra de vida
    float porcentagem = (float)vidaAtual / (float)vidaMaxima;
    int larguraVida = (int)(largura * porcentagem);
    DrawRectangle(x, y, larguraVida, altura, cor);

    // Borda
    DrawRectangleLines(x, y, largura, altura, WHITE);

    // Texto
    const char *textoVida = TextFormat("%d/%d", vidaAtual, vidaMaxima);
    DrawText(textoVida, x + largura / 2 - MeasureText(textoVida, 20) / 2, y + altura / 2 - 10, 20, WHITE);
}

void DesenharBarraPoder(int x, int y, int largura, int altura, int poderAtual, int poderMaximo)
{
    // Fundo da barra
    DrawRectangle(x, y, largura, altura, DARKGRAY);

    // Barra de poder
    float porcentagem = (float)poderAtual / (float)poderMaximo;
    int larguraPoder = (int)(largura * porcentagem);
    DrawRectangle(x, y, larguraPoder, altura, BLUE);

    // Borda
    DrawRectangleLines(x, y, largura, altura, WHITE);

    // Texto
    const char *textoPoder = TextFormat("%d/%d", poderAtual, poderMaximo);
    DrawText(textoPoder, x + largura / 2 - MeasureText(textoPoder, 16) / 2, y + altura / 2 - 8, 16, WHITE);
}

void DesenharHUD(EstadoJogo *estado, Font fonte)
{
    // HUD Player 1
    DrawText(estado->jogador1->nome, 50, 50, 32, BLUE);
    DrawText("VIDA:", 50, 90, 20, WHITE);
    DesenharBarraVida(120, 90, 300, 25, estado->jogador1->vidaAtual, estado->jogador1->vidaMaxima, RED);
    DrawText("PODER:", 50, 125, 20, WHITE);
    DesenharBarraPoder(120, 125, 300, 20, estado->jogador1->poderAtual, estado->jogador1->poderMaximo);

    // HUD Player 2
    DrawText(estado->jogador2->nome, LARGURA_TELA - 450, 50, 32, RED);
    DrawText("VIDA:", LARGURA_TELA - 450, 90, 20, WHITE);
    DesenharBarraVida(LARGURA_TELA - 420, 90, 300, 25, estado->jogador2->vidaAtual, estado->jogador2->vidaMaxima, RED);
    DrawText("PODER:", LARGURA_TELA - 450, 125, 20, WHITE);
    DesenharBarraPoder(LARGURA_TELA - 420, 125, 300, 20, estado->jogador2->poderAtual, estado->jogador2->poderMaximo);

    // Timer e Round
    const char *textoRound = TextFormat("ROUND %d/%d", estado->roundAtual, MAX_ROUNDS);
    DrawText(textoRound, LARGURA_TELA / 2 - MeasureText(textoRound, 32) / 2, 30, 32, YELLOW);

    const char *textoTempo = TextFormat("%.0f", estado->tempoRound);
    DrawText(textoTempo, LARGURA_TELA / 2 - MeasureText(textoTempo, 48) / 2, 70, 48, WHITE);
}

void DesenharTelaSelecaoMapa(Mapa mapas[], int mapaAtual, Font fonte)
{
    // Fundo escuro
    DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(BLACK, 0.8f));

    // Título
    const char *titulo = "ESCOLHA O MAPA DE LUTA";
    DrawTextEx(fonte, titulo, (Vector2){LARGURA_TELA / 2 - MeasureTextEx(fonte, titulo, 48, 1).x / 2, 100}, 48, 1, WHITE);

    // Preview do mapa
    int larguraMapa = 800;
    int alturaMapa = 450;
    int xMapa = (LARGURA_TELA - larguraMapa) / 2;
    int yMapa = 200;

    DrawTexturePro(mapas[mapaAtual].textura,
                   (Rectangle){0, 0, mapas[mapaAtual].textura.width, mapas[mapaAtual].textura.height},
                   (Rectangle){xMapa, yMapa, larguraMapa, alturaMapa},
                   (Vector2){0, 0}, 0.0f, WHITE);

    // Borda do mapa selecionado
    DrawRectangleLines(xMapa - 5, yMapa - 5, larguraMapa + 10, alturaMapa + 10, YELLOW);

    // Nome do mapa
    const char *nomeMapa = mapas[mapaAtual].nome;
    DrawTextEx(fonte, nomeMapa, (Vector2){LARGURA_TELA / 2 - MeasureTextEx(fonte, nomeMapa, 36, 1).x / 2, yMapa + alturaMapa + 30}, 36, 1, YELLOW);

    // Descrição do mapa
    const char *descricao = mapas[mapaAtual].descricao;
    DrawTextEx(fonte, descricao, (Vector2){LARGURA_TELA / 2 - MeasureTextEx(fonte, descricao, 24, 1).x / 2, yMapa + alturaMapa + 80}, 24, 1, LIGHTGRAY);

    // Controles
    DrawText("← → para navegar mapas", 50, ALTURA_TELA - 100, 24, WHITE);
    DrawText("ENTER para confirmar", 50, ALTURA_TELA - 70, 24, GREEN);
    DrawText("BACKSPACE para voltar", 50, ALTURA_TELA - 40, 24, GRAY);
}

void CarregarPosesPersonagem(Personagem *p, const char *nomePersonagem)
{
    char caminhoArquivo[256];
    printf("Carregando poses para: %s\n", nomePersonagem);

    // Tentar carregar poses específicas para cada personagem
    // Formato: personagens/[nome]_soco.png, personagens/[nome]_chute.png, etc.

    // Pose de soco
    sprintf(caminhoArquivo, "./personagens/%s_soco.png", nomePersonagem);
    Texture2D texturaSoco = LoadTexture(caminhoArquivo);
    if (texturaSoco.id > 0)
    {
        p->texturaSoco = texturaSoco;
        printf("✓ Carregada pose de soco para %s!\n", nomePersonagem);
    }
    else
    {
        printf("✗ Não encontrada pose de soco: %s\n", caminhoArquivo);
    }

    // Pose de chute
    sprintf(caminhoArquivo, "./personagens/%s_chute.png", nomePersonagem);
    Texture2D texturaChute = LoadTexture(caminhoArquivo);
    if (texturaChute.id > 0)
    {
        p->texturaChute = texturaChute;
        printf("✓ Carregada pose de chute para %s!\n", nomePersonagem);
    }
    else
    {
        printf("✗ Não encontrada pose de chute: %s\n", caminhoArquivo);
    }

    // Pose de poder
    sprintf(caminhoArquivo, "./personagens/%s_poder.png", nomePersonagem);
    Texture2D texturaPoder = LoadTexture(caminhoArquivo);
    if (texturaPoder.id > 0)
    {
        p->texturaPoder = texturaPoder;
        printf("✓ Carregada pose de poder para %s!\n", nomePersonagem);
    }
    else
    {
        printf("✗ Não encontrada pose de poder: %s\n", caminhoArquivo);
    }

    // Pose de defesa
    sprintf(caminhoArquivo, "./personagens/%s_defesa.png", nomePersonagem);
    Texture2D texturaDefesa = LoadTexture(caminhoArquivo);
    if (texturaDefesa.id > 0)
    {
        p->texturaDefesa = texturaDefesa;
        printf("✓ Carregada pose de defesa para %s!\n", nomePersonagem);
    }

    // Pose de dano
    sprintf(caminhoArquivo, "./personagens/%s_dano.png", nomePersonagem);
    Texture2D texturaDano = LoadTexture(caminhoArquivo);
    if (texturaDano.id > 0)
    {
        p->texturaDano = texturaDano;
        printf("✓ Carregada pose de dano para %s!\n", nomePersonagem);
    }

    // Casos especiais para personagens específicos
    if (strcmp(nomePersonagem, "Joana") == 0)
    {
        // Tentar carregar poses específicas do João para Joana
        sprintf(caminhoArquivo, "./personagens/joao_soco.png");
        Texture2D joaoSoco = LoadTexture(caminhoArquivo);
        if (joaoSoco.id > 0 && p->texturaSoco.id == 0)
        {
            p->texturaSoco = joaoSoco;
            printf("✓ Usando pose de soco do João para Joana!\n");
        }

        sprintf(caminhoArquivo, "./personagens/joao_chute.png");
        Texture2D joaoChute = LoadTexture(caminhoArquivo);
        if (joaoChute.id > 0 && p->texturaChute.id == 0)
        {
            p->texturaChute = joaoChute;
            printf("✓ Usando pose de chute do João para Joana!\n");
        }

        sprintf(caminhoArquivo, "./personagens/joao_poder.png");
        Texture2D joaoPoder = LoadTexture(caminhoArquivo);
        if (joaoPoder.id > 0 && p->texturaPoder.id == 0)
        {
            p->texturaPoder = joaoPoder;
            printf("✓ Usando pose de poder do João para Joana!\n");
        }

        // Pose inicial especial
        Texture2D poseInicial = LoadTexture("./personagens/joao pose inicial.png");
        if (poseInicial.id > 0)
        {
            p->texturaLuta = poseInicial;
            printf("✓ Carregada pose inicial especial para Joana!\n");
        }
    }

    printf("Carregamento de poses concluído para %s\n\n", nomePersonagem);
}

void InicializarPersonagem(Personagem *p, const char *nomePersonagem)
{
    p->vidaMaxima = VIDA_MAXIMA;
    p->vidaAtual = VIDA_MAXIMA;
    p->poderMaximo = PODER_MAXIMO;
    p->poderAtual = 0;
    p->danoSoco = 10;
    p->danoChute = 20;
    p->danoPoder = 50;

    // Inicializar sistema de animação
    p->poseAtual = POSE_IDLE;
    p->timerAnimacao = 0.0f;
    p->duracaoAnimacao = 0.0f;
    p->animando = false;

    // Carregar texturas de defesa e dano (usando as existentes como base)
    p->texturaDefesa = p->texturaLuta;
    p->texturaDano = p->texturaLuta;

    // Carregar poses específicas do personagem
    CarregarPosesPersonagem(p, nomePersonagem);
}

void IniciarAnimacao(Personagem *p, TipoPose pose, float duracao)
{
    p->poseAtual = pose;
    p->timerAnimacao = 0.0f;
    p->duracaoAnimacao = duracao;
    p->animando = true;
}

void AtualizarAnimacao(Personagem *p, float deltaTime)
{
    if (p->animando)
    {
        p->timerAnimacao += deltaTime;
        if (p->timerAnimacao >= p->duracaoAnimacao)
        {
            p->poseAtual = POSE_IDLE;
            p->animando = false;
            p->timerAnimacao = 0.0f;
        }
    }
}

Texture2D ObterTexturaAtual(Personagem *p)
{
    switch (p->poseAtual)
    {
    case POSE_SOCO:
        return p->texturaSoco;
    case POSE_CHUTE:
        return p->texturaChute;
    case POSE_PODER:
        return p->texturaPoder;
    case POSE_DEFESA:
        return p->texturaDefesa;
    case POSE_DANO:
        return p->texturaDano;
    case POSE_IDLE:
    default:
        return p->texturaLuta;
    }
}

void CriarEfeitoImpacto(SistemaParticulas *sistema, Vector2 posicao, Color cor, int numParticulas)
{
    sistema->ativo = true;
    sistema->timer = 0.0f;
    sistema->numParticulas = numParticulas;

    for (int i = 0; i < numParticulas && i < 50; i++)
    {
        sistema->particulas[i].posicao = posicao;
        sistema->particulas[i].velocidade = (Vector2){
            (float)(GetRandomValue(-200, 200)),
            (float)(GetRandomValue(-200, 200))};
        sistema->particulas[i].cor = cor;
        sistema->particulas[i].vida = 1.0f;
        sistema->particulas[i].vidaMaxima = 1.0f;
        sistema->particulas[i].tamanho = (float)GetRandomValue(3, 8);
    }
}

void AtualizarParticulas(SistemaParticulas *sistema, float deltaTime)
{
    if (!sistema->ativo)
        return;

    sistema->timer += deltaTime;

    for (int i = 0; i < sistema->numParticulas; i++)
    {
        Particula *p = &sistema->particulas[i];

        // Atualizar posição
        p->posicao.x += p->velocidade.x * deltaTime;
        p->posicao.y += p->velocidade.y * deltaTime;

        // Aplicar gravidade
        p->velocidade.y += 300.0f * deltaTime;

        // Reduzir vida
        p->vida -= deltaTime * 2.0f;

        // Atualizar cor com fade
        float alpha = p->vida / p->vidaMaxima;
        p->cor.a = (unsigned char)(255 * alpha);
    }

    // Desativar sistema quando todas as partículas morrerem
    if (sistema->timer > 1.0f)
    {
        sistema->ativo = false;
    }
}

void DesenharParticulas(SistemaParticulas *sistema)
{
    if (!sistema->ativo)
        return;

    for (int i = 0; i < sistema->numParticulas; i++)
    {
        Particula *p = &sistema->particulas[i];
        if (p->vida > 0)
        {
            DrawCircleV(p->posicao, p->tamanho, p->cor);
        }
    }
}

// -------------------------------- FIM - FUNÇÕES DO SISTEMA DE LUTA ------------------------------------

// -------------------------------- INICIO - FUNÇÕES DO MENU DE OPÇÕES ---------------------------------

void InicializarConfiguracaoPadrao(ConfiguracaoJogo *config)
{
    // Controles padrão Player 1
    config->teclaPoderP1 = KEY_Q;
    config->teclaSocoP1 = KEY_E;
    config->teclaChute1 = KEY_R;
    config->teclaEsquerdaP1 = KEY_A;
    config->teclaDireitaP1 = KEY_D;
    config->teclaConfirmarP1 = KEY_ENTER;

    // Controles padrão Player 2
    config->teclaPoderP2 = KEY_P;
    config->teclaSocoP2 = KEY_O;
    config->teclaChute2 = KEY_I;
    config->teclaEsquerdaP2 = KEY_LEFT;
    config->teclaDireitaP2 = KEY_RIGHT;
    config->teclaConfirmarP2 = KEY_SPACE;

    // Volume padrão
    config->volumeMusica = 0.5f;
    config->volumeEfeitos = 0.7f;
}

const char *ObterNomeTecla(int tecla)
{
    switch (tecla)
    {
    case KEY_Q:
        return "Q";
    case KEY_W:
        return "W";
    case KEY_E:
        return "E";
    case KEY_R:
        return "R";
    case KEY_T:
        return "T";
    case KEY_Y:
        return "Y";
    case KEY_U:
        return "U";
    case KEY_I:
        return "I";
    case KEY_O:
        return "O";
    case KEY_P:
        return "P";
    case KEY_A:
        return "A";
    case KEY_S:
        return "S";
    case KEY_D:
        return "D";
    case KEY_F:
        return "F";
    case KEY_G:
        return "G";
    case KEY_H:
        return "H";
    case KEY_J:
        return "J";
    case KEY_K:
        return "K";
    case KEY_L:
        return "L";
    case KEY_Z:
        return "Z";
    case KEY_X:
        return "X";
    case KEY_C:
        return "C";
    case KEY_V:
        return "V";
    case KEY_B:
        return "B";
    case KEY_N:
        return "N";
    case KEY_M:
        return "M";
    case KEY_SPACE:
        return "SPACE";
    case KEY_ENTER:
        return "ENTER";
    case KEY_LEFT:
        return "←";
    case KEY_RIGHT:
        return "→";
    case KEY_UP:
        return "↑";
    case KEY_DOWN:
        return "↓";
    default:
        return "?";
    }
}

void DesenharTelaOpcoes(ConfiguracaoJogo *config, int opcaoSelecionada, bool aguardandoTecla, Font fonte)
{
    // Fundo escuro
    DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(BLACK, 0.9f));

    // Título
    const char *titulo = "OPÇÕES";
    int larguraTitulo = MeasureText(titulo, 60);
    DrawText(titulo, LARGURA_TELA / 2 - larguraTitulo / 2, 100, 60, WHITE);

    // Opções de configuração
    const char *opcoes[OPCOES_CONFIG] = {
        "CONTROLES PLAYER 1",
        "CONTROLES PLAYER 2",
        "VOLUME MÚSICA",
        "VOLUME EFEITOS",
        "RESETAR CONTROLES",
        "SALVAR CONFIGURAÇÕES",
        "CARREGAR CONFIGURAÇÕES",
        "VOLTAR"};

    int posY = 250;
    int espacamento = 80;

    for (int i = 0; i < OPCOES_CONFIG; i++)
    {
        Color corTexto = (i == opcaoSelecionada) ? YELLOW : WHITE;
        Color corCaixa = (i == opcaoSelecionada) ? Fade(YELLOW, 0.3f) : Fade(WHITE, 0.1f);

        // Caixa da opção
        int larguraCaixa = 800;
        int alturaCaixa = 60;
        int x = (LARGURA_TELA - larguraCaixa) / 2;
        int y = posY + i * espacamento - 10;

        DrawRectangleRounded((Rectangle){x, y, larguraCaixa, alturaCaixa}, 0.1f, 10, corCaixa);
        DrawRectangleRoundedLines((Rectangle){x, y, larguraCaixa, alturaCaixa}, 0.1f, 10, corTexto);

        // Texto da opção
        DrawText(opcoes[i], x + 20, posY + i * espacamento, 32, corTexto);

        // Valores específicos
        if (i == 0) // Controles Player 1
        {
            const char *controles = TextFormat("Poder:%s Soco:%s Chute:%s",
                                               ObterNomeTecla(config->teclaPoderP1),
                                               ObterNomeTecla(config->teclaSocoP1),
                                               ObterNomeTecla(config->teclaChute1));
            DrawText(controles, x + 20, posY + i * espacamento + 35, 20, LIGHTGRAY);
        }
        else if (i == 1) // Controles Player 2
        {
            const char *controles = TextFormat("Poder:%s Soco:%s Chute:%s",
                                               ObterNomeTecla(config->teclaPoderP2),
                                               ObterNomeTecla(config->teclaSocoP2),
                                               ObterNomeTecla(config->teclaChute2));
            DrawText(controles, x + 20, posY + i * espacamento + 35, 20, LIGHTGRAY);
        }
        else if (i == 2) // Volume Música
        {
            int barraLargura = 200;
            int barraX = x + 400;
            int barraY = posY + i * espacamento + 15;

            // Barra de volume
            DrawRectangle(barraX, barraY, barraLargura, 20, DARKGRAY);
            DrawRectangle(barraX, barraY, (int)(barraLargura * config->volumeMusica), 20, GREEN);
            DrawRectangleLines(barraX, barraY, barraLargura, 20, WHITE);

            const char *volumeTexto = TextFormat("%.0f%%", config->volumeMusica * 100);
            DrawText(volumeTexto, barraX + barraLargura + 10, barraY, 20, WHITE);
        }
        else if (i == 3) // Volume Efeitos
        {
            int barraLargura = 200;
            int barraX = x + 400;
            int barraY = posY + i * espacamento + 15;

            // Barra de volume
            DrawRectangle(barraX, barraY, barraLargura, 20, DARKGRAY);
            DrawRectangle(barraX, barraY, (int)(barraLargura * config->volumeEfeitos), 20, BLUE);
            DrawRectangleLines(barraX, barraY, barraLargura, 20, WHITE);

            const char *volumeTexto = TextFormat("%.0f%%", config->volumeEfeitos * 100);
            DrawText(volumeTexto, barraX + barraLargura + 10, barraY, 20, WHITE);
        }
    }

    // Instruções
    if (aguardandoTecla)
    {
        const char *instrucao = "PRESSIONE UMA TECLA PARA CONFIGURAR...";
        int larguraInstrucao = MeasureText(instrucao, 24);
        DrawText(instrucao, LARGURA_TELA / 2 - larguraInstrucao / 2, ALTURA_TELA - 100, 24, YELLOW);
    }
    else
    {
        const char *instrucoes = "↑/↓: Navegar | ENTER: Selecionar | ←/→: Ajustar Volume | BACKSPACE: Voltar";
        int larguraInstrucoes = MeasureText(instrucoes, 20);
        DrawText(instrucoes, LARGURA_TELA / 2 - larguraInstrucoes / 2, ALTURA_TELA - 80, 20, LIGHTGRAY);
    }
}

// -------------------------------- FIM - FUNÇÕES DO MENU DE OPÇÕES ------------------------------------

int main(void)
{
    InitWindow(LARGURA_TELA, ALTURA_TELA, "UNI FIGHT - Jogo de Luta");
    SetTargetFPS(60);
    InitAudioDevice();

    Font fontePixel = LoadFont("fontes/Pixelify_Sans/PixelifySans-VariableFont_wght.ttf");
    Texture2D fundoMenu = LoadTexture("./fundoMenu2.jpg");
    Texture2D fundoSelecao = LoadTexture("./fundo.png");
    Texture2D logo = LoadTexture("./logoUnifight3.png");

    // Carregar música de fundo
    Music musicaFundo = LoadMusicStream("musicas/menu_background.mp3");
    PlayMusicStream(musicaFundo);

    // Carregar sons (placeholder - você pode adicionar arquivos de som)
    // Sons sons = {0};
    // sons.somSoco = LoadSound("sons/soco.wav");
    // sons.somChute = LoadSound("sons/chute.wav");
    // sons.somPoder = LoadSound("sons/poder.wav");

    // Configurações do jogo
    ConfiguracaoJogo config = {0};
    InicializarConfiguracaoPadrao(&config);

    // Variáveis do menu de opções
    int opcaoSelecionada = 0;
    bool aguardandoTecla = false;
    int teclaParaConfigurar = -1; // 0-5 para as 6 teclas configuráveis

    OpcaoMenu menu[OPCOES_MENU] = {
        {"JOGAR", 600},
        {"OPCOES", 700},
        {"SAIR", 800}};
    int menuSelecionado = 0;

    Personagem personagens[MAX_PERSONAGENS] = {
        {LoadTexture("./personagens/kaelPerfil.jpg"),
         "Kael",
         "Fogo",
         "Homem de pele morena, corpo forte e definido. Treinou com monges e feiticeiros, aprendendo a controlar o fogo. Agora é caçador de monstros e artefatos arcanos.",
         RED,
         LoadTexture("./personagens/kael.jpg"), LoadTexture("./personagens/kael.jpg"), LoadTexture("./personagens/kael.jpg"), LoadTexture("./personagens/kael.jpg")},
        {LoadTexture("./personagens/milenaPerfil.jpg"),
         "Dandara",
         "Gelo",
         "Mulher esbelta com cabelos rosas. Ex-mercenária abandonada por seu grupo, tornou-se caçadora de recompensas. Guerreira experiente e astuta.",
         SKYBLUE,
         LoadTexture("./personagens/milena.jpg"), LoadTexture("./personagens/milena.jpg"), LoadTexture("./personagens/milena.jpg"), LoadTexture("./personagens/milena.jpg")},
        {LoadTexture("./personagens/juliaPerfil.jpg"),
         "Valéria",
         "Gelo",
         "Porte ágil, cabelos castanho-escuros curtos. Após assassinato dos pais, trabalhou como mensageira e espiã. Hoje é caçadora de segredos.",
         SKYBLUE,
         LoadTexture("./personagens/julia.jpg"), LoadTexture("./personagens/julia.jpg"), LoadTexture("./personagens/julia.jpg"), LoadTexture("./personagens/julia.jpg")},
        {LoadTexture("./personagens/kaykPerfil.jpg"),
         "Tim",
         "Gelo",
         "Jovem guerreiro das montanhas. Treinado pelo avô, carrega kimono herdado. Promessa de usar força apenas para proteger outros.",
         SKYBLUE,
         LoadTexture("./personagens/kayk.png"), LoadTexture("./personagens/kayk.png"), LoadTexture("./personagens/kayk.png"), LoadTexture("./personagens/kayk.png")},
        {LoadTexture("./personagens/joaoPerfil.jpg"),
         "Joana",
         "Fogo",
         "Perita em artes marciais. Quando vê pessoas em perigo, usa suas habilidades e domínio sobre o fogo para deter inimigos.",
         RED,
         LoadTexture("./personagens/joao.png"), LoadTexture("./personagens/joao.png"), LoadTexture("./personagens/joao.png"), LoadTexture("./personagens/joao.png")},
        {LoadTexture("./personagens/yuriPerfil.jpg"),
         "Yuri",
         "Fogo",
         "Físico construído para combate. Ex-capitão da guarda, foi traído e preso. Fugiu e vive como mercenário e guarda-costas.",
         RED,
         LoadTexture("./personagens/yuri.jpg"), LoadTexture("./personagens/yuri.jpg"), LoadTexture("./personagens/yuri.jpg"), LoadTexture("./personagens/yuri.jpg")}};

    // Inicializar stats dos personagens
    for (int i = 0; i < MAX_PERSONAGENS; i++)
    {
        InicializarPersonagem(&personagens[i], personagens[i].nome);
    }

    // Mapas de luta
    Mapa mapas[MAX_MAPAS] = {
        {LoadTexture("./frames/frame-1.png"), "Arena Vulcânica", "Arena cercada por lava e rochas ígneas. Ideal para lutadores de fogo."},
        {LoadTexture("./frames/frame-2.png"), "Caverna Cristalina", "Caverna mística com cristais de gelo brilhantes. Favorece usuários de gelo."},
        {LoadTexture("./frames/frame-3.png"), "Templo Gelado", "Antigo templo nas montanhas geladas. Ambiente sagrado e equilibrado."},
        {LoadTexture("./frames/frame-4.png"), "Floresta Ancestral", "Floresta antiga com ruínas místicas. Energia natural balanceada."},
        {LoadTexture("./frames/frame-5.png"), "Coliseu Neutro", "Arena equilibrada sem vantagens elementais para todos os lutadores."},
        {LoadTexture("./frames/frame-6.png"), "Santuário Perdido", "Ruínas de um antigo santuário com poderes místicos adormecidos."}};

    int selecionadoJogador1 = 0;
    int selecionadoJogador2 = MAX_PERSONAGENS - 1;
    int mapaAtual = 0;

    // Estado do jogo
    EstadoJogo estado = {0};
    estado.roundAtual = 1;
    estado.tempoRound = TEMPO_ROUND;
    estado.jogador1Confirmado = false;
    estado.jogador2Confirmado = false;
    estado.mapaConfirmado = false;

    Tela telaAtual = TELA_MENU;
    int tamanhoFonte = 40;

    while (!WindowShouldClose())
    {
        // Atualizar música de fundo
        UpdateMusicStream(musicaFundo);
        SetMusicVolume(musicaFundo, config.volumeMusica);

        if (telaAtual == TELA_MENU)
        {
            if (IsKeyPressed(KEY_DOWN))
                menuSelecionado = (menuSelecionado + 1) % OPCOES_MENU;
            if (IsKeyPressed(KEY_UP))
                menuSelecionado = (menuSelecionado - 1 + OPCOES_MENU) % OPCOES_MENU;

            if (IsKeyPressed(KEY_ENTER))
            {
                if (menuSelecionado == 0)
                {
                    telaAtual = TELA_SELECAO;
                }
                else if (menuSelecionado == 1)
                {
                    telaAtual = TELA_OPCOES;
                    opcaoSelecionada = 0;
                }
                else if (menuSelecionado == 2)
                {
                    break;
                }
            }
        }
        else if (telaAtual == TELA_OPCOES)
        {
            if (!aguardandoTecla)
            {
                // Navegação no menu de opções
                if (IsKeyPressed(KEY_DOWN))
                    opcaoSelecionada = (opcaoSelecionada + 1) % OPCOES_CONFIG;
                if (IsKeyPressed(KEY_UP))
                    opcaoSelecionada = (opcaoSelecionada - 1 + OPCOES_CONFIG) % OPCOES_CONFIG;

                // Ajustar volume com setas
                if (opcaoSelecionada == 2) // Volume Música
                {
                    if (IsKeyPressed(KEY_LEFT))
                        config.volumeMusica = fmaxf(0.0f, config.volumeMusica - 0.1f);
                    if (IsKeyPressed(KEY_RIGHT))
                        config.volumeMusica = fminf(1.0f, config.volumeMusica + 0.1f);
                }
                else if (opcaoSelecionada == 3) // Volume Efeitos
                {
                    if (IsKeyPressed(KEY_LEFT))
                        config.volumeEfeitos = fmaxf(0.0f, config.volumeEfeitos - 0.1f);
                    if (IsKeyPressed(KEY_RIGHT))
                        config.volumeEfeitos = fminf(1.0f, config.volumeEfeitos + 0.1f);
                }

                // Selecionar opção
                if (IsKeyPressed(KEY_ENTER))
                {
                    if (opcaoSelecionada == 0) // Controles Player 1
                    {
                        aguardandoTecla = true;
                        teclaParaConfigurar = 0; // Começar com poder P1
                    }
                    else if (opcaoSelecionada == 1) // Controles Player 2
                    {
                        aguardandoTecla = true;
                        teclaParaConfigurar = 3; // Começar com poder P2
                    }
                    else if (opcaoSelecionada == 4) // Resetar controles
                    {
                        InicializarConfiguracaoPadrao(&config);
                    }
                    else if (opcaoSelecionada == 7) // Voltar
                    {
                        telaAtual = TELA_MENU;
                    }
                }

                // Voltar ao menu
                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    telaAtual = TELA_MENU;
                }
            }
            else
            {
                // Aguardando configuração de tecla
                int tecla = GetKeyPressed();
                if (tecla != 0)
                {
                    // Configurar a tecla baseada no índice
                    switch (teclaParaConfigurar)
                    {
                    case 0:
                        config.teclaPoderP1 = tecla;
                        break;
                    case 1:
                        config.teclaSocoP1 = tecla;
                        break;
                    case 2:
                        config.teclaChute1 = tecla;
                        break;
                    case 3:
                        config.teclaPoderP2 = tecla;
                        break;
                    case 4:
                        config.teclaSocoP2 = tecla;
                        break;
                    case 5:
                        config.teclaChute2 = tecla;
                        break;
                    }

                    teclaParaConfigurar++;

                    // Se configurou todas as teclas do player atual, parar
                    if ((teclaParaConfigurar == 3 && opcaoSelecionada == 0) ||
                        (teclaParaConfigurar == 6 && opcaoSelecionada == 1))
                    {
                        aguardandoTecla = false;
                        teclaParaConfigurar = -1;
                    }
                }

                // Cancelar configuração
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    aguardandoTecla = false;
                    teclaParaConfigurar = -1;
                }
            }
        }
        else if (telaAtual == TELA_SELECAO)
        {
            // Controles Player 1
            if (IsKeyPressed(KEY_D))
                selecionadoJogador1 = (selecionadoJogador1 + 1) % MAX_PERSONAGENS;
            if (IsKeyPressed(KEY_A))
                selecionadoJogador1 = (selecionadoJogador1 - 1 + MAX_PERSONAGENS) % MAX_PERSONAGENS;

            // Controles Player 2
            if (IsKeyPressed(KEY_RIGHT))
                selecionadoJogador2 = (selecionadoJogador2 + 1) % MAX_PERSONAGENS;
            if (IsKeyPressed(KEY_LEFT))
                selecionadoJogador2 = (selecionadoJogador2 - 1 + MAX_PERSONAGENS) % MAX_PERSONAGENS;

            // Navegação de mapas
            if (IsKeyPressed(KEY_UP))
                mapaAtual = (mapaAtual - 1 + MAX_MAPAS) % MAX_MAPAS;
            if (IsKeyPressed(KEY_DOWN))
                mapaAtual = (mapaAtual + 1) % MAX_MAPAS;

            // Confirmação de seleção
            if (IsKeyPressed(KEY_ENTER) && !estado.jogador1Confirmado)
            {
                estado.jogador1Confirmado = true;
                estado.jogador1 = &personagens[selecionadoJogador1];
                printf("Jogador 1 escolheu: %s (Habilidade: %s)\n",
                       personagens[selecionadoJogador1].nome,
                       personagens[selecionadoJogador1].habilidade);
            }
            if (IsKeyPressed(KEY_SPACE) && !estado.jogador2Confirmado)
            {
                estado.jogador2Confirmado = true;
                estado.jogador2 = &personagens[selecionadoJogador2];
                printf("Jogador 2 escolheu: %s (Habilidade: %s)\n",
                       personagens[selecionadoJogador2].nome,
                       personagens[selecionadoJogador2].habilidade);
            }

            // Ir para seleção de mapa quando ambos confirmaram
            if (estado.jogador1Confirmado && estado.jogador2Confirmado)
            {
                telaAtual = TELA_SELECAO_MAPA;
            }

            // Voltar ao menu
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                telaAtual = TELA_MENU;
                estado.jogador1Confirmado = false;
                estado.jogador2Confirmado = false;
            }
        }
        else if (telaAtual == TELA_SELECAO_MAPA)
        {
            // Navegação de mapas
            if (IsKeyPressed(KEY_LEFT))
                mapaAtual = (mapaAtual - 1 + MAX_MAPAS) % MAX_MAPAS;
            if (IsKeyPressed(KEY_RIGHT))
                mapaAtual = (mapaAtual + 1) % MAX_MAPAS;

            // Confirmar mapa
            if (IsKeyPressed(KEY_ENTER))
            {
                estado.mapaAtual = &mapas[mapaAtual];
                estado.mapaConfirmado = true;
                telaAtual = TELA_LUTA;
            }

            // Voltar para seleção de personagens
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                telaAtual = TELA_SELECAO;
                estado.jogador1Confirmado = false;
                estado.jogador2Confirmado = false;
            }
        }
        else if (telaAtual == TELA_LUTA)
        {
            // Atualizar timer
            estado.tempoRound -= GetFrameTime();
            if (estado.tempoRound <= 0)
            {
                estado.tempoRound = TEMPO_ROUND;
                estado.roundAtual++;
                if (estado.roundAtual > MAX_ROUNDS)
                {
                    // Fim do jogo - voltar ao menu
                    telaAtual = TELA_MENU;
                    estado.roundAtual = 1;
                    estado.jogador1Confirmado = false;
                    estado.jogador2Confirmado = false;
                    estado.mapaConfirmado = false;
                }
            }

            // Atualizar animações dos personagens
            AtualizarAnimacao(estado.jogador1, GetFrameTime());
            AtualizarAnimacao(estado.jogador2, GetFrameTime());

            // Atualizar efeitos visuais
            AtualizarParticulas(&estado.particulasImpacto, GetFrameTime());
            if (estado.flashTela > 0)
                estado.flashTela -= GetFrameTime() * 3.0f;
            if (estado.tremor > 0)
                estado.tremor -= GetFrameTime() * 5.0f;

            // Controles de luta Player 1 (configuráveis)
            if (IsKeyPressed(config.teclaPoderP1) && estado.jogador1->poderAtual >= 50 && !estado.jogador1->animando)
            {
                estado.jogador2->vidaAtual -= estado.jogador1->danoPoder;
                estado.jogador1->poderAtual -= 50;
                IniciarAnimacao(estado.jogador1, POSE_PODER, 0.8f);
                IniciarAnimacao(estado.jogador2, POSE_DANO, 0.5f);

                // Efeitos visuais para ataque de poder
                Vector2 posImpacto = {LARGURA_TELA - 400 + 100, ALTURA_TELA - 400 + 150};
                CriarEfeitoImpacto(&estado.particulasImpacto, posImpacto, PURPLE, 25);
                estado.flashTela = 0.3f;
                estado.tremor = 0.2f;
                // PlaySound(sons.somPoder);
            }
            if (IsKeyPressed(config.teclaSocoP1) && !estado.jogador1->animando)
            {
                estado.jogador2->vidaAtual -= estado.jogador1->danoSoco;
                estado.jogador1->poderAtual += 10;
                IniciarAnimacao(estado.jogador1, POSE_SOCO, 0.3f);
                IniciarAnimacao(estado.jogador2, POSE_DANO, 0.3f);

                // Efeitos visuais para soco
                Vector2 posImpacto = {LARGURA_TELA - 400 + 100, ALTURA_TELA - 400 + 150};
                CriarEfeitoImpacto(&estado.particulasImpacto, posImpacto, YELLOW, 15);
                estado.tremor = 0.1f;
                // PlaySound(sons.somSoco);
            }
            if (IsKeyPressed(config.teclaChute1) && !estado.jogador1->animando)
            {
                estado.jogador2->vidaAtual -= estado.jogador1->danoChute;
                estado.jogador1->poderAtual += 15;
                IniciarAnimacao(estado.jogador1, POSE_CHUTE, 0.5f);
                IniciarAnimacao(estado.jogador2, POSE_DANO, 0.4f);

                // Efeitos visuais para chute
                Vector2 posImpacto = {LARGURA_TELA - 400 + 100, ALTURA_TELA - 400 + 150};
                CriarEfeitoImpacto(&estado.particulasImpacto, posImpacto, ORANGE, 20);
                estado.tremor = 0.15f;
                // PlaySound(sons.somChute);
            }

            // Controles de luta Player 2 (configuráveis)
            if (IsKeyPressed(config.teclaPoderP2) && estado.jogador2->poderAtual >= 50 && !estado.jogador2->animando)
            {
                estado.jogador1->vidaAtual -= estado.jogador2->danoPoder;
                estado.jogador2->poderAtual -= 50;
                IniciarAnimacao(estado.jogador2, POSE_PODER, 0.8f);
                IniciarAnimacao(estado.jogador1, POSE_DANO, 0.5f);

                // Efeitos visuais para ataque de poder
                Vector2 posImpacto = {200 + 100, ALTURA_TELA - 400 + 150};
                CriarEfeitoImpacto(&estado.particulasImpacto, posImpacto, PURPLE, 25);
                estado.flashTela = 0.3f;
                estado.tremor = 0.2f;
                // PlaySound(sons.somPoder);
            }
            if (IsKeyPressed(config.teclaSocoP2) && !estado.jogador2->animando)
            {
                estado.jogador1->vidaAtual -= estado.jogador2->danoSoco;
                estado.jogador2->poderAtual += 10;
                IniciarAnimacao(estado.jogador2, POSE_SOCO, 0.3f);
                IniciarAnimacao(estado.jogador1, POSE_DANO, 0.3f);

                // Efeitos visuais para soco
                Vector2 posImpacto = {200 + 100, ALTURA_TELA - 400 + 150};
                CriarEfeitoImpacto(&estado.particulasImpacto, posImpacto, YELLOW, 15);
                estado.tremor = 0.1f;
                // PlaySound(sons.somSoco);
            }
            if (IsKeyPressed(config.teclaChute2) && !estado.jogador2->animando)
            {
                estado.jogador1->vidaAtual -= estado.jogador2->danoChute;
                estado.jogador2->poderAtual += 15;
                IniciarAnimacao(estado.jogador2, POSE_CHUTE, 0.5f);
                IniciarAnimacao(estado.jogador1, POSE_DANO, 0.4f);

                // Efeitos visuais para chute
                Vector2 posImpacto = {200 + 100, ALTURA_TELA - 400 + 150};
                CriarEfeitoImpacto(&estado.particulasImpacto, posImpacto, ORANGE, 20);
                estado.tremor = 0.15f;
                // PlaySound(sons.somChute);
            }

            // Verificar se alguém morreu
            if (estado.jogador1->vidaAtual <= 0 || estado.jogador2->vidaAtual <= 0)
            {
                // Fim da luta - voltar ao menu
                telaAtual = TELA_MENU;
                estado.roundAtual = 1;
                estado.jogador1Confirmado = false;
                estado.jogador2Confirmado = false;
                estado.mapaConfirmado = false;

                // Reinicializar personagens
                for (int i = 0; i < MAX_PERSONAGENS; i++)
                {
                    InicializarPersonagem(&personagens[i], personagens[i].nome);
                }
            }

            // Voltar ao menu
            if (IsKeyPressed(KEY_ESCAPE))
            {
                telaAtual = TELA_MENU;
                estado.roundAtual = 1;
                estado.jogador1Confirmado = false;
                estado.jogador2Confirmado = false;
                estado.mapaConfirmado = false;
            }
        }

        // --------------------------------------------- DESENHO ----------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        if (telaAtual == TELA_MENU)
        {
            // Fundo do menu redimensionado para preencher toda a tela
            DrawTexturePro(fundoMenu,
                           (Rectangle){0, 0, (float)fundoMenu.width, (float)fundoMenu.height},
                           (Rectangle){0, 0, LARGURA_TELA, ALTURA_TELA},
                           (Vector2){0, 0}, 0.0f, WHITE);

            float escala = 0.5f;
            int posX = (LARGURA_TELA - (int)(logo.width * escala)) / 2;
            int posY = 70;

            DrawTexturePro(logo, (Rectangle){0, 0, (float)logo.width, (float)logo.height},
                           (Rectangle){posX, posY, logo.width * escala, logo.height * escala},
                           (Vector2){0, 0}, 0.0f, RAYWHITE);

            for (int i = 0; i < OPCOES_MENU; i++)
            {
                Color corTexto = (i == menuSelecionado) ? BLACK : WHITE;
                Color corCaixa = (i == menuSelecionado) ? Fade(YELLOW, 0.7f) : Fade(WHITE, 0.2f);

                const char *texto = menu[i].texto;
                int paddingY = 20;
                int larguraCaixa = 400;
                int alturaCaixa = tamanhoFonte + paddingY * 2;
                int bx = (LARGURA_TELA - larguraCaixa) / 2;
                int by = menu[i].posY - paddingY;

                DrawRectangleRounded((Rectangle){bx, by, larguraCaixa, alturaCaixa}, 0.3f, 10, corCaixa);

                Vector2 tamTexto = MeasureTextEx(fontePixel, texto, tamanhoFonte, 1);
                DrawTextEx(fontePixel, texto, (Vector2){(LARGURA_TELA - tamTexto.x) / 2, menu[i].posY}, tamanhoFonte, 1, corTexto);
            }
        }
        else if (telaAtual == TELA_OPCOES)
        {
            DesenharTelaOpcoes(&config, opcaoSelecionada, aguardandoTecla, fontePixel);
        }
        else if (telaAtual == TELA_SELECAO)
        {
            // Fundo redimensionado para preencher toda a tela
            DrawTexturePro(fundoSelecao,
                           (Rectangle){0, 0, (float)fundoSelecao.width, (float)fundoSelecao.height},
                           (Rectangle){0, 0, LARGURA_TELA, ALTURA_TELA},
                           (Vector2){0, 0}, 0.0f, WHITE);

            // Overlay escuro para melhor contraste do texto
            DrawRectangleGradientV(0, 0, LARGURA_TELA, ALTURA_TELA,
                                   Fade(BLACK, 0.3f), Fade(BLACK, 0.6f));

            int metadeLargura = LARGURA_TELA / 2;
            int alturaPersonagens = 500;

            // Painéis de fundo para as áreas dos personagens
            DrawRectangleRounded((Rectangle){20, 130, metadeLargura - 40, alturaPersonagens + 40}, 0.1f, 10, Fade(BLACK, 0.7f));
            DrawRectangleRounded((Rectangle){metadeLargura + 20, 130, metadeLargura - 40, alturaPersonagens + 40}, 0.1f, 10, Fade(BLACK, 0.7f));

            // Bordas dos painéis
            DrawRectangleRoundedLines((Rectangle){20, 130, metadeLargura - 40, alturaPersonagens + 40}, 0.1f, 10, estado.jogador1Confirmado ? GREEN : BLUE);
            DrawRectangleRoundedLines((Rectangle){metadeLargura + 20, 130, metadeLargura - 40, alturaPersonagens + 40}, 0.1f, 10, estado.jogador2Confirmado ? GREEN : RED);

            // Desenhar seleção de personagens
            DesenharSelecaoPersonagem(personagens, selecionadoJogador1, 0, 150, metadeLargura, alturaPersonagens, estado.jogador1Confirmado ? GREEN : BLUE);
            DesenharSelecaoPersonagem(personagens, selecionadoJogador2, metadeLargura, 150, metadeLargura, alturaPersonagens, estado.jogador2Confirmado ? GREEN : RED);

            // Painel do título
            DrawRectangleRounded((Rectangle){20, 20, LARGURA_TELA - 40, 80}, 0.2f, 10, Fade(BLACK, 0.8f));
            DrawRectangleRoundedLines((Rectangle){20, 20, LARGURA_TELA - 40, 80}, 0.2f, 10, WHITE);

            // Título
            const char *titulo = "SELECIONE SEU PERSONAGEM";
            int larguraTitulo = MeasureText(titulo, 50);
            DrawText(titulo, LARGURA_TELA / 2 - larguraTitulo / 2, 40, 50, WHITE);

            // Status de confirmação
            if (estado.jogador1Confirmado)
                DrawText("PLAYER 1 CONFIRMADO!", 50, 110, 24, GREEN);
            else
                DrawText("PLAYER 1 (A / D)", 50, 110, 24, BLUE);

            if (estado.jogador2Confirmado)
                DrawText("PLAYER 2 CONFIRMADO!", metadeLargura + 50, 110, 24, GREEN);
            else
                DrawText("PLAYER 2 (← / →)", metadeLargura + 50, 110, 24, RED);

            // Painel de fundo para informações dos personagens
            int posYInfo = 680;
            DrawRectangleRounded((Rectangle){30, posYInfo, LARGURA_TELA - 60, 300}, 0.1f, 10, Fade(BLACK, 0.8f));
            DrawRectangleRoundedLines((Rectangle){30, posYInfo, LARGURA_TELA - 60, 300}, 0.1f, 10, WHITE);

            // Linha divisória vertical entre os dois jogadores
            DrawLine(LARGURA_TELA / 2, posYInfo + 20, LARGURA_TELA / 2, posYInfo + 280, WHITE);

            // Informações detalhadas dos personagens selecionados
            DesenharInfoPersonagem(personagens[selecionadoJogador1], 50, posYInfo + 20, metadeLargura - 80, 260, fontePixel);
            DesenharInfoPersonagem(personagens[selecionadoJogador2], metadeLargura + 30, posYInfo + 20, metadeLargura - 80, 260, fontePixel);

            // Painel de instruções
            DrawRectangleRounded((Rectangle){30, ALTURA_TELA - 80, LARGURA_TELA - 60, 50}, 0.1f, 10, Fade(BLACK, 0.9f));
            DrawRectangleRoundedLines((Rectangle){30, ALTURA_TELA - 80, LARGURA_TELA - 60, 50}, 0.1f, 10, GRAY);

            // Instruções na parte inferior
            if (!estado.jogador1Confirmado)
                DrawText("ENTER: Confirmar Player 1", 50, ALTURA_TELA - 60, 18, BLUE);
            if (!estado.jogador2Confirmado)
                DrawText("SPACE: Confirmar Player 2", 400, ALTURA_TELA - 60, 18, RED);

            if (estado.jogador1Confirmado && estado.jogador2Confirmado)
            {
                const char *textoTransicao = "Aguarde... Indo para seleção de mapa!";
                int larguraTexto = MeasureText(textoTransicao, 24);
                DrawText(textoTransicao, LARGURA_TELA / 2 - larguraTexto / 2, ALTURA_TELA - 55, 24, YELLOW);
            }
            else
            {
                const char *textoVoltar = "BACKSPACE: Voltar ao menu";
                int larguraTexto = MeasureText(textoVoltar, 18);
                DrawText(textoVoltar, LARGURA_TELA / 2 - larguraTexto / 2, ALTURA_TELA - 40, 18, WHITE);
            }
        }
        else if (telaAtual == TELA_SELECAO_MAPA)
        {
            DesenharTelaSelecaoMapa(mapas, mapaAtual, fontePixel);
        }
        else if (telaAtual == TELA_LUTA)
        {
            // Aplicar tremor na tela
            int offsetTremor = 0;
            if (estado.tremor > 0)
            {
                offsetTremor = (int)(estado.tremor * 20 * sin(GetTime() * 50));
            }

            // Fundo do mapa redimensionado para preencher toda a tela com tremor
            DrawTexturePro(estado.mapaAtual->textura,
                           (Rectangle){0, 0, (float)estado.mapaAtual->textura.width, (float)estado.mapaAtual->textura.height},
                           (Rectangle){offsetTremor, offsetTremor, LARGURA_TELA, ALTURA_TELA},
                           (Vector2){0, 0}, 0.0f, WHITE);

            // Desenhar personagens com animação
            int posPlayer1X = 200;
            int posPlayer2X = LARGURA_TELA - 400;
            int posPlayersY = ALTURA_TELA - 400;

            // Obter texturas atuais baseadas na pose
            Texture2D texturaPlayer1 = ObterTexturaAtual(estado.jogador1);
            Texture2D texturaPlayer2 = ObterTexturaAtual(estado.jogador2);

            // Efeito de movimento durante animação
            int offsetX1 = 0, offsetY1 = 0;
            int offsetX2 = 0, offsetY2 = 0;

            if (estado.jogador1->animando)
            {
                switch (estado.jogador1->poseAtual)
                {
                case POSE_SOCO:
                    offsetX1 = 20; // Move para frente
                    break;
                case POSE_CHUTE:
                    offsetX1 = 30;  // Move mais para frente
                    offsetY1 = -10; // Pula um pouco
                    break;
                case POSE_PODER:
                    offsetY1 = -15; // Levita
                    break;
                case POSE_DANO:
                    offsetX1 = -15; // Recua
                    break;
                case POSE_DEFESA:
                    offsetX1 = -5; // Recua levemente
                    break;
                case POSE_IDLE:
                default:
                    // Sem movimento
                    break;
                }
            }

            if (estado.jogador2->animando)
            {
                switch (estado.jogador2->poseAtual)
                {
                case POSE_SOCO:
                    offsetX2 = -20; // Move para frente (direção oposta)
                    break;
                case POSE_CHUTE:
                    offsetX2 = -30; // Move mais para frente
                    offsetY2 = -10; // Pula um pouco
                    break;
                case POSE_PODER:
                    offsetY2 = -15; // Levita
                    break;
                case POSE_DANO:
                    offsetX2 = 15; // Recua
                    break;
                case POSE_DEFESA:
                    offsetX2 = 5; // Recua levemente (direção oposta)
                    break;
                case POSE_IDLE:
                default:
                    // Sem movimento
                    break;
                }
            }

            // Efeito de brilho durante animação
            Color corPlayer1 = WHITE;
            Color corPlayer2 = WHITE;

            if (estado.jogador1->animando)
            {
                float intensidade = 0.5f + 0.5f * sin(GetTime() * 10);
                switch (estado.jogador1->poseAtual)
                {
                case POSE_PODER:
                    corPlayer1 = ColorLerp(WHITE, PURPLE, intensidade * 0.5f);
                    break;
                case POSE_SOCO:
                    corPlayer1 = ColorLerp(WHITE, YELLOW, intensidade * 0.3f);
                    break;
                case POSE_CHUTE:
                    corPlayer1 = ColorLerp(WHITE, ORANGE, intensidade * 0.3f);
                    break;
                case POSE_DANO:
                    corPlayer1 = ColorLerp(WHITE, RED, intensidade * 0.4f);
                    break;
                case POSE_DEFESA:
                    corPlayer1 = ColorLerp(WHITE, BLUE, intensidade * 0.2f);
                    break;
                case POSE_IDLE:
                default:
                    // Sem efeito de cor
                    break;
                }
            }

            if (estado.jogador2->animando)
            {
                float intensidade = 0.5f + 0.5f * sin(GetTime() * 10);
                switch (estado.jogador2->poseAtual)
                {
                case POSE_PODER:
                    corPlayer2 = ColorLerp(WHITE, PURPLE, intensidade * 0.5f);
                    break;
                case POSE_SOCO:
                    corPlayer2 = ColorLerp(WHITE, YELLOW, intensidade * 0.3f);
                    break;
                case POSE_CHUTE:
                    corPlayer2 = ColorLerp(WHITE, ORANGE, intensidade * 0.3f);
                    break;
                case POSE_DANO:
                    corPlayer2 = ColorLerp(WHITE, RED, intensidade * 0.4f);
                    break;
                case POSE_DEFESA:
                    corPlayer2 = ColorLerp(WHITE, BLUE, intensidade * 0.2f);
                    break;
                case POSE_IDLE:
                default:
                    // Sem efeito de cor
                    break;
                }
            }

            DrawTexturePro(texturaPlayer1,
                           (Rectangle){0, 0, texturaPlayer1.width, texturaPlayer1.height},
                           (Rectangle){posPlayer1X + offsetX1, posPlayersY + offsetY1, 200, 300},
                           (Vector2){0, 0}, 0.0f, corPlayer1);

            DrawTexturePro(texturaPlayer2,
                           (Rectangle){0, 0, texturaPlayer2.width, texturaPlayer2.height},
                           (Rectangle){posPlayer2X + offsetX2, posPlayersY + offsetY2, 200, 300},
                           (Vector2){0, 0}, 0.0f, corPlayer2);

            // Desenhar efeitos de partículas
            DesenharParticulas(&estado.particulasImpacto);

            // Desenhar HUD
            DesenharHUD(&estado, fontePixel);

            // Efeito de flash na tela
            if (estado.flashTela > 0)
            {
                Color corFlash = Fade(WHITE, estado.flashTela);
                DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, corFlash);
            }

            // Controles de luta (dinâmicos baseados na configuração)
            const char *controlesP1 = TextFormat("Player 1: %s=Poder, %s=Soco, %s=Chute",
                                                 ObterNomeTecla(config.teclaPoderP1),
                                                 ObterNomeTecla(config.teclaSocoP1),
                                                 ObterNomeTecla(config.teclaChute1));
            DrawText(controlesP1, 50, ALTURA_TELA - 80, 18, BLUE);

            const char *controlesP2 = TextFormat("Player 2: %s=Poder, %s=Soco, %s=Chute",
                                                 ObterNomeTecla(config.teclaPoderP2),
                                                 ObterNomeTecla(config.teclaSocoP2),
                                                 ObterNomeTecla(config.teclaChute2));
            DrawText(controlesP2, 50, ALTURA_TELA - 60, 18, RED);
            DrawText("ESC: Voltar ao menu", 50, ALTURA_TELA - 40, 18, WHITE);
        }

        EndDrawing();
    }

    // Cleanup personagens
    for (int i = 0; i < MAX_PERSONAGENS; i++)
    {
        UnloadTexture(personagens[i].textura);
        UnloadTexture(personagens[i].texturaLuta);
        UnloadTexture(personagens[i].texturaSoco);
        UnloadTexture(personagens[i].texturaChute);
        UnloadTexture(personagens[i].texturaPoder);
    }

    // Cleanup mapas
    for (int i = 0; i < MAX_MAPAS; i++)
    {
        UnloadTexture(mapas[i].textura);
    }

    // Cleanup demais texturas
    UnloadTexture(fundoMenu);
    UnloadTexture(fundoSelecao);
    UnloadTexture(logo);
    UnloadFont(fontePixel);

    // Cleanup áudio
    UnloadMusicStream(musicaFundo);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}