<#
.SYNOPSIS
    自动化构建和运行脚本
.DESCRIPTION
    使用方法:
    1. .\run.ps1          -> 仅执行编译 (cmake --build) 并运行游戏
    2. .\run.ps1 -i       -> 删除 build 文件夹，重新生成配置，重新编译，拷贝 DLL，并运行游戏
#>

param (
    [switch]$i  # 定义 -i 开关参数
)

# 项目路径配置
$BuildDir = "build"
$ExeName = "soulProject.exe"
$DllSource = "F:/soulProject/vcpkg/installed/x64-mingw-dynamic/debug/bin" 

# 1. 如果指定了 -i 参数，执行清理和初始化
if ($i) {
    Write-Host ">>> [模式: 初始化] 正在清理旧构建..." -ForegroundColor Yellow
    if (Test-Path $BuildDir) {
        Remove-Item -Recurse -Force $BuildDir
    }
    
    Write-Host ">>> 正在生成 CMake 配置..." -ForegroundColor Cyan
    cmake -G "MinGW Makefiles" -B $BuildDir -S . -DCMAKE_TOOLCHAIN_FILE=F:/soulProject/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host ">>> CMake 配置失败，停止执行。" -ForegroundColor Red
        exit
    }
}

# 2. 执行编译
Write-Host ">>> 正在编译项目..." -ForegroundColor Cyan
cmake --build $BuildDir

if ($LASTEXITCODE -ne 0) {
    Write-Host ">>> 编译失败，停止执行。" -ForegroundColor Red
    exit
}

# 3. 如果是 -i 模式，编译完成后需要拷贝 DLL
if ($i) {
    Write-Host ">>> [初始化] 正在拷贝 DLL 文件..." -ForegroundColor Yellow
    
    # 检查 DLL 目录是否存在
    if (Test-Path $DllSource) {
        Copy-Item "$DllSource/*.dll" -Destination $BuildDir -Force
        Write-Host ">>> DLL 拷贝完成。" -ForegroundColor Green
    } else {
        Write-Host ">>> 警告：找不到 DLL 源目录: $DllSource" -ForegroundColor Red
    }
}

# 4. 启动游戏
$ExePath = "$BuildDir/$ExeName"
if (Test-Path $ExePath) {
    Write-Host ">>> 正在启动游戏..." -ForegroundColor Green
    Write-Host "--------------------------------------------------" -ForegroundColor DarkGray
    & $ExePath
} else {
    Write-Host ">>> 错误：找不到可执行文件: $ExePath" -ForegroundColor Red
}