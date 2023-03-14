/**
 * 4种内存分配方式
 * **/
#include<iostream>
#include<cstdlib>
#include<memory>
#include<memory.h>
#include<alloca.h>
using namespace std;
int main(){

  int* int_malloc_pointer = nullptr;
  int_malloc_pointer = (int*)malloc(32);
  *int_malloc_pointer = 10;
  cout << int_malloc_pointer << "   " << *int_malloc_pointer << endl;
  free( int_malloc_pointer);

  int* int_new_pointer = nullptr;
  int_new_pointer = new int;
  *int_new_pointer = 10;  
  cout << int_new_pointer << "   " << *int_new_pointer << endl;
  delete int_new_pointer;

  int* int_operator_new_pointer = nullptr;  
  int_operator_new_pointer = static_cast<int*> ( ::operator new( 32));
  *int_operator_new_pointer = 10;
  cout << int_operator_new_pointer << "   " << *int_operator_new_pointer << endl;
  ::operator delete (int_operator_new_pointer);

  int* int_allocate_pointer = nullptr;
#ifdef _MSC_VER
  //以下兩函數都是 non-static，定要通過 object 調用。以下分配 3 個 ints.
  int_allocate_pointer = allocator<int>().allocate(3, (int*)0); 
  *int_allocate_pointer = 10;
  cout << int_allocate_pointer << "   " << *int_allocate_pointer << endl;
  allocator<int>().deallocate( int_allocate_pointer,3);           
#endif
#ifdef __BORLANDC__
  //以下兩函數都是 non-static，定要通過 object 調用。以下分配 5 個 ints.
  int_allocate_pointer = allocator<int>().allocate(5);  
  *int_allocate_pointer = 10;
  cout << int_allocate_pointer << "   " << *int_allocate_pointer << endl;
  allocator<int>().deallocate( int_allocate_pointer,5);       
#endif
#ifdef __GNUC__
  //以下兩函數都是 static，可通過全名調用之。以下分配 512 bytes.
  //void* p4 = alloc::allocate(512); 
  //alloc::deallocate(p4,512);   
  
  //以下兩函數都是 non-static，定要通過 object 調用。以下分配 4 個 ints.    
	int_allocate_pointer = allocator<int>().allocate( 4); 
  *int_allocate_pointer = 10;
  cout << int_allocate_pointer << "   " << *int_allocate_pointer << endl;
  allocator<int>().deallocate((int*)int_allocate_pointer, 4);     
	
  //以下兩函數都是 non-static，定要通過 object 調用。以下分配 9 個 ints.	
	//void* p5 = __gnu_cxx::__pool_alloc<int>().allocate(9); 
  //__gnu_cxx::__pool_alloc<int>().deallocate((int*)p5,9);	
#endif

  return 0;
}
