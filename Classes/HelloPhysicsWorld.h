#ifndef __HelloPhysicsWorld_SCENE_H__
#define __HelloPhysicsWorld_SCENE_H__


#include "cocos2d.h"
#include "FinishScene.h"

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
	void updateIceItem(float dt);

	void rotateArrow(Point point);

	CREATE_FUNC(HelloPhysicsWorld);


	void createOurBird();
	void createEnemyBird(Point p);
	void createPigPoint(Point p);
	void createOurPig(Point p);
	void createBlock(Point p);
	void createUnderBlock(Point p);
	void createTombstone(Point p);
	void createEnemySilence(Point p);
	void createOurSilence(Point p);
	void createRepeatActionBlock(Point p1, Point p2, double t);

private:
	Label* label;
	Label* timeLabel;
	Sprite* left_head;
	Sprite* right_head;
	Sprite* processBar;
	Sprite* processBarCover;
	Sprite* enemySilence;
	Sprite* blockSprite;
	Sprite* ourbatterySilence;
	Point processBarPoint;
	Point preTouchPoint;
	Point currTouchPoint;
	Point speed;
	Point collisionPointA;
	Point collisionPointB;
	Point icePoint;
	ProgressTimer* timer;
	double rotateRadians;
	double rotateDegrees;
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
	Vector<Sprite*> iceItemVec;
};


#endif // __HelloPhysicsWorld_SCENE_H__