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
            break;
        }
        pre = cur;
        cur = cur->next;
    }
}
void Update(Literal *ltr, Literal *target, Node *tochange, int n)//删除单个元素时，对下一元素的 pre 进行更新
{
    Literal *p = ltr[n].next;
    while (p)
    {
        if (p->pos == tochange)
            p->pre = target->pre;
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
    }
    return FALSE;
}
status SoloClause(HeadNode *head, Literal *ltr, int valnum) // 对存在单子句的情况进行处理
{
    HeadNode *clausetrav = head;
    while (clausetrav)
    {
        if (clausetrav->num == 1)
        {
            /*删除单子句*/
            int n;
            if (clausetrav->child->data < 0)
                n = clausetrav->child->data + valnum;
            else
                n = clausetrav->child->data + valnum - 1;
            HeadNode *pre, *cur;
            pre = ltr[n].uncle;
            cur = ltr[n].parent;
            DeleteNode(ltr, clausetrav, valnum);
            if (pre == nullptr)
                head = cur->next;
            else
                pre->next = cur->next;
            delete cur;

            if(head == nullptr)
                return TRUE;
            
            /*删除含有单子句元素的子句*/
            Literal *ltrp = ltr[n].next;
            while (ltrp)
            {
                HeadNode *pre, *current;
                pre = ltrp->uncle;
                current = ltrp->parent;
                DeleteNode(ltr, current, valnum);
                if (pre == nullptr)
                    head = current->next;
                else
                    pre->next = current->next;
                delete current;

                ltrp = ltrp->next;
            }

            if(head == nullptr)
                return TRUE;

            /*删除子句中含有的单子句元素反值*/
            int m;
            if (clausetrav->child->data > 0)
                m = -clausetrav->child->data + valnum;
            else
                m = -clausetrav->child->data + valnum - 1;
            ltrp = ltr[m].next;
            while (ltrp)
            {
                ltrp->parent->num--;

                /* 此 处 判 断 该 CNF 文 件 是 否 有 解 */
                if(ltrp->parent->num == 0)
                {
                    Destroy(head,ltr,valnum);
                    return NORESULT;
                }
                    
                
                Node *pre, *current;
                pre = ltrp->pre;
                current = ltrp->pos;

                /*删除反值元素时，更新文字地址库*/
                int tochange;
                if (current->next)
                {
                    if (current->next->data < 0)
                        tochange = current->next->data + valnum;
                    else
                        tochange = -current->next->data + valnum - 1;
                    Update(ltr,ltrp,current->next,tochange);
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
    }
}