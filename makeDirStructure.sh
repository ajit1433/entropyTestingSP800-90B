# Create directory structure required for running the tests

ROOT_DIR="./ExperimentData"
RAW_DATA_STORE="./RawDataStore"
DATA_STORE="./DataStore"
ENTROPY_ESTIMATE_STORE="./SP80090B_Output"
RESULTS="./Results"

ATECC_DIR="./atecc508a"
URANDOM_DIR="./urandom"
SHA2_256_DIR="./sha2_256"
SHA3_256_DIR="./sha3_256"
S01_50_50_DIR="./01_50_50"
S01_25_75_DIR="./01_25_75"
S01_125_875_DIR="./01_125_875"

# make root dir
mkdir $ROOT_DIR

#make base directories
cd $ROOT_DIR
mkdir $RAW_DATA_STORE
mkdir $DATA_STORE
mkdir $ENTROPY_ESTIMATE_STORE
mkdir $RESULTS

# make data store
cd $DATA_STORE
mkdir $ATECC_DIR
mkdir $URANDOM_DIR
mkdir $SHA2_256_DIR
mkdir $SHA3_256_DIR
mkdir $S01_50_50_DIR
mkdir $S01_25_75_DIR
mkdir $S01_125_875_DIR
cd ..

# make entropy store
cd $ENTROPY_ESTIMATE_STORE
mkdir $ATECC_DIR
mkdir $URANDOM_DIR
mkdir $SHA2_256_DIR
mkdir $SHA3_256_DIR
mkdir $S01_50_50_DIR
mkdir $S01_25_75_DIR
mkdir $S01_125_875_DIR
cd ..


cd ..

echo "complete..."
