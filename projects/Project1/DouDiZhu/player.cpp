/*
	��������࣬������
	1.��ұ���Ľṹ���������磺���Ƽ��������ơ������ȣ�
	2.�˹�������غ������磺�������ơ������ư����������Ͳ�ּ���ϡ�
	  �жϽе���������Ӧ����ʲô�Ƶ��жϵȡ�
*/

#include <Windows.h>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include "cardgroup.h"
#include "game.h"
#include "player.h"
#include <stdexcept>

using namespace std;

Player::Player(Game &game)
: game(game)
, test(false)
, nodiscard(false)
, score(1000)
{

}
//��ʼ�µ�һ�֣���һЩ��ʼ�����ϵȵĲ���
void Player::NewGame()
{
	test = false;
	nodiscard = false;
	cards.clear();
	ClearAnalyse();
	selection.Clear();
	discard.Clear();
}
//��շ����Ƽ���
void Player::ClearAnalyse()
{
	if (analyse.empty())
		return;

	for (auto mem : analyse)
		delete mem;
	analyse.clear();

	return;
}
//�ص�����
bool Player::MyCompare(CardGroup *c1, CardGroup *c2)
{
	if (c1->type != c2->type)
		return c1->type < c2->type;
	else
		return c1->value < c2->value;
}

