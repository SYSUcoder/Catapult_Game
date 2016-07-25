#ifndef __SETTINGSCENE_H__
#define __SETTINGSCENE_H__


#include "cocos2d.h"

USING_NS_CC;


class SettingScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	CREATE_FUNC(SettingScene);

	void onBack(Ref *Sender);

private:
	Sprite* musicControl;
	Sprite* backBut;
	ProgressTimer* timer;

	bool isMusicOn;
};


#endif 