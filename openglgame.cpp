#include "openglgame.h"
#include "engine/itemmanager.h"
#include "gamewidget.h"
#include "musicfactory.h"

#include <QGraphicsOpacityEffect>

OpenGLGame::OpenGLGame(QWidget *parent) : QOpenGLWidget(parent)
{
	Instance = this;
	pauseLock = false;
	pauseStatus = 0;
	setAttribute(Qt::WA_AlwaysStackOnTop);
}

OpenGLGame::~OpenGLGame()
{
	makeCurrent();
	ItemManager::INSTANCE()->destroy();
}

void OpenGLGame::pause()
{
	pauseLock = true;
	pauseStatus = 1;
	ItemManager::INSTANCE()->addItem(ItemManager::P1, 1, new PauseMenuEvent(0, -0.45f, false));
	ItemManager::INSTANCE()->addItem(ItemManager::P2B, 1, new PauseMenuEndEvent(1, -0.55f, false));
	ItemManager::INSTANCE()->addItem(ItemManager::P3B, 1, new PauseMenuEndEvent(2, -0.65f, false));
}

void OpenGLGame::endPause()
{
	if(pauseStatus > 4) {
		QSound::play(":/std/cancel.wav");
		pauseStatus &= ~24;
		if(pauseStatus == 1) ItemManager::INSTANCE()->addItem(ItemManager::P1, 1, new PauseMenuEvent(0, -0.45f, false));
		else ItemManager::INSTANCE()->addItem(ItemManager::P1B, 1, new PauseMenuEndEvent(0, -0.45f, false));
		if(pauseStatus == 2) ItemManager::INSTANCE()->addItem(ItemManager::P2, 1, new PauseMenuEvent(1, -0.55f, false));
		else ItemManager::INSTANCE()->addItem(ItemManager::P2B, 1, new PauseMenuEndEvent(1, -0.55f, false));
		if(pauseStatus == 4) ItemManager::INSTANCE()->addItem(ItemManager::P3, 1, new PauseMenuEvent(2, -0.65f, false));
		else ItemManager::INSTANCE()->addItem(ItemManager::P3B, 1, new PauseMenuEndEvent(2, -0.65f, false));
		return;
	}
	pauseLock = false;
	pauseStatus = -40;
}

bool OpenGLGame::isPause()
{
	return pauseLock;
}

void OpenGLGame::startLeft()
{
	if(pauseLock) return;
	if(status & 2) status &= ~2;
	status |= 1;
	myPlaneTimeLine = 0;
	myPlaneOffset = 8;
}

OpenGLGame *OpenGLGame::Instance = nullptr;

void OpenGLGame::startRight()
{
	if(pauseLock) return;
	if(status & 1) status &= ~1;
	status |= 2;
	myPlaneTimeLine = 0;
	myPlaneOffset = 16;
}

void OpenGLGame::startUp()
{
	if(pauseLock) {
		QSound::play(":/std/select.wav");
		if(pauseStatus <= 4) {
			if(pauseStatus == 1) ItemManager::INSTANCE()->addItem(ItemManager::P1B, 0, new PauseMenuEndEvent(0, -0.45f, true));
			else if(pauseStatus == 2) ItemManager::INSTANCE()->addItem(ItemManager::P2B, 0, new PauseMenuEndEvent(1, -0.55f, true));
			else ItemManager::INSTANCE()->addItem(ItemManager::P3B, 0, new PauseMenuEndEvent(2, -0.65f, true));
			if(pauseStatus == 1) pauseStatus = 4;
			else pauseStatus >>= 1;
			if(pauseStatus == 1) ItemManager::INSTANCE()->addItem(ItemManager::P1, 0, new PauseMenuEvent(0, -0.45f, true));
			else if(pauseStatus == 2) ItemManager::INSTANCE()->addItem(ItemManager::P2, 0, new PauseMenuEvent(1, -0.55f, true));
			else ItemManager::INSTANCE()->addItem(ItemManager::P3, 0, new PauseMenuEvent(2, -0.65f, true));
		}else{
			if(pauseStatus & (1 << 3)) ItemManager::INSTANCE()->addItem(ItemManager::PNB, 0, new PauseMenu2EndEvent(3, -0.55f, true));
			else ItemManager::INSTANCE()->addItem(ItemManager::PYB, 0, new PauseMenu2EndEvent(4, -0.65f, true));
			if(pauseStatus & (1 << 3)) {
				pauseStatus &= ~(1 << 3);
				pauseStatus |= 1 << 4;
				ItemManager::INSTANCE()->addItem(ItemManager::PY, 0, new PauseMenu2Event(4, -0.65f, true));
			}else{
				pauseStatus &= ~(1 << 4);
				pauseStatus |= 1 << 3;
				ItemManager::INSTANCE()->addItem(ItemManager::PN, 0, new PauseMenu2Event(3, -0.55f, true));
			}
		}
		return;
	}
	status |= 4;
}

