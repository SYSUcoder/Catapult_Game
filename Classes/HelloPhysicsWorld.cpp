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
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);   

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
	winTheGame = 5;
	loseTheGame = 5;
	borderSize = cocos2d::Size(1024 * 1.3, 500);
	preTouchPoint = Vec2::ZERO;
	currTouchPoint = Vec2::ZERO;
	isEnemyAttack = true;
	isOurAttack = true;
	remainTime = 60;
	ourBirdCount = 0;
	enemyBirdCount = 0;

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
	mapSprite = Sprite::create("body.png");
	mapSprite->setAnchorPoint(Vec2(0.5, 0));
	mapSprite->setPosition(Vec2(190, 230));
	mapSprite->setScale(0.2, 0.2);
	mapSprite->setTag(7);
	auto batteryBody = PhysicsBody::createBox(Size(160, 100), PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(-33, -33));
	batteryBody->setDynamic(false);
	mapSprite->setPhysicsBody(batteryBody);
	// 设置掩码
	mapSprite->getPhysicsBody()->setCategoryBitmask(0x01F);
	mapSprite->getPhysicsBody()->setContactTestBitmask(0x03);
	mapSprite->getPhysicsBody()->setCollisionBitmask(0x01F);
	addChild(mapSprite);

	left_head = Sprite::create("head.png");
	left_head->setPosition(Vec2(185, 275));
	processBarPoint.setPoint(left_head->getPosition().x, left_head->getPosition().y);
	left_head->setScale(0.2, 0.2);
	left_head->setTag(7);

	batteryBody = PhysicsBody::createBox(Size(left_head->getContentSize().width * 0.5, left_head->getContentSize().height * 0.5));
	batteryBody->setDynamic(false);
	left_head->setPhysicsBody(batteryBody);
	// 设置掩码
	left_head->getPhysicsBody()->setCategoryBitmask(0x01F);
	left_head->getPhysicsBody()->setContactTestBitmask(0x03);
	left_head->getPhysicsBody()->setCollisionBitmask(0x01F);
	
	addChild(left_head);
	

	// enemy's battery
	mapSprite = Sprite::create("body1.png");
	mapSprite->setAnchorPoint(Vec2(0.5, 0));
	mapSprite->setPosition(Vec2(visibleSize.width - 190, 230));
	mapSprite->setScale(0.2, 0.2);
	mapSprite->setTag(10);
	batteryBody = PhysicsBody::createBox(Size(160, 100), PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(33, -33));
	batteryBody->setDynamic(false);
	mapSprite->setPhysicsBody(batteryBody);
	// 设置掩码
	mapSprite->getPhysicsBody()->setCategoryBitmask(0x01F);
	mapSprite->getPhysicsBody()->setContactTestBitmask(0x01);
	mapSprite->getPhysicsBody()->setCollisionBitmask(0x01F);
	addChild(mapSprite);

    right_head = Sprite::create("head1.png");
	right_head->setPosition(Vec2(visibleSize.width - 185, 275));
	right_head->setScale(0.2, 0.2);
	right_head->setTag(10);

	batteryBody = PhysicsBody::createBox(Size(right_head->getContentSize().width * 0.5, right_head->getContentSize().height * 0.5));
	batteryBody->setDynamic(false);
	right_head->setPhysicsBody(batteryBody);
	// 设置掩码
	right_head->getPhysicsBody()->setCategoryBitmask(0x01F);
	right_head->getPhysicsBody()->setContactTestBitmask(0x01);
	right_head->getPhysicsBody()->setCollisionBitmask(0x01F);

	addChild(right_head);

	double increase_height = 100;
	// point pig
	createPigPoint(Vec2(882, 317 + increase_height));
	createPigPoint(Vec2(1282, 605 + increase_height));
	createPigPoint(Vec2(1284, 179 + increase_height));
	createPigPoint(Vec2(1193, 368 + increase_height));
	createPigPoint(Vec2(777, 179 + increase_height));

	createOurPig(Vec2(visibleSize.width - 882, 317 + increase_height));
	createOurPig(Vec2(visibleSize.width - 1282, 605 + increase_height));
	createOurPig(Vec2(visibleSize.width - 1284, 179 + increase_height));
	createOurPig(Vec2(visibleSize.width - 1193, 368 + increase_height));
	createOurPig(Vec2(visibleSize.width - 777, 179 + increase_height));

	// middle block
	
	createBlock(Vec2(visibleSize.width / 2, 275));
	createRepeatActionBlock(Point(visibleSize.width / 2, 375), Point(visibleSize.width / 2, 275), 1);
	createBlock(Vec2(visibleSize.width / 2, 385));
	createRepeatActionBlock(Point(visibleSize.width / 2, 485), Point(visibleSize.width / 2, 385), 2);
	createBlock(Vec2(visibleSize.width / 2, 495));
	createRepeatActionBlock(Point(visibleSize.width / 2, 595), Point(visibleSize.width / 2, 495), 3);


	// processBar
	processBar = Sprite::create("Arrow.png");
	processBar->setPosition(220, 320); // 蓄力条位置
	processBar->setScale(0.2, 0.2);
	processBar->setVisible(false);
	
	this->addChild(processBar, 0);

	// to cover processBar
	processBarCover = Sprite::create("ArrowCover.png");
	timer = ProgressTimer::create(processBarCover); // progressTimer
	timer->setPosition(220, 320);
	timer->setType(kCCProgressTimerTypeBar);
	timer->setBarChangeRate(Vec2(1, 0));
	timer->setMidpoint(Vec2(0, 0));
	timer->setScale(0.2, 0.2);
	this->addChild(timer, 0);

	// event listener
	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(HelloPhysicsWorld::onContactBegin, this);
	//listener->onContactPreSolve = CC_CALLBACK_1(HelloPhysicsWorld::onContactPreSolve, this);
	//listener->onContactPostSolve = CC_CALLBACK_1(HelloPhysicsWorld::onContactPostSolve, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	// AI攻击
	schedule(schedule_selector(HelloPhysicsWorld::updateEnemyAttack), 2.0f);

	// item appears
	schedule(schedule_selector(HelloPhysicsWorld::updateItem), 3.0f);
	                                           
	setTouchEnabled(true);                                              
	//设置为单点触摸
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	return true;
}



