$ErrorActionPreference = "Stop"

Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "      Building ChartSystem Project...     " -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan

$CXX = "g++"
$CXXFLAGS = "-std=c++14", "-g"
$INCLUDES = "-I", "src"

$SrcFiles = Get-ChildItem -Path "src" -Filter "*.cpp" -Recurse | Select-Object -ExpandProperty FullName

$LIBS = "-leasyx", "-lgdi32", "-lole32", "-loleaut32", "-luuid", "-lwinmm", "-lmsimg32"
$OUT_EXE = "main.exe"

Write-Host "Compiling files..."
$ArgList = @()
$ArgList += $CXXFLAGS
$ArgList += $INCLUDES
$ArgList += $SrcFiles
$ArgList += $LIBS
$ArgList += "-o", $OUT_EXE

$process = Start-Process -FilePath $CXX -ArgumentList $ArgList -NoNewWindow -Wait -PassThru

if ($process.ExitCode -eq 0) {
    Write-Host "`n[SUCCESS] Build completed successfully! Executable: $OUT_EXE" -ForegroundColor Green
} else {
    Write-Host "`n[ERROR] Build failed! Please check the compilation errors." -ForegroundColor Red
}
