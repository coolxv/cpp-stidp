# linux环境安装
- [安装LLVM套件](https://apt.llvm.org/)  bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
- 示例程序
```
#include<stdint.h>
#include<stddef.h>

bool VulnerableFunction1(const uint8_t* data, size_t size) {
  bool result = false;
  if (size >= 3) {
    result = data[0] == 'F' &&
             data[1] == 'U' &&
             data[2] == 'Z' &&
             data[3] == 'Z';
  }
  return result;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    VulnerableFunction1(data, size);
    return 0;
}

```
- 编译 clang++-11 -g -fsanitize=address,fuzzer  test.cpp

# windows环境安装
- [安装](https://releases.llvm.org/download.html)Pre-Built Binaries: Windows (64-bit) 
- 区别导入库和静态库：lib /list clang_rt.fuzzer-x86_64.lib
- 示例程序
```
#include<stdint.h>
#include<stddef.h>

bool VulnerableFunction1(const uint8_t* data, size_t size) {
  bool result = false;
  if (size >= 3) {
    result = data[0] == 'F' &&
             data[1] == 'U' &&
             data[2] == 'Z' &&
             data[3] == 'Z';
  }
  return result;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    VulnerableFunction1(data, size);
    return 0;
}

```
- 编译 clang++ test.cpp  -o test.exe -g -std=c++11 -fsanitize=address -fsanitize-coverage=trace-pc -Xlinker /libpath:. -Xlinker /wholearchive:clang_rt.fuzzer-x86_64.lib