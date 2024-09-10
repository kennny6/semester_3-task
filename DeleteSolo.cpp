#define _CRT_SECURE_NO_WARNINGS 1
#include "global.h"

void Update(Literal *ltr, Node *object, int n) // 删除整个子句时对子句下的元素进行更新
{
    Literal *pre, *cur;
    pre = &ltr[n];
    cur = ltr[n].next;
    while (cur)
    {
        if (cur->pos == object)
        {
            pre->next = cur->next;
            delete cur;
            ltr[n].n--;
            break;
        }
        pre = cur;
        cur = cur->next;
    }
}
void Update(Literal *ltr, Literal *target, Node *tochange, int n) // 删除单个元素时，对下一元素的 pre 进行更新
{
    Literal *p = ltr[n].next;
    while (p)
    {
        if (p->pos == tochange)
        {
            p->pre = target->pre;
            break;
        }
        p = p->next;
    }
}
void DeleteNode(Literal *ltr, HeadNode *trash, int valnum) // 删除子句中的结点元素
{
    Node *pre, *current;
    pre = trash->child;
    current = pre->next;
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
        current = current->next;
    }
    if (pre->data < 0)
        n = pre->data + valnum;
    else
        n = pre->data + valnum - 1;
    Update(ltr, pre, n);
    delete pre;
    trash->child = nullptr;
}
status CheckSolo(HeadNode *head) // 检查是否存在单子句
{
    HeadNode *ptr = head;
    while (ptr)
    {
        if (ptr->num == 1)
            return TRUE;
        ptr = ptr->next;
    }
    return FALSE;
}
status SoloClause(HeadNode *&head, Literal *ltr, int valnum, int *res, int &flag) // 对存在单子句的情况进行处理
{
    flag = 0;
    HeadNode *clausetrav = head;
    while (clausetrav)
    {
        if (clausetrav->num == 1)
        {
            /* 删除单子句 并在答案中赋值 */
            int n;
            if (clausetrav->child->data < 0)
            {
                n = clausetrav->child->data + valnum;
                res[-clausetrav->child->data - 1] = -1;
            }
            else
            {
                n = clausetrav->child->data + valnum - 1;
                res[clausetrav->child->data - 1] = 1;
            }
            HeadNode *pre, *cur;
            Literal *find = ltr[n].next;
            while (find)
            {
                if (find->pos == clausetrav->child)
                    break;
                find = find->next;
            }
            if (!find)
                return FALSE;
            pre = find->uncle;
            cur = find->parent;

            if (cur->next)
            {
                Node *tochange = cur->next->child;
                while (tochange)
                {
                    int i;
                    if (tochange->data < 0)
                        i = tochange->data + valnum;
                    else
                        i = tochange->data + valnum - 1;
                    Literal *p = ltr[i].next;
                    while (p)
                    {
                        if (p->pos == tochange)
                            break;
                        p = p->next;
                    }
                    p->uncle = pre;
                    tochange = tochange->next;
                }
            }

            DeleteNode(ltr, clausetrav, valnum);
            if (pre == nullptr)
            {
                head = cur->next;
                clausetrav = cur->next;
            }
            else
            {
                pre->next = cur->next;
                clausetrav = pre; // 更新clausetrav
            }
            delete cur;
            if (head == nullptr)
                return TRUE;

            /* 删除含有单子句元素的子句 */
            Literal *ltrp = ltr[n].next;
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
                        clausetrav = current->next;
                }

                /* 更新待删除句子后一句子的文字地址库 */
                if (current->next)
                {
                    Node *tochange = current->next->child;
                    while (tochange)
                    {
                        int i;
                        if (tochange->data < 0)
                            i = tochange->data + valnum;
                        else
                            i = tochange->data + valnum - 1;
                        Literal *p = ltr[i].next;
                        while (p)
                        {
                            if (p->pos == tochange)
                                break;
                            p = p->next;
                        }
                        p->uncle = pre;
                        tochange = tochange->next;
                    }
                }
                DeleteNode(ltr, current, valnum);
                if (pre == nullptr)
                    head = current->next;
                else
                    pre->next = current->next;
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
                ltrp->parent->num--;

                /* 此 处 判 断 该 CNF 文 件 是 否 有 解 */
                if (ltrp->parent->num == 0)
                {
                    if (clausetrav->child->data < 0)
                        res[-clausetrav->child->data - 1] = 0;
                    else
                        res[clausetrav->child->data - 1] = 0;
                    Destroy(head, ltr, valnum);
                    return NORESULT;
                }
                else if (ltrp->parent->num == 1)
                    flag = 1;

                Node *pre, *current;
                pre = ltrp->pre;
                current = ltrp->pos;

                /* 删除反值元素时，更新文字地址库 */
                int tochange;
                if (current->next)
                {
                    if (current->next->data < 0)
                        tochange = current->next->data + valnum;
                    else
                        tochange = current->next->data + valnum - 1;
                    Update(ltr, ltrp, current->next, tochange);
                }

                if (pre == nullptr)
                {
                    ltrp->parent->child = current->next;
                    Update(ltr, current, m);
                    delete current;
                }
                else
                {
                    pre->next = current->next;
                    Update(ltr, current, m);
                    delete current;
                }
                ltrp = ltr[m].next;
            }
        }
        clausetrav = clausetrav->next;
    }
    return TRUE;
}