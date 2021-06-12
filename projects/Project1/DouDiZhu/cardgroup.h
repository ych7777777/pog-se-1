#pragma once



//һ���ƵĿ�������
enum Type{
	Unkown,//δ֪
	Single,//����
	Double,//����
	Three,//����
	SingleSeq,//��˳
	DoubleSeq,//˫˳
	ThreeSeq,//��˳
	ThreePlus,//����һ��һ�Ż�һ�ԣ�
	Airplane,//�ɻ�
	FourSeq,//�Ĵ��������Ż����ԣ�
	Bomb,//ը������ը
};
//���ͽṹ
class CardGroup{
	friend class Player;
	friend class Game;
	friend class Scene;
public:
	CardGroup();
	CardGroup(Type t, int v);
	CardGroup& operator=(CardGroup &cg);
	void AddNumber(int num);//���0-53��ʾ����Ԫ��
	void DeleteNumber(int num);//ȥ��һ����
	void Clear(void);//���ô˽ṹ
	//��0-53ת����3-17Ȩֵ������A��14����2��15����С����16����������17��
	static int Translate(int num);
private:
	std::map<int, int> group;//3-17Ȩֵ����
	std::set<int> cards;//0-53��ɵļ��ϣ���Ҫ���ڷ��㻭����ʾ
	Type type;//�������ͣ����ơ����ӵȵȣ�
	int value;//Ȩֵ
	int count;//�˽ṹԪ����������������
};