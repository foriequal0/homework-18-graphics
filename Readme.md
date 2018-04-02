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

### Known problems

빌드에 LeakSanitizer를 포함하게 되어있는데, i965_dri.so 가 사용되는 시스템에서 빌드할 경우 프로그램 종료시에 LeakSanitizer의 누수경고가 출력된다. 기능상에 문제가 없으므로 무시하면 된다.

### Requirements

 * GCC/Clang (with c++ 14 support)
 * OpenGL 1.5
 * FreeGLUT
 * CMake >= 3.8
 * git  (Github 에서 eigen 의존성을 다운로드 받으며 사용한다)


 * 인터넷 연결 (Github 에서 eigen 의존성을 다운로드 받기 위해 필요하다)