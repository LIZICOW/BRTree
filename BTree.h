#pragma once
#include <iostream>
#include <cstring>
#include <string>
using namespace std;

enum class NodeColor
{
	RED,
	BLACK
};

template <class KeyType, class ValType>
struct Node
{
	KeyType key;
	ValType val;
	NodeColor color;

	Node *leftChild;
	Node *rightChild;
	Node *father;
};

template <class KeyType, class ValType>
class BTree
{
private:
	Node<KeyType, ValType> *root;

public:
	BTree();
	~BTree();
	ValType &getData(const KeyType &key);
	BTree<KeyType, ValType> &remove(const KeyType &key);
	BTree<KeyType, ValType>& setData(const KeyType& key, const ValType& val);

private:
	void releaseMemory(Node<KeyType, ValType>* node);
	void rotateLeft(Node<KeyType, ValType>* node);
	void rotateRight(Node<KeyType, ValType>* node);
	void fitNeighbourRedNode(Node<KeyType, ValType>* node);
};

template <class KeyType, class ValType>
BTree<KeyType, ValType>::BTree()
{

}

template <class KeyType, class ValType>
BTree<KeyType, ValType>::~BTree()
{
	if (root != nullptr)
	{
		this->releaseMemory(this->root);
		this->root = nullptr;
	}
}

template <class KeyType, class ValType>
ValType& BTree<KeyType, ValType>::getData(const KeyType& key)
{
	Node<KeyType, ValType>* currentNode = root;
	while (currentNode != nullptr)
	{
		if (currentNode->key == key)
			return currentNode->val;
		if (currentNode->key < key)
			currentNode = currentNode->rightChild;
		else
			currentNode = currentNode->leftChild;
	}
	throw runtime_error("there isn't a key in the tree that you give");
}

template <class KeyType, class ValType>
BTree<KeyType, ValType>& BTree<KeyType, ValType>::remove(const KeyType& key)
{
}

/* 插入一个键为key 值为val的节点 */
template <class KeyType, class ValType>
BTree<KeyType, ValType>& BTree<KeyType, ValType>::setData(const KeyType& key, const ValType& val)
{
	Node<KeyType, ValType>* currentNode = this->root;
	Node<KeyType, ValType>* currentFather = nullptr;
	while (currentNode != nullptr)
	{
		if (currentNode->key == key)
		{ /* 如果已经存在键，就直接修改 */
			currentNode->val = val;
			return *this;
		}
		// 寻找插入的位置
		if (currentNode->key < key)
		{
			currentFather = currentNode;
			currentNode = currentNode->rightChild;
		}
		else
		{
			currentFather = currentNode;
			currentNode = currentNode->leftChild;
		}
	}
	currentNode = new Node<KeyType, ValType>;
	if (currentFather == nullptr)
	{ // 此时说明是空树，直接插入根节点
		root = currentNode;
		root->key = key;
		root->val = val;
		root->color = NodeColor::BLACK;
		root->father = nullptr;
		root->rightChild = nullptr;
		root->leftChild = nullptr;
		return *this;
	}
	currentNode->val = val;
	currentNode->key = key;
	currentNode->color = NodeColor::RED; //先设置为红色，若出现冲突再修改
	currentNode->rightChild = nullptr;
	currentNode->father = currentFather;
	currentNode->leftChild = nullptr;
	if (currentFather->key < key)
		currentFather->rightChild = currentNode;
	else
		currentFather->leftChild = currentNode;
	this->fitNeighbourRedNode(currentNode);
	return *this;
}
int cnt = 0;
template <class KeyType, class ValType>
void BTree<KeyType, ValType>::releaseMemory(Node<KeyType, ValType>* node)
{
	//cout << cnt++ << '\n';
	if (node->leftChild != nullptr)
		releaseMemory(node->leftChild);
	if (node->rightChild != nullptr)
		releaseMemory(node->rightChild);
	delete node;
}

