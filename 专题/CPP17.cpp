
#include "CPP17.h"

using namespace std;

/*****************************************************************
 * 辅助功能
 * 
 * 
 * **************************************************************/
template <class T>
constexpr
std::string_view
type_name()
{
    using namespace std;
#ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
    string_view p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
    return string_view(p.data() + 36, p.size() - 36 - 1);
#  else
    return string_view(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
    string_view p = __FUNCSIG__;
    return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
}



/*****************************************************************
 * g++ -std=c++17 CPP17.cpp -o CPP17
 * 
 * 
 * **************************************************************/


void Structured_Bindings()
{
    struct MyStruct {
        int i = 0;
        string s = "haha";
    };
    //test 1,变量赋值
    {
        MyStruct ms;
        auto [u, v] = ms;
        cout << u << v << endl;
    }

    //test 2，函数返回值
    {
        struct GetMyStruct 
        {
            MyStruct operator() (){return MyStruct{42, "hello"};}
        } getMyStruct;
        auto [id, val] = getMyStruct(); 
        cout << id << val << endl;
        //test 3，迭代
        map<string, string> mymap = {{"a","hi"}, {"b", "ha"}};
        for (const auto& [key, val] : mymap) 
        {
            cout << key << ": " << val << endl;
        }
    }
    //test 3,绑定到一个匿名实体:结构化绑定其实是绑定到一个匿名实体(副本)，绑定引入的变量是副本成员的别名
    //匿名实体同样遵循通常的内存对齐规则，结构化绑定的每一个变量都会根据相应成员的类型进行对齐
    {
        MyStruct ms{42, "hello"};
        auto [u, v] = ms;
        ms.i = 77;
        cout << u << endl; // 打 印 出42
        u = 99;
        cout << ms.i << endl; // 打 印 出77
    }
    //test 4, 使用修饰符
    //如果一个结构化绑定是引用类型，而且是对一个临时对象的引用，那么和往常一样，临时对象的生命周期会被延长到结构化绑定的生命周期
    {
        MyStruct ms{42, "hello"};
        //const auto& [u, v] = ms
        auto& [u, v] = ms; // 被 初 始 化 的 实 体 是ms的 引 用
        ms.i = 77; // 影 响 到u的 值
        cout << u << endl; // 打 印 出77
        u = 99; // 修 改 了ms.i
        cout << ms.i << endl; // 打 印 出99

    }
    //修饰符会作用在新的匿名实体上，而不是结构化绑定引入的新的变量名上。
    {
        MyStruct ms{42, "hello"};
        //const auto& [u, v] = ms
        alignas(16) auto [u, v] = ms; // 对 齐 匿 名 实 体， 而 不 是v
        cout << &v << endl;

    }
    //auto 关键字应用在匿名实体上，这里匿名实体整体并不会发生类型退化化 (decay)。
    {
        struct S {
            const char x[6];
            const char y[3];
        };
        S s1{};
        auto [a, b] = s1; // a和b的 类 型 是 结 构 体 成 员 的 精 确 类 型
        cout << typeid(a).name() << endl;
        cout << type_name<decltype(a)>() << endl;
    }
    //move
    {
        MyStruct ms = { 42, "Jim" };
        auto&& [v, n] = std::move(ms); // 匿 名 实 体 是ms的 右 值 引 用
        cout << "ms.s: " << ms.s << endl; // 打 印 出"Jim
        string s = std::move(n); // 把ms.s移 动 到s
        cout << "ms.s: " << ms.s << endl; // 打 印 出 未 定 义 的 值
        cout << "n: " << n << endl; // 打 印 出 未 定 义 的 值
        cout << "s: " << s << endl; // 打 印 出"Jim"
    }
    {
        MyStruct ms = {42, "Jim" };
        auto [v, n] = std::move(ms); // 新 的 匿 名 实 体 持 有 从ms处 移 动 走 的 值
        cout << "ms.s: " << ms.s << endl; // 打 印 出 未 定 义 的 值
        cout << "n: " << n << endl; // 打 印 出"Jim"
        string s = std::move(n); // 把n移 动 到s
        n = "Lara";
        cout << "ms.s: " << ms.s << endl; // 打 印 出 未 定 义 的 值
        cout << "n: " << n << endl; // 打 印 出"Lara"
        cout << "s: " << s << endl; // 打 印 出"Jim"
    }
    //test 5,
    /*
    • 对于所有非静态数据成员都是 public 的结构体和类，你可以把每一个成员绑定到一个新的变量名上。
    • 对于原生数组，你可以把数组的每一个元素都绑定到新的变量名上。
    • 对于任何类型，你可以使用 tuple­like API 来绑定新的名称，无论这套 API 是如何定义“元素”的。
    对于一个类型 type这套 API 需要如下的组件：
        – std::tuple_size<type>::value 要返回元素的数量。
        – std::tuple_element<idx, type>::type 要返回第 idx 个元素的类型。
        – 一个全局或成员函数 get<idx>() 要返回第 idx 个元素的
    */
   //在任何情况下，结构化绑定中声明的变量名的数量都必须和元素或数据成员的数量相同。
   //继承，所有的非静态数据成员必须在同一个类中定义（也就是说，这些成员要么是全部直接来自于最终的类，要么是全部来自同一个父类）
    {
        struct B {int a = 1;int b = 2;};
        struct D1 : B {};
        auto [x, y] = D1{};
        /*
        struct D2 : B {int c = 3;};
        auto [i, j, k] = D2{}; // 编 译 期ERR
        */
        /*
        struct B {int a = 1;int b = 2;private:int c;};
        struct D1 : B {};
        auto [x, y] = D1{};// 编 译 期ERR
        */
    }
    //c array
    {
        int arr[] = { 47, 11 };
        auto [x, y] = arr; // x和y是arr中 的int元 素 的 拷 贝
        //auto [z] = arr; // ERROR： 元 素 的 数 量 不 匹 配
        //注意 C++允许通过引用来返回带有大小信息的数组，结构化绑定可以应用于返回这种数组的函数：
        struct GetMyStruct 
        {
            auto operator() () -> int(&)[2]{static int sarr[] = { 47, 11 };return sarr;}
        } getArr;
        auto [p, q] = getArr(); // x和y是 返 回 的 数 组 中 的int元 素 的 拷 贝
    }
    // std array ,tuple
    {
        std::array<int, 4> stdarr { 1, 2, 3, 4 };
        auto& [a, b, c, d] = stdarr;
        a += 10; // OK： 修 改 了stdarr[0]
        const auto& [e, f, g, h] = stdarr;
        //e += 10; // ERROR： 引 用 指 向 常 量 对 象
        auto&& [i, j, k, l] = stdarr;
        i += 10; // OK： 修 改 了stdarr[0]
        auto [m, n, o, p] = stdarr;
        m += 10; // OK： 但 是 修 改 的 是stdarr[0]的 拷 贝
        //我们不能用临时对象 (prvalue)初始化一个非 const引用：
        struct GetMyStruct 
        {
            std::array<int, 4>  operator() () {static std::array<int, 4> sstdarr { 1, 2, 3, 4 };return sstdarr;}
        }  getArray;
        //auto& [a, b, c, d] = getArray(); // ERROR

    }
    //std pair
    //如果被赋的值可以赋给一个 std::pair<> 或 std::tuple<> ，你可以使用 std::tie()把值一起赋给所有变量(解引用效果)。
    {
        std::map<std::string, int> coll;
        auto [pos, ok] = coll.insert({"new", 42});
        if (!ok) {
            // 如 果 插 入 失 败， 用pos处 理 错 误
            cout << "map insert error!" << endl;
        }
    }

}

void if_and_switch_with_Initialization()
{
    //test 1, if
    {
        std::map<std::string, int> coll;
        if (auto [pos, ok] = coll.insert({"new", 42});!ok) {
            // 如 果 插 入 失 败， 用pos处 理 错 误
            cout << "map insert error!" << endl;
        }
    }
    {
        std::map<std::string, int> coll;
        coll.insert({"new", 42});
        std::mutex collMutex;
        if (std::lock_guard<std::mutex> _{collMutex}; !coll.empty()) {
            cout << coll["new"] << endl;
        }
    }
    //test 2, switch
    {
        namespace fs = std::filesystem;
        switch (fs::path p{"./CPP17.cpp"}; fs::status(p).type()) {
            case fs::file_type::not_found:
                std::cout << p << " not found\n";
            break;
            case fs::file_type::directory:
                std::cout << p << ":\n";
                for (const auto& e : std::filesystem::directory_iterator{p}) {
                    std::cout << "- " << e.path() << '\n';
                }
            break;
            default:
                std::cout << p << " exists\n";
            break;
        }
    }

}

void Inline_Variables()
{
    //一次定义原则 (ODR)，一个变量或实体的定义只能出现在一个编译单元内——除非该变量或实体被定义为inline 的。
    //CPP17.h里
    {
       
        cout << ns_Inline_Variables::myGlobalMsg<string> << endl;
        cout << ns_Inline_Variables::MyClass01::msg << endl;
        cout << ns_Inline_Variables::MyClass02::trace << endl;
        cout << ns_Inline_Variables::getMsg() << endl;
        cout << ns_Inline_Variables::MyClass03::getMsg() << endl;
        struct MyClassIV : public ns_Inline_Variables::MyClassStatics<> {};
        cout << MyClassIV::msg << endl;
        cout << ns_Inline_Variables::MyType::max.value << endl;
        cout << ns_Inline_Variables::D::n << endl;
        cout << ns_Inline_Variables::cache[0] << ns_Inline_Variables::cache[1] << endl;
    }
}

void Aggregate_Extensions()
{
    //test 1
    {
        struct Data {
            std::string name;
            double value;
        };
        struct MoreData : Data {
            bool done;
        };
        //聚合体初始化时可以用一个子聚合体初始化来初始化类中来自基类的成员
        MoreData x{{"test1", 6.778}, false};
        //如果给出了内层初始化需要的所有值就可以省略内层的花括号：
        MoreData y{"test1", 6.778, false}; // OK

        
    }
    //test 2
    {
        struct Data {
            const char* name;
            double value;
        };
        struct CppData : Data {
            bool critical;
            void print() const {
                std::cout << '[' << name << ',' << value << "]\n";
            }
        };
        CppData y{{"test1", 6.778}, false};
        y.print();

        CppData x1{}; // 所 有 成 员 默 认 初 始 化 为0值
        CppData x2{{"msg"}}; // 和{{"msg", 0.0}, false}等 价
        CppData x3{{}, true}; // 和{{nullptr, 0.0}, true}等 价
        CppData x4; // 成 员 的 值 未 定 义
    }
    //test 3
    {

        ns_Aggregate_Extensions::AG<float> s{{"hello"}, {4.5, 6.7}, "world"}; // 自 从C++17起OK
        cout << std::is_aggregate<decltype(s)>::value << endl; // 输 出1(true)

    }
    //test 4, 向后的不兼容性
    {
        struct Derived;
        struct Base {
            friend struct Derived;
            private:
            Base() {
            }
        };
        struct Derived : Base {
        };
        //因为基类有一个 private的构造函数（见上一节）所以不能使用花括号来初始化。
        //Derived d1{}; // 自 从C++17起ERROR
        Derived d2; // 仍 然OK（但 可 能 不 会 初 始 化）

    }
}

void Mandatory_Copy_Elision_or_Passing_Unmaterialized_Objects()
{
    //test 1,
    {
        using namespace ns_Mandatory_Copy_Elision_or_Passing_Unmaterialized_Objects;
        int i = create<int>(42);
        std::unique_ptr<int> up = create<std::unique_ptr<int>>(new int{42});
        std::atomic<int> ai = create<std::atomic<int>>(42);

    }
    {
        class CopyOnly {
            public:
            CopyOnly() {
            }
            CopyOnly(int) {
            }
            CopyOnly(const CopyOnly&) = default;
            CopyOnly(CopyOnly&&) = delete; // 显 式delete
            };
            struct GetMyStruct 
            {
                CopyOnly operator() () {return CopyOnly{};}
            } ret;
            CopyOnly result = ret() ;
            CopyOnly x = 42; // 自 从C++17起OK

    }
    /*
        lvalue(左值) 的例子有：
        • 只含有单个变量、函数或成员的表达式
        • 只含有字符串字面量的表达式
        • 内建的一元 * 运算符（解引用运算符）的结果
        • 一个返回 lvalue(左值) 引用 (type&) 的函数的返回值
        prvalue(纯右值) 的例子有：
        • 除字符串字面量和用户自定义字面量之外的字面量组成的表达式
        • 内建的一元 & 运算符（取地址运算符）的运算结果
        • 内建的数学运算符的结果
        • 一个返回值的函数的返回值
        • 一个 lambda表达式
        xvalue(到期值) 的例子有：
        • 一个返回 rvalue(右值) 引用 (type&&) 的函数的返回值（尤其是std::move() 的返回值）
        • 把一个对象转换为 rvalue(右值) 引用的操作的结果
        简单来讲：
        • 所有用作表达式的变量名都是 lvalue(左值)。
        • 所有用作表达式的字符串字面量是 lvalue(左值)。
        • 所有其他的字面量（4.2, true, nullptr）是 prvalue(纯右值)。
        • 所有临时对象（尤其是以值返回的对象）是 prvalue(纯右值)。
        • std::move() 的结果是一个 xvalue(到期值)
    */
   /*
        C++中的每个表达式都有两种属性，一个是type（类型），另一个就是value category（值类别）。
        理解值类型体系的关键是现在广义上来说，我们只有两种类型的表达式：
        • glvaue：描述对象或函数位置的表达式
        • prvalue：用于初始化的表达式
        而xvalue可以认为是一种特殊的位置，它代表一个资源可以被回收利用的对象（通常是因为该对象的生命周期
        即将结束）。
        C++17引入了一个新的术语：（临时对象的）实质化 (materialization)，目前 prvalue就是一种临时对象。因此，
        临时对象实质化转换 (temporary materialization conversion) 是一种 prvalue到xvalue的转换。
        在任何情况下 prvalue出现在需要 glvalue（lvalue或者 xvalue）的地方都是有效的，此时会创建一个临时对象
        并用该 prvalue来初始化（注意prvalue主要就是用来初始化的值）。然后该prvalue会被临时创建的 xvalue类型的
        临时对象替换。

        1. 返回值优化（RVO），即通过将返回值所占空间的分配地点从被调用端转移至调用端的手段来避免拷贝操作。返回值优化包括具名返回值优化（NRVO）与无名返回值优化（URVO），两者的区别在于返回值是具名的局部变量还是无名的临时对象。
        2. 右值拷贝优化，当某一个类类型的临时对象被拷贝赋予同一类型的另一个对象时，通过直接利用该临时对象的方法来避免拷贝操作。

        有identity，也可以移动的表达式为xvalue表达式；
        有identity，但不能移动的表达式为lvalue表达式；
        没有identity，但是可以移动的表达式为prvalue表达式；
        至于没有identity，也不可以移动的表达式，在实际应用中不存在这样的表达式，也没必要有这样的表达式。
        有identity的表达式，值类别为glvalue；
        可以移动的表达式，值类别为rvalue。

   */
   {
        //值得强调的一点是严格来讲 glvalue(广义左值)、prvalue(纯右值)、xvalue(到期值) 是描述表达式的术语而不是描
        //述值的术语（这意味着这些术语其实是误称）。例如，一个变量自身并不是左值，只含有这个变量的表达式才是左值
        class X {
        };
        X v;
        const X c;
        struct GetMyStruct 
        {
            void operator() (const X&) {cout << "X&" << endl;return;}// 接 受 任 何 值 类 型
            void operator() (X&&) {cout << "X&&" << endl;return;}// 只 接 受prvalue和xvalue， 但 是 相 比 上 边 的 版 本 是 更 好 的 匹 配
        } f;
        f(v); // 给 第 一 个f()传 递 了 一 个 可 修 改lvalue
        f(c); // 给 第 一 个f()传 递 了 不 可 修 改 的lvalue
        f(X()); // 给 第 二 个f()传 递 了 一 个prvalue
        f(std::move(v)); // 给 第 二 个f()传 递 了 一 个xvalue

   }
    
}
void Lambda_Extensions()
{
    //C++11引入的 lambda和 C++14引入的泛型 lambda
    //• 在常量表达式中使用（也就是在编译期间使用）
    //• 在需要当前对象的拷贝时使用（例如，当在不同的线程中调用 lambda时）
    //自从 C++17 起，lambda表达式会尽可能的隐式声明 constexpr。
    //test 1, const lambda
    {
        auto squared = [](auto val) { // 自 从C++17起 隐 式constexpr
            return val*val;
        };
        std::array<int, squared(5)> a; // 自 从C++17起OK => std::array<int, 25>
    }

    {
        auto squared2 = [](auto val) { // 自 从C++17起 隐 式constexpr
            static int calls = 0; // OK， 但 会 使 该lambda不 能 成 为constexpr
            return val*val;
        };
        //std::array<int, squared2(5)> a; // ERROR： 在 编 译 期 上 下 文 中 使 用 了 静 态 变 量
        std::cout << squared2(5) << '\n'; // OK
    }
    {
        auto squared4 = [](auto val) constexpr {
            //static int calls = 0; // ERROR： 在 编 译 期 上 下 文 中 使 用 了 静 态 变 量
            return val*val;
        };
    }
    {
        //一个隐式或显式的 constexpr lambda的函数调用符也是 constexpr。也就是说，如下定义：
        auto squared = [](int val) { // 从C++17起 隐 式constexpr
            return val*val;
        };
        //将会被转换为如下闭包类型 (closure type)：
        class CompilerSpecificName {
        public:
            constexpr int operator() (int val) const {
                return val*val;
            }
        };
    }

    //test 2,向 lambda 传递 this 的拷贝
    {
        class Data {
        private:
            std::string name;
            public:
            Data(const std::string& s) : name(s) {
            }
            auto startThreadWithCopyOfThis() const {
                // 开 启 并 返 回 新 线 程， 新 线 程 将 在3秒 后 使 用this：
                using namespace std::literals;
                std::thread t([*this] {
                std::this_thread::sleep_for(1s);
                std::cout << name << '\n';
                });
                return t;
            }
        };

        std::thread t;
        {
            Data d{"c1"};
            t = d.startThreadWithCopyOfThis();
        } // d不 再 有 效
        t.join();
                
    }
    //test 3,以常量引用捕获
    {
        std::vector<int> coll{8, 15, 7, 42};
        auto printColl = [&coll = std::as_const(coll)] {
            std::cout << "coll: ";
            for (int elem : coll) {
                std::cout << elem << ' ';
            }
            std::cout << '\n';
        };

        printColl();
    }

    
}
void New_Attributes_and_Attribute_Features()
{
    //test [[nodiscard]], 鼓励编译器在某个函数的返回值未被使用时给出警告。
    struct B {
        [[nodiscard]] bool foo(){return true;}
    };
    struct D : B {
        bool foo(){return true;}
    };
    struct E : B {
        [[nodiscard]] bool foo(){return true;}
    };
    B b;
    b.foo(); // 警 告
    (void)b.foo(); // 没 有 警 告
    D d;
    d.foo(); // 没 有 警 告
    E e;
    e.foo(); // 警 告

    //test [[maybe_unused]],  可以避免编译器在某个变量未被使用时发出警告。
    //属性可以应用于类的声明、使用typedef 或者 using 定义的类型、一个变量、一个非静态数据成员、一个函数、一个枚举类型、一个枚举值等场景
    struct GetMyStruct 
    {
        void operator() (int val, [[maybe_unused]] std::string msg)
        {
            #ifdef DEBUG
            cout << msg << endl;
            #endif
        }
    } f;
    f(1,"hello");

    //test [[fallthrough]] ,可以避免编译器在 switch 语句中某一个标签缺少 break语句时发出警告。
    int place = 1;
    switch (place) {
        case 1:
            std::cout << "very ";
            [[fallthrough]];
        case 2:
            std::cout << "well\n";
            break;
        default:
            std::cout << "OK\n";
            break;
    }

}
void Other_Language_Features()
{
    //嵌套命名空间
    {
        ns_Other_Language_Features::Nested_Namespaces::C::print();
    }
    //有定义的表达式求值顺序
    {
        std::string s = "I heard it even works if you don't believe";
        s.replace(0,8,"").replace(s.find("even"),4,"sometimes").replace(s.find("you don't"),9,"I");
        cout << s << endl; // it sometimes works if I believe
        //
        int i = 0;
        cout << ++i << ' ' << --i << endl;
        /*
            为了解决这种未定义的问题，C++17 标准重新定义了一些运算符的的求值顺序，因此这些运算符现在有了固
            定的求值顺序：
            • 对于运算
            e1 [ e2 ]
            e1 . e2
            e1 .* e2
            e1 ->* e2
            e1 << e2
            e1 >> e2
            e1现在保证一定会在 e2之前求值，因此求值顺序是从左向右。然而，注意同一个函数调用中的不同参数的计
            算顺序仍然是未定义的。
            • 所有的赋值运算
            e2 = e1
            e2 += e1
            e2 *= e1
            中右侧的 e1现在保证一定会在左侧的 e2之前求值。
            • 最后，类似于如下的 new表达式
            new Type(e)
            中保证内存分配的操作在对 e求值之前发生。新的对象的初始化操作保证在第一次使用该对象之前完成。
            所有这些保证适用于所有基本类型和自定义类型。
        */
    }
    //更宽松的用整型初始化枚举值的规则
    {
        //对于一个有固定底层类型的枚举类型变量，自从 C++17 开始可以用一个整型值直接进行列表初始化。
        // 指 明 底 层 类 型 的 无 作 用 域 枚 举 类 型
        enum MyInt1 : char { };
        MyInt1 i11{42}; // 自 从C++17起OK（C++17以 前ERROR）
        //MyInt1 i12 = 42; // 仍 然ERROR
        //MyInt1 i13(42); // 仍 然ERROR
        //MyInt1 i14 = {42}; // 仍 然ERROR
        // 带 有 默 认 底 层 类 型 的 有 作 用 域 枚 举
        enum class Weekday1 { mon, tue, wed, thu, fri, sat, sun };
        Weekday1 s11{0}; // 自 从C++17起OK（C++17以 前ERROR）
        //Weekday1 s12 = 0; // 仍 然ERROR
        //Weekday1 s13(0); // 仍 然ERROR
        //Weekday1 s14 = {0}; // 仍 然ERROR
        // 带 有 明 确 底 层 类 型 的 有 作 用 域 枚 举
        enum class Weekday2 : char { mon, tue, wed, thu, fri, sat, sun };
        Weekday2 s21{0}; // 自 从C++17起OK（C++17以 前ERROR）
        //Weekday2 s22 = 0; // 仍 然ERROR
        //Weekday2 s23(0); // 仍 然ERROR
        //Weekday2 s24 = {0}; // 仍 然ERROR
        //对于没有明确底层类型的无作用域枚举类型（没有 class的 enum），你仍然不能使用列表初始化：
        enum Flag { bit1=1, bit2=2, bit3=4 };
        //Flag f1{0}; // 仍 然ERROR
        //注意列表初始化不允许窄化，所以你不能传递一个浮点数：
        enum MyInt2 : char { };//一个定义新的整数类型的技巧
        //MyInt2 i25{42.2}; // 仍 然ERROR

    }
    //修正 auto 类型的列表初始化
    {
        int x{42}; // 初 始 化 一 个int
        //int y{1, 2, 3}; // ERROR
        auto a{42}; // 现 在 初 始 化 一 个int // 以前初 始 化 一 个std::initializer_list<int>
        //auto b{1, 2, 3}; // 现 在ERROR //// 以前初 始 化 一 个std::initializer_list<int>
        //注意当使用 auto 进行拷贝列表初始化（使用了 =）时仍然是初始化一个 std::initializer_list<>：
        auto c = {42}; // 仍 然 初 始 化 一 个std::initializer_list<int>
        auto d = {1, 2, 3}; // 仍 然OK： 初 始 化 一 个std::initializer_list<int>
        //因此，现在直接初始化（没有 =）和拷贝初始化（有 =）之间又有了显著的不同：
        auto e{42}; // 现 在 初 始 化 一 个int
        auto f = {42}; // 仍 然 初 始 化 一 个std::initializer_list<int>
    }
    //十六进制浮点数字面量
    {
        // 初 始 化 浮 点 数
        std::initializer_list<double> values {
            0x1p4, // 16
            0xA, // 10
            0xAp2, // 40
            5e0, // 5
            0x1.4p+2, // 5
            1e5, // 100000
            0x1.86Ap+16, // 100000
            0xC.68p+2, // 49.625
        };
        // 分 别 以 十 进 制 和 十 六 进 制 打 印 出 值：
        for (double d : values) {
            cout << "dec: " << std::setw(6) << std::defaultfloat << d  << " hex: " << std::hexfloat << d << endl;
        }

    }
    //UTF­8 字符字面量
    {
        //自从 C++11起，C++就已经支持以 u8 为前缀的 UTF­8 字符串字面量。然而，这个前缀不能用于字符字面量。
        //在C++17 中，u8'6' 的类型是 char，在 C++20中可能会变为 char8_t，因此这里使用 auto 会更好一些。
        //C++17 修复了这个问题，所以现在可以这么写：
        auto c = u8'6'; // UTF-8编 码 的 字 符6
        cout << c << endl;
        
        //对于源码中的字符和字符串字面量，C++标准化了你可以
        //使用的字符而不是这些字符的值。这些值取决于源码字符集。当编译器为源码生成可执行程序时它使用运行字
        //符集。源码字符集几乎总是 7 位的 US­ASCII 编码，而运行字符集通常是相同的。

        //字符和字符串字面量现在接受如下前缀：
        //• u8 用于单字节 US­ASCII 和 UTF­8 编码
        //• u 用于两字节的 UTF­16 编码
        //• U 用于四字节的UTF­32 编码
        // • L用于没有明确编码的宽字符，可能是两个或者四个字节
    }
    //异常声明作为类型的一部分
    {
        void (*fp)() noexcept; // 指 向 不 抛 异 常 的 函 数 的 指 针
        fp = ns_Other_Language_Features::fNoexcept; // OK
        //fp = ns_Other_Language_Features::fMightThrow; // 自 从C++17起ERROR
        //把一个不会抛出异常的函数赋给一个可能抛出异常的函数指针仍然是有效的：
        void (*fp2)(); // 指 向 可 能 抛 出 异 常 的 函 数 的 指 针
        fp2 = ns_Other_Language_Features::fNoexcept; // OK
        fp2 = ns_Other_Language_Features::fMightThrow; // OK
        class Base {
            public:
            virtual void foo() noexcept;
        };
        class Derived : public Base {
            public:
            //void foo() override; // ERROR： 不 能 重 载
        };

    }
    //单参数 static_assert
    {
        ns_Other_Language_Features::C<ns_Other_Language_Features::CC> c;
    }
    //预处理条件__has_include
    {
        //__has_include 是一个纯粹的预处理指令。所以不能将它用作源码里的条件表达式：
        //if (__has_include(<filesystem>)) { // ERROR
        //}
    }


    
}
int main(int argc, char *argv[])
{
    // Part 1
    /// Chapter 1
    Structured_Bindings();
    /// Chapter 2
    if_and_switch_with_Initialization();
    /// Chapter 3
    Inline_Variables();
    /// Chapter 4
    Aggregate_Extensions();
    /// Chapter 5
    Mandatory_Copy_Elision_or_Passing_Unmaterialized_Objects();
    /// Chapter 6
    Lambda_Extensions();
    /// Chapter 7
    New_Attributes_and_Attribute_Features();
    /// Chapter 8
    Other_Language_Features();
    // Part 2

    return 0;
}