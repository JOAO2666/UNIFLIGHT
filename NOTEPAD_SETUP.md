# 🎮 UNI FIGHT - SETUP PARA NOTEPAD++

## 📋 Requisitos

1. **Notepad++** instalado
2. **MinGW-w64** (compilador GCC) instalado
3. **Raylib** para Windows

## 🚀 Setup Rápido

### 1. Instalar MinGW-w64
- Baixe de: https://www.mingw-w64.org/downloads/
- Adicione ao PATH do Windows

### 2. Instalar Raylib
**Opção A - Pasta Local:**
- Baixe: https://github.com/raysan5/raylib/releases
- Extraia na pasta `raylib/` dentro desta pasta
- Estrutura: `unifight/raylib/include/raylib.h`

**Opção B - Pasta Global:**
- Extraia em `C:\raylib\`
- Estrutura: `C:\raylib\include\raylib.h`

### 3. Configurar Notepad++
1. Abra o Notepad++
2. Vá em **Executar > Executar... (F5)**
3. Cole este comando:
```
cmd /k cd /d "$(CURRENT_DIRECTORY)" && compile_notepad.bat
```
4. Clique em **Salvar** e dê um nome como "Compilar UNI FIGHT"

## 🎯 Como Usar

1. Abra `unifight.c` no Notepad++
2. Pressione **F5** ou use o comando salvo
3. O jogo será compilado e executado automaticamente

## 🎮 Controles do Jogo

### Player 1
- **A/D**: Mover
- **E**: Soco
- **R**: Chute  
- **Q**: Poder Especial

### Player 2
- **←/→**: Mover
- **O**: Soco
- **I**: Chute
- **P**: Poder Especial

## 🛠️ Resolução de Problemas

### "GCC não encontrado"
- Instale MinGW-w64 e adicione ao PATH

### "Raylib não encontrada"
- Verifique se extraiu a Raylib corretamente
- Teste os dois locais: pasta local ou C:\raylib

### Erro de compilação
- Verifique se todos os arquivos estão na pasta
- Certifique-se que `structs.c` está presente

## 📁 Arquivos Necessários

```
unifight/
├── unifight.c          # Código principal
├── structs.c           # Estruturas de dados  
├── compile_notepad.bat # Script de compilação
├── personagens/        # Imagens dos personagens
├── frames/            # Mapas de batalha
├── fontes/            # Arquivos de fonte
├── musicas/           # Arquivos de áudio
└── raylib/            # Biblioteca Raylib (opcional)
```

---
**🎯 Tudo pronto! Agora é só jogar!** 🥊 