//�����Ƿ��뵱������������������
//����ԭ��ο���http://hi.baidu.com/lvhxwhcuygafpyr/item/1cf4b335205f78627c034ba9
//��Ϊ�ڶ������У������ը��������2������Ϊ�Ǵ��ƣ����Խ�����Ҫ������Щ�ƵĶ������жϡ�������һ���򵥵�ԭ��
//�ٶ����Ϊ8�֣�ը��Ϊ6�֣�����4�֣�С��3�֣�һ��2Ϊ2�֣��򵱷���
//���ڵ���7��ʱ��������
//���ڵ���5��ʱ�ж�����
//���ڵ���3��ʱ��һ����
//С�����ֲ��С�
int Player::GetBaseScore(int questioned, int nowscore)
{
	if (questioned == 2 && nowscore == 0)//���ǰ��λ��δ���ƣ�ֱ��3�ֵ��������㶮��~
		return 3;

	int sum = 0;
	map<int, int> needanalyse;//���������Ȩֵ-��������
	for (auto mem : cards)
		++needanalyse[CardGroup::Translate(mem)];//�������ƹ������������

	if (needanalyse.find(16) != needanalyse.end() &&
		needanalyse.find(17) != needanalyse.end())//������ը
		sum += 8;
	else if (needanalyse.find(16) != needanalyse.end())//һ��С��
		sum += 3;
	else if (needanalyse.find(17) != needanalyse.end())//һ�Ŵ���
		sum += 4;

	if (needanalyse.find(15) != needanalyse.end())//2������
		sum += 2 * needanalyse[15];

	for (auto mem : needanalyse){
		if (mem.second == 4)//ը��
			sum += 6;
	}
	int result;
	if (sum >= 7)
		result = 3;
	else if (sum >= 5 && sum < 7)
		result = 2;
	else if (sum >= 3 && sum < 5)
		result = 1;
	else
		result = 0;
	return (result > nowscore ? result : 0);
}
//����ѡ���Ƿ���Ϲ涨
bool Player::IsValid()
{
	if (game.lastone && game.lastone->discard.count != selection.count &&
		selection.count != 4 && selection.count != 2)//���ƣ������������Ҳ�����Ϊը��
		return false;

	selection.type = Unkown;
	AnalyseSelection();//������ѡ�Ƶ����ͼ�Ȩֵ

	if (selection.type == Unkown)//��ѡ�Ʋ����Ϲ涨
		return false;

	if (game.lastone){
		if (selection.type == Bomb &&
			(game.lastone->discard.type != Bomb ||
			selection.value > game.lastone->discard.value))
			return true;
		if (selection.type != game.lastone->discard.type ||
			selection.count != game.lastone->discard.count)//���Ͳ�������������
			return false;
		if (selection.value <= game.lastone->discard.value)//ѡ�Ʋ������ϼ���
			return false;
	}
	return true;
}
//��ѡ�ƽ��з���
void Player::AnalyseSelection()
{
	int NumMax = 0,//ͬ������������
		ValueMax = 0;//������������Ȩֵ

	//�ж��Ƿ�Ϊ��ը
	if (selection.count == 2 &&
		selection.group.find(16) != selection.group.end() &&
		selection.group.find(17) != selection.group.end()){
		selection.type = Bomb;
		selection.value = 17;
		return;
	}
	//�ҳ���ͬ�������������������Ȩֵ
	for (auto mem : selection.group){
		if (mem.second >= NumMax && mem.first > ValueMax){
			NumMax = mem.second;
			ValueMax = mem.first;
		}
	}
	//����������ͬ����������ж�����
	switch (NumMax){
	case 4:
		if (selection.count == 4){//ը��
			selection.type = Bomb;
			selection.value = ValueMax;
			return;
		}
		if (selection.count == 6){//�Ĵ�����
			selection.type = FourSeq;
			selection.value = ValueMax;
			return;
		}
		if (selection.count == 8){//�Ĵ�����
			for (auto mem : selection.group){
				if (mem.second != 2 && mem.second != 4)//���治�Ϲ�
					return;
			}
			selection.type = FourSeq;
			selection.value = ValueMax;
			return;
		}
		return;//���治�Ϲ�
	case 3:
	{
			  if (selection.count == 3){//����
				  selection.type = Three;
				  selection.value = ValueMax;
				  return;
			  }
			  if (selection.count == 4){//����һ��
				  selection.type = ThreePlus;
				  selection.value = ValueMax;
				  return;
			  }
			  if (selection.count == 5){//��������
				  for (auto mem : selection.group){
					  if (mem.second != 3 && mem.second != 2)
						  return;
				  }
				  selection.type = ThreePlus;
				  selection.value = ValueMax;
				  return;
			  }
			  int begin = 0, n = 0;
			  for (auto mem : selection.group){//�ж�������3��������������
				  if (mem.second == 3){
					  if (!begin || begin == mem.first)
						  ++n;
					  if (!begin)
						  begin = mem.first;
					  if (begin != mem.first && n == 1){
						  n = 1;
						  begin = mem.first;
					  }
					  ++begin;
				  }
			  }
			  if (selection.count == 3 * n){//��˳
				  selection.type = ThreeSeq;
				  selection.value = ValueMax;
				  return;
			  }
			  if (selection.count == 4 * n){//�ɻ������ŵĳ��
				  selection.type = Airplane;
				  selection.value = ValueMax;
				  return;
			  }
			  if (selection.count == 5 * n){//�ɻ������ӳ��
				  for (auto mem : selection.group){
					  if (mem.second != 2 && mem.second != 3)//�Ʋ��Ϲ�
						  return;
				  }
				  selection.type = Airplane;
				  selection.value = ValueMax;
				  return;
			  }
			  return;//�Ʋ��Ϲ�
	}
	case 2:
		if (selection.count == 2){//һ��
			selection.type = Double;
			selection.value = ValueMax;
			return;
		}
		if (selection.count >= 6 && !(selection.count % 2)){//����
			int begin = 0;
			for (auto mem : selection.group){//ȷ�����������ģ����Ҷ��ǳɶԵ�
				if (!begin)
					begin = mem.first;
				if (begin++ != mem.first || mem.second != 2)//�Ʋ����Ϲ涨
					return;
			}
			selection.type = DoubleSeq;
			selection.value = ValueMax;
			return;
		}
		return;//�Ʋ����Ϲ涨
	case 1:
		if (selection.count == 1){//����
			selection.type = Single;
			selection.value = ValueMax;
			return;
		}
		else if (selection.count >= 5){//�ж��Ƿ�Ϊ˳��
			int begin = 0;
			for (auto mem : selection.group){
				if (!begin)
					begin = mem.first;
				if (begin++ != mem.first || mem.first >= 15)//�Ʋ��������Ļ��ߴ���2�����ϵ���
					return;
			}
			selection.type = SingleSeq;//��˳
			selection.value = ValueMax;
			return;
		}
	default://���䣬�����Ϲ涨
		return;
	}

}

//����Ȩֵ���Ӽ����в�����Ӧ0-53���֣�Ȼ��Ӽ�����ɾ�������ظ����֣������ڻ���Ч����-1
int Player::ValueToNum(set<int> &cardscopy, int value)
{
	if (value<3 || value>17 || cardscopy.empty())
		throw runtime_error("Value not in set!");

	if (value == 16 && cardscopy.find(52) != cardscopy.end()){
		cardscopy.erase(52);
		return 52;
	}
	else if (value == 17 && cardscopy.find(53) != cardscopy.end()){
		cardscopy.erase(53);
		return 53;
	}
	else{
		for (int i = (value - 3) * 4, j = 0; j < 4; ++j){
			if (cardscopy.find(i + j) != cardscopy.end()){
				cardscopy.erase(i + j);
				return i + j;
			}

		}
		throw runtime_error("Value not in set!");
	}
}

