#ifndef GAMERESOURCE_H
#define GAMERESOURCE_H

#include <QObject>
#include <QBuffer>
#include <QImage>
#include <QFile>

const int RESOURCE_NUM = 141;
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
	BGM4_WAV,//35
	TITLE_PNG,//36，标题图
	BGMVOLUME_PNG,//37
	NUM0_PNG,//38
	NUM1_PNG,//39
	NUM2_PNG,//40
	NUM3_PNG,//41
	NUM4_PNG,//42
	NUM5_PNG,//43
	NUM6_PNG,//44
	NUM7_PNG,//45
	NUM8_PNG,//46
	NUM9_PNG,//47
	GAME_BG_PNG,//48
	PLAYER_PNG,//49
	SPELLCARD_PNG,//50
	REDSTAR_PNG,//51
	BLUESTAR_PNG,//52
	LINGLI_PNG,//53
	DEDIAN_PNG,//54
	GRAZE_PNG,//55
	RED0_PNG,//56
	RED1_PNG,//57
	RED2_PNG,//58
	RED3_PNG,//59
	RED4_PNG,//60
	RED5_PNG,//61
	RED6_PNG,//62
	RED7_PNG,//62
	RED8_PNG,//64
	RED9_PNG,//65
	REDPOINT_PNG,//66
	REDXIE_PNG,//67
	BLUE0_PNG,//68
	BLUE1_PNG,//69
	BLUE2_PNG,//70
	BLUE3_PNG,//71
	BLUE4_PNG,//72
	BLUE5_PNG,//73
	BLUE6_PNG,//74
	BLUE7_PNG,//75
	BLUE8_PNG,//76
	BLUE9_PNG,//77
	MAIN_GAME_BG_PNG,//78，主游戏背景图片
	TREE_PNG,//79
	MAIN_GAME_STAR_PNG,//80
	MARISA1_PNG,//81
	MARISA2_PNG,//82
	MARISA3_PNG,//83
	MARISA4_PNG,//84
	MARISA5_PNG,//85
	MARISA6_PNG,//86
	MARISA7_PNG,//87
	MARISA8_PNG,//88
	MARISA9_PNG,//89
	MARISA10_PNG,//90
	MARISA11_PNG,//91
	MARISA12_PNG,//92
	MARISA13_PNG,//93
	MARISA14_PNG,//94
	MARISA15_PNG,//95
	MARISA16_PNG,//96
	MARISA17_PNG,//97
	MARISA18_PNG,//98
	MARISA19_PNG,//99
	MARISA20_PNG,//100
	MARISA21_PNG,//101
	MARISA22_PNG,//102
	MARISA23_PNG,//103
	MARISA24_PNG,//104
	SLOWEFFECT1_PNG,//105
	SLOWEFFECT2_PNG,//106
	CENTER_PNG,//107判定点贴图
	MARISA_BULLET_PNG,//108
	MARISA_BALL_PNG,//109
	MARISA_LINE_PNG,//110
	POWER_ITEM_PNG,//111power贴图
	POINT_ITEM_PNG,//112
	UP_ITEM_PNG,//113
	SPELL_ITEM_PNG,//114
	SCORE_PNG,//115
	SPELL_EXTEND_PNG,//116
	EXTEND_PNG,//117
	ITEM_GET_LINE_PNG,//118
	W_PLUS_PNG,//119
	W_0_PNG,//120
	W_1_PNG,//121
	W_2_PNG,//122
	W_3_PNG,//123
	W_4_PNG,//124
	W_5_PNG,//125
	W_6_PNG,//126
	W_7_PNG,//127
	W_8_PNG,//128
	W_9_PNG,//129
	PAUSE_CONTINUE_PNG,//130
	PAUSE_GIVEUP_PNG,//131
	PAUSE_RETURN_PNG,//132
	PAUSE_CONTINUE_B_PNG,//133
	PAUSE_GIVEUP_B_PNG,//134
	PAUSE_RETURN_B_PNG,//135
	PAUSE_SURE_PNG,//136
	PAUSE_YES_PNG,//137
	PAUSE_NO_PNG,//138
	PAUSE_YES_B_PNG,//139
	PAUSE_NO_B_PNG,//140
	MASTER_PNG//141
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
		MENU,
		GAME
	};
	void destroy();//销毁单例，清除所有游戏资源
	void load(ResourceID id);//加载一个特定的资源，通常指bgm
	void pop(ResourceID id);//注销一个特定的资源
	GameResourceData *getData(ResourceID id);//获得资源数据
	void changeStatus(ResourceStatus status);//修改资源状态机状态
	static GameResource *getInstance();//获得单例对象指针
	~GameResource();
signals:
	void changeStatusSuccess();//状态修改成功[未使用]
	void loadSuccess();//加载游戏资源成功[未使用]

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
