/*
	�洢���ϻ����Ƶ�һ���ƣ����������Ƶ����͡����桢�����ȡ�
*/

#include <map>
#include <set>
#include "cardgroup.h"


CardGroup::CardGroup()
:type(Unkown)
,value(0)
,count(0)
{

}

CardGroup::CardGroup(Type t,int v)
: type(t)
, value(v)
, count(0)
{

}

CardGroup &CardGroup::operator=(CardGroup &cg)
{
	this->group = cg.group;
	this->cards = cg.cards;
	this->type = cg.type;
	this->value = cg.value;
	this->count = cg.count;
	return *this;
}
//��������
void CardGroup::Clear(void)
{
	group.clear();
	cards.clear();
	type = Unkown;
	value = 0;
	count = 0;
	return;
}
//���0-53��ʾ����
void CardGroup::AddNumber(int num)
{
	++count;
	cards.insert(num);
	++group[Translate(num)];
}
//ȥ��һ����
void CardGroup::DeleteNumber(int num)
{
	if (cards.find(num) == cards.end())//ȷ��Ҫȥ�������ڽṹ��
		return;
	--count;
	cards.erase(num);
	if (--group[Translate(num)] == 0)
		group.erase(Translate(num));
}
//��0-53ת����3-17Ȩֵ������A��14����2��15����С����16����������17��
int CardGroup::Translate(int num)
{
	if (num < 52)
		return num / 4 + 3;
	else
		return num - 36;
}