#include "baseitem.h"

BaseItem::BaseItem( QGraphicsItem *parent) : QGraphicsItem(parent)
{

}

void BaseItem::init()
{
	initializeOpenGLFunctions();
	glViewport(0, 0, 487, 557);
}
