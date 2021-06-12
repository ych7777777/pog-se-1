/*
	���Ƽ���ʾ��Ϸ���棬��������Ϸ�������û����������磺
	�û�����Ϸ������֮���ɸò��ֽ����ж��Ƿ�����Ӧ�ƣ�
	���Ը��ƽ���ѡ��Ȼ���ٷ�ӳ����Ϸ���������Ӧ����
*/

#include <Windows.h>
#include <vector>
#include <map>
#include <set>
#include "winmain.h"
#include "cardgroup.h"
#include "player.h"
#include "game.h"
#include "scene.h"

Scene::Scene(Game *g)
: game(g)
{
	sceneSize.cx = 850;
	sceneSize.cy = 540;
	cardSize.cx = 71;
	cardSize.cy = 96;
	game->RegisterScene(this);
}

Scene::~Scene()
{
	DeleteDC(hdcScene);
	DeleteDC(hdcBkg);
	DeleteDC(hdcHumanCards);
	DeleteDC(hdcCardBack);
	DeleteDC(hdcCards);
	DeleteObject(hbmBkg);
	DeleteObject(hbmCardBack);
	DeleteObject(hbmCards);
	DeleteObject(hbmScene);
	DeleteObject(hbmHumanCards);
	DeleteObject(hbrush);
}
//������Ϸ�����ߴ�
SIZE Scene::GetSize()
{
	return sceneSize;
}
//��ʼ����Ϸ����
void Scene::InitScene(HWND hwnd)
{
	int x = sceneSize.cx / 2;
	int y = sceneSize.cy - 130 - 40;
	//������ť
	no = CreateWindow(TEXT("mybutton"), TEXT("no"), WS_CHILD,
		x - 160, y, 68, 35, hwnd, (HMENU)No, GetModuleHandle(NULL), NULL);
	score1 = CreateWindow(TEXT("mybutton"), TEXT("score1"), WS_CHILD,
		x - 80, y, 68, 35, hwnd, (HMENU)Score1, GetModuleHandle(NULL), NULL);
	score2 = CreateWindow(TEXT("mybutton"), TEXT("score2"), WS_CHILD,
		x, y, 68, 35, hwnd, (HMENU)Score2, GetModuleHandle(NULL), NULL);
	score3 = CreateWindow(TEXT("mybutton"), TEXT("score3"), WS_CHILD,
		x + 80, y, 68, 35, hwnd, (HMENU)Score3, GetModuleHandle(NULL), NULL);
	discand = CreateWindow(TEXT("mybutton"), TEXT("discard"), WS_CHILD,
		sceneSize.cx - 340, y, 68, 35, hwnd, (HMENU)Discard, GetModuleHandle(NULL), NULL);
	pass = CreateWindow(TEXT("mybutton"), TEXT("pass"), WS_CHILD,
		sceneSize.cx - 260, y, 68, 35, hwnd, (HMENU)Pass, GetModuleHandle(NULL), NULL);
	hint = CreateWindow(TEXT("mybutton"), TEXT("hint"), WS_CHILD,
		sceneSize.cx - 180, y, 68, 35, hwnd, (HMENU)Hint, GetModuleHandle(NULL), NULL);
	//�������λͼ�ڴ��豸����
	HDC hdc = GetDC(hwnd);
	HINSTANCE hInst = GetModuleHandle(NULL);

	HBITMAP hbitmap = LoadBitmap(hInst, TEXT("background"));
	hbrush = CreatePatternBrush(hbitmap);
	DeleteObject(hbitmap);

	hbmScene = CreateCompatibleBitmap(hdc, sceneSize.cx, sceneSize.cy);
	hbmBkg = CreateCompatibleBitmap(hdc, sceneSize.cx, sceneSize.cy);
	hbmHumanCards = CreateCompatibleBitmap(hdc, 650, 128);
	hbmCardBack = LoadBitmap(hInst, TEXT("cardback"));
	hbmCards = LoadBitmap(hInst, TEXT("cards"));
	hbmNoDiscard = LoadBitmap(hInst, TEXT("nodiscard"));

	hdcScene = CreateCompatibleDC(hdc);
	hdcBkg = CreateCompatibleDC(hdc);
	hdcHumanCards = CreateCompatibleDC(hdc);
	hdcCardBack = CreateCompatibleDC(hdc);
	hdcCards = CreateCompatibleDC(hdc);
	hdcNoDiscard = CreateCompatibleDC(hdc);
	SelectObject(hdcScene, hbmScene);
	SelectObject(hdcBkg, hbmBkg);
	SelectObject(hdcBkg, hbrush);
	SelectObject(hdcHumanCards, hbmHumanCards);
	SelectObject(hdcHumanCards, hbrush);
	SelectObject(hdcCardBack, hbmCardBack);
	SelectObject(hdcCards, hbmCards);
	SelectObject(hdcNoDiscard, hbmNoDiscard);

	ReleaseDC(hwnd, hdc);
}
//��ʾ��Ϸ����1
void Scene::ShowScene(HDC hdc)
{
	BitBlt(hdcScene, 0, 0, sceneSize.cx, sceneSize.cy, hdcBkg, 0, 0, SRCCOPY);
	DrawComputerCards();
	DrawChars();
	DrawHumanCards(hdcScene);//ѡ����Ƶ���Ϸ���滷������Ϊ�˷�ֹ���λ��Ʋ�������˸
	BitBlt(hdc, 0, 0, sceneSize.cx, sceneSize.cy, hdcScene, 0, 0, SRCCOPY);
}
//��ʾ��Ϸ����2
void Scene::ShowScene(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);
	ShowScene(hdc);
	ReleaseDC(hwnd, hdc);
}
//��ʾ�е�����ť
void Scene::ShowQuestionBtn()
{
	PostMessage(score1, WM_MYBUTTON, TRUE, 0);
	PostMessage(score2, WM_MYBUTTON, TRUE, 0);

	switch (game->basescore){
	case 2:
		PostMessage(score2, WM_MYBUTTON, FALSE, 0);
	case 1://����
		PostMessage(score1, WM_MYBUTTON, FALSE, 0);
	}
	ShowWindow(no, SW_SHOW);
	ShowWindow(score1, SW_SHOW);
	ShowWindow(score2, SW_SHOW);
	ShowWindow(score3, SW_SHOW);
}
//���ؽе�����ť
void Scene::HideQuestionBtn()
{
	ShowWindow(no, SW_HIDE);
	ShowWindow(score1, SW_HIDE);
	ShowWindow(score2, SW_HIDE);
	ShowWindow(score3, SW_HIDE);
}
//��ʾ���ư�ť
void Scene::ShowDiscardBtn()
{
	PostMessage(discand, WM_MYBUTTON, FALSE, 0);

	if (!game->lastone)
		PostMessage(pass, WM_MYBUTTON, FALSE, 0);
	else
		PostMessage(pass, WM_MYBUTTON, TRUE, 0);
	ShowWindow(discand, SW_SHOW);
	ShowWindow(pass, SW_SHOW);
	ShowWindow(hint, SW_SHOW);
	InvalidateRgn(hint, NULL, FALSE);
}
//���س��ư�ť
void Scene::HideDiscardBtn()
{
	ShowWindow(discand, SW_HIDE);
	ShowWindow(pass, SW_HIDE);
	ShowWindow(hint, SW_HIDE);
}

