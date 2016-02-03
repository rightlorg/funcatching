#include "player.h"

Player::Player(QString *name)
{

}

Player::~Player()
{

}

QPointF Player::getRealPos() const
{
	return realPos;
}

void Player::setRealPos(const QPointF &value)
{
	realPos = value;
}

quint8 Player::getFloor() const
{
	return floor;
}

void Player::setFloor(const quint8 &value)
{
	floor = value;
}

QString Player::getPlayerName() const
{
	return playerName;
}

QImage Player::getHeadImage() const
{
	return headImage;
}

void Player::setHeadImage(const QPixmap &value)
{
	headImage = value;
	headImage = headImage.scaled(27, 27, Qt::IgnoreAspectRatio);

	this->setPixmap(headImage);

}
