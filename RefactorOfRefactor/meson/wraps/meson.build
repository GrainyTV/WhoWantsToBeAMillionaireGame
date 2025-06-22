dependencies = []

dependency_ids = [
    'tomlplusplus',
    'glm',
    'sdl3',
    'sdl3_image',
    'sdl3_mixer',
    'sdl3_ttf',
    'fmt',
    'cpptrace',
]

foreach id : dependency_ids
    subproject(id)
    dependencies += dependency(id)
endforeach
