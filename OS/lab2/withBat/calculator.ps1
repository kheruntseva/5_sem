param (
    [switch]$Exit
)

if ($Exit) {
    exit
}

while ($true) {
    Write-Host "`nInteractive Calculator" -ForegroundColor Green
    Write-Host "Available operations: +, -, *, /, ^ (power), mod (remainder), sin, cos, tan, log (ln), log10, sqrt (root)" -ForegroundColor Yellow
    Write-Host "Examples: 2 + 3, 5 ^ 2, sin 0.5, sqrt 16, 10 mod 3" -ForegroundColor Yellow
    Write-Host "Enter expression (or 'exit' to quit):" -ForegroundColor Cyan

    $input = Read-Host
    if ($input -eq "exit") {
        break
    }

    try {
        # Разбиваем входные данные на части
        $parts = $input -split ' '
        if ($parts.Length -eq 0) {
            Write-Host "Error: Enter an expression." -ForegroundColor Red
            continue
        }

        $operation = $parts[0].ToLower()
        $num1 = $null
        if ($parts.Length -gt 1) {
            $num1 = [double]$parts[1]
        }
        $num2 = $null
        if ($parts.Length -gt 2) {
            $num2 = [double]$parts[2]
        }

        $result = $null

        # Проверяем однозначные функции
        if ($num1 -ne $null -and $num2 -eq $null -and $operation -in "sin", "cos", "tan", "log", "log10", "sqrt") {
            switch ($operation) {
                "sin" { $result = [math]::Sin($num1) }
                "cos" { $result = [math]::Cos($num1) }
                "tan" { $result = [math]::Tan($num1) }
                "log" { if ($num1 -le 0) { throw "Logarithm undefined for non-positive numbers" } $result = [math]::Log($num1) }
                "log10" { if ($num1 -le 0) { throw "Logarithm undefined for non-positive numbers" } $result = [math]::Log10($num1) }
                "sqrt" { if ($num1 -lt 0) { throw "Square root undefined for negative numbers" } $result = [math]::Sqrt($num1) }
            }
        }
        # Проверяем бинарные операции
        elseif ($num1 -ne $null -and $num2 -ne $null -and $operation -in "+", "-", "*", "/", "^", "mod") {
            switch ($operation) {
                "+" { $result = $num1 + $num2 }
                "-" { $result = $num1 - $num2 }
                "*" { $result = $num1 * $num2 }
                "/" { if ($num2 -eq 0) { throw "Division by zero" } $result = $num1 / $num2 }
                "^" { $result = [math]::Pow($num1, $num2) }
                "mod" { $result = $num1 % $num2 }
            }
        }
        else {
            throw "Error: Invalid expression format. Use: operation number (e.g., sin 0.5) or number operation number (e.g., 2 + 3)."
        }

        Write-Host "Result: $result" -ForegroundColor Green
    }
    catch {
        Write-Host "Error: $($_.Exception.Message)" -ForegroundColor Red
    }
}
