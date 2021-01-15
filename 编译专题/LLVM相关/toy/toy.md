# 项目
https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html
https://github.com/llvm-mirror/llvm/tree/master/examples/Kaleidoscope


# clang11 编译运行(on Ubuntu 20.04) 
```
clang++ -g -O3 toy.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -o toy

./toy
ready> def foo(x y) x+foo(y, 4.0);
ready> Read function definition:define double @foo(double %x, double %y) {
entry:
  %calltmp = call double @foo(double %y, double 4.000000e+00)
  %addtmp = fadd double %x, %calltmp
  ret double %addtmp
}

```
