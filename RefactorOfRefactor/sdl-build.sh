CC="zig cc"
CXX="zig c++"
CORECOUNT=$( nproc )

# ========= #
# SDL Build #
# ========= #

git clone --depth 1 --branch $( curl -s https://api.github.com/repos/libsdl-org/SDL/releases | jq -r '.[0].tag_name' ) https://github.com/libsdl-org/SDL.git
cd SDL
CC="zig cc" CXX="zig c++" cmake -DSDL_STATIC=ON -DSDL_SHARED=OFF -B ./build
cd build
make all -j $CORECOUNT
cd ../..

# =============== #
# SDL_image Build #
# =============== #

git clone https://github.com/libsdl-org/SDL_image.git
cd SDL_image
CC="zig cc" CXX="zig c++" cmake -DBUILD_SHARED_LIBS=OFF -DSDL3_DIR="../SDL/build" -B ./build
cd build
make all -j $CORECOUNT
cd ../..

# =============== #
# SDL_mixer Build #
# =============== #

git clone https://github.com/libsdl-org/SDL_mixer.git
cd SDL_mixer
CC="zig cc" CXX="zig c++" cmake -DBUILD_SHARED_LIBS=OFF -DSDL3_DIR="../SDL/build" -B ./build
cd build
make all -j $CORECOUNT
cd ../..

# ============= #
# SDL_ttf Build #
# ============= #

git clone https://github.com/libsdl-org/SDL_ttf.git
cd SDL_ttf/external
source download.sh

CC=$CC CXX=$CXX cmake             \
    -B ./build                    \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DBUILD_SHARED_LIBS=OFF       \
    -DSDL3_DIR="../SDL/build"     \
    -DSDL3TTF_HARFBUZZ=ON         \
    -DSDL3TTF_VENDORED=ON         \
    -DSDL3TTF_SAMPLES=OFF       

cd build
make all -j $CORECOUNT
cd ../..

# ============================== #
# Relocate Libraries and Headers #
# ============================== #

mv SDL/build/libSDL3.a lib/libSDL3.a
mv SDL/include/SDL3 hdr/SDL3

mv SDL_image/build/libSDL3_image.a lib/libSDL3_image.a
mv SDL_image/include/SDL3_image hdr/SDL3_image

mv SDL_mixer/build/libSDL3_mixer.a lib/libSDL3_mixer.a
mv SDL_mixer/include/SDL3_mixer hdr/SDL3_mixer

mv SDL_ttf/build/libSDL3_ttf.a lib/libSDL3_ttf.a
mv SDL_ttf/include/SDL3_ttf hdr/SDL3_ttf

# ======= #
# Cleanup #
# ======= #

rm SDL -rf
rm SDL_image -rf
rm SDL_mixer -rf
rm SDL_ttf -rf