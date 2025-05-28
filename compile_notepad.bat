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

echo Compilando unifight.c...

REM Tentar várias configurações comuns da Raylib
if exist "raylib\include\raylib.h" (
    echo Usando Raylib da pasta local...
    gcc -o unifight.exe unifight.c -I"raylib/include" -L"raylib/lib" -lraylib -lopengl32 -lgdi32 -lwinmm -std=c99 -DPLATFORM_DESKTOP
) else if exist "C:\raylib\include\raylib.h" (
    echo Usando Raylib do diretorio C:\raylib...
    gcc -o unifight.exe unifight.c -I"C:/raylib/include" -L"C:/raylib/lib" -lraylib -lopengl32 -lgdi32 -lwinmm -std=c99 -DPLATFORM_DESKTOP
) else if exist "C:\raylib\raylib\src\raylib.h" (
    echo Usando Raylib source do diretorio C:\raylib\raylib\src...
    gcc -o unifight.exe unifight.c -I"C:/raylib/raylib/src" -L"C:/raylib/raylib/src" -lraylib -lopengl32 -lgdi32 -lwinmm -std=c99 -DPLATFORM_DESKTOP
) else (
    echo Tentando compilacao simples assumindo Raylib no PATH...
    gcc -o unifight.exe unifight.c -lraylib -lopengl32 -lgdi32 -lwinmm -std=c99 -DPLATFORM_DESKTOP
)

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo    COMPILACAO CONCLUIDA COM SUCESSO!
    echo ========================================
    echo.
    echo Executavel criado: unifight.exe
    echo Para executar: unifight.exe
    echo.
    if exist "unifight.exe" (
        echo Executando o jogo...
        start unifight.exe
    )
) else (
    echo.
    echo ========================================
    echo         ERRO NA COMPILACAO!
    echo ========================================
    echo.
    echo SOLUCOES POSSIVEIS:
    echo 1. Baixe a Raylib pre-compilada para Windows
    echo 2. Extraia na pasta 'raylib' dentro desta pasta
    echo 3. Ou extraia em 'C:\raylib'
    echo 4. Download: https://github.com/raysan5/raylib/releases
    echo.
)

pause 