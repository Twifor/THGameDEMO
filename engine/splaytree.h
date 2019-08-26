#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include <QObject>
#include <QDebug>

template<typename T>
struct SplayNode {
	T value;
	SplayNode *ch[2], *fa;
	explicit SplayNode(T v) : value(v){
		ch[0] = ch[1] = fa = nullptr;
	}
};

template<typename T>
class SplayTree
{
private:
	SplayNode<T> *root;
	void DFS_delete(SplayNode<T> *p){
		if(p == nullptr) return;
		DFS_delete(p->ch[0]);
		delete p;
		DFS_delete(p->ch[1]);
	}
	void change(SplayNode<T> *f, SplayNode<T> *c, int w){
		if(f != nullptr) f->ch[w] = c;
		if(c != nullptr) c->fa = f;
	}
	int identify(SplayNode<T> *p){
		if(p != nullptr && p->fa != nullptr) return p->fa->ch[1] == p;
		return 0;
	}
	void rotate(SplayNode<T> *p){
		if(p == nullptr || p->fa == nullptr) return;
		SplayNode<T> *fa = p->fa;
		SplayNode<T> *g = fa->fa;
		int i = identify(p), j = identify(fa);
		change(fa, p->ch[i ^ 1], i);
		change(p, fa, i ^ 1);
		change(g, p, j);
	}
	SplayNode<T> *getMax(SplayNode<T> *p){
		while(p->ch[1] != nullptr) p = p->ch[1];
		return p;
	}
	SplayNode<T> *getMin(SplayNode<T> *p){
		while(p->ch[0] != nullptr) p = p->ch[0];
		return p;
	}
	int num;

public:
	explicit SplayTree(){
		root = nullptr;
		num = 0;
	}
	~SplayTree(){
		DFS_delete(root);
	}
	SplayNode<T> *getRoot(){
		return root;
	}
	void splay(SplayNode<T> *p, SplayNode<T> *pre = nullptr){
		while(p->fa != pre) {
			SplayNode<T> *fa = p->fa, *g = fa->fa;
			if(g != pre) {
				if(identify(p) == identify(fa)) rotate(fa);
				else rotate(p);
			}
			rotate(p);
		}
		if(pre == nullptr) root = p;
	}
	void insert(SplayNode<T> *p){
		++num;
		if(root == nullptr) {
			root = p;
		}else{
			SplayNode<T> *cur = root, *next = nullptr;
			for(;;) {
				if(p->value < cur->value) next = cur->ch[0];
				else next = cur->ch[1];
				if(next == nullptr) {
					if(p->value < cur->value) change(cur, p, 0);
					else change(cur, p, 1);
					break;
				}else cur = next;
			}
		}
		splay(p);
	}
	void del(SplayNode<T> *p){
		--num;
		splay(p);
		if(p->ch[0] == nullptr && p->ch[1] == nullptr) {
			root = nullptr;
		}else if(p->ch[0] == nullptr || p->ch[1] == nullptr) {
			if(p->ch[0] == nullptr) {
				p->ch[1]->fa = nullptr;
				root = p->ch[1];
			}else{
				p->ch[0]->fa = nullptr;
				root = p->ch[0];
			}
		}else{
			SplayNode<T> *pre = getMax(p->ch[0]);
			SplayNode<T> *next = getMin(p->ch[1]);
			splay(pre);
			splay(next, pre);
			next->ch[0] = nullptr;
		}
		delete p;
	}
	int size(){
		return num;
	}

signals:

public slots:


};

#endif // SPLAYTREE_H
