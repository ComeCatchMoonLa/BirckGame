param(
    [Parameter(Mandatory = $true)]
    [string]$Workspace
)

$build = Join-Path $Workspace 'build'
$running = @(
    'brick_launcher', 'brick_tests', 'Tetris', 'TetrisPro', 'Snack', 'SnackPro',
    'Eliminate', 'FullUp', 'NaughtyBrick', 'CantMove', 'Racing', 'MazeOne', 'MazeRoot'
)
Get-Process -Name $running -ErrorAction SilentlyContinue | Stop-Process -Force

if (-not (Test-Path (Join-Path $build 'CMakeCache.txt'))) {
    & cmake -S $Workspace -B $build -G 'MinGW Makefiles' -DCMAKE_BUILD_TYPE=Debug
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

& cmake --build $build
exit $LASTEXITCODE
