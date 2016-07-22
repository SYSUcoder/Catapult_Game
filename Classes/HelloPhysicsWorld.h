#ifndef __HELLOPHYSICSWORLD_H__
#define __HELLOPHYSICSWORLD_H__


#include "cocos2d.h"
#include "FinishScene.h"
#include "Action.h"

USING_NS_CC;


class HelloPhysicsWorld : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	virtual bool onContactBegin(PhysicsContact& contact);
	//virtual bool onContactPreSolve(PhysicsContact& contact);
	//virtual void onContactPostSolve(PhysicsContact& contact);

	void updateTwoCollison(float dt);
	void updateOneCollison(float dt);
	void scheduleFunc(float dt);
	void updateEnemyAttack(float dt);
	void updateEnemySilence(float dt);
	void updateOurSilence(float dt);
	void updateTime(float dt);
	void updateItem(float dt);

	void rotateArrow(Point point);

	CREATE_FUNC(HelloPhysicsWorld);

private:
	My_Action* actionManager;

	Label* label;
	Label* timeLabel;

	Sprite* processBar;
	Sprite* processBarCover;
	Sprite* enemySilence;
	Sprite* ourbatterySilence;

	Point processBarPoint;
	Point preTouchPoint;
	Point currTouchPoint;
	Point speed;
	Point collisionPointA;
	Point collisionPointB;
	Point icePoint;

	ProgressTimer* m_timer;

	cocos2d::Size borderSize;
	cocos2d::Size visibleSize;
	int winTheGame;
	int loseTheGame;
	int remainTime;
	bool isEnemyAttack;
	bool isOurAttack;
	Vector<Sprite*> ourBirdVec;
	int ourBirdCount;
	Vector<Sprite*> enemyBirdVec;
	int enemyBirdCount;
};


#endif