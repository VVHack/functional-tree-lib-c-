#ifndef TREE_CLOSURE_H_
#define TREE_CLOSURE_H_

#include "ListClosure.h"

template<typename T>
using tree_t=std::function<void*(std::function<void*(T*,List<void*>)>)>;
template<typename T>
using Tree=tree_t<T>*;


template<typename T>
tree_t<T> make_tree(T val, List<Tree<T>> tail) {
    T* v = new T(std::move(val));
    return [=](std::function<void*(T*,List<void*>)> f) {
        return f(v, (List<void*>)tail);
    };
}

template<typename T>
Tree<T> MakeTree(T val, List<Tree<T>> children) {
    return new auto(make_tree(val, children));
}

template<typename T>
Tree<T> Leaf(T val) {
    return MakeTree<T>(val, nullptr);
}

template<typename T>
T TreeVal(Tree<T> tree) {
    return *(T*)((*tree)([](T* val, List<void*> children) { return val; }));
}

template<typename T>
List<Tree<T>> TreeChildren(Tree<T> tree) {
    return (List<Tree<T>>)((*tree)([](T* val, List<void*> children) { return children; }));
}

template<typename T>
void deallocate(Tree<T> tree);

template<typename T>
void deallocate_children(List<Tree<T>> children) {
    if (nullptr == children) return;
    deallocate(ListHead(children));
    deallocate_children(ListTail(children));
}

template<typename T>
T* TreeValPointer(Tree<T> tree) {
    return (T*)((*tree)([](T* val, List<void*> children) { return val; }));
}

template<typename T>
void deallocate(Tree<T> tree) {
    if (nullptr == tree) return;
    deallocate_children(TreeChildren(tree));
    deallocate(TreeChildren(tree));
    delete TreeValPointer(tree);
    delete tree;
}

// BFS basically
template<typename T>
void print_levels(List<Tree<T>> this_level, List<Tree<T>> next_level) {
    if (ListLen(this_level) == 0) {
        std::cout << std::endl;
        if (ListLen(next_level) == 0) return;
        return print_levels(next_level, MakeEmptyList<Tree<T>>());
    }
    std::cout << TreeVal(ListHead(this_level)) << " ";
    return print_levels(ListTail(this_level), concat(next_level, TreeChildren(ListHead(this_level))));
}

template<typename T>
void print_levels(Tree<T> tree) {
    print_levels(MakeList(tree), MakeEmptyList<Tree<T>>());
}

#endif