//ɾ��������������Ϊ���Ԫ��
void Player::FreshenMap(map<int, int> &m)
{
	bool notcomplete = true;
	while (notcomplete){
		notcomplete = false;
		auto b = m.begin();
		for (; b != m.end(); ++b){
			if (b->second == 0){
				m.erase(b);
				notcomplete = true;
				break;
			}
		}
	}
}

//����������Ͳ���ɻ����Ƽ���
void Player::DivideIntoGroups(void)
{
	if (analyse.size())//���ͼ��Ϸǿգ�����
		return;

	set<int> cardscopy(cards);//���Ƹ���
	map<int, int> needanalyse;//���������Ȩֵ-��������

	for (auto mem : cardscopy)
		++needanalyse[CardGroup::Translate(mem)];//�������ƹ������������

	if (needanalyse.find(16) != needanalyse.end() &&
		needanalyse.find(17) != needanalyse.end()){//��������������ը
		CardGroup *c = new CardGroup(Bomb, 17);
		for (int i = 16; i < 18; ++i){
			c->AddNumber(ValueToNum(cardscopy, i));
			needanalyse.erase(i);
		}
		analyse.push_back(c);
	}

	for (auto mem : needanalyse){
		if (mem.second == 4){	//ը��
			CardGroup *c = new CardGroup(Bomb, mem.first);
			for (int i = 0; i < 4; ++i){
				c->AddNumber(ValueToNum(cardscopy, mem.first));
			}
			analyse.push_back(c);
			needanalyse[mem.first] = 0;
		}
	}
	//ɾ��������������Ϊ���Ԫ��
	FreshenMap(needanalyse);

	//��ǰ����2
	if (needanalyse.find(15) != needanalyse.end()){
		CardGroup *c = new CardGroup(Unkown, 15);
		int n = needanalyse[15];
		switch (n){
		case 3:
			c->type = Three;
			break;
		case 2:
			c->type = Double;
			break;
		case 1:
			c->type = Single;
			break;
		}
		for (int i = 0; i < n; ++i)
			c->AddNumber(ValueToNum(cardscopy, 15));
		needanalyse.erase(15);
		analyse.push_back(c);
	}
	//���ҵ�˳
	int begin, n;
	bool exist = true;
	while (exist && needanalyse.size()){
		begin = n = 0;
		for (auto b = needanalyse.begin(); b != needanalyse.end(); ++b){
			if (b->second > 0){//����Ϊ���Ԫ��
				if (!begin)
					begin = b->first;
				if (begin == b->first)
					++n;
				++begin;
			}
			if (n == 5){//������ɵ�˳������
				auto p = b;
				int first = p->first - 4;//��˳�ĵ�һ��
				CardGroup *c = new CardGroup(SingleSeq, p->first);
				for (first; first <= p->first; ++first){
					c->AddNumber(ValueToNum(cardscopy, first));
					--needanalyse[first];//��һ
				}
				analyse.push_back(c);
				exist = true;
				break;//�ӿ�ʼ���²���
			}
			//������������С����������¼����������ѵ�������������Բ�����
			auto end = needanalyse.end();
			if (begin - 1 != b->first || b == --end){
				if (b->second > 0){
					begin = b->first;
					++begin;
					n = 1;
				}
				else
					begin = n = 0;
				exist = false;
			}

		}
	}

	//ɾ��������������Ϊ���Ԫ��
	FreshenMap(needanalyse);
	//����ܣ���������˳�����ʣ����
	for (auto mem : analyse){
		if (mem->type == SingleSeq){//���ÿ����˳
			for (auto m : needanalyse){
				if (m.second > 0 && m.first == mem->value + 1){//ʣ�����л��бȵ�˳����һ����
					mem->AddNumber(ValueToNum(cardscopy, m.first));
					++mem->value;
					--needanalyse[m.first];

				}
			}
		}
	}
	//ɾ��������������Ϊ���Ԫ��
	FreshenMap(needanalyse);

	//�����е�˳���п��ԶԽӳɸ����ĵ�˳����������˳Ԫ����ͬ����ϳ�˫˳
	for (auto mem1 : analyse){
		if (mem1->type == SingleSeq){//��˳1
			for (auto mem2 : analyse){
				if (mem2->type == SingleSeq && mem1 != mem2){//��˳2���Һ͵�˳1����ͬһ��
					if (mem1->value < mem2->value){//mem1��ǰ
						if (mem1->value == mem2->value - mem2->count){//����ƴ��
							for (auto m : mem2->cards)
								mem1->AddNumber(m);
							mem1->value = mem2->value;
							mem2->type = Unkown;
						}
					}
					else if (mem1->value > mem2->value){//mem1�ں�
						if (mem2->value == mem1->value - mem1->count){
							for (auto m : mem1->cards)
								mem2->AddNumber(m);
							mem2->value = mem1->value;
							mem1->type = Unkown;
						}
					}
					else{//�����Ƿ���ȫһ�������Ժϲ���˫˳
						if (mem1->count == mem2->count){
							for (auto m : mem2->cards)
								mem1->AddNumber(m);
							mem1->type = DoubleSeq;
							mem2->type = Unkown;
						}
					}
				}
			}
		}
	}
	if (needanalyse.empty()){//���������ѿգ�����
		DeleteUnkown();
		sort(analyse.begin(), analyse.end(), MyCompare);
		return;
	}

	//˫˳��ֻ�����������ڵ���2�������ƣ�����3����������
	begin = n = 0;
	auto last = --needanalyse.end();
	for (auto b = needanalyse.begin(); b != needanalyse.end(); ++b){
		if (b->second >= 2){
			if (!begin)
				begin = b->first;
			if (begin == b->first)
				++n;
			++begin;
		}
		if (begin && begin - 1 != b->first || b == last){//������֮ǰ��������,���ѵ��������
			if (n >= 3){
				auto p = b;
				if (begin - 1 != b->first)
					--p;
				CardGroup *c = new CardGroup(DoubleSeq, p->first);
				for (int i = n; i > 0; --i, --p){
					for (int j = 0; j < 2; ++j){
						c->AddNumber(ValueToNum(cardscopy, p->first));
						--p->second;
					}
				}
				analyse.push_back(c);
			}
			if (b->second >= 2){
				n = 1;//��ǰ���������������ϵ�
				begin = b->first;
				++begin;
			}
			else{
				n = 0;
				begin = 0;
			}
		}
	}

	//ɾ��������������Ϊ���Ԫ��
	FreshenMap(needanalyse);

	//��˳
	//�����Ƿ����غϵĵ�˳��˫˳��ϳ���˳
	for (auto mem1 : analyse){
		if (mem1->type == SingleSeq){
			for (auto mem2 : analyse){
				if (mem2->type == DoubleSeq){
					if (mem1->value == mem2->value && mem1->count * 2 == mem2->count){
						for (auto m : mem1->cards)
							mem2->AddNumber(m);
						mem2->type = ThreeSeq;
						mem1->type = Unkown;
					}
				}
			}
		}
	}

	if (needanalyse.empty()){
		DeleteUnkown();
		sort(analyse.begin(), analyse.end(), MyCompare);
		return;
	}
	//ʣ�����в�����˳
	begin = n = 0;
	last = --needanalyse.end();
	for (auto b = needanalyse.begin(); b != needanalyse.end(); ++b){
		if (b->second == 3){
			if (!begin)
				begin = b->first;
			if (begin == b->first)
				++n;
			++begin;
		}
		if (begin && begin - 1 != b->first || b == last){//������֮ǰ��������,���ѵ��������
			if (n >= 2){//����2�鼰����
				auto p = b;
				if (begin - 1 != b->first)
					--p;
				CardGroup *c = new CardGroup(ThreeSeq, p->first);
				for (int i = n; i > 0; --i, --p){
					for (int j = 0; j < 3; ++j){
						c->AddNumber(ValueToNum(cardscopy, p->first));
						--p->second;
					}
				}
				analyse.push_back(c);
				if (b->second == 3){//��ǰ������Ϊ3�ţ�
					n = 1;
					begin = b->first;
					++begin;
				}
				else{
					n = 0;
					begin = 0;
				}
			}
		}
	}
	//����
	for (auto mem : needanalyse){
		if (mem.second == 3){
			CardGroup *c = new CardGroup(Three, mem.first);
			for (int i = 0; i < 3; ++i)
				c->AddNumber(ValueToNum(cardscopy, mem.first));
			needanalyse[mem.first] = 0;
			analyse.push_back(c);
		}
	}

	//����
	for (auto mem : needanalyse){
		if (mem.second == 2){
			CardGroup *c = new CardGroup(Double, mem.first);
			for (int i = 0; i < 2; ++i)
				c->AddNumber(ValueToNum(cardscopy, mem.first));
			needanalyse[mem.first] = 0;
			analyse.push_back(c);
		}
	}
	//ɾ��������������Ϊ���Ԫ��
	FreshenMap(needanalyse);

	//����
	for (auto mem : needanalyse){
		if (mem.second != 1)
			throw runtime_error("Still has singleness card");
		CardGroup *c = new CardGroup(Single, mem.first);
		c->AddNumber(ValueToNum(cardscopy, mem.first));
		needanalyse[mem.first] = 0;
		analyse.push_back(c);
	}
	//ɾ��������������Ϊ���Ԫ��
	FreshenMap(needanalyse);

	DeleteUnkown();
	sort(analyse.begin(), analyse.end(), MyCompare);

}

