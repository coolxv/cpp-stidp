#pragma once
#include<iostream>
#include<string>
#include<complex>
#include<array>
#include<set>
#include<list>
#include<map>
#include<unordered_set>
#include<unordered_map>
#include<tuple>
#include<typeinfo>
#include<type_traits>
#include<mutex>
#include<algorithm>
#include<functional>
#include<iterator>
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

    template<typename T1, typename T2, typename T3 = T2>
    class C
    {
        public:
            C (T1 x = {}, T2 y = {}, T3 z = {}) {

            }

    };

    class Cust
    {
        std::string name_;
        public:
            Cust(std::string name):name_(name){}
            std::string getName() const { return name_; }

    };

    template<typename T1, typename T2>
    struct Pair3 {
        T1 first;
        T2 second;
        Pair3(const T1& x, const T2& y) : first{x}, second{y} { }
    };
    // 为 构 造 函 数 定 义 的 推 导 指 引
    template<typename T1, typename T2> 
    Pair3(T1, T2) -> Pair3<T1, T2>;

    template<typename T>
    struct S {
        T val;
    };
    S(const char*) -> S<std::string>; // 把S<字 符 串 字 面 量>映 射 为S<std::string>

    template<typename T>
    struct SS {
        T val;
    };
    explicit SS(const char*) -> SS<std::string>;

    template<typename T>
    struct Ptr
    {
        Ptr(T) { std::cout << "Ptr(T)\n"; }
        template<typename U>
        Ptr(U) { std::cout << "Ptr(U)\n"; }
    };
    template<typename T>
    explicit Ptr(T) -> Ptr<T*>;

    template<typename T>
    struct A {
        T val;
    };

    A(const char*) -> A<std::string>;
}

namespace ns_Compile_Time_if
{
    template <typename T>
    std::string asString(T x)
    {
        if constexpr(std::is_same_v<T, std::string>) {
            return x; // 如 果T不 能 自 动 转 换 为string该 语 句 将 无 效
        }
        else if constexpr(std::is_arithmetic_v<T>) {
            return std::to_string(x); // 如 果T不 是 数 字 类 型 该 语 句 将 无 效
        }
        else {
            return std::string(x); // 如 果 不 能 转 换 为string该 语 句 将 无 效
        }
    }

    template<typename T>
    constexpr auto bar(const T& val)
    {
        if constexpr (std::is_integral<T>::value && T{} < 10) {
            return val * 2;
        }
        return val;
    }

    template<typename Callable, typename... Args>
    decltype(auto) call(Callable op, Args&&... args)
    {
        if constexpr(std::is_void_v<std::invoke_result_t<Callable, Args...>>) {
            // 返 回 值 类 型 是void：
            op(std::forward<Args>(args)...);
            // 在 返 回 前 进 行 一 些 处 理
            return;
        }
        else {
            // 返 回 值 类 型 不 是void:
            decltype(auto) ret{op(std::forward<Args>(args)...)};
            // 在 返 回 前 用ret进 行 一 些 处 理
            return ret;
        }
    }

    template<typename Iterator, typename Distance>
    void advance_test(Iterator& pos, Distance n) {
        using cat = typename std::iterator_traits<Iterator>::iterator_category;
        if constexpr (std::is_convertible_v<cat, std::random_access_iterator_tag>) {
            pos += n;
        }
        else if constexpr (std::is_convertible_v<cat, std::bidirectional_iterator_tag>) {
            if (n >= 0) {
                while (n--) {
                    ++pos;
                }
            }
            else {
                while (n++) {
                    --pos;
                }
            }
        }
        else { // input_iterator_tag
            while (n--) {
                ++pos;
            }
        }
    }

}



namespace ns_Fold_Expressions
{
    //CPP17之前
    template<typename T>
    auto foldSumRec (T arg) {
        return arg;
    }
    template<typename T1, typename... Ts>
    auto foldSumRec (T1 arg1, Ts... otherArgs) {
        return arg1 + foldSumRec(otherArgs...);
    }
    //CPP17之后
    template<typename... T>
    auto foldSum (T... args) {
        return (... + args); // arg1 + arg2 + arg3...
    }

