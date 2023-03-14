# 内存管理 9
## new/delete
### new
```cpp
主要工作：
  1. 分配内存：通过调用operator new，而operator new内部也是调用malloc 
  2. 调用构造函数
有一个try-catch，当分配失败时抛出std::bad_alloc异常,但是如果使用nothorw则不会抛出异常
```
```cpp
std::nothrow 是 C++ 标准库中的一个常量对象，它通常用于表示分配内存操作不会抛出异常。在 C++ 中，当使用 
new 运算符分配内存时，如果无法满足分配请求，将抛出一个 std::bad_alloc 异常。但是，如果在 new 运算符后
添加 std::nothrow，则 new 运算符在分配内存失败时不会抛出异常，而是返回一个空指针。
```
```cpp
int* p = new (std::nothrow) int[10];
//通过检查返回的指针是否为空，我们可以确定内存分配是否成功。
if (p == nullptr) { //分配失败，返回一个空指针。
    std::cerr << "Failed to allocate memory\n";
} else {  //分配成功，将返回一个指向该内存空间的指针
    // 使用 p 指针进行操作
    delete[] p;
}
```
```cpp
当malloc调用失败时，表示堆上已经没有足够的空间分配内存，可能导致程序中断或崩溃，C++提供了一种机制，可以在 
malloc() 分配内存失败时调用用户自定义的处理函数，这个处理函数被称为 new_handler，它可以是一个函数指针或一
个函数对象。

可以通过调用 std::set_new_handler() 函数来注册一个 new_handler。

set_new_handler() 函数仅对使用 new 和 delete 运算符动态分配和释放内存的代码起作用。对于使用 malloc() 和
free() 函数分配和释放内存的代码，则不会调用 new_handler。
```
```cpp
#include <iostream>
#include <new>

void my_new_handler() {
    std::cerr << "Failed to allocate memory!\n";
    exit(1);
}

int main() {
    std::set_new_handler(my_new_handler);

    // 尝试分配一个非常大的内存块
    int* ptr = new int[1000000000000];
}
```
#### placement new
```cpp
允许在指定的内存地址上构造一个对象。它与常规new操作符的区别在于，它不会在堆上分配内存，而是在已经分配的内存
上构造对象。
使用placement new时，要手动分配一段内存，并将其指定为构造对象的地址，然后调用placement new操作符来在这段
内存上构造对象。常用于以下情况：

1. 在特定的内存区域上构造对象，例如在某个硬件寄存器的地址上构造对象。
2. 实现自定义的内存池，以提高内存分配效率。

需要注意的是，由于使用placement new时需要手动分配内存，并且需要手动调用对象的析构函数来释放内存
```
```cpp
void* operator new (size_t size, void* ptr) throw();
size是要分配的内存大小，ptr是指向要构造对象的内存地址的指针
```
### delete
```cpp
主要工作：
  1. 调用析构函数
  2. 释放内存
调用operator delete，而operator delete 调用 free
```   
### array new / array delete
```cpp
要搭配使用，array new 时回分配一个cookie记录长度以及数组里有多少元素，用于计算delete时要调用多少次析构函数,
如果没有[]的delete只调用一次dtor，内存泄漏就是因为dtor调用次数不够造成,如果类中没有内存分配，没有特殊设计的
dtor，那么就算没有[]也不会造成内存泄漏
```

## operator new/operator delete
```cpp
内部调用malloc
```