//����������˳���Ƴ�����һ�ͷɻ������ҵ��ƣ����Ҷ��ӣ��������ͱ���ԭ��
void Player::ThreeplusAndAirplane()
{
	int n,
		doublecount = 0,//ͳ�ƶ��ӵ��������������������
		singlecount = 0;//ͳ�Ƶ�������

	for (auto mem : analyse){
		if (mem->type == Single)
			++singlecount;
		else if (mem->type == Double)
			++doublecount;
	}

	for (auto mem : analyse){//���Ʒɻ�
		if (mem->type == ThreeSeq){
			n = mem->count / 3;
			if (singlecount >= n){
				for (auto temp : analyse){
					if (temp->type == Single){
						for (auto m : temp->cards)
							mem->AddNumber(m);
						temp->type = Unkown;
						--singlecount;
						--n;
					}
					if (!n){
						mem->type = Airplane;
						break;
					}
				}
			}
			else if (doublecount >= n){
				for (auto temp : analyse){
					if (temp->type == Double){
						for (auto m : temp->cards)
							mem->AddNumber(m);
						temp->type = Unkown;
						--doublecount;
						--n;
					}
					if (!n){
						mem->type = Airplane;
						break;
					}
				}
			}
		}
	}
	for (auto mem : analyse){//��������һ
		if (mem->type == Three){
			if (singlecount){
				for (auto temp : analyse){
					if (temp->type == Single){
						for (auto m : temp->cards)
							mem->AddNumber(m);
						temp->type = Unkown;
						--singlecount;
						mem->type = ThreePlus;
						break;
					}
				}
			}
			else if (doublecount){
				for (auto temp : analyse){
					if (temp->type == Double){
						for (auto m : temp->cards)
							mem->AddNumber(m);
						temp->type = Unkown;
						--doublecount;
						mem->type = ThreePlus;
						break;
					}
				}
			}
		}
	}
}

