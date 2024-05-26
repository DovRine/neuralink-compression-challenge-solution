# Paths to executables and test files
$encodeExec = "./encode.exe"
$decodeExec = "./decode.exe"
$dataDir = "./data"
$inputFile = "$dataDir/00d4f842-fc92-45f5-8cae-3effdc2245f5.wav"
$encodedFile = "output_encoded.txt"
$decodedFile = "output_decoded.wav"

# Manually set the original data size, sample rate, and bits per sample
$dataSize = 441000
$sampleRate = 44100
$bitsPerSample = 16

# Encode the WAV file
& $encodeExec $inputFile $encodedFile

# Decode the encoded file
& $decodeExec $encodedFile $decodedFile $dataSize $sampleRate $bitsPerSample

# Compare the original and decoded files
if (Compare-Object -ReferenceObject (Get-Content $inputFile) -DifferenceObject (Get-Content $decodedFile)) {
    Write-Host "ERROR: The files are different!"
} else {
    Write-Host "SUCCESS: The files are identical!"
}