void OpenGLGame::startDown()
{
	if(pauseLock) {
		QSound::play(":/std/select.wav");
		if(pauseStatus <= 4) {
			if(pauseStatus == 1) ItemManager::INSTANCE()->addItem(ItemManager::P1B, 0, new PauseMenuEndEvent(0, -0.45f, true));
			else if(pauseStatus == 2) ItemManager::INSTANCE()->addItem(ItemManager::P2B, 0, new PauseMenuEndEvent(1, -0.55f, true));
			else ItemManager::INSTANCE()->addItem(ItemManager::P3B, 0, new PauseMenuEndEvent(2, -0.65f, true));
			if(pauseStatus == 4) pauseStatus = 1;
			else pauseStatus <<= 1;
			if(pauseStatus == 1) ItemManager::INSTANCE()->addItem(ItemManager::P1, 0, new PauseMenuEvent(0, -0.45f, true));
			else if(pauseStatus == 2) ItemManager::INSTANCE()->addItem(ItemManager::P2, 0, new PauseMenuEvent(1, -0.55f, true));
			else ItemManager::INSTANCE()->addItem(ItemManager::P3, 0, new PauseMenuEvent(2, -0.65f, true));
		}else{
			if(pauseStatus & (1 << 3)) ItemManager::INSTANCE()->addItem(ItemManager::PNB, 0, new PauseMenu2EndEvent(3, -0.55f, true));
			else ItemManager::INSTANCE()->addItem(ItemManager::PYB, 0, new PauseMenu2EndEvent(4, -0.65f, true));
			if(pauseStatus & (1 << 3)) {
				pauseStatus &= ~(1 << 3);
				pauseStatus |= 1 << 4;
				ItemManager::INSTANCE()->addItem(ItemManager::PY, 0, new PauseMenu2Event(4, -0.65f, true));
			}else{
				pauseStatus &= ~(1 << 4);
				pauseStatus |= 1 << 3;
				ItemManager::INSTANCE()->addItem(ItemManager::PN, 0, new PauseMenu2Event(3, -0.55f, true));
			}
		}
		return;
	}
	status |= 8;
}

void OpenGLGame::endtLeft()
{
	status &= ~1;
	if(status & 2);
	else myPlaneOffset = 0;
}

