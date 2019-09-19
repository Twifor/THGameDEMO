#include "itemmanager.h"
#include "shadermanager.h"
#include "texturemanager.h"
#include "gameresource.h"

ItemManager::ItemManager(QObject *parent) : QObject(parent)
{
	for(int i = 0; i < ITEM_NUMBER; i++) {
		mainSplayTree[i] = new SplayTree<TreeData>();
		to[i] = nullptr;
	}
	typeSplayTree = new SplayTree<ItemType>();
	num = 0;
	numOfType = 0;

	ShaderManager::INSTANCE()->setProgram(0, ":/opengl/v0.vert", ":/opengl/f0.frag");
	ShaderManager::INSTANCE()->setProgram(1, ":/opengl/v1.vert", ":/opengl/f1.frag");
	ShaderManager::INSTANCE()->setProgram(2, ":/opengl/v2.vert", ":/opengl/f2.frag");
	ShaderManager::INSTANCE()->setProgram(3, ":/opengl/v3.vert", ":/opengl/f3.frag");
	ShaderManager::INSTANCE()->setProgram(4, ":/opengl/v4.vert", ":/opengl/f4.frag");
	ShaderManager::INSTANCE()->setProgram(5, ":/opengl/v5.vert", ":/opengl/f5.frag");

	ShaderManager::INSTANCE()->getProgram(0)->setUniformValue("mainTexture", 0);
	ShaderManager::INSTANCE()->getProgram(1)->setUniformValue("mainTexture", 0);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("mainTexture", 0);
	ShaderManager::INSTANCE()->getProgram(3)->setUniformValue("mainTexture", 0);
	ShaderManager::INSTANCE()->getProgram(4)->setUniformValue("mainTexture", 0);
	ShaderManager::INSTANCE()->getProgram(5)->setUniformValue("mainTexture", 0);

	QImage image;
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MAIN_GAME_BG_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::BACKGROUND, new QOpenGLTexture(image));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MAIN_GAME_STAR_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::STAR_BACKGROUND, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA1_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA1, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA2_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA2, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA3_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA3, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA4_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA4, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA5_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA5, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA6_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA6, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA7_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA7, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA8_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA8, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(TREE_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::TREE, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA9_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA9, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA10_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA10, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA11_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA11, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA12_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA12, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA13_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA13, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA14_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA14, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA15_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA15, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA16_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA16, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA17_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA17, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA18_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA18, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA19_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA19, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA20_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA20, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA21_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA21, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA22_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA22, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA23_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA23, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA24_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA24, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(SLOWEFFECT1_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::SLOWEFFECT1_PNG, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(SLOWEFFECT2_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::SLOWEFFECT2_PNG, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(CENTER_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::CENTER, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA_BALL_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::BALL, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA_LINE_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MARISA_LINE, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MARISA_BULLET_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MY_BULLET, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(POWER_ITEM_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::POWER, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(POINT_ITEM_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::POINT, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(UP_ITEM_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::EXTEND, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(SPELL_ITEM_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::SPEXTEND, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(EXTEND_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::EXTEND_TIP, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(SPELL_EXTEND_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::SPEXTEND_TIP, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(ITEM_GET_LINE_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::ITEM_GET_LINE, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(W_PLUS_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::W_PLUS, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(W_0_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::W0, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(W_1_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::W1, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(W_2_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::W2, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(W_3_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::W3, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(W_4_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::W4, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(W_5_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::W5, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(W_6_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::W6, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(W_7_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::W7, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(W_8_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::W8, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(W_9_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::W9, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(PAUSE_CONTINUE_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::P1, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(PAUSE_GIVEUP_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::P2, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(PAUSE_RETURN_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::P3, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(PAUSE_CONTINUE_B_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::P1B, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(PAUSE_GIVEUP_B_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::P2B, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(PAUSE_RETURN_B_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::P3B, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(PAUSE_SURE_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::PS, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(PAUSE_YES_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::PY, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(PAUSE_NO_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::PN, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(PAUSE_YES_B_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::PYB, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(PAUSE_NO_B_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::PNB, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(P2_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::SPP, new QOpenGLTexture(image.mirrored(false, true)));
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(MASTER_PNG))->loadData(image);
	TextureManager::INSTANCE()->setTexture(TextureManager::MASTER, new QOpenGLTexture(image.mirrored(false, true)));

}

ItemManager *ItemManager::in = nullptr;

ItemManager::~ItemManager()//别忘了删event
{
	ShaderManager::INSTANCE()->destory();
	TextureManager::INSTANCE()->destroy();
	DFS_delete1(typeSplayTree->getRoot());
	for(int i = 0; i < ITEM_NUMBER; i++) delete mainSplayTree[i];
	for(int i = 0; i < ITEM_NUMBER; i++) delete render[i];
	delete typeSplayTree;
}

void ItemManager::addItem(ItemType type, int depth, ItemDataEventBase *base)
{
	++num;
	SplayNode<TreeData> *newNode = new SplayNode<TreeData>(TreeData(type, depth, base));
	if(mainSplayTree[type]->size() == 0) {
		typeSplayTree->insert(to[type] = new SplayNode<ItemType>(type));
		++numOfType;
	}
	mainSplayTree[type]->insert(newNode);
}

void ItemManager::addNewItem(ItemManager::ItemType type, int depth, ItemDataEventBase *base)
{
	UserData data;
	data.tp = type;
	data.depth = depth;
	data.event = base;
	newQue.enqueue(data);
}

void ItemManager::update()
{
	DFS(typeSplayTree->getRoot());
	dealWithDel();
	dealWithNew();
}

int ItemManager::size()
{
	return num;
}

int ItemManager::getNumOfType()
{
	return numOfType;
}

void ItemManager::installRender(ItemManager::ItemType tp, RenderBase *rd)
{
	render[tp] = rd;
}

ItemManager *ItemManager::INSTANCE()
{
	if(in == nullptr) in = new ItemManager;
	return in;
}

void ItemManager::destroy()
{
	delete in;
	in = nullptr;
}

void ItemManager::setDiv(float d)
{
	div = d;
}

float ItemManager::getDiv()
{
	return div;
}

void ItemManager::DFS(SplayNode<ItemManager::ItemType> *rt)
{
	if(rt == nullptr) return;
	DFS(rt->ch[0]);
	render[rt->value]->init();
	DFS(mainSplayTree[rt->value]->getRoot());
	render[rt->value]->render();
	DFS(rt->ch[1]);
}

void ItemManager::DFS(SplayNode<TreeData> *rt)
{
	if(rt == nullptr) return;
	DFS(rt->ch[0]);
	if(rt->value.event->update(render[rt->value.type])) que.enqueue(rt);
	DFS(rt->ch[1]);
}

void ItemManager::dealWithDel()
{
	while (!que.empty()) {
		SplayNode<TreeData> *nd = que.dequeue();
		if (mainSplayTree[nd->value.type]->size() == 1) {
			typeSplayTree->del(to[nd->value.type]);
			--numOfType;
			to[nd->value.type] = nullptr;
		}
		delete nd->value.event;
		mainSplayTree[nd->value.type]->del(nd);
		--num;
	}
}


void ItemManager::dealWithNew()
{
	while(!newQue.empty()) {
		UserData data = newQue.dequeue();
		addItem(data.tp, data.depth, data.event);
	}
}

void ItemManager::DFS_delete1(SplayNode<ItemManager::ItemType> *rt)
{
	if (rt == nullptr) return;
	DFS_delete1(rt->ch[0]);
	DFS_delete2(mainSplayTree[rt->value]->getRoot());
	DFS_delete1(rt->ch[1]);
}

void ItemManager::DFS_delete2(SplayNode<TreeData> *rt)
{
	if (rt == nullptr) return;
	DFS_delete2(rt->ch[0]);
	delete rt->value.event;
	DFS_delete2(rt->ch[1]);
}

