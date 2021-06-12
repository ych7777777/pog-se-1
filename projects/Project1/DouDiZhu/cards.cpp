/*
��Ϸ������Ҫ���˿��࣬������ʾһ���˿ˡ�����ϴ�Ƽ����ƺ�����
*/

#include <stdexcept>
#include <random>
#include <time.h>
#include "cards.h"

using namespace std;

Cards::Cards()
{
	for (int i = 0; i < 54; ++i)
		cards[i] = i;//��ʼ��Ϊ0-53
	RandCards();//ϴ��һ�β������ƶ�ʣ������
	
	return;
}
//ϴ��
void Cards::RandCards(void)
{
	default_random_engine e(time(nullptr));//���������
	uniform_int_distribution<unsigned> u(0, 53);//���ͷֲ�

	for (int i = 0; i < 54; i++){
		int rand = u(e);
		swap(cards[i], cards[rand]);
	}
	remain = 54;
	
	return;
}
//Ĩ��
int Cards::GetCard(void)
{
	if (!remain)
		throw runtime_error("No more card in cardset!");

	return cards[--remain];
}