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
// Tag:   11 is tombstone

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

	// map
	auto mapSprite = Sprite::create("map.jpg");
	mapSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	addChild(mapSprite, 0);

	// test label
	HelloPhysicsWorld::label = Label::create();
	label->setPosition(Vec2(50, visibleSize.height - 50));
	label->setAnchorPoint(Vec2::ZERO);
	label->setSystemFontSize(30);
	this->addChild(label, 1);

	// our battery
	mapSprite = Sprite::create("battery.png");
	mapSprite->setPosition(Vec2(212, 204));
	mapSprite->setScale(0.5, 0.5);
	mapSprite->setTag(7);
	auto batteryBody = PhysicsBody::createBox(Size(160, 100), PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(-33, -33));
	batteryBody->setDynamic(false);
	mapSprite->setPhysicsBody(batteryBody);
	// 设置掩码
	mapSprite->getPhysicsBody()->setCategoryBitmask(0x01F);
	mapSprite->getPhysicsBody()->setContactTestBitmask(0x01);
	mapSprite->getPhysicsBody()->setCollisionBitmask(0x01F);
	addChild(mapSprite);

	// enemy's battery
	mapSprite = Sprite::create("battery1.png");
	mapSprite->setPosition(Vec2(visibleSize.width - 212, 204));
	mapSprite->setScale(0.5, 0.5);
	mapSprite->setTag(10);
	batteryBody = PhysicsBody::createBox(Size(160, 100), PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(33, -33));
	batteryBody->setDynamic(false);
	mapSprite->setPhysicsBody(batteryBody);
	// 设置掩码
	mapSprite->getPhysicsBody()->setCategoryBitmask(0x01F);
	mapSprite->getPhysicsBody()->setContactTestBitmask(0x01);
	mapSprite->getPhysicsBody()->setCollisionBitmask(0x01F);
	addChild(mapSprite);

	// point pig
	createPigPoint(Vec2(882, 317));
	createPigPoint(Vec2(1282, 605));
	createPigPoint(Vec2(1284, 179));
	createPigPoint(Vec2(1193, 368));
	createPigPoint(Vec2(777, 179));

	createOurPig(Vec2(visibleSize.width - 882, 317));
	createOurPig(Vec2(visibleSize.width - 1282, 605));
	createOurPig(Vec2(visibleSize.width - 1284, 179));
	createOurPig(Vec2(visibleSize.width - 1193, 368));
	createOurPig(Vec2(visibleSize.width - 777, 179));

	// middle block
	createBlock(Vec2(visibleSize.width / 2, 175));
	createBlock(Vec2(visibleSize.width / 2, 225));
	createBlock(Vec2(visibleSize.width / 2, 275));
	createBlock(Vec2(visibleSize.width / 2, 325));
	createBlock(Vec2(visibleSize.width / 2, 375));

	// processBar
	processBar = Sprite::create("Arrow.png");
	processBar->setAnchorPoint(Vec2(0, 0.5));
	processBar->setPosition(230, 230); // 蓄力条位置
	processBar->setScale(0.18, 0.18);
	processBar->setVisible(false);
	processBarPoint.setPoint(processBar->getPosition().x, processBar->getPosition().y);
	this->addChild(processBar, 0);

	// to cover processBar
	processBarCover = Sprite::create("ArrowCover.png");
	timer = ProgressTimer::create(processBarCover); // progressTimer
	timer->setAnchorPoint(Vec2(0, 0.5));
	timer->setPosition(230, 230);
	timer->setType(kCCProgressTimerTypeBar);
	timer->setBarChangeRate(Vec2(1, 0));
	timer->setMidpoint(Vec2(0, 0));
	timer->setScale(0.18, 0.18);
	this->addChild(timer, 0);

	// event listener
	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(HelloPhysicsWorld::onContactBegin, this);
	//listener->onContactPreSolve = CC_CALLBACK_1(HelloPhysicsWorld::onContactPreSolve, this);
	//listener->onContactPostSolve = CC_CALLBACK_1(HelloPhysicsWorld::onContactPostSolve, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	// AI攻击
	schedule(schedule_selector(HelloPhysicsWorld::updateEnemyAttack), 3.0f);

	//定义世界的边界
	auto body = PhysicsBody::createEdgeBox(borderSize);
	body->setContactTestBitmask(0x01);
	auto edgeNode = Node::create();                                         
	edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));               
	edgeNode->setPhysicsBody(body);
	this->addChild(edgeNode);                                            
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
	speed = Vec2::ZERO;
	schedule(schedule_selector(HelloPhysicsWorld::scheduleFunc), 0.02f);
	processBar->setVisible(true);
	
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
	createOurBird();
	unschedule(schedule_selector(HelloPhysicsWorld::scheduleFunc));
	timer->setPercentage(0);
	processBar->setVisible(false);
}


bool HelloPhysicsWorld::onContactBegin(PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA && nodeB)
	{
		// collision of our bird and enemy's pig
		if ((nodeA->getTag() == 5 && nodeB->getTag() == 8) || (nodeA->getTag() == 8 && nodeB->getTag() == 5))
		{
			collisionPointA = contact.getShapeA()->getBody()->getPosition();
			collisionPointB = contact.getShapeB()->getBody()->getPosition();
			scheduleOnce(schedule_selector(HelloPhysicsWorld::updateTwoCollison), 0.01f);
			nodeA->removeFromParentAndCleanup(true);
			nodeB->removeFromParentAndCleanup(true);
		}
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
* enemy_pig :  category   11
*              contact    01
*              collision  01
*
* bird:        category   01
*			   contact    10
*			   collision  01
*
* our_p:       category   110
*			   contact    01
*			   collision  110
*
* enemy:	   category   11
*			   contact    01
*			   collision  11
*
* block:       category   101
*			   contact    101
*			   collision  101
*
* tomb:        category   10001
*              contact    01
*              collision  1100
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
	sp->getPhysicsBody()->setCategoryBitmask(0x05);
	sp->getPhysicsBody()->setContactTestBitmask(0x09);
	sp->getPhysicsBody()->setCollisionBitmask(0x05);

	this->addChild(sp, 1);
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
	auto blockSprite = Sprite::create("block1.png");
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
	auto body = PhysicsBody::createBox(tombSprite->getContentSize());
	tombSprite->setPhysicsBody(body);

	// 设置掩码
	tombSprite->getPhysicsBody()->setCategoryBitmask(0x010);
	tombSprite->getPhysicsBody()->setContactTestBitmask(0x01);
	tombSprite->getPhysicsBody()->setCollisionBitmask(0x08);
	addChild(tombSprite, 0);
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
	createEnemyBird(Point(visibleSize.width - processBarPoint.x, processBarPoint.y));
}