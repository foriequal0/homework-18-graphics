### How to build

```sh
mkdir build && cd build
cmake ..
make
./snu_graphics
# to visualize local frame axis
export AXIS=1
./snu_graphics
```

### Requirements

 * GCC/Clang (with c++ 14 support)
 * OpenGL 1.5
 * FreeGLUT
 * git
* CMAKE >= 3.8
 * 인터넷 연결 (Github 에서 eigen 외부 라이브러리 의존성을 자동으로 다운받기 때문에)