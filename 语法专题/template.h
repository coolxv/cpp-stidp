#pragma once
#include <iostream>
#include <string>
#include <atomic>
#include <stdexcept>
#include <memory>
#include <vector>
#include <deque>
#include <type_traits>

#include <assert.h>
#include <stdlib.h>



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



namespace ns_Function_Templates
{
    template<typename T>
    T Max (T a, T b)
    {
        // 如果 b < a, 返回 a，否则返回 b
        return b < a ? a : b;
    }
    template<typename T>
    T foo(T*) { }

    template<typename T>
    void bar(T t)
    {
        //两阶段编译检查(Two-Phase Translation)
        //1. 在模板定义阶段，模板的检查并不包含类型参数的检查。只包含下面几个方面：
        // - 语法检查。比如少了分号。
        // - 使用了未定义的不依赖于模板参数的名称（类型名，函数名，......）。
        // - 未使用模板参数的 static assertions。
        //2. 在模板实例化阶段，为确保所有代码都是有效的，模板会再次被检查，尤其是那些依赖于类型参数的部分。
        //有些编译器并不会执行第一阶段中的所有检查。因此如果模板没有被至少实例化一次的话，你可能一直都不会发现代码中的常规错误。
        //当实例化一个模板的时候，编译器需要看到模板的完整定义。
        //undeclared(); // 如果 undeclared()未定义，第一阶段就会报错，因为与模板参数无关
        //undeclared(t); //如果 undeclared(t)未定义，第二阶段会报错，因为与模板参数有关
        //static_assert(sizeof(int) > 10,"int too small"); // 与模板参数无关，总是报错
        //static_assert(sizeof(T) > 10, "T too small"); //与模板参数有关，只会在第二阶段报错
    }

    template<typename T>
    void print() { T a; std::cout << a << std::endl;}

    template<typename T>
    void f(T = ""){}

    template<typename T = std::string>
    void ff(T = ""){}

    template <typename T>
    void fff(T  &a)
    {
        std::cout << type_name<decltype(a)>() << std::endl;
    }
    //std::common_type<>的结果也是退化的
    template<typename T1, typename T2>
    std::common_type_t<T1,T2> max_ct (T1 a, T2 b)
    {
        return b < a ? a : b;
    }
    #include <type_traits>
    template<typename T1, typename T2, typename RT =
    std::common_type_t<T1,T2>>
    RT max_r1 (T1 a, T2 b)
    {
        return b < a ? a : b;
    }
    #include <type_traits>
    template<typename T1, typename T2, typename RT =
    std::decay_t<decltype(true ? T1() : T2())>>
    RT max_r2 (T1 a, T2 b)
    {
        return b < a ? a : b;
    }

    // maximum of two int values:
    int max_ol (int a, int b)
    {
        return b < a ? a : b;
    }
    // maximum of two values of any type:
    template<typename T>
    T max_ol (T a, T b)
    {
        return b < a ? a : b;
    }

    template<typename T1, typename T2>
    auto max_ret (T1 a, T2 b)
    {
        return b < a ? a : b;
    }
    template<typename RT, typename T1, typename T2>
    RT max_ret (T1 a, T2 b)
    {
        return b < a ? a : b;
    }

    template <int N>
    struct Factorial{
        static int const value = N * Factorial<N-1>::value;
    };

    template <>
    struct Factorial<1>{
        static int const value = 1;
    };

    constexpr int factorial (int num) 
    {
        return num < 2 ? 1 : num * factorial (num - 1);
    }

}

