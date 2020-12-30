#pragma once
#include<iostream>
#include<string>
#include<complex>
#include<array>
#include<list>
#include<map>
#include<tuple>
#include<typeinfo>
#include<type_traits>
#include<mutex>
#include<algorithm>

#include<vector>
#include <utility>
#include <memory>
#include <atomic>
#include <thread>
#if __has_include(<filesystem>)
# include <filesystem>
# define HAS_FILESYSTEM 1
#elif __has_include(<experimental/filesystem>)
# include <experimental/filesystem>
# define HAS_FILESYSTEM 1
# define FILESYSTEM_IS_EXPERIMENTAL 1
#elif __has_include("filesystem.hpp")
# include "filesystem.hpp"
# define HAS_FILESYSTEM 1
# define FILESYSTEM_IS_EXPERIMENTAL 1
#else
# define HAS_FILESYSTEM 0
#endif


#ifndef _MSC_VER
#include <cxxabi.h>
#endif

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


namespace ns_Mandatory_Copy_Elision_or_Passing_Unmaterialized_Objects
{
    template <typename T, typename... Args>
    T create(Args&&... args)
    {
        return T{std::forward<Args>(args)...};
    }
}


namespace ns_New_Attributes_and_Attribute_Features
{
    namespace [[deprecated]] DraftAPI {

    }
    enum class City { 
        Berlin = 0,
        NewYork = 1,
        Mumbai = 2,
        Bombay [[deprecated]] = Mumbai,
    };
}



namespace ns_Other_Language_Features {
    namespace Nested_Namespaces::C
    {
        void print()
        {
            std::cout << "Nested Namespaces" << std::endl;
        }
    }
    //自从 C++17 之后，异常处理声明变成了函数类型的一部分。也就是说，如下的两个函数的类型是不同的：
    inline void fMightThrow()
    {

    }
    inline void fNoexcept() noexcept
    {

    }
    //重载一个签名完全相同只有异常声明不同的函数是不允许的（就像不允许重载只有返回值不同的函数一样）：
    void f33();
    //void f33(); noexcept; // ERROR
    void f1();
    void f2() noexcept;
    void f3() noexcept(sizeof(int)<4); // 和f1()或f2()的 类 型 相 同
    void f4() noexcept(sizeof(int)>=4); // 和f3()的 类 型 不 同
    void f5() throw(); // 和void f5() noexcept等 价 但 已 经 被 废 弃
    //带参数的动态异常声明不再被支持（自从 C++11起被废弃）：
    //void f6() throw(std::bad_alloc); // ERROR： 自 从C++17起 无 效

    //单参数 static_assert
    template<typename T>
    class C {
        // 自 从C++11起OK
        static_assert(std::is_default_constructible<T>::value, "class C: elements must be default-constructible");
        // 自 从C++17起OK
        static_assert(std::is_default_constructible_v<T>);
    };

    class CC {

    };
   
}

namespace ns_Class_Template_Argument_Deduction {
    template<typename T, int SZ>
    class MyClass {
    public:
        MyClass (T(&)[SZ]) {
        }
    };

    template<typename... Args>
    auto make_vector(const Args&... elems) {
        return std::vector{elems...};
    }

    template<typename CB>
    class CountCalls
    {
    private:
        CB callback; // 要 调 用 的 回 调 函 数
        long calls = 0; // 调 用 的 次 数
    public:
        CountCalls(CB cb) : callback(cb) {
        }
        template<typename... Args>
        decltype(auto) operator() (Args&&... args) {
            ++calls;
            return callback(std::forward<Args>(args)...);
        }
        long count() const {
            return calls;
        }
};



}