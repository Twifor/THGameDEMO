#include "openglgame.h"
#include "engine/itemmanager.h"
#include "gamewidget.h"

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
	slowEffectTimeLine = 1;
	slowEffectRotate1 = 0;
	slowEffectRotate2 = 45;
	ballTimeLine = 1;
	ItemManager::INSTANCE()->addItem(ItemManager::SLOWEFFECT1, 1, new SlowEffectEvent1);
	ItemManager::INSTANCE()->addItem(ItemManager::SLOWEFFECT2, 1, new SlowEffectEvent2);
	ItemManager::INSTANCE()->addItem(ItemManager::CENTER, 1, new CenterEvent);
}

void OpenGLGame::endShift()
{
	status &= ~16;
	slowEffectTimeLine = -20;
	ballTimeLine = -ballTimeLine;
}

void OpenGLGame::startZ()
{
	status |= 32;
}

void OpenGLGame::endZ()
{
	status &= ~32;
}

void OpenGLGame::levelUp()
{
	ItemManager::INSTANCE()->addItem(ItemManager::BALL, 1, new BallEvent(GameWidget::Instance->getLevel()));
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
	slowEffectRotate1 = 0;
	slowEffectAlpha1 = 0.0f;
	slowEffectTimeLine = 0;
	ballTimeLine = 0;
	ballLine = 0;
	//安装渲染器
	ItemManager::INSTANCE()->installRender(ItemManager::BACKGROUND, new BackGroundRender(TextureManager::BACKGROUND));
	ItemManager::INSTANCE()->installRender(ItemManager::STAR_BACKGROUND, new StarBackGroundRender(TextureManager::STAR_BACKGROUND));
	ItemManager::INSTANCE()->installRender(ItemManager::MARISA, new PlaneRender);
	ItemManager::INSTANCE()->installRender(ItemManager::TREE, new TreeRender(TextureManager::TREE));
	ItemManager::INSTANCE()->installRender(ItemManager::TREE2, new TreeRender2(TextureManager::TREE));
	ItemManager::INSTANCE()->installRender(ItemManager::SLOWEFFECT1, new SlowEffectRender1);
	ItemManager::INSTANCE()->installRender(ItemManager::SLOWEFFECT2, new SlowEffectRender2);
	ItemManager::INSTANCE()->installRender(ItemManager::CENTER, new CenterRender);
	ItemManager::INSTANCE()->installRender(ItemManager::BALL, new BallRender);
	ItemManager::INSTANCE()->installRender(ItemManager::MARISA_LINE, new LineRender);

	//注册事件
	ItemManager::INSTANCE()->addItem(ItemManager::BACKGROUND, 1, new BackGroundEvent(-26.0));
	ItemManager::INSTANCE()->addItem(ItemManager::BACKGROUND, 1, new BackGroundEvent(-14.0));
	ItemManager::INSTANCE()->addItem(ItemManager::BACKGROUND, 1, new BackGroundEvent(-2.0));
	ItemManager::INSTANCE()->addItem(ItemManager::STAR_BACKGROUND, 1, new NullEvent());
	ItemManager::INSTANCE()->addItem(ItemManager::MARISA, 1, new MarisaEvent);
	ItemManager::INSTANCE()->addItem(ItemManager::TREE, 1, new TreeEvent(-18.0f));
	ItemManager::INSTANCE()->addItem(ItemManager::TREE, 1, new TreeEvent(-6.0f));
	ItemManager::INSTANCE()->addItem(ItemManager::TREE2, 1, new TreeEvent(-2.0f));
	ItemManager::INSTANCE()->addItem(ItemManager::TREE2, 1, new TreeEvent(-14.0f));
	ItemManager::INSTANCE()->addItem(ItemManager::BALL, 1, new BallEvent(0));

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
		if(status & 16) myPlaneX -= 0.0125f;
		else myPlaneX -= 0.025f;
		if(myPlaneX <= -0.915f) myPlaneX = -0.915f;
	}
	if(status & 2) {
		if(status & 16) myPlaneX += 0.0125f;
		else myPlaneX += 0.025f;
		if(myPlaneX >= 0.915f) myPlaneX = 0.915f;
	}
	if(status & 4) {
		if(status & 16) myPlaneY += 0.0125f;
		else myPlaneY += 0.025f;
		if(myPlaneY >= 0.81f) myPlaneY = 0.81f;
	}
	if(status & 8) {
		if(status & 16) myPlaneY -= 0.0125f;
		else myPlaneY -= 0.025f;
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

	drawSlowEffect();
	drawBalls();
	drawLines();
}