bool HelloPhysicsWorld::onTouchBegan(Touch* touch, Event* event)                 
{
	currTouchPoint = touch->getLocation();
	if (!currTouchPoint.equals(preTouchPoint)){
		rotateArrow(currTouchPoint);
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
		rotateArrow(currTouchPoint);
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
		createOurBird();
		unschedule(schedule_selector(HelloPhysicsWorld::scheduleFunc));
		timer->setPercentage(0);
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
				createEnemySilence(Vec2(right_head->getPosition().x + 20, right_head->getPosition().y + 35));
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
				createOurSilence(Vec2(left_head->getPosition().x - 20, left_head->getPosition().y + 35));
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
				createEnemySilence(Vec2(right_head->getPosition().x + 20, right_head->getPosition().y + 35));
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
			remainTime += 30;
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

/*
bool onContactPreSolve(PhysicsContact& contact)
{
	int a = 1;
	return false;
}


void onContactPostSolve(PhysicsContact& contact)
{
	
}
*/

/*
pig :  category   11
		contact    01
		collision  01

bird:  category   101
		contact    1001
		collision  101

our_p: category   1010
		contact    101
		collision  1010

enemy: category   1110
		contact    1001
		collision  11

block: category   11111
		contact    01
		collision  11111

tomb:  category   10000
		contact    01
		collision  1000

our_b  category   11111
		contact    11
		collision  11111

ene_b  category   11111
		contact    01
		collision  11111
*/

void HelloPhysicsWorld::createOurBird()                                 
{
	auto sp = Sprite::create("our_bird.png");                                                                                          
	sp->setTag(5);
	sp->setScale(0.05, 0.05);
	auto body = PhysicsBody::createCircle(sp->getContentSize().width / 2);    

	auto total_speed = timer->getPercentage() * 5;
	double origin_speed = 0;
	body->setVelocity(Vec2(origin_speed + total_speed * cos(M_PI / 180 * rotateDegrees), -1 * (origin_speed + total_speed * sin(M_PI / 180 * rotateDegrees))));
	body->setGravityEnable(true);                   
	sp->setPhysicsBody(body);                                            
	sp->setPosition(processBarPoint);

	// 设置掩码
	sp->getPhysicsBody()->setCategoryBitmask(0x025);
	sp->getPhysicsBody()->setContactTestBitmask(0x09);
	sp->getPhysicsBody()->setCollisionBitmask(0x05);

	this->addChild(sp, 1);

	ourBirdVec.pushBack(sp);
	ourBirdCount++;
}

void HelloPhysicsWorld::createEnemyBird(Point p)
{
	auto sp = Sprite::create("enemy_bird.png");
	sp->setTag(8);
	sp->setScale(0.05, 0.05);
	auto body = PhysicsBody::createCircle(sp->getContentSize().width / 2);
	auto x_speed = cocos2d::random(-500, -300);
	auto y_speed = cocos2d::random(250, 350);
	body->setVelocity(Vec2(x_speed, y_speed));
	body->setGravityEnable(true);
	sp->setPhysicsBody(body);
	sp->setPosition(p);
	// 设置掩码
	sp->getPhysicsBody()->setCategoryBitmask(0x0E);
	sp->getPhysicsBody()->setContactTestBitmask(0x09);
	sp->getPhysicsBody()->setCollisionBitmask(0x03);
	addChild(sp, 1);

	enemyBirdVec.pushBack(sp);
	enemyBirdCount++;
}


// add pig point
void HelloPhysicsWorld::createPigPoint(Point p)
{
	auto pointSprite = Sprite::create("enemy_pig.png");
	pointSprite->setPosition(p);
	pointSprite->setTag(6);
	pointSprite->setScale(0.05, 0.05);
	double pigRadius = pointSprite->getContentSize().width / 2;
	auto body = PhysicsBody::createCircle(pigRadius);
	pointSprite->setPhysicsBody(body);
	// 设置掩码
	pointSprite->getPhysicsBody()->setCategoryBitmask(0x03);
	pointSprite->getPhysicsBody()->setContactTestBitmask(0x01);
	pointSprite->getPhysicsBody()->setCollisionBitmask(0x01);
	addChild(pointSprite, 1);

	createUnderBlock(Point(p.x, p.y - pigRadius * 0.05));
}

void HelloPhysicsWorld::createOurPig(Point p)
{
	auto pointSprite = Sprite::create("our_pig.png");
	pointSprite->setPosition(p);
	pointSprite->setTag(9);
	pointSprite->setScale(0.05, 0.05);
	double pigRadius = pointSprite->getContentSize().width / 2;
	auto body = PhysicsBody::createCircle(pigRadius);
	pointSprite->setPhysicsBody(body);
	// 设置掩码
	pointSprite->getPhysicsBody()->setCategoryBitmask(0x0A);
	pointSprite->getPhysicsBody()->setContactTestBitmask(0x05);
	pointSprite->getPhysicsBody()->setCollisionBitmask(0x0A);
	addChild(pointSprite, 1);

	createUnderBlock(Point(p.x, p.y - pigRadius * 0.05));
}

void HelloPhysicsWorld::createBlock(Point p)
{
	Point temp = p;
	blockSprite = Sprite::create("block1.png");
	blockSprite->setPosition(temp);
	auto body = PhysicsBody::createBox(blockSprite->getContentSize());
	body->setDynamic(false);
	blockSprite->setPhysicsBody(body);
	
	// 设置掩码
	blockSprite->getPhysicsBody()->setCategoryBitmask(0x01F);
	blockSprite->getPhysicsBody()->setContactTestBitmask(0x01);
	blockSprite->getPhysicsBody()->setCollisionBitmask(0x01F);

	addChild(blockSprite, 1);
}


void HelloPhysicsWorld::createUnderBlock(Point p)
{
	auto pointSprite = Sprite::create("block2.png");
	pointSprite->setPosition(Point(p.x, p.y - pointSprite->getContentSize().height));
	auto body = PhysicsBody::createBox(pointSprite->getContentSize());
	body->setDynamic(false);
	pointSprite->setPhysicsBody(body);
	// 设置掩码
	pointSprite->getPhysicsBody()->setCategoryBitmask(0x01F);
	pointSprite->getPhysicsBody()->setContactTestBitmask(0x01);
	pointSprite->getPhysicsBody()->setCollisionBitmask(0x01F);
	addChild(pointSprite, 1);
}

void HelloPhysicsWorld::createTombstone(Point p)
{
	auto tombSprite = Sprite::create("tombstone.png");
	tombSprite->setPosition(p);
	tombSprite->setTag(11);
	tombSprite->setScale(0.05, 0.05);
	auto body = PhysicsBody::createBox(tombSprite->getContentSize());
	tombSprite->setPhysicsBody(body);

	// 设置掩码
	tombSprite->getPhysicsBody()->setCategoryBitmask(0x010);
	tombSprite->getPhysicsBody()->setContactTestBitmask(0x01);
	tombSprite->getPhysicsBody()->setCollisionBitmask(0x08);
	addChild(tombSprite, 0);
}

// silence after being attacked
void HelloPhysicsWorld::createEnemySilence(Point p)
{
	enemySilence = Sprite::create("silence.png");
	enemySilence->setPosition(p);
	enemySilence->setScale(0.5, 0.5);
	addChild(enemySilence);
}

void HelloPhysicsWorld::createOurSilence(Point p)
{
	ourbatterySilence = Sprite::create("our_silence.png");
	ourbatterySilence->setPosition(p);
	ourbatterySilence->setScale(0.5, 0.5);
	addChild(ourbatterySilence);
}

void HelloPhysicsWorld::createRepeatActionBlock(Point p1, Point p2, double t)
{
	auto moveTo = MoveTo::create(t, p1);
	auto moveBack = MoveTo::create(t, p2);
	auto action = Sequence::create(moveTo, moveBack, NULL);
	auto repeat = RepeatForever::create(action);
	blockSprite->runAction(repeat);
}

void HelloPhysicsWorld::createIceItem(Point p)
{
	itemSprite = Sprite::create("iceItem.png");
	itemSprite->setPosition(p);
	itemSprite->setScale(0.2, 0.2);
	itemSprite->setTag(12);
	auto itemBody = PhysicsBody::createCircle(itemSprite->getContentSize().width / 2);
	itemBody->setGravityEnable(false);
	itemBody->setVelocity(Vec2(0, -30));
	itemSprite->setPhysicsBody(itemBody);
	// 设置掩码
	itemSprite->getPhysicsBody()->setCategoryBitmask(0x01);
	itemSprite->getPhysicsBody()->setContactTestBitmask(0x03);
	itemSprite->getPhysicsBody()->setCollisionBitmask(0x020);

	addChild(itemSprite, 1);
}

void HelloPhysicsWorld::createTimeItem(Point p)
{
	auto itemSprite = Sprite::create("timeItem.png");
	itemSprite->setPosition(p);
	itemSprite->setScale(0.2, 0.2);
	itemSprite->setTag(13);
	auto itemBody = PhysicsBody::createCircle(itemSprite->getContentSize().width / 2);
	itemBody->setGravityEnable(false);
	itemBody->setVelocity(Vec2(0, -30));
	itemSprite->setPhysicsBody(itemBody);
	// 设置掩码
	itemSprite->getPhysicsBody()->setCategoryBitmask(0x01);
	itemSprite->getPhysicsBody()->setContactTestBitmask(0x03);
	itemSprite->getPhysicsBody()->setCollisionBitmask(0x020);

	addChild(itemSprite, 1);
}

void HelloPhysicsWorld::createDefendItem(Point p)
{
	auto itemSprite = Sprite::create("defendItem.png");
	itemSprite->setPosition(p);
	itemSprite->setScale(0.2, 0.2);
	itemSprite->setTag(14);
	auto itemBody = PhysicsBody::createCircle(itemSprite->getContentSize().width / 2);
	itemBody->setGravityEnable(false);
	itemBody->setVelocity(Vec2(0, -30));
	itemSprite->setPhysicsBody(itemBody);
	// 设置掩码
	itemSprite->getPhysicsBody()->setCategoryBitmask(0x01);
	itemSprite->getPhysicsBody()->setContactTestBitmask(0x03);
	itemSprite->getPhysicsBody()->setCollisionBitmask(0x020);

	addChild(itemSprite, 1);
}

// rotate processbar
void HelloPhysicsWorld::rotateArrow(Point touchPoint)
{
	Point vector = touchPoint - processBarPoint;
	rotateRadians = vector.getAngle();
	rotateDegrees = CC_RADIANS_TO_DEGREES(-1 * rotateRadians);
	auto speed = 0.5 / M_PI;
	auto rotateDuration = fabs(rotateRadians * speed);
	processBar->runAction(Sequence::create(RotateTo::create(rotateDuration, rotateDegrees),
		NULL));
	timer->runAction(Sequence::create(RotateTo::create(rotateDuration, rotateDegrees),
		NULL));
	left_head->runAction(Sequence::create(RotateTo::create(rotateDuration, rotateDegrees),
		NULL));
}

void HelloPhysicsWorld::scheduleFunc(float dt)
{
	auto increase = timer->getPercentage() + 1.4;
	if (increase > 100) increase = 100;
	timer->setPercentage(increase);

	if (timer->getPercentage() >= 100)
	{
		unschedule(schedule_selector(HelloPhysicsWorld::scheduleFunc));
	}
}

void HelloPhysicsWorld::updateTwoCollison(float dt)
{
	createTombstone(collisionPointA);
	createTombstone(collisionPointB);
}

void HelloPhysicsWorld::updateOneCollison(float dt)
{
	createTombstone(collisionPointA);
}

void HelloPhysicsWorld::updateEnemyAttack(float dt)
{
	if (isEnemyAttack)
		createEnemyBird(Point(visibleSize.width - processBarPoint.x, processBarPoint.y));
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
	Point p = Point(cocos2d::random(300, 800), random(300, 800));
	int temp = (int)cocos2d::random(0, 3);
	if (0 == temp)
	{
		createIceItem(p);
	}
	else if (1 == temp)
	{
		createTimeItem(p);
	}
	else
	{
		createDefendItem(p);
	}
}

