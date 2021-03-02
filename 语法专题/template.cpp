
#include "template.h"

using namespace std;

/*****************************************************************
 *
 * g++ -std=c++17 template.cpp -lpthread -o template
 * g++ -std=c++20 template.cpp -lpthread -o template
 * 
 * **************************************************************/

void Function_Templates()
{
    using namespace ns_Function_Templates;
    //test 1
    {
        //用具体类型取代模板类型参数的过程叫做“实例化”。
        int i = 42;
        std::cout << "Max(7,i): " << Max(7,i) << std::endl;
        double f1 = 3.4;
        double f2 = -6.7;
        std::cout << "Max(f1,f2): " << Max(f1,f2) << std::endl;
        std::string s1 = "mathematics";
        std::string s2 = "math";
        std::cout << "Max(s1,s2): " << Max(s1,s2) << std::endl;
    }
    //test 2
    {
        //void 作为模板参数也是有效的。
        void* vp = nullptr;
        bar(vp); // OK: 模板参数被推断为 void

    }
    //test 3
    {
        //模板参数推断
        // 在类型推断的时候自动的类型转换是受限制的：
        // - 如果调用参数是按引用传递的，任何类型转换都不被允许。通过模板类型参数 T 定义的
        //   两个参数，它们实参的类型必须完全一样。
        // - 如果调用参数是按值传递的，那么只有退化（decay）这一类简单转换是被允许的：const
        //   和 volatile 限制符会被忽略，引用被转换成被引用的类型，raw array 和函数被转换为相
        //   应的指针类型。通过模板类型参数 T 定义的两个参数，它们实参的类型在退化（decay）
        //   后必须一样。


        //print(); // error,couldn’t deduce template parameter ‘T’
        print<int>();
    }
    //test 4
    {
        int const c = 42;
        int i = 1; //原书缺少 i 的定义
        Max(i, c); // OK: T 被推断为 int，c 中的 const 被 decay 掉
        Max(c, c); // OK: T 被推断为 int
        int& ir = i;
        Max(i, ir); // OK: T 被推断为 int， ir 中的引用被 decay 掉
        int arr[4];
        Max(&i, arr); // OK: T 被推断为 int* 
        //Max(4, 7.2); // ERROR: 不确定 T 该被推断为 int 还是 double
        //std::string s;
        //foo("hello", s); //ERROR: 不确定 T 该被推断为 const[6] 还是 std::string
        //有两种办法解决以上错误：
        //1. 对参数做类型转换
        Max(static_cast<double>(4), 7.2); // OK
        //2. 显式地指出类型参数 T 的类型，这样编译器就不再会去做类型推导。
        Max<double>(4, 7.2); // OK
        //3. 指明调用参数可能有不同的类型（多个模板参数）。
    }
    //test 5
    {
        //类型推断并不适用于默认调用参数。
        f(1); // OK: T 被推断为 int, 调用 f<int> (1)
        //f(); // ERROR: 无法推断 T 的类型
        ff(1); // OK: T 被推断为 int, 调用 f<int> (1)
        ff(); // OK
    }
    //test 6
    {
        char a[] = "hello";
        char *p = a;
        fff(a);
        fff(p);
    }
    //test 7
    {
        cout << max_ct(7.4, 4) << endl;
        cout << max_r1(7.4, 4) << endl;
        cout << max_r2(7.4, 4) << endl;
    }
    //test 8
    {
        //个非模板函数可以和一个与其同名的函数模板共存，并且这个同名的函数模
        //板可以被实例化为与非模板函数具有相同类型的调用参数。在所有其它因素都相同的情况
        //下，模板解析过程将优先选择非模板函数，而不是从模板实例化出来的函数。
        max_ol(7, 42); // calls the nontemplate for two ints
        //如果模板可以实例化出一个更匹配的函数，那么就会选择这个模板。
        max_ol(7.0, 42.0); // calls max<double> (by argument deduction)
        max_ol('a', 'b'); //calls max<char> (by argument deduction)
        //也可以显式指定一个空的模板列表。这表明它会被解析成一个模板调用，
        max_ol<>(7, 42); // calls max<int> (by argumentdeduction)
        max_ol<double>(7, 42); // calls max<double> (no argumentdeduction)
        //由于在模板参数推断时不允许自动类型转换，而常规函数是允许的
        max_ol('a', 42.7); //calls the nontemplate for two ints
        max_ol<int>(4, 7.2); // ERROR: both function templates match
    }
    //test 9
    {
        auto a = max_ret(4, 7.2); // uses first template
        auto b = max_ret<long double>(7.2, 4); // uses second template
        //auto c = ::max<int>(4, 7.2); // ERROR: both function templates match
    }
    //test 10
    {
        std::cout << "Factorial<5>::value: " << Factorial<5>::value << std::endl;
        std::cout << "Factorial<10>::value: " << Factorial<10>::value << std::endl;
        std::cout << "factorial(10): " << factorial(10) << std::endl;
    }
}

