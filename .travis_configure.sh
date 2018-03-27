mkdir build && cd build
cmake -DCMAKE_C_COMPILER=$COMPILER ..
make
cd .. && rm -r ./build
