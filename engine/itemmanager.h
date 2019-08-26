#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H

#include <QObject>
#include "splaytree.h"
#include "itemdataevent.h"
#include <QQueue>
#include "renderbase.h"

#define ITEM_NUMBER 5

struct ItemData;
struct TreeData;
struct UserData;
class ItemManager : public QObject
{
	Q_OBJECT
public:
	~ItemManager();
	enum ItemType {
		BACKGROUND,
		STAR_BACKGROUND,
		TREE,
		TREE2,
		MARISA,
	};
	void addItem(ItemType type, int depth, ItemDataEventBase *base);
	void addNewItem(ItemType type, int depth, ItemDataEventBase *base);
	void update();
	int size();
	int getNumOfType();
	void installRender(ItemType tp, RenderBase *rd);
	static ItemManager* INSTANCE();
	void destroy();
	void setDiv(float d);
	float getDiv();

signals:

public slots:
protected:
	void DFS(SplayNode<ItemType> *rt);
	void DFS(SplayNode<TreeData> *rt);
	void dealWithDel();
	void dealWithNew();

private:
	explicit ItemManager(QObject *parent = nullptr);
	static ItemManager *in;
	SplayTree<TreeData> *mainSplayTree[ITEM_NUMBER];
	SplayTree<ItemType> *typeSplayTree;
	SplayNode<ItemType> *to[ITEM_NUMBER];
	RenderBase *render[ITEM_NUMBER];

	QQueue<SplayNode<TreeData>*> que;
	QQueue<UserData> newQue;
	float div;
	int num, numOfType;
};

struct TreeData {
	ItemManager::ItemType type;
	int depth;
	ItemDataEventBase *event;
	TreeData(ItemManager::ItemType p, int dp, ItemDataEventBase *b) : type(p), depth(dp), event(b){
	}
	bool operator<(const TreeData&s){
		return depth < s.depth;
	}
};

struct UserData {
	ItemManager::ItemType tp;
	int depth;
	ItemDataEventBase *event;
};

#endif // ITEMMANAGER_H
