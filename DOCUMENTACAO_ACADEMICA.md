# DOCUMENTAÇÃO DO PROJETO DE INTRODUÇÃO À PROGRAMAÇÃO

## Informações Acadêmicas
- **Disciplina**: Algoritmos e Programação
- **Professora**: Ana Emília
- **Temática do Grupo**: UNI FIGHT (JOGO DE LUTA)

## Equipe de Desenvolvimento

### Integrantes do Grupo:
- **Líder (Gerente)**: Eduardo dos Santos Ferreira Sousa
- **Desenvolvedores**:
  - Milena Franca de Oliveira
  - Eduardo dos Santos Ferreira Sousa
  - Julia Ribeiro da Silva Carvalho
  - João Emanuel Almeida Ramos
  - Kayk Souza Calixto

---

## DOCUMENTAÇÃO DO JOGO

### 🎮 Informações Gerais
- **Nome do Jogo**: UNI FIGHT
- **Gênero**: Jogo de Luta 2D
- **Plataforma**: PC (Windows)
- **Engine**: Raylib (C)

### 🎯 Objetivos
O nosso jogo é chamado de "UNI FIGHT", é um jogo de lutas que tem como objetivo uma batalha entre dois jogadores em busca de um troféu. O jogo contém 6 personagens, cada um deles com um poder diferente sendo entre (Gelo e Fogo), acompanhado de chutes e socos, sendo que cada um arranca uma quantidade "XP" fixa da vida do usuário.

### ⚔️ Sistema de Combate
- **Quantidade de Rounds**: 3 Rounds com 60 segundos cada
- **Quantidade de Mapas**: 6 mapas diferentes
- **Sistema de Dano**:
  - **Soco**: 10 XP
  - **Chute**: 20 XP
  - **Poder de Gelo**: 50 XP
  - **Poder de Fogo**: 50 XP

---

## 👥 PERSONAGENS

### Personagem 01: Kael
- **Habilidades**: Fogo
- **História**: Kael é um homem de pele morena quente, com corpo forte e definido pelo treinamento constante. Seus cabelos são castanhos e ondulados, junto com olhos de âmbar intenso. Passou anos treinando com monges e feiticeiros que cultuavam os elementos, aprendendo a controlar o fogo. Ele agora viaja como caçador de monstros e artefatos arcanos, buscando pistas sobre a origem do fogo que habita seu corpo.

### Personagem 02: Dandara
- **Habilidades**: Gelo
- **História**: Dandara é uma mulher esbelta com cabelos rosas que caem sobre seus ombros. Ela é uma antiga mercenária que foi abandonada por seu grupo e se tornou uma caçadora de recompensas. Sua experiência e conhecimento de combate a tornaram uma guerreira experiente e astuta.

### Personagem 03: Valéria
- **Habilidades**: Gelo
- **História**: Valéria tem um porte ágil e seus cabelos castanho-escuros são curtos, nunca longos, pois podem atrapalhar durante uma missão. Após o assassinato de seus pais, ela precisou cuidar de suas duas irmãs sozinha. Para sobreviver, passou anos trabalhando como mensageira e espiã para nobres rivais. Hoje, age por conta própria como uma caçadora de segredos, vendendo informações e buscando respostas, e vingança, sobre aqueles que sem nenhuma hesitação destruíram aquilo que ela mais amava: sua família.

### Personagem 04: Tim
- **Habilidades**: Gelo
- **História**: Tim é um jovem guerreiro que deixou sua vila nas montanhas em busca de grandes desafios. Treinado por seu avô, carrega consigo um antigo kimono herdado de seu mestre — símbolo de sua promessa de nunca usar a força por orgulho, mas sempre para proteger os outros.

### Personagem 05: Joana
- **Habilidades**: Fogo
- **História**: Joana é perita em artes marciais, porém quando ela vê pessoas em perigo, ela parte para deter os inimigos usando suas habilidades e seu domínio sobre o fogo.

### Personagem 06: Yuri
- **Habilidades**: Fogo
- **História**: Um homem com físico construído para o combate. Ex-capitão da guarda de um reino extinto, foi traído por nobres corruptos e passou anos como prisioneiro. Fugiu sozinho e desde então vive como mercenário e guarda-costas.

---

## 🗺️ MAPAS DE LUTA

1. **Arena Vulcânica** - Cercada por lava e rochas ígneas. Ideal para lutadores de fogo.
2. **Caverna Cristalina** - Caverna mística com cristais de gelo brilhantes. Favorece usuários de gelo.
3. **Templo Gelado** - Antigo templo nas montanhas geladas. Ambiente sagrado e equilibrado.
4. **Floresta Ancestral** - Floresta antiga com ruínas místicas. Energia natural balanceada.
5. **Coliseu Neutro** - Arena equilibrada sem vantagens elementais para todos os lutadores.
6. **Santuário Perdido** - Ruínas de um antigo santuário com poderes místicos adormecidos.

---

## 🛠️ ASPECTOS TÉCNICOS

### Tecnologias Utilizadas:
- **Linguagem**: C
- **Engine Gráfica**: Raylib
- **Resolução**: 1920x1080 (Full HD)
- **Taxa de Atualização**: 60 FPS

### Funcionalidades Implementadas:
- ✅ Sistema de menu interativo
- ✅ Seleção de personagens com informações detalhadas
- ✅ Seleção de mapas com preview
- ✅ Sistema de combate completo
- ✅ Sistema de animação avançado
- ✅ Efeitos visuais (partículas, tremor, flash)
- ✅ HUD completo com barras de vida e poder
- ✅ Sistema de rounds com timer
- ✅ Controles para 2 jogadores

### Controles:
- **Player 1**: Q=Poder, E=Soco, R=Chute, A/D=Navegar
- **Player 2**: P=Poder, O=Soco, I=Chute, ←/→=Navegar

---

## 📊 RESULTADOS ALCANÇADOS

### Objetivos Cumpridos:
- [x] Desenvolvimento de jogo funcional em C
- [x] Implementação de sistema de combate
- [x] Criação de 6 personagens únicos
- [x] Desenvolvimento de 6 mapas diferentes
- [x] Sistema de animação em tempo real
- [x] Interface gráfica completa
- [x] Documentação técnica abrangente

### Aprendizados Obtidos:
- Programação em linguagem C
- Uso da biblioteca Raylib para jogos
- Estruturas de dados complexas
- Gerenciamento de estados de jogo
- Trabalho em equipe e versionamento com Git
- Documentação de software

---

## 📁 ESTRUTURA DO PROJETO

```
unifight/
├── unifight.c              # Código principal
├── structs.c               # Estruturas de dados
├── README.md               # Documentação principal
├── ANIMACOES.md            # Sistema de animação
├── personagens/            # Sprites dos personagens
├── frames/                 # Mapas de luta
├── fontes/                 # Fontes utilizadas
└── Golpes/                 # Poses de combate
```

---

## 🎓 CONCLUSÃO

O projeto UNI FIGHT foi desenvolvido com sucesso, cumprindo todos os objetivos propostos para a disciplina de Algoritmos e Programação. O jogo demonstra a aplicação prática dos conceitos aprendidos em sala de aula, incluindo estruturas de dados, controle de fluxo, funções e gerenciamento de memória.

A equipe conseguiu criar um produto funcional e visualmente atrativo, que serve como exemplo da aplicação dos conhecimentos adquiridos na disciplina.

---

**Data de Conclusão**: Janeiro 2025  
**Repositório**: [https://github.com/JOAO2666/UNIFLIGHT.git](https://github.com/JOAO2666/UNIFLIGHT.git) 