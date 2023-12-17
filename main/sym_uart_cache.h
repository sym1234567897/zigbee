#ifndef __UART_CACHE_H__
#define __UART_CACHE_H__
/*头件的中的#ifndef，这是一个很关键的东西。比如你有两个C文件，这两个C文件都include了同一个头文件。而编译时，这两个C文件要一同编译成一个可运行文件，于是问题来了，大量的声明冲突。 

还是把头文件的内容都放在#ifndef和#endif中吧。不管你的头文件会不会被多个文件引用，你都要加上这个。一般格式是这样的： */







#include <stdio.h>
#include <stdlib.h>


typedef void* uart_cache_data;    /*不知道 要存的数据是什么类型  ，就用void*?*/

typedef struct _uart_node_
{
	uart_cache_data data;
	struct _uart_node_ *next;
}uart_cache_node,* uart_cache_list;

extern uart_cache_list CreateEmptyCacheList ();
extern int EmptyCacheList (uart_cache_list);
extern uart_cache_list GetCacheNode (uart_cache_list, uart_cache_list *);
extern int InsertCacheNode (uart_cache_list *, uart_cache_data);


#endif













