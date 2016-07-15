#include "MainScene.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
	auto scene = Scene::create();

	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;

}

bool MainScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	
	auto newGame = MenuItemLabel::create(Label::createWithTTF("New Game", "fonts/Marker Felt.ttf", 60), this, menu_selector(MainScene::onNewGame));

	auto menu = Menu::createWithItem(newGame);
	menu->alignItemsVertically();
	addChild(menu, 1);

	return true;
}

void MainScene::onNewGame(Ref *Sender)
{
	CCScene *scene = HelloPhysicsWorld::createScene();

	CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(1.2f, scene));


}