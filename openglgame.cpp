#include "openglgame.h"
#include "engine/itemmanager.h"

OpenGLGame::OpenGLGame(QWidget *parent) : QOpenGLWidget(parent)
{
	Instance = this;
}

void OpenGLGame::startLeft()
{
	status |= 1;
	myPlaneTimeLine = 0;
	myPlaneOffset = 8;
}

OpenGLGame *OpenGLGame::Instance = nullptr;

void OpenGLGame::startRight()
{
	status |= 2;
	myPlaneTimeLine = 0;
	myPlaneOffset = 16;
}

void OpenGLGame::startUp()
{
	status |= 4;
}

void OpenGLGame::startDown()
{
	status |= 8;
}

void OpenGLGame::endtLeft()
{
	status &= ~1;
	myPlaneOffset = 0;
}

void OpenGLGame::endRight()
{
	status &= ~2;
	myPlaneOffset = 0;
}

void OpenGLGame::endUp()
{
	status &= ~4;
}

void OpenGLGame::endDown()
{
	status &= ~8;
}

void OpenGLGame::startShift()
{
	status |= 16;
}

void OpenGLGame::endShift()
{
	status &= ~16;
}

void OpenGLGame::startZ()
{
	status |= 32;
}

void OpenGLGame::endZ()
{
	status &= ~32;
}

void OpenGLGame::initializeGL()
{
	initializeOpenGLFunctions();
	glViewport(0, 0, 487, 557);

	ItemManager::INSTANCE()->setDiv(557.0f / 487.0f);
	backGroundMax = -26.0f;
	treeLMax = -18.0f;
	treeRMax = -14.0f;
	status = 0;
	myPlaneX = 0.0f;
	myPlaneY = -0.75f;
	myPlaneTimeLine = 0;
	myPlaneOffset = 0;
	ItemManager::INSTANCE()->installRender(ItemManager::BACKGROUND, new BackGroundRender(TextureManager::BACKGROUND));
	ItemManager::INSTANCE()->installRender(ItemManager::STAR_BACKGROUND, new StarBackGroundRender(TextureManager::STAR_BACKGROUND));
	ItemManager::INSTANCE()->installRender(ItemManager::MARISA, new PlaneRender);
	ItemManager::INSTANCE()->installRender(ItemManager::TREE, new TreeRender(TextureManager::TREE));
	ItemManager::INSTANCE()->installRender(ItemManager::TREE2, new TreeRender2(TextureManager::TREE));
	ItemManager::INSTANCE()->addItem(ItemManager::BACKGROUND, 1, new BackGroundEvent(-26.0));
	ItemManager::INSTANCE()->addItem(ItemManager::BACKGROUND, 1, new BackGroundEvent(-14.0));
	ItemManager::INSTANCE()->addItem(ItemManager::BACKGROUND, 1, new BackGroundEvent(-2.0));
	ItemManager::INSTANCE()->addItem(ItemManager::STAR_BACKGROUND, 1, new NullEvent());
	ItemManager::INSTANCE()->addItem(ItemManager::MARISA, 1, new MarisaEvent);
	ItemManager::INSTANCE()->addItem(ItemManager::TREE, 1, new TreeEvent(-18.0f));
	ItemManager::INSTANCE()->addItem(ItemManager::TREE, 1, new TreeEvent(-6.0f));
	ItemManager::INSTANCE()->addItem(ItemManager::TREE2, 1, new TreeEvent(-2.0f));
	ItemManager::INSTANCE()->addItem(ItemManager::TREE2, 1, new TreeEvent(-14.0f));
}

void OpenGLGame::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void OpenGLGame::paintGL()
{
	glClear( GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawBackGround();
	drawMyPlane();
	ItemManager::INSTANCE()->update();
}

void OpenGLGame::drawBackGround()
{
	backGroundMax += 0.08f;
	if(backGroundMax <= -14.0f && backGroundMax + 0.08f > -14.0f) {
		ItemManager::INSTANCE()->addItem(ItemManager::BACKGROUND, 1, new BackGroundEvent(-26.0f));
		backGroundMax = -26.0f;
	}
	treeLMax += 0.08f * 1.5f;
	if(treeLMax <= -10.0f && treeLMax + 0.08f * 1.5f > -10.0f) {
		ItemManager::INSTANCE()->addItem(ItemManager::TREE, 1, new TreeEvent(-18.0f));
		treeLMax = -18.0f;
	}
	treeRMax += 0.08f * 1.5f;
	if(treeRMax <= -6.0f && treeRMax + 0.08f * 1.5f > -6.0f) {
		ItemManager::INSTANCE()->addItem(ItemManager::TREE2, 1, new TreeEvent(-14.0f));
		treeRMax = -14.0f;
	}
}

void OpenGLGame::drawMyPlane()
{
	if(status & 1) {
		myPlaneX -= 0.025f;
		if(myPlaneX <= -0.915f) myPlaneX = -0.915f;
	}else if(status & 2) {
		myPlaneX += 0.025f;
		if(myPlaneX >= 0.915f) myPlaneX = 0.915f;
	}else if(status & 4) {
		myPlaneY += 0.025f;
		if(myPlaneY >= 0.81f) myPlaneY = 0.81f;
	}else if(status & 8) {
		myPlaneY -= 0.025f;
		if(myPlaneY <= -0.79f) myPlaneY = -0.79f;
	}
	++myPlaneTimeLine;
	if(myPlaneTimeLine == 5) {
		++myPlaneOffset;
		if(status & 1) {
			if(myPlaneOffset == 16) myPlaneOffset = 12;
		}else if(status & 2) {
			if(myPlaneOffset == 24) myPlaneOffset = 20;
		}else if(myPlaneOffset == 8) myPlaneOffset = 0;
		myPlaneTimeLine = 0;
	}
}
