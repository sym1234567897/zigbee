




/*
*
* typedef void* uart_cache_data;    /*不知道 要存的数据是什么类型，就用void*?*/
*
*  typedef struct _uart_node_
*  {
*	 uart_cache_data data;
*	 struct _uart_node_ *next;
*   }uart_cache_node,* uart_cache_list;
*
*/

#include "sym_uart_cache.h"


uart_cache_list CreateEmptyCacheList()
{
	uart_cache_list h;
	h = (uart_cache_list) malloc(size of (uart_cache_node));/*malloc 返回值是是void * */
		/*uart_cache_node 为什么不用uart_cache_list？*/
	h->next =NULL;
	return h;
		
}
int EmptyCacheList (uart_cache_list h)  /*c语言没有bool类型 只能用int来弄*/
{
	return h->next == NULL;
}
uart_cache_list GetCacheNode (uart_cache_list h, uart_cache_list * t);/*传入 *t  和 t 有什么区别吗*/
{
	if (1 == EmptyCacheList(h))  return 0;
	uart_cache_list p = h->next;
	h->next = p->next;
	if (p->next == NULL)
	{
		*t = h;
	}
	return p;   /*这个get没搞明白    ，答：get后半部分在自己迭代*/
}
	
		
}
int InsertCacheNode (uart_cache_list * t, uart_cache_data x)
{
	q = (uart_cache_list) malloc(size of (uart_cache_node));
	if (q == NULL) 
	{
		printf("InsertCacheNode error\n");
		return -1;
	}
	(*t)->next =q;
	*t =q;  /*这是啥意思？*/
	q->data = x;
	q->next = NULL;   /*尾插法*/

	return 0;
	
}

























