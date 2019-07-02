#ifndef GAMERESOURCE_H
#define GAMERESOURCE_H

#include <QObject>
#include <QBuffer>
#include <QImage>
#include <QFile>

const int RESOURCE_NUM = 35;
enum ResourceID {//游戏资源ID，所有游戏资源必须在这里进行注册才能够使用
	P_PNG,//粒子,1
	P2_PNG,//另一个粒子,2
	GAMESTART_PNG,//gamestart菜单选项,3
	GAMESTART_B_PNG,//4
	MUSICROOM_PNG,//musicroom,5
	MUSICROOM_B_PNG,//6
	CONFIG_PNG,//config,7
	CONFIG_B_PNG,//8
	QUIT_PNG,//quit,9
	QUIT_B_PNG,//10
	MAGIC2_PNG,//标题界面魔法阵,11
	STARTN_PNG,//鵺立绘剪影,12
	STARTK_PNG,//恋,13
	STARTF_PNG,//芙兰,14
	MUSICROOM_BG_PNG,//musicroom背景,15
	E1_PNG,//第一首bgm图片,16
	E1B_PNG,//17
	E2_PNG,//18
	E2B_PNG,//19
	E3_PNG,//20
	E3B_PNG,//21
	E4_PNG,//22
	E4B_PNG,//23
	LABA_PNG,//喇叭,24
	LABA2_PNG,//25
	LABA3_PNG,//26
	MAGIC_PNG,//musicroom魔法阵,27
	SONG1_PNG,//四首bgm的评论,28
	SONG2_PNG,//29
	SONG3_PNG,//30
	SONG4_PNG,//31
	BGM1_WAV,//四首bgm,32,bgm类资源每一次只能加载一首，不允许加载多首
	BGM2_WAV,//33
	BGM3_WAV,//34
	BGM4_WAV//35
};

class GameResourceData {//游戏资源数据基类
	friend class GameResource;
private:
	char *data;
	qint64 l;
protected:
	char *getRawData();
	qint64 getLength();
public:
	explicit GameResourceData();
};

class GameResourceWAVData : public GameResourceData {//wav数据
public:
	void loadData(QBuffer *buffer);//装载数据
};

class GameResourcePNGData : public GameResourceData {//png数据
public:
	void loadData(QImage &image);
};

class GameResource : public QObject//单例模式，统一管理游戏资源
{
	Q_OBJECT
public:
	enum ResourceStatus {//状态控制加载资源的广度
		MENU
	};
	void destroy();//销毁单例，清除所有游戏资源
	void load(ResourceID id);//加载一个特定的资源，通常指bgm
	void pop(ResourceID id);//注销一个特定的资源
	GameResourceData *getData(ResourceID id);//获得资源数据
	void changeStatus(ResourceStatus status);//修改资源状态机状态
	static GameResource *getInstance();//获得单例对象指针
	~GameResource();
signals:
	void changeStatusSuccess();//状态修改成功
	void loadSuccess();//加载游戏资源成功

public slots:

private:
	explicit GameResource();
	static GameResource *INSTANCE;
	ResourceStatus status;
	GameResourceData data[RESOURCE_NUM];
	QFile *res, *index;
	QDataStream *ress, *indexs;
	void update();//根据状态更新资源加载
};

#endif // GAMERESOURCE_H
