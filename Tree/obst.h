#ifndef OBST_H
#define OBST_H

//���Ŷ���������

#include "vector"
#include "iostream"
#include "queue"

using std::vector;
using std::queue;
using std::cout;
using std::endl;

struct node{
	node *left, *right;
	int val;
	node(int val) :val(val), left(nullptr), right(nullptr){}
};

class obst{
public:
	obst(vector<double>p, vector<double>q) :p(p), q(q), n(p.size() - 1){
		w = vector<vector<double>>(n + 2, vector<double>(n + 1, 0));
		e = vector<vector<double>>(n + 2, vector<double>(n + 1, 0));
		root_stable = vector<vector<int>>(n + 1, vector<int>(n + 1, 0));
		
		//�õ�������α�ؼ��ڵ�������������������ʺ�
		for (int i = 1; i < n + 2; i++){
			w[i][i - 1] = q[i - 1];
			e[i][i - 1] = q[i - 1];
		}
		
		//�õ�(i,j)��������Ӧ�ĸ��ڵ�root_stable���������
		for (int j = 1; j <= n; j++){			//jӦ�ô�С�����������ΪҪ����j
			for (int i = j; i > 0; i--){			//iӦ�ôӴ�С����Ϊ(i,j)�ķ�ΧӦ�ô�С����
				w[i][j] = w[i][j - 1] + p[j] + p[j];			//������ʺ�
				e[i][j] = n+2;
				//�����õ���С��������������
				for (int r = i; r <= j; r++){
					float tmp = e[i][r - 1] + e[r + 1][j] + w[i][j];
					if (e[i][j]>tmp){
						e[i][j] = tmp;				//�����������
						root_stable[i][j] = r;		//�õ�������Сʱ(i,j)�ĸ��ڵ�
					}
				}//for
			}//for
		}//for
		root = Span_tree(1, n);
	}

	//�����������������
	void obst_walkTree(node *nd){
		queue<node*> que;
		if (nd != nullptr)
			que.push(nd);
		while (!que.empty()){
			node *ptr = que.front();
			cout << ptr->val << endl;
			que.pop();
			if (ptr->left != nullptr)
				que.push(ptr->left);
			if (ptr->right != nullptr)
				que.push(ptr->right);
		}
	}
	node *root;			//���ɵĶ���������
private:
	vector<double> p, q;					//pΪ�ؼ��֣�qΪα�ؼ���
	vector<vector<double>> w;		//wΪ(i,j)�ĸ��ʺ�
	vector<vector<double>> e;		//eΪ(i,j)��������������
	vector<vector<int>> root_stable;		//root_stableΪ(i,j)�ĸ��ڵ�
	int n;		//nΪ�ؼ��ָ���
	
	//����root_stable�õ�������
	node* Span_tree(int begin, int end){
		if (begin > end)
			return nullptr;
		int root_pos = root_stable[begin][end];
		node *newnode = new node(root_pos);
		newnode->left = Span_tree(begin, root_pos - 1);
		newnode->right = Span_tree(root_pos + 1, end);
		return newnode;
	}
};

#endif