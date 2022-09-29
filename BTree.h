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

/* ����һ����Ϊkey ֵΪval�Ľڵ� */
template <class KeyType, class ValType>
BTree<KeyType, ValType>& BTree<KeyType, ValType>::setData(const KeyType& key, const ValType& val)
{
	Node<KeyType, ValType>* currentNode = this->root;
	Node<KeyType, ValType>* currentFather = nullptr;
	while (currentNode != nullptr)
	{
		if (currentNode->key == key)
		{ /* ����Ѿ����ڼ�����ֱ���޸� */
			currentNode->val = val;
			return *this;
		}
		// Ѱ�Ҳ����λ��
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
	{ // ��ʱ˵���ǿ�����ֱ�Ӳ�����ڵ�
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
	currentNode->color = NodeColor::RED; //������Ϊ��ɫ�������ֳ�ͻ���޸�
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
	//����Ǹ��ڵ�
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
	node = rightOld; //��ʱ�Ҷ����Ѿ���ԭ�ڵ��λ��
	node->father = father;
	node->leftChild = old; //ԭ�Ҷ��ӵ���ڵ���ԭ�ڵ�
	node->leftChild->father = node;
	//if (rightLeftOld != nullptr)//�������ԭ�Ҷ��ӵ���ڵ�
	node->leftChild->rightChild = rightLeftOld; //��ӵ�ԭ�ڵ���ҽڵ�
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
* �޸��������ڽڵ㶼�Ǻ�ɫ�����
*				��
*			   /  \
*			 ��    ��
*			/ \   / \
*		   ��-->node
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

		//�������Ҳ�Ǻ�ɫ���Ͱ�����͸��׺�үү��ɫ
		if (uncle != nullptr && uncle->color == NodeColor::RED) {
			uncle->color = father->color = NodeColor::BLACK;
			grandPa->color = NodeColor::RED;
		}
		else {
			//���һ
			/***
			*				��
			*			   /  \
			*			 ��    ��(NULL)
			*			/ \   / \
			*		   �� RB RB RB
			**/
			if (father->leftChild == currentNode && grandPa->leftChild == father) {
				this->rotateRight(grandPa);
				grandPa->color = NodeColor::RED;
				father->color = NodeColor::BLACK;
			}
			//�����
			/***
			*				��
			*			   /  \
			*			 ��    ��(NULL)
			*			/ \   / \
			*		   RB �� RB RB
			**/
			else if (father->rightChild == currentNode && grandPa->leftChild == father) {
				this->rotateLeft(father);
				this->rotateRight(grandPa);
				grandPa->color = NodeColor::RED;
				currentNode->color = NodeColor::BLACK;
			}
			//�����
			/***
			*				��
			*			   /  \
			*		��(NULL)   ��
			*			/ \   / \
			*		   RB RB RB ��
			**/
			else if (father->rightChild == currentNode && grandPa->rightChild == father) {
				this->rotateLeft(grandPa);
				grandPa->color = NodeColor::RED;
				father->color = NodeColor::BLACK;
			}
			//�����
			/***
			*				��
			*			   /  \
			*		��(NULL)   ��
			*			/ \   / \
			*		   RB RB �� RB
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
