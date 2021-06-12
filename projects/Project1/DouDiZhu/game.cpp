/*
��Ϸ�࣬����������Ϸ�Ľ��С�������Ϸ���г��������������Ԫ��
*/

#include <Windows.h>
#include <fstream>
#include <random>
#include <time.h>
#include <stdexcept>
#include <vector>
#include <map>
#include <set>
#include "winmain.h"
#include "cardgroup.h"
#include "player.h"
#include "cards.h"
#include "scene.h"
#include "game.h"

using namespace std;

Game::Game(HWND hwnd)
: landlord(nullptr)
, curplayer(nullptr)
, lastone(nullptr)
, basescore(0)
, times(0)
, questioned(0)
, hMainWnd(hwnd)
, status(NOTSTART)
, callbegin(0)
{
	for (int i = 0; i < 3; ++i){
		callscore[i] = 0;
		player[i] = new Player(*this);
	}
}

Game::~Game()
{
	for (int i = 0; i < 3; ++i)
		delete player[i];
}
//��ʼ����ؽṹ
void Game::InitGame()
{
	landlord = curplayer = lastone = nullptr;
	basescore = questioned = 0;
	times = 1;
	for (int i = 0; i < 3; ++i){
		player[i]->NewGame();
	}
	cardheap.RandCards();
	status = GETLANDLORD;
}
//��ʼ����Ϸ
void Game::GameStart()
{
	InitGame();
	SendCard();
	scene->HideDiscardBtn();
	scene->HideQuestionBtn();
	scene->DrawBackground();
	scene->ShowScene(hMainWnd);;
	status = GETLANDLORD;
	SetTimer(hMainWnd, 1, 500, NULL);

}

void Game::LoadPlayerScore()
{
	ifstream in(szDataFile);

	if (!in)
		return;
	for (int i = 0; i < 3; ++i)
		in >> player[i]->score;
}

void Game::StorePlayerScore()
{
	ofstream out(szDataFile);

	if (!out)
		return;
	for (int i = 0; i < 3; ++i)
		out << player[i]->score << "\n";
}

//��ȡ��Ϸ�ĵ�ǰ����
Status Game::GetStatus()
{
	return status;
}
//��ȡ��ǰ��ҵ��ϼ�
Player *Game::ProPlayer()
{
	int i;
	for (i = 0; i < 3; ++i){
		if (player[i] == curplayer)
			break;
	}
	return player[(i + 2) % 3];
}
//��ȡ��ǰ��ҵ��¼�
Player *Game::NextPlayer()
{
	return player[NextPlayerNum()];
}
//��ǰ��ҵ��¼������ָ�������е��±�
int Game::NextPlayerNum(void)
{
	int i;
	for (i = 0; i < 3; ++i){
		if (player[i] == curplayer)
			break;
	}
	return (i + 1) % 3;
}

bool Game::IsHumanTurn(void)
{
	return curplayer == player[0];
}

