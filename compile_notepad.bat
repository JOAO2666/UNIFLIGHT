@echo off
echo ========================================
echo    COMPILANDO UNI FIGHT - NOTEPAD++
echo ========================================

REM Verificar se o compilador existe
where gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo ERRO: GCC nao encontrado! Instale o MinGW-w64 primeiro.
    echo Download: https://www.mingw-w64.org/downloads/
    pause
    exit /b 1
)

REM Verificar se a Raylib existe
if not exist "raylib\include\raylib.h" (
    echo ERRO: Raylib nao encontrada na pasta raylib\
    echo Baixe a Raylib pre-compilada para Windows
    echo https://github.com/raysan5/raylib/releases
    pause
    exit /b 1
)

echo Compilando unifight.c...

REM Comando de compilacao com todas as flags necessarias
gcc -o unifight.exe unifight.c ^
    -I"raylib/include" ^
    -L"raylib/lib" ^
    -lraylib -lopengl32 -lgdi32 -lwinmm ^
    -std=c99 -Wall -Wextra ^
    -DPLATFORM_DESKTOP

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo    COMPILACAO CONCLUIDA COM SUCESSO!
    echo ========================================
    echo.
    echo Executavel criado: unifight.exe
    echo Para executar: unifight.exe
    echo.
) else (
    echo.
    echo ========================================
    echo         ERRO NA COMPILACAO!
    echo ========================================
    echo.
    echo Verifique os erros acima e corrija o codigo.
)

pause 