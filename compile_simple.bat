@echo off
cls
echo ==========================================
echo     UNI FIGHT - COMPILACAO NOTEPAD++
echo ==========================================
echo.

REM Compilar o jogo
echo Compilando...
gcc -o unifight.exe unifight.c -lraylib -lopengl32 -lgdi32 -lwinmm -std=c99 -DPLATFORM_DESKTOP

REM Verificar se compilou com sucesso
if exist unifight.exe (
    echo.
    echo ==========================================
    echo        COMPILACAO BEM-SUCEDIDA!
    echo ==========================================
    echo.
    echo Iniciando o jogo...
    echo.
    unifight.exe
) else (
    echo.
    echo ==========================================
    echo            ERRO DE COMPILACAO
    echo ==========================================
    echo.
    echo VERIFIQUE:
    echo 1. Se o MinGW-w64 esta instalado
    echo 2. Se a Raylib esta configurada
    echo 3. Se todos os arquivos estao presentes
    echo.
    echo Para mais detalhes, use: compile_notepad.bat
    echo.
)

pause 