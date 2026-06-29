project('avd-fw', 'c', default_options: ['c_std=c11'], meson_version: '>=1.4.0', version: '0.1')

# cross files are needed as those are the only way to set default compiler
# and specify the host arch
if not meson.is_cross_build()
    error('must be invoked with "meson setup --cross-file=./llvm.ini"')
endif

add_project_arguments(['-nostdlib', '-mcpu=cortex-m3', '-mthumb'], language : 'c')
add_project_link_arguments('-nostdlib', language : 'c')

# set target for clang build
if meson.get_compiler('c').get_id() == 'clang'
    add_project_arguments('--target=arm-none-eabi', language : 'c')
    add_project_link_arguments('--target=arm-none-eabi', language : 'c')
endif

firmware_dir = get_option('libdir') / 'firmware/apple'

objcopy = find_program('llvm-objcopy')

sources = files(['src/avd.c', 'src/irq.c', 'src/util.c', 'src/piodma.c'])
ld_scripts = files('avd-cm3.ld')

avd_variants = [
    { 'name': 'avd-fw-v2-t0', 'version': '2', 'tier': '0', 'pad': '0xc000'},
    { 'name': 'avd-fw-v3-t0', 'version': '3', 'tier': '0', 'pad': '0x10000'},
    { 'name': 'avd-fw-v3-t1', 'version': '3', 'tier': '1', 'pad': '0x10000'},
    { 'name': 'avd-fw-v4-t0', 'version': '4', 'tier': '0', 'pad': '0x12000'},
    { 'name': 'avd-fw-v5-t0', 'version': '5', 'tier': '0', 'pad': '0x10000'},
    { 'name': 'avd-fw-v5-t1', 'version': '5', 'tier': '1', 'pad': '0x12000'},
]

foreach var : avd_variants
    e = executable(var['name'] + '.elf',
        sources,
        c_args: ['-DAVD_VER=' + var['version'], '-DAVD_TIER=' + var['tier']],
        link_args: ['-T', ld_scripts[0].full_path()]
    )

    if meson.version().version_compare('>=1.10.0') == true
        custom_target(
            output: var['name'] + '.bin',
            build_subdir: 'apple',
            input: e,
            command: [objcopy, '-O', 'binary',  '--strip-debug', '--pad-to=' + var['pad'], '@INPUT0@', '@OUTPUT0@'],
            install: true,
            install_dir: firmware_dir
        )
    else
        custom_target(
            output: var['name'] + '.bin',
            input: e,
            command: [objcopy, '-O', 'binary',  '--strip-debug', '--pad-to=' + var['pad'], '@INPUT0@', '@OUTPUT0@'],
            install: true,
            install_dir: firmware_dir
        )
    endif
endforeach
