#include "Action.h"
#include "SimpleAudioEngine.h"
#include <string>
#include <sstream>
#include <cmath>
#include <iostream>

using std::string;
using std::stringstream;

USING_NS_CC;

// Tag:   5 is our bird, 6 is enemy's pigPoint, 7 is our battery
// Tag:   8 is enemy's bird, 9 is our pigPoint, 10 is enemy's battery
// Tag:   11 is tombstone, 12 is ice item


/*
edge:  category   11111
contact    01
collision  11111

pig :  category   11
contact    01
collision  01

bird:  category   100101
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

item   category   01
contact    11
collision  100000
*/

My_Action::My_Action()
{
	
}

Node* My_Action::createOurBattery(Point p)
{
	auto mapSprite = Sprite::create("body.png");
	mapSprite->setAnchorPoint(Vec2(0.5, 0));
	mapSprite->setPosition(p);
	mapSprite->setScale(0.2, 0.2);
	mapSprite->setTag(7);
	auto batteryBody = PhysicsBody::createBox(Size(160, 100), PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(-33, -33));
	batteryBody->setDynamic(false);
	mapSprite->setPhysicsBody(batteryBody);
	// ÉèÖÃÑÚÂë
	mapSprite->getPhysicsBody()->setCategoryBitmask(0x01F);
	mapSprite->getPhysicsBody()->setContactTestBitmask(0x03);
	mapSprite->getPhysicsBody()->setCollisionBitmask(0x01F);
	
	return mapSprite;
}

Node* My_Action::createOurHead(Point p)
{
	left_head = Sprite::create("head.png");
	left_head->setPosition(p);
	left_head->setScale(0.2, 0.2);
	left_head->setTag(7);

	auto batteryBody = PhysicsBody::createBox(Size(left_head->getContentSize().width * 0.5, left_head->getContentSize().height * 0.5));
	batteryBody->setDynamic(false);
	left_head->setPhysicsBody(batteryBody);
	// ÉèÖÃÑÚÂë
	left_head->getPhysicsBody()->setCategoryBitmask(0x01F);
	left_head->getPhysicsBody()->setContactTestBitmask(0x03);
	left_head->getPhysicsBody()->setCollisionBitmask(0x01F);

	return left_head;
}

Node* My_Action::createEnemyBattery(Point p)
{
	auto mapSprite = Sprite::create("body1.png");
	mapSprite->setAnchorPoint(Vec2(0.5, 0));
	mapSprite->setPosition(p);
	mapSprite->setScale(0.2, 0.2);
	mapSprite->setTag(10);
	auto batteryBody = PhysicsBody::createBox(Size(160, 100), PHYSICSSHAPE_MATERIAL_DEFAULT, Vec2(33, -33));
	batteryBody->setDynamic(false);
	mapSprite->setPhysicsBody(batteryBody);
	// ÉèÖÃÑÚÂë
	mapSprite->getPhysicsBody()->setCategoryBitmask(0x01F);
	mapSprite->getPhysicsBody()->setContactTestBitmask(0x01);
	mapSprite->getPhysicsBody()->setCollisionBitmask(0x01F);
	
	return mapSprite;
}

Node* My_Action::createEnemyHead(Point p)
{
	right_head = Sprite::create("head1.png");
	right_head->setPosition(p);
	right_head->setScale(0.2, 0.2);
	right_head->setTag(10);

	auto batteryBody = PhysicsBody::createBox(Size(right_head->getContentSize().width * 0.5, right_head->getContentSize().height * 0.5));
	batteryBody->setDynamic(false);
	right_head->setPhysicsBody(batteryBody);
	// ÉèÖÃÑÚÂë
	right_head->getPhysicsBody()->setCategoryBitmask(0x01F);
	right_head->getPhysicsBody()->setContactTestBitmask(0x01);
	right_head->getPhysicsBody()->setCollisionBitmask(0x01F);

	return right_head;
}

Node* My_Action::createProcessBar(Point p)
{
	processBar = Sprite::create("Arrow.png");
	processBar->setPosition(p); // ÐîÁ¦ÌõÎ»ÖÃ
	processBar->setScale(0.2, 0.2);
	processBar->setVisible(false);

	return processBar;
}

