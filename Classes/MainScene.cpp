#include "MainScene.h"
#include "SettingScene.h"

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

	auto mapSprite = Sprite::create("mainSceneBg.jpg");
	mapSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	mapSprite->setScale(1.1);
	addChild(mapSprite);

	
	auto newGame = MenuItemLabel::create(Label::createWithTTF("New Game", "fonts/Marker Felt.ttf", 70), this, menu_selector(MainScene::onNewGame));
	newGame->setColor(Color3B::YELLOW);
	auto settings = MenuItemLabel::create(Label::createWithTTF("Settings", "fonts/Marker Felt.ttf", 50), this, menu_selector(MainScene::onSettings));
	//ettings->setColor(Color3B::BLACK);
	auto onHelp = MenuItemLabel::create(Label::createWithTTF("Help", "fonts/Marker Felt.ttf", 50), this, menu_selector(MainScene::onHelp));
	//onHelp->setColor(Color3B::BLACK);
	auto onQuit = MenuItemLabel::create(Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 50), this, menu_selector(MainScene::onQuit));
	//onQuit->setColor(Color3B::BLACK);

	auto menu = Menu::create(newGame, settings, onHelp, onQuit, NULL);
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 3);
	menu->alignItemsVerticallyWithPadding(20);
	addChild(menu, 1);

	return true;
}

void MainScene::onNewGame(Ref *Sender)
{
	CCScene *scene = HelloPhysicsWorld::createScene();

	CCDirector::sharedDirector()->replaceScene(TransitionPageTurn::create(1.0f, scene, true));


}

void MainScene::onSettings(Ref *Sender)
{
	CCScene *scene = SettingScene::createScene();

	CCDirector::sharedDirector()->replaceScene(TransitionPageTurn::create(1.0f, scene, true));


}

void MainScene::onHelp(Ref *Sender)
{
	CCScene *scene = HelloPhysicsWorld::createScene();

	CCDirector::sharedDirector()->replaceScene(TransitionPageTurn::create(1.0f, scene, true));


}

// quit the game
void MainScene::onQuit(Ref *Sender)
{
	Director::getInstance()->end();


}