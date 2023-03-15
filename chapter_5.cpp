#include<iostream>
#include<memory.h>
#include<stdlib.h>

using namespace std;

class M{
private:
  int k;
public:
  const int get_k() { return k; }
  M():k(0) { };
  M( int k_) : k ( k_){ }
  static void* operator new( size_t t, char c){
    cout << "operator new( size_t t, char c)" << endl;
    return (M*)malloc( t);
  }
  static void* operator new( size_t t){
    cout << "operator new( size_t t)" << endl;
    return (M*)malloc( t);
  }
  //void* operator new( char c, size_t t){  //error : ‘operator new’的第一个形参类型为‘size_t’(‘unsigned int’)
  //  cout << "operator new( char c, size_t t)" << endl;
  //  return (M*)malloc(t);
  //}
  static void operator delete( void* ptr){
    cout << "operator delete( void* ptr)" << endl;
    free( ptr);
  }
};

int main(){
  M* m;
  m = static_cast<M*> ( M::operator new( 32, 'c'));
  M::operator delete( m);
  return 0;
}