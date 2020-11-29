#include <iostream>
#include <string>
#include <functional>

using namespace std;

template<typename T>
using tree_t=function<const void*(function<const void*(const void*,const T*,const void*)>)>;

template<typename T>
tree_t<T> make_tree(const void* left, const T val, const void* right) {
    const T* v = new T(std::move(val));
    return [=](function<const void*(const void*,const T*,const void*)> f) {
        return f(left, v, right);
    };
}

template<typename T>
const tree_t<T>* Tree(const void* left, const T val, const void* right) {
    return new auto(make_tree(left, val, right));
}

template<typename T>
const tree_t<T>* Leaf(const T val) {
    return Tree(nullptr, val, nullptr);
}

template<typename T>
T TreeVal(const tree_t<T>* tree) {
    return *(const T*)((*tree)([](const void* left, const T* val, const void* right) {
        return val;
    }));
}

template<typename T>
const T* TreeValPointer(const tree_t<T>* tree) {
    return (const T*)((*tree)([](const void* left, const T* val, const void* right) {
        return val;
    }));
}

template<typename T>
const tree_t<T>* TreeLeft(const tree_t<T>* tree) {
    return (const tree_t<T>*)((*tree)([](const void* left, const T* val, const void* right) {
        return left;
    }));
}

template<typename T>
const tree_t<T>* TreeRight(const tree_t<T>* tree) {
    return (const tree_t<T>*)((*tree)([](const void* left, const T* val, const void* right) {
        return right;
    }));
}

template <typename T>
string traversal(const tree_t<T>* tree) {
    if (TreeLeft<T>(tree) == nullptr && TreeRight<T>(tree) == nullptr) {
        return to_string(TreeVal<T>(tree));
    }
    if (TreeLeft<T>(tree) == nullptr) {
        return to_string(TreeVal<T>(tree)) + string(" ") + traversal<T>(TreeRight(tree));
    }
    if (TreeRight<T>(tree) == nullptr) {
        return traversal<T>(TreeLeft<T>(tree)) + string(" ") + to_string(TreeVal<T>(tree));
    }
    return traversal<T>(TreeLeft<T>(tree)) + string(" ") + to_string(TreeVal<T>(tree)) + string(" ") + traversal<T>(TreeRight<T>(tree));
}

template<typename T>
void deallocate(const tree_t<T>* tree) {
    if (nullptr == tree) return;
    deallocate<T>(TreeLeft<T>(tree));
    deallocate<T>(TreeRight<T>(tree));
    delete TreeValPointer<T>(tree);
    delete tree;
}

int main()
{
    const auto t = Tree<int>(Leaf<int>(6), 8, Tree<int>(Leaf<int>(4), 7, Leaf<int>(5)));
    cout << traversal<int>(t) << endl; // prints 6 8 4 7 5!!!!!
    deallocate<int>(t);
    return 0;
}

