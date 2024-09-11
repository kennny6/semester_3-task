#define _CRT_SECURE_NO_WARNINGS 1
#include "global.h"

void Update(Literal *ltr, Node *object, int n) // 删除整个子句时对子句下的元素进行更新
{
    ltr[n].n--;

    if (object->left)
    {
        Node *pre = object->left;
        pre->right = object->right;
    }
    if (object->right)
    {
        Node *next = object->right;
        next->left = object->left;
    }
    if (!object->left)
        ltr[n].next = object->right;
}
void DeleteNode(Literal *ltr, HeadNode *trash, int valnum) // 删除子句中的结点元素
{
    Node *pre, *current;
    pre = trash->down;
    current = pre->down;
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
        current = current->down;
    }
    if (pre->data < 0)
        n = pre->data + valnum;
    else
        n = pre->data + valnum - 1;
    Update(ltr, pre, n);
    delete pre;
    trash->down = nullptr;
}
status CheckSolo(HeadNode *head) // 检查是否存在单子句
{
    HeadNode *ptr = head;
    while (ptr)
    {
        if (ptr->data == 1)
            return TRUE;
        ptr = ptr->right;
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
            if (clausetrav->down->data < 0)
            {
                n = clausetrav->down->data + valnum;
                res[-clausetrav->down->data - 1] = -1;
            }
            else
            {
                n = clausetrav->down->data + valnum - 1;
                res[clausetrav->down->data - 1] = 1;
            }
            HeadNode *pre, *cur;
            if (!ltr[n].next)
                return FALSE;
            pre = clausetrav->down->uncle;
            cur = clausetrav->down->parent;

            if (cur->right)
            {
                Node *tochange = cur->right->down;
                while (tochange)
                {
                    tochange->uncle = pre;
                    tochange = tochange->down;
                }
            }

            DeleteNode(ltr, clausetrav, valnum);
            if (pre == nullptr)
            {
                head = cur->right;
                clausetrav = cur->right;
            }
            else
            {
                pre->right = cur->right;
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
                        clausetrav = current->right;
                }

                /* 更新待删除句子后一句子的文字地址库 */
                if (current->right)
                {
                    Node *tochange = current->right->down;
                    while (tochange)
                    {
                        tochange->uncle = pre;
                        tochange = tochange->down;
                    }
                }
                DeleteNode(ltr, current, valnum);
                if (pre == nullptr)
                    head = current->right;
                else
                    pre->right = current->right;
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
                    if (clausetrav->down->data < 0)
                        res[-clausetrav->down->data - 1] = 0;
                    else
                        res[clausetrav->down->data - 1] = 0;
                    Destroy(head, ltr, valnum);
                    return NORESULT;
                }
                else if (ltrp->parent->data == 1)
                    flag = 1;

                Node *pre, *current;
                pre = ltrp->pre;
                current = ltrp;

                /* 删除反值元素时，更新文字地址库 */
                if (current->down)
                {
                    current->down->pre = current->pre;
                }

                if (pre == nullptr)
                {
                    ltrp->parent->down = current->down;
                    Update(ltr, current, m);
                    delete current;
                }
                else
                {
                    pre->down = current->down;
                    Update(ltr, current, m);
                    delete current;
                }
                ltrp = ltr[m].next;
            }
        }
        clausetrav = clausetrav->right;
    }
    return TRUE;
}