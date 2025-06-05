#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "structs.c"
#include <locale.h>

#define LARGURA_TELA 1920
#define ALTURA_TELA 1080
#define MAX_PERSONAGENS 6
#define OPCOES_MENU 3
#define OPCOES_CONFIG 8
#define MAX_MAPAS 8
#define VIDA_MAXIMA 250
#define PODER_MAXIMO 100

#define TEMPO_ROUND 60.0f
#define MAX_ROUNDS 3

// ===== FUNÇÕES DE MOVIMENTO E SPRITES =====

void InicializarPosicaoPersonagem(Personagem *p, float x, float y, bool viradoDireita)
{
    p->posicao.x = x;
    p->posicao.y = y;
    p->velocidade.x = 0;
    p->velocidade.y = 0;
    p->velocidadeMaxima = 200.0f;
    p->viradoParaDireita = viradoDireita;
    p->podeAtacar = true;
    p->podeSeMovimentar = true;

    // Configurar hitbox
    p->hitbox.width = 80;
    p->hitbox.height = 120;
    p->hitbox.x = p->posicao.x - p->hitbox.width / 2;
    p->hitbox.y = p->posicao.y - p->hitbox.height;

    // Configurar alcances de ataque
    p->alcanceSoco = 90.0f;
    p->alcanceChute = 110.0f;
    p->alcancePoder = 150.0f;

    // Sistema de sprites melhorado
    p->frameAtual = 0;
    p->totalFrames = 4;
    p->timerFrame = 0.0f;
    p->duracaoFrame = 0.12f;                    // Animação mais rápida
    p->frameSource = (Rectangle){0, 0, 64, 64}; // Tamanho padrão do frame
}

void AtualizarHitbox(Personagem *p)
{
    p->hitbox.x = p->posicao.x - p->hitbox.width / 2;
    p->hitbox.y = p->posicao.y - p->hitbox.height;
}

void AtualizarAlcanceAtaque(Personagem *p)
{
    float alcance = 0;
    switch (p->poseAtual)
    {
    case POSE_SOCO:
        alcance = p->alcanceSoco;
        break;
    case POSE_CHUTE:
        alcance = p->alcanceChute;
        break;
    case POSE_PODER:
        alcance = p->alcancePoder;
        break;
    default:
        alcance = 0;
        break;
    }

    if (p->viradoParaDireita)
    {
        p->alcanceAtaque.x = p->posicao.x;
        p->alcanceAtaque.y = p->posicao.y - 60;
        p->alcanceAtaque.width = alcance;
        p->alcanceAtaque.height = 80;
    }
    else
    {
        p->alcanceAtaque.x = p->posicao.x - alcance;
        p->alcanceAtaque.y = p->posicao.y - 60;
        p->alcanceAtaque.width = alcance;
        p->alcanceAtaque.height = 80;
    }
}

bool VerificarProximidade(Personagem *atacante, Personagem *alvo)
{
    AtualizarAlcanceAtaque(atacante);
    return CheckCollisionRecs(atacante->alcanceAtaque, alvo->hitbox);
}

void MoverPersonagem(Personagem *p, float deltaTime, float limiteEsquerdo, float limiteDireito)
{
    if (!p->podeSeMovimentar || p->animando)
        return;

    // Aplicar movimento
    p->posicao.x += p->velocidade.x * deltaTime;

    // Verificar limites da arena
    if (p->posicao.x < limiteEsquerdo + p->hitbox.width / 2)
    {
        p->posicao.x = limiteEsquerdo + p->hitbox.width / 2;
        p->velocidade.x = 0;
    }
    if (p->posicao.x > limiteDireito - p->hitbox.width / 2)
    {
        p->posicao.x = limiteDireito - p->hitbox.width / 2;
        p->velocidade.x = 0;
    }

    // Atualizar hitbox
    AtualizarHitbox(p);

    // Determinar direção que está virado
    if (p->velocidade.x > 0)
        p->viradoParaDireita = true;
    else if (p->velocidade.x < 0)
        p->viradoParaDireita = false;
}

void ProcessarMovimentoPlayer1(Personagem *p, ConfiguracaoJogo *config)
{
    if (!p->podeSeMovimentar || p->animando)
        return;

    p->velocidade.x = 0;
    bool movendo = false;

    // Movimento mais responsivo estilo Street Fighter
    if (IsKeyDown(config->teclaEsquerdaP1))
    {
        p->velocidade.x = -p->velocidadeMaxima;
        movendo = true;
    }
    else if (IsKeyDown(config->teclaDireitaP1))
    {
        p->velocidade.x = p->velocidadeMaxima;
        movendo = true;
    }

    // Atualizar pose baseada no movimento
    if (movendo && p->poseAtual == POSE_IDLE)
    {
        p->poseAtual = POSE_WALK;
    }
    else if (!movendo && p->poseAtual == POSE_WALK && !p->animando)
    {
        p->poseAtual = POSE_IDLE;
    }
}

void ProcessarMovimentoPlayer2(Personagem *p, ConfiguracaoJogo *config)
{
    if (!p->podeSeMovimentar || p->animando)
        return;

    p->velocidade.x = 0;
    bool movendo = false;

    // Movimento mais responsivo estilo Street Fighter
    if (IsKeyDown(config->teclaEsquerdaP2))
    {
        p->velocidade.x = -p->velocidadeMaxima;
        movendo = true;
    }
    else if (IsKeyDown(config->teclaDireitaP2))
    {
        p->velocidade.x = p->velocidadeMaxima;
        movendo = true;
    }

    // Atualizar pose baseada no movimento
    if (movendo && p->poseAtual == POSE_IDLE)
    {
        p->poseAtual = POSE_WALK;
    }
    else if (!movendo && p->poseAtual == POSE_WALK && !p->animando)
    {
        p->poseAtual = POSE_IDLE;
    }
}

void AtualizarAnimacaoSprite(Personagem *p, float deltaTime)
{
    // Atualizar timer do frame
    p->timerFrame += deltaTime;

    if (p->timerFrame >= p->duracaoFrame)
    {
        p->timerFrame = 0.0f;
        p->frameAtual++;

        // Configurar frames por pose com animações mais suaves
        int maxFrames = 1;
        switch (p->poseAtual)
        {
        case POSE_IDLE:
            maxFrames = 4; // Respiração suave
            p->duracaoFrame = 0.25f;
            break;
        case POSE_WALK:
            maxFrames = 6; // Caminhada fluida
            p->duracaoFrame = 0.15f;
            break;
        case POSE_SOCO:
            maxFrames = 4; // Soco rápido
            p->duracaoFrame = 0.08f;
            break;
        case POSE_CHUTE:
            maxFrames = 5; // Chute com preparação
            p->duracaoFrame = 0.1f;
            break;
        case POSE_PODER:
            maxFrames = 8; // Poder especial elaborado
            p->duracaoFrame = 0.12f;
            break;
        case POSE_DANO:
            maxFrames = 3; // Reação ao dano
            p->duracaoFrame = 0.15f;
            break;
        case POSE_DEFESA:
            maxFrames = 2; // Posição defensiva
            p->duracaoFrame = 0.2f;
            break;
        }

        if (p->frameAtual >= maxFrames)
        {
            p->frameAtual = 0;

            // Se é uma animação de ataque, voltar ao idle
            if (p->animando && (p->poseAtual == POSE_SOCO || p->poseAtual == POSE_CHUTE ||
                                p->poseAtual == POSE_PODER || p->poseAtual == POSE_DANO))
            {
                // A animação será finalizada pela função AtualizarAnimacao
            }
        }
    }

    // Configurar source rectangle para sprite
    int frameWidth = 64;
    int frameHeight = 64;

    p->frameSource.x = p->frameAtual * frameWidth;
    p->frameSource.y = (int)p->poseAtual * frameHeight;
    p->frameSource.width = frameWidth;
    p->frameSource.height = frameHeight;
}

Texture2D ObterTexturaAtual(Personagem *p)
{
    switch (p->poseAtual)
    {
    case POSE_WALK:
        return p->texturaWalk.id > 0 ? p->texturaWalk : p->texturaLuta;
    case POSE_SOCO:
        return p->texturaSoco.id > 0 ? p->texturaSoco : p->texturaLuta;
    case POSE_CHUTE:
        return p->texturaChute.id > 0 ? p->texturaChute : p->texturaLuta;
    case POSE_PODER:
        return p->texturaPoder.id > 0 ? p->texturaPoder : p->texturaLuta;
    case POSE_DEFESA:
        return p->texturaDefesa.id > 0 ? p->texturaDefesa : p->texturaLuta;
    case POSE_DANO:
        return p->texturaDano.id > 0 ? p->texturaDano : p->texturaLuta;
    case POSE_IDLE:
    default:
        return p->texturaLuta;
    }
}