void OpenGLGame::endRight()
{
	status &= ~2;
	if(status & 1);
	else myPlaneOffset = 0;
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
	if(pauseLock) return;
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
	if(pauseLock) {
		QSound::play(":/std/ok.wav");
		if(pauseStatus == 1) {
			GameWidget::Instance->quit();
		}else if(pauseStatus == 2) {
			pauseStatus |= 8;
			ItemManager::INSTANCE()->addItem(ItemManager::PS, 1, new PauseMenu2Event(1, -0.45f, false));
			ItemManager::INSTANCE()->addItem(ItemManager::PN, 1, new PauseMenu2Event(3, -0.55f, false));
			ItemManager::INSTANCE()->addItem(ItemManager::PYB, 1, new PauseMenu2EndEvent(4, -0.65f, false));

		}else if(pauseStatus == 4) {
			pauseStatus |= 8;
			ItemManager::INSTANCE()->addItem(ItemManager::PS, 1, new PauseMenu2Event(2, -0.45f, false));
			ItemManager::INSTANCE()->addItem(ItemManager::PN, 1, new PauseMenu2Event(3, -0.55f, false));
			ItemManager::INSTANCE()->addItem(ItemManager::PYB, 1, new PauseMenu2EndEvent(4, -0.65f, false));
		}else if(pauseStatus & 8) {
			pauseStatus &= ~24;
			if(pauseStatus == 1) ItemManager::INSTANCE()->addItem(ItemManager::P1, 1, new PauseMenuEvent(0, -0.45f, false));
			else ItemManager::INSTANCE()->addItem(ItemManager::P1B, 1, new PauseMenuEndEvent(0, -0.45f, false));
			if(pauseStatus == 2) ItemManager::INSTANCE()->addItem(ItemManager::P2, 1, new PauseMenuEvent(1, -0.55f, false));
			else ItemManager::INSTANCE()->addItem(ItemManager::P2B, 1, new PauseMenuEndEvent(1, -0.55f, false));
			if(pauseStatus == 4) ItemManager::INSTANCE()->addItem(ItemManager::P3, 1, new PauseMenuEvent(2, -0.65f, false));
			else ItemManager::INSTANCE()->addItem(ItemManager::P3B, 1, new PauseMenuEndEvent(2, -0.65f, false));
		}else if(pauseStatus & 16) {
			pauseStatus &= ~24;
			if(pauseStatus == 2) QTimer::singleShot(800, [&](){
					GameWidget::Instance->reset();
				});
			else {
				QTimer::singleShot(0, [&](){
					GameWidget::Instance->back();
				});
			}
		}
		return;
	}
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
	qDebug() << "init";
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
	bulletTime = 0;
	activeItems = false;
	ballRotate = 0;
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
	ItemManager::INSTANCE()->installRender(ItemManager::MY_BULLET, new MyBulletRender);
	ItemManager::INSTANCE()->installRender(ItemManager::POWER, new ItemRender(TextureManager::POWER));
	ItemManager::INSTANCE()->installRender(ItemManager::POINT, new ItemRender(TextureManager::POINT));
	ItemManager::INSTANCE()->installRender(ItemManager::EXTEND, new SmallItemRender(TextureManager::EXTEND));
	ItemManager::INSTANCE()->installRender(ItemManager::SPEXTEND, new SmallItemRender(TextureManager::SPEXTEND));
	ItemManager::INSTANCE()->installRender(ItemManager::EXTEND_TIP, new TipRender(TextureManager::EXTEND_TIP));
	ItemManager::INSTANCE()->installRender(ItemManager::SPEXTEND_TIP, new TipRender(TextureManager::SPEXTEND_TIP));
	ItemManager::INSTANCE()->installRender(ItemManager::ITEM_GET_LINE, new TipRender(TextureManager::ITEM_GET_LINE));
	ItemManager::INSTANCE()->installRender(ItemManager::W_PLUS, new WhiteNumberRender(TextureManager::W_PLUS));
	ItemManager::INSTANCE()->installRender(ItemManager::W0, new WhiteNumberRender(TextureManager::W0));
	ItemManager::INSTANCE()->installRender(ItemManager::W1, new WhiteNumberRender(TextureManager::W1));
	ItemManager::INSTANCE()->installRender(ItemManager::W2, new WhiteNumberRender(TextureManager::W2));
	ItemManager::INSTANCE()->installRender(ItemManager::W3, new WhiteNumberRender(TextureManager::W3));
	ItemManager::INSTANCE()->installRender(ItemManager::W4, new WhiteNumberRender(TextureManager::W4));
	ItemManager::INSTANCE()->installRender(ItemManager::W5, new WhiteNumberRender(TextureManager::W5));
	ItemManager::INSTANCE()->installRender(ItemManager::W6, new WhiteNumberRender(TextureManager::W6));
	ItemManager::INSTANCE()->installRender(ItemManager::W7, new WhiteNumberRender(TextureManager::W7));
	ItemManager::INSTANCE()->installRender(ItemManager::W8, new WhiteNumberRender(TextureManager::W8));
	ItemManager::INSTANCE()->installRender(ItemManager::W9, new WhiteNumberRender(TextureManager::W9));
	ItemManager::INSTANCE()->installRender(ItemManager::W9, new WhiteNumberRender(TextureManager::W9));
	ItemManager::INSTANCE()->installRender(ItemManager::P1, new PauseMenuRender(TextureManager::P1));
	ItemManager::INSTANCE()->installRender(ItemManager::P2, new PauseMenuRender(TextureManager::P2));
	ItemManager::INSTANCE()->installRender(ItemManager::P3, new PauseMenuRender(TextureManager::P3));
	ItemManager::INSTANCE()->installRender(ItemManager::P1B, new PauseMenuRender(TextureManager::P1B));
	ItemManager::INSTANCE()->installRender(ItemManager::P2B, new PauseMenuRender(TextureManager::P2B));
	ItemManager::INSTANCE()->installRender(ItemManager::P3B, new PauseMenuRender(TextureManager::P3B));
	ItemManager::INSTANCE()->installRender(ItemManager::P3B, new PauseMenuRender(TextureManager::P3B));
	ItemManager::INSTANCE()->installRender(ItemManager::PS, new PauseMenuRender(TextureManager::PS));
	ItemManager::INSTANCE()->installRender(ItemManager::PY, new PauseMenuRender(TextureManager::PY));
	ItemManager::INSTANCE()->installRender(ItemManager::PN, new PauseMenuRender(TextureManager::PN));
	ItemManager::INSTANCE()->installRender(ItemManager::PYB, new PauseMenuRender(TextureManager::PYB));
	ItemManager::INSTANCE()->installRender(ItemManager::PNB, new PauseMenuRender(TextureManager::PNB));

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
	ItemManager::INSTANCE()->addItem(ItemManager::ITEM_GET_LINE, 1, new ItemGetLineEvent);

	glEnable(GL_BLEND);
}