void OpenGLGame::drawSlowEffect()
{
	if(slowEffectTimeLine == 0) return;
	if(slowEffectTimeLine > 0) {
		++slowEffectTimeLine;
		slowEffectAlpha1 = slowEffectTimeLine / (20.0f) * 0.8f;
		slowEffectAlpha2 = slowEffectTimeLine / (20.0f) * 0.4f;
		centerAlpha = slowEffectTimeLine / (20.0f) * 0.8f;
		if(slowEffectTimeLine == 20) slowEffectTimeLine = 19;
	}else{
		++slowEffectTimeLine;
		slowEffectAlpha1 = -slowEffectTimeLine / (20.0f) * 0.8f;
		slowEffectAlpha2 = -slowEffectTimeLine / (20.0f) * 0.4f;
		centerAlpha = -slowEffectTimeLine / (20.0f) * 0.8f;
	}
	slowEffectRotate1 += 6;
	slowEffectRotate2 -= 6;
	if(slowEffectRotate1 > 360) slowEffectRotate1 = 0;
	if(slowEffectRotate2 < 0) slowEffectRotate2 = 360;
}

void OpenGLGame::drawBalls()
{
	ballRotate += 3;
	if(ballRotate >= 360) ballRotate = 0;
	if(GameWidget::Instance->getLevel() == 0) {
		if(ballTimeLine == 0) {
			ballX[0] = myPlaneX;
			ballY[0] = myPlaneY + 0.12f;
		}else if(ballTimeLine > 0) {
			ballX[0] = myPlaneX;
			ballY[0] = myPlaneY + 0.12f - ballTimeLine / 400.0f;
			++ballTimeLine;
			if(ballTimeLine == 11) ballTimeLine = 10;
		}else{
			ballX[0] = myPlaneX;
			ballY[0] = myPlaneY + 0.12f + ballTimeLine / 400.0f;
			++ballTimeLine;
		}
	}else if(GameWidget::Instance->getLevel() == 1) {
		if(ballTimeLine == 0) {
			ballX[0] = myPlaneX - 0.06f;
			ballX[1] = myPlaneX + 0.06f;
			ballY[0] = myPlaneY + 0.12f;
			ballY[1] = myPlaneY + 0.12f;
		}else if(ballTimeLine > 0) {
			ballX[0] = myPlaneX - 0.06f + ballTimeLine / 500.0f;
			ballX[1] = myPlaneX + 0.06f - ballTimeLine / 500.0f;
			ballY[0] = myPlaneY + 0.12f - ballTimeLine / 500.0f;
			ballY[1] = myPlaneY + 0.12f - ballTimeLine / 500.0f;
			++ballTimeLine;
			if(ballTimeLine == 11) ballTimeLine = 10;
		}else{
			ballX[0] = myPlaneX - 0.06f - ballTimeLine / 500.0f;
			ballX[1] = myPlaneX + 0.06f + ballTimeLine / 500.0f;
			ballY[0] = myPlaneY + 0.12f + ballTimeLine / 500.0f;
			ballY[1] = myPlaneY + 0.12f + ballTimeLine / 500.0f;
			++ballTimeLine;
		}
	}else if(GameWidget::Instance->getLevel() == 2) {
		if(ballTimeLine == 0) {
			ballX[0] = myPlaneX - 0.12f;
			ballX[1] = myPlaneX;
			ballX[2] = myPlaneX + 0.12f;
			ballY[0] = myPlaneY + 0.08f;
			ballY[1] = myPlaneY + 0.16f;
			ballY[2] = myPlaneY + 0.08f;
		}else if(ballTimeLine > 0) {
			ballX[0] = myPlaneX - 0.12f + ballTimeLine / 150.0f;
			ballX[1] = myPlaneX;
			ballX[2] = myPlaneX + 0.12f - ballTimeLine / 150.0f;
			ballY[0] = myPlaneY + 0.08f - ballTimeLine / 800.0f;
			ballY[1] = myPlaneY + 0.16f - ballTimeLine / 200.0f;
			ballY[2] = myPlaneY + 0.08f - ballTimeLine / 800.0f;
			++ballTimeLine;
			if(ballTimeLine == 11) ballTimeLine = 10;
		}else{
			ballX[0] = myPlaneX - 0.12f - ballTimeLine / 150.0f;
			ballX[1] = myPlaneX;
			ballX[2] = myPlaneX + 0.12f + ballTimeLine / 150.0f;
			ballY[0] = myPlaneY + 0.08f + ballTimeLine / 800.0f;
			ballY[1] = myPlaneY + 0.16f + ballTimeLine / 200.0f;
			ballY[2] = myPlaneY + 0.08f + ballTimeLine / 800.0f;
			++ballTimeLine;
		}
	}else if(GameWidget::Instance->getLevel() == 3) {
		if(ballTimeLine == 0) {
			ballX[0] = myPlaneX - 0.16f;
			ballX[1] = myPlaneX - 0.08f;
			ballX[2] = myPlaneX + 0.08f;
			ballX[3] = myPlaneX + 0.16f;
			ballY[0] = myPlaneY + 0.08f;
			ballY[1] = myPlaneY + 0.16f;
			ballY[2] = myPlaneY + 0.16f;
			ballY[3] = myPlaneY + 0.08f;
		}else if(ballTimeLine > 0) {
			ballX[0] = myPlaneX - 0.16f + ballTimeLine / 150.0f;
			ballX[1] = myPlaneX - 0.08f + ballTimeLine / 250.0f;
			ballX[2] = myPlaneX + 0.08f - ballTimeLine / 250.0f;
			ballX[3] = myPlaneX + 0.16f - ballTimeLine / 150.0f;
			ballY[0] = myPlaneY + 0.08f - ballTimeLine / 800.0f;
			ballY[1] = myPlaneY + 0.16f - ballTimeLine / 200.0f;
			ballY[2] = myPlaneY + 0.16f - ballTimeLine / 200.0f;
			ballY[3] = myPlaneY + 0.08f - ballTimeLine / 800.0f;
			++ballTimeLine;
			if(ballTimeLine == 11) ballTimeLine = 10;
		}else{
			ballX[0] = myPlaneX - 0.16f - ballTimeLine / 150.0f;
			ballX[1] = myPlaneX - 0.08f - ballTimeLine / 250.0f;
			ballX[2] = myPlaneX + 0.08f + ballTimeLine / 250.0f;
			ballX[3] = myPlaneX + 0.16f + ballTimeLine / 150.0f;
			ballY[0] = myPlaneY + 0.08f + ballTimeLine / 800.0f;
			ballY[1] = myPlaneY + 0.16f + ballTimeLine / 200.0f;
			ballY[2] = myPlaneY + 0.16f + ballTimeLine / 200.0f;
			ballY[3] = myPlaneY + 0.08f + ballTimeLine / 800.0f;
			++ballTimeLine;
		}
	}else if(GameWidget::Instance->getLevel() == 4) {
		if(ballTimeLine == 0) {
			ballX[0] = myPlaneX - 0.16f;
			ballX[1] = myPlaneX - 0.10f;
			ballX[2] = myPlaneX;
			ballX[3] = myPlaneX + 0.10f;
			ballX[4] = myPlaneX + 0.16f;
			ballY[0] = myPlaneY + 0.08f;
			ballY[1] = myPlaneY + 0.14f;
			ballY[2] = myPlaneY + 0.16f;
			ballY[3] = myPlaneY + 0.14f;
			ballY[4] = myPlaneY + 0.08f;
		}else if(ballTimeLine > 0) {
			ballX[0] = myPlaneX - 0.16f + ballTimeLine / 150.0f;
			ballX[1] = myPlaneX - 0.10f + ballTimeLine / 250.0f;
			ballX[2] = myPlaneX;
			ballX[3] = myPlaneX + 0.10f - ballTimeLine / 250.0f;
			ballX[4] = myPlaneX + 0.16f - ballTimeLine / 150.0f;
			ballY[0] = myPlaneY + 0.08f - ballTimeLine / 800.0f;
			ballY[1] = myPlaneY + 0.14f - ballTimeLine / 400.0f;
			ballY[2] = myPlaneY + 0.16f - ballTimeLine / 300.0f;
			ballY[3] = myPlaneY + 0.14f - ballTimeLine / 400.0f;
			ballY[4] = myPlaneY + 0.08f - ballTimeLine / 800.0f;
			++ballTimeLine;
			if(ballTimeLine == 11) ballTimeLine = 10;
		}else{
			ballX[0] = myPlaneX - 0.16f - ballTimeLine / 150.0f;
			ballX[1] = myPlaneX - 0.10f - ballTimeLine / 250.0f;
			ballX[2] = myPlaneX;
			ballX[3] = myPlaneX + 0.10f + ballTimeLine / 250.0f;
			ballX[4] = myPlaneX + 0.16f + ballTimeLine / 150.0f;
			ballY[0] = myPlaneY + 0.08f + ballTimeLine / 800.0f;
			ballY[1] = myPlaneY + 0.14f + ballTimeLine / 400.0f;
			ballY[2] = myPlaneY + 0.16f + ballTimeLine / 300.0f;
			ballY[3] = myPlaneY + 0.14f + ballTimeLine / 400.0f;
			ballY[4] = myPlaneY + 0.08f + ballTimeLine / 800.0f;
			++ballTimeLine;
		}
	}
}

void OpenGLGame::drawLines()
{
	for(int i = 0; i <= GameWidget::Instance->getLevel() && (status & 16); i++) {
		float s = ballLine + ballY[i];
		ItemManager::INSTANCE()->addItem(ItemManager::MARISA_LINE, 1, new LineEvent(ballX[i], s * ItemManager::INSTANCE()->getDiv(), ballY[i] * ItemManager::INSTANCE()->getDiv()));
		s = s * ItemManager::INSTANCE()->getDiv() + 1.0f;
		while(s <= 1.5f) {
			ItemManager::INSTANCE()->addItem(ItemManager::MARISA_LINE, 1, new LineEvent(ballX[i], s, ballY[i] * ItemManager::INSTANCE()->getDiv()));
			s += 1.0f;
		}
	}
	ballLine += 0.05;
	if(ballLine >= 0.5f / ItemManager::INSTANCE()->getDiv()) ballLine = 0;
}