Node* My_Action::createProcessBarCover(Point p)
{
	auto processBarCover = Sprite::create("ArrowCover.png");
	timer = ProgressTimer::create(processBarCover); // progressTimer
	timer->setPosition(p);
	timer->setType(kCCProgressTimerTypeBar);
	timer->setBarChangeRate(Vec2(1, 0));
	timer->setMidpoint(Vec2(0, 0));
	timer->setScale(0.2, 0.2);
	
	return timer;
}

Node* My_Action::createOurBird(Point p)
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
	sp->setPosition(p);

	// ÉèÖÃÑÚÂë
	sp->getPhysicsBody()->setCategoryBitmask(0x025);
	sp->getPhysicsBody()->setContactTestBitmask(0x09);
	sp->getPhysicsBody()->setCollisionBitmask(0x05);

	return sp;
}

Node* My_Action::createEnemyBird(Point p)
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
	// ÉèÖÃÑÚÂë
	sp->getPhysicsBody()->setCategoryBitmask(0x0E);
	sp->getPhysicsBody()->setContactTestBitmask(0x09);
	sp->getPhysicsBody()->setCollisionBitmask(0x03);

	return sp;
}


// add pig point
Node* My_Action::createPigPoint(Point p)
{
	auto pointSprite = Sprite::create("enemy_pig.png");
	pointSprite->setPosition(p);
	pointSprite->setTag(6);
	pointSprite->setScale(0.05, 0.05);
	double pigRadius = pointSprite->getContentSize().width / 2;
	auto body = PhysicsBody::createCircle(pigRadius);
	pointSprite->setPhysicsBody(body);
	// ÉèÖÃÑÚÂë
	pointSprite->getPhysicsBody()->setCategoryBitmask(0x03);
	pointSprite->getPhysicsBody()->setContactTestBitmask(0x01);
	pointSprite->getPhysicsBody()->setCollisionBitmask(0x01);

	return pointSprite;
}

Node* My_Action::createOurPig(Point p)
{
	auto pointSprite = Sprite::create("our_pig.png");
	pointSprite->setPosition(p);
	pointSprite->setTag(9);
	pointSprite->setScale(0.05, 0.05);
	auto body = PhysicsBody::createCircle(pointSprite->getContentSize().width / 2);
	pointSprite->setPhysicsBody(body);
	// ÉèÖÃÑÚÂë
	pointSprite->getPhysicsBody()->setCategoryBitmask(0x0A);
	pointSprite->getPhysicsBody()->setContactTestBitmask(0x05);
	pointSprite->getPhysicsBody()->setCollisionBitmask(0x0A);

	return pointSprite;
}

Node* My_Action::createBlock(Point p)
{
	Point temp = p;
	blockSprite = Sprite::create("block1.png");
	blockSprite->setPosition(temp);
	auto body = PhysicsBody::createBox(blockSprite->getContentSize());
	body->setDynamic(false);
	blockSprite->setPhysicsBody(body);

	// ÉèÖÃÑÚÂë
	blockSprite->getPhysicsBody()->setCategoryBitmask(0x01F);
	blockSprite->getPhysicsBody()->setContactTestBitmask(0x01);
	blockSprite->getPhysicsBody()->setCollisionBitmask(0x01F);

	return blockSprite;
}


Node* My_Action::createUnderBlock(Point p)
{
	auto pointSprite = Sprite::create("block2.png");
	pointSprite->setPosition(Point(p.x, p.y - pointSprite->getContentSize().height));
	auto body = PhysicsBody::createBox(pointSprite->getContentSize());
	body->setDynamic(false);
	pointSprite->setPhysicsBody(body);
	// ÉèÖÃÑÚÂë
	pointSprite->getPhysicsBody()->setCategoryBitmask(0x01F);
	pointSprite->getPhysicsBody()->setContactTestBitmask(0x01);
	pointSprite->getPhysicsBody()->setCollisionBitmask(0x01F);
	
	return pointSprite;
}

