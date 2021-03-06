#include <stdio.h>

#include "queue.h"
#include "stack.h"
#include "bst.h"

int main(void) {
    int i;

    Stack stack;
    Queue queue;
    Tree tree;

    InitStack(&stack);      // 스택 초기화
    InitQueue(&queue);      // 큐 초기화
    InitTree(&tree);        // 트리 초기화


    insert(&tree, 1);
    printf("Empty? %d\n", IsEmpty_t(&tree));
    insert(&tree, 3);
    printf("%d\n", tree.root->data);

    for(i = 0; i < 5; i++){
        Push(&stack, i);
        Enqueue(&queue, i);
    }
    
    printf("Stack : ");
    while(!IsEmpty_s(&stack)){
        printf("%d ", Pop(&stack));
    }
    printf("\n");

    printf("Queue : ");
    while (!IsEmpty_q(&queue))
    {
        printf("%d ", Dequeue(&queue));
    }
    printf("\n");

    return 0;
}