//ɾ������δ֪���͵�����
void Player::DeleteUnkown(void)
{
	auto b = analyse.begin();
	while (b != analyse.end()){
		if ((*b)->type == Unkown){
			delete *b;
			b = analyse.erase(b);
		}
		else
			++b;
	}
}

//����ѡ��
void Player::SelectCards(bool hint)
{
	if (analyse.empty())//�Ƿ���Ҫ���·�������
		DivideIntoGroups();
	ThreeplusAndAirplane();
	DeleteUnkown();
	sort(analyse.begin(), analyse.end(), MyCompare);

	if (analyse.size() == 2){//����Ϊ2�������ʺϵ�ը��ֱ�ӳ�
		for (auto mem : analyse){
			if (mem->type == Bomb){
				if (game.lastone != nullptr &&//����Լ��ǽӱ��˵���
					game.lastone->discard.type == Bomb &&//����������Ϊը����
					mem->value <= game.lastone->discard.value)//���Լ���ը�������ڶԷ�ʱ��
					continue;//����ѡ�����
				selection = *mem;
				return;
			}
		}
	}
	
	if (game.lastone == nullptr)
		Myself();//ֱ�ӳ���
	else if (!hint && this != game.landlord && game.lastone != game.landlord)
		Friend();//���ѷ��ƣ������Ƶ����ѷ�,���Ҳ�����ʾ
	else
		Enemy(hint);//���з����ƻ���ʾ
}

