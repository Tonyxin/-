#ifndef BST_H
#define BST_H

#include "iostream"

using std::cout;
using std::endl;

/*
问题描述：
实现一个二叉搜索树的增删改查操作
*/
using value_type = int;

struct BSTnode{
	BSTnode *p, *left, *right;
	value_type val;
	BSTnode(value_type x) :p(nullptr), left(nullptr), right(nullptr), val(x){}
	~BSTnode(){
		if (left != nullptr)
			delete left;
		left = nullptr;
		if (right != nullptr)
			delete right;
		right = nullptr;
		p = nullptr;
	}
};

class BSTree {
public:
	~BSTree(){
		if (root != nullptr){
			delete root;
			root = nullptr;
		}
	}
	//中序遍历节点
	void BST_walk_in_order(BSTnode *node){
		if (node == nullptr)
			return;
		BST_walk_in_order(node->left);
		cout << node->val << endl;
		BST_walk_in_order(node->right);
	}
	//根据关键字查找
	BSTnode* BST_search(value_type k){
		BSTnode *tmp = root;
		while (tmp != nullptr&&tmp->val != k){
			if (k > tmp->val)
				tmp = tmp->right;
			else
				tmp = tmp->left;
		}
		return tmp;
	}
	//查找最大元素节点
	BSTnode* BST_maxmum(BSTnode *x){
		while (x->right != nullptr)
			x = x->right;
		return x;
	}
	//查找最小元素节点
	BSTnode* BST_minimum(BSTnode *x){
		while (x->left != nullptr)
			x = x->left;
		return x;
	}
	//查找后继节点(假定为中序遍历方式)
	BSTnode* BST_successor(BSTnode *x){
		if (x->right != nullptr)
			return BST_minimum(x->right);
		auto y = x->p;
		while (y != nullptr&&y->right == x){
			y = y->p;
			x = x->p;
		}
		return y;
	}
	//查找前驱结点
	BSTnode* BST_predecessor(BSTnode *x){
		if (x->left != nullptr)
			return BST_maxmum(x->left);
		auto y = x->p;
		while (y != nullptr&&y->left == x){
			y = y->p;
			x = x->p;
		}
		return y;
	}
	//插入节点
	void BST_insert(value_type val){
		auto x = root, y = root;
		while (x != nullptr){
			y = x;
			if (val < x->val)
				x = x->left;
			else
				x = x->right;
		}
		BSTnode *z = new BSTnode(val);
		z->p = y;
		if (y == nullptr)
			root = z;
		else if (z->val < y->val)
			y->left = z;
		else
			y->right = z;
	}
	//删除节点
	void BST_delete(BSTnode *z){
		if (z->left == nullptr)
			BST_transplant(z, z->right);
		else if (z->right == nullptr)
			BST_transplant(z, z->left);
		else{
			auto x = BST_successor(z);		//x是z的后继节点
			//后继节点是x的右孩子
			if (x->p == z){
				BST_transplant(z, x);
				x->left = z->left;
			}
			else{
				//如果后继节点有右孩子，线将x->right移动到x
				//注意移动只处理了与父节点的关系，并没有处理与孩子节点的关系
				if (x->right != nullptr){
					BST_transplant(x, x->right);
					x->right = nullptr;
				}
				//再将x移动到z
				BST_transplant(z, x);
				x->left = z->left;
				x->right = z->right;
				z->left->p = x;
				z->right->p = x;
			}
		}
		z->left = nullptr;
		z->right = nullptr;
		delete z;
		z = nullptr;
	}
	//使用target子树，替换src子树，只是替换src与src->p的关系
	void BST_transplant(BSTnode* src, BSTnode* target){
		if (src->p == nullptr)
			root = target;
		else if (src == src->p->left)
			src->p->left = target;
		else
			src->p->right = target;
		if (target!=nullptr)
			target->p = src->p;
	}
	BSTnode *root=nullptr;
};

#endif