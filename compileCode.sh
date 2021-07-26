echo ">>>>> Data File Creator"
g++ Code/CreateDataFiles/main.cpp -o Bin/createDataFiles
echo ">>>>> Complete"

echo ">>>>> Result Files (SP800-90B) Merge"
g++ Code/MergeSP80090B_Output/main.cpp -o Bin/MergeSP80090B_Output
echo ">>>>> Complete"

echo ">>>>> Estimate Entropy"
g++ -c -Wall -fpic Code/EntropyEstimation/SP800-90B_EntropyAssessment/cpp/non_iid_main.cpp
g++ -shared -o libnon_iid_main.so non_iid_main.o
g++ -L. -Wl,-rpath=. -Wall Code/EntropyEstimation/main.cpp -o EntropyEstimation -lnon_iid_main -lpthread -ldivsufsort
mv EntropyEstimation Bin/
mv libnon_iid_main.so Bin/
rm non_iid_main.o
echo ">>>>> Complete"