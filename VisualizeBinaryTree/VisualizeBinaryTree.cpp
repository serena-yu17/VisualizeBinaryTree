// VisualizeBinaryTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <climits>
#include <sstream>
using namespace std;

struct TreeNode {
	int val;
	struct TreeNode *left;
	struct TreeNode *right;
	TreeNode(int x) : val(x), left(NULL), right(NULL) {}
	TreeNode(): left(NULL), right(NULL) {}
};

std::vector<int> getint(std::string s) {
	if (s.size() == 0)
		return std::vector<int>();
	std::vector<int> nums;
	int i = 0;
	while (i < s.size())
	{
		int buffer = 0;
		int found = 0;
		while (i < s.size() && (isdigit(s[i]) || s[i] == '-'))
		{
			buffer *= 10;
			buffer += s[i] - '0';
			i++;
			found = 1;
		}
		if (found)
			nums.push_back(buffer);
		if (i < s.size() - 4 && s[i] == 'n' && s[i + 1] == 'u' && s[i + 2] == 'l' && s[i + 3] == 'l')
		{
			nums.push_back(INT_MIN);
			i += 4;
		}
		i++;
	}
	return nums;
}

vector<TreeNode*>* buildLevel(vector<TreeNode*>* level, const vector<int>& num, int& p)
{
	vector<TreeNode*>* nextLv = new vector<TreeNode*>;
	for (int i = 0; i < level->size() && p < num.size(); i++)
	{
		if ((*level)[i])
		{
			if (num[p] != INT_MIN)
				(*level)[i]->left = new TreeNode(num[p]);
			else
				(*level)[i]->left = NULL;
			nextLv->push_back((*level)[i]->left);
			p++;
			if (p < num.size())
			{
				if (num[p] != INT_MIN)
					(*level)[i]->right = new TreeNode(num[p]);
				else
					(*level)[i]->right = NULL;
				nextLv->push_back((*level)[i]->right);
				p++;
			}
		}
		else
		{
			p++;
			if (p < num.size())
				p++;
		}
	}
	delete level;
	return nextLv;
}

TreeNode* buildTree(const vector<int>& num)
{
	if (num[0] == INT_MIN)
		return NULL;
	TreeNode* root = new TreeNode(num[0]);
	if (num.size() == 1)
		return 	root;
	vector<TreeNode*>* level = new vector<TreeNode*>;
	level->push_back(root);
	int p = 1;
	while (p < num.size())
		level = buildLevel(level, num, p);
	delete level;
	return root;
}

void recPrintNode(FILE* fs, TreeNode* node, int* nodeCount, int* nullCount)
{	
	int currentnode = *nodeCount;
	if (node->left)
	{
		fprintf(fs, "    node%d [label=\"%d\"];\n", ++(*nodeCount), node->left->val);
		fprintf(fs, "    node%d -> node%d;\n", currentnode, *nodeCount);
		if (!node->right)
		{
			fprintf(fs, "    null%d [shape=point, ];\n", nullCount);
			fprintf(fs, "    node%d -> null%d;\n", currentnode, nullCount);
			*nullCount++;
		}
		recPrintNode(fs, node->left, nodeCount, nullCount);
	}
	if (node->right)
	{
		fprintf(fs, "    node%d [label=\"%d\"];\n", ++(*nodeCount), node->right->val);
		fprintf(fs, "    node%d -> node%d;\n", currentnode, *nodeCount);
		if (!node->left)
		{
			fprintf(fs, "    null%d [shape=point, ];\n", nullCount);
			fprintf(fs, "    node%d -> null%d;\n", currentnode, nullCount);
			*nullCount++;
		}
		recPrintNode(fs, node->right, nodeCount, nullCount);
	}
}

void print_dot(TreeNode* root)
{
	FILE* fs = fopen("./tree.gv", "w");
	fprintf(fs, "digraph binaryTree {\n");
	fprintf(fs, "    node [fontname=\"Arial\"];\n");
	if (!root)
		fprintf(fs, "\n");
	else if (!root->left && root->right)
		fprintf(fs, "    node%d;\n", root->val);
	else
	{
		int* nullCount = new int();
		int* nodeCount = new int();
		fprintf(fs, "    node0 [label=\"%d\"];\n", root->val);
		recPrintNode(fs, root, nodeCount, nullCount);
		delete nullCount;
		delete nodeCount;
	}
	fprintf(fs, "}\n");
	fclose(fs);
}

int main()
{
	ios::sync_with_stdio(false);
	while (1)
	{
		cout << "Input the tree in rows:" << endl;
		string raw;
		getline(cin, raw);
		vector<int> num = getint(raw);
		TreeNode* root = NULL;
		if (num.size())
			root = buildTree(num);
		print_dot(root);
		system("dot -Tsvg ./tree.gv -o graph1.svg ");
		system("graph1.svg");
	}
	return 0;
}