void Player::Myself()
{
	if (analyse.size() == 1){//ʣ���һ����
		selection = *analyse[0];
		return;
	}

	if (analyse.size() == 2){//ʣ�����ƣ�����������
		//���鿴������������ƣ�ֻΪ����ʣ�����е�����
		int maxNum = 0;
		Player *p = game.ProPlayer();
		if (*p->cards.rbegin() > maxNum)
			maxNum = *p->cards.rbegin();
		p = game.NextPlayer();
		if (*p->cards.rbegin() > maxNum)
			maxNum = *p->cards.rbegin();
		for (auto mem : analyse){//��������б�ʣ���ƻ����һ���ƣ����ȴ������
			if (mem->value > CardGroup::Translate(maxNum)){
				selection = *mem;
				return;
			}
		}
		//���򣬴��������������
		selection = *analyse[1];
		return;
	}
	if (game.NextPlayer()->cards.size() == 1){//�¼�������Ϊ1
		if (this != game.landlord && game.lastone != game.landlord){//�¼�Ϊ�ѷ�
			//û������¼��ƣ��ʹ����С��һ�ţ��������������
			if (!test){
				if (analyse[0]->type == Single &&
					analyse[0]->value == CardGroup::Translate(*(cards.begin()))){
					selection = *analyse[0];
					return;
				}
				else{
					selection.AddNumber(*(cards.begin()));
					selection.type = Single;
					selection.value = selection.group.begin()->first;
					//�����ˣ�Ҫ���·�����
					ClearAnalyse();
					return;
				}

			}

		}
		else{//�¼�Ϊ�з�ʣ1��
			//��ʵ�֣���
			//˼·�Ǿ����������ƣ�ֻ�е����˵Ļ��ʹӴ�С��
		}
	}
	//����˳����ƣ�(A���ϵ��ƾ�����ֱ�ӳ���ը����ֱ�ӳ�)
	//���ơ����ӡ�˫˳����˳������������һ���ɻ�
	for (auto mem : analyse){
		if ((mem->type == Single || mem->type == Double) &&
			mem->value >= 15 || mem->type == Bomb)
			continue;
		selection = *mem;
		return;
	}
	selection = *analyse[0];
	return;
}

void Player::Friend()
{
	if (game.lastone != game.landlord && game.ProPlayer() == game.landlord){
		return;//�ϼ�Ϊ�������������Ʒ�Ϊ�ѷ����򲻳���
	}
	for (auto mem : analyse){//������Ӧ��
		if (mem->type == game.lastone->discard.type &&
			mem->count == game.lastone->discard.count &&
			mem->value > game.lastone->discard.value){

			selection = *mem;
			break;
		}
	}
	if (analyse.size() > 2 && selection.value > 14)
		selection.Clear();//������������2��������ѡ��Ȩֵ����14��A�����򲻳���
	return;
}

void Player::Enemy(bool hint)
{
	auto lastdiscard = game.lastone->discard;//�з�����

	//��ɻ�����
	ClearAnalyse();
	DivideIntoGroups();
	sort(analyse.begin(), analyse.end(), MyCompare);

	for (auto mem : analyse){//�鿴�Ƿ�����Ӧ�ƣ�����Ȩֵ��
		if (mem->type == lastdiscard.type &&
			mem->count == lastdiscard.count &&
			mem->value > lastdiscard.value){

			selection = *mem;
			return;
		}
	}
	//��Ҫ����
	switch (lastdiscard.type){
	case Single://�з������ǵ���
		NeedSigle();
		break;
	case Double:
		NeedDouble();
		break;
	case SingleSeq:
		NeedSigleSeq();
		break;
	case Three:
		break;
	case ThreePlus://����һ
		NeedThreePlus();
		break;
	case Airplane://�ɻ�����Ҫ���
		NeedAirplane();
		break;
	default:
		break;
	}
	if (selection.count)
		return;
	//�з�ʣһ���ƣ������ʺϵ�ը�����ͳ�ը��
	if (hint || lastdiscard.count > 3 || lastdiscard.value > 14){
		for (auto mem : analyse){
			if (mem->type == Bomb){
				if (game.lastone->discard.type == Bomb &&//�������������Ϊը����
					mem->value <= game.lastone->discard.value)//���Լ���ը�������ڶԷ�ʱ��
					continue;//����ѡ�����
				selection = *mem;
				return;
			}
		}
	}
	return;
}

