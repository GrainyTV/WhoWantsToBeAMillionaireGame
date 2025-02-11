cmake = import('cmake')

cpptrace_proj = cmake.subproject('cpptrace')
cpptrace_dep = declare_dependency(
    dependencies: [
        cpptrace_proj.dependency('libzstd_static'),
        cpptrace_proj.dependency('dwarf'),
        cpptrace_proj.dependency('cpptrace-lib')
    ]
)

meson.override_dependency('cpptrace', cpptrace_dep)
