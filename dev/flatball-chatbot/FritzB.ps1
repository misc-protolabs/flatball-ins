# Kill any existing ollama processes to prevent file locking issues
Stop-Process -Name "ollama" -Force -ErrorAction SilentlyContinue

# Ensure .log folder exists
$logDir = Join-Path $PSScriptRoot ".log"
if (-not (Test-Path $logDir)) {
    New-Item -ItemType Directory -Path $logDir | Out-Null
}

# Define log paths
$ollamaOut = Join-Path $logDir "flatball-chat-ollama-log.md"
$ollamaErr = Join-Path $logDir "flatball-chat-ollama-log.err.md"
$npmOut    = Join-Path $logDir "flatball-chat-npm-log.md"
$npmErr    = Join-Path $logDir "flatball-chat-npm-log.err.md"

# Initialize logs
"# Flatball Ollama Log`n`n## Started: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')`n" | Out-File $ollamaOut
"# Flatball Ollama Errors`n`n" | Out-File $ollamaErr
"# Flatball npm Log`n`n## Started: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')`n" | Out-File $npmOut
"# Flatball npm Errors`n`n" | Out-File $npmErr

# Launch Ollama
$ollamaProc = Start-Process "ollama" -ArgumentList "serve" `
    -RedirectStandardOutput $ollamaOut `
    -RedirectStandardError $ollamaErr `
    -PassThru

if (-not $ollamaProc) {
    Write-Host "Failed to launch ollama. Aborting."
    exit 1
}

# Launch npm via cmd shim
$npmProc = Start-Process "cmd.exe" -ArgumentList "/c npm run start" `
    -RedirectStandardOutput $npmOut `
    -RedirectStandardError $npmErr `
    -PassThru

if (-not $npmProc) {
    Write-Host "Failed to launch npm. Killing ollama and aborting."
    Stop-Process -Id $ollamaProc.Id -Force
    exit 1
}

# Monitor both
while ($true) {
    Start-Sleep -Seconds 2

    $ollamaAlive = Get-Process -Id $ollamaProc.Id -ErrorAction SilentlyContinue
    $npmAlive    = Get-Process -Id $npmProc.Id -ErrorAction SilentlyContinue

    if (-not $ollamaAlive) {
		"`n## Ollama exited at $(Get-Date -Format 'HH:mm:ss') -- terminating npm" | Out-File $npmOut -Append
        Stop-Process -Id $npmProc.Id -Force
        break
    }

    if (-not $npmAlive) {
		"`n## npm exited at $(Get-Date -Format 'HH:mm:ss') -- terminating Ollama" | Out-File $ollamaOut -Append
        Stop-Process -Id $ollamaProc.Id -Force
        break
    }
}

# Final timestamps
"`n## Ended: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')" | Out-File $ollamaOut -Append
"`n## Ended: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')" | Out-File $npmOut -Append
