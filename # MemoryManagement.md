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

使用placement new时，要手动分配一段内存，并将其指定为构造对象的地址，然后调用placement new操作符来在这段内
存上构造对象。常用于以下情况：
1. 在特定的内存区域上构造对象，例如在某个硬件寄存器的地址上构造对象。
2. 实现自定义的内存池，以提高内存分配效率。

需要注意的是，由于使用placement new时需要手动分配内存，并且需要手动调用对象的析构函数来释放内存
```
```cpp
void* operator new (size_t size, void* ptr) throw();
size是要分配的内存大小，ptr是指向要构造对象的内存地址的指针
```
```cpp
重载placement new的注意事项如下：

1. 确保重载的placement new函数参数列表与标准的placement new函数参数列表一致。标准的placement new函数包含两个参
数，一个是void*类型的指针，指向分配的内存块，另一个是size_t类型的参数，表示内存块的大小。如果你的重载函数参数不
一致，就不能作为placement new函数来使用。

2. 重载的placement new函数应该只分配所需大小的内存。因为placement new函数是用来在已分配的内存块中构造对象的，因
此在使用placement new函数时，需要手动分配内存。如果你在重载的placement new函数中分配了比所需更多的内存，那么就会
出现问题。

3. 如果你重载了placement new函数，那么也应该同时重载对应的placement delete函数。因为当你在已分配的内存块中构造对
象时，也需要手动调用对应的placement delete函数来销毁对象并释放内存。

4. 确保你的重载placement new函数正确处理内存对齐。内存对齐是C++中的一个重要概念，它确保数据存储在内存中的位置能够
被CPU高效地读取和写入。如果你不正确地处理内存对齐，就会导致不可预测的行为和内存访问错误。

5. 当你重载placement new函数时，应该注意在多线程环境下的线程安全性。如果你的重载函数不是线程安全的，那么在多线程
环境下使用可能会导致未定义行为或内存泄漏。因此，应该使用适当的同步机制来确保线程安全。
```
```cpp
重载class member placement new的注意事项：

1. 第一个参数必须是size_t,第二第三等参数可以自定义。

2. 必须在类内声明placement new操作符：placement new操作符必须在类内声明为静态成员函数，并且函数名必须为operator 
new。例如，如果要在类Foo中重载placement new，那么必须在Foo类内声明一个静态成员函数operator new。

3. 重载的placement new必须接受额外的参数：与常规new不同，placement new必须接受一个额外的参数，用于指定分配内存的
位置。通常，此参数为一个指针，指向预先分配的内存区域。

4. 重载的placement new必须返回指定位置的指针:placement new必须返回一个指向指定位置的指针，即接受的参数指向的位置。
通常，返回值为void*类型，指向已分配的内存区域。

5. 不能抛出异常：由于placement new通常用于分配内存的特定位置，因此不允许抛出异常。如果分配失败，应该返回一个null指
针。

6. 必须手动调用构造函数：placement new只是为对象分配内存，而不会调用构造函数。因此，在使用placement new时，必须手
动调用对象的构造函数。
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
在设计的某一个类中可以重载operator new和operator delete
全局的operator new和operator delete也可以重载，但是很少见，因为影响很深远
```
```cpp
//重载类中的operator new/delete
//这些函数必须是静态函数，编译器会帮你加上static，故自己写不写都行
class MyClass{
public:
  static void* operator new( size_t size);
  static void operator delete( void*, size_t); //第二参数size_t可有可无
  static void* operator new[]( size_t size);
  static void operator delete( void*, size_t); 
};
通过::new MyClass(size);可使用全局的operator new
```