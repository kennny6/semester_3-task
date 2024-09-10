#define _CRT_SECURE_NO_WARNINGS 1
#include "global.h"

void Update(Literal *ltr, Node *object, int n) // 删除整个子句时对子句下的元素进行更新
{
    ltr[n].n--;

    if (object->prev_same)
    {
        Node *pre = object->prev_same;
        pre->next_node = object->next_same;
    }
    if (object->next_same)
    {
        Node *next = object->next_same;
        next->prev_same = object->prev_same;
    }
    if (!object->prev_same)
        ltr[n].next = object->next_same;
}
void DeleteNode(Literal *ltr, HeadNode *trash, int valnum) // 删除子句中的结点元素
{
    Node *pre, *current;
    pre = trash->next_node;
    current = pre->next_node;
    int n;
    while (current)
    {
        if (pre->data < 0)
            n = pre->data + valnum;
        else
            n = pre->data + valnum - 1;
        Update(ltr, pre, n);
        delete pre;
        pre = current;
        current = current->next_node;
    }
    if (pre->data < 0)
        n = pre->data + valnum;
    else
        n = pre->data + valnum - 1;
    Update(ltr, pre, n);
    delete pre;
    trash->next_node = nullptr;
}
status CheckSolo(HeadNode *head) // 检查是否存在单子句
{
    HeadNode *ptr = head;
    while (ptr)
    {
        if (ptr->data == 1)
            return TRUE;
        ptr = ptr->next_same;
    }
    return FALSE;
}
status SoloClause(HeadNode *&head, Literal *ltr, int valnum, int *res, int &flag) // 对存在单子句的情况进行处理
{
    flag = 0;
    HeadNode *clausetrav = head;
    while (clausetrav)
    {
        if (clausetrav->data == 1)
        {
            /* 删除单子句 并在答案中赋值 */
            int n;
            if (clausetrav->next_node->data < 0)
            {
                n = clausetrav->next_node->data + valnum;
                res[-clausetrav->next_node->data - 1] = -1;
            }
            else
            {
                n = clausetrav->next_node->data + valnum - 1;
                res[clausetrav->next_node->data - 1] = 1;
            }
            HeadNode *pre, *cur;
            if (!ltr[n].next)
                return FALSE;
            pre = clausetrav->next_node->uncle;
            cur = clausetrav->next_node->parent;

            if (cur->next_same)
            {
                Node *tochange = cur->next_same->next_node;
                while (tochange)
                {
                    tochange->uncle = pre;
                    tochange = tochange->next_node;
                }
            }

            DeleteNode(ltr, clausetrav, valnum);
            if (pre == nullptr)
            {
                head = cur->next_same;
                clausetrav = cur->next_same;
            }
            else
            {
                pre->next_same = cur->next_same;
                clausetrav = pre; // 更新clausetrav
            }
            delete cur;
            if (head == nullptr)
                return TRUE;

            /* 删除含有单子句元素的子句 */
            Node *ltrp = ltr[n].next;
            while (ltrp)
            {
                HeadNode *pre, *current;
                pre = ltrp->uncle;
                current = ltrp->parent;
                if (clausetrav == current) // 更新clausetrav
                {
                    if (pre)
                        clausetrav = pre;
                    else
                        clausetrav = current->next_same;
                }

                /* 更新待删除句子后一句子的文字地址库 */
                if (current->next_same)
                {
                    Node *tochange = current->next_same->next_node;
                    while (tochange)
                    {
                        tochange->uncle = pre;
                        tochange = tochange->next_node;
                    }
                }
                DeleteNode(ltr, current, valnum);
                if (pre == nullptr)
                    head = current->next_same;
                else
                    pre->next_same = current->next_same;
                delete current;

                ltrp = ltr[n].next;
            }

            if (head == nullptr)
                return TRUE;

            /* 删除子句中含有的单子句元素反值 */
            int m = 2 * valnum - n - 1;

            ltrp = ltr[m].next;
            while (ltrp)
            {
                ltrp->parent->data--;

                /* 此 处 判 断 该 CNF 文 件 是 否 有 解 */
                if (ltrp->parent->data == 0)
                {
                    if (clausetrav->next_node->data < 0)
                        res[-clausetrav->next_node->data - 1] = 0;
                    else
                        res[clausetrav->next_node->data - 1] = 0;
                    Destroy(head, ltr, valnum);
                    return NORESULT;
                }
                else if (ltrp->parent->data == 1)
                    flag = 1;

                Node *pre, *current;
                pre = ltrp->pre;
                current = ltrp;

                /* 删除反值元素时，更新文字地址库 */
                int tochange;
                if (current->next_node)
                {
                    current->next_node->pre = current->pre;
                }

                if (pre == nullptr)
                {
                    ltrp->parent->next_node = current->next_node;
                    Update(ltr, current, m);
                    delete current;
                }
                else
                {
                    pre->next_node = current->next_node;
                    Update(ltr, current, m);
                    delete current;
                }
                ltrp = ltr[m].next;
            }
        }
        clausetrav = clausetrav->next_same;
    }
    return TRUE;
}