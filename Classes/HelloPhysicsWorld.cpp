#include "HelloPhysicsWorld.h"
#include "SimpleAudioEngine.h"
#include <string>
#include <sstream>
#include <cmath>
#include <iostream>

using std::string;
using std::stringstream;

USING_NS_CC;

Scene* HelloPhysicsWorld::createScene()
{
	auto scene = Scene::createWithPhysics(); 
	// scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);   

	auto layer = HelloPhysicsWorld::create();
	scene->addChild(layer);
	return scene;

}


// Tag:   5 is our bird, 6 is enemy's pigPoint, 7 is our battery
// Tag:   8 is enemy's bird, 9 is our pigPoint, 10 is enemy's battery
// Tag:   11 is tombstone, 12 is ice item

bool HelloPhysicsWorld::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	borderSize = cocos2d::Size(1280, 500);
	preTouchPoint = Vec2::ZERO;
	currTouchPoint = Vec2::ZERO;

	winTheGame = 5;
	loseTheGame = 5;
	isEnemyAttack = true;
	isOurAttack = true;
	remainTime = 60;

	actionManager = My_Action::getInstance();
	

	//定义世界的边界
	auto body = PhysicsBody::createEdgeBox(Size(borderSize.width, borderSize.height + 25));
	// 设置掩码
	body->setCategoryBitmask(0x01F);
	body->setContactTestBitmask(0x01);
	body->setCollisionBitmask(0x01F);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 95));
	edgeNode->setPhysicsBody(body);
	this->addChild(edgeNode);

	// map
	auto mapSprite = Sprite::create("map1.jpg");
	mapSprite->setScale(1.15, 1.15);
	mapSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	addChild(mapSprite, 0);

	// test label
	label = Label::createWithTTF("", "fonts/Marker Felt.ttf", 40);
	label->setPosition(Vec2(50, visibleSize.height - 50));
	label->setAnchorPoint(Vec2::ZERO);
	addChild(label, 1);

	// time label
	timeLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 70);
	timeLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 85));
	timeLabel->setColor(Color3B::GREEN);
	stringstream ss;
	ss << remainTime;
	timeLabel->setString(ss.str());
	ss.str("");
	addChild(timeLabel, 1);

	// decrease time
	schedule(schedule_selector(HelloPhysicsWorld::updateTime), 1.0f);

	// our battery
	
	actionManager->addIntoScene(this, actionManager->createOurBattery(Point(140, 192)), 1);
	actionManager->addIntoScene(this, actionManager->createOurHead(Point(135, 237)), 1);

	// enemy's battery

	actionManager->addIntoScene(this, actionManager->createEnemyBattery(Point(visibleSize.width - 140, 192)), 1);
	actionManager->addIntoScene(this, actionManager->createEnemyHead(Point(visibleSize.width - 135, 237)), 1);

	// our point pig
	actionManager->addIntoScene(this, actionManager->createOurPig(Point(65, 280)), 1);
	actionManager->addIntoScene(this, actionManager->createOurPig(Point(480, 235)), 1);
	actionManager->addIntoScene(this, actionManager->createOurPig(Point(110, 430)), 1);
	actionManager->addIntoScene(this, actionManager->createOurPig(Point(65, 660)), 1);
	actionManager->addIntoScene(this, actionManager->createOurPig(Point(480, 660)), 1);
	
	double blockLower = 10;
	// create under block
	actionManager->addIntoScene(this, actionManager->createUnderBlock(Point(65, 280 - blockLower)), 1);
	actionManager->addIntoScene(this, actionManager->createUnderBlock(Point(480, 235 - blockLower)), 1);
	actionManager->addIntoScene(this, actionManager->createUnderBlock(Point(110, 430 - blockLower)), 1);
	actionManager->addIntoScene(this, actionManager->createUnderBlock(Point(65, 660 - blockLower)), 1);
	actionManager->addIntoScene(this, actionManager->createUnderBlock(Point(480, 660 - blockLower)), 1);

	// enemy point pig
	actionManager->addIntoScene(this, actionManager->createPigPoint(Point(visibleSize.width - 65, 280)), 1);
	actionManager->addIntoScene(this, actionManager->createPigPoint(Point(visibleSize.width - 480, 235)), 1);
	actionManager->addIntoScene(this, actionManager->createPigPoint(Point(visibleSize.width - 110, 430)), 1);
	actionManager->addIntoScene(this, actionManager->createPigPoint(Point(visibleSize.width - 65, 660)), 1);
	actionManager->addIntoScene(this, actionManager->createPigPoint(Point(visibleSize.width - 480, 660)), 1);

	// create under block
	actionManager->addIntoScene(this, actionManager->createUnderBlock(Point(visibleSize.width - 65, 280 - blockLower)), 1);
	actionManager->addIntoScene(this, actionManager->createUnderBlock(Point(visibleSize.width - 480, 235 - blockLower)), 1);
	actionManager->addIntoScene(this, actionManager->createUnderBlock(Point(visibleSize.width - 110, 430 - blockLower)), 1);
	actionManager->addIntoScene(this, actionManager->createUnderBlock(Point(visibleSize.width - 65, 660 - blockLower)), 1);
	actionManager->addIntoScene(this, actionManager->createUnderBlock(Point(visibleSize.width - 480, 660 - blockLower)), 1);

	// middle block and its action

	actionManager->addIntoScene(this, actionManager->createBlock(Point(visibleSize.width / 2, 225)));
	actionManager->createRepeatActionBlock(Point(visibleSize.width / 2, 325), Point(visibleSize.width / 2, 225), 3);
	/*
	actionManager->addIntoScene(this, actionManager->createBlock(Point(visibleSize.width / 2, 385)));
	actionManager->createRepeatActionBlock(Point(visibleSize.width / 2, 485), Point(visibleSize.width / 2, 385), 5);
	*/
	actionManager->addIntoScene(this, actionManager->createBlock(Point(visibleSize.width / 2, 445)));
	actionManager->createRepeatActionBlock(Point(visibleSize.width / 2, 505), Point(visibleSize.width / 2, 400), 5);

	// processBar
	processBar = (Sprite*)actionManager->createProcessBar(Point(180, 260));
	actionManager->addIntoScene(this, processBar, 2);
	processBarPoint.setPoint(135, 237);

	// to cover processBar
	m_timer = (ProgressTimer*)actionManager->createProcessBarCover(Point(180, 260));
	actionManager->addIntoScene(this, m_timer, 2);

	// event listener
	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(HelloPhysicsWorld::onContactBegin, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	// AI攻击
	schedule(schedule_selector(HelloPhysicsWorld::updateEnemyAttack), 2.0f);

	// item appears
	schedule(schedule_selector(HelloPhysicsWorld::updateItem), 10.0f);
	                                           
	setTouchEnabled(true);                                              
	//设置为单点触摸
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	return true;
}



