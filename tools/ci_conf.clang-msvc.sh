source tools/ci_conf_wincommon.sh

run export CFLAGS='-m32 -Xclang -target-cpu -Xclang x86-64 -fms-compatibility-version=18 -imsvc /mnt/msvc/vc12/include -imsvc /mnt/msvc/vc12/win_sdk/include/um -imsvc /mnt/msvc/vc12/win_sdk/include/shared'
run export CXXFLAGS="$CFLAGS -DDOCTEST_CONFIG_COLORS_ANSI"
run export LINKFLAGS='/libpath:/mnt/msvc/vc12/lib /libpath:/mnt/msvc/vc12/win_sdk/lib/winv6.3/um/x86'
run export WINRCFLAGS=-m32
