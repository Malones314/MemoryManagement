#include<iostream>
#include<memory.h>
#include<stdlib.h>

using namespace std;

/**
 * @brief 分配给定大小的空间
 * @param 分配字节数量
 * **/
void* myAlloc( size_t size){
  return malloc( size);
}

/**
 * @brief 释放分配空间
 * @param 待释放的指针
 * **/
void myFree( void* ptr){
  return free( ptr);
}

/**
 * @brief 重载operator new
 * @param 需要分配的字节数量
 * **/
inline void* operator new( size_t size){
  cout << "operator new is called" << endl;
  return myAlloc( size);
}

/**
 * @brief 重载operator delete
 * @param 待释放的指针
 * **/
inline void operator delete( void* ptr){
  cout << "operator delete is called" << endl;
  return myFree( ptr);
}

/**
 * @brief 重载array operator new
 * @param 需要分配的字节数量
 * **/
inline void* operator new[]( size_t size){
  cout << "operator new[] is called" << endl;
  return myAlloc( size);
}

/**
 * @brief 重载array operator delete
 * @param 待释放的指针
 * **/
inline void operator delete[]( void* ptr){
  cout << "operator delete[] is called" << endl;
  return myFree( ptr);
}

int main(){
  int* ptr;
  ptr = (int*)operator new( 32);
  operator delete( ptr);
  ptr = ( int*) operator new[]( 64);
  operator delete[]( ptr);
  return 0;
}