#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//Описание структур стека
struct NodeList
{
    double inf;
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
bool Push(Stack* st, double val)
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
bool Pop(Stack* st, char* val_c, double* val_d)
{
    bool res = false;

    if (st && (st -> Top) && ((val_c && !val_d) || (!val_c && val_d)))
    {
        if (val_c)
        {
            *val_c = st -> Top -> inf;
        }
        else
        {
            *val_d = st -> Top -> inf;
        }

        Node *tmp = st -> Top;

        st -> Top = tmp -> next;
        
        free(tmp);
        st -> size--;
        
        res = true;
    }

    return res;
}

//Получение элемента в вершине без выталкивания
bool showTop(Stack st, char* val_c, double* val_d)
{
    bool res = false;

    if (((val_c && !val_d) || (!val_c && val_d)) && (st.Top))
    {
        if (val_c)
        {
            *val_c = st.Top -> inf;
        }
        else
        {
            *val_d = st.Top -> inf;
        }

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

        for (size_t i = 0; i < 256; i++)
        {
            Tab[i] = -3;
        }

        //операнды
        for (char i = 'a'; i <= 'z'; i++)
        {
            Tab[i] = -1;
        }
        for (char i = 'A'; i <= 'Z'; i++)
        {
            Tab[i] = -1;
        }
        for (char i = '0'; i <= '9'; i++)
        {
            Tab[i] = -1;
        }

        //скобки
        Tab['('] = 0;
        Tab[')'] = 1;

        //операции
        Tab['+'] = 3;
        Tab['-'] = 3;
        Tab['*'] = 4;
        Tab['/'] = 4;

        //сравнительный приоритет
        Tab['='] = 5;

        //Пробел
        Tab[' '] = -2;

        Stack stack;
        stack.Top = NULL;
        stack.size = 0;
        
        bool flag_stop = false;

        size_t j = 0;
        char prev_char = 0;

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
                    if (exp[i] == '-')
                    {
                        if (prev_char == 0 || Tab[prev_char] >= 3 || prev_char == '(')
                        {
                            if (j < len_rpn -1)
                            {
                                rpn[j] = '0';
                                j++;
                            }
                            else
                            {
                                res = -1;
                                flag_stop = true;
                            }
                        }
                    }
                    
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
                            showTop(stack, &top_val, NULL);

                            if (exp[i] == ')')
                            {
                                char for_bracket;
                                size_t count_while = 0;

                                while (stack.Top && top_val != '(')
                                {
                                    Pop(&stack, &rpn[j], NULL);
                                    showTop(stack, &top_val, NULL);

                                    count_while++;
                                    j++;
                                }

                                if (stack.Top)
                                {
                                    if (!count_while)
                                    {
                                        res = 3;
                                        flag_stop = true;
                                    }
                                    else
                                    {
                                        Pop(&stack, &for_bracket, NULL);
                                    }
                                }
                                else
                                {
                                    res = 3;
                                    flag_stop = true;
                                }
                            }
                            else
                            {   
                                int stack_p = Tab[top_val];

                                if (top_val == '=')
                                {
                                    stack_p = 2;
                                }
                                
                                if (Tab[exp[i]] > stack_p)
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
                                    while (stack.Top && (stack_p >= Tab[exp[i]]))
                                    {
                                        Pop(&stack, &rpn[j], NULL);
                                        j++;   
                                        
                                        showTop(stack, &top_val, NULL);
                                        
                                        if (stack.Top)
                                        {
                                            stack_p = Tab[top_val];

                                            if (top_val == '=')
                                            {
                                                stack_p = 2;
                                            }
                                        }
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

                    prev_char = exp[i];
                }
            }
        }

        if (!flag_stop)
        {   
            for (size_t i = 0; stack.Top && !flag_stop; i++)
            {
                Pop(&stack, &rpn[j], NULL);

                if (rpn[j] == '(')
                {
                    res = 3;
                    flag_stop = true;
                }

                j++;
            }
        }

        rpn[j] = '\0';
        clearStack(&stack);
    }

    return res;
}

