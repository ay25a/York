rm -rf build/
rm -rf bin/

echo -e "\n============== Configuring Debug ==============\n"
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug

echo -e "\n\n============== Configuring Release ==============\n"
cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release

echo -e "\n\n============== Configuring RelWithDebInfo ==============\n"
cmake -S . -B build/relwithdebinfo -DCMAKE_BUILD_TYPE=RelWithDebInfo

echo -e "\n============== Using Debug compile commands =============="
ln -sf debug/compile_commands.json build/compile_commands.json
