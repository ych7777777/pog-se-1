#pragma once

#include <set>
#include "cards.h"


//��Ϸ����״̬
enum Status{
	NOTSTART,//��Ϸδ��ʼ
	GETLANDLORD,//�е����׶�
	SENDLANDLORDCARD,//�������ƽ׶�
	DISCARD,//���ƽ׶�
	GAMEOVER//��Ϸ����
};
//��Ϸ���ṹ
class Game{
	friend class Player;
	friend class Scene;
public:
	Game(HWND hwnd);
	~Game();

	Status GetStatus(void);//��ȡ��ǰ��Ϸ����״̬
	void GameStart(void);//��ʼ����Ϸ
	void InitGame(void);//��ʼ����ؽṹ
	void LoadPlayerScore();
	void StorePlayerScore();
	void RegisterScene(Scene *s){ this->scene = s; }
	inline void SendCard(void);//����
	void GetLandlord(void);//�е���
	void SendScore(int result);//������ҽе�������
	void SendLandlordCard(void);//��������
	Player *ProPlayer(void);//��ǰ��ҵ��ϼ�
	Player *NextPlayer(void);//��ǰ����¼�
	int NextPlayerNum(void);//��ǰ��ҵ��¼������ָ�������е��±�
	bool IsHumanTurn(void);//��ǰ���Ϊplayer[0]
	void Discard(void);//����
	void Hint(void);//��ʾ
	void Pass(void);//����
	void GameOver(void);//��Ϸ����

private:
	Status status;//��Ϸ����
	HWND hMainWnd;//������
	Scene *scene;//��Ϸ����
	Cards cardheap;//���ƶ�
	Player *player[3];//������ұ��Ϊ0
	Player *landlord;//����
	Player *curplayer;//��ǰ�������
	Player *lastone;//�����Ʒ�
	int callscore[3];//���ҽе����ķ���
	int callbegin;//��һ���е��������
	int basescore;//���ֻ�����
	int times;//���ֱ���
	int questioned;//��ѯ������
	int landlordcard[3];//���ֵ�����ר���ƴ洢��
};