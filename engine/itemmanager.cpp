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

	ShaderManager::INSTANCE()->setProgram(0, "#version 330 core\n"
										  "layout (location = 0) in vec3 aPos;\n"
										  "layout (location = 1) in vec2 aTexCoord;\n"
										  "out vec2 TexCoord;\n"
										  "uniform float div;\n"
										  "void main()\n"
										  "{\n"
										  "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
										  "gl_Position.y = gl_Position.y * div;\n"
										  "TexCoord = aTexCoord;\n"
										  "}",
										  "#version 330 core\n"
										  "out vec4 FragColor;\n"
										  "in vec2 TexCoord;\n"
										  "uniform sampler2D mainTexture;"
										  ""
										  "void main()\n"
										  "{\n"
										  "FragColor = texture(mainTexture, TexCoord);\n"
										  "}");
	ShaderManager::INSTANCE()->setProgram(1, "#version 330 core\n"
										  "layout (location = 0) in vec3 aPos;\n"
										  "layout (location = 1) in vec2 aTexCoord;\n"
										  "layout (location = 2) in vec3 rotate;\n"
										  "uniform float div;\n"
										  "\n"
										  "out vec2 TexCoord;\n"
										  "\n"
										  "mat4 getRotateMatrix(){\n"
										  "mat4 t = mat4(vec4(1.0,0.0,0.0,0.0),vec4(0.0,1.0,0.0,0.0),vec4(0.0,0.0,1.0,0.0),vec4(rotate.x,rotate.y,0.0,1.0));\n"
										  "mat4 r = mat4(vec4(cos(rotate.z),sin(rotate.z),0.0,0.0),vec4(-sin(rotate.z),cos(rotate.z),0.0,0.0),vec4(0.0,0.0,1.0,0.0),vec4(0.0,0.0,0.0,1.0));\n"
										  "return t * r;\n"
										  "}\n"
										  "\n"
										  "void main(){\n"
										  "gl_Position = getRotateMatrix()*vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
										  "gl_Position.y = gl_Position.y * div;\n"
										  "TexCoord = aTexCoord;\n"
										  "}\n",
										  "#version 330 core\n"
										  "out vec4 FragColor;\n"
										  "in vec2 TexCoord;\n"
										  "uniform sampler2D mainTexture;"
										  ""
										  "void main()\n"
										  "{\n"
										  "FragColor = texture(mainTexture, TexCoord);\n"
										  "}");
	ShaderManager::INSTANCE()->setProgram(2, "#version 330 core\n"
										  "layout (location = 0) in vec3 aPos;\n"
										  "layout (location = 1) in vec2 aTexCoord;\n"
										  "out vec2 TexCoord;\n"
										  "out vec2 fog;\n"
										  "uniform mat4 projection;\n"
										  "void main()\n"
										  "{\n"
										  "gl_Position = projection * vec4(aPos, 1.0);\n"
										  "fog.x=gl_Position.z/25.0;\n"
										  "fog.y=0;\n"
										  "TexCoord = aTexCoord;\n"
										  "}", "#version 330 core\n"
										  "out vec4 FragColor;\n"
										  "in vec2 TexCoord;\n"
										  "in vec2 fog;\n"
										  "uniform sampler2D mainTexture;\n"
										  "uniform vec2 alpha;\n"

										  "void main()\n"
										  "{\n"
										  "FragColor = texture(mainTexture, TexCoord);\n"
										  "FragColor = mix(FragColor,vec4(0.0,0.0,0.0,0.0),fog.x)*alpha.x;\n"
										  "}");
	ShaderManager::INSTANCE()->setProgram(3, "#version 330 core\n"
										  "layout (location = 0) in vec3 aPos;\n"
										  "layout (location = 1) in vec2 aTexCoord;\n"
										  "out vec2 TexCoord;\n"
										  "out vec2 lock;\n"
										  "uniform mat4 matrix;\n"
										  "uniform vec2 limit;\n"
										  "void main()\n"
										  "{\n"
										  "gl_Position = matrix * vec4(aPos, 1.0);\n"
										  "lock.x=gl_Position.y-limit.y;\n"
										  "TexCoord = aTexCoord;\n"
										  "}", "#version 330 core\n"
										  "out vec4 FragColor;\n"
										  "in vec2 TexCoord;\n"
										  "in vec2 fog;\n"
										  "in vec2 lock;\n"

										  "uniform sampler2D mainTexture;\n"
										  "uniform vec2 alpha;\n"

										  "void main()\n"
										  "{\n"
										  "FragColor = texture(mainTexture, TexCoord);\n"
										  "FragColor.a *= alpha.x;\n"
										  "if(lock.x<0.0)FragColor.a = 0;\n"
										  "}");

	ShaderManager::INSTANCE()->getProgram(0)->setUniformValue("mainTexture", 0);
	ShaderManager::INSTANCE()->getProgram(1)->setUniformValue("mainTexture", 0);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("mainTexture", 0);
	ShaderManager::INSTANCE()->getProgram(3)->setUniformValue("mainTexture", 0);

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
}

ItemManager *ItemManager::in = nullptr;

ItemManager::~ItemManager()
{
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
	newQue.push_back(data);
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
	if(rt->value.event->update(render[rt->value.type])) que.push_back(rt);
	DFS(rt->ch[1]);
}

void ItemManager::dealWithDel()
{
	while (!que.empty()) {
		SplayNode<TreeData> *nd = que.front();
		que.pop_front();
		delete nd->value.event;
		mainSplayTree[nd->value.type]->del(nd);
		if(mainSplayTree[nd->value.type]->size() == 0) {
			typeSplayTree->del(to[nd->value.type]);
			--numOfType;
			to[nd->value.type] = nullptr;
		}
		--num;
	}
}

void ItemManager::dealWithNew()
{
	while(!newQue.empty()) {
		UserData data = newQue.front();
		newQue.pop_back();
		addItem(data.tp, data.depth, data.event);
	}
}