void IniciarAnimacao(Personagem *p, TipoPose pose, float duracao)
{
    p->poseAtual = pose;
    p->timerAnimacao = 0.0f;
    p->duracaoAnimacao = duracao;
    p->animando = true;
    p->frameAtual = 0; // Resetar frame para início da animação
    p->timerFrame = 0.0f;

    // Bloquear movimento durante ataques
    if (pose == POSE_SOCO || pose == POSE_CHUTE || pose == POSE_PODER)
    {
        p->podeSeMovimentar = false;
        p->podeAtacar = false;
    }
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
            p->frameAtual = 0;
            p->timerFrame = 0.0f;

            // Restaurar capacidades
            p->podeSeMovimentar = true;
            p->podeAtacar = true;
        }
    }
}

Rectangle ObterSourceRectSprite(Personagem *p)
{
    return p->frameSource;
}

void VirarPersonagemParaOponente(Personagem *p1, Personagem *p2)
{
    // Só virar se não estiver atacando
    if (!p1->animando || (p1->poseAtual != POSE_SOCO && p1->poseAtual != POSE_CHUTE && p1->poseAtual != POSE_PODER))
    {
        if (p2->posicao.x > p1->posicao.x)
            p1->viradoParaDireita = true;
        else
            p1->viradoParaDireita = false;
    }

    if (!p2->animando || (p2->poseAtual != POSE_SOCO && p2->poseAtual != POSE_CHUTE && p2->poseAtual != POSE_PODER))
    {
        if (p1->posicao.x > p2->posicao.x)
            p2->viradoParaDireita = true;
        else
            p2->viradoParaDireita = false;
    }
}

bool ExecutarAtaque(Personagem *atacante, Personagem *alvo, TipoPose tipoAtaque, int dano)
{
    if (!atacante->podeAtacar || atacante->animando)
        return false;

    // Verificar se está próximo o suficiente
    if (!VerificarProximidade(atacante, alvo))
        return false;

    // Executar ataque
    alvo->vidaAtual -= dano;
    if (alvo->vidaAtual < 0)
        alvo->vidaAtual = 0;

    // Iniciar animações
    float duracaoAtaque = 0.4f;
    if (tipoAtaque == POSE_SOCO)
        duracaoAtaque = 0.3f;
    else if (tipoAtaque == POSE_CHUTE)
        duracaoAtaque = 0.5f;
    else if (tipoAtaque == POSE_PODER)
        duracaoAtaque = 0.8f;

    IniciarAnimacao(atacante, tipoAtaque, duracaoAtaque);
    IniciarAnimacao(alvo, POSE_DANO, 0.3f);

    // Empurrar o alvo para trás
    float forcaEmpurrao = 30.0f;
    if (tipoAtaque == POSE_CHUTE)
        forcaEmpurrao = 45.0f;
    else if (tipoAtaque == POSE_PODER)
        forcaEmpurrao = 60.0f;

    if (atacante->viradoParaDireita)
    {
        alvo->posicao.x += forcaEmpurrao;
    }
    else
    {
        alvo->posicao.x -= forcaEmpurrao;
    }

    return true;
}

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

void DesenharInfoPersonagem(Personagem personagem, int x, int y, int largura, int altura, Font fonte)
{
    // Painel de fundo com gradiente
    DrawRectangleRounded((Rectangle){x, y, largura, altura}, 0.1f, 10, Fade(BLACK, 0.85f));
    DrawRectangleRoundedLines((Rectangle){x, y, largura, altura}, 0.1f, 10, personagem.corHabilidade);

    int posY = y + 25;
    int espacamento = 45;

    // Nome do personagem com fonte ainda maior
    DrawTextEx(fonte, personagem.nome, (Vector2){x + 25, posY}, 42, 2, WHITE);
    posY += espacamento + 20;

    // Habilidade com fonte maior e melhor destaque
    const char *habilidadeTexto = TextFormat("Habilidade: %s", personagem.habilidade);
    DrawTextEx(fonte, habilidadeTexto, (Vector2){x + 25, posY}, 32, 2, personagem.corHabilidade);
    posY += espacamento;

    // História com fonte maior
    const char *historiaTexto = "Historia:";
    DrawTextEx(fonte, historiaTexto, (Vector2){x + 25, posY}, 28, 2, LIGHTGRAY);
    posY += 35;

    // Texto da história com fonte maior e melhor formatação
    int larguraTexto = largura - 50;
    const char *historia = personagem.historia;
    int tamanhoFonte = 24; // Aumentado de 20 para 24

    char linha[250]; // Aumentado o buffer
    int posicao = 0;
    int caracteresPorLinha = (larguraTexto / (tamanhoFonte * 0.55f)); // Ajustado para fonte maior

    if (caracteresPorLinha <= 0)
        caracteresPorLinha = 25; // Aumentado

    while (historia[posicao] != '\0' && posY < y + altura - 35)
    {
        int fim = posicao + caracteresPorLinha;
        int tamanhoHistoria = strlen(historia);

        if (fim >= tamanhoHistoria)
        {
            fim = tamanhoHistoria;
        }
        else
        {
            // Procurar quebra de palavra mais inteligente
            while (fim > posicao && historia[fim] != ' ' && historia[fim] != '.' && historia[fim] != ',')
            {
                fim--;
            }
            // Se não encontrou espaço, quebra na palavra mesmo
            if (fim == posicao)
            {
                fim = posicao + caracteresPorLinha;
            }
        }

        int tamanho = fim - posicao;
        if (tamanho > 0 && tamanho < 249)
        {
            strncpy(linha, &historia[posicao], tamanho);
            linha[tamanho] = '\0';

            // Remover espaços no início da linha
            char *linhaLimpa = linha;
            while (*linhaLimpa == ' ')
                linhaLimpa++;

            DrawTextEx(fonte, linhaLimpa, (Vector2){x + 25, posY}, tamanhoFonte, 2, WHITE);
            posY += 30; // Aumentado o espaçamento entre linhas
            posicao = (fim < tamanhoHistoria && (historia[fim] == ' ' || historia[fim] == '.' || historia[fim] == ',')) ? fim + 1 : fim;
        }
        else
        {
            break;
        }
    }
}

void DesenharBarraVida(int x, int y, int largura, int altura, int vidaAtual, int vidaMaxima, Color cor, const char *nomeJogador)
{
    // Fundo da barra com gradiente
    DrawRectangleGradientV(x, y, largura, altura, Fade(DARKGRAY, 0.8f), DARKGRAY);

    // Barra de vida com gradiente baseado na porcentagem
    float porcentagem = (float)vidaAtual / (float)vidaMaxima;
    int larguraVida = (int)(largura * porcentagem);

    // Cor da vida baseada na porcentagem com gradiente
    Color corVida;
    if (porcentagem > 0.6f)
        corVida = ColorLerp(YELLOW, GREEN, (porcentagem - 0.6f) / 0.4f);
    else if (porcentagem > 0.3f)
        corVida = ColorLerp(RED, ORANGE, (porcentagem - 0.3f) / 0.3f);
    else
        corVida = ColorLerp((Color){128, 0, 0, 255}, RED, porcentagem / 0.3f); // Substituindo DARKRED por cor RGB

    // Desenhar barra com efeito de gradiente
    DrawRectangleGradientH(x, y, larguraVida, altura, corVida, Fade(corVida, 0.7f));

    // Efeito de brilho na barra
    if (porcentagem > 0.8f)
    {
        float brilho = 0.3f + 0.2f * sin(GetTime() * 6);
        DrawRectangleGradientH(x, y, larguraVida, altura / 3, Fade(WHITE, brilho), BLANK);
    }

    // Borda estilizada
    DrawRectangleLines(x - 1, y - 1, largura + 2, altura + 2, BLACK);
    DrawRectangleLines(x, y, largura, altura, WHITE);

    // Texto da vida centralizado e estilizado
    const char *textoVida = TextFormat("%d/%d", vidaAtual, vidaMaxima);
    int larguraTexto = MeasureText(textoVida, 16);

    // Sombra do texto
    DrawText(textoVida, x + largura / 2 - larguraTexto / 2 + 1, y + altura / 2 - 8 + 1, 16, BLACK);
    // Texto principal
    DrawText(textoVida, x + largura / 2 - larguraTexto / 2, y + altura / 2 - 8, 16, WHITE);

    // Nome do jogador acima da barra
    int larguraNome = MeasureText(nomeJogador, 20);
    DrawText(nomeJogador, x + largura / 2 - larguraNome / 2 + 1, y - 26, 20, BLACK); // Sombra
    DrawText(nomeJogador, x + largura / 2 - larguraNome / 2, y - 25, 20, cor);
}

