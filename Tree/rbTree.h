#ifndef RBTREE_H
#define RBTREE_H

#include "iostream"
#include "queue"

using std::cout;
using std::endl;
using std::queue;
using value_type = int;

enum color{
	red = 0, black = 1
};

struct rbNode
{
	rbNode *p, *left, *right;
	value_type val;
	color clr;
	rbNode(value_type x, color clr) :p(nullptr), left(nullptr), right(nullptr), val(x), clr(clr){}
	~rbNode(){
		if (left != nullptr)
			delete left;
		left = nullptr;
		if (right != nullptr)
			delete right;
		right = nullptr;
		p = nullptr;
	}
};

class rbTree 
{
public:
	~rbTree(){
		if (root != nullptr){
			delete root;
			root = nullptr;
		}
	}
	//中序遍历节点
	void rbTree_walk_in_order(rbNode *node){
		if (node == nullptr)
			return;
		rbTree_walk_in_order(node->left);
		cout << node->val << endl;
		rbTree_walk_in_order(node->right);
	}
	//根据关键字查找
	rbNode* rbTree_search(value_type k){
		rbNode *tmp = root;
		while (tmp != nullptr&&tmp->val != k){
			if (k > tmp->val)
				tmp = tmp->right;
			else
				tmp = tmp->left;
		}
		return tmp;
	}
	//查找最大元素节点
	rbNode* rbTree_maxmum(rbNode *x){
		while (x->right != nullptr)
			x = x->right;
		return x;
	}
	//查找最小元素节点
	rbNode* rbTree_minimum(rbNode *x){
		while (x->left != nullptr)
			x = x->left;
		return x;
	}
	//查找后继节点(假定为中序遍历方式)
	rbNode* rbTree_successor(rbNode *x){
		if (x->right != nullptr)
			return rbTree_minimum(x->right);
		auto y = x->p;
		while (y != nullptr&&y->right == x){
			y = y->p;
			x = x->p;
		}
		return y;
	}
	//查找前驱结点
	rbNode* rbTree_predecessor(rbNode *x){
		if (x->left != nullptr)
			return rbTree_maxmum(x->left);
		auto y = x->p;
		while (y != nullptr&&y->left == x){
			y = y->p;
			x = x->p;
		}
		return y;
	}

	//插入节点
	void rbTree_insert(value_type k){
		rbNode *tmp = root, *tmp_p = root;
		while (tmp != nullptr){						//tmp_p为插入节点位置
			tmp_p = tmp;
			if (tmp->val > k)
				tmp = tmp->left;
			else if (tmp->val < k)
				tmp = tmp->right;
		}//while
		//插入节点
		rbNode *newnode = new rbNode(k, red);
		if (tmp_p == nullptr)
			root = newnode;
		else{
			if (tmp_p->val > k)
				tmp_p->left = newnode;
			else
				tmp_p->right = newnode;
			newnode->p = tmp_p;
		}
		rbTree_insert_fixup(newnode);			//由于插入节点，可能破坏红黑树性质，要对红黑树进行调整
	}
	//插入节点后可能要对红黑树进行颜色和结构的调整
	void rbTree_insert_fixup(rbNode *z){
		//z为插入的节点，y为z的叔节点
		while (z->p != nullptr&&z->p->clr == red){					//如果z的父节点为红色，就需要一直调整
			//如果父节点为爷节点的左孩子
			if (z->p == z->p->p->left){
				rbNode *y = z->p->p->right;
				//情形1：叔节点为红色，将父节点、叔节点变为黑色，爷节点变为红色
				if (y->clr == red){
					z->p->clr = black;
					y->clr = black;
					z->p->p->clr = red;
					//由于爷节点称为红色，需要针对爷节点在进行调整
					z = z->p->p;
				}//if
				else{
					//情形2：叔节点为黑色，z为右孩子，需要把z调整为左孩子
					if (z == z->p->right){
						z = z->p;
						rbTree_leftRotate(z);
					}
					//情形3：叔节点为黑色，z为左孩子，把父节点变黑，爷节点变红对爷节点右转
					z->p->clr = black;
					z->p->p->clr = red;
					rbTree_rightRotate(z->p->p);
				}
			}//if
			//如果父节点为爷节点的右孩子
			else{
				rbNode *y = z->p->p->left;
				//情形1：叔节点为红色，将父节点、叔节点变为黑色，爷节点变为红色
				if (y->clr == red){
					z->p->clr = black;
					y->p->clr = black;
					z->p->p->clr = red;
					//由于爷节点称为红色，需要针对爷节点在进行调整
					z = z->p->p;
				}//if
				//情形2：叔节点为黑色，z为右孩子，需要把z调整为左孩子
				else if (z == z->p->left){
					z = z->p;
					rbTree_rightRotate(z);
				}
				//情形3：叔节点为黑色，z为左孩子，把父节点变黑，爷节点变红对爷节点右转
				z->p->clr = black;
				z->p->p->clr = red;
				rbTree_leftRotate(z->p->p);
			}
		}
		root->clr = black;
	}
	//左转
	void rbTree_leftRotate(rbNode *z){
		rbNode *x = z->right;
		rbTree_transplant(z, x);
		z->right = x->left;
		if (x->left != nullptr)
			x->left->p = z;
		x->left = z;
		z->p = x;
	}
	//右转
	void rbTree_rightRotate(rbNode *z){
		rbNode *x = z->left;
		rbTree_transplant(z, x);
		z->left = x->right;
		if (x->right != nullptr)
			x->right->p = z;
		x->right = z;
		z->p = x;
	}
	//删除节点
	void rbTree_delete(value_type k){
		//这里要删除z节点，用y节点替换，替换导致y节点的颜色被删除
		//如果y节点的颜色为黑色，会导致红黑树的性质被破坏
		//需要记录y节点的原始颜色，x节点为顶替y节点的点
		rbNode *z = rbTree_search(k);
		color y_original_color = z->clr;
		rbNode *x;
		//情况1：z没有左孩子，y和z实际是一个节点，x顶替z
		if (z->left == nullptr){
			x = z->right;
			rbTree_transplant(z, z->right);
		}
		//情况2：z没有右孩子，y和z实际是一个节点，x顶替z
		else if (z->right == nullptr){
			x = z->left;
			rbTree_transplant(z, z->left);
		}
		//情况3：z有左右孩子，y是z的后继节点，y顶替z，y的颜色变成z的颜色；x顶替y，但是x的颜色不改变，如果y的颜色是黑色，需要对x进行调整
		else{
			rbNode *y = rbTree_successor(z);		//y为z的后继节点
			y_original_color = y->clr;
			x = y->right;
			if (y->p != z){
				rbTree_transplant(y, x);
				y->right = z->right;
				if (z->right != nullptr)
					z->right->p = y;
			}
			rbTree_transplant(z, y);
			y->left = z->left;
			if (z->left != nullptr)
				z->left->p = y;
			y->clr = z->clr;
		}//else
		z->right = nullptr;
		z->left = nullptr;
		delete z;
		if (y_original_color == black)
			rbTree_delete_fixup(x);
	}

