# SDL3 Game Base

## コマンド

```console
cmake -B build -G Ninja `
 -DCMAKE_CXX_COMPILER=clang++ `
 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON `
 -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
```

```console
cmake --build build
```
