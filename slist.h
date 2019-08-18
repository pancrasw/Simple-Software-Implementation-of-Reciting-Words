#ifndef SLIST_H
#define SLIST_H
//@author:霄
#include <iostream>
#include <time.h>
using namespace std;

template<typename Type>
struct Snode
{
	Type data;
	Snode* next = nullptr;
};

template<typename Type>
class SnodeArray //存放节点指针的数组
{
public:
	SnodeArray(int);
	~SnodeArray();
	Snode<Type>**head;
};

template<typename Type>
SnodeArray<Type>::SnodeArray(int count) 
{
	head = new Snode<Type>*[count];
}

template<typename Type>
SnodeArray<Type>::~SnodeArray()
{
	delete head;
}

template<typename Type>
class Slist
{
protected:
	Snode<Type>* head;//头指针
	Snode<Type>* tail;//尾指针
	//friend ostream& operator<<(ostream&, Slist<Type>);//友元，允许ostream的函数有权访问链表//lnk2019暂时错误不明
	int total = 0;	
public:
	Slist();
	Slist(Type*first, Type*last);
	~Slist();
	Snode<Type>* access(int n);//没有报错处理，访问第n+1个数据
	void plusmerge(Slist a, Slist b);
	void insert(Type n, int position);
	void insert(Snode<Type>*, Snode<Type>*, Type n);
	void show();//输出链表每个元素
	void delete_node(int n);//删除第n+1个元素
	void shuffle();//洗牌，打乱牌序
	int count();//元素总数
	Snode<Type> pop();//获取最后一个元素
	Slist& operator+(Type a);//末位加单词
};

template<typename Type>
void Slist<Type>::insert(Snode<Type>*a, Snode<Type>*b, Type n) //在两个相邻节点中间插入新节点
{
	a = a->next = new Snode<Type>;
	a->data = n;
	a->next = b;
	total++;
}

template<typename Type>
void Slist<Type>::insert (Type n, int position)//第一个传递所要插入的值，第二个输入插入的位置,即把插入元素设为第position个元素
{//无访问空节点报错
	Snode<Type>*p1 = this->head;//前指针指向哨兵位
	Snode<Type>*p2 = p1->next;//后指针
	for (int i = 0; i < position - 1; i++, p2 = p2->next, p1 = p1->next);//
	p1 = p1->next = new Snode<Type>;
	p1->data = n;
	p1->next = p2;
	total++;
}

template<typename Type>
void Slist<Type>::plusmerge(Slist a, Slist b) //两个有序正序链表合为一个有序正序链表，拷贝方法，加到末尾
{//直接隐性引用，不返回
	Snode<Type>*p1 = a.head->next;//指向第一个元素
	Snode<Type>*p2 = b.head->next;
	for (;;)
	{
		if (p1->data >= p2->data)
		{
			*this + p2->data;
			if (p2->next == nullptr)
			{
				for (; p1 != nullptr; p1 = p1->next) { *this + p1->data; }
				break;
			}
			else p2 = p2->next;
		}
		if (p1->data < p2->data)
		{
			*this + p1->data;
			if (p1->next == nullptr)
			{
				for (; p2 != nullptr; p2 = p2->next) { *this + p2->data; }
				break;
			}
			else p1 = p1->next;
		}
	}
}

//template<typename type>
//ostream& operator<<(ostream&a, Slist<type> b)//重载cout，输出链表全内容
//{
//	Snode<type>*p = b.head->next;//直接指向第一个元素，无报错处理
//	a << (p->data);
//	for (; p->next != nullptr;)
//	{
//		p = p->next;
//		a <<' '<< (p->data) ;
//	}
//	return a;
//}

template<typename type>
void Slist<type>::show()
{
	Snode<type>*p = this->head->next;//直接指向第一个元素，无报错处理
	cout << (p->data);
	for (; p->next != nullptr;)
	{
		p = p->next;
		cout << ' ' << (p->data);
	}
}

template<typename Type>
Slist<Type>::Slist()
{
	this->head = new Snode<Type>;
	this->head->next = nullptr;
	tail = this->head;
}

template<typename Type>
Slist<Type>::Slist(Type*first, Type*last)
{
	Type*p1 = first;
	this->head = new Snode<Type>;//创建哨兵位
	Snode<Type>*p2 = this->head->next = new Snode<Type>;//p2指向链表第一个元素
	for (;; p1++)
	{
		if (p1 == last)break;
		else
		{
			p2->data = *p1;
			p2 = p2->next = new Snode<Type>;
			total++;
		}
	}
	tail = p2;
}

template<typename Type>
Snode<Type>* Slist<Type>::access(int n)
{
	Snode<Type>*p = this->head->next;
	for (int i = 0; i < n; i++, p = p->next);
	return p;
}

template<typename Type>
Slist<Type>& Slist<Type>::operator+(Type a)//末位增添节点
{
	Snode<Type>*p = this->head;
	for (; p->next != nullptr; p = p->next);//读到末位节点
	p->next = new Snode<Type>;
	total++;
	p = p->next;
	p->data = a;
	p->next = nullptr;
	tail = p;
	return *this;
}

template<typename Type>
Slist<Type>::~Slist()
{
	Snode<Type>*p;
	while (head->next != nullptr)
	{
		p = head->next;
		head->next = head->next->next;
		delete p;
	}
}

template<typename Type>
void Slist<Type>::delete_node(int n)
{
	Snode<Type>*ptr_fast=this->head->next;
	Snode<Type>*ptr_slow = this->head;
	for (int i = 0; i < n; i++, ptr_slow = ptr_fast, ptr_fast = ptr_fast->next);
	ptr_slow->next = ptr_fast->next;
	delete ptr_fast;
	total--;
}

template<typename Type>
void Slist<Type>::shuffle()
{
	SnodeArray<Type> a(total);
	Snode<Type>*ptr = head->next;
	for (int i = 0; i < total; i++) 
	{
		a.head[i] = ptr;
		ptr = ptr->next;
	}
	srand((unsigned)time(0));
	Snode<Type>*mid = nullptr;
	int rad = 0;
	for (int i = 0; i < total - 3; i++) //数组洗牌
	{
		rad = rand() % (total - i - 2);
		mid = a.head[rad];
		a.head[rad] = a.head[total - i - 1];
		a.head[total - i - 1] = mid;		
	}
	ptr = head;
	for (int i = 0; i < total; i++) 
	{
		ptr = ptr->next = a.head[i];
		
		if (i == total - 1) 
		{
			tail = a.head[i];
			a.head[i]->next = nullptr;
		}
	}
}

template<typename Type>
int Slist<Type>::count() 
{
	return total;
}


template<typename Type>
Snode<Type> Slist<Type>::pop()
{
	return *tail;
}

#endif // !SLIST_H