void DesenharBarraPoder(int x, int y, int largura, int altura, int poderAtual, int poderMaximo, Color corPersonagem)
{
    // Fundo da barra
    DrawRectangleGradientV(x, y, largura, altura, Fade(DARKGRAY, 0.8f), DARKGRAY);

    // Barra de poder com efeito pulsante quando disponível
    float porcentagem = (float)poderAtual / (float)poderMaximo;
    int larguraPoder = (int)(largura * porcentagem);

    // Cor do poder com efeito especial
    Color corPoder;
    if (poderAtual >= 50)
    {
        // Efeito pulsante quando especial está disponível
        float pulso = 0.6f + 0.4f * sin(GetTime() * 10);
        corPoder = ColorLerp(BLUE, YELLOW, pulso);

        // Efeito de partículas visuais
        for (int i = 0; i < 3; i++)
        {
            float offset = i * 2.0f;
            float alpha = 0.3f * sin(GetTime() * 8 + offset);
            DrawRectangle(x + larguraPoder - 5, y - 2, 10, altura + 4, Fade(YELLOW, alpha));
        }
    }
    else
    {
        corPoder = ColorLerp(DARKBLUE, BLUE, porcentagem);
    }

    DrawRectangleGradientH(x, y, larguraPoder, altura, corPoder, Fade(corPoder, 0.6f));

    // Linha indicadora dos 50 pontos (metade da barra)
    int linhaMeio = x + largura / 2;
    DrawLine(linhaMeio, y, linhaMeio, y + altura, Fade(WHITE, 0.8f));
    DrawLine(linhaMeio - 1, y, linhaMeio - 1, y + altura, Fade(BLACK, 0.5f));

    // Borda estilizada
    DrawRectangleLines(x - 1, y - 1, largura + 2, altura + 2, BLACK);
    DrawRectangleLines(x, y, largura, altura, WHITE);

    // Texto do poder
    const char *textoPoder = TextFormat("%d/%d", poderAtual, poderMaximo);
    int larguraTexto = MeasureText(textoPoder, 14);

    // Sombra do texto
    DrawText(textoPoder, x + largura / 2 - larguraTexto / 2 + 1, y + altura / 2 - 7 + 1, 14, BLACK);
    DrawText(textoPoder, x + largura / 2 - larguraTexto / 2, y + altura / 2 - 7, 14, WHITE);

    // Indicador de poder especial disponível
    if (poderAtual >= 50)
    {
        const char *textoEspecial = "ESPECIAL PRONTO!";
        float alpha = 0.7f + 0.3f * sin(GetTime() * 6);
        Color corTexto = ColorLerp(YELLOW, WHITE, alpha);

        DrawText(textoEspecial, x + largura + 15 + 1, y + altura / 2 - 7 + 1, 14, BLACK); // Sombra
        DrawText(textoEspecial, x + largura + 15, y + altura / 2 - 7, 14, corTexto);
    }
}

void DesenharHUD(EstadoJogo *estado, Font fonte)
{
    // Verificar se os jogadores existem
    if (!estado->jogador1 || !estado->jogador2)
        return;

    // Painel superior para HUD
    DrawRectangleGradientV(0, 0, LARGURA_TELA, 180, Fade(BLACK, 0.7f), Fade(BLACK, 0.3f));
    DrawLine(0, 180, LARGURA_TELA, 180, Fade(WHITE, 0.5f));

    // === PLAYER 1 HUD (lado esquerdo) ===
    int margemEsquerda = 30;
    int larguraBarra = 350;
    int alturaBarra = 25;

    DesenharBarraVida(margemEsquerda, 45, larguraBarra, alturaBarra,
                      estado->jogador1->vidaAtual, estado->jogador1->vidaMaxima,
                      BLUE, estado->jogador1->nome);

    // Barra de poder do Player 1
    DesenharBarraPoder(margemEsquerda, 85, larguraBarra, 20,
                       estado->jogador1->poderAtual, estado->jogador1->poderMaximo,
                       estado->jogador1->corHabilidade);

    // Habilidade do Player 1
    const char *habilidadeP1 = TextFormat("Habilidade: %s", estado->jogador1->habilidade);
    DrawTextEx(fonte, habilidadeP1, (Vector2){margemEsquerda, 115}, 18, 2, estado->jogador1->corHabilidade);

    // === PLAYER 2 HUD (lado direito) ===
    int margemDireita = LARGURA_TELA - larguraBarra - 30;

    DesenharBarraVida(margemDireita, 45, larguraBarra, alturaBarra,
                      estado->jogador2->vidaAtual, estado->jogador2->vidaMaxima,
                      RED, estado->jogador2->nome);

    // Barra de poder do Player 2
    DesenharBarraPoder(margemDireita, 85, larguraBarra, 20,
                       estado->jogador2->poderAtual, estado->jogador2->poderMaximo,
                       estado->jogador2->corHabilidade);

    // Habilidade do Player 2
    const char *habilidadeP2 = TextFormat("Habilidade: %s", estado->jogador2->habilidade);
    Vector2 tamanhoHab2 = MeasureTextEx(fonte, habilidadeP2, 18, 2);
    DrawTextEx(fonte, habilidadeP2, (Vector2){margemDireita + larguraBarra - tamanhoHab2.x, 115}, 18, 2, estado->jogador2->corHabilidade);

    // === INFORMAÇÕES CENTRAIS ===
    // Round atual
    const char *textoRound = TextFormat("ROUND %d/%d", estado->roundAtual, MAX_ROUNDS);
    Vector2 tamanhoRound = MeasureTextEx(fonte, textoRound, 32, 2);
    DrawTextEx(fonte, textoRound, (Vector2){LARGURA_TELA / 2 - tamanhoRound.x / 2, 25}, 32, 2, YELLOW);

    // Timer com efeito de urgência
    Color corTempo = WHITE;
    if (estado->tempoRound <= 10.0f)
    {
        float intensidade = 0.5f + 0.5f * sin(GetTime() * 8);
        corTempo = ColorLerp(WHITE, RED, intensidade);
    }

    const char *textoTempo = TextFormat("%.0f", estado->tempoRound);
    Vector2 tamanhoTempo = MeasureTextEx(fonte, textoTempo, 48, 2);
    // Sombra do tempo
    DrawTextEx(fonte, textoTempo, (Vector2){LARGURA_TELA / 2 - tamanhoTempo.x / 2 + 2, 62}, 48, 2, BLACK);
    DrawTextEx(fonte, textoTempo, (Vector2){LARGURA_TELA / 2 - tamanhoTempo.x / 2, 60}, 48, 2, corTempo);

    // Indicador VS entre os jogadores
    const char *textoVS = "VS";
    Vector2 tamanhoVS = MeasureTextEx(fonte, textoVS, 24, 2);
    DrawTextEx(fonte, textoVS, (Vector2){LARGURA_TELA / 2 - tamanhoVS.x / 2, 135}, 24, 2, Fade(WHITE, 0.8f));
}

void DesenharTelaSelecaoMapa(Mapa mapas[], int mapaAtual, Font fonte)
{

    DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(BLACK, 0.8f));

    const char *titulo = "ESCOLHA O MAPA DE LUTA";
    Vector2 tamanhoTitulo = MeasureTextEx(fonte, titulo, 52, 2);
    DrawTextEx(fonte, titulo, (Vector2){LARGURA_TELA / 2 - tamanhoTitulo.x / 2, 100}, 52, 2, WHITE);

    int larguraMapa = 800;
    int alturaMapa = 450;
    int xMapa = (LARGURA_TELA - larguraMapa) / 2;
    int yMapa = 200;

    DrawTexturePro(mapas[mapaAtual].textura,
                   (Rectangle){0, 0, mapas[mapaAtual].textura.width, mapas[mapaAtual].textura.height},
                   (Rectangle){xMapa, yMapa, larguraMapa, alturaMapa},
                   (Vector2){0, 0}, 0.0f, WHITE);

    DrawRectangleLines(xMapa - 5, yMapa - 5, larguraMapa + 10, alturaMapa + 10, YELLOW);

    const char *nomeMapa = mapas[mapaAtual].nome;
    Vector2 tamanhoNome = MeasureTextEx(fonte, nomeMapa, 40, 2);
    DrawTextEx(fonte, nomeMapa, (Vector2){LARGURA_TELA / 2 - tamanhoNome.x / 2, yMapa + alturaMapa + 30}, 40, 2, YELLOW);

    const char *descricao = mapas[mapaAtual].descricao;
    Vector2 tamanhoDesc = MeasureTextEx(fonte, descricao, 28, 2);
    DrawTextEx(fonte, descricao, (Vector2){LARGURA_TELA / 2 - tamanhoDesc.x / 2, yMapa + alturaMapa + 80}, 28, 2, LIGHTGRAY);

    DrawTextEx(fonte, "← → para navegar mapas", (Vector2){50, ALTURA_TELA - 100}, 28, 2, WHITE);
    DrawTextEx(fonte, "ENTER para confirmar", (Vector2){50, ALTURA_TELA - 70}, 28, 2, GREEN);
    DrawTextEx(fonte, "BACKSPACE para voltar", (Vector2){50, ALTURA_TELA - 40}, 28, 2, GRAY);
}