void OpenGLGame::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void OpenGLGame::paintGL()
{
	glClear( GL_COLOR_BUFFER_BIT);
	dealSomeThing();

	if(!pauseLock) {
		if(rand() % 10 == 0) ItemManager::INSTANCE()->addItem(ItemManager::POINT, 1, new PointEvent(sin(rand()), 1.0f));
		drawBackGround();
		drawMyPlane();
	}
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
	activeItems = myPlaneY > 0.6f;
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
	drawBullets();
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

void OpenGLGame::drawBullets()
{
	++bulletTime;
	if((status & 32) && bulletTime == 5) {
		MusicFactory::getInstance()->playFire();
		if(GameWidget::Instance->getLevel() == 0) {
			ItemManager::INSTANCE()->addItem(ItemManager::MY_BULLET, 1, new MyBulletEvent(myPlaneX, myPlaneY + 0.001f));
		}else if(GameWidget::Instance->getLevel() == 1) {
			ItemManager::INSTANCE()->addItem(ItemManager::MY_BULLET, 1, new MyBulletEvent(myPlaneX - 0.03f, myPlaneY + 0.001f));
			ItemManager::INSTANCE()->addItem(ItemManager::MY_BULLET, 1, new MyBulletEvent(myPlaneX + 0.03f, myPlaneY + 0.001f));
		}else if(GameWidget::Instance->getLevel() == 2) {
			ItemManager::INSTANCE()->addItem(ItemManager::MY_BULLET, 1, new MyBulletEvent(myPlaneX - 0.04f, myPlaneY + 0.001f));
			ItemManager::INSTANCE()->addItem(ItemManager::MY_BULLET, 1, new MyBulletEvent(myPlaneX + 0.04f, myPlaneY + 0.001f));
			ItemManager::INSTANCE()->addItem(ItemManager::MY_BULLET, 1, new MyBulletEvent(myPlaneX, myPlaneY + 0.001f));
		}else if(GameWidget::Instance->getLevel() == 3) {
			ItemManager::INSTANCE()->addItem(ItemManager::MY_BULLET, 1, new MyBulletEvent(myPlaneX - 0.02f, myPlaneY + 0.001f));
			ItemManager::INSTANCE()->addItem(ItemManager::MY_BULLET, 1, new MyBulletEvent(myPlaneX + 0.02f, myPlaneY + 0.001f));
			ItemManager::INSTANCE()->addItem(ItemManager::MY_BULLET, 1, new MyBulletEvent(myPlaneX - 0.06f, myPlaneY + 0.001f));
			ItemManager::INSTANCE()->addItem(ItemManager::MY_BULLET, 1, new MyBulletEvent(myPlaneX + 0.06f, myPlaneY + 0.001f));
		}else if(GameWidget::Instance->getLevel() == 4) {
			ItemManager::INSTANCE()->addItem(ItemManager::MY_BULLET, 1, new MyBulletEvent(myPlaneX - 0.04f, myPlaneY + 0.001f));
			ItemManager::INSTANCE()->addItem(ItemManager::MY_BULLET, 1, new MyBulletEvent(myPlaneX + 0.04f, myPlaneY + 0.001f));
			ItemManager::INSTANCE()->addItem(ItemManager::MY_BULLET, 1, new MyBulletEvent(myPlaneX - 0.08f, myPlaneY + 0.001f));
			ItemManager::INSTANCE()->addItem(ItemManager::MY_BULLET, 1, new MyBulletEvent(myPlaneX + 0.08f, myPlaneY + 0.001f));
			ItemManager::INSTANCE()->addItem(ItemManager::MY_BULLET, 1, new MyBulletEvent(myPlaneX, myPlaneY + 0.001f));
		}
	}
	if(bulletTime == 5) bulletTime = 0;
}

void OpenGLGame::dealSomeThing()
{
	if(pauseStatus < 0) ++pauseStatus;
}
