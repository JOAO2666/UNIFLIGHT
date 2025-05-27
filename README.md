# UNI FIGHT - Jogo de Luta

## Melhorias Implementadas

### 📋 Resumo dos Personagens
Agora cada personagem possui informações detalhadas que são exibidas na tela de seleção:

#### Personagens Disponíveis:
1. **Kael** (Fogo) - Homem de pele morena, corpo forte e definido. Treinou com monges e feiticeiros, aprendendo a controlar o fogo.
2. **Dandara** (Gelo) - Mulher esbelta com cabelos rosas. Ex-mercenária que se tornou caçadora de recompensas.
3. **Valéria** (Gelo) - Porte ágil, cabelos castanho-escuros curtos. Caçadora de segredos em busca de vingança.
4. **Tim** (Gelo) - Jovem guerreiro das montanhas que usa a força apenas para proteger outros.
5. **Joana** (Fogo) - Perita em artes marciais que usa suas habilidades para deter inimigos.
6. **Yuri** (Fogo) - Ex-capitão da guarda que agora vive como mercenário.

### 🗺️ Sistema de Mapas
Implementado sistema de seleção de mapas com 6 arenas diferentes:

1. **Arena Vulcânica** - Cercada por lava e rochas ígneas. Ideal para lutadores de fogo.
2. **Caverna Cristalina** - Caverna mística com cristais de gelo brilhantes. Favorece usuários de gelo.
3. **Templo Gelado** - Antigo templo nas montanhas geladas. Ambiente sagrado e equilibrado.
4. **Floresta Ancestral** - Floresta antiga com ruínas místicas. Energia natural balanceada.
5. **Coliseu Neutro** - Arena equilibrada sem vantagens elementais para todos os lutadores.
6. **Santuário Perdido** - Ruínas de um antigo santuário com poderes místicos adormecidos.

### 🎮 Controles Completos

#### Seleção de Personagens:
- **Player 1**: A/D para navegar personagens, ENTER para confirmar
- **Player 2**: ←/→ para navegar personagens, SPACE para confirmar
- **BACKSPACE**: Voltar ao menu

#### Seleção de Mapas:
- **←/→**: Navegar entre mapas
- **ENTER**: Confirmar mapa selecionado
- **BACKSPACE**: Voltar para seleção de personagens

#### Sistema de Luta:
- **Player 1**: Q=Poder Especial, E=Soco, R=Chute
- **Player 2**: P=Poder Especial, O=Soco, I=Chute
- **ESC**: Voltar ao menu principal

### ✨ Interface Melhorada
- **Tela de Seleção**: Informações detalhadas dos personagens com confirmação visual
- **Tela de Mapas**: Preview completo dos mapas com descrições
- **Tela de Luta**: HUD completo com barras de vida e poder
- **Sistema de Rounds**: Timer de 60 segundos por round, 3 rounds por luta
- **Cores Distintivas**: Fogo = Vermelho, Gelo = Azul, Confirmado = Verde



### 🔧 Estruturas de Dados
Expandidas as estruturas para incluir:
- História do personagem
- Tipo de habilidade (Fogo/Gelo)
- Cor da habilidade
- Sistema de mapas com descrições

### 💪 Sistema de Combate (Conforme Documentação)
- **Soco**: 10 XP de dano
- **Chute**: 20 XP de dano
- **Habilidade Gelo**: 50 XP de dano
- **Habilidade Fogo**: 50 XP de dano
- **3 Rounds** de 60 segundos cada

### 🎬 Sistema de Animação Avançado
Implementado sistema completo de animação com troca de sprites em tempo real:

#### Poses Dinâmicas:
- **POSE_SOCO**: Animação rápida (0.3s) com movimento para frente
- **POSE_CHUTE**: Animação com salto (0.5s) e movimento amplo
- **POSE_PODER**: Animação especial (0.8s) com levitação
- **POSE_DANO**: Reação ao receber ataques

#### Efeitos Visuais:
- **Sistema de Partículas**: Até 50 partículas simultâneas com física
- **Tremor de Tela**: Proporcional ao impacto dos golpes
- **Flash de Tela**: Efeito especial para ataques de poder
- **Brilho dos Personagens**: Cores pulsantes durante animações

#### Formato de Arquivos:
```
personagens/[NomePersonagem]_[pose].png
Exemplo: joao_soco.png, Dandara_chute.png
```

#### Sistema Anti-Spam:
- Personagens não podem atacar enquanto animando
- Cada golpe tem duração específica
- Efeitos visuais únicos por tipo de ataque

Para documentação completa, consulte `ANIMACOES.md`

## Como Executar
1. Abra o `unifight.c` no **Notepad++ for Raylib**
2. Pressione `F5` ou clique em "Run" para executar o jogo
3. **Resolução**: 1920x1080 (Full HD Horizontal)
4. Certifique-se de que todas as imagens estão nas pastas corretas:
   - `personagens/` - Imagens dos perfis dos personagens
   - `frames/` - Imagens dos mapas de luta
   - `fontes/` - Fonte utilizada no jogo



## Próximos Passos
- ✅ ~~Implementar tela de luta real~~ **CONCLUÍDO**
- ✅ ~~Sistema de combate com os ataques definidos~~ **CONCLUÍDO**
- ✅ ~~Animações dos personagens~~ **CONCLUÍDO**
- ✅ ~~Sistema de partículas e efeitos visuais~~ **CONCLUÍDO**
- 🔄 Efeitos sonoros (estrutura preparada)
- 🔄 Sistema de pontuação por rounds
- 🔄 Mais poses personalizadas para cada personagem 