bool HelloPhysicsWorld::onTouchBegan(Touch* touch, Event* event)
{
	currTouchPoint = touch->getLocation();
	if (!currTouchPoint.equals(preTouchPoint)){
		actionManager->rotateArrow(currTouchPoint);
		preTouchPoint = currTouchPoint;
	}
	// processBar begin increasing
	if (isOurAttack)
	{
		speed = Vec2::ZERO;
		schedule(schedule_selector(HelloPhysicsWorld::scheduleFunc), 0.02f);
		processBar->setVisible(true);
	}
	
	// show the position of mouse
	stringstream ss;
	ss << currTouchPoint.x;
	string str = "X: ";
	str += ss.str();
	ss.str("");
	str += ", Y: ";
	ss << currTouchPoint.y;
	str += ss.str();
	label->setString(str);
	return true;
}

void HelloPhysicsWorld::onTouchMoved(Touch* touch, Event* event)
{
	currTouchPoint = touch->getLocation();
	if (!currTouchPoint.equals(preTouchPoint)){
		actionManager->rotateArrow(currTouchPoint);
		preTouchPoint = currTouchPoint;
	}

	// show the position of mouse
	stringstream ss;
	ss << currTouchPoint.x;
	string str = "X: ";
	str += ss.str();
	ss.str("");
	str += ", Y: ";
	ss << currTouchPoint.y;
	str += ss.str();
	label->setString(str);
}

