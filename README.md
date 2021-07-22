## HEADING (to be added)

###### Create directory structure for the test
```
chmod +x makeDirStructure.sh
./makeDirStructure.sh
```

###### Copy following files to ExperimentData/RawDataStore
- atecc50a_full.bin
- urandom_full.bin
- sha2_256_full.bin
- sha3_256_full.bin

###### Compile code to generate required binaries
```
chmod +x compileCode.sh
./compileCode.sh
```

###### Create input files from Raw files
```
  chmod +x Bin/createDataFiles
  cd Bin/
  ./createDataFiles
```
