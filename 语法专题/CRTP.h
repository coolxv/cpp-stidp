#pragma once
#include <iostream>
#include <string>
#include <atomic>
#include <stdexcept>
#include <memory>
#include <vector>

#include <assert.h>
#include <stdlib.h>


/*!
	The version of C++ standard that is being used.
	The C++20 standard.
*/
#ifndef MY_HAS_CPP20
#	ifdef _MSVC_LANG
#		if _MSVC_LANG > 201703L
#			define MY_HAS_CPP20	1
#		else /* _MSVC_LANG > 201703L */
#			define MY_HAS_CPP20	0
#		endif /* _MSVC_LANG > 201703L */
#	else /* _MSVC_LANG */
#		if __cplusplus > 201703L
#			define MY_HAS_CPP20	1
#		else /* __cplusplus > 201703L */
#			define MY_HAS_CPP20	0
#		endif /* __cplusplus > 201703L */
	#endif /* _MSVC_LANG */
#endif /* MY_HAS_CPP20 */


/*!
	The version of C++ standard that is being used.
	The C++17 standard.
*/
#ifndef MY_HAS_CPP17
#	ifdef _MSVC_LANG
#		if _MSVC_LANG > 201402L
#			define MY_HAS_CPP17	1
#		else /* _MSVC_LANG > 201402L */
#			define MY_HAS_CPP17	0
#		endif /* _MSVC_LANG > 201402L */
#	else /* _MSVC_LANG */
#		if __cplusplus > 201402L
#			define MY_HAS_CPP17	1
#		else /* __cplusplus > 201402L */
#			define MY_HAS_CPP17	0
#		endif /* __cplusplus > 201402L */
	#endif /* _MSVC_LANG */
#endif /* MY_HAS_CPP17 */
 
/*!
	The version of C++ standard that is being used.
	The C++14 standard.
*/
#ifndef MY_HAS_CPP14
#	ifdef _MSVC_LANG
#		if _MSVC_LANG > 201103L
#			define MY_HAS_CPP14	1
#		else /* _MSVC_LANG > 201103L */
#			define MY_HAS_CPP14	0
#		endif /* _MSVC_LANG > 201103L */
#	else /* _MSVC_LANG */
#		if __cplusplus > 201103L
#			define MY_HAS_CPP14	1
#		else /* __cplusplus > 201103L */
#			define MY_HAS_CPP14	0
#		endif /* __cplusplus > 201103L */
#	endif /* _MSVC_LANG */
#endif /* MY_HAS_CPP14 */
 
/*!
	The version of C++ standard that is being used.
	The C++11 standard.
*/
#ifndef MY_HAS_CPP11
#	ifdef _MSVC_LANG
#		if _MSVC_LANG > 199711L
#			define MY_HAS_CPP11	1
#		else /* _MSVC_LANG > 199711L */
#			define MY_HAS_CPP11	0
#		endif /* _MSVC_LANG > 199711L */
#	else /* _MSVC_LANG */
#		if __cplusplus > 199711L
#			define MY_HAS_CPP11	1
#		else /* __cplusplus > 199711L */
#			define MY_HAS_CPP11	0
#		endif /* __cplusplus > 199711L */
#	endif /* _MSVC_LANG */
#endif /* MY_HAS_CPP11 */



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



namespace ns_Static_Polymorphism_1
{

    template<typename specific_animal>
    struct animal {
        void who() { static_cast<specific_animal*>(this)->who(); }
    };

    struct dog : animal<dog> {
        void who() { std::cout << "dog" << std::endl; }
    };

    struct cat : animal<cat> {
        void who() { std::cout << "cat" << std::endl; }
    };

    template<typename specific_animal>
    void who_am_i(animal<specific_animal> &animal) {
        animal.who();
    }

}

namespace ns_Static_Polymorphism_2
{
    template <typename Derived>
    struct Base{
    void interface(){
        static_cast<Derived*>(this)->implementation();
    }
    void implementation(){
        std::cout << "Implementation Base" << std::endl;
    }
    };

    struct Derived1: Base<Derived1>{
    void implementation(){
        std::cout << "Implementation Derived1" << std::endl;
    }
    };

