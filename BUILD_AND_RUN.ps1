# build C++, the launch Godot project

# Navigate to your project directory (uncomment if needed)
# cd C:\Users\Richard\Documents\GitHub\Godot\EEGodotCPP4

$GodotPath = 'C:\Program Files\Godot\Godot_v4.4.1-stable_win64\Godot_v4.4.1-stable_win64.exe'

Write-Host "--- Running SCons build ---"


# Execute scons and capture its exit code

# scons # WARNING! unwise
scons platform=windows target=template_debug # we need debug for the editor
# scons platform=windows target=template_release # CHANGED... specify windows non-debug



# Check the exit code of the last command (scons)
if ($LASTEXITCODE -ne 0) {
    Write-Error "SCons build failed with exit code: $LASTEXITCODE. Aborting script."
    # [Console]::Beep(440, 500) # Frequency (Hz), Duration (ms)
    exit $LASTEXITCODE # Exit the script with the scons error code
}

Write-Host "--- SCons build successful ---"

# Launch Godot project if scons succeeded
& $GodotPath .\demo\project.godot
# & $GodotPath .\demo\project.godot --verbose # verbose