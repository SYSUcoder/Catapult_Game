#include "FinishScene.h"

USING_NS_CC;

Scene* FinishScene::createScene()
{
	auto scene = Scene::create();

	auto layer = FinishScene::create();
	scene->addChild(layer);
	return scene;

}

bool FinishScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::create();
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	label->setSystemFontSize(70);
	if (UserDefault::getInstance()->getBoolForKey("isWin"))
	{
		label->setString("You defeat AlphaGo!");
	}
	else
	{
		label->setString("You lose the game...");
	}
	addChild(label, 1);

	return true;
}