Node* My_Action::createTombstone(Point p)
{
	auto tombSprite = Sprite::create("tombstone.png");
	tombSprite->setPosition(p);
	tombSprite->setTag(11);
	tombSprite->setScale(0.05, 0.05);
	auto body = PhysicsBody::createBox(tombSprite->getContentSize());
	tombSprite->setPhysicsBody(body);

	// ÉèÖÃÑÚÂë
	tombSprite->getPhysicsBody()->setCategoryBitmask(0x010);
	tombSprite->getPhysicsBody()->setContactTestBitmask(0x01);
	tombSprite->getPhysicsBody()->setCollisionBitmask(0x08);
	
	return tombSprite;
}

// silence after being attacked
Node* My_Action::createEnemySilence(Point p)
{
	auto enemySilence = Sprite::create("silence.png");
	enemySilence->setPosition(p);
	enemySilence->setScale(0.5, 0.5);
	
	return enemySilence;
}

Node* My_Action::createOurSilence(Point p)
{
	auto ourbatterySilence = Sprite::create("our_silence.png");
	ourbatterySilence->setPosition(p);
	ourbatterySilence->setScale(0.5, 0.5);
	
	return ourbatterySilence;
}

Node* My_Action::createIceItem(Point p)
{
	itemSprite = Sprite::create("iceItem.png");
	itemSprite->setPosition(p);
	itemSprite->setScale(0.2, 0.2);
	itemSprite->setTag(12);
	auto itemBody = PhysicsBody::createCircle(itemSprite->getContentSize().width / 2);
	itemBody->setGravityEnable(false);
	itemBody->setVelocity(Vec2(0, -30));
	itemSprite->setPhysicsBody(itemBody);
	// ÉèÖÃÑÚÂë
	itemSprite->getPhysicsBody()->setCategoryBitmask(0x01);
	itemSprite->getPhysicsBody()->setContactTestBitmask(0x03);
	itemSprite->getPhysicsBody()->setCollisionBitmask(0x020);

	return itemSprite;
}

Node* My_Action::createTimeItem(Point p)
{
	auto itemSprite = Sprite::create("timeItem.png");
	itemSprite->setPosition(p);
	itemSprite->setScale(0.2, 0.2);
	itemSprite->setTag(13);
	auto itemBody = PhysicsBody::createCircle(itemSprite->getContentSize().width / 2);
	itemBody->setGravityEnable(false);
	itemBody->setVelocity(Vec2(0, -30));
	itemSprite->setPhysicsBody(itemBody);
	// ÉèÖÃÑÚÂë
	itemSprite->getPhysicsBody()->setCategoryBitmask(0x01);
	itemSprite->getPhysicsBody()->setContactTestBitmask(0x03);
	itemSprite->getPhysicsBody()->setCollisionBitmask(0x020);

	return itemSprite;
}

Node* My_Action::createDefendItem(Point p)
{
	auto itemSprite = Sprite::create("defendItem.png");
	itemSprite->setPosition(p);
	itemSprite->setScale(0.2, 0.2);
	itemSprite->setTag(14);
	auto itemBody = PhysicsBody::createCircle(itemSprite->getContentSize().width / 2);
	itemBody->setGravityEnable(false);
	itemBody->setVelocity(Vec2(0, -30));
	itemSprite->setPhysicsBody(itemBody);
	// ÉèÖÃÑÚÂë
	itemSprite->getPhysicsBody()->setCategoryBitmask(0x01);
	itemSprite->getPhysicsBody()->setContactTestBitmask(0x03);
	itemSprite->getPhysicsBody()->setCollisionBitmask(0x020);

	return itemSprite;
}

void My_Action::createRepeatActionBlock(Point p1, Point p2, double t)
{
	auto moveTo = MoveTo::create(t, p1);
	auto moveBack = MoveTo::create(t, p2);
	auto action = Sequence::create(moveTo, moveBack, NULL);
	auto repeat = RepeatForever::create(action);
	blockSprite->runAction(repeat);
}

// rotate processbar
void My_Action::rotateArrow(Point touchPoint)
{
	Point processBarPoint = Point(185, 275);
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