    template<typename T>
    const T& spaceBefore(const T& arg) {
        std::cout << ' ';
        return arg;
    }
    template <typename First, typename... Args>
    void print1 (const First& firstarg, const Args&... args) {
        std::cout << firstarg;
        (std::cout << ... << spaceBefore(args)) << std::endl;
    }

    template<typename First, typename... Args>
    void print2 (const First& firstarg, const Args&... args) {
        std::cout << firstarg;
        (std::cout << ... << [] (const auto& arg) -> decltype(auto) {
            std::cout << ' ';
            return arg;
        }(args)) << std::endl;
    }
    template<typename First, typename... Args>
    void print3(First first, const Args&... args) {
        std::cout << first;
        auto outWithSpace = [] (const auto& arg) {
            std::cout << ' ' << arg;
        };
        (... , outWithSpace(args));
        std::cout << std::endl;
    }

    template<typename T>
    void hashCombine (std::size_t& seed, const T& val)
    {
        seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    }
    template<typename... Types>
    std::size_t combinedHashValue (const Types&... args)
    {
        std::size_t seed = 0; // 初 始 化seed
        (... , hashCombine(seed, args)); // 链 式 调 用hashCombine()
        return seed;
    }
    
    template<typename... Bases>
    class MultiBase : private Bases...
    {
    public:
        void print() {
            // 调 用 所 有 基 类 的print()函 数
            (... , Bases::print());
        }
    };
    struct A {
        void print() { std::cout << "A::print()\n"; }
    };
    struct B {
        void print() { std::cout << "B::print()\n"; }
    };
    struct C {
        void print() { std::cout << "C::print()\n"; }
    };

    // 检 查 是 否 所 有 类 型 都 相 同：
    template<typename T1, typename... TN>
    struct IsHomogeneous {
        static constexpr bool value = (std::is_same_v<T1, TN> && ...);
    };
    // 检 查 是 否 所 有 传 入 的 参 数 类 型 相 同：
    template<typename T1, typename... TN>
    constexpr bool isHomogeneous(T1, TN...)
    {
        return (std::is_same_v<T1, TN> && ...);
    }

}



namespace ns_Dealing_with_Strings_as_Template_Parameters{
    template<const char* str>
    class Message {
    };

    template<int* p>
    struct A {
    };
    constexpr int* pNum(int *p) {
        return p;
    }

}
namespace ns_Dealing_Placeholder_Types_like_auto_as_Template_Parameters{
    template<auto N> class S { };
    template<int N> class S<N> { };
    template<typename T, auto N>
    class A {
    public:
        A(const std::array<T, N>&) { }
        A(T(&)[N]) { }
    };
    //你也可以修饰 auto，例如，可以确保参数类型必须是个指针：
    template<const auto* P> struct SS{};
    //另外，通过使用可变参数模板，你可以使用多个不同类型的模板参数来实例化模板：
    template<auto... VS> class HeteroValueList {
    };
    //也可以用多个相同类型的参数：
    template<auto V1, decltype(V1)... VS> class HomoValueList {
    };

    template<auto Sep = ' ', typename First, typename... Args>
    void print(const First& first, const Args&... args) {
        std::cout << first;
        auto outWithSep = [] (const auto& arg) {
            std::cout << Sep << arg;
        };
        (... , outWithSep(args));
        std::cout << std::endl;
    }
    template<auto v>
    struct constant
    {
        static constexpr auto value = v;
    };

    template<decltype(auto) N>
    struct Sp {
        void printN() const {
            std::cout << "N: " << N << '\n';
        }
    };

    template<typename T, auto N> std::array<T, N> arr{};
    template<auto N> constexpr auto val = N; // 自 从C++17起OK

}
namespace ns_Dealing_Extended_Using_Declarations{
    // 继 承 所 有 基 类 里 的 函 数 调 用 运 算 符
    template<typename... Ts>
    struct overload : Ts...
    {
        using Ts::operator()...;
    };
    // 基 类 的 类 型 从 传 入 的 参 数 中 推 导
    template<typename... Ts>
    overload(Ts...) -> overload<Ts...>;

    template<typename T>
    class Base {
    T value{};
    public:
        Base() {
        }
        Base(T v) : value{v} {
        }
    };
    template<typename... Types>
    class Multi : private Base<Types>...
    {
    public:
        // 继 承 所 有 构 造 函 数：
        using Base<Types>::Base...;

    };

}