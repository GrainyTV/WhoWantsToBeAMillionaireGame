CC="zig cc"
CXX="zig c++"
CORECOUNT=$( nproc )

# ========= #
# SDL Build #
# ========= #

git clone https://github.com/libsdl-org/SDL.git
cd SDL

CC=$CC CXX=$CXX cmake             \
    -B ./build                    \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DSDL_STATIC=ON               \
    -DSDL_SHARED=OFF              \
    -DSDL_TEST_LIBRARY=OFF

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
git submodule init external/mpg123
git submodule update external/mpg123

CC=$CC CXX=$CXX cmake   \
    -B ./build                    \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DBUILD_SHARED_LIBS=OFF       \
    -DSDL3_DIR="../SDL/build"     \
    -DSDL3MIXER_DEPS_SHARED=OFF   \
    -DSDL3MIXER_MP3_MPG123=ON     \
    -DSDL3MIXER_VENDORED=ON       \
    -DSDL3MIXER_STRICT=ON         \
    -DSDL3MIXER_FLAC=OFF          \
    -DSDL3MIXER_GME=OFF           \
    -DSDL3MIXER_MP3_MINIMP3=OFF   \
    -DSDL3MIXER_OPUS=OFF          \
    -DSDL3MIXER_VORBIS=OFF        \
    -DSDL3MIXER_WAVE=OFF          \
    -DSDL3MIXER_WAVPACK=OFF       \
    -DSDL3MIXER_MOD=OFF           \
    -DSDL3MIXER_CMD=OFF           \
    -DSDL3MIXER_MIDI=OFF          \
    -DSDL3MIXER_SAMPLES=OFF

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