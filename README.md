#  UNIFIGHT - Jogo de Luta.

Um jogo de luta 2D desenvolvido em C com Raylib, apresentando personagens únicos com habilidades elementais.

##  Projeto da faculdade 
- **Disciplina**: Algoritmos e Programação
- **Professora**: Ana Emília
- **Equipe**: Eduardo dos Santos Ferreira Sousa (Líder), Milena Franca de Oliveira, Julia Ribeiro da Silva Carvalho, João Emanuel Almeida Ramos, Kayk Souza Calixto


##  Melhorias Recentes Implementadas

###  Interface de Combate Aprimorada
- **HUD Redesenhado**: Interface mais limpa e profissional com gradientes e efeitos visuais
- **Barras de Vida Melhoradas**: 
  - Gradientes de cor baseados na porcentagem de vida
  - Efeito de brilho quando a vida está alta
  - Nomes dos jogadores exibidos acima das barras
  - Sombras nos textos para melhor legibilidade
- **Barras de Poder Aprimoradas**:
  - Efeito pulsante quando o especial está disponível
  - Indicador visual "ESPECIAL PRONTO!" com animação
  - Partículas visuais ao redor da barra quando carregada

###  Organização dos Personagens
- **Renderização Otimizada**: Personagens melhor posicionados e proporcionais na tela
- **Efeitos Visuais Melhorados**:
  - Sombras realistas sob os personagens
  - Auras de poder com múltiplos círculos animados

##  Características Principais

### Personagens Disponíveis
1. **Kael** - Especialista em Fogo
2. **Dandara** - Mestre do Gelo  
3. **Valéria** - Combatente de Gelo
4. **Tim** - Guerreiro das Montanhas (Gelo)
5. **Joana** - Lutadora de Fogo
6. **Yuri** - Mercenário de Fogo

### Sistema de Combate
- **Ataques Básicos**: Soco (20 dano) e Chute (30 dano)
- **Poder Especial**: Ataque devastador (50 dano) - requer 50 pontos de poder
- **Sistema de Proximidade**: É necessário estar próximo para atacar
- **Acúmulo de Poder**: Ataques básicos geram pontos de poder

### Mapas de Batalha
- 6 arenas únicas com temas diferentes
- Desde vulcões até templos gelados
- Cada mapa com sua própria atmosfera visual

##  Controles

### Player 1
- **A/D**: Movimento
- **E**: Soco
- **R**: Chute  
- **Q**: Poder Especial (quando disponível)

### Player 2
- **←/→**: Movimento
- **O**: Soco
- **I**: Chute
- **P**: Poder Especial (quando disponível)

##  Para rodar:

### Requisitos:
- **MinGW-w64** (GCC para Windows)
- **Raylib** (biblioteca gráfica)
- **Notepad++ for Raylib** (recomendado)

### Configuração da Raylib:
1. Baixe de: https://www.raylib.com/
2. Instala o Raylib.

##  Estrutura de Arquivos

```
unifight/
├── unifight.c          # Código principal do jogo
├── structs.c           # Estruturas de dados
├── compile_simple.bat  # Compilação rápida
├── compile_notepad.bat # Compilação com detecção automática
├── NOTEPAD_SETUP.md    # Guia do Notepad++
├── personagens/        # Imagens dos personagens
├── frames/            # Mapas de batalha
├── fontes/            # Arquivos de fonte
├── musicas/           # Arquivos de áudio
└── README.md          # Esta documentação
```

##  Recursos Visuais

- **Animações Fluidas**: Sistema de sprites com transições suaves, porém falta organizar as imagens 
- **Efeitos Especiais**: Partículas, brilhos e auras durante ataques
- **Interface Responsiva**: HUD adaptativo com feedback visual
- **Personagens Únicos**: Cada lutador com visual e habilidades distintas

##  Recursos de Áudio

- Música de fundo ambiente
- Efeitos sonoros para ataques (quando implementados, porque ainda não tem)
- Sistema de volume configurável

## ⚙ Opções e Configurações

- Configuração de controles personalizáveis
- Ajuste de volume de música e efeitos
- Sistema de salvamento de configurações

## Próximos Passos
- ✅ ~~Implementar tela de luta real~~ **CONCLUÍDO**
- ✅ ~~Sistema de combate com os ataques definidos~~ **CONCLUÍDO**
- ✅ ~~Animações dos personagens~~ **CONCLUÍDO**
- ✅ ~~Sistema de partículas e efeitos visuais~~ **CONCLUÍDO**
- 🔄 Efeitos sonoros (estrutura preparada)
- 🔄 Animação dos personagens andando, se movendo e etc.
- 🔄 Sistema de pontuação por rounds
- 🔄 Mais poses personalizadas para cada personagem

