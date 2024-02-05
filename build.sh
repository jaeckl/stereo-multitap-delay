rm -rf build
cmake . -B build -DCMAKE_PREFIX_PATH=~/JUCE -DCMAKE_BUILD_TYPE=Debug;
cmake --build build -j8;

