
# 编译
```
objcopy -I binary -O elf64-x86-64 -B i386 strings.txt strings.o
nm strings.o
gcc main.c -c
gcc main.o  strings.o -o main
 ```

 # 参考
 + [objcopy](https://man7.org/linux/man-pages/man1/objcopy.1.html)