    struct Derived2: Base<Derived2>{
    void implementation(){
        std::cout << "Implementation Derived2" << std::endl;
    }
    };

    struct Derived3: Base<Derived3>{};

    template <typename T>
    void execute(T& base){
        base.interface();
    }
}


namespace ns_Limiting_Object_Count
{
    template <class ToBeLimited, uint32_t maxInstance>
    struct LimitNoOfInstances {
        static std::atomic<uint32_t> cnt;

        LimitNoOfInstances() {
            if (cnt >= maxInstance)
                throw std::logic_error{"Too Many Instances"};
            ++cnt;
        }
        ~LimitNoOfInstances() { --cnt; }
    }; // Copy, move & other sanity checks to be complete

    struct One : LimitNoOfInstances<One, 1> {};
    struct Two : LimitNoOfInstances<Two, 2> {};

    template <class T, uint32_t maxNoOfInstace>
    std::atomic<uint32_t> LimitNoOfInstances<T, maxNoOfInstace>::cnt(0);

}

namespace ns_Avoid_Code_Duplication
{
    template <typename T>
    class Container {
        T &actual() { return *static_cast<T *>(this); }
        T const &actual() const { return *static_cast<T const *>(this); }

    public:
        decltype(auto) front() { return *actual().begin(); }
        decltype(auto) back() { return *std::prev(actual().end()); }
        decltype(auto) size() const { return std::distance(actual().begin(), actual().end()); }
        decltype(auto) operator[](size_t i) { return *std::next(actual().begin(), i); }
    };
    template <typename T>
    class DynArray : public Container<DynArray<T>> {
        size_t m_size;
        std::unique_ptr<T[]> m_data;

    public:
        DynArray(size_t s) : m_size{s}, m_data{std::make_unique<T[]>(s)} {}

        T *begin() { return m_data.get(); }
        const T *begin() const { return m_data.get(); }

        T *end() { return m_data.get() + m_size; }
        const T *end() const { return m_data.get() + m_size; }
    };
}

namespace ns_Composite_Design_Pattern
{
    template <typename Self>
    struct SomeNeurons {
        template <typename T>
        void connect_to(T &other);
    };

    struct Neuron : SomeNeurons<Neuron> {
        std::vector<Neuron*>     in, out;
        uint32_t            id;

        Neuron() {
            static int id = 1;
            this->id = id++;
        }

        Neuron* begin() { return this; }
        Neuron* end() { return this + 1; }
    };

    struct NeuronLayer : std::vector<Neuron>, SomeNeurons<NeuronLayer> {
        NeuronLayer(int count) {
            while (count-- > 0)
                emplace_back(Neuron{});
        }
    };

    /* ----------------------------------------------------------------------- */
    template <typename Self>
    template <typename T>
    void SomeNeurons<Self>::connect_to(T &other) {
        for (Neuron &from : *static_cast<Self *>(this)) {
            for (Neuron &to : other) {
                from.out.push_back(&to);
                to.in.push_back(&from);
            }
        }
    }

    /* ----------------------------------------------------------------------- */
    template <typename Self>
    std::ostream &operator<<(std::ostream &os, SomeNeurons<Self> &object) {
        for (Neuron &obj : *static_cast<Self *>(&object)) {
            for (Neuron *n : obj.in)
                os << n->id << "\t-->\t[" << obj.id << "]" << std::endl;

            for (Neuron *n : obj.out)
                os << "[" << obj.id << "]\t-->\t" << n->id << std::endl;
        }
        return os;
    }
}

namespace ns_Spaceship_Operator
{
    template <class derived>
    struct compare {};

    struct value_cpp17 : compare<value_cpp17> {
        int m_x;
        value_cpp17(int x) : m_x(x) {}
        bool operator < (const value_cpp17 &rhs) const { return m_x < rhs.m_x; }
    };

    template <class derived>
    bool operator > (const compare<derived> &lhs, const compare<derived> &rhs) {
        // static_assert(std::is_base_of_v<compare<derived>, derived>); // Compile time safety measures
        return (static_cast<const derived&>(rhs) < static_cast<const derived&>(lhs));
    }

