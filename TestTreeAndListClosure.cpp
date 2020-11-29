#include <iostream>
#include <string>
#include "TreeClosure.h"

using namespace std;

int main()
{
    auto l = MakeList(5, MakeList(4, MakeList(3, MakeList(6, MakeList(8)))));
    cout << ListLen(l) << endl; // prints 4
    print_list(l); // prints 5 4 3 6 
    auto t = MakeTree(8,
             MakeList(Leaf(4), MakeList(Leaf(5), MakeList(MakeTree(3,
                                                          MakeList(Leaf(9), MakeList(Leaf(10))))))));
    print_levels(t);
    /*
    Prints: 
    8
    4 5 3
    9 10
    */
    deallocate(t);
    print_list(map<int, int>(l, [](int a) { return 2 * a; }));
    print_list(map<int, std::string>(l, [](int a) { 
        return string("Twice the number is ") + to_string(2 * a);
    }));
    print_list(filter<int>(l, [] (int a) { return a % 2 == 0; }));
    print_list(flatmap<int, int>(l, [] (int a) { return MakeList(a, MakeList(a + 1)); }));
    return 0;
}

