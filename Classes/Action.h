#ifndef __ACTION_H__
#define __ACTION_H__


#include "cocos2d.h"

USING_NS_CC;

class My_Action
{
public:
	static My_Action* getInstance()
	{
		static My_Action instance;
		return &instance;
	}

	void rotateArrow(Point point);

	Node* createOurBattery(Point p);
	Node* createOurHead(Point p);
	Node* createEnemyBattery(Point p);
	Node* createEnemyHead(Point p);
	Node* createProcessBar(Point p);
	Node* createProcessBarCover(Point p);
	Node* createOurBird(Point p);
	Node* createEnemyBird(Point p);
	Node* createPigPoint(Point p);
	Node* createOurPig(Point p);
	Node* createBlock(Point p);
	Node* createUnderBlock(Point p);
	Node* createTombstone(Point p);
	Node* createEnemySilence(Point p);
	Node* createOurSilence(Point p);
	Node* createIceItem(Point p);
	Node* createTimeItem(Point p);
	Node* createDefendItem(Point p);

	void createRepeatActionBlock(Point p1, Point p2, double t);

	void addIntoScene(cocos2d::Layer* layer, cocos2d::Node* node, int z = 0)
	{
		layer->addChild(node, z);
	}

private:
	My_Action();

	Sprite* left_head;
	Sprite* right_head;

	Sprite* blockSprite;
	Sprite* itemSprite;
	Sprite* processBar;

	ProgressTimer* timer;

	double rotateRadians;
	double rotateDegrees;

};


#endif