	//对删除节点后的红黑书进行调整
	void rbTree_delete_fixup(rbNode *z){
		//停止调整的条件
		while (z != root&&z->clr == black){
			//如果z为左孩子
			if (z == z->p->left){
				//w是z的兄弟节点
				rbNode *w = z->p->right;
				//情况1：w为红色，对z->p左转，将w调整为黑色，转到情况2、3、4
				if (w != nullptr&&w->clr == red){
					rbTree_leftRotate(z->p);
					w->clr = black;
					z->p->clr = red;
					w = z->p->right;
				}
				else{
					//情况2：w为黑色，并且左右孩子均为黑色，将w变红，z=z->p，递归到上层树解决
					if (w->left != nullptr&&w->left->clr == black&&w->right != nullptr&&w->right->clr == black){
						w->clr = red;
						z = z->p;
					}
					//情况3：w为黑色，并且左孩子为红色，右孩子为黑色，对w右转，调整颜色
					else if (w->left != nullptr&&w->left->clr == red&&w->right != nullptr&&w->right->clr == black){
						rbTree_rightRotate(w);
						w->p->clr = black;
						w->clr = red;
						w = w->p;
					}
					//情况4：w为黑色，右孩子为红色，z->p左转
					else if (w->right != nullptr&&w->right->clr == red){
						rbTree_leftRotate(z->p);
						z->p->clr = black;
						w->clr = red;
						w->right->clr = black;
						z = root;
					}
				}
			}//if
			else{
				//w是z的兄弟节点
				rbNode *w = z->p->right;
				//情况1：w为红色，对z->p左转，将w调整为黑色，转到情况2、3、4
				if (w != nullptr&&w->clr == red){
					rbTree_rightRotate(z->p);
					w->clr = black;
					z->p->clr = red;
					w = z->p->right;
				}
				else{
					if (w->right != nullptr&&w->right->clr == black&&w->left != nullptr&&w->left->clr == black){
						w->clr = red;
						z = z->p;
					}
					else if (w->right != nullptr&&w->right->clr == red&&w->left != nullptr&&w->left->clr == black){
						rbTree_leftRotate(w);
						w->p->clr = black;
						w->clr = red;
						w = w->p;
					}
					else if (w->left != nullptr&&w->left->clr == red){
						rbTree_rightRotate(z->p);
						z->p->clr = black;
						w->clr = red;
						w->left->clr = black;
						z = root;
					}
				}
			}//else
		}//while
		z->clr = black;
	}

	//使用target子树，替换src子树，只是替换src与src->p的关系
	void rbTree_transplant(rbNode* src, rbNode* target){
		if (src->p == nullptr)
			root = target;
		else if (src == src->p->left)
			src->p->left = target;
		else
			src->p->right = target;
		if (target != nullptr)
			target->p = src->p;
	}
	//广度优先搜索遍历红黑书
	void rbTree_walkTree(rbNode *node){
		queue<rbNode*> que;
		if (node != nullptr)
			que.push(node);
		while (!que.empty()){
			rbNode *ptr = que.front();
			cout << ptr->val << " " << ptr->clr << endl;
			que.pop();
			if (ptr->left != nullptr)
				que.push(ptr->left);
			if (ptr->right != nullptr)
				que.push(ptr->right);
		}
	}
	rbNode *root = nullptr;
};

#endif