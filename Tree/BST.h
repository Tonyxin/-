#ifndef BST_H
#define BST_H

#include "iostream"

using std::cout;
using std::endl;

/*
����������
ʵ��һ����������������ɾ�Ĳ����
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
	//��������ڵ�
	void BST_walk_in_order(BSTnode *node){
		if (node == nullptr)
			return;
		BST_walk_in_order(node->left);
		cout << node->val << endl;
		BST_walk_in_order(node->right);
	}
	//���ݹؼ��ֲ���
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
	//�������Ԫ�ؽڵ�
	BSTnode* BST_maxmum(BSTnode *x){
		while (x->right != nullptr)
			x = x->right;
		return x;
	}
	//������СԪ�ؽڵ�
	BSTnode* BST_minimum(BSTnode *x){
		while (x->left != nullptr)
			x = x->left;
		return x;
	}
	//���Һ�̽ڵ�(�ٶ�Ϊ���������ʽ)
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
	//����ǰ�����
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
	//����ڵ�
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
	//ɾ���ڵ�
	void BST_delete(BSTnode *z){
		if (z->left == nullptr)
			BST_transplant(z, z->right);
		else if (z->right == nullptr)
			BST_transplant(z, z->left);
		else{
			auto x = BST_successor(z);		//x��z�ĺ�̽ڵ�
			//��̽ڵ���x���Һ���
			if (x->p == z){
				BST_transplant(z, x);
				x->left = z->left;
			}
			else{
				//�����̽ڵ����Һ��ӣ��߽�x->right�ƶ���x
				//ע���ƶ�ֻ�������븸�ڵ�Ĺ�ϵ����û�д����뺢�ӽڵ�Ĺ�ϵ
				if (x->right != nullptr){
					BST_transplant(x, x->right);
					x->right = nullptr;
				}
				//�ٽ�x�ƶ���z
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
	//ʹ��target�������滻src������ֻ���滻src��src->p�Ĺ�ϵ
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