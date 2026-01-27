Write-Host "Building NexusPark..." -ForegroundColor Cyan
cd backend

# Try to compile
g++ -std=c++17 main.cpp src/*.cpp -Iinclude -o nexuspark.exe

if ($LASTEXITCODE -eq 0) {
    Write-Host "✅ Build successful!" -ForegroundColor Green
    Write-Host "Run: .\nexuspark.exe" -ForegroundColor Yellow
} else {
    Write-Host "❌ Build failed!" -ForegroundColor Red
}
