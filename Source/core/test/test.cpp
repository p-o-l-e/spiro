#include <iostream>
#include <memory>
#include <vector>


class A
{
    const int value {0};
    public:
        virtual void foo() { std::cout<<"A function. \n"; }
        A(const int& v): value(v) { std::cout<<"A created..\n"; };
        ~A(){};
};

class B
{
    const A a;
    const int value {0};
    public:
        virtual void foo() { std::cout<<"B function. \n"; }
        B(const int& v): a(v) { std::cout<<"B created..\n"; };
        ~B() {};
};

class C
{
    const B b;
    const int value {0};
    public:
        virtual void foo() { std::cout<<"C function. \n"; }
        C() { std::cout<<"C created..\n"; };
        ~C() {};
};


int main()
{
    C c;
    return 0;
}