    /*  Same goes with other operators
        == :: returns !(lhs < rhs) and !(rhs < lhs)
        != :: returns !(lhs == rhs)
        >= :: returns (rhs < lhs) or (rhs == lhs)
        <= :: returns (lhs < rhs) or (rhs == lhs) 
    */
#if MY_HAS_CPP20   
    struct value_cpp20{
        int m_x;
        value_cpp20(int x) : m_x(x) {}
        auto operator<=>(const value_cpp20 &rhs) const = default;
    };
    // Defaulted equality comparisons
    // More Info: https://en.cppreference.com/w/cpp/language/default_comparisons
#endif

}

namespace ns_Enabling_Polymorphic_Method_Chaining
{
    template <typename ConcretePrinter>
    class Printer {
        std::ostream &m_stream;
    public:
        Printer(std::ostream &s) : m_stream(s) { }

        ConcretePrinter &print(auto &&t) {
            m_stream << t;
            return static_cast<ConcretePrinter &>(*this);
        }

        ConcretePrinter &println(auto &&t) {
            m_stream << t << std::endl;
            return static_cast<ConcretePrinter &>(*this);
        }
    };

    struct ColorPrinter : Printer<ColorPrinter> {
        enum Color { red, blue, green };
        ColorPrinter(std::ostream &s) : Printer(s) {}

        ColorPrinter &SetConsoleColor(Color c) {
            // ...
            return *this;
        }
    };


}

namespace ns_Enabling_Polymorphic_Copy_Construction
{
#if 1
//Static Polymorphism
    template <class specific>
    struct animal {
        std::unique_ptr<animal> clone() {
            return std::make_unique<specific>(static_cast<specific &>(*this));
        }

    protected: // Forcing animal class to be inherited
        animal(const animal &) = default;
        animal() = default;
    };

    struct dog : animal<dog> {
        dog(const dog &) { std::cout << "copied dog" << std::endl; }
        dog() = default;
    };

    struct cat : animal<cat> {
        cat(const cat &) { std::cout << "copied cat" << std::endl; }
        cat() = default;
    };

    template <class specific>
    void who_am_i(animal<specific> *who) {
        auto duplicate_who = who->clone(); // `copy` object of same type i.e. pointed by who ?
        std::cout << type_name<decltype(duplicate_who)>() << std::endl;
    }
#else
 //Dynamic Polymorphism 
    struct animal {
        virtual std::unique_ptr<animal> clone() = 0;
    };

    struct dog : animal {
        std::unique_ptr<animal> clone() override { return std::make_unique<dog>(*this); }
    };

    struct cat : animal {
        std::unique_ptr<animal> clone() override { return std::make_unique<cat>(*this); }
    };

    void who_am_i(animal *who) {
        auto duplicate_who = who->clone(); // `copy` object of same type i.e. pointed by who ? 
    }
#endif
}


namespace ns_Mixins
{
    template<class Derived>
    class Equality{};

    template <class Derived>
    bool operator == (Equality<Derived> const& op1, Equality<Derived> const & op2){
        Derived const& d1 = static_cast<Derived const&>(op1);     
        Derived const& d2 = static_cast<Derived const&>(op2); 
        return !(d1 < d2) && !(d2 < d1);
    }

    template <class Derived>
    bool operator != (Equality<Derived> const& op1, Equality<Derived> const & op2){
        Derived const& d1 = static_cast<Derived const&>(op1);     
        Derived const& d2 = static_cast<Derived const&>(op2); 
        return !(op1 == op2);
    }

    struct Apple:public Equality<Apple>{
        Apple(int s): size{s}{};
        int size;
    };

    bool operator < (Apple const& a1, Apple const& a2){
        return a1.size < a2.size;
    }

    struct Man:public Equality<Man>{
        Man(std::string n): name{n}{}
        std::string name;
    };

    bool operator < (Man const& m1, Man const& m2){
        return m1.name < m2.name;
    }


}

namespace ns_std_enable_shared_from_this
{
    class ShareMe: public std::enable_shared_from_this<ShareMe>{
        public:
            std::shared_ptr<ShareMe> getShared(){
                return shared_from_this();
            }
            typedef std::shared_ptr<ShareMe> pointer_t;
            using pointer_u = std::shared_ptr<ShareMe>;
    };

}