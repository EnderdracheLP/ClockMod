Param(
    [Parameter(Mandatory=$false, HelpMessage="The name the output qmod file should have")][String] $qmodname="ClockMod",

    [Parameter(Mandatory=$false, HelpMessage="Switch to create a clean compilation")]
    [Alias("rebuild")]
    [Switch] $clean,

    [Parameter(Mandatory=$false, HelpMessage="Switch to create a release build")]
    [Switch] $release,

    [Parameter(Mandatory=$false, HelpMessage="Prints the help instructions")]
    [Switch] $help,

    [Parameter(Mandatory=$false, HelpMessage="Tells the script to not compile and only package the existing files")]
    [Alias("actions", "pack")]
    [Switch] $package
)

# Builds a .qmod file for loading with QP or MBF


if ($help -eq $true) {
    echo "`"BuildQmod <qmodName>`" - Copiles your mod into a `".so`" or a `".a`" library"
    echo "`n-- Parameters --`n"
    echo "qmodName `t The file name of your qmod"

    echo "`n-- Arguments --`n"

    echo "-clean `t`t Performs a clean build on both your library and the qmod"
    echo "-help `t`t Prints this"
    echo "-package `t Only packages existing files, without recompiling`n"

    exit
}

if ($qmodName -eq "")
{
    echo "Give a proper qmod name and try again"
    exit
}

if (($args.Count -eq 0 -or $dev -eq $true) -And $package -eq $false) {
echo "Packaging QMod $qmodName"
    & $PSScriptRoot/build.ps1 -clean:$clean --release:$release

    if ($LASTEXITCODE -ne 0) {
        echo "Failed to build, exiting..."
        exit $LASTEXITCODE
    }
}

$qpmshared = "./qpm.shared.json"
$qpmsharedJson = Get-Content $qpmshared -Raw | ConvertFrom-Json

# if ([string]::IsNullOrEmpty($env:version)) {
#     $qmodVersion = $qpmsharedJson.config.info.version
#     $qmodName += "_v$qmodVersion"
#     echo "qmodName set to $qmodName"
# }


if ($package -eq $true -And $env:version.Contains('-Dev')) {
    $qmodName = "$($env:module_id)_$($env:version)"
echo "Actions: Packaging QMod $qmodName"
} elseif ($package -eq $true) {
        $qmodName = "$($env:module_id)"
echo "Actions: Packaging QMod $qmodName"
} else {
    $qmodName += "_$($qpmsharedJson.config.info.version)"
}

$qmod = $qmodName + ".qmod"

qpm qmod zip -i ./build/ -i ./extern/libs/ $qmod

echo "Task Completed"