void Player::NeedSigle()
{
	auto lastdiscard = game.lastone->discard;//�з�����

	for (auto mem : analyse){
		if (mem->type == SingleSeq && mem->count > 5){//����,��˳��������5��
			if (mem->group.begin()->first > lastdiscard.value){
				selection.AddNumber(*mem->cards.begin());
				selection.value = mem->group.begin()->first;
				selection.type = Single;
				ClearAnalyse();//�����ˣ�һ��Ҫ���
				return;
			}
			else if (mem->group.rbegin()->first > lastdiscard.value){
				selection.AddNumber(*(mem->cards.rbegin()));
				selection.value = mem->value;
				selection.type = Single;
				ClearAnalyse();//�����ˣ�һ��Ҫ���
				return;
			}
		}
	}
	for (auto mem : analyse){
		if (mem->type == Three){//���,������
			if (mem->group.begin()->first > lastdiscard.value){
				selection.AddNumber(*mem->cards.begin());
				selection.value = mem->group.begin()->first;
				selection.type = Single;
				ClearAnalyse();//�����ˣ�һ��Ҫ���
				return;
			}
		}
	}
	for (auto mem : analyse){
		if (mem->type == Double){//����,�����
			if (mem->group.begin()->first > lastdiscard.value){
				selection.AddNumber(*mem->cards.begin());
				selection.value = mem->group.begin()->first;
				selection.type = Single;
				ClearAnalyse();//�����ˣ�һ��Ҫ���
				return;
			}
		}
	}
}

void Player::NeedDouble()
{
	auto lastdiscard = game.lastone->discard;//�з�����

	for (auto mem : analyse){
		if (mem->type == Three){//������
			if (mem->group.begin()->first > lastdiscard.value){
				auto b = mem->cards.begin();
				for (int i = 0; i < 2; ++i)
					selection.AddNumber(*b++);
				selection.value = mem->group.begin()->first;
				selection.type = Double;
				ClearAnalyse();//�����ˣ�һ��Ҫ���
				return;
			}
		}
	}
	for (auto mem : analyse){
		int i = 0, m = 0;
		if (mem->type == ThreeSeq){//����˳
			if (mem->group.begin()->first > lastdiscard.value){
				auto b = mem->cards.begin();
				for (int i = 0; i < 2; ++i)
					selection.AddNumber(*b++);
				selection.value = mem->group.begin()->first;
				selection.type = Double;
				ClearAnalyse();//�����ˣ�һ��Ҫ���
				return;
			}
			else if (mem->group.rbegin()->first > lastdiscard.value){
				selection.AddNumber(*(mem->cards.rbegin()));
				selection.value = mem->value;
				selection.type = Double;
				ClearAnalyse();//�����ˣ�һ��Ҫ���
				return;
			}
		}
	}
}

void Player::NeedSigleSeq()
{
	auto lastdiscard = game.lastone->discard;//�з�����

	for (auto mem : analyse){
		if (mem->type == SingleSeq &&
			mem->value > lastdiscard.value &&
			mem->count > lastdiscard.count){//������ĵ�˳
			if (mem->count - (mem->value - lastdiscard.value) >= lastdiscard.count){
				//����˳�ǴӶ̵�˳�Ŀ�ʼ��Ԫ�ػ��С��Ԫ�ؿ�ʼ��
				for (int i = lastdiscard.value - lastdiscard.count + 2, j = 0;
					j < lastdiscard.count; ++j)
					selection.AddNumber(ValueToNum(mem->cards, i + j));
				selection.value = lastdiscard.value + 1;
				selection.type = SingleSeq;
				ClearAnalyse();//�����ˣ�һ��Ҫ���
				return;
			}
			else{//����˳�Ŀ�ʼԪ�رȶ̵�˳�Ŀ�ʼԪ�ش�
				int i = 0;
				auto b = mem->cards.begin();
				for (; i < lastdiscard.count; ++i, ++b)
					selection.AddNumber(*b);
				selection.value = CardGroup::Translate(*--b);
				selection.type = SingleSeq;
				ClearAnalyse();//�����ˣ�һ��Ҫ���
				return;
			}
		}
	}
}