//Вычислить ПОЛИЗ
int solveRevPolNot(char* rpn, double* calc_res, double* Tab_val, int* Tab_occ)
{
    int res = -1;

    if (rpn && calc_res && Tab_val && Tab_occ)
    {
        res = 0;

        int eq_count = 0;
        for (size_t k = 0; rpn[k] != '\0'; k++)
        {
            if (rpn[k] == '=')
            {
                eq_count++;
            }
        }
        
        int var_index = 0;

        Stack stack;
        stack.Top = NULL;
        stack.size = 0;

        bool flag_stop = false;

        for (size_t i = 0; rpn[i] != '\0' && !flag_stop; i++)
        {
            if ('0' <= rpn[i] && '9' >= rpn[i])
            {
                double op_val = (double)(rpn[i] - '0');

                bool check_stack = Push(&stack, op_val);

                if (!check_stack)
                {
                    res = 1;
                    flag_stop = true;
                }
            }
            else
            {
                if (('a' <= rpn[i] && 'z' >= rpn[i]) || ('A' <= rpn[i] && 'Z' >= rpn[i]))
                {
                    var_index++;

                    if (var_index <= eq_count)
                    {
                        double op_val = (double)rpn[i];

                        bool check_stack = Push(&stack, op_val);

                        if (!check_stack)
                        {
                            res = 1;
                            flag_stop = true;
                        }
                    }
                    else
                    {
                        if (Tab_occ[(unsigned char)rpn[i]] == 0)
                        {
                            res = 2;
                            flag_stop = true;
                        }
                        else
                        {
                            double op_val = Tab_val[(unsigned char)rpn[i]];

                            bool check_stack = Push(&stack, op_val);

                            if (!check_stack)
                            {
                                res = 1;
                                flag_stop = true;
                            }
                        }
                    }
                }
                else
                {
                    if (rpn[i] == '+' || rpn[i] == '-' || rpn[i] == '*' || rpn[i] == '/' || rpn[i] == '=')
                    {
                        double poped_op_1;
                        double poped_op_2;

                        if (!stack.Top)
                        {
                            res = 2;
                            flag_stop = true;
                        }
                        else
                        {
                            if (stack.size >= 2)
                            {
                                Pop(&stack, NULL, &poped_op_1);
                                Pop(&stack, NULL, &poped_op_2);

                                if (rpn[i] == '=')
                                {
                                    char var_name = (char)poped_op_2;
                                    
                                    Tab_val[(unsigned char)var_name] = poped_op_1;
                                    Tab_occ[(unsigned char)var_name] = 1;
                                }

                                if (rpn[i] == '+')
                                {
                                    poped_op_1 = poped_op_2 + poped_op_1;
                                }
                                if (rpn[i] == '-')
                                {
                                    poped_op_1 = poped_op_2 - poped_op_1;
                                }
                                if (rpn[i] == '*')
                                {
                                    poped_op_1 = poped_op_2 * poped_op_1;
                                }
                                if (rpn[i] == '/')
                                {
                                    if (poped_op_1 == 0)
                                    {
                                        res = 2;
                                        flag_stop = true;
                                    }
                                    else
                                    {
                                        poped_op_1 = poped_op_2 / poped_op_1;
                                    }
                                }

                                bool stack_check = Push(&stack, poped_op_1);

                                if (!stack_check)
                                {
                                    res = 1;
                                    flag_stop = true;
                                }
                            }
                            else
                            {
                                res = 2;
                                flag_stop = true;
                            }
                        }
                    }
                    else
                    {
                        res = 2;
                        flag_stop = true;
                    }
                }
            }
        }

        if (stack.size == 1)
        {
            Pop(&stack, NULL, calc_res);
        }
        else
        {
            res = 2;
        }

        clearStack(&stack);
    }

    return res;
}

int main()
{   
    printf("build test:\n\n");
    
    char expression[100] = "a + b * c - d / (a + b)\0";
    //char expression[100] = "a + b * c - d / (a + b\0";
    //char expression[100] = "a + b * c - d / a + b)\0";
    //char expression[100] = "-()()1\0";

    //char expression[100] = "y = a + -1\0";

    //char expression[100] = "a = b = c - d - 1\0";
    
    char rpn[100];

    int res = buildRevPolNot(expression, rpn, 100);

    printf("return code: %d\nexpression: %s\nrpn: %s ", res, expression, rpn);

    
    
    printf("\n\n");
    
    printf("solve tests: \n\n");
    
    double Tab_val[128];
    int Tab_occ[128] = {0};
    
    char exps[12][100] = {
        
        "-2\0",
        "2 + 2 * 2\0",
        "(2 + 2) * 2\0",
        "22 + 22\0",
        "2 ** 2\0",
        "2 / 2\0",
        "2 / 0\0",

        "a = 2\0",
        "b = a\0",
        "c = b\0",
        "d = c\0",
        "a + b * c - d / (a + b)\0"
    
    };

    for (size_t i = 0; i < 12; i++)
    {
        char rpns[100];
        double calc_res;

        buildRevPolNot(exps[i], rpns, 500);

        int ret_code = solveRevPolNot((char*)&rpns, &calc_res, Tab_val, Tab_occ);

        if (ret_code == 0)
        {
            printf("code: 0\nres: %lf\nexpression: %s\nrpn: %s\n\n", calc_res, exps[i], rpns);
        }
        else
        {
            printf("error code: %d\nexpression: %s\nrpn: %s\n\n", ret_code, exps[i], rpns);
        }
    }

    return 0;
}