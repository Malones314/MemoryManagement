#include<iostream>
#include<stdlib.h>
using namespace std;
class A{
  public:
    A(){
      cout << "A() is called" << endl;
    }
    ~A(){
      cout << "~A() is called" << endl;
    }
};
//array delete和调用构造函数次数测试
int main(){
  cout << "with [] new/delete" << endl;
  A* array_A = new A [5];
  delete [] array_A;
  cout << endl;

  cout << "without [] delete" << endl;
  A* array_A = new A [5];
  delete [] array_A;

  return 0;
}