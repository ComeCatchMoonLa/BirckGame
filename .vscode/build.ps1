param(
    [Parameter(Mandatory = $true)]
    [string]$Workspace
)

$build = Join-Path $Workspace 'build'
$bin = Join-Path $build 'bin'
Get-ChildItem $bin -Filter '*.exe' -ErrorAction SilentlyContinue |
    ForEach-Object { Get-Process -Name $_.BaseName -ErrorAction SilentlyContinue | Stop-Process -Force }

if (-not (Test-Path (Join-Path $build 'CMakeCache.txt'))) {
    & cmake -S $Workspace -B $build -G 'MinGW Makefiles' -DCMAKE_BUILD_TYPE=Debug
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

& cmake --build $build
exit $LASTEXITCODE
