# SISTEMA DE OPÇÕES - UNI FIGHT

## 🎛️ Funcionalidades Implementadas

### Menu de Opções Completo
- **Acesso**: Menu Principal → "OPÇÕES"
- **Interface**: Tela dedicada com navegação intuitiva
- **Controles**: ↑/↓ para navegar, ENTER para selecionar, BACKSPACE para voltar

### 🎮 Configuração de Controles
#### Player 1 - Controles Personalizáveis:
- **Poder**: Padrão Q (configurável)
- **Soco**: Padrão E (configurável) 
- **Chute**: Padrão R (configurável)
- **Navegação**: A/D (fixo)
- **Confirmar**: ENTER (fixo)

#### Player 2 - Controles Personalizáveis:
- **Poder**: Padrão P (configurável)
- **Soco**: Padrão O (configurável)
- **Chute**: Padrão I (configurável)
- **Navegação**: ←/→ (fixo)
- **Confirmar**: SPACE (fixo)

### 🔧 Sistema de Configuração
- **Configuração Sequencial**: Ao selecionar "Controles Player X", configure as 3 teclas em sequência
- **Feedback Visual**: Mostra as teclas atuais configuradas em tempo real
- **Cancelamento**: ESC para cancelar configuração em andamento
- **Reset**: Opção para restaurar controles padrão

### 🔊 Controle de Volume
#### Volume da Música:
- **Controle**: Setas ←/→ para ajustar
- **Range**: 0% a 100%
- **Feedback**: Barra visual + porcentagem
- **Aplicação**: Tempo real na música de fundo

#### Volume dos Efeitos:
- **Controle**: Setas ←/→ para ajustar  
- **Range**: 0% a 100%
- **Feedback**: Barra visual + porcentagem
- **Preparado**: Para sons de combate (quando implementados)

### 🎵 Música de Fundo
- **Arquivo**: `musicas/menu_background.mp3`
- **Tipo**: Música calma e relaxante
- **Loop**: Contínuo durante o jogo
- **Volume**: Controlável via opções
- **Fonte**: Música livre de direitos autorais

## 🖥️ Interface Visual

### Tela de Opções:
```
                    OPÇÕES

┌─────────────────────────────────────────┐
│ CONTROLES PLAYER 1                      │
│ Poder:Q Soco:E Chute:R                  │
├─────────────────────────────────────────┤
│ CONTROLES PLAYER 2                      │
│ Poder:P Soco:O Chute:I                  │
├─────────────────────────────────────────┤
│ VOLUME MÚSICA          ████████░░ 80%   │
├─────────────────────────────────────────┤
│ VOLUME EFEITOS         ██████████ 100%  │
├─────────────────────────────────────────┤
│ RESETAR CONTROLES                       │
├─────────────────────────────────────────┤
│ SALVAR CONFIGURAÇÕES                    │
├─────────────────────────────────────────┤
│ CARREGAR CONFIGURAÇÕES                  │
├─────────────────────────────────────────┤
│ VOLTAR                                  │
└─────────────────────────────────────────┘

↑/↓: Navegar | ENTER: Selecionar | ←/→: Ajustar Volume | BACKSPACE: Voltar
```

### Tela de Luta Atualizada:
- **Controles Dinâmicos**: Mostra as teclas configuradas pelo usuário
- **Exemplo**: "Player 1: Q=Poder, E=Soco, R=Chute" → "Player 1: F=Poder, G=Soco, H=Chute"

## 🔧 Implementação Técnica

### Estruturas Adicionadas:
```c
typedef struct {
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
```

### Funções Principais:
- `InicializarConfiguracaoPadrao()`: Define valores padrão
- `ObterNomeTecla()`: Converte código de tecla para string
- `DesenharTelaOpcoes()`: Renderiza interface de opções
- `UpdateMusicStream()`: Atualiza música de fundo
- `SetMusicVolume()`: Aplica volume configurado

### Estados de Tela:
- `TELA_OPCOES`: Nova tela adicionada ao enum
- Navegação: MENU → OPCOES → MENU
- Integração completa com sistema existente

## 🎯 Benefícios para o Usuário

### Acessibilidade:
- **Controles Personalizáveis**: Adapta-se a diferentes preferências
- **Feedback Visual**: Interface clara e intuitiva
- **Configuração Simples**: Processo guiado passo a passo

### Experiência de Jogo:
- **Música Ambiente**: Atmosfera mais imersiva
- **Controle de Volume**: Ajuste conforme preferência
- **Flexibilidade**: Cada jogador pode ter seus controles preferidos

### Qualidade Técnica:
- **Tempo Real**: Mudanças aplicadas imediatamente
- **Persistência**: Configurações mantidas durante a sessão
- **Robustez**: Sistema à prova de erros

## 🚀 Funcionalidades Futuras (Preparadas)

### Sistema de Persistência:
- **Salvar Configurações**: Estrutura preparada para arquivo
- **Carregar Configurações**: Sistema de loading implementável
- **Perfis**: Base para múltiplos perfis de usuário

### Expansão de Áudio:
- **Efeitos Sonoros**: Volume já configurável
- **Múltiplas Músicas**: Sistema preparado para playlist
- **Qualidade**: Suporte a diferentes formatos

---

**Status**: ✅ **IMPLEMENTADO E FUNCIONAL**  
**Compatibilidade**: Raylib 5.5+  
**Testado**: Windows 10/11  
**Documentação**: Completa e atualizada 