void HelloPhysicsWorld::onTouchEnded(Touch* touch, Event* event)
{
	currTouchPoint = touch->getLocation();
	if (isOurAttack)
	{
		Sprite* sp = (Sprite*)actionManager->createOurBird(processBarPoint);
		actionManager->addIntoScene(this, sp, 1);
		ourBirdVec.pushBack(sp);
		unschedule(schedule_selector(HelloPhysicsWorld::scheduleFunc));
		m_timer->setPercentage(0);
		processBar->setVisible(false);
	}
}


bool HelloPhysicsWorld::onContactBegin(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA && nodeB)
	{
		// collision of our bird and enemy's bird
		if ((nodeA->getTag() == 5 && nodeB->getTag() == 8) || (nodeA->getTag() == 8 && nodeB->getTag() == 5))
		{
			collisionPointA = contact.getShapeA()->getBody()->getPosition();
			collisionPointB = contact.getShapeB()->getBody()->getPosition();
			scheduleOnce(schedule_selector(HelloPhysicsWorld::updateTwoCollison), 0.01f);
			nodeA->removeFromParentAndCleanup(true);
			nodeB->removeFromParentAndCleanup(true);
		}
		// collision of our bird and enemy's pig
		else if ((nodeA->getTag() == 5 && nodeB->getTag() == 6) || (nodeA->getTag() == 6 && nodeB->getTag() == 5))
		{
			collisionPointA = contact.getShapeA()->getBody()->getPosition();
			collisionPointB = contact.getShapeB()->getBody()->getPosition();
			scheduleOnce(schedule_selector(HelloPhysicsWorld::updateTwoCollison), 0.01f);
			nodeA->removeFromParentAndCleanup(true);
			nodeB->removeFromParentAndCleanup(true);
			if (--winTheGame <= 0) // you win the game
			{
				UserDefault::getInstance()->setBoolForKey("isWin", true);
				auto scene = FinishScene::createScene();
				Director::sharedDirector()->replaceScene(scene);
			}
		}
		// collision of enemy's bird and our pig
		else if ((nodeA->getTag() == 8 && nodeB->getTag() == 9) || (nodeA->getTag() == 9 && nodeB->getTag() == 8))
		{
			collisionPointA = contact.getShapeA()->getBody()->getPosition();
			collisionPointB = contact.getShapeB()->getBody()->getPosition();
			scheduleOnce(schedule_selector(HelloPhysicsWorld::updateTwoCollison), 0.01f);
			nodeA->removeFromParentAndCleanup(true);
			nodeB->removeFromParentAndCleanup(true);
			if (--loseTheGame <= 0) // you lose the game
			{
				UserDefault::getInstance()->setBoolForKey("isWin", false);
				auto scene = FinishScene::createScene();
				Director::sharedDirector()->replaceScene(scene);
			}
		}
		// collision of our bird and enemy's battery
		else if ((nodeA->getTag() == 5 && nodeB->getTag() == 10) || (nodeA->getTag() == 10 && nodeB->getTag() == 5))
		{
			if (nodeA->getTag() == 5)
			{
				collisionPointA = contact.getShapeA()->getBody()->getPosition();
				nodeA->removeFromParentAndCleanup(true);
			}
			else
			{
				collisionPointA = contact.getShapeB()->getBody()->getPosition();
				nodeB->removeFromParentAndCleanup(true);
			}
			scheduleOnce(schedule_selector(HelloPhysicsWorld::updateOneCollison), 0.01f);
			
			if (isEnemyAttack)
			{
				enemySilence = (Sprite*)actionManager->createEnemySilence(Point(visibleSize.width - 165, 310));
				actionManager->addIntoScene(this, enemySilence);
				scheduleOnce(schedule_selector(HelloPhysicsWorld::updateEnemySilence), 5.0f);
			}
				
			isEnemyAttack = false;
		}
		// collision of enemy's bird and our battery
		else if ((nodeA->getTag() == 7 && nodeB->getTag() == 8) || (nodeA->getTag() == 8 && nodeB->getTag() == 7))
		{
			if (nodeA->getTag() == 8)
			{
				collisionPointA = contact.getShapeA()->getBody()->getPosition();
				nodeA->removeFromParentAndCleanup(true);
			}
			else
			{
				collisionPointA = contact.getShapeB()->getBody()->getPosition();
				nodeB->removeFromParentAndCleanup(true);
			}
			scheduleOnce(schedule_selector(HelloPhysicsWorld::updateOneCollison), 0.01f);
			
			if (isOurAttack)
			{
				ourbatterySilence = (Sprite*)actionManager->createOurSilence(Point(165, 310));
				actionManager->addIntoScene(this, ourbatterySilence);
				scheduleOnce(schedule_selector(HelloPhysicsWorld::updateOurSilence), 5.0f);
			}
				
			isOurAttack = false;
		}
		// collision of our bird and iceItem
		else if ((nodeA->getTag() == 5 && nodeB->getTag() == 12) || (nodeA->getTag() == 12 && nodeB->getTag() == 5))
		{
			if (nodeA->getTag() == 5)
			{
				collisionPointA = contact.getShapeA()->getBody()->getPosition();
				scheduleOnce(schedule_selector(HelloPhysicsWorld::updateOneCollison), 0.01f);
				nodeA->removeFromParentAndCleanup(true);
				nodeB->removeFromParentAndCleanup(true);
			}
			else
			{
				collisionPointA = contact.getShapeB()->getBody()->getPosition();
				scheduleOnce(schedule_selector(HelloPhysicsWorld::updateOneCollison), 0.01f);
				nodeA->removeFromParentAndCleanup(true);
				nodeB->removeFromParentAndCleanup(true);
			}
			auto it = enemyBirdVec.begin();
			while (it != enemyBirdVec.end())
			{
				(*it)->getPhysicsBody()->setVelocity(Vec2(0, 0));
				it++;
			}
			if (isEnemyAttack)
			{
				enemySilence = (Sprite*)actionManager->createEnemySilence(Point(visibleSize.width - 165, 310));
				actionManager->addIntoScene(this, enemySilence);
				scheduleOnce(schedule_selector(HelloPhysicsWorld::updateEnemySilence), 2.0f);
			}
			isEnemyAttack = false;
		}
		// collision of our bird and timeItem
		else if ((nodeA->getTag() == 5 && nodeB->getTag() == 13) || (nodeA->getTag() == 13 && nodeB->getTag() == 5))
		{
			if (nodeA->getTag() == 5)
			{
				collisionPointA = contact.getShapeA()->getBody()->getPosition();
				scheduleOnce(schedule_selector(HelloPhysicsWorld::updateOneCollison), 0.01f);
				nodeA->removeFromParentAndCleanup(true);
				nodeB->removeFromParentAndCleanup(true);
			}
			else
			{
				collisionPointA = contact.getShapeB()->getBody()->getPosition();
				scheduleOnce(schedule_selector(HelloPhysicsWorld::updateOneCollison), 0.01f);
				nodeA->removeFromParentAndCleanup(true);
				nodeB->removeFromParentAndCleanup(true);
			}
			remainTime += 20;
			timeLabel->setColor(Color3B::GREEN);
			stringstream ss;
			ss << remainTime;
			timeLabel->setString(ss.str());
			ss.str("");
		}
		else if ((nodeA->getTag() == 5 && nodeB->getTag() == 14) || (nodeA->getTag() == 14 && nodeB->getTag() == 5))
		{
			if (nodeA->getTag() == 5)
			{
				collisionPointA = contact.getShapeA()->getBody()->getPosition();
				scheduleOnce(schedule_selector(HelloPhysicsWorld::updateOneCollison), 0.01f);
				nodeA->removeFromParentAndCleanup(true);
				nodeB->removeFromParentAndCleanup(true);
			}
			else
			{
				collisionPointA = contact.getShapeB()->getBody()->getPosition();
				scheduleOnce(schedule_selector(HelloPhysicsWorld::updateOneCollison), 0.01f);
				nodeA->removeFromParentAndCleanup(true);
				nodeB->removeFromParentAndCleanup(true);
			}
			auto it = enemyBirdVec.begin();
			while (it != enemyBirdVec.end())
			{
				(*it)->removeFromParentAndCleanup(true);
				it++;
			}
		}
		else  // delete low speed bird
		{
			if (fabs(contact.getShapeA()->getBody()->getVelocity().x) < 20 &&
				fabs(contact.getShapeA()->getBody()->getVelocity().y) < 20 &&
				(nodeA->getTag() == 5 || nodeA->getTag() == 8))
			{
				collisionPointA = contact.getShapeA()->getBody()->getPosition();
				scheduleOnce(schedule_selector(HelloPhysicsWorld::updateOneCollison), 0.1f);
				nodeA->removeFromParentAndCleanup(true);
			}
				
			if (fabs(contact.getShapeB()->getBody()->getVelocity().x) < 20 &&
				fabs(contact.getShapeB()->getBody()->getVelocity().y) < 20 &&
				(nodeB->getTag() == 5 || nodeB->getTag() == 8))
			{
				collisionPointA = contact.getShapeB()->getBody()->getPosition();
				scheduleOnce(schedule_selector(HelloPhysicsWorld::updateOneCollison), 0.1f);
				nodeB->removeFromParentAndCleanup(true);
			}
		}
		
	}
    
	return true;
}



