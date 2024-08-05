export CC="zig cc"
export CXX="zig c++"
CORECOUNT=$( nproc )

make_dependency() {
    cd build
    make all -j $CORECOUNT
    cd ../..
}

# ┏━━━━━┓
# ┃ SDL ┃
# ┗━━━━━┛

git clone https://github.com/libsdl-org/SDL.git
cd SDL
git reset --hard 537c62f

cmake -B ./build                  \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DSDL_STATIC=ON               \
    -DSDL_SHARED=OFF              \
    -DSDL_DBUS=OFF                \
    -DSDL_TEST_LIBRARY=OFF

make_dependency

# ┏━━━━━━━━━━━┓
# ┃ SDL_image ┃
# ┗━━━━━━━━━━━┛

git clone https://github.com/libsdl-org/SDL_image.git
cd SDL_image
git reset --hard ff62eb6

git submodule init external/zlib
git submodule update external/zlib

git submodule init external/libpng
git submodule update external/libpng

cmake -B ./build                  \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DBUILD_SHARED_LIBS=OFF       \
    -DSDL3_DIR="../SDL/build"     \
    -DSDL3IMAGE_DEPS_SHARED=OFF   \
    -DSDL3IMAGE_PNG=ON            \
    -DSDL3IMAGE_VENDORED=ON       \
    -DSDL3IMAGE_STRICT=ON         \
    -DSDL3IMAGE_SAMPLES=OFF       \
    -DSDL3IMAGE_TESTS=OFF         \
    -DSDL3IMAGE_AVIF=OFF -DSDL3IMAGE_BACKEND_STB=OFF -DSDL3IMAGE_BMP=OFF -DSDL3IMAGE_GIF=OFF -DSDL3IMAGE_JPG=OFF -DSDL3IMAGE_JXL=OFF \
    -DSDL3IMAGE_LBM=OFF -DSDL3IMAGE_PCX=OFF -DSDL3IMAGE_PNM=OFF -DSDL3IMAGE_QOI=OFF -DSDL3IMAGE_SVG=OFF -DSDL3IMAGE_TGA=OFF          \
    -DSDL3IMAGE_TIF=OFF -DSDL3IMAGE_WEBP=OFF -DSDL3IMAGE_XCF=OFF -DSDL3IMAGE_XPM=OFF -DSDL3IMAGE_XV=OFF

make_dependency

# ┏━━━━━━━━━━━┓
# ┃ SDL_mixer ┃
# ┗━━━━━━━━━━━┛

git clone https://github.com/libsdl-org/SDL_mixer.git
cd SDL_mixer
git reset --hard ace2d37

git submodule init external/mpg123
git submodule update external/mpg123

cmake -B ./build                  \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DBUILD_SHARED_LIBS=OFF       \
    -DSDL3_DIR="../SDL/build"     \
    -DSDL3MIXER_DEPS_SHARED=OFF   \
    -DSDL3MIXER_MP3_MPG123=ON     \
    -DSDL3MIXER_VENDORED=ON       \
    -DSDL3MIXER_STRICT=ON         \
    -DSDL3MIXER_SAMPLES=OFF       \
    -DSDL3MIXER_FLAC=OFF -DSDL3MIXER_GME=OFF -DSDL3MIXER_MP3_MINIMP3=OFF -DSDL3MIXER_OPUS=OFF -DSDL3MIXER_VORBIS=OFF \
    -DSDL3MIXER_WAVE=OFF -DSDL3MIXER_WAVPACK=OFF -DSDL3MIXER_MOD=OFF -DSDL3MIXER_CMD=OFF -DSDL3MIXER_MIDI=OFF

make_dependency

# ┏━━━━━━━━━┓
# ┃ SDL_ttf ┃
# ┗━━━━━━━━━┛

git clone https://github.com/libsdl-org/SDL_ttf.git
cd SDL_ttf
git reset --hard 2227f7a

git submodule init external/harfbuzz
git submodule update external/harfbuzz

git submodule init external/freetype
git submodule update external/freetype

cmake -B ./build                  \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DBUILD_SHARED_LIBS=OFF       \
    -DSDL3_DIR="../SDL/build"     \
    -DSDL3TTF_HARFBUZZ=ON         \
    -DSDL3TTF_VENDORED=ON         \
    -DSDL3TTF_SAMPLES=OFF

make_dependency

# ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
# ┃ Relocate Libraries and Headers ┃
# ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

mv SDL/build/libSDL3.a lib/libSDL3.a
mv SDL/include/SDL3 hdr/SDL3

mv SDL_image/build/libSDL3_image.a lib/libSDL3_image.a
mv SDL_image/build/external/zlib/libz.a lib/libz.a
mv SDL_image/build/external/libpng/libpng16.a lib/libpng16.a
mv SDL_image/include/SDL3_image hdr/SDL3_image

mv SDL_mixer/build/libSDL3_mixer.a lib/libSDL3_mixer.a
mv SDL_mixer/build/external/mpg123/ports/cmake/src/libmpg123/libmpg123.a lib/libmpg123.a
mv SDL_mixer/include/SDL3_mixer hdr/SDL3_mixer

mv SDL_ttf/build/libSDL3_ttf.a lib/libSDL3_ttf.a
mv SDL_ttf/build/external/freetype/libfreetype.a lib/libfreetype.a
mv SDL_ttf/build/external/harfbuzz/libharfbuzz.a lib/libharfbuzz.a
mv SDL_ttf/include/SDL3_ttf hdr/SDL3_ttf

# ======= #
# Cleanup #
# ======= #

rm SDL -rf
rm SDL_image -rf
rm SDL_mixer -rf
rm SDL_ttf -rf