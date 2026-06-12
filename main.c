#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//Описание структур стека
struct NodeList
{
    char inf;
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
bool Push(Stack* st, char val)
{
    bool res = false;

    if (st)
    {
        Node *el = (Node*)malloc(sizeof(Node));

        if (el)
        {
            el -> inf = val;
            el -> next = NULL;

            el -> next = st -> Top;
            st -> Top = el;
            st -> size++;
            
            res = true;
        }
    }

    return res;
}

//Извлечь элемент из стека
bool Pop(Stack* st, char* val)
{
    bool res = false;

    if (st && (st -> Top) && val)
    {
        *val = st -> Top -> inf;

        Node *tmp = st -> Top;

        st -> Top = tmp -> next;
        
        free(tmp);
        st -> size--;
        
        res = true;
    }

    return res;
}

//Получение элемента в вершине без выталкивания
bool showTop(Stack st, char* val)
{
    bool res = false;

    if (val && (st.Top))
    {
        *val = st.Top -> inf;

        res = true;
    }

    return res;
}

//Очистка стека
void clearStack(Stack* st)
{
    if (st)
    {
        Node *tmp;

        while (st -> Top)
        {
            tmp = st -> Top;
            st -> Top = tmp -> next;
            free(tmp);
        }

        st -> size = 0;
    }
}

//Работа с ПОЛИЗом

//Построить ПОЛИЗ
int buildRevPolNot(char* exp, char* rpn, size_t len_rpn)
{
    int res = -1;

    if (exp && rpn && (strlen(exp) < len_rpn))
    {
        res = 0;
        
        char Tab[256];

        memset(Tab, -3, sizeof(Tab));

        //операнды
        for (char i = 'a'; i <= 'z'; i++)
        {
            Tab[i] = -1;
        }
        for (char i = 'A'; i <= 'Z'; i++)
        {
            Tab[i] = -1;
        }

        //скобки
        Tab['('] = 0;
        Tab[')'] = 1;

        //операции
        Tab['+'] = 2;
        Tab['-'] = 2;
        Tab['*'] = 3;
        Tab['/'] = 3;

        //Пробел
        Tab[' '] = -2;

        Stack stack;
        stack.Top = NULL;
        stack.size = 0;
        
        bool flag_stop = false;

        size_t j = 0;

        //трансляция в ПОЛИЗ
        for (size_t i = 0; (exp[i] != '\0') && !flag_stop; i++)
        {
            if (Tab[exp[i]] == -3)
            {
                res = 2;
                flag_stop = true;
            }
            else
            {
                if (Tab[exp[i]] != -2)
                {
                    if(Tab[exp[i]] == -1)
                    {
                        rpn[j] = exp[i];
                        j++;
                    }
                    else
                    {
                        if (!(stack.Top) || !(Tab[exp[i]]))
                        {
                            bool stack_check = Push(&stack, exp[i]);

                            if (!stack_check)
                            {
                                res = 1;
                                flag_stop = true;
                            }
                        }
                        else
                        {
                            char top_val;
                            showTop(stack, &top_val);

                            if (exp[i] == ')')
                            {
                                char for_bracket;

                                while (stack.Top && top_val != '(')
                                {
                                    Pop(&stack, &rpn[j]);
                                    showTop(stack, &top_val);

                                    j++;
                                }

                                if (stack.Top)
                                {
                                    Pop(&stack, &for_bracket);
                                }
                                else
                                {
                                    res = 3;
                                    flag_stop = true;
                                }
                            }
                            else
                            {
                                if (Tab[exp[i]] > Tab[top_val])
                                {
                                    bool stack_check = Push(&stack, exp[i]);

                                    if (!stack_check)
                                    {
                                        res = 1;
                                        flag_stop = true;
                                    }
                                }
                                else
                                {
                                    while (stack.Top && (Tab[top_val] >= Tab[exp[i]]))
                                    {
                                        Pop(&stack, &rpn[j]);
                                        j++;   
                                        
                                        showTop(stack, &top_val);
                                    }

                                    bool stack_check = Push(&stack, exp[i]);

                                    if (!stack_check)
                                    {
                                        res = 1;
                                        flag_stop = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (!flag_stop)
        {   
            for (size_t i = 0; stack.Top; i++)
            {
                Pop(&stack, &rpn[j]);
                j++;
            }
        }

        rpn[j] = '\0';
        clearStack(&stack);
    }

    return res;
}

int main()
{   
    char expression[100] = "a + b * c - d / (a + b)\0";
    char rpn[100];

    int res = buildRevPolNot(expression, rpn, 100);

    printf("return code: %d\nexpression: %s\nrpn: %s ", res, expression, rpn);

    return 0;
}