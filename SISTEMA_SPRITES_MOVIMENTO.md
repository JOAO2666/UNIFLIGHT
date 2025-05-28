# SISTEMA DE SPRITES E MOVIMENTO - UNI FIGHT

## 🎯 **SISTEMA COMPLETO IMPLEMENTADO**

### ✅ **1. Sistema de Sprites com Recorte Correto**

#### **Estruturas Atualizadas:**
```c
typedef struct {
    // Sistema de posição e movimento
    Vector2 posicao;
    Vector2 velocidade;
    float velocidadeMaxima;
    bool viradoParaDireita;
    Rectangle hitbox;
    Rectangle alcanceAtaque;

    // Sistema de sprites (para spritesheets)
    int frameAtual;
    int totalFrames;
    float timerFrame;
    float duracaoFrame;
    Rectangle frameSource;
    
    // Nova pose de caminhada
    Texture2D texturaWalk;
} Personagem;
```

#### **Poses Implementadas:**
- `POSE_IDLE` - Parado (4 frames)
- `POSE_WALK` - Caminhando (6 frames) ✨ **NOVO**
- `POSE_SOCO` - Soco (3 frames)
- `POSE_CHUTE` - Chute (4 frames)
- `POSE_PODER` - Habilidade especial (5 frames)
- `POSE_DANO` - Recebendo dano (2 frames)
- `POSE_DEFESA` - Defendendo (1 frame)

#### **Sistema de Recorte de Sprites:**
```c
void AtualizarAnimacaoSprite(Personagem *p, float deltaTime)
{
    // Atualizar timer do frame
    p->timerFrame += deltaTime;
    
    if (p->timerFrame >= p->duracaoFrame) {
        p->timerFrame = 0.0f;
        p->frameAtual++;
        
        // Configurar frames por pose
        int maxFrames = obterMaxFramesPorPose(p->poseAtual);
        if (p->frameAtual >= maxFrames) {
            p->frameAtual = 0;
        }
    }
    
    // Configurar source rectangle para sprite
    p->frameSource.x = p->frameAtual * frameWidth;
    p->frameSource.y = (int)p->poseAtual * frameHeight;
    p->frameSource.width = frameWidth;
    p->frameSource.height = frameHeight;
}
```

### ✅ **2. Sistema de Movimento Completo**

#### **Controles de Movimento:**
- **Player 1**: A/D para mover
- **Player 2**: ←/→ para mover
- **Velocidade**: 200 pixels/segundo
- **Limites**: Arena com bordas definidas

#### **Funções de Movimento:**
```c
void ProcessarMovimentoPlayer1(Personagem *p, ConfiguracaoJogo *config)
{
    if (!p->podeSeMovimentar) return;
    
    p->velocidade.x = 0;
    
    if (IsKeyDown(config->teclaEsquerdaP1)) {
        p->velocidade.x = -p->velocidadeMaxima;
        if (p->poseAtual == POSE_IDLE) {
            p->poseAtual = POSE_WALK; // Animação de caminhada
        }
    }
    // ... similar para direita
}

void MoverPersonagem(Personagem *p, float deltaTime, float limiteEsquerdo, float limiteDireito)
{
    if (!p->podeSeMovimentar) return;
    
    // Aplicar movimento
    p->posicao.x += p->velocidade.x * deltaTime;
    
    // Verificar limites da arena
    if (p->posicao.x < limiteEsquerdo + p->hitbox.width / 2) {
        p->posicao.x = limiteEsquerdo + p->hitbox.width / 2;
        p->velocidade.x = 0;
    }
    // ... verificar limite direito
    
    // Atualizar hitbox
    AtualizarHitbox(p);
}
```

### ✅ **3. Sistema de Detecção de Proximidade**

#### **Hitboxes e Alcance:**
```c
typedef struct {
    Rectangle hitbox;        // Área do personagem (80x120)
    Rectangle alcanceAtaque; // Área de alcance do ataque
    float alcanceSoco;       // 90 pixels
    float alcanceChute;      // 110 pixels  
    float alcancePoder;      // 150 pixels
} Personagem;
```

#### **Detecção de Proximidade:**
```c
bool VerificarProximidade(Personagem *atacante, Personagem *alvo)
{
    AtualizarAlcanceAtaque(atacante);
    return CheckCollisionRecs(atacante->alcanceAtaque, alvo->hitbox);
}

bool ExecutarAtaque(Personagem *atacante, Personagem *alvo, TipoPose tipoAtaque, int dano)
{
    if (!atacante->podeAtacar || atacante->animando) return false;
    
    // Verificar se está próximo o suficiente
    if (!VerificarProximidade(atacante, alvo)) return false;
    
    // Executar ataque
    alvo->vidaAtual -= dano;
    IniciarAnimacao(atacante, tipoAtaque, 0.5f);
    IniciarAnimacao(alvo, POSE_DANO, 0.3f);
    
    // Empurrar o alvo para trás
    float forcaEmpurrao = 50.0f;
    if (atacante->viradoParaDireita) {
        alvo->posicao.x += forcaEmpurrao;
    } else {
        alvo->posicao.x -= forcaEmpurrao;
    }
    
    return true;
}
```

### ✅ **4. Sistema de Renderização Dinâmica**

