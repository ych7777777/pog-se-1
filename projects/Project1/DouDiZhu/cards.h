#pragma once


//���ƶ���
class Cards{
public:
	Cards();
	void RandCards(void);//ϴ��
	int GetCard();//Ĩ��
	int GetRemain(void){ return remain; };

private:
	int cards[54];//������
	int remain;//ʣ������
};