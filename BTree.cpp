#include"BTree.h"
using namespace std;

template<class KeyType, class ValType>
BTree<KeyType, ValType>::BTree() {

}



template<class KeyType, class ValType>
BTree<KeyType, ValType>::~BTree() {
	if (root != nullptr) {
		this->releaseMemory(this->root);
		this->root = nullptr;
	}
}



template<class KeyType, class ValType>
ValType& BTree<KeyType, ValType>::getData(const KeyType& key) {
	Node<KeyType, ValType>* currentNode = root;
	while (currentNode != nullptr) {
		if (currentNode->key == key)
			return currentNode->val;
		if (currentNode->key < key)
			currentNode = currentNode->rightChild;
		else
			currentNode = currentNode->leftChild;
	}
	throw runtime_error("there isn't a key in the tree that you give");
}



template<class KeyType, class ValType>
BTree<KeyType, ValType>& BTree<KeyType, ValType>::remove(const KeyType& key) {

}



template<class KeyType, class ValType>
BTree<KeyType, ValType>& BTree<KeyType, ValType>::setData(const KeyType& key, const ValType& val) {

}



template<class KeyType, class ValType>
void BTree<KeyType, ValType>::releaseMemory(const Node<KeyType, ValType>* node) {
	if (node != nullptr) {
		if (node->leftChild != nullptr)
			releaseMemory(node->leftChild);
		if (node->rightChild != nullptr)
			releaseMemory(node->rightChild);
		delete node;
	}
}



template<class KeyType, class ValType>
void BTree<KeyType, ValType>::rotateLeft(const Node<KeyType, ValType>* node) {
	Node<KeyType, ValType>* father = node->father;
	Node<KeyType, ValType>* old = node;
	Node<KeyType, ValType>* rightOld = node->rightChild;
	Node<KeyType, ValType>* rightLeftOld = leftOld->leftChild;
	//���¸��ڵ���ӽڵ�
	if (father == nullptr)
	{
		this->root = rightOld;
	}
	else {
		if (father->leftChild == node)
			father->leftChild = rightOld;
		else if (father->rightChild == node)
			father->rightChild = rightOld;
	}
	node = rightOld;//��ǰ�ڵ��Ѿ���Ϊԭ�Ҷ���
	node->father = father;
	node->leftChild = old;//�Ҷ��ӵ�����Ӹ�Ϊԭ�ڵ�
	if (rightLeftOld != nullptr)
		node->leftChild->rightChild = rightLeftOld;//���ԭ�Ҷ��ӵ�����Ӵ��������ڱ���˸��ڵ������ӵ��Ҷ���

}



template<class KeyType, class ValType>
void BTree<KeyType, ValType>::rotateRight(const Node<KeyType, ValType>* node) {
	Node<KeyType, ValType>* father = node->father;
	Node<KeyType, ValType>* old = node;
	Node<KeyType, ValType>* leftOld = node->leftChild;
	Node<KeyType, ValType>* leftRightOld = leftOld->rightChild;
	//���¸��ڵ���ӽڵ�
	if (father == nullptr)
	{
		this->root = rightOld;
	}
	else {
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


