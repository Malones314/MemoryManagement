#include <iostream>

class MyClass {
public:
    MyClass(int n) : num(n) {
        std::cout << "MyClass Constructor, num = " << num << std::endl;
    }
    ~MyClass() {
        std::cout << "MyClass Destructor, num = " << num << std::endl;
    }
    const int get_num(){
      return num;
    }
private:
    int num;
};

int main() {
    // 分配一段内存，大小为sizeof(MyClass)
    void* mem = malloc(sizeof(MyClass));

    // 在这段内存上构造一个MyClass对象, 使用placement new
    MyClass* obj = new(mem) MyClass(123);

    // 使用对象
    std::cout << "Object num = " << obj->get_num() << std::endl;

    // 手动调用析构函数
    obj->~MyClass();

    // 释放内存
    free(mem);

    return 0;
}
