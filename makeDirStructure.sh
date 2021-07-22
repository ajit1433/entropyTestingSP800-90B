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
cd ..

# make entropy store
cd $ENTROPY_ESTIMATE_STORE
mkdir $ATECC_DIR
mkdir $URANDOM_DIR
mkdir $SHA2_256_DIR
mkdir $SHA3_256_DIR
cd ..


cd ..

echo "complete..."
