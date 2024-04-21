git clone --depth 1 --branch $( curl -s https://api.github.com/repos/libsdl-org/SDL/releases | jq -r '.[0].tag_name' ) https://github.com/libsdl-org/SDL.git
cd SDL
CC="zig cc" CXX="zig c++" cmake -DSDL_STATIC=ON -DSDL_SHARED=OFF -B ./build
cd build
make all
mv libSDL3.a ../../lib/libSDL3.a
mv ../include/SDL3 ../../hdr/SDL3
cd ../..
rm SDL -r