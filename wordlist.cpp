#ifndef WORDLIST_CPP
#define WORDLIST_CPP
#include"wordlist.h"
//@author:Ïö

bool Wordnode::isEbbinghaus()
{
	short lastdate = da.get_short();
	Date d;
	d.set_date();
	short today = d.get_short();
	if ((ti == Wordnode::first && (today - lastdate == 1)) ||
		(ti == Wordnode::second && (today - lastdate == 1)) ||
		(ti == Wordnode::third && (today - lastdate == 2)) ||
		(ti == Wordnode::fourth && (today - lastdate == 3)))
		return true;
	return false;
}

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

int WordList_daily::count_new()
{
	return new_word;
}

void WordList_daily::addword(Wordnode w) 
{
	WordList::addword(w);
	if (w.ti == Wordnode::none)new_word++;
}

void WordList_daily::put_tail()
{
	Snode<Wordnode>* ptr = Slist::head->next;
	Slist::head = ptr->next;
	Slist::tail = Slist::tail->next = ptr;
	ptr->next = nullptr;
}
#endif