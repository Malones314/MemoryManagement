#include<iostream>
#include<memory.h>
#include<stdlib.h>
#include<string>
using namespace std;

/**
 * @brief 简单分配int大小的内存池
 * **/

const int pool_size = 512;  //内存池大小
const int block_size = 32;  //内存块大小

/**
 * 空闲链表设计：
 *  Memory_node 节点类
 *  Memory_list 链表类
 * **/

struct Memory_node{
  struct Memory_node* next;
};
struct Memory_list{
  struct Memory_node* head;
};

/**
 * @brief 链表初始化
 * @param m_list 链表 
 * @param memory_pool_address 内存池地址
 * **/
void init_list( struct Memory_list* m_list, void* memory_pool_address ){
  int number_blocks = pool_size / block_size; //内存块数量
  for( int i = 0; i < number_blocks; i++){
    void* this_block_address = memory_pool_address + i * block_size; //当前内存块地址
    struct Memory_node* this_node = ( struct Memory_node*) this_block_address;  //分配节点
    
    //头插法插入节点
    this_node->next = m_list->head;
    m_list->head = this_node;
  }
}

/**
 * @brief 得到一个内存块 
 * @param m_list 空闲链表
 * @return 若空闲链表非空，返回空闲链表头节点
 * **/
void* get_block( struct Memory_list* m_list){
  if( m_list->head == nullptr){
    cout << "空闲链表中无空闲节点" << endl;
    return nullptr;
  }
  struct Memory_node* this_block = m_list->head;  //空闲链表头节点
  m_list->head = this_block->next;
  return ( void*) this_block;
}

/**
 * @brief 找到空闲位置，插入给定节点，合并空闲节点
 * @param m_list 待插入节点
 * @param insert_node 插入节点
 * @param m_pool 内存池，用于读取内存块大小
 *  **/
void insert_block( struct Memory_list* m_list, void* insert_node, Memory_pool* m_pool) {
  
  struct Memory_node* insert_block = ( struct Memory_node*) insert_node;
  
  struct Memory_node* prev = nullptr; //前一个节点
  struct Memory_node* cur = m_list->head; //当前节点
  
  //寻找插入位置
  while ( cur != nullptr && cur != insert_block) {
    prev = cur;
    cur = cur->next;
  }

  if( prev == nullptr){ //要插入到头节点
    insert_block->next = m_list->head;
    m_list->head = insert_block;
  } else { 
    // 插入到中间或者尾部
    insert_block->next = prev->next;
    prev->next = insert_block;
  }
  //合并空闲块
  if( ( char*)prev == ( char*)insert_block){

  }
  
  

}


/**
数据：

 1. 内存池大小：内存池应该能够容纳的内存大小。

 2. 内存块大小：内存池中的内存块应该具有的大小。

 3. 空闲链表：内存池中维护的空闲内存块链表，用于记录当前可用的内存块。

接口：

 1. 构造函数：初始化内存池大小和内存块大小，并为空闲链表分配内存。

 2. 析构函数：释放内存池中的所有内存块和空闲链表。

 3. allocate() 方法：从内存池中分配一个指定大小的内存块，如果没有可用内存块，则返回 NULL。

 4. deallocate() 方法：释放一个已经分配的内存块，将其加入到空闲链表的头部。

 5. is_valid_address() 方法：检查一个地址是否是内存池中的合法地址。

 6. is_valid_block_size() 方法：检查一个块大小是否合法，即是否是内存块大小的整数倍。

 7. construct() 方法：对一个已经分配的内存块进行构造操作，即调用其构造函数。

 8. destruct() 方法：对一个已经分配的内存块进行析构操作，即调用其析构函数。

 9. resize() 方法：重新分配内存池的大小，用于动态调整内存池大小。

 10. clear() 方法：清空内存池中的所有内存块，用于回收内存。
***/

/**
 * 
 * 
 * 
 * **/

class Memory_pool{
private:
  int per_memory_size; //单个内存块大小
  int total_memory_size;  //总内存池大小
public:
  const int get_per_memory_size(){
    return per_memory_size;
  }
  const int get_total_memory_size(){
    return total_memory_size;
  }
};

int main() {
  
  return 0;
}