void Class_Templates()
{
    using namespace ns_Class_Templates;
    //test 1
    {
        //不同于非模板类，不可以在函数内部或者块作用域内（{...}）声明和定义模板。通常模板只能定义在 global/namespace 作用域，或者是其它类的声明里面
        //模板函数和模板成员函数只有在被调用的时候才会实例化。
        //如果一个类模板有 static 成员，对每一个用到这个类模板的类型，相应的静态成员也只会被实例化一次
        Stack< int> intStack; // stack of ints
        Stack<std::string> stringStack; // stack of strings
        // manipulate int stack
        intStack.push(7);
        std::cout << intStack.top() << std::endl;
        // manipulate string stack
        stringStack.push("hello");
        std::cout << stringStack.top() << std::endl;
        stringStack.pop();
    }
    //test 2
    {
        Stack<std::pair< int, int>> ps; // note: std::pair<> has no operator<<   defined
        ps.push({4, 5}); // OK
        ps.push({6, 7}); // OK
        std::cout << ps.top().first << std::endl; // OK
        std::cout << ps.top().second << std::endl; // OK
        //只有在调用 printOn()的时候，才会导致错误，因为它无法为这一类型实例化出对 operator<<  的调用：
        //ps.printOn(std::cout); // ERROR: operator<< not supported for element type
    }
    //test 3
    {
        Stack<std::pair< int, int>> ps; // std::pair<> has no operator<< defined
        ps.push({4, 5}); // OK
        ps.push({6, 7}); // OK
        std::cout << ps.top().first << std::endl; // OK
        std::cout << ps.top().second << std::endl; // OK
        //std::cout << ps << std::endl; // ERROR: operator<< not supported // for elementtype
    }
    //test 4
    {
        Stack< int*> ptrStack; // stack of pointers (specialimplementation)
        ptrStack.push(new int{42});
        std::cout << *ptrStack.top() << std::endl;
        delete ptrStack.pop();
    }
    //test 5
    {
        MyClass< int, float> mif; // uses MyClass<T1,T2>
        MyClass< float, float> mff; // uses MyClass<T,T>
        MyClass< float, int> mfi; // uses MyClass<T,int>
        MyClass< int*, float*> mp; // uses MyClass<T1*,T2*>
        //如果有不止一个特例化的版本可以以相同的情形匹配某一个调用，说明定义是有歧义的：
        //MyClass< int, int> m; // ERROR: matches MyClass<T,T> // and  MyClass<T,int>
        //MyClass< int*, int*> m; // ERROR: matches MyClass<T,T> // and MyClass<T1*,T2*>
    }
    //test 6
    {
        // stack of ints:
        Stack2< int> intStack;
        // stack of doubles using a std::deque<> to manage the elements
        Stack2< double,std::deque< double>> dblStack;
        // manipulate int stack
        intStack.push(7);
        std::cout << intStack.top() << std::endl;
        intStack.pop();
        // manipulate double stack
        dblStack.push(42.42);
        std::cout << dblStack.top() << std::endl;
        dblStack.pop();
    }
    //test 7
    {
        IntStack istack[10];
        DequeStack<int> idstack[10];
        DequeStack<int>::iter tmp;
    }




} 


int main(int argc, char *argv[])
{
    Function_Templates();
    Class_Templates();
    return 0;
}