import os
from dataclasses import dataclass

@dataclass
class MesonBuildFile:
    present: str
    cached: str

def was_updated(meson: MesonBuildFile) -> bool:
    modiftime_present = os.path.getmtime(meson.present)
    modiftime_cached = os.path.getmtime(meson.cached)

    with open(meson.present, 'rb') as f1, open(meson.cached, 'rb') as f2:
        return modiftime_present > modiftime_cached and f1.read() != f2.read()

def update(meson: MesonBuildFile) -> None:
    with open(meson.present, 'rb') as present, open(meson.cached, 'wb') as cached:
        cached.write(present.read())
        print(f'Updated {meson.cached}.')

def main():
    excluded_folders = ['packagecache', 'packagefiles']
    os.chdir('subprojects')
    
    mesonbuilds = [
        MesonBuildFile(
            present=os.path.join(entry, 'meson.build'),
            cached=os.path.join('packagefiles', entry.split('-', 1)[0], 'meson.build')
        )
        for entry in os.listdir()
        if os.path.isdir(entry) and entry not in excluded_folders
    ]

    mesonbuilds_with_overrides = [
        meson for meson in mesonbuilds
        if os.path.exists(meson.cached)
    ]
    
    needed_to_update = [
        meson for meson in mesonbuilds_with_overrides
        if was_updated(meson)
    ]

    if len(needed_to_update) > 0:
        [update(meson) for meson in needed_to_update]

    else:
        print('Nothing needs to be updated.')

if __name__ == '__main__':
    main()
