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

	MenuItemFont::setFontName("Marker Felt");
	MenuItemFont::setFontSize(40);
	MenuItemFont* newGame = MenuItemFont::create("Restart", this, menu_selector(FinishScene::onNewGame));

	auto menu = Menu::createWithItem(newGame);
	menu->alignItemsVertically();
	addChild(menu, 1);

	return true;
}

void FinishScene::onNewGame(Ref *Sender)
{
	CCScene *scene = HelloPhysicsWorld::createScene();

	CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(1.2f, scene));


}