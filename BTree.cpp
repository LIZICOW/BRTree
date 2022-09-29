#include "BTree.h"
using namespace std;

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
ValType &BTree<KeyType, ValType>::getData(const KeyType &key)
{
	Node<KeyType, ValType> *currentNode = root;
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
BTree<KeyType, ValType> &BTree<KeyType, ValType>::remove(const KeyType &key)
{
}

/* 插入一个键为key 值为val的节点 */
template <class KeyType, class ValType>
BTree<KeyType, ValType> &BTree<KeyType, ValType>::setData(const KeyType &key, const ValType &val)
{
	Node<KeyType, ValType> *currentNode = this->root;
	Node<KeyType, ValType> *currentFather = nullptr;
	while (currentNode != nullptr)
	{
		if (currentNode->key == key)
		{ /* 如果已经存在键，就直接修改 */
			curretNode->val = val;
			return *this;
		}
		// 寻找插入的位置
		if (currentNode->key < key)
		{
			currentFather = currentNode;
			currentNode = currenNode->rightChild;
		}
		else
		{
			currentFather = currentNode;
			currentNode = currentNode->leftChild;
		}
	}
	currrentNode = new Node;
	if (currentFather == nullptr)
	{ // 此时说明是空树，直接插入根节点
		root = currentNode;
		root->key = key;
		root->val = val;
		root->color = BLACK;
		root->father = nullptr;
		root->rightChild = nullptr;
		root->leftChild = nullptr;
		return *this;
	}
	currentNode->val = val;
	currentNode->key = key;
	currentNode->color = RED; //先设置为红色，若出现冲突再修改
	currentNode->rightChild = nullptr;
	currentNode->father = currentFather;
	currentNode->leftChild = nullptr;
	if (currentFather->key < key)
	{
		currentFather->rightChild = currentNode;
	}
	else
	{
		currentFather->leftChild = currentNode;
	}
}

template <class KeyType, class ValType>
void BTree<KeyType, ValType>::releaseMemory(const Node<KeyType, ValType> *node)
{
	if (node != nullptr)
	{
		if (node->leftChild != nullptr)
			releaseMemory(node->leftChild);
		if (node->rightChild != nullptr)
			releaseMemory(node->rightChild);
		delete node;
	}
}

template <class KeyType, class ValType>
void BTree<KeyType, ValType>::rotateLeft(const Node<KeyType, ValType> *node)
{
	Node<KeyType, ValType> *father = node->father;
	Node<KeyType, ValType> *old = node;
	Node<KeyType, ValType> *rightOld = node->rightChild;
	Node<KeyType, ValType> *rightLeftOld = leftOld->leftChild;
	//?????????????
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
	node = rightOld; //??????????????????
	node->father = father;
	node->leftChild = old; //?????????????????
	if (rightLeftOld != nullptr)
		node->leftChild->rightChild = rightLeftOld; //????????????????????????????????????????????
}

template <class KeyType, class ValType>
void BTree<KeyType, ValType>::rotateRight(const Node<KeyType, ValType> *node)
{
	Node<KeyType, ValType> *father = node->father;
	Node<KeyType, ValType> *old = node;
	Node<KeyType, ValType> *leftOld = node->leftChild;
	Node<KeyType, ValType> *leftRightOld = leftOld->rightChild;
	//?????????????
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
	node = leftOld;
	node->father = father;
	node->rightChild = fatherOld;
	if (leftRightOld != nullptr)
		node->rightChild->leftChild = leftRightOld;
}