void Game::SendCard(void)
{
	while (cardheap.GetRemain() > 3){
		player[0]->AddCard(cardheap.GetCard());
		player[1]->AddCard(cardheap.GetCard());
		player[2]->AddCard(cardheap.GetCard());
	}
	for (int i = 0; i < 3; ++i)
		landlordcard[i] = cardheap.GetCard();
}
//�е���
void Game::GetLandlord()
{
	int i = -1;
	if (!questioned){//���ȷ����ʼѯ�ʵ����
		default_random_engine e((UINT)time(nullptr));
		uniform_int_distribution<unsigned> u(0, 2);

		callbegin = i = u(e);
		status = GETLANDLORD;
	}
	else if (questioned == 3){//������Ҷ���ѯ�ʹ�
		if (lastone){//����������ߵ�Ϊ����
			curplayer = landlord = lastone;
			lastone = nullptr;
		}
		else{//����Ϊ���ƣ����¿�ʼ��Ϸ
			status = NOTSTART;
		}
		scene->ShowScene(hMainWnd);
		if (landlord)//�����Ѿ�ȷ����������������ƽ׶�
			status = SENDLANDLORDCARD;
		SetTimer(hMainWnd, 1, 500, NULL);
		return;
	}
	if (i == -1)//���ǵ�һ��ѯ�ʣ�ȷ��Ҫѯ�ʵ��¼�
		i = NextPlayerNum();

	if (i == 0){//ѯ�ʵ�������ң���Ҫ�ȴ��������ѡ��
		scene->ShowQuestionBtn();
		curplayer = player[0];
		return;
	}
	else{//����ֱ�ӵ�����ҵ�AI����
		curplayer = player[i];
		int result = curplayer->GetBaseScore(questioned, basescore);
		callscore[i] = result;
		if (result == 3){//�������־�ֱ�ӵ�����
			basescore = result;
			landlord = curplayer;
			lastone = nullptr;
		}
		else if (result > basescore){//���򣬸������������ϴθ������
			basescore = result;
			lastone = curplayer;//�ͰѸ���Ҽ�¼����
		}
		++questioned;
		scene->ShowScene(hMainWnd);;
	}
	if (landlord)//������ȷ�������뷢�����ƽ׶�
		status = SENDLANDLORDCARD;
	SetTimer(hMainWnd, 1, 500, NULL);
}
//����������ҽе����ķ���
void Game::SendScore(int result)
{
	callscore[0] = result;
	if (result == 3){
		basescore = result;
		landlord = player[0];
		lastone = nullptr;
	}
	else if (result > basescore){
		basescore = result;
		lastone = player[0];
	}
	++questioned;
	scene->ShowScene(hMainWnd);
	if (landlord)
		status = SENDLANDLORDCARD;
	SetTimer(hMainWnd, 1, 500, NULL);
}
//��������
void Game::SendLandlordCard()
{
	for (auto mem : landlordcard)
		landlord->AddCard(mem);
	scene->DrawBackground();//�����˵�������������������ʾ
	scene->ShowScene(hMainWnd);;
	status = DISCARD;//��ǰ״̬Ϊ���ƽ׶�
	SetTimer(hMainWnd, 1, 500, NULL);
}
//����
void Game::Discard()
{
	if (lastone == curplayer){//����ҳ���û��ѹ������һ�ֳ���
		lastone = nullptr;
		for (int i = 0; i < 3; ++i){//��ճ�����
			player[i]->discard.Clear();
			player[i]->nodiscard = false;
		}

	}
	else{//��յ�ǰ������ҳ�����
		curplayer->discard.Clear();
		curplayer->nodiscard = false;
	}
	scene->ShowScene(hMainWnd);

	if (curplayer == player[0]){//��ǰ���Ϊ��
		if (curplayer->selection.count &&
			curplayer->HumanDiscard()){//�����ѡ�Ʋ��ҷ��Ϲ涨
			scene->HideDiscardBtn();
			lastone = curplayer;
			if (curplayer->discard.type == Bomb)//�������Ϊը�������ӱ���
				++times;
		}
		else{//��������ȴ����ѡ��
			scene->ShowScene(hMainWnd);
			scene->ShowDiscardBtn();
			return;
		}

	}
	else{//��ǰ���Ʒ�Ϊ����
		curplayer->SelectCards();
		if (curplayer->Discard())
			lastone = curplayer;
		if (curplayer->discard.type == Bomb)//ը��
			++times;
	}
	scene->ShowScene(hMainWnd);

	if (lastone->cards.empty())//�����Ʒ���������
		status = GAMEOVER;//��Ϸ����
	else
		curplayer = NextPlayer();//�¼Ҽ�������

	SetTimer(hMainWnd, 1, 500, NULL);
}
//������ʾ
void Game::Hint()
{
	player[0]->selection.Clear();
	player[0]->SelectCards(true);
	if (player[0]->selection.count != 0)
		PostMessage(scene->discand, WM_MYBUTTON, TRUE, 0);
	InvalidateRect(hMainWnd, NULL, FALSE);
}
//����
void Game::Pass()
{
	player[0]->Pass();
	curplayer = NextPlayer();//�¼ҳ���

	scene->ShowScene(hMainWnd);
	SetTimer(hMainWnd, 1, 500, NULL);
}

//��Ϸ����
void Game::GameOver()
{
	int score = basescore * times;
	bool IsPeopleWin = false;

	curplayer = landlord;//�ѵ�����Ϊ��ǰ��ң������ȡ�ϼҺ��¼�
	if (landlord->cards.size()){//ũ��ʤ��
		landlord->score -= score * 2;
		ProPlayer()->score += score;
		NextPlayer()->score += score;
		if (player[0] != landlord)
			IsPeopleWin = true;
	}
	else{//����ʤ��
		landlord->score += score * 2;
		ProPlayer()->score -= score;
		NextPlayer()->score -= score;
		if (player[0] == landlord)
			IsPeopleWin = true;
	}

	scene->ShowScene(hMainWnd);
	if (IsPeopleWin)
		MessageBox(hMainWnd, TEXT("��ϲ������ʤ�ˣ�"), TEXT("��Ϸ����"), 0);
	else
		MessageBox(hMainWnd, TEXT("���ź��������ˡ���"), TEXT("��Ϸ����"), 0);
	GameStart();
}