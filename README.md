# entropyTestingSP800-90B

>> create directory structure for the test
chmod +x makeDirStructure.sh
./makeDirStructure.sh

>> copy following files to ExperimentData/RawDataStore
  atecc50a_full.bin
  urandom_full.bin
  sha2_256_full.bin
  sha3_256_full.bin

>> run ./compileCode.sh to compile all required binaries

>> do
  chmod +x Bin/createDataFiles
  cd Bin/
  ./createDataFiles 