namespace ns_Class_Templates
{
    template<typename T>
    class Stack {
        private:
            std::vector<T> elems; // elements
        public:
            void push(T const& elem); // push element
            T pop(); // pop element
            T const& top() const; // return top element
            bool empty() const { // return whether the stack is empty
                return elems.empty();
            }
            void printOn(std::ostream& strm) const {
                for (T const& elem : elems) {
                    strm << elem << ' '; // call << for each element
                }
            }
            friend std::ostream& operator<< (std::ostream& strm, Stack<T> const& s) {
                s.printOn(strm);
                return strm;
            }

    };
    template<typename T>
    void Stack<T>::push (T const& elem)
    {
        elems.push_back(elem); // append copy of passed elem
    }
    template<typename T>
    T Stack<T>::pop ()
    {
        assert(!elems.empty());
        T elem = elems.back(); // save copy of last element
        elems.pop_back(); // remove last element
        return elem; // return copy of saved element
    }
    template<typename T>
    T const& Stack<T>::top () const
    {
        assert(!elems.empty());
        return elems.back(); // return copy of last element
    }

    //full specialization of class Stack<> for pointers:
    template<>
    class Stack<std::string> {
        private:
            std::deque<std::string> elems; // elements
        public:
            void push(std::string const&); // push element
            std::string pop(); // pop element
            std::string const& top() const; // return top element

            bool empty() const { // return whether the stack is empty
                return elems.empty();
            }
        };

        void Stack<std::string>::push (std::string const& elem)
        {
            elems.push_back(elem); // append copy of passed elem
        }
        std::string Stack<std::string>::pop ()
        {
            assert(!elems.empty());
            std::string elem = elems.back(); // save copy of last element
            elems.pop_back(); // remove last element
            return elem; // return copy of saved element
        }
        std::string const& Stack<std::string>::top () const
        {
            assert(!elems.empty());
            return elems.back(); // return copy of last element
        }

    // partial specialization of class Stack<> for pointers:
    template<typename T>
    class Stack<T*> {
        private:
            std::vector<T*> elems; // elements
        public:
            void push(T*); // push element
            T* pop(); // pop element
            T* top() const; // return top element
            bool empty() const { // return whether the stack is empty
                return elems.empty();
            }
    };
    template<typename T>
    void Stack<T*>::push (T* elem)
    {
        elems.push_back(elem); // append copy of passed elem
    }
    template<typename T>
    T* Stack<T*>::pop ()
    {
        assert(!elems.empty());
        T* p = elems.back();
        elems.pop_back(); // remove last element
        return p; // and return it (unlike in the general case)
    }
    template<typename T>
    T* Stack<T*>::top () const
    {
        assert(!elems.empty());
        return elems.back(); // return copy of last element
    }

    template<typename T1, typename T2>
    class MyClass { };
    // partial specialization: both template parameters have same type
    template<typename T>
    class MyClass<T,T> { };
    // partial specialization: second type is int
    template<typename T>
    class MyClass<T,int> { };
    // partial specialization: both template parameters are pointer types
    template<typename T1, typename T2>
    class MyClass<T1*,T2*> { };


    template<typename T, typename Cont = std::vector<T>>
    class Stack2 {
        private:
            Cont elems; // elements
        public:
            using iter = typename Cont::iterator;
            void push(T const& elem); // push element
            T pop(); // pop element
            T const& top() const; // return top element
            bool empty() const { // return whether the stack is empty
                return elems.empty();
            }
    };
    template<typename T, typename Cont>
    void Stack2<T,Cont>::push (T const& elem)
    {
        elems.push_back(elem); // append copy of passed elem
    }
    template<typename T, typename Cont>
    T Stack2<T,Cont>::pop ()
    {
        assert(!elems.empty());
        T elem = elems.back(); // save copy of last element
        elems.pop_back(); // remove last element
        return elem; // return copy of saved element
    }
    template<typename T, typename Cont>
    T const& Stack2<T,Cont>::top () const
    {
        assert(!elems.empty());
        return elems.back(); // return copy of last element
    }

    //这种声明方式为 typedef， 被定义的名字叫做 typedef-name.
    typedef Stack <int> IntStack;
    //被称为 type alias declaration
    using IntStack = Stack <int>;

    template<typename T>
    using DequeStack = Stack2<T, std::deque<T>>;

}