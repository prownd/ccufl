/**
 * @file stack.h
 * @time 2015-7-2
 * @author hanjinpeng127@163.com
 * @version 1.0
 */

#ifndef __STACK_H__
#define __STACk_H__

typedef struct _stack_ stack_t;


extern stack_t *stack_create(unsigned int nodesize,void (*s_free)(void *));

extern void stack_free(stack_t *stack);

extern void stack_init(stack_t *stack);

extern int stack_count(stack_t *stack);


extern void *stack_top(stack_t *stack);

extern int stack_push(stack_t *stack, void *node);


extern void* stack_pop(stack_t *stack);


#endif /* __STACk_H__ */

