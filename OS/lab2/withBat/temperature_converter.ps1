param (
    [string]$Temperature
)

function Convert-Temperature {
    param ([string]$tempStr)

    if (-not $tempStr) {
        Write-Host "Enter temperature in format: 100K (Kelvin), 100C (Celsius), 100F (Fahrenheit)" -ForegroundColor Red
        return
    }

    $value = [double]::Parse($tempStr -replace '[^0-9\.]')
    $unit = $tempStr -replace '[0-9\.]'

    switch ($unit.ToUpper()) {
        "K" {
            $celsius = $value - 273.15
            $fahrenheit = $celsius * 9/5 + 32
            Write-Host "$value K = $celsius C = $fahrenheit F" -ForegroundColor Green
        }
        "C" {
            $kelvin = $value + 273.15
            $fahrenheit = $value * 9/5 + 32
            Write-Host "$value C = $kelvin K = $fahrenheit F" -ForegroundColor Green
        }
        "F" {
            $celsius = ($value - 32) * 5/9
            $kelvin = $celsius + 273.15
            Write-Host "$value F = $celsius C = $kelvin K" -ForegroundColor Green
        }
        default {
            Write-Host "Invalid unit format. Use K, C, or F." -ForegroundColor Red
        }
    }
}

if ($Temperature) {
    Convert-Temperature -tempStr $Temperature
} else {
    while ($true) {
        $tempInput = Read-Host "Enter temperature (or 'exit' to quit)"
        if ($tempInput -eq "exit") { break }
        Convert-Temperature -tempStr $tempInput
    }
}
