#!/usr/bin/env python3

import sys
import os
import shutil

def main():
    assert len(sys.argv[1:]) == 1
    
    target_dir = sys.argv[1]
    assert os.path.exists(target_dir) and os.path.isdir(target_dir)
    
    source_dir = os.getenv('MESON_SOURCE_ROOT')
    assert source_dir is not None

    build_dir = os.getenv('MESON_BUILD_ROOT')
    assert build_dir is not None

    to_copy = os.path.join(source_dir, target_dir)
    where_to = os.path.join(build_dir, target_dir)

    shutil.copytree(to_copy, where_to)

if __name__ == '__main__':
    main()
