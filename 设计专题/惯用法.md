

# 创建一个重载的 lambda的集合

```
// CPP17
// "inherit" all function call operators of passed base types:
template<typename... Ts>
struct overload : Ts...
{
    using Ts::operator()...;
};

// base types are deduced from passed arguments:
template<typename... Ts>
overload(Ts...) -> overload<Ts...>;

```
```
auto twice = overload {
    [](std::string& s) { s += s; },
    [](auto& v) { v *= 2; }
};

int i = 42;
twice(i);
std::cout << "i: " << i << '\n'; // 打 印 出：84
std::string s = "hi";
twice(s);
std::cout << "s: " << s << '\n'; // 打 印 出：hihi

```

参考
======

