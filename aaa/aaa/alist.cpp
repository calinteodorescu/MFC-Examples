#include <cstdlib>
#include <cstdio>
#include "node.hpp"
void reverse(Node*& head) {
    if(head ==NULL)
        return;
    Node *pre, *cur, *ne;
    pre=head;
    cur=head->next;
    while(cur)
    {
        ne = cur->next;
        cur->next = pre;
        pre = cur;
        cur = ne;
    }
    head->next = NULL;
    head = pre;
}

//����һ�����õݹ�ķ��������ַ����Ļ���˼�����ڷ�ת��ǰ�ڵ�֮ǰ�ȵ��õݹ麯����ת�����ڵ㡣
//�������������һ��ȱ�㣬�����ڷ�ת������һ�������γ�һ���������Ա��뽫�����ķ��صĽڵ��next����ΪNULL��
//��ΪҪ�ı�headָ�룬�������������á�
Node* reverse(Node* p,Node*& head)
{
    if(p == NULL || p->next == NULL)
    {
        head=p;
        return p;
    }
    else
    {
        Node* tmp = reverse(p->next,head);
        tmp->next = p;
        return p;
    }
}

//(1)��֪�����ͷ���head,дһ������������������� ( Intel)

Node * ReverseList(Node *head) //��������
{
    if ( head == NULL || head->next == NULL )
        return head;
    Node *p1 = head ;
    Node *p2 = p1->next ;
    Node *p3 = p2->next ;
    p1->next = NULL ;
    while ( p3 != NULL )
    {
        p2->next = p1 ;
        p1 = p2 ;
        p2 = p3 ;
        p3 = p3->next ;
    }
    p2->next = p1 ;
    head = p2 ;
    return head ;
}
//(2)��֪��������head1 ��head2 ��������������Ǻϲ���һ��������Ȼ����(�������н�㣬�����С��ͬ��
Node * Merge(Node *head1 , Node *head2)
{
    if ( head1 == NULL)
        return head2 ;
    if ( head2 == NULL)
        return head1 ;
    Node *head = NULL ;
    Node *p1 = NULL;
    Node *p2 = NULL;
    if ( head1->data < head2->data )
    {
        head = head1 ;
        p1 = head1->next;
        p2 = head2 ;
    }
    else
    {
        head = head2 ;
        p2 = head2->next ;
        p1 = head1 ;
    }
    Node *pcurrent = head ;
    while ( p1 != NULL && p2 != NULL)
    {
        if ( p1->data <= p2->data )
        {
            pcurrent->next = p1 ;
            pcurrent = p1 ;
            p1 = p1->next ;
        }
        else
        {
            pcurrent->next = p2 ;
            pcurrent = p2 ;
            p2 = p2->next ;
        }
    }
    if ( p1 != NULL )
        pcurrent->next = p1 ;
    if ( p2 != NULL )
        pcurrent->next = p2 ;
    return head ;
}
//(3)��֪��������head1 ��head2 ��������������Ǻϲ���һ��������Ȼ�������Ҫ���õݹ鷽�����С� (Autodesk)

Node * MergeRecursive(Node *head1 , Node *head2)
{
    if ( head1 == NULL )
        return head2 ;
    if ( head2 == NULL)
        return head1 ;
    Node *head = NULL ;
    if ( head1->data < head2->data )
    {
        head = head1 ;
        head->next = MergeRecursive(head1->next,head2);
    }
    else
    {
        head = head2 ;
        head->next = MergeRecursive(head1,head2->next);
    }
    return head ;
}


///����ж�һ�����������л��ģ���ע�ⲻ���ñ�־λ�����ֻ������������ָ�룩
//һ��O��n���İ취���ǣ�������ָ�룬һ��ÿ�ε���һ����һ��ÿ�ε�������������л��Ļ����߱�Ȼ�غϣ���֮��Ȼ����
bool check(const Node* head)
{
    if(head==NULL) return false;
    const Node *low=head;
    Node *fast=head->next;
    while(fast!=NULL && fast->next!=NULL)
    {
        low=low->next;
        fast=fast->next->next;
        if(low==fast) return true;
    }
    return false;
}


///////////////////////////////

TNode* root=NULL;
void testappend()
{
   append(63);
   append(45);
   append(32);
   append(77);
   append(96);
   append(21);
   append(17); // Again, �����������
}
void append(int N)
{
   TNode* NewNode=(TNode *)malloc(sizeof(TNode));
   NewNode->value=N;
   
   if(root==NULL)
   {
      root=NewNode;
      return;
   }
   else
   {
      TNode* temp;
      temp=root;

      while((N>=temp->value && temp->left!=NULL) || (N !=NULL))
      {
         while(N>=temp->value && temp->left!=NULL)
            temp=temp->left;
         while(N)    temp=temp->right;//??
      }
      if(N>=temp->value)
         temp->left=NewNode;
      else
         temp->right=NewNode;
      return;
   }
} 