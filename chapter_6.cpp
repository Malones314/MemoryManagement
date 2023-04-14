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
  //检查当前空闲块的前一个块是否是空闲块，以及前一个块与当前块之间的空间是否足够大以合并为一个更大的块。
  //如果是，则会将前一个块的next指针指向当前块的next指针所指向的块，相当于将当前块从空闲链表中删除，同时
  //将insert_block指针指向前一个块，相当于将当前块与前一个块合并成一个更大的空闲块。
  if( ( char*)prev + m_pool->get_per_memory_size() == ( char*)insert_block){
      prev->next = insert_block->next;
      insert_block = prev;
  }

  //检查当前空闲块的后一个块是否是空闲块，以及当前块与后一个块之间的空间是否足够大以合并为一个更大的块。
  //如果是，则会将当前块的next指针指向后一个块的next指针所指向的块，相当于将后一个块从空闲链表中删除，
  //从而将当前块与后一个块合并成一个更大的空闲块。
  if( (char*)insert_block + m_pool->get_per_memory_size() == (char*)insert_block->next ){
    insert_block->next = insert_block->next->next;
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

class Memory_pool{
private:
  int per_memory_size; //单个内存块大小
  int total_memory_size;  //总内存池大小
  struct Memory_list* memory_pool_list; //空闲链表
public:
  Memory_pool();  //构造函数，完成内存池的初始化
  ~Memory_pool(); //析构函数，完成内存的释放
  
  void* allocate( int needed_block_size); //从内存池中分配指定大小的内存块，如果没有可用内存块，则返回 NULL
  
  void deallocate( void* block_address);  //释放已经分配的内存块，将其加入到空闲链表的头部。

  bool is_valid_address( void* check_block);  //检查地址是否是内存池中的合法地址。

  bool is_valid_block_size( size_t block_size );  //检查块大小是否合法，即是否是内存块大小的整数倍。

  void construct( void* block_address );  //对一个已经分配的内存块进行构造操作，即调用其构造函数。
                                          //因为这个内存池为int类型变量的内存池，故不做任何动作

  void destruct( void* block_address);  //对一个已经分配的内存块进行析构操作，即调用其析构函数。
                                        //因为这个内存池为int类型变量的内存池，故不做任何动作

  void resize( size_t new_size);  //重新分配内存池的大小，用于动态调整内存池大小。
  void clear(); //清空内存池中的所有内存块，用于回收内存。
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