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
	//��������ڵ�
	void rbTree_walk_in_order(rbNode *node){
		if (node == nullptr)
			return;
		rbTree_walk_in_order(node->left);
		cout << node->val << endl;
		rbTree_walk_in_order(node->right);
	}
	//���ݹؼ��ֲ���
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
	//�������Ԫ�ؽڵ�
	rbNode* rbTree_maxmum(rbNode *x){
		while (x->right != nullptr)
			x = x->right;
		return x;
	}
	//������СԪ�ؽڵ�
	rbNode* rbTree_minimum(rbNode *x){
		while (x->left != nullptr)
			x = x->left;
		return x;
	}
	//���Һ�̽ڵ�(�ٶ�Ϊ���������ʽ)
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
	//����ǰ�����
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

	//����ڵ�
	void rbTree_insert(value_type k){
		rbNode *tmp = root, *tmp_p = root;
		while (tmp != nullptr){						//tmp_pΪ����ڵ�λ��
			tmp_p = tmp;
			if (tmp->val > k)
				tmp = tmp->left;
			else if (tmp->val < k)
				tmp = tmp->right;
		}//while
		//����ڵ�
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
		rbTree_insert_fixup(newnode);			//���ڲ���ڵ㣬�����ƻ���������ʣ�Ҫ�Ժ�������е���
	}
	//����ڵ�����Ҫ�Ժ����������ɫ�ͽṹ�ĵ���
	void rbTree_insert_fixup(rbNode *z){
		//zΪ����Ľڵ㣬yΪz����ڵ�
		while (z->p != nullptr&&z->p->clr == red){					//���z�ĸ��ڵ�Ϊ��ɫ������Ҫһֱ����
			//������ڵ�Ϊү�ڵ������
			if (z->p == z->p->p->left){
				rbNode *y = z->p->p->right;
				//����1����ڵ�Ϊ��ɫ�������ڵ㡢��ڵ��Ϊ��ɫ��ү�ڵ��Ϊ��ɫ
				if (y->clr == red){
					z->p->clr = black;
					y->clr = black;
					z->p->p->clr = red;
					//����ү�ڵ��Ϊ��ɫ����Ҫ���ү�ڵ��ڽ��е���
					z = z->p->p;
				}//if
				else{
					//����2����ڵ�Ϊ��ɫ��zΪ�Һ��ӣ���Ҫ��z����Ϊ����
					if (z == z->p->right){
						z = z->p;
						rbTree_leftRotate(z);
					}
					//����3����ڵ�Ϊ��ɫ��zΪ���ӣ��Ѹ��ڵ��ڣ�ү�ڵ����ү�ڵ���ת
					z->p->clr = black;
					z->p->p->clr = red;
					rbTree_rightRotate(z->p->p);
				}
			}//if
			//������ڵ�Ϊү�ڵ���Һ���
			else{
				rbNode *y = z->p->p->left;
				//����1����ڵ�Ϊ��ɫ�������ڵ㡢��ڵ��Ϊ��ɫ��ү�ڵ��Ϊ��ɫ
				if (y->clr == red){
					z->p->clr = black;
					y->p->clr = black;
					z->p->p->clr = red;
					//����ү�ڵ��Ϊ��ɫ����Ҫ���ү�ڵ��ڽ��е���
					z = z->p->p;
				}//if
				//����2����ڵ�Ϊ��ɫ��zΪ�Һ��ӣ���Ҫ��z����Ϊ����
				else if (z == z->p->left){
					z = z->p;
					rbTree_rightRotate(z);
				}
				//����3����ڵ�Ϊ��ɫ��zΪ���ӣ��Ѹ��ڵ��ڣ�ү�ڵ����ү�ڵ���ת
				z->p->clr = black;
				z->p->p->clr = red;
				rbTree_leftRotate(z->p->p);
			}
		}
		root->clr = black;
	}
	//��ת
	void rbTree_leftRotate(rbNode *z){
		rbNode *x = z->right;
		rbTree_transplant(z, x);
		z->right = x->left;
		if (x->left != nullptr)
			x->left->p = z;
		x->left = z;
		z->p = x;
	}
	//��ת
	void rbTree_rightRotate(rbNode *z){
		rbNode *x = z->left;
		rbTree_transplant(z, x);
		z->left = x->right;
		if (x->right != nullptr)
			x->right->p = z;
		x->right = z;
		z->p = x;
	}
	//ɾ���ڵ�
	void rbTree_delete(value_type k){
		//����Ҫɾ��z�ڵ㣬��y�ڵ��滻���滻����y�ڵ����ɫ��ɾ��
		//���y�ڵ����ɫΪ��ɫ���ᵼ�º���������ʱ��ƻ�
		//��Ҫ��¼y�ڵ��ԭʼ��ɫ��x�ڵ�Ϊ����y�ڵ�ĵ�
		rbNode *z = rbTree_search(k);
		color y_original_color = z->clr;
		rbNode *x;
		//���1��zû�����ӣ�y��zʵ����һ���ڵ㣬x����z
		if (z->left == nullptr){
			x = z->right;
			rbTree_transplant(z, z->right);
		}
		//���2��zû���Һ��ӣ�y��zʵ����һ���ڵ㣬x����z
		else if (z->right == nullptr){
			x = z->left;
			rbTree_transplant(z, z->left);
		}
		//���3��z�����Һ��ӣ�y��z�ĺ�̽ڵ㣬y����z��y����ɫ���z����ɫ��x����y������x����ɫ���ı䣬���y����ɫ�Ǻ�ɫ����Ҫ��x���е���
		else{
			rbNode *y = rbTree_successor(z);		//yΪz�ĺ�̽ڵ�
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

	//��ɾ���ڵ��ĺ������е���
	void rbTree_delete_fixup(rbNode *z){
		//ֹͣ����������
		while (z != root&&z->clr == black){
			//���zΪ����
			if (z == z->p->left){
				//w��z���ֵܽڵ�
				rbNode *w = z->p->right;
				//���1��wΪ��ɫ����z->p��ת����w����Ϊ��ɫ��ת�����2��3��4
				if (w != nullptr&&w->clr == red){
					rbTree_leftRotate(z->p);
					w->clr = black;
					z->p->clr = red;
					w = z->p->right;
				}
				else{
					//���2��wΪ��ɫ���������Һ��Ӿ�Ϊ��ɫ����w��죬z=z->p���ݹ鵽�ϲ������
					if (w->left != nullptr&&w->left->clr == black&&w->right != nullptr&&w->right->clr == black){
						w->clr = red;
						z = z->p;
					}
					//���3��wΪ��ɫ����������Ϊ��ɫ���Һ���Ϊ��ɫ����w��ת��������ɫ
					else if (w->left != nullptr&&w->left->clr == red&&w->right != nullptr&&w->right->clr == black){
						rbTree_rightRotate(w);
						w->p->clr = black;
						w->clr = red;
						w = w->p;
					}
					//���4��wΪ��ɫ���Һ���Ϊ��ɫ��z->p��ת
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
				//w��z���ֵܽڵ�
				rbNode *w = z->p->right;
				//���1��wΪ��ɫ����z->p��ת����w����Ϊ��ɫ��ת�����2��3��4
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

	//ʹ��target�������滻src������ֻ���滻src��src->p�Ĺ�ϵ
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
	//��������������������
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