#ifndef OBST_H
#define OBST_H

//最优二叉搜索树

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
		
		//得到仅含有伪关键节点子树的搜索代价与概率和
		for (int i = 1; i < n + 2; i++){
			w[i][i - 1] = q[i - 1];
			e[i][i - 1] = q[i - 1];
		}
		
		//得到(i,j)上子树对应的根节点root_stable与代价期望
		for (int j = 1; j <= n; j++){			//j应该从小到大递增，因为要计算j
			for (int i = j; i > 0; i--){			//i应该从大到小，因为(i,j)的范围应该从小到大
				w[i][j] = w[i][j - 1] + p[j] + p[j];			//计算概率和
				e[i][j] = n+2;
				//遍历得到最小的搜索代价期望
				for (int r = i; r <= j; r++){
					float tmp = e[i][r - 1] + e[r + 1][j] + w[i][j];
					if (e[i][j]>tmp){
						e[i][j] = tmp;				//保存代价期望
						root_stable[i][j] = r;		//得到代价最小时(i,j)的根节点
					}
				}//for
			}//for
		}//for
		root = Span_tree(1, n);
	}

	//广度优先搜索遍历树
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
	node *root;			//生成的二叉搜索树
private:
	vector<double> p, q;					//p为关键字，q为伪关键字
	vector<vector<double>> w;		//w为(i,j)的概率和
	vector<vector<double>> e;		//e为(i,j)的搜索代价期望
	vector<vector<int>> root_stable;		//root_stable为(i,j)的根节点
	int n;		//n为关键字个数
	
	//根据root_stable得到二叉树
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