#pragma once
#include<string>
#include<array>
#include<vector>
#include<list>
#include<map>
#include<tuple>

namespace ns_Inline_Variables
{
    struct MyClass01 {
        inline static std::string msg{"OK"}; // OK（自C++17起）
    };

    inline MyClass01 myGlobalObj; // 即 使 被 多 个CPP文 件 包 含 也OK
    //所有这些方法都会导致签名重载，可读性也会变差，使用该变量的方式也变得不同。另外，全局变量的初始化可能会推迟到第一次使用时。
    //你可以在一个 class/struct 的定义中初始化数字或枚举类型的常量静态成员
    struct MyClass02 {
        static const bool trace = false; // OK， 字 面 类 型
    };
    //你可以定义一个返回该值的 static 的成员函数
    struct MyClass03 {
        static std::string& getMsg() {
            static std::string msg{"OK"};
            return msg;
        }
    };
    //你可以定义一个返回 static 的局部变量的内联函数
    inline std::string& getMsg() {
        static std::string msg{"OK"};
        return msg;
    }
    //你可以使用变量模板（自 C++14 起）
    template<typename T = std::string>
    T myGlobalMsg{"OK"};
    //你可以为静态成员定义一个模板类
    template<typename = void>
    struct MyClassStatics
    {
        static std::string msg;
    };
    template<typename T>
    std::string MyClassStatics<T>::msg{"OK"};

    //注意你仍然必须确保在你初始化内联变量之前它们的类型必须是完整的
    struct MyType {
        int value;
        MyType(int i) : value{i} {
        }
        // 一 个 存 储 该 类 型 最 大 值 的 静 态 对 象
        static MyType max; // 这 里 只 能 进 行 声 明
    };
    inline  MyType MyType::max{10};

    //对于静态成员，constexpr 修饰符现在隐含着 inline。
    struct D {
        static constexpr int n = 5; // C++11/C++14: 声 明
                                    // 自 从C++17起: 定 义
    };
    //thread_local 你可以为每个线程创建一个内联变量
    inline thread_local std::vector<std::string> cache = {"hello", "world"}; // 每 个 线 程 都 有 一 份cac

}

namespace ns_Aggregate_Extensions
{
    template<typename T>
    struct AG : std::string, std::complex<T> {
        std::string data;
    };
}