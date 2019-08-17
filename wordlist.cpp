
#include"wordlist.h"
//@author:Ïö

WordList::WordList() {}

WordList::~WordList() {}

void WordList::addword(short id, char state, char times,short date)
{
	Wordnode a;
	a.id= id;
	a.st = state;
	a.ti = times;
	a.da = Date(date);
	this->Slist::operator+(a);
	switch (state)
	{
	case Wordnode::undone: undone_t++; break;
	case Wordnode::done:done_t++; break;
	case Wordnode::wrong:wrong_t++; break;
	case Wordnode::right:right_t++;
	default:
		break;
	}
}

void WordList::addword(Wordnode w) 
{
	
	this->Slist::operator+(w);
	switch (w.st)
	{
	case Wordnode::undone: undone_t++; break;
	case Wordnode::done:done_t++; break;
	case Wordnode::wrong:wrong_t++; break;
	case Wordnode::right:right_t++;
	default:
		break;
	}
}

Wordnode WordList::pop() 
{
	Wordnode word_obj=Slist::access(0)->data;
	return word_obj;
}

int WordList::count_total()
{
	return Slist::total;
}

int WordList::count_undone()
{
	return undone_t;
}

int WordList::count_done()
{
	return done_t;
}

int WordList::count_wrong()
{
	return wrong_t;
}

int WordList::count_right()
{
	return right_t;
}


void WordList_daily::check(bool correct)
{
	Wordnode* word_obj = &(Slist::access(0)->data);
	word_obj->da.set_date();
	if(!(word_obj->da.istoday()))
	{
		word_obj->st = Wordnode::undone;
	}	
	if(correct)
	{
		switch (word_obj->st)
		{
		case Wordnode::undone:
			word_obj->st = Wordnode::done;
			undone_t--;
			done_t++;
			word_obj->ti++;
			break;
		case Wordnode::done:
			break;
		case Wordnode::wrong:
			word_obj->st = Wordnode::right;
			wrong_t--; 
			right_t++;
			break;
		case Wordnode::right:
			word_obj->st = Wordnode::done;
			right_t--;
			done_t++;
			word_obj->ti++;
		}
	}
	else
	{
		switch (word_obj->st)
		{
		case Wordnode::undone:
			word_obj->st = Wordnode::wrong;
			undone_t--;
			wrong_t++;
			break;
		case Wordnode::done:
			break;
		case Wordnode::wrong:
			break;
		case Wordnode::right:
			word_obj->st = Wordnode::wrong;
			right_t--;
			wrong_t++;
		default:
			break;
		}
	}
	void put_tail();
}


void WordList_daily::put_tail()
{
	Snode<Wordnode>* ptr = Slist::head->next;
	Slist::head = ptr->next;
	Slist::tail = Slist::tail->next = ptr;
	ptr->next = nullptr;
}

