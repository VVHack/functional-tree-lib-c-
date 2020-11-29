#ifndef LIST_CLOSURE_H_
#define LIST_CLOSURE_H_

#include <functional>

template<typename T>
using list_t=std::function<void*(std::function<void*(T*,void*, int*)>)>;
template<typename T>
using List=list_t<T>*;

template<typename T>
int ListLen(List<T> list);

template<typename T>
list_t<T> make_list(T head, List<T> tail) {
    T* v = new T(std::move(head));
    int* len = new int(ListLen<T>(tail) + 1);
    return [=](std::function<void*(T*,void*, int*)> f) {
        return f(v, (void*)tail, len);
    };
}

template<typename T>
List<T> MakeList(T head, List<T> tail) {
    return new auto(make_list<T>(head, tail));
}

template<typename T>
List<T> MakeEmptyList() {
    return (List<T>)nullptr;
}

template<typename T>
List<T> MakeList(T head) {
    return new auto(make_list<T>(head, nullptr));
}

template<typename T>
int ListLen(List<T> list) {
    if (nullptr == list) return 0;
    return *(int*)(*list)([=](T* head, void* tail, int* len) { return (void*)len; });
}

template<typename T>
T ListHead(List<T> list) {
    return *(T*)(*list)([](T* head, void* tail, int* len){ return (void*)head; });
}

template<typename T>
List<T> ListTail(List<T> list) {
    return (List<T>)(*list)([](T* head, void* tail, int* len){ return tail; });
}

template<typename T>
bool empty(List<T> list) {
    return (list == nullptr);
}

template<typename T>
List<T> concat(List<T> list1, List<T> list2) {
    if (empty(list1)) return list2;
    return MakeList(ListHead(list1), concat(ListTail(list1), list2));
}

template<typename T>
void print_list(List<T> list) {
    if (empty<T>(list)) { std::cout << std::endl; return; }
    std::cout << ListHead(list) << " ";
    print_list(ListTail(list));
}

template<typename T>
int* ListLenPointer(List<T> list) {
    if (nullptr == list) return 0;
    return (int*)(*list)([=](T* head, void* tail, int* len) { return (void*)len; });
}

template<typename T>
T* ListHeadPointer(List<T> list) {
    return (T*)(*list)([](T* head, void* tail, int* len){ return (void*)head; });
}

template<typename T>
void deallocate(List<T> list) {
    if (nullptr == list) return;
    deallocate(ListTail(list));
    delete ListHeadPointer(list);
    delete ListLenPointer(list);
    delete list;
}

template<typename T, typename U>
List<U> map(List<T> list, std::function<U(T)> f) {
    if (empty(list)) {
        return MakeEmptyList<U>();
    }
    return MakeList(f(ListHead(list)), map(ListTail(list), f));
}

template<typename T>
List<T> filter(List<T> list, std::function<bool(T)> f) {
    if (empty(list)) {
        return MakeEmptyList<T>();
    }
    if (f(ListHead(list))) {
        return MakeList(ListHead(list), filter(ListTail(list), f));
    }
    return filter(ListTail(list), f);
}

template<typename T, typename U>
List<U> flatmap(List<T> list, std::function<List<U>(T)> f) {
    if (empty(list)) {
        return MakeEmptyList<U>();
    }
    return concat(f(ListHead(list)), flatmap(ListTail(list), f));
}

#endif
