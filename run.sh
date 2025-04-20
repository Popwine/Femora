rm -rf build

mkdir build

cd build

cmake ..

cmake --build .

cd ../

./build/fem_sim