template <class KeyType, class ValType>
void BTree<KeyType, ValType>::rotateLeft(Node<KeyType, ValType>* node)
{
	Node<KeyType, ValType>* father = node->father;
	Node<KeyType, ValType>* old = node;
	Node<KeyType, ValType>* rightOld = node->rightChild;
	Node<KeyType, ValType>* rightLeftOld = rightOld->leftChild;
	//如果是根节点
	if (father == nullptr)
	{
		this->root = rightOld;
	}
	else
	{
		if (father->leftChild == node)
			father->leftChild = rightOld;
		else if (father->rightChild == node)
			father->rightChild = rightOld;
	}
	node = rightOld; //此时右儿子已经是原节点的位置
	node->father = father;
	node->leftChild = old; //原右儿子的左节点是原节点
	node->leftChild->father = node;
	//if (rightLeftOld != nullptr)//如果存在原右儿子的左节点
	node->leftChild->rightChild = rightLeftOld; //添加到原节点的右节点
	if(rightLeftOld != nullptr)
		node->leftChild->rightChild->father = old;
}

template <class KeyType, class ValType>
void BTree<KeyType, ValType>::rotateRight(Node<KeyType, ValType>* node)
{
	Node<KeyType, ValType>* father = node->father;
	Node<KeyType, ValType>* old = node;
	Node<KeyType, ValType>* leftOld = node->leftChild;
	Node<KeyType, ValType>* leftRightOld = leftOld->rightChild;

	if (father == nullptr)
	{
		this->root = leftOld;
	}
	else
	{
		if (father->leftChild == node)
			father->leftChild = leftOld;
		else if (father->rightChild == node)
			father->rightChild = leftOld;
	}
	node = leftOld;
	node->father = father;
	node->rightChild = old;
	node->rightChild->father = old;
	//if (leftRightOld != nullptr)
	node->rightChild->leftChild = leftRightOld;
	if(leftRightOld!=nullptr)
		node->rightChild->leftChild->father = old;
}


/**
* 修改两个相邻节点都是红色的情况
*				黑
*			   /  \
*			 红    黑
*			/ \   / \
*		   红-->node
**/
template <class KeyType, class ValType>
void BTree<KeyType, ValType>::fitNeighbourRedNode(Node<KeyType, ValType>* node) {
	Node<KeyType, ValType>* currentNode = node;
	Node<KeyType, ValType>* grandPa = node->father->father;
	Node<KeyType, ValType>* father = node->father;
	Node<KeyType, ValType>* uncle;
	while (grandPa != nullptr) {
		if (currentNode->father == nullptr) {
			currentNode->color = NodeColor::BLACK;
			break;
		}
		father = currentNode->father;
		if (father->color == NodeColor::BLACK)
			break;

		grandPa = father->father;
		if (grandPa->leftChild == father)
			uncle = grandPa->rightChild;
		else
			uncle = grandPa->leftChild;

		//如果叔叔也是红色，就把叔叔和父亲和爷爷反色
		if (uncle != nullptr && uncle->color == NodeColor::RED) {
			uncle->color = father->color = NodeColor::BLACK;
			grandPa->color = NodeColor::RED;
		}
		else {
			//情况一
			/***
			*				黑
			*			   /  \
			*			 红    黑(NULL)
			*			/ \   / \
			*		   红 RB RB RB
			**/
			if (father->leftChild == currentNode && grandPa->leftChild == father) {
				this->rotateRight(grandPa);
				grandPa->color = NodeColor::RED;
				father->color = NodeColor::BLACK;
			}
			//情况二
			/***
			*				黑
			*			   /  \
			*			 红    黑(NULL)
			*			/ \   / \
			*		   RB 红 RB RB
			**/
			else if (father->rightChild == currentNode && grandPa->leftChild == father) {
				this->rotateLeft(father);
				this->rotateRight(grandPa);
				grandPa->color = NodeColor::RED;
				currentNode->color = NodeColor::BLACK;
			}
			//情况三
			/***
			*				黑
			*			   /  \
			*		黑(NULL)   红
			*			/ \   / \
			*		   RB RB RB 红
			**/
			else if (father->rightChild == currentNode && grandPa->rightChild == father) {
				this->rotateLeft(grandPa);
				grandPa->color = NodeColor::RED;
				father->color = NodeColor::BLACK;
			}
			//情况四
			/***
			*				黑
			*			   /  \
			*		黑(NULL)   红
			*			/ \   / \
			*		   RB RB 红 RB
			**/
			else if (father->leftChild == currentNode && grandPa->leftChild == father) {
				this->rotateRight(father);
				this->rotateLeft(grandPa);
				grandPa->color = NodeColor::RED;
				currentNode->color = NodeColor::BLACK;
			}
			break;
		}
		currentNode = grandPa;
	}

}