void Player::NeedThreePlus()
{
	auto b = analyse.begin();
	for (; b != analyse.end(); ++b){//���ұȶԷ�������ͬ�Ƶ�����������
		if ((*b)->type == Three && (*b)->value > game.lastone->discard.value){
			break;
		}
	}
	if (b == analyse.end())//���û��
		return;//����
	if (game.lastone->discard.count == 4){//������Ϊ����һ��
		if (analyse[0]->type == Single){//�е���
			for (auto m : analyse[0]->cards)
				(*b)->AddNumber(m);
			(*b)->type = ThreePlus;
			analyse[0]->type = Unkown;
			selection = **b;
			return;
		}
		else{//��Ҫ����
			for (auto mem : analyse){
				if (mem->type == SingleSeq && mem->count > 5){//����,��˳��������5��
					selection = **b;
					selection.AddNumber(*mem->cards.begin());
					selection.type = ThreePlus;
					ClearAnalyse();//�����ˣ�һ��Ҫ���
					return;
				}
			}
			for (auto mem : analyse){
				if (mem->type == Three && mem != *b && mem->value < 14){//���,������
					selection = **b;
					selection.AddNumber(*mem->cards.begin());
					selection.type = ThreePlus;
					ClearAnalyse();//�����ˣ�һ��Ҫ���
					return;
				}
			}
			for (auto mem : analyse){
				if (mem->type == Double && mem->value < 14){//����,�����
					selection = **b;
					selection.AddNumber(*mem->cards.begin());
					selection.type = ThreePlus;
					ClearAnalyse();//�����ˣ�һ��Ҫ���
					return;
				}
			}
		}
	}
	else{//����һ��
		for (auto mem : analyse){//���Ҷ���
			if (mem->type == Double && mem->value < 14){
				for (auto m : mem->cards)
					(*b)->AddNumber(m);
				(*b)->type = ThreePlus;
				mem->type = Unkown;
				selection = **b;
				return;
			}
		}
		for (auto mem : analyse){
			if (mem->type == Three && mem != *b && mem->value < 14){//���,������
				selection = **b;
				for (int i = 0; i < 3; ++i)
					selection.AddNumber(*mem->cards.begin());
				selection.type = ThreePlus;
				ClearAnalyse();//�����ˣ�һ��Ҫ���
				return;
			}
		}
	}
}

void Player::NeedAirplane()
{
	ClearAnalyse();
	DivideIntoGroups();
	sort(analyse.begin(), analyse.end(), MyCompare);

	int wing = 0,//�������
		n = 0;//��˳�������Ƶĸ���
	for (auto mem : game.lastone->discard.group){
		if (mem.second == 3)
			++n;
	}
	if (game.lastone->discard.count == 5 * n)//�ɻ����Ϊ����
		wing = 2;
	else{//�ɻ����Ϊ����
		while (game.lastone->discard.count != 4 * n)
			--n;
		wing = 1;
	}
	auto b = analyse.begin();
	for (; b != analyse.end(); ++b){
		if ((*b)->type == ThreeSeq &&
			(*b)->count == 3 * n &&
			(*b)->value > game.lastone->discard.value)
			break;
	}
	if (b == analyse.end())
		return;
	int count = 0;
	for (auto mem : analyse){
		if (mem->type == (wing == 1 ? Single : Double))
			++count;
	}
	if (count < n)
		return;
	for (auto mem : analyse){
		if (mem->type == (wing == 1 ? Single : Double)){
			for (auto m : mem->cards)
				(*b)->AddNumber(m);
			mem->type = Unkown;
			--n;
		}
		if (!n)
			break;
	}
	(*b)->type = Airplane;
	selection = **b;
	return;
}

//���Ʋ����÷�������
bool Player::DiscardAndClear()
{
	discard = selection;//��ѡ�Ʒ�������������ѡ��
	bool needclear = true;//���γ����Ƿ�Ϊ���ƣ���Ҫ���·����ƶ�
	for (auto b = analyse.begin(); b != analyse.end(); ++b){
		if ((*b)->type == selection.type &&
			(*b)->value == selection.value &&
			(*b)->count == selection.count){//���ǲ���
			delete (*b);
			analyse.erase(b);
			needclear = false;//����Ҫ���
			break;
		}
	}
	if (needclear)//��Ҫ��գ��´γ���Ҫ���·���
		ClearAnalyse();

	for (auto mem : selection.cards){
		cards.erase(mem);//��������ɾ�������
	}
	selection.Clear();//���ѡ����
	return true;
}
//���Գ���
bool Player::Discard(void)
{
	if (selection.count == 0){//����ѡ����Ϊ�գ�˵������
		nodiscard = true;
		return false;
	}
	//�����������
	return DiscardAndClear();
}
//��ҳ���
bool Player::HumanDiscard()
{
	if (!IsValid()){//ѡ�Ʋ����Ϲ涨
		selection.Clear();//���ѡ��
		return false;//�������
	}
	//��������������������Ƿ����
	return DiscardAndClear();
}
//����
void Player::Pass(void)
{
	nodiscard = true;
	selection.Clear();
	return;
}