//���Ʊ���
void Scene::DrawBackground(void)
{
	PatBlt(hdcBkg, 0, 0, sceneSize.cx, sceneSize.cy, PATCOPY);
	HPEN hpen = CreatePen(PS_SOLID, 3, RGB(192, 192, 192));
	HPEN oldpen = (HPEN)SelectObject(hdcBkg, hpen);
	Rectangle(hdcBkg, 100, -5, sceneSize.cx - 100, sceneSize.cy - 130);
	Rectangle(hdcBkg, 310, 10, 310 + cardSize.cx, 10 + cardSize.cy);
	Rectangle(hdcBkg, 390, 10, 390 + cardSize.cx, 10 + cardSize.cy);
	Rectangle(hdcBkg, 470, 10, 470 + cardSize.cx, 10 + cardSize.cy);

	int i = 0;
	if (game->landlord){//������ʾ������
		for (auto mem : game->landlordcard){
			TransparentBlt(hdcBkg, 310 + 80 * i, 10, cardSize.cx, cardSize.cy,
				hdcCards, cardSize.cx * mem, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
			++i;
		}
	}
	else{//δ���������������Ʒ�����ʾ
		for (; i < 3; ++i){
			TransparentBlt(hdcBkg, 310 + 80 * i, 10, cardSize.cx, cardSize.cy,
				hdcCardBack, 0, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
		}
	}

	//BitBlt(hdcScene, 0, 0, sceneSize.cx, sceneSize.cy, hdcBkg, 0, 0, SRCCOPY);

	SelectObject(hdcBkg, oldpen);
	DeleteObject(hpen);
}
//���Ƶ��Է����ơ�����ơ���������������
void Scene::DrawComputerCards(void)
{
	int c, i = 0;

	if (game->status == GAMEOVER){//��Ϸ��������������������ʾ

		for (auto rb = game->player[1]->cards.rbegin(); rb != game->player[1]->cards.rend();++rb){
			TransparentBlt(hdcScene, sceneSize.cx - 50 - cardSize.cx / 2, 65 + 16 * i,
				cardSize.cx, cardSize.cy, hdcCards, cardSize.cx * *rb, 0,
				cardSize.cx, cardSize.cy, RGB(0, 0, 255));
			++i;
		}

		i = 0;
		for (auto rb = game->player[2]->cards.rbegin(); rb != game->player[2]->cards.rend(); ++rb){
			TransparentBlt(hdcScene, (100 - cardSize.cx) / 2, 65 + 16 * i,
				cardSize.cx, cardSize.cy, hdcCards, cardSize.cx * *rb, 0,
				cardSize.cx, cardSize.cy, RGB(0, 0, 255));
			++i;
		}


	}
	else{//�������Ʒ�����ʾ�������ʣ��������
		SIZE size;
		TCHAR szText[5];
		HFONT hfont = CreateFont(72, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("Calibri"));
		HFONT oldfont = (HFONT)SelectObject(hdcScene, hfont);
		COLORREF color = SetTextColor(hdcScene, RGB(255, 0, 128));
		SetBkMode(hdcScene, TRANSPARENT);

		i = game->player[1]->cards.size();
		if (i){//����ʾ�Ƶı���
			TransparentBlt(hdcScene, sceneSize.cx - 50 - cardSize.cx / 2, 65, cardSize.cx, cardSize.cy,
				hdcCardBack, 0, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));

			//Ȼ��д��ʣ������
			wsprintf(szText, TEXT("%d"), i);
			GetTextExtentPoint32(hdcScene, szText, wcslen(szText), &size);
			TextOut(hdcScene, sceneSize.cx - 50 - size.cx / 2,
				65 + (cardSize.cy - size.cy) / 2, szText, wcslen(szText));

		}

		i = game->player[2]->cards.size();
		if (i){//��ʾһ���Ƶı���
			TransparentBlt(hdcScene, 50 - cardSize.cx / 2, 65, cardSize.cx, cardSize.cy,
				hdcCardBack, 0, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
			//д��ʣ������
			wsprintf(szText, TEXT("%d"), i);
			GetTextExtentPoint32(hdcScene, szText, wcslen(szText), &size);
			TextOut(hdcScene, 50 - size.cx / 2, 65 + (cardSize.cy - size.cy) / 2,
				szText, wcslen(szText));

		}

		SetTextColor(hdcScene, color);
		SelectObject(hdcScene, oldfont);
		DeleteObject(hfont);
	}

	if (game->player[0]->nodiscard){//��ʾ��������
		TransparentBlt(hdcScene, (sceneSize.cx - 62) / 2, sceneSize.cy - 200,
			63, 27, hdcNoDiscard, 0, 0, 63, 27, RGB(255, 255, 255));
	}
	else{
		i = 0;
		c = game->player[0]->discard.count;
		for (auto rb = game->player[0]->discard.cards.rbegin();
			rb != game->player[0]->discard.cards.rend(); ++rb){//��ʾ�������
			TransparentBlt(hdcScene, (sceneSize.cx - cardSize.cx - 20 * c + 20) / 2 + 20 * i,
				sceneSize.cy - cardSize.cy - 135, cardSize.cx, cardSize.cy, hdcCards,
				cardSize.cx * *rb, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
			++i;
		}
	}
	if (game->player[1]->nodiscard){//��ʾ��������
		TransparentBlt(hdcScene, sceneSize.cx - 168, 200, 63, 27,
			hdcNoDiscard, 0, 0, 63, 27, RGB(255, 255, 255));
	}
	else{
		i = 0;
		c = game->player[1]->discard.count;
		for (auto rb = game->player[1]->discard.cards.rbegin();
			rb != game->player[1]->discard.cards.rend(); ++rb){//��ʾ�������
			TransparentBlt(hdcScene, sceneSize.cx - cardSize.cx - 20 * c - 85 + 20 * i,
				160, cardSize.cx, cardSize.cy, hdcCards, cardSize.cx * *rb, 0,
				cardSize.cx, cardSize.cy, RGB(0, 0, 255));
			++i;
		}
	}
	if (game->player[2]->nodiscard){//��ʾ��������
		TransparentBlt(hdcScene, 105, 200, 63, 27,
			hdcNoDiscard, 0, 0, 63, 27, RGB(255, 255, 255));
	}
	else{
		i = 0;
		for (auto rb = game->player[2]->discard.cards.rbegin();
			rb != game->player[2]->discard.cards.rend(); ++rb){//��ʾ�������
			TransparentBlt(hdcScene, 105 + 20 * i, 160, cardSize.cx, cardSize.cy, hdcCards,
				cardSize.cx * *rb, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
			++i;
		}
	}
}
//������ҵ���
void Scene::DrawHumanCards(HDC hdc, int highlight)
{
	PatBlt(hdcHumanCards, 0, 0, sceneSize.cx, sceneSize.cy, PATCOPY);

	int i = 0, y1,
		c = game->player[0]->cards.size(),
		x = (650 - cardSize.cx - 22 * c + 22) / 2,
		y = 128 - cardSize.cy - 15;

	for (auto rb = game->player[0]->cards.rbegin(); rb != game->player[0]->cards.rend(); ++rb){//��ʾ�������
		if (game->player[0]->selection.cards.find(*rb) !=
			game->player[0]->selection.cards.end())//����ѡ���������һ�Σ�ͻ����ʾ
			y1 = y - 15;
		else
			y1 = y;
		TransparentBlt(hdcHumanCards, x + 22 * i, y1, cardSize.cx, cardSize.cy, hdcCards,
			cardSize.cx * *rb, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
		//if (mem == highlight){
		//	HPEN pen = CreatePen(PS_SOLID, 3, RGB(0, 255, 255));
		//	HPEN oldpen = (HPEN)SelectObject(hdcScene, pen);
		//	SelectObject(hdcScene, GetStockObject(NULL_BRUSH));
		//	Rectangle(hdcScene, x + 22 * i, y1, x + 22 * i+cardSize.cx, y1+cardSize.cy);
		//	SelectObject(hdcScene, oldpen);
		//	DeleteObject(pen);
		//	}
		++i;
	}
	BitBlt(hdc, 100, 412, 650, 128, hdcHumanCards, 0, 0, SRCCOPY);
}
//��������Ƶ���һ��������ʽ
void Scene::DrawHumanCards(HWND hwnd, int highlight)
{
	HDC hdc = GetDC(hwnd);
	DrawHumanCards(hdc);
	ReleaseDC(hwnd, hdc);
}

//�����������
void Scene::DrawChars()
{
	HFONT hfont, oldfont;
	COLORREF color = SetTextColor(hdcScene, RGB(255, 255, 0));
	SetBkMode(hdcScene, TRANSPARENT);
	TCHAR szText[15];
	UINT textalign;

	switch (game->status){
	case GETLANDLORD://�е����׶Σ���ʾ�Ƿ�е���������
		hfont = CreateFont(28, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("�����п�"));
		oldfont = (HFONT)SelectObject(hdcScene, hfont);
		for (int i = game->questioned, j = game->callbegin; i > 0; --i, j = (j + 1) % 3){
			int x, y;

			switch (j){
			case 0://���
				textalign = SetTextAlign(hdcScene, TA_CENTER);
				x = sceneSize.cx / 2;
				y = sceneSize.cy - 200;
				break;
			case 1://����
				textalign = SetTextAlign(hdcScene, TA_RIGHT);
				x = sceneSize.cx - 105;
				y = 200;
				break;
			case 2://����
				textalign = SetTextAlign(hdcScene, TA_LEFT);
				x = 105;
				y = 200;
				break;
			}
			if (game->callscore[j] == 0)
				TextOut(hdcScene, x, y, szText, wsprintf(szText, TEXT("����")));
			else
				TextOut(hdcScene, x, y, szText, wsprintf(szText, TEXT("%d��"), game->callscore[j]));
			SetTextAlign(hdcScene, textalign);
		}
		SelectObject(hdcScene, oldfont);
		DeleteObject(hfont);//����
	case NOTSTART://��ʾ���Ժ��������
		hfont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("������κ"));
		oldfont = (HFONT)SelectObject(hdcScene, hfont);
		textalign = SetTextAlign(hdcScene, TA_CENTER);

		TextOut(hdcScene, 50, sceneSize.cy - cardSize.cy, szText,
			wsprintf(szText, TEXT("%s"), TEXT("���")));
		TextOut(hdcScene, sceneSize.cx - 50, 10, szText, wsprintf(szText, TEXT("%s"), TEXT("����")));
		TextOut(hdcScene, 50, 10, szText, wsprintf(szText, TEXT("%s"), TEXT("����")));

		SetTextAlign(hdcScene, textalign);
		SelectObject(hdcScene, oldfont);
		DeleteObject(hfont);
		break;
	case SENDLANDLORDCARD:
	case DISCARD:
	case GAMEOVER:
		//���������ơ����Ƽ���Ϸ����ʱ��ʾ���������ʡ�������ũ��
		hfont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("������κ"));
		oldfont = (HFONT)SelectObject(hdcScene, hfont);
		textalign = SetTextAlign(hdcScene, TA_CENTER);

		TextOut(hdcScene, 50, sceneSize.cy - cardSize.cy, szText,
			wsprintf(szText, TEXT("%s"), game->landlord == game->player[0] ?
			TEXT("����") : TEXT("ũ��")));
		TextOut(hdcScene, sceneSize.cx - 50, 10, szText, wsprintf(szText, TEXT("%s"),
			game->landlord == game->player[1] ? TEXT("����") : TEXT("ũ��")));
		TextOut(hdcScene, 50, 10, szText, wsprintf(szText, TEXT("%s"),
			game->landlord == game->player[2] ? TEXT("����") : TEXT("ũ��")));
		SelectObject(hdcScene, oldfont);
		DeleteObject(hfont);

		hfont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("����"));
		oldfont = (HFONT)SelectObject(hdcScene, hfont);

		TextOut(hdcScene, sceneSize.cx / 2, 15 + cardSize.cy, szText,
			wsprintf(szText, TEXT("%d�� %d��"), game->basescore, game->times));

		SetTextAlign(hdcScene, textalign);
		SelectObject(hdcScene, oldfont);
		DeleteObject(hfont);
		break;
	}

	hfont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("����"));
	oldfont = (HFONT)SelectObject(hdcScene, hfont);
	textalign = SetTextAlign(hdcScene, TA_CENTER);
	//����ҷ���
	TextOut(hdcScene, 50, sceneSize.cy - 66, szText,
		wsprintf(szText, TEXT("%d"), game->player[0]->score));
	TextOut(hdcScene, sceneSize.cx - 50, 36, szText,
		wsprintf(szText, TEXT("%d"), game->player[1]->score));
	TextOut(hdcScene, 50, 36, szText,
		wsprintf(szText, TEXT("%d"), game->player[2]->score));

	SetTextAlign(hdcScene, textalign);
	SelectObject(hdcScene, oldfont);
	DeleteObject(hfont);
}
//������Ϸ��������
void Scene::DrawResult(void)
{

}
//�ж�ĳһ�����Ƿ����ƣ������ƣ����ظ��Ƶ�ֵ��0-53��
int Scene::PointInWhich(POINT point)
{
	auto human = game->player[0];

	int c = human->cards.size(),
		x = (sceneSize.cx - cardSize.cx - 22 * c + 22) / 2,
		y = sceneSize.cy - cardSize.cy - 15;
	RECT rect;
	auto b = human->cards.begin();

	for (int i = c; i > 0 && b != human->cards.end(); --i, ++b){
		if (human->selection.cards.find(*b) != human->selection.cards.end())
			rect.top = y - 15;
		else
			rect.top = y;
		rect.left = x + 22 * (i - 1);
		rect.right = rect.left + cardSize.cx;
		rect.bottom = rect.top + cardSize.cy;
		if (PtInRect(&rect, point))
			return *b;
	}
	return -1;
}
//�����ӵĹ��ܣ��Ա����ָ������Ͼ���ʱ����Ӧ������������Ӧ
void Scene::MouseMove(POINT point)
{
	//BitBlt(hdcScene, 0, 0, sceneSize.cx, sceneSize.cy, hdcBkg, 0, 0, SRCCOPY);
	//DrawComputerCards();
	//DrawHumanCards(PointInWhich(point));
	//DrawChars();
	InvalidateRect(GetParent(no), NULL, FALSE);
}
//��ѡ��ĳ����ʱ
void Scene::SelectCard(POINT point)
{
	int num = PointInWhich(point);
	if (num >= 0){
		//if (game->player[0]->selection.cards.find(num) !=
		//	game->player[0]->selection.cards.end())
		//	game->player[0]->selection.DeleteNumber(num);
		//	else
		if (game->player[0]->selection.cards.find(num) ==
			game->player[0]->selection.cards.end())
			game->player[0]->selection.AddNumber(num);
		PostMessage(discand, WM_MYBUTTON, TRUE, 0);
		DrawHumanCards(GetParent(no));
	}

}
//��ȡ��ѡ��ĳ����ʱ
void Scene::DeleteCard(POINT point)
{
	int num = PointInWhich(point);
	if (num >= 0){
		game->player[0]->selection.DeleteNumber(num);
		if (game->player[0]->selection.count == 0)
			PostMessage(discand, WM_MYBUTTON, FALSE, 0);
		DrawHumanCards(GetParent(no));
	}
}