project(
    'sdl3_mixer',
    'c',
    version: 'pre-3.0.0'
)

sdl3 = dependency('sdl3')

sources = files(
    'src/codecs/load_aiff.c',
    'src/codecs/load_voc.c',
    'src/codecs/load_sndfile.c',
    'src/codecs/mp3utils.c',
    'src/codecs/music_drflac.c',
    'src/codecs/music_flac.c',
    'src/codecs/music_fluidsynth.c',
    'src/codecs/music_gme.c',
    'src/codecs/music_minimp3.c',
    'src/codecs/music_modplug.c',
    'src/codecs/music_mpg123.c',
    'src/codecs/music_nativemidi.c',
    'src/codecs/music_ogg.c',
    'src/codecs/music_ogg_stb.c',
    'src/codecs/music_opus.c',
    'src/codecs/music_timidity.c',
    'src/codecs/music_wav.c',
    'src/codecs/music_wavpack.c',
    'src/codecs/music_xmp.c',
    'src/effect_position.c',
    'src/effect_stereoreverse.c',
    'src/effects_internal.c',
    'src/mixer.c',
    'src/music.c',
    'src/utils.c'
)

incdirs = [
    'src',
    'src/codecs',
    'include'
]

sdl3_mixer_lib = static_library(
    meson.project_name(),
    sources,
    include_directories: incdirs,
    dependencies: sdl3
)

sdl3_mixer_dep = declare_dependency(
    include_directories: incdirs,
    link_with: sdl3_mixer_lib
)

meson.override_dependency('sdl3_mixer', sdl3_mixer_dep)