void InicializarPersonagem(Personagem *p, const char *nomePersonagem)
{
    p->vidaMaxima = VIDA_MAXIMA;
    p->vidaAtual = VIDA_MAXIMA;
    p->poderMaximo = PODER_MAXIMO;
    p->poderAtual = PODER_MAXIMO; // Começar com poder no máximo
    p->danoSoco = 20;
    p->danoChute = 30;
    p->danoPoder = 50;

    p->poseAtual = POSE_IDLE;
    p->timerAnimacao = 0.0f;
    p->duracaoAnimacao = 0.0f;
    p->animando = false;

    // Inicializar sistema de movimento e sprites
    p->velocidade = (Vector2){0, 0};
    p->velocidadeMaxima = 200.0f;
    p->viradoParaDireita = true;
    p->podeAtacar = true;
    p->podeSeMovimentar = true;

    // Inicializar sprites
    p->frameAtual = 0;
    p->totalFrames = 4;
    p->timerFrame = 0.0f;
    p->duracaoFrame = 0.12f;
    p->frameSource = (Rectangle){0, 0, 64, 64};

    p->texturaDefesa = p->texturaLuta;
    p->texturaDano = p->texturaLuta;
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

        p->posicao.x += p->velocidade.x * deltaTime;
        p->posicao.y += p->velocidade.y * deltaTime;

        p->velocidade.y += 300.0f * deltaTime;

        p->vida -= deltaTime * 2.0f;

        float alpha = p->vida / p->vidaMaxima;
        p->cor.a = (unsigned char)(255 * alpha);
    }

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

void InicializarConfiguracaoPadrao(ConfiguracaoJogo *config)
{

    config->teclaPoderP1 = KEY_Q;
    config->teclaSocoP1 = KEY_E;
    config->teclaChute1 = KEY_R;
    config->teclaEsquerdaP1 = KEY_A;
    config->teclaDireitaP1 = KEY_D;
    config->teclaConfirmarP1 = KEY_ENTER;

    config->teclaPoderP2 = KEY_P;
    config->teclaSocoP2 = KEY_O;
    config->teclaChute2 = KEY_I;
    config->teclaEsquerdaP2 = KEY_LEFT;
    config->teclaDireitaP2 = KEY_RIGHT;
    config->teclaConfirmarP2 = KEY_SPACE;

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

    DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(BLACK, 0.9f));

    const char *titulo = "OPÇÕES";
    Vector2 tamanhoTitulo = MeasureTextEx(fonte, titulo, 64, 2);
    DrawTextEx(fonte, titulo, (Vector2){LARGURA_TELA / 2 - tamanhoTitulo.x / 2, 100}, 64, 2, WHITE);

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

        int larguraCaixa = 800;
        int alturaCaixa = 60;
        int x = (LARGURA_TELA - larguraCaixa) / 2;
        int y = posY + i * espacamento - 10;

        DrawRectangleRounded((Rectangle){x, y, larguraCaixa, alturaCaixa}, 0.1f, 10, corCaixa);
        DrawRectangleRoundedLines((Rectangle){x, y, larguraCaixa, alturaCaixa}, 0.1f, 10, corTexto);

        DrawTextEx(fonte, opcoes[i], (Vector2){x + 20, posY + i * espacamento}, 36, 2, corTexto);

        if (i == 0)
        {
            const char *controles = TextFormat("Poder:%s Soco:%s Chute:%s",
                                               ObterNomeTecla(config->teclaPoderP1),
                                               ObterNomeTecla(config->teclaSocoP1),
                                               ObterNomeTecla(config->teclaChute1));
            DrawTextEx(fonte, controles, (Vector2){x + 20, posY + i * espacamento + 35}, 24, 2, LIGHTGRAY);
        }
        else if (i == 1)
        {
            const char *controles = TextFormat("Poder:%s Soco:%s Chute:%s",
                                               ObterNomeTecla(config->teclaPoderP2),
                                               ObterNomeTecla(config->teclaSocoP2),
                                               ObterNomeTecla(config->teclaChute2));
            DrawTextEx(fonte, controles, (Vector2){x + 20, posY + i * espacamento + 35}, 24, 2, LIGHTGRAY);
        }
        else if (i == 2)
        {
            int barraLargura = 200;
            int barraX = x + 400;
            int barraY = posY + i * espacamento + 15;

            DrawRectangle(barraX, barraY, barraLargura, 20, DARKGRAY);
            DrawRectangle(barraX, barraY, (int)(barraLargura * config->volumeMusica), 20, GREEN);
            DrawRectangleLines(barraX, barraY, barraLargura, 20, WHITE);

            const char *volumeTexto = TextFormat("%.0f%%", config->volumeMusica * 100);
            DrawTextEx(fonte, volumeTexto, (Vector2){barraX + barraLargura + 10, barraY}, 24, 2, WHITE);
        }
        else if (i == 3)
        {
            int barraLargura = 200;
            int barraX = x + 400;
            int barraY = posY + i * espacamento + 15;

            DrawRectangle(barraX, barraY, barraLargura, 20, DARKGRAY);
            DrawRectangle(barraX, barraY, (int)(barraLargura * config->volumeEfeitos), 20, BLUE);
            DrawRectangleLines(barraX, barraY, barraLargura, 20, WHITE);

            const char *volumeTexto = TextFormat("%.0f%%", config->volumeEfeitos * 100);
            DrawTextEx(fonte, volumeTexto, (Vector2){barraX + barraLargura + 10, barraY}, 24, 2, WHITE);
        }
    }

    if (aguardandoTecla)
    {
        const char *instrucao = "PRESSIONE UMA TECLA PARA CONFIGURAR...";
        Vector2 tamanhoInstrucao = MeasureTextEx(fonte, instrucao, 28, 2);
        DrawTextEx(fonte, instrucao, (Vector2){LARGURA_TELA / 2 - tamanhoInstrucao.x / 2, ALTURA_TELA - 100}, 28, 2, YELLOW);
    }
    else
    {
        const char *instrucoes = "↑/↓: Navegar | ENTER: Selecionar | ←/→: Ajustar Volume | BACKSPACE: Voltar";
        Vector2 tamanhoInstrucoes = MeasureTextEx(fonte, instrucoes, 24, 2);
        DrawTextEx(fonte, instrucoes, (Vector2){LARGURA_TELA / 2 - tamanhoInstrucoes.x / 2, ALTURA_TELA - 80}, 24, 2, LIGHTGRAY);
    }
}

