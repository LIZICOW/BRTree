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
	//更新父节点的子节点
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
	node = rightOld;//当前节点已经改为原右儿子
	node->father = father;
	node->leftChild = old;//右儿子的左儿子改为原节点
	if (rightLeftOld != nullptr)
		node->leftChild->rightChild = rightLeftOld;//如果原右儿子的左儿子存在则现在变成了根节点的左儿子的右儿子

}



template<class KeyType, class ValType>
void BTree<KeyType, ValType>::rotateRight(const Node<KeyType, ValType>* node) {
	Node<KeyType, ValType>* father = node->father;
	Node<KeyType, ValType>* old = node;
	Node<KeyType, ValType>* leftOld = node->leftChild;
	Node<KeyType, ValType>* leftRightOld = leftOld->rightChild;
	//更新父节点的子节点
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


