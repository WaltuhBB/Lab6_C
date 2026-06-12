#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Описание структур стека
struct NodeList
{
    int inf;
    struct NodeList* next;
};

struct StackList
{
    struct NodeList* Top;
    size_t size;
};

typedef struct NodeList Node;
typedef struct StackList Stack;

//Функции для работы со стеком

//Добавить элемент в стек
bool Push(Stack* st, int val)
{
    bool res = false;

    if (st)
    {
        Node *el = (Node*)malloc(sizeof(Node));

        if (el)
        {
            el -> inf = val;
            el -> next = NULL;

            if (st -> Top == NULL)
            {
                st -> Top = el;
            }
            else
            {
                el -> next = st -> Top;
                st -> Top = el;
            }

            st -> size++;
            
            res = true;
        }
    }

    return res;
}



int main()
{
    Stack stack;

    stack.Top = NULL;
    stack.size = 0;

    Push(&stack, 33);
    Push(&stack, 28);
    Push(&stack, 46);
    Push(&stack, 157);

    Node *ptr_i = NULL;

    if (stack.size == 0)
    {
        printf("Stack is empty\n");
    }
    else
    {
        for (ptr_i = stack.Top; ptr_i; ptr_i = ptr_i -> next)
        {
            printf("%d\n", ptr_i -> inf);
        }
    }

    return 0;
}