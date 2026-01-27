Write-Host "=== NexusPark File Verification ===" -ForegroundColor Cyan

# Check required directories
$requiredDirs = @("include", "src")
foreach ($dir in $requiredDirs) {
    if (Test-Path "backend\$dir") {
        Write-Host "✅ $dir directory exists" -ForegroundColor Green
    } else {
        Write-Host "❌ $dir directory missing" -ForegroundColor Red
    }
}

# Count header files
$headerCount = (Get-ChildItem "backend\include\*.h" -ErrorAction SilentlyContinue).Count
Write-Host "Header files: $headerCount/10" -ForegroundColor $(if ($headerCount -eq 10) { "Green" } else { "Yellow" })

# Count source files  
$sourceCount = (Get-ChildItem "backend\src\*.cpp" -ErrorAction SilentlyContinue).Count
Write-Host "Source files: $sourceCount/10" -ForegroundColor $(if ($sourceCount -eq 10) { "Green" } else { "Yellow" })

Write-Host "=== Verification Complete ===" -ForegroundColor Cyan
