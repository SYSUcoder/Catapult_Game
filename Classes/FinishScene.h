#ifndef __FINISHSCENE_H__
#define __FINISHSCENE_H__


#include "cocos2d.h"
#include "HelloPhysicsWorld.h"

USING_NS_CC;


class FinishScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(FinishScene);

	void onNewGame(Ref *Sender);

};


#endif 