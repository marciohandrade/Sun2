@echo off
rem 매개변수-  작업폴더 tmd파일path+파일명

set workdir=%1
set tmdfile=%2

set batchdir=%workdir%\Client\BuildSystem\batch

echo.
echo ▷ 3DFileAccess.dll Packing
echo.      

call %batchdir%\EngineDLLPacking_Themida.bat %workdir% 3DFileAccess.dll %tmdfile%


echo.
echo ▷ 3DRendererD3D.dll Packing
echo.      

call %batchdir%\EngineDLLPacking_Themida.bat %workdir% 3DRendererD3D.dll %tmdfile%

echo.
echo ▷ 3DTerrain.dll Packing
echo.      

call %batchdir%\EngineDLLPacking_Themida.bat %workdir% 3DTerrain.dll %tmdfile%

echo.
echo ▷ ControlsDLL.dll Packing
echo.      

call %batchdir%\EngineDLLPacking_Themida.bat %workdir% ControlsDLL.dll %tmdfile%

echo.
echo ▷ Effect.dll Packing
echo.      

call %batchdir%\EngineDLLPacking_Themida.bat %workdir% Effect.dll %tmdfile%

