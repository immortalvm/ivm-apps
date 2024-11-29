set -e
export INPUT_FILE="${1:-"../../../testdata/source/wav/CantinaBand3.wav"}"

cp $INPUT_FILE input.wav
ivm64-fsgen input.wav > ivmfs.c
rm input.wav
ivm64-gcc main.c -o main
rm ivmfs.c
ivm64-as main --bin main.b --sym /dev/null
rm main
mkdir -p wav
