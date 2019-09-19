#include "gameresource.h"
#include <QDebug>

GameResource* GameResource::INSTANCE = nullptr;

void GameResource::destroy()
{
	if(INSTANCE != nullptr) delete INSTANCE;
}

void GameResource::load(ResourceID id)
{
	int rk = static_cast<int>(id);
	if(data[rk].data == nullptr) {
		if(rk >= 31 && rk <= 34) {//四首bgm
			for(int i = 31; i <= 34; i++) pop(static_cast<ResourceID>(i)); //只保留一首，减小内存占用
		}
		indexs->device()->seek(0);
		indexs->skipRawData(rk << 4);//跳过rk*16个字节
		qint64 x, y;
		*indexs >> x >> y;//读取开始位置以及长度
		ress->device()->seek(0);
		ress->skipRawData(x);
		data[rk].data = new char[y];
		ress->readRawData(data[rk].data, y);//读y个字节的数据
		data[rk].l = y;
	}
}

void GameResource::pop(ResourceID id)
{
	int rk = static_cast<int>(id);
	if(data[rk].data != nullptr) {
		delete data[rk].data;
		data[rk].l = 0;
		data[rk].data = nullptr;
	}
}

GameResourceData *GameResource::getData(ResourceID id)
{
	int rk = static_cast<int>(id);
	return (data[rk].data == nullptr ? nullptr : data + rk);
}

void GameResource::changeStatus(GameResource::ResourceStatus status)
{
	this->status = status;
	update();
}

GameResource *GameResource::getInstance()
{
	if(INSTANCE == nullptr) return INSTANCE = new GameResource;
	return INSTANCE;
}

GameResource::~GameResource()
{
	for(int i = 0; i < RESOURCE_NUM; i++) pop(static_cast<ResourceID>(i));
	delete res;
	delete index;
	delete ress;
	delete indexs;
}

GameResource::GameResource() : QObject (nullptr)
{
	status = MENU;
	for(int i = 0; i < RESOURCE_NUM; i++) {
		data[i].data = nullptr;
		data[i].l = 0;
	}
	res = new QFile("res.dat");
	res->open(QIODevice::ReadOnly);
	index = new QFile("index.dat");
	index->open(QIODevice::ReadOnly);
	ress = new QDataStream(res);
	indexs = new QDataStream(index);

	update();
}

void GameResource::update()//这里需要修改
{
	if(status == MENU) {
		for(int i = 0; i < RESOURCE_NUM; i++) {//遍历所有资源
			if(i <= 31) load(static_cast<ResourceID>(i));
			else pop(static_cast<ResourceID>(i));
		}
		for(int i = 35; i < 47; i++) load(static_cast<ResourceID>(i)); //之后的资源(历史遗留问题)
	}else if(status == GAME) {
		for(int i = 0; i < RESOURCE_NUM; i++) {
			if(i <= 31 || (i >= 35 && i < 47)) pop(static_cast<ResourceID>(i)); //释放之前的资源
			else if(i >= 47 && i <= 140) load(static_cast<ResourceID>(i)); //加载游戏所需的资源文件
		}
		load(static_cast<ResourceID>(1));
	}
}

char *GameResourceData::getRawData()
{
	return data;
}

qint64 GameResourceData::getLength()
{
	return l;
}

GameResourceData::GameResourceData()
{
	data = nullptr;
}

void GameResourceWAVData::loadData(QBuffer *buffer)
{
	buffer->setData(QByteArray(getRawData(), getLength()));
	buffer->open(QIODevice::ReadOnly);
	buffer->seek(0);
}

void GameResourcePNGData::loadData(QImage &image)
{
	image.loadFromData(QByteArray(getRawData(), getLength()));
}
