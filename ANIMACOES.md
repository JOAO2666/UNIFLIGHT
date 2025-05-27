# Sistema de Animação UNI FIGHT

## Visão Geral
O sistema de animação do UNI FIGHT permite que os personagens troquem de sprites (imagens PNG) em tempo real durante os golpes, criando uma sensação fluida de movimento e combate dinâmico.

## Como Funciona

### 1. Poses Disponíveis
Cada personagem pode ter as seguintes poses:
- **POSE_IDLE**: Pose padrão (em repouso)
- **POSE_SOCO**: Animação de soco
- **POSE_CHUTE**: Animação de chute
- **POSE_PODER**: Animação de ataque especial
- **POSE_DEFESA**: Pose defensiva
- **POSE_DANO**: Reação ao receber dano

### 2. Formato dos Arquivos
Para adicionar poses personalizadas, coloque os arquivos PNG na pasta `personagens/` seguindo este formato:

```
personagens/[NomePersonagem]_[pose].png
```

**Exemplos:**
- `personagens/joao_soco.png`
- `personagens/joao_chute.png`
- `personagens/joao_poder.png`
- `personagens/Dandara_soco.png`
- `personagens/Dandara_chute.png`

### 3. Efeitos Visuais Durante Animação

#### Movimento dos Personagens:
- **Soco**: Move 20px para frente
- **Chute**: Move 30px para frente + pula 10px
- **Poder**: Levita 15px
- **Dano**: Recua 15px

#### Efeitos de Cor:
- **Poder**: Brilho roxo pulsante
- **Soco**: Brilho amarelo
- **Chute**: Brilho laranja
- **Dano**: Brilho vermelho

#### Sistema de Partículas:
- **Poder**: 25 partículas roxas
- **Soco**: 15 partículas amarelas
- **Chute**: 20 partículas laranjas

#### Efeitos de Tela:
- **Flash branco** para ataques de poder
- **Tremor da tela** proporcional ao impacto
- **Partículas com física** (gravidade e fade)

### 4. Controles de Luta

**Player 1:**
- Q = Ataque de Poder (consome 50 de energia)
- E = Soco (ganha 10 de energia)
- R = Chute (ganha 15 de energia)

**Player 2:**
- P = Ataque de Poder (consome 50 de energia)
- O = Soco (ganha 10 de energia)
- I = Chute (ganha 15 de energia)

### 5. Sistema Anti-Spam
- Personagens não podem atacar enquanto estão animando
- Cada animação tem duração específica:
  - Soco: 0.3 segundos
  - Chute: 0.5 segundos
  - Poder: 0.8 segundos
  - Dano: 0.3-0.5 segundos

### 6. Adicionando Novos Personagens

Para adicionar poses para um novo personagem:

1. Crie as imagens PNG com as poses desejadas
2. Nomeie seguindo o padrão: `[Nome]_[pose].png`
3. Coloque na pasta `personagens/`
4. O sistema carregará automaticamente as poses disponíveis

### 7. Fallback System
Se uma pose específica não for encontrada, o sistema usa:
1. A textura padrão do personagem
2. Para Joana: tenta usar as poses do João como backup

### 8. Debug e Logs
O sistema exibe logs no console mostrando:
- ✓ Poses carregadas com sucesso
- ✗ Poses não encontradas
- Caminhos dos arquivos tentados

## Exemplo de Uso Completo

```
personagens/
├── joao.png              (pose padrão)
├── joao_soco.png         (pose de soco)
├── joao_chute.png        (pose de chute)
├── joao_poder.png        (pose de poder)
├── Dandara_soco.png      (pose de soco da Dandara)
├── Dandara_chute.png     (pose de chute da Dandara)
└── Dandara_poder.png     (pose de poder da Dandara)
```

## Tecnologia
- **Engine**: Raylib
- **Linguagem**: C
- **Sistema de Partículas**: 50 partículas simultâneas
- **Taxa de Atualização**: 60 FPS
- **Resolução**: 1920x1080

O sistema foi projetado para ser facilmente expansível, permitindo adicionar novas poses e efeitos sem modificar o código principal. 