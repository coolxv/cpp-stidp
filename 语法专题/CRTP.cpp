
#include "CRTP.h"

using namespace std;

/*****************************************************************
 *
 * g++ -std=c++17 CRTP.cpp -lpthread -o CRTP
 * g++ -std=c++20 CRTP.cpp -lpthread -o CRTP
 * 核心特征：基类持有子类
 * **************************************************************/


void Static_Polymorphism_1()
{
    using namespace ns_Static_Polymorphism_1;
    cat c;
    who_am_i(c); // prints `cat`

    dog d;
    who_am_i(d); // prints `dog`
}

void Static_Polymorphism_2()
{
    using namespace ns_Static_Polymorphism_2;
    Derived1 d1;
    execute(d1);
        
    Derived2 d2;
    execute(d2);
    
    Derived3 d3;
    execute(d3);
  
}
void Limiting_Object_Count()
{
    using namespace ns_Limiting_Object_Count;

    Two _2_0, _2_1;

    try {
        One _1_0, _1_1;
    } catch (exception &e) {
        cout << e.what() << endl;
    }

}

void Avoid_Code_Duplication()
{
    using namespace ns_Avoid_Code_Duplication;
    DynArray<int> arr(10);
    arr.front() = 2;
    arr[2]        = 5;
    assert(arr.size() == 10);
    for(auto a : arr)
    {
        cout << a << endl;
    }
}

void Composite_Design_Pattern()
{
    using namespace ns_Composite_Design_Pattern;

    Neuron n1, n2;
    NeuronLayer l1{1}, l2{2};

    n1.connect_to(l1); // Scenario 1: Neuron connects to Layer
    l2.connect_to(n2); // Scenario 2: Layer connects to Neuron
    l1.connect_to(l2); // Scenario 3: Layer connects to Layer
    n1.connect_to(n2); // Scenario 4: Neuron connects to Neuron

    cout << "Neuron " << n1.id << endl << n1 << endl;
    cout << "Neuron " << n2.id << endl << n2 << endl;

    cout << "Layer " << endl << l1 << endl;
    cout << "Layer " << endl << l2 << endl;

/* Output
Neuron 1
[1]    -->    3
[1]    -->    2

Neuron 2
4    -->    [2]
5    -->    [2]
1    -->    [2]

Layer 
1    -->    [3]
[3]    -->    4
[3]    -->    5

Layer 
3    -->    [4]
[4]    -->    2
3    -->    [5]
[5]    -->    2
*/
}

void Spaceship_Operator() //C++20引入了<=>的Spaceship Operator
{
    using namespace ns_Spaceship_Operator;

    value_cpp17 v1{5}, v2{10};
    cout << boolalpha << "v1 > v2: " << (v1 > v2) << '\n';
#if MY_HAS_CPP20  
    value_cpp20 v3{5}, v4{10};
    cout << boolalpha << "v1 > v2: " << (v3 > v4) << '\n';
// Now no need to write comparator operators for all the classes, 
// Write only type dependent `operator <` & inherit with `compare<T>`
#endif
}

void Enabling_Polymorphic_Method_Chaining()
{
    using namespace ns_Enabling_Polymorphic_Method_Chaining;

    ColorPrinter(cout).print("Hello ").SetConsoleColor(ColorPrinter::Color::red).println("Printer!");


}
void Enabling_Polymorphic_Copy_Construction()
{
    using namespace ns_Enabling_Polymorphic_Copy_Construction;
    dog d;
    who_am_i(&d);
    cat c;
    who_am_i(&c);
}

void Mixins()
{
    using namespace ns_Mixins;
    Apple apple1{5};
    Apple apple2{10}; 
    std::cout << "apple1 == apple2: " << (apple1 == apple2) << std::endl;

    Man man1{"grimm"};
    Man man2{"jaud"};
    std::cout << "man1 != man2: " << (man1 != man2) << std::endl;
}

void std_enable_shared_from_this()
{
    using namespace ns_std_enable_shared_from_this;
    std::shared_ptr<ShareMe> shareMe(new ShareMe);
    ShareMe::pointer_t shareMe1= shareMe->getShared();
    {
        ShareMe::pointer_u shareMe2(shareMe1);
        std::cout << "shareMe.use_count(): "  << shareMe.use_count() << std::endl;
    }
    std::cout << "shareMe.use_count(): "  << shareMe.use_count() << std::endl;
    shareMe1.reset();
    std::cout << "shareMe.use_count(): "  << shareMe.use_count() << std::endl;
}

int main(int argc, char *argv[])
{
    Static_Polymorphism_1();
    Static_Polymorphism_2();
    Limiting_Object_Count();
    Avoid_Code_Duplication();
    Composite_Design_Pattern();
    Spaceship_Operator();
    Enabling_Polymorphic_Method_Chaining();
    Enabling_Polymorphic_Copy_Construction();
    Mixins();
    std_enable_shared_from_this();
    return 0;
}