#### **Posicionamento Baseado em Coordenadas:**
```c
// Calcular posições de renderização (centralizadas na posição do personagem)
float renderX1 = estado.jogador1->posicao.x - larguraPersonagem / 2 + offsetX1;
float renderY1 = estado.jogador1->posicao.y - alturaPersonagem + offsetY1;

// Player 1 - com flip horizontal se necessário
Rectangle destPlayer1 = {renderX1, renderY1, larguraPersonagem, alturaPersonagem};
if (!estado.jogador1->viradoParaDireita) {
    destPlayer1.width = -larguraPersonagem; // Flip horizontal
}

DrawTexturePro(texturaPlayer1, sourcePlayer1, destPlayer1, (Vector2){0, 0}, 0.0f, corPlayer1);
```

#### **Flip Automático:**
- Personagens se viram automaticamente um para o outro
- Sprites são espelhados horizontalmente quando necessário
- Alcance de ataque se ajusta à direção

### ✅ **5. Feedback Visual e Debug**

#### **Informações na Tela:**
```c
// Controles atualizados
"P1: A/D=Mover, Q=Poder, E=Soco, R=Chute"
"P2: ←/→=Mover, P=Poder, O=Soco, I=Chute"
"APROXIME-SE para atacar! ESC: Voltar ao menu"

// Debug de distância
float distancia = fabs(estado.jogador1->posicao.x - estado.jogador2->posicao.x);
"Distância: %.0f pixels"
```

#### **Debug Visual (Opcional):**
```c
// Desenhar hitboxes para debug
if (DEBUG_MODE) {
    DrawRectangleLines(p->hitbox.x, p->hitbox.y, p->hitbox.width, p->hitbox.height, GREEN);
    DrawRectangleLines(p->alcanceAtaque.x, p->alcanceAtaque.y, 
                      p->alcanceAtaque.width, p->alcanceAtaque.height, BLUE);
}
```

### ✅ **6. Inicialização e Configuração**

#### **Posições Iniciais:**
```c
// Configurar limites da arena
estado.limiteEsquerdo = 100.0f;
estado.limiteDireito = LARGURA_TELA - 100.0f;
estado.chao = ALTURA_TELA - 100.0f;

// Inicializar posições dos personagens na arena
InicializarPosicaoPersonagem(estado.jogador1, 300.0f, estado.chao, true);
InicializarPosicaoPersonagem(estado.jogador2, LARGURA_TELA - 300.0f, estado.chao, false);
```

#### **Carregamento de Sprites:**
```c
// Carregar pose de caminhada
sprintf(caminhoArquivo, "./personagens/%s_walk.png", nomeArquivo);
Texture2D texturaWalk = LoadTexture(caminhoArquivo);
if (texturaWalk.id > 0) {
    p->texturaWalk = texturaWalk;
} else {
    p->texturaWalk = p->texturaLuta; // Fallback
}
```

## 🎮 **Como Funciona o Sistema**

### **1. Movimento:**
1. Jogador pressiona A/D (P1) ou ←/→ (P2)
2. `ProcessarMovimentoPlayer1/2()` define velocidade
3. `MoverPersonagem()` aplica movimento e verifica limites
4. Pose muda para `POSE_WALK` automaticamente
5. `AtualizarAnimacaoSprite()` anima os frames

### **2. Combate:**
1. Jogador pressiona tecla de ataque (Q/E/R ou P/O/I)
2. `ExecutarAtaque()` verifica proximidade
3. Se próximo: aplica dano, inicia animações, empurra oponente
4. Se longe: mostra mensagem "muito longe!"
5. Efeitos visuais na posição real do oponente

### **3. Sprites:**
1. Cada personagem tem múltiplas texturas por pose
2. `ObterTexturaAtual()` retorna textura baseada na pose
3. `ObterSourceRectSprite()` define recorte do frame atual
4. `DrawTexturePro()` renderiza com flip se necessário

## 📊 **Alcances de Ataque**

| Ataque | Alcance | Dano | Poder Ganho |
|--------|---------|------|-------------|
| **Soco** | 90px | 10 | +10 |
| **Chute** | 110px | 20 | +15 |
| **Poder** | 150px | 50 | -50 |

## 🔧 **Arquivos de Sprites Necessários**

Para cada personagem (`nome`):
- `nome_walk.png` - Animação de caminhada ✨ **NOVO**
- `nome_soco.png` - Animação de soco
- `nome_chute.png` - Animação de chute  
- `nome_poder.png` - Animação de poder
- `nome_defesa.png` - Pose de defesa (opcional)
- `nome_dano.png` - Pose de dano (opcional)

## ✅ **Status Final**

### **Implementado com Sucesso:**
- ✅ **Sistema de sprites** com recorte correto
- ✅ **Movimento livre** dos personagens
- ✅ **Detecção de proximidade** para ataques
- ✅ **Hitboxes e alcances** configuráveis
- ✅ **Animações dinâmicas** baseadas em movimento
- ✅ **Flip automático** dos sprites
- ✅ **Feedback visual** de distância
- ✅ **Sistema de empurrão** ao atacar
- ✅ **Limites da arena** funcionais

### **Resultado:**
🎮 **FIGHTING GAME COMPLETO** com movimento, sprites animados e sistema de combate baseado em proximidade!

---

**Status**: ✅ **SISTEMA COMPLETO IMPLEMENTADO**  
**Tipo**: Fighting Game 2D com movimento livre  
**Sprites**: Sistema de recorte e animação  
**Combate**: Baseado em proximidade e hitboxes 