int main(void)
{
    InitWindow(LARGURA_TELA, ALTURA_TELA, "UNI FIGHT - Jogo de Luta");
    SetTargetFPS(60);
    InitAudioDevice();

    Font fontePixel = LoadFont("fontes/Pixelify_Sans/PixelifySans-VariableFont_wght.ttf");
    Texture2D fundoMenu = LoadTexture("./fundoMenu2.jpg");
    Texture2D fundoSelecao = LoadTexture("./fundo.png");
    Texture2D logo = LoadTexture("./logoUnifight3.png");

    Music musicaFundo = LoadMusicStream("musicas/menu_background.mp3");
    PlayMusicStream(musicaFundo);

    ConfiguracaoJogo config = {0};
    InicializarConfiguracaoPadrao(&config);

    int opcaoSelecionada = 0;
    bool aguardandoTecla = false;
    int teclaParaConfigurar = -1;

    OpcaoMenu menu[OPCOES_MENU] = {
        {"JOGAR", 600},
        {"OPCOES", 700},
        {"SAIR", 800}};
    int menuSelecionado = 0;

    Personagem personagens[MAX_PERSONAGENS] = {
        {.textura = LoadTexture("./personagens/foto de perfil, selecao de personagem ryu/ryu foto de perfil.png"),
         .nome = "Ryu",
         .habilidade = "Hadoken",
         .historia = "Artista marcial errante.",
         .corHabilidade = BLUE,
         .texturaLuta = LoadTexture("./personagens/ryu com pose parada/0.png"),
         .texturaWalk = LoadTexture("./personagens/ryu com pose parada/0.png"),
         .texturaSoco = LoadTexture("./personagens/ryu com pose parada/0.png"),
         .texturaChute = LoadTexture("./personagens/ryu com pose parada/0.png"),
         .texturaPoder = LoadTexture("./personagens/ryu com pose parada/0.png"),
         .texturaDefesa = LoadTexture("./personagens/ryu com pose parada/0.png"),
         .texturaDano = LoadTexture("./personagens/ryu com pose parada/0.png")},
        {.textura = LoadTexture("./personagens/foto de perfil do ken/0.png"),
         .nome = "Ken Masters",
         .habilidade = "Shoryuken",
         .historia = "Rival amigável de Ryu e campeão dos EUA.",
         .corHabilidade = RED,
         .texturaLuta = LoadTexture("./personagens/foto de perfil do ken/0.png"),   // Placeholder
         .texturaWalk = LoadTexture("./personagens/foto de perfil do ken/0.png"),   // Placeholder
         .texturaSoco = LoadTexture("./personagens/foto de perfil do ken/0.png"),   // Placeholder
         .texturaChute = LoadTexture("./personagens/foto de perfil do ken/0.png"),  // Placeholder
         .texturaPoder = LoadTexture("./personagens/foto de perfil do ken/0.png"),  // Placeholder
         .texturaDefesa = LoadTexture("./personagens/foto de perfil do ken/0.png"), // Placeholder
         .texturaDano = LoadTexture("./personagens/foto de perfil do ken/0.png")},  // Placeholder
        {.textura = LoadTexture("./personagens/guile, foto de perfil/0.png"),
         .nome = "Guile",
         .habilidade = "Sonic Boom",
         .historia = "Major da Força Aérea dos EUA buscando vingança.",
         .corHabilidade = GREEN,
         .texturaLuta = LoadTexture("./personagens/guile parado na hora de combate/0.png"),
         .texturaWalk = LoadTexture("./personagens/guile parado na hora de combate/0.png"),
         .texturaSoco = LoadTexture("./personagens/guile parado na hora de combate/0.png"),
         .texturaChute = LoadTexture("./personagens/guile parado na hora de combate/0.png"),
         .texturaPoder = LoadTexture("./personagens/guile parado na hora de combate/0.png"),
         .texturaDefesa = LoadTexture("./personagens/guile parado na hora de combate/0.png"),
         .texturaDano = LoadTexture("./personagens/guile parado na hora de combate/0.png")},
        {.textura = LoadTexture("./personagens/sagat foto de perfil/0.png"),
         .nome = "Sagat",
         .habilidade = "Tiger Shot",
         .historia = "O imperador do Muay Thai.",
         .corHabilidade = ORANGE,
         .texturaLuta = LoadTexture("./personagens/sagat parado em pose de soco/0.png"),
         .texturaWalk = LoadTexture("./personagens/sagat parado em pose de soco/0.png"),
         .texturaSoco = LoadTexture("./personagens/sagat parado em pose de soco/0.png"),
         .texturaChute = LoadTexture("./personagens/sagat parado em pose de soco/0.png"),
         .texturaPoder = LoadTexture("./personagens/sagat parado em pose de soco/0.png"),
         .texturaDefesa = LoadTexture("./personagens/sagat parado em pose de soco/0.png"),
         .texturaDano = LoadTexture("./personagens/sagat parado em pose de soco/0.png")},
        {.textura = LoadTexture("./personagens/zangief foto de perfil/0.png"),
         .nome = "Zangief",
         .habilidade = "Spinning Piledriver",
         .historia = "O Ciclone Vermelho da Rússia.",
         .corHabilidade = MAROON,
         .texturaLuta = LoadTexture("./personagens/zangief parado na hora da luta/0.png"),
         .texturaWalk = LoadTexture("./personagens/zangief parado na hora da luta/0.png"),
         .texturaSoco = LoadTexture("./personagens/zangief parado na hora da luta/0.png"),
         .texturaChute = LoadTexture("./personagens/zangief parado na hora da luta/0.png"),
         .texturaPoder = LoadTexture("./personagens/zangief parado na hora da luta/0.png"),
         .texturaDefesa = LoadTexture("./personagens/zangief parado na hora da luta/0.png"),
         .texturaDano = LoadTexture("./personagens/zangief parado na hora da luta/0.png")},
        {.textura = LoadTexture("./personagens/m_bison foto de perfil/0.png"),
         .nome = "M. Bison",
         .habilidade = "Psycho Crusher",
         .historia = "Líder da Shadaloo e usuário do Psycho Power.",
         .corHabilidade = PURPLE,
         .texturaLuta = LoadTexture("./personagens/m_bison foto de perfil/0.png"),   // Placeholder
         .texturaWalk = LoadTexture("./personagens/m_bison foto de perfil/0.png"),   // Placeholder
         .texturaSoco = LoadTexture("./personagens/m_bison foto de perfil/0.png"),   // Placeholder
         .texturaChute = LoadTexture("./personagens/m_bison foto de perfil/0.png"),  // Placeholder
         .texturaPoder = LoadTexture("./personagens/m_bison foto de perfil/0.png"),  // Placeholder
         .texturaDefesa = LoadTexture("./personagens/m_bison foto de perfil/0.png"), // Placeholder
         .texturaDano = LoadTexture("./personagens/m_bison foto de perfil/0.png")}   // Placeholder
    };

    for (int i = 0; i < MAX_PERSONAGENS; i++)
    {
        InicializarPersonagem(&personagens[i], personagens[i].nome);
    }

    Mapa mapas[MAX_MAPAS] = {
        {LoadTexture("./frames/frame-1.png"), "Arena Vulcânica", "Arena cercada por lava e rochas ígneas. Ideal para lutadores de fogo."},
        {LoadTexture("./frames/frame-2.png"), "Caverna Cristalina", "Caverna mística com cristais de gelo brilhantes. Favorece usuários de gelo."},
        {LoadTexture("./frames/frame-3.png"), "Templo Gelado", "Antigo templo nas montanhas geladas. Ambiente sagrado e equilibrado."},
        {LoadTexture("./frames/frame-4.png"), "Floresta Ancestral", "Floresta antiga com ruínas místicas. Energia natural balanceada."},
        {LoadTexture("./frames/frame-5.png"), "Coliseu Neutro", "Arena equilibrada sem vantagens elementais para todos os lutadores."},
        {LoadTexture("./frames/frame-6.png"), "Santuário Perdido", "Ruínas de um antigo santuário com poderes místicos adormecidos."},
        {LoadTexture("./recursos_visuais_mapas/frame-7.png"), "Ponte Desafiadora", "Uma ponte estreita sobre um abismo."},
        {LoadTexture("./recursos_visuais_mapas/frame-8.png"), "Cidade Neon", "Lute sob as luzes brilhantes da cidade."}};

    int selecionadoJogador1 = 0;
    int selecionadoJogador2 = MAX_PERSONAGENS - 1;
    int mapaAtual = 0;

    EstadoJogo estado = {0};
    estado.roundAtual = 1;
    estado.tempoRound = TEMPO_ROUND;
    estado.jogador1Confirmado = false;
    estado.jogador2Confirmado = false;
    estado.mapaConfirmado = false;

    // Configurar limites da arena
    estado.limiteEsquerdo = 100.0f;
    estado.limiteDireito = LARGURA_TELA - 100.0f;
    estado.chao = ALTURA_TELA - 100.0f;

    Tela telaAtual = TELA_MENU;
    int tamanhoFonte = 44;

    while (!WindowShouldClose())
    {

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

                if (IsKeyPressed(KEY_DOWN))
                    opcaoSelecionada = (opcaoSelecionada + 1) % OPCOES_CONFIG;
                if (IsKeyPressed(KEY_UP))
                    opcaoSelecionada = (opcaoSelecionada - 1 + OPCOES_CONFIG) % OPCOES_CONFIG;

                if (opcaoSelecionada == 2)
                {
                    if (IsKeyPressed(KEY_LEFT))
                        config.volumeMusica = fmaxf(0.0f, config.volumeMusica - 0.1f);
                    if (IsKeyPressed(KEY_RIGHT))
                        config.volumeMusica = fminf(1.0f, config.volumeMusica + 0.1f);
                }
                else if (opcaoSelecionada == 3)
                {
                    if (IsKeyPressed(KEY_LEFT))
                        config.volumeEfeitos = fmaxf(0.0f, config.volumeEfeitos - 0.1f);
                    if (IsKeyPressed(KEY_RIGHT))
                        config.volumeEfeitos = fminf(1.0f, config.volumeEfeitos + 0.1f);
                }

                if (IsKeyPressed(KEY_ENTER))
                {
                    if (opcaoSelecionada == 0)
                    {
                        aguardandoTecla = true;
                        teclaParaConfigurar = 0;
                    }
                    else if (opcaoSelecionada == 1)
                    {
                        aguardandoTecla = true;
                        teclaParaConfigurar = 3;
                    }
                    else if (opcaoSelecionada == 4)
                    {
                        InicializarConfiguracaoPadrao(&config);
                    }
                    else if (opcaoSelecionada == 7)
                    {
                        telaAtual = TELA_MENU;
                    }
                }

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    telaAtual = TELA_MENU;
                }
            }
            else
            {

                int tecla = GetKeyPressed();
                if (tecla != 0)
                {

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

                    if ((teclaParaConfigurar == 3 && opcaoSelecionada == 0) ||
                        (teclaParaConfigurar == 6 && opcaoSelecionada == 1))
                    {
                        aguardandoTecla = false;
                        teclaParaConfigurar = -1;
                    }
                }

                if (IsKeyPressed(KEY_ESCAPE))
                {
                    aguardandoTecla = false;
                    teclaParaConfigurar = -1;
                }
            }
        }
        else if (telaAtual == TELA_SELECAO)
        {

            if (IsKeyPressed(KEY_D))
                selecionadoJogador1 = (selecionadoJogador1 + 1) % MAX_PERSONAGENS;
            if (IsKeyPressed(KEY_A))
                selecionadoJogador1 = (selecionadoJogador1 - 1 + MAX_PERSONAGENS) % MAX_PERSONAGENS;

            if (IsKeyPressed(KEY_RIGHT))
                selecionadoJogador2 = (selecionadoJogador2 + 1) % MAX_PERSONAGENS;
            if (IsKeyPressed(KEY_LEFT))
                selecionadoJogador2 = (selecionadoJogador2 - 1 + MAX_PERSONAGENS) % MAX_PERSONAGENS;

            if (IsKeyPressed(KEY_UP))
                mapaAtual = (mapaAtual - 1 + MAX_MAPAS) % MAX_MAPAS;
            if (IsKeyPressed(KEY_DOWN))
                mapaAtual = (mapaAtual + 1) % MAX_MAPAS;

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

            if (estado.jogador1Confirmado && estado.jogador2Confirmado)
            {
                telaAtual = TELA_SELECAO_MAPA;
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                telaAtual = TELA_MENU;
                estado.jogador1Confirmado = false;
                estado.jogador2Confirmado = false;
            }
        }
        else if (telaAtual == TELA_SELECAO_MAPA)
        {

            if (IsKeyPressed(KEY_LEFT))
                mapaAtual = (mapaAtual - 1 + MAX_MAPAS) % MAX_MAPAS;
            if (IsKeyPressed(KEY_RIGHT))
                mapaAtual = (mapaAtual + 1) % MAX_MAPAS;

            if (IsKeyPressed(KEY_ENTER))
            {
                estado.mapaAtual = &mapas[mapaAtual];
                estado.mapaConfirmado = true;

                // Inicializar posições dos personagens na arena
                InicializarPosicaoPersonagem(estado.jogador1, 300.0f, estado.chao, true);
                InicializarPosicaoPersonagem(estado.jogador2, LARGURA_TELA - 300.0f, estado.chao, false);

                // Fazer os personagens se virarem um para o outro
                VirarPersonagemParaOponente(estado.jogador1, estado.jogador2);

                telaAtual = TELA_LUTA;
                printf("Mapa selecionado: %s\n", mapas[mapaAtual].nome);
                printf("Posições inicializadas - P1: (%.0f, %.0f) P2: (%.0f, %.0f)\n",
                       estado.jogador1->posicao.x, estado.jogador1->posicao.y,
                       estado.jogador2->posicao.x, estado.jogador2->posicao.y);
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                telaAtual = TELA_SELECAO;
                estado.jogador1Confirmado = false;
                estado.jogador2Confirmado = false;
            }
        }
        else if (telaAtual == TELA_LUTA)
        {
            float deltaTime = GetFrameTime();

            estado.tempoRound -= deltaTime;
            if (estado.tempoRound <= 0)
            {
                estado.tempoRound = TEMPO_ROUND;
                estado.roundAtual++;
                if (estado.roundAtual > MAX_ROUNDS)
                {

                    telaAtual = TELA_MENU;
                    estado.roundAtual = 1;
                    estado.jogador1Confirmado = false;
                    estado.jogador2Confirmado = false;
                    estado.mapaConfirmado = false;
                }
            }

            // Processar movimento dos personagens
            ProcessarMovimentoPlayer1(estado.jogador1, &config);
            ProcessarMovimentoPlayer2(estado.jogador2, &config);

            // Mover personagens
            MoverPersonagem(estado.jogador1, deltaTime, estado.limiteEsquerdo, estado.limiteDireito);
            MoverPersonagem(estado.jogador2, deltaTime, estado.limiteEsquerdo, estado.limiteDireito);

            // Fazer personagens se virarem um para o outro automaticamente
            VirarPersonagemParaOponente(estado.jogador1, estado.jogador2);

            // Atualizar animações
            AtualizarAnimacao(estado.jogador1, deltaTime);
            AtualizarAnimacao(estado.jogador2, deltaTime);
            AtualizarAnimacaoSprite(estado.jogador1, deltaTime);
            AtualizarAnimacaoSprite(estado.jogador2, deltaTime);

            AtualizarParticulas(&estado.particulasImpacto, deltaTime);
            if (estado.flashTela > 0)
                estado.flashTela -= deltaTime * 3.0f;
            if (estado.tremor > 0)
                estado.tremor -= deltaTime * 5.0f;

            // Ataques Player 1
            if (IsKeyPressed(config.teclaPoderP1) && estado.jogador1->poderAtual >= 50 && estado.jogador1->podeAtacar)
            {
                if (ExecutarAtaque(estado.jogador1, estado.jogador2, POSE_PODER, estado.jogador1->danoPoder))
                {
                    estado.jogador1->poderAtual = 0;
                    Vector2 posImpacto = {estado.jogador2->posicao.x, estado.jogador2->posicao.y - 50};
                    CriarEfeitoImpacto(&estado.particulasImpacto, posImpacto, estado.jogador1->corHabilidade, 30);
                    estado.flashTela = 0.4f;
                    estado.tremor = 0.3f;
                }
            }
            if (IsKeyPressed(config.teclaSocoP1) && estado.jogador1->podeAtacar)
            {
                if (ExecutarAtaque(estado.jogador1, estado.jogador2, POSE_SOCO, estado.jogador1->danoSoco))
                {
                    estado.jogador1->poderAtual += 10;
                    if (estado.jogador1->poderAtual > PODER_MAXIMO)
                        estado.jogador1->poderAtual = PODER_MAXIMO;
                    Vector2 posImpacto = {estado.jogador2->posicao.x, estado.jogador2->posicao.y - 50};
                    CriarEfeitoImpacto(&estado.particulasImpacto, posImpacto, YELLOW, 15);
                    estado.tremor = 0.1f;
                }
            }
            if (IsKeyPressed(config.teclaChute1) && estado.jogador1->podeAtacar)
            {
                if (ExecutarAtaque(estado.jogador1, estado.jogador2, POSE_CHUTE, estado.jogador1->danoChute))
                {
                    estado.jogador1->poderAtual += 15;
                    if (estado.jogador1->poderAtual > PODER_MAXIMO)
                        estado.jogador1->poderAtual = PODER_MAXIMO;
                    Vector2 posImpacto = {estado.jogador2->posicao.x, estado.jogador2->posicao.y - 50};
                    CriarEfeitoImpacto(&estado.particulasImpacto, posImpacto, ORANGE, 20);
                    estado.tremor = 0.15f;
                }
            }

            // Ataques Player 2
            if (IsKeyPressed(config.teclaPoderP2) && estado.jogador2->poderAtual >= 50 && estado.jogador2->podeAtacar)
            {
                if (ExecutarAtaque(estado.jogador2, estado.jogador1, POSE_PODER, estado.jogador2->danoPoder))
                {
                    estado.jogador2->poderAtual = 0;
                    Vector2 posImpacto = {estado.jogador1->posicao.x, estado.jogador1->posicao.y - 50};
                    CriarEfeitoImpacto(&estado.particulasImpacto, posImpacto, estado.jogador2->corHabilidade, 30);
                    estado.flashTela = 0.4f;
                    estado.tremor = 0.3f;
                }
            }
            if (IsKeyPressed(config.teclaSocoP2) && estado.jogador2->podeAtacar)
            {
                if (ExecutarAtaque(estado.jogador2, estado.jogador1, POSE_SOCO, estado.jogador2->danoSoco))
                {
                    estado.jogador2->poderAtual += 10;
                    if (estado.jogador2->poderAtual > PODER_MAXIMO)
                        estado.jogador2->poderAtual = PODER_MAXIMO;
                    Vector2 posImpacto = {estado.jogador1->posicao.x, estado.jogador1->posicao.y - 50};
                    CriarEfeitoImpacto(&estado.particulasImpacto, posImpacto, YELLOW, 15);
                    estado.tremor = 0.1f;
                }
            }
            if (IsKeyPressed(config.teclaChute2) && estado.jogador2->podeAtacar)
            {
                if (ExecutarAtaque(estado.jogador2, estado.jogador1, POSE_CHUTE, estado.jogador2->danoChute))
                {
                    estado.jogador2->poderAtual += 15;
                    if (estado.jogador2->poderAtual > PODER_MAXIMO)
                        estado.jogador2->poderAtual = PODER_MAXIMO;
                    Vector2 posImpacto = {estado.jogador1->posicao.x, estado.jogador1->posicao.y - 50};
                    CriarEfeitoImpacto(&estado.particulasImpacto, posImpacto, ORANGE, 20);
                    estado.tremor = 0.15f;
                }
            }

            // Verificar fim de jogo
            if (estado.jogador1->vidaAtual <= 0 || estado.jogador2->vidaAtual <= 0)
            {
                // Mostrar vencedor por 2 segundos antes de voltar ao menu
                const char *vencedor = estado.jogador1->vidaAtual <= 0 ? estado.jogador2->nome : estado.jogador1->nome;
                Color corVencedor = estado.jogador1->vidaAtual <= 0 ? RED : BLUE;

                DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(BLACK, 0.7f));

                const char *textoVitoria = TextFormat("%s VENCEU!", vencedor);
                Vector2 tamanhoTexto = MeasureTextEx(fontePixel, textoVitoria, 72, 2);
                DrawTextEx(fontePixel, textoVitoria, (Vector2){LARGURA_TELA / 2 - tamanhoTexto.x / 2, ALTURA_TELA / 2 - 50}, 72, 2, corVencedor);

                DrawTextEx(fontePixel, "Voltando ao menu em instantes...", (Vector2){LARGURA_TELA / 2 - 200, ALTURA_TELA / 2 + 50}, 28, 2, WHITE);

                // Delay antes de voltar ao menu
                static float timerVitoria = 0;
                timerVitoria += GetFrameTime();

                if (timerVitoria > 3.0f || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
                {
                    timerVitoria = 0;
                    telaAtual = TELA_MENU;
                    estado.roundAtual = 1;
                    estado.tempoRound = TEMPO_ROUND;
                    estado.jogador1Confirmado = false;
                    estado.jogador2Confirmado = false;
                    estado.mapaConfirmado = false;

                    // Resetar todos os personagens
                    for (int i = 0; i < MAX_PERSONAGENS; i++)
                    {
                        InicializarPersonagem(&personagens[i], personagens[i].nome);
                    }
                }
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                telaAtual = TELA_MENU;
                estado.roundAtual = 1;
                estado.tempoRound = TEMPO_ROUND;
                estado.jogador1Confirmado = false;
                estado.jogador2Confirmado = false;
                estado.mapaConfirmado = false;

                // Resetar personagens ao sair da luta
                for (int i = 0; i < MAX_PERSONAGENS; i++)
                {
                    InicializarPersonagem(&personagens[i], personagens[i].nome);
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (telaAtual == TELA_MENU)
        {

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

                Vector2 tamTexto = MeasureTextEx(fontePixel, texto, tamanhoFonte, 2);
                DrawTextEx(fontePixel, texto, (Vector2){(LARGURA_TELA - tamTexto.x) / 2, menu[i].posY}, tamanhoFonte, 2, corTexto);
            }
        }
        else if (telaAtual == TELA_OPCOES)
        {
            DesenharTelaOpcoes(&config, opcaoSelecionada, aguardandoTecla, fontePixel);
        }
        else if (telaAtual == TELA_SELECAO)
        {

            DrawTexturePro(fundoSelecao,
                           (Rectangle){0, 0, (float)fundoSelecao.width, (float)fundoSelecao.height},
                           (Rectangle){0, 0, LARGURA_TELA, ALTURA_TELA},
                           (Vector2){0, 0}, 0.0f, WHITE);

            DrawRectangleGradientV(0, 0, LARGURA_TELA, ALTURA_TELA,
                                   Fade(BLACK, 0.3f), Fade(BLACK, 0.6f));

            int metadeLargura = LARGURA_TELA / 2;
            int alturaPersonagens = 500;

            DrawRectangleRounded((Rectangle){20, 130, metadeLargura - 40, alturaPersonagens + 40}, 0.1f, 10, Fade(BLACK, 0.7f));
            DrawRectangleRounded((Rectangle){metadeLargura + 20, 130, metadeLargura - 40, alturaPersonagens + 40}, 0.1f, 10, Fade(BLACK, 0.7f));

            DrawRectangleRoundedLines((Rectangle){20, 130, metadeLargura - 40, alturaPersonagens + 40}, 0.1f, 10, estado.jogador1Confirmado ? GREEN : BLUE);
            DrawRectangleRoundedLines((Rectangle){metadeLargura + 20, 130, metadeLargura - 40, alturaPersonagens + 40}, 0.1f, 10, estado.jogador2Confirmado ? GREEN : RED);

            DesenharSelecaoPersonagem(personagens, selecionadoJogador1, 0, 150, metadeLargura, alturaPersonagens, estado.jogador1Confirmado ? GREEN : BLUE);
            DesenharSelecaoPersonagem(personagens, selecionadoJogador2, metadeLargura, 150, metadeLargura, alturaPersonagens, estado.jogador2Confirmado ? GREEN : RED);

            DrawRectangleRounded((Rectangle){20, 20, LARGURA_TELA - 40, 80}, 0.2f, 10, Fade(BLACK, 0.8f));
            DrawRectangleRoundedLines((Rectangle){20, 20, LARGURA_TELA - 40, 80}, 0.2f, 10, WHITE);

            const char *titulo = "SELECIONE SEU PERSONAGEM";
            Vector2 tamanhoTitulo = MeasureTextEx(fontePixel, titulo, 54, 2);
            DrawTextEx(fontePixel, titulo, (Vector2){LARGURA_TELA / 2 - tamanhoTitulo.x / 2, 40}, 54, 2, WHITE);

            // Status de confirmação com fontes melhoradas e estilização
            // Player 1 - Painel estilizado
            int larguraPainel = 350;
            int alturaPainel = 45;
            int xPlayer1 = 50;
            int yPlayer1 = 105;

            Color corFundoP1 = estado.jogador1Confirmado ? Fade(GREEN, 0.3f) : Fade(BLUE, 0.3f);
            Color corBordaP1 = estado.jogador1Confirmado ? GREEN : BLUE;
            Color corTextoP1 = estado.jogador1Confirmado ? GREEN : BLUE;

            DrawRectangleRounded((Rectangle){xPlayer1, yPlayer1, larguraPainel, alturaPainel}, 0.2f, 10, corFundoP1);
            DrawRectangleRoundedLines((Rectangle){xPlayer1, yPlayer1, larguraPainel, alturaPainel}, 0.2f, 10, corBordaP1);

            if (estado.jogador1Confirmado)
            {
                DrawTextEx(fontePixel, "✓ PLAYER 1 CONFIRMADO!", (Vector2){xPlayer1 + 15, yPlayer1 + 8}, 32, 2, corTextoP1);
            }
            else
            {
                DrawTextEx(fontePixel, "PLAYER 1 (A / D)", (Vector2){xPlayer1 + 15, yPlayer1 + 8}, 32, 2, corTextoP1);
            }

            // Player 2 - Painel estilizado
            int xPlayer2 = metadeLargura + 50;
            int yPlayer2 = 105;

            Color corFundoP2 = estado.jogador2Confirmado ? Fade(GREEN, 0.3f) : Fade(RED, 0.3f);
            Color corBordaP2 = estado.jogador2Confirmado ? GREEN : RED;
            Color corTextoP2 = estado.jogador2Confirmado ? GREEN : RED;

            DrawRectangleRounded((Rectangle){xPlayer2, yPlayer2, larguraPainel, alturaPainel}, 0.2f, 10, corFundoP2);
            DrawRectangleRoundedLines((Rectangle){xPlayer2, yPlayer2, larguraPainel, alturaPainel}, 0.2f, 10, corBordaP2);

            if (estado.jogador2Confirmado)
            {
                DrawTextEx(fontePixel, "✓ PLAYER 2 CONFIRMADO!", (Vector2){xPlayer2 + 15, yPlayer2 + 8}, 32, 2, corTextoP2);
            }
            else
            {
                DrawTextEx(fontePixel, "PLAYER 2 (← / →)", (Vector2){xPlayer2 + 15, yPlayer2 + 8}, 32, 2, corTextoP2);
            }

            int posYInfo = 680;
            DrawRectangleRounded((Rectangle){30, posYInfo, LARGURA_TELA - 60, 300}, 0.1f, 10, Fade(BLACK, 0.8f));
            DrawRectangleRoundedLines((Rectangle){30, posYInfo, LARGURA_TELA - 60, 300}, 0.1f, 10, WHITE);

            DrawLine(LARGURA_TELA / 2, posYInfo + 20, LARGURA_TELA / 2, posYInfo + 280, WHITE);

            DesenharInfoPersonagem(personagens[selecionadoJogador1], 50, posYInfo + 20, metadeLargura - 80, 260, fontePixel);
            DesenharInfoPersonagem(personagens[selecionadoJogador2], metadeLargura + 30, posYInfo + 20, metadeLargura - 80, 260, fontePixel);

            DrawRectangleRounded((Rectangle){30, ALTURA_TELA - 80, LARGURA_TELA - 60, 50}, 0.1f, 10, Fade(BLACK, 0.9f));
            DrawRectangleRoundedLines((Rectangle){30, ALTURA_TELA - 80, LARGURA_TELA - 60, 50}, 0.1f, 10, GRAY);

            if (!estado.jogador1Confirmado)
                DrawTextEx(fontePixel, "ENTER: Confirmar Player 1", (Vector2){50, ALTURA_TELA - 60}, 22, 2, BLUE);
            if (!estado.jogador2Confirmado)
                DrawTextEx(fontePixel, "SPACE: Confirmar Player 2", (Vector2){400, ALTURA_TELA - 60}, 22, 2, RED);

            if (estado.jogador1Confirmado && estado.jogador2Confirmado)
            {
                const char *textoTransicao = "Aguarde... Indo para seleção de mapa!";
                Vector2 tamanhoTexto = MeasureTextEx(fontePixel, textoTransicao, 28, 2);
                DrawTextEx(fontePixel, textoTransicao, (Vector2){LARGURA_TELA / 2 - tamanhoTexto.x / 2, ALTURA_TELA - 55}, 28, 2, YELLOW);
            }
            else
            {
                const char *textoVoltar = "BACKSPACE: Voltar ao menu";
                Vector2 tamanhoTexto = MeasureTextEx(fontePixel, textoVoltar, 22, 2);
                DrawTextEx(fontePixel, textoVoltar, (Vector2){LARGURA_TELA / 2 - tamanhoTexto.x / 2, ALTURA_TELA - 40}, 22, 2, WHITE);
            }
        }
        else if (telaAtual == TELA_SELECAO_MAPA)
        {
            DesenharTelaSelecaoMapa(mapas, mapaAtual, fontePixel);
        }
        else if (telaAtual == TELA_LUTA)
        {
            int offsetTremor = 0;
            if (estado.tremor > 0)
            {
                offsetTremor = (int)(estado.tremor * 20 * sin(GetTime() * 50));
            }

            DrawTexturePro(estado.mapaAtual->textura,
                           (Rectangle){0, 0, (float)estado.mapaAtual->textura.width, (float)estado.mapaAtual->textura.height},
                           (Rectangle){offsetTremor, offsetTremor, LARGURA_TELA, ALTURA_TELA},
                           (Vector2){0, 0}, 0.0f, WHITE);

            // === RENDERIZAÇÃO MELHORADA DOS PERSONAGENS ===

            // Configurações base para renderização
            float larguraPersonagemBase = 200.0f; // Aumentado para melhor visibilidade
            float alturaPersonagemBase = 280.0f;  // Aumentado para melhor visibilidade

            // Player 1 - Posicionamento e renderização
            float larguraP1 = larguraPersonagemBase;
            float alturaP1 = alturaPersonagemBase;

            // Centralizar personagem no chão
            float renderXP1 = estado.jogador1->posicao.x - larguraP1 / 2;
            float renderYP1 = estado.chao - alturaP1;

            Rectangle destP1 = {renderXP1, renderYP1, larguraP1, alturaP1};
            Vector2 origemP1 = {0, 0};

            // Aplicar flip horizontal se necessário
            if (!estado.jogador1->viradoParaDireita)
            {
                destP1.width = -larguraP1;
                origemP1.x = larguraP1;
            }

            // Player 2 - Posicionamento e renderização
            float larguraP2 = larguraPersonagemBase;
            float alturaP2 = alturaPersonagemBase;

            float renderXP2 = estado.jogador2->posicao.x - larguraP2 / 2;
            float renderYP2 = estado.chao - alturaP2;

            Rectangle destP2 = {renderXP2, renderYP2, larguraP2, alturaP2};
            Vector2 origemP2 = {0, 0};

            if (!estado.jogador2->viradoParaDireita)
            {
                destP2.width = -larguraP2;
                origemP2.x = larguraP2;
            }

            // === EFEITOS VISUAIS MELHORADOS ===

            // Sombras dos personagens (mais visíveis)
            Color corSombra = Fade(BLACK, 0.4f);
            DrawEllipse((int)estado.jogador1->posicao.x, (int)(estado.chao + 5), (int)(larguraP1 * 0.5f), 20, corSombra);
            DrawEllipse((int)estado.jogador2->posicao.x, (int)(estado.chao + 5), (int)(larguraP2 * 0.5f), 20, corSombra);

            // Obter texturas corretas - usar sempre a texturaLuta que é a principal
            Texture2D texturaPlayer1 = estado.jogador1->texturaLuta;
            Texture2D texturaPlayer2 = estado.jogador2->texturaLuta;

            // Source rectangle simples - usar textura completa
            Rectangle sourcePlayer1 = {0, 0, (float)texturaPlayer1.width, (float)texturaPlayer1.height};
            Rectangle sourcePlayer2 = {0, 0, (float)texturaPlayer2.width, (float)texturaPlayer2.height};

            // Cores dos personagens
            Color corPlayer1 = WHITE;
            Color corPlayer2 = WHITE;

            // Renderizar personagens com as texturas corretas
            DrawTexturePro(texturaPlayer1, sourcePlayer1, destP1, origemP1, 0.0f, corPlayer1);
            DrawTexturePro(texturaPlayer2, sourcePlayer2, destP2, origemP2, 0.0f, corPlayer2);

            // Auras de poder melhoradas
            if (estado.jogador1->poseAtual == POSE_PODER && estado.jogador1->animando)
            {
                float raioAura = 60 + 15 * sin(GetTime() * 12);
                Vector2 centroAura = {estado.jogador1->posicao.x, estado.jogador1->posicao.y - 80};

                // Múltiplos círculos para efeito de aura
                for (int i = 0; i < 3; i++)
                {
                    float alpha = (0.4f - i * 0.1f) * sin(GetTime() * 8 + i);
                    DrawCircleLines((int)centroAura.x, (int)centroAura.y, raioAura + i * 10,
                                    Fade(estado.jogador1->corHabilidade, fabs(alpha)));
                }
            }

            if (estado.jogador2->poseAtual == POSE_PODER && estado.jogador2->animando)
            {
                float raioAura = 60 + 15 * sin(GetTime() * 12);
                Vector2 centroAura = {estado.jogador2->posicao.x, estado.jogador2->posicao.y - 80};

                for (int i = 0; i < 3; i++)
                {
                    float alpha = (0.4f - i * 0.1f) * sin(GetTime() * 8 + i);
                    DrawCircleLines((int)centroAura.x, (int)centroAura.y, raioAura + i * 10,
                                    Fade(estado.jogador2->corHabilidade, fabs(alpha)));
                }
            }

            // Desenhar hitboxes para debug (opcional - remover depois)
            if (false) // Mude para true para ver as hitboxes
            {
                DrawRectangleLines((int)estado.jogador1->hitbox.x, (int)estado.jogador1->hitbox.y,
                                   (int)estado.jogador1->hitbox.width, (int)estado.jogador1->hitbox.height, GREEN);
                DrawRectangleLines((int)estado.jogador2->hitbox.x, (int)estado.jogador2->hitbox.y,
                                   (int)estado.jogador2->hitbox.width, (int)estado.jogador2->hitbox.height, RED);

                // Desenhar alcance de ataque se atacando
                if (estado.jogador1->poseAtual == POSE_SOCO || estado.jogador1->poseAtual == POSE_CHUTE || estado.jogador1->poseAtual == POSE_PODER)
                {
                    AtualizarAlcanceAtaque(estado.jogador1);
                    DrawRectangleLines((int)estado.jogador1->alcanceAtaque.x, (int)estado.jogador1->alcanceAtaque.y,
                                       (int)estado.jogador1->alcanceAtaque.width, (int)estado.jogador1->alcanceAtaque.height, BLUE);
                }
                if (estado.jogador2->poseAtual == POSE_SOCO || estado.jogador2->poseAtual == POSE_CHUTE || estado.jogador2->poseAtual == POSE_PODER)
                {
                    AtualizarAlcanceAtaque(estado.jogador2);
                    DrawRectangleLines((int)estado.jogador2->alcanceAtaque.x, (int)estado.jogador2->alcanceAtaque.y,
                                       (int)estado.jogador2->alcanceAtaque.width, (int)estado.jogador2->alcanceAtaque.height, YELLOW);
                }
            }

            DesenharParticulas(&estado.particulasImpacto);

            DesenharHUD(&estado, fontePixel);

            if (estado.flashTela > 0)
            {
                Color corFlash = Fade(WHITE, estado.flashTela);
                DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, corFlash);
            }

            // Painel de controles melhorado
            DrawRectangleRounded((Rectangle){30, ALTURA_TELA - 140, LARGURA_TELA - 60, 110}, 0.1f, 10, Fade(BLACK, 0.8f));
            DrawRectangleRoundedLines((Rectangle){30, ALTURA_TELA - 140, LARGURA_TELA - 60, 110}, 0.1f, 10, WHITE);

            const char *controlesP1 = TextFormat("P1: %s/%s=Mover | %s=Soco | %s=Chute | %s=ESPECIAL",
                                                 ObterNomeTecla(config.teclaEsquerdaP1),
                                                 ObterNomeTecla(config.teclaDireitaP1),
                                                 ObterNomeTecla(config.teclaSocoP1),
                                                 ObterNomeTecla(config.teclaChute1),
                                                 ObterNomeTecla(config.teclaPoderP1));
            DrawTextEx(fontePixel, controlesP1, (Vector2){50, ALTURA_TELA - 125}, 16, 2, BLUE);

            const char *controlesP2 = TextFormat("P2: %s/%s=Mover | %s=Soco | %s=Chute | %s=ESPECIAL",
                                                 ObterNomeTecla(config.teclaEsquerdaP2),
                                                 ObterNomeTecla(config.teclaDireitaP2),
                                                 ObterNomeTecla(config.teclaSocoP2),
                                                 ObterNomeTecla(config.teclaChute2),
                                                 ObterNomeTecla(config.teclaPoderP2));
            DrawTextEx(fontePixel, controlesP2, (Vector2){50, ALTURA_TELA - 105}, 16, 2, RED);

            DrawTextEx(fontePixel, "APROXIME-SE do oponente para atacar! Acumule poder com ataques básicos!", (Vector2){50, ALTURA_TELA - 85}, 18, 2, YELLOW);

            DrawTextEx(fontePixel, "ESC: Voltar ao menu", (Vector2){LARGURA_TELA / 2 - 100, ALTURA_TELA - 45}, 16, 2, LIGHTGRAY);
        }

        EndDrawing();
    }

    for (int i = 0; i < MAX_PERSONAGENS; i++)
    {
        UnloadTexture(personagens[i].textura);
        UnloadTexture(personagens[i].texturaLuta);
    }

    for (int i = 0; i < MAX_MAPAS; i++)
    {
        UnloadTexture(mapas[i].textura);
    }

    UnloadTexture(fundoMenu);
    UnloadTexture(fundoSelecao);
    UnloadTexture(logo);
    UnloadFont(fontePixel);

    UnloadMusicStream(musicaFundo);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}