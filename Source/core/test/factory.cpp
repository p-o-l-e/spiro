#include <iostream>
#include <memory>
#include <vector>


class root
{
    public:
        virtual void foo() { std::cout<<"Root function. \n"; }
        root(){ std::cout<<"Root node created..\n"; };
        ~root(){};
};

class branch_a: public root
{
    public:
        virtual void foo() { std::cout<<"Branch A function. \n"; }
        branch_a() { std::cout<<"Branch A node created..\n"; };
        ~branch_a() {};
};

class branch_b: public root
{
    public:
        virtual void foo() { std::cout<<"Branch B function. \n"; }
        branch_b() { std::cout<<"Branch B node created..\n"; };
        ~branch_b() {};
};

class factory
{
    public:
        const int size = 10;
        std::unique_ptr<root*[]> node;
        root* create()
        {
            branch_a a;
            return &a;
            // return new branch_a();
        }
        factory() 
        { 
            std::cout<<"Factory created. \n"; 
            node = std::make_unique<root*[]>(size);
            std::cout<<"Space allocated. \n"; 

            for(int i = 0; i < size; ++i) node[i] = create();

        };
        ~factory() { /*delete node;*/ };
};





// std::unique_ptr<module*[]> node;

int main()
{
    // branch_a ba;
    factory f;
    f.node[0]->foo();
    // root r;

    // r.foo();
    // branch_a ba;
    // ba.foo();
    // for(int i = 0; i < 100; ++i)
    // {
    //     // std::cout<<i<<" ";
    // }
    return 0;
}