void HelloPhysicsWorld::scheduleFunc(float dt)
{
	if (isOurAttack)
	{
		auto increase = m_timer->getPercentage() + 1.4;
		if (increase > 100) increase = 100;
		m_timer->setPercentage(increase);

		if (m_timer->getPercentage() >= 100)
		{
			unschedule(schedule_selector(HelloPhysicsWorld::scheduleFunc));
		}
	}
}

void HelloPhysicsWorld::updateTwoCollison(float dt)
{
	actionManager->addIntoScene(this, actionManager->createTombstone(collisionPointA));
	actionManager->addIntoScene(this, actionManager->createTombstone(collisionPointB));
}

void HelloPhysicsWorld::updateOneCollison(float dt)
{
	actionManager->addIntoScene(this, actionManager->createTombstone(collisionPointA));
}

void HelloPhysicsWorld::updateEnemyAttack(float dt)
{
	if (isEnemyAttack)
	{
		Sprite* sp = (Sprite*)actionManager->createEnemyBird(Point(visibleSize.width - processBarPoint.x, processBarPoint.y));
		actionManager->addIntoScene(this, sp, 1);
		enemyBirdVec.pushBack(sp);
	}
		
}

void HelloPhysicsWorld::updateEnemySilence(float dt)
{
	isEnemyAttack = true;
	enemySilence->removeFromParentAndCleanup(true);
}

void HelloPhysicsWorld::updateOurSilence(float dt)
{
	isOurAttack = true;
	ourbatterySilence->removeFromParentAndCleanup(true);
}

void HelloPhysicsWorld::updateTime(float dt)
{
	remainTime--;
	stringstream ss;
	ss << remainTime;
	timeLabel->setString(ss.str());
	ss.str("");
	if (remainTime == 10)
	{
		timeLabel->setColor(Color3B::RED);
	}
	if (remainTime <= 0)
	{
		UserDefault::getInstance()->setBoolForKey("isWin", false);
		auto scene = FinishScene::createScene();
		Director::sharedDirector()->replaceScene(scene);
	}
}

void HelloPhysicsWorld::updateItem(float dt)
{
	Point p = Point(cocos2d::random(100, 1200), random(500, 800));
	int temp = (int)cocos2d::random(0, 3);
	if (0 == temp)
	{
		actionManager->addIntoScene(this, actionManager->createIceItem(p), 1);
	}
	else if (1 == temp)
	{
		actionManager->addIntoScene(this, actionManager->createTimeItem(p), 1);
	}
	else
	{
		actionManager->addIntoScene(this, actionManager->createDefendItem(p), 1);
	}
}

