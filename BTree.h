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
enum class ChildSide
{
	LEFT,
	RIGHT
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
	int exist(const KeyType& key);
	ValType &getData(const KeyType &key);
	BTree<KeyType, ValType> &remove(const KeyType &key);
	BTree<KeyType, ValType> &setData(const KeyType &key, const ValType &val);
	ValType& operator[](const KeyType& key);
	int empty() {
		return root == nullptr;
	}
private:
	void releaseMemory(Node<KeyType, ValType> *node);
	void rotateLeft(Node<KeyType, ValType> *node);
	void rotateRight(Node<KeyType, ValType> *node);
	void fitNeighbourRedNode(Node<KeyType, ValType> *node);
	void fitUnblancedBlackNode(Node<KeyType, ValType> *node);
	void print(Node<KeyType, ValType>* node);
};

template <class KeyType, class ValType>
BTree<KeyType, ValType>::BTree()
{
}

template <class KeyType, class ValType>
int BTree<KeyType, ValType>::exist(const KeyType& key)
{
	Node<KeyType, ValType>* currentNode = root;
	while (currentNode != nullptr)
	{
		if (currentNode->key == key)
			return 1;
		if (currentNode->key < key)
			currentNode = currentNode->rightChild;
		else
			currentNode = currentNode->leftChild;
	}
	return 0;
}

template <class KeyType, class ValType>
void BTree<KeyType, ValType>::print(Node<KeyType, ValType>* n)
{
	static int level = -1; //记录是第几层次
	int i;
	if (NULL == n)
		return;

	level++;
	print(n->rightChild);
	level--;

	level++;
	for (i = 0; i < level; i++)
		printf("\t");
	//printf("%2d\n", n->key);
	if (n->color == NodeColor::RED)
		printf("R\n");
	else
		printf("B\n");
	print(n->leftChild);
	level--;
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
	throw runtime_error("there isn't a key in the tree that you give.");
}

template <class KeyType, class ValType>
ValType &BTree<KeyType, ValType>::operator[](const KeyType &key)
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
	throw runtime_error("there isn't a key in the tree that you give.");
}

template <class KeyType, class ValType>
BTree<KeyType, ValType> &BTree<KeyType, ValType>::remove(const KeyType &key)
{
	Node<KeyType, ValType> *currentNode = root;
	while (currentNode != nullptr)
	{
		if (key == currentNode->key)
			break;
		else
			currentNode = (currentNode->key > key ? currentNode->leftChild : currentNode->rightChild);
	}

	if (currentNode == nullptr)
		throw std::runtime_error("there isn't a key in the tree that you give.");

	while (currentNode->leftChild != nullptr || currentNode->rightChild != nullptr)
	{
		Node<KeyType, ValType> *forwardSon = currentNode->leftChild;
		Node<KeyType, ValType> *backwardSon = currentNode->rightChild;
		Node<KeyType, ValType> *replaceNode = nullptr;
		// 当前存在右儿子，则寻找后继节点，第一次向右搜，之后找最深左节点
		if (backwardSon != nullptr)
		{
			while (backwardSon->leftChild != nullptr)
			{
				backwardSon = backwardSon->leftChild;
			}
			replaceNode = backwardSon;
		}
		// 当前一定仅存在左儿子，找最深右节点
		else
		{
			while (forwardSon->rightChild != nullptr)
			{
				forwardSon = forwardSon->rightChild;
			}
			replaceNode = forwardSon;
		}
		NodeColor tmp = currentNode->color;

		// 交换颜色
		currentNode->color = replaceNode->color;
		replaceNode->color = tmp;

		struct nodeRelationship
		{
			Node<KeyType, ValType> *father;
			Node<KeyType, ValType> *leftChild;
			Node<KeyType, ValType> *rightChild;
		} cur, rep;
		cur = {currentNode->father, currentNode->leftChild, currentNode->rightChild};
		rep = {replaceNode->father, replaceNode->leftChild, replaceNode->rightChild};

		if (currentNode->father != nullptr && currentNode->father->leftChild == currentNode)
			currentNode->father->leftChild = replaceNode;
		else if (currentNode->father != nullptr)
			currentNode->father->rightChild = replaceNode;
		else
			this->root = replaceNode;

		if (replaceNode->leftChild != nullptr)
		{
			replaceNode->leftChild->father = currentNode;
		}
		if (replaceNode->rightChild != nullptr)
		{
			replaceNode->rightChild->father = currentNode;
		}

		if (currentNode->leftChild == replaceNode) {
			replaceNode->leftChild = currentNode;
			replaceNode->father = currentNode->father;
			replaceNode->rightChild = currentNode->rightChild;
			if (currentNode->rightChild != nullptr)
				currentNode->rightChild->father = replaceNode;
			currentNode->leftChild = rep.leftChild;
			currentNode->rightChild = rep.rightChild;
			currentNode->father = replaceNode;
			continue;
		}
		else if (currentNode->rightChild == replaceNode) {
			replaceNode->rightChild = currentNode;
			replaceNode->father = currentNode->father;
			replaceNode->leftChild = currentNode->leftChild;
			if (currentNode->leftChild != nullptr)
				currentNode->leftChild->father = replaceNode;
			currentNode->leftChild = rep.leftChild;
			currentNode->rightChild = rep.rightChild;
			currentNode->father = replaceNode;
			continue;
		}

		if (currentNode->leftChild != nullptr)
		{
			currentNode->leftChild->father = replaceNode;
		}

		if (currentNode->rightChild != nullptr)
		{
			currentNode->rightChild->father = replaceNode;
		}

		if (replaceNode->father->leftChild == replaceNode)
			replaceNode->father->leftChild = currentNode;
		else
			replaceNode->father->rightChild = currentNode;
		

		currentNode->father = rep.father;
		currentNode->leftChild = rep.leftChild;
		currentNode->rightChild = rep.rightChild;

		replaceNode->father = cur.father;
		replaceNode->leftChild = cur.leftChild;
		replaceNode->rightChild = cur.rightChild;
	}

	if (currentNode == root)
	{
		delete root;
		root = nullptr;
	}
	else if (currentNode->color == NodeColor::RED)
	{
		if (currentNode == currentNode->father->leftChild)
		{
			currentNode->father->leftChild = nullptr;
		}
		else
		{
			currentNode->father->rightChild = nullptr;
		}
	}
	else
	{
		Node<KeyType, ValType> *brother = (currentNode->father->leftChild == currentNode ? currentNode->father->rightChild : currentNode->father->leftChild);
		Node<KeyType, ValType> *father = currentNode->father;
		ChildSide brotherSideToFather = (father->leftChild == currentNode ? ChildSide::RIGHT : ChildSide::LEFT);

		if (father->leftChild == currentNode)
		{
			father->leftChild = nullptr;
		}
		else
		{
			father->rightChild = nullptr;
		}
		// 释放节点。
		delete currentNode;
		if (father->color == NodeColor::RED)
		{
			if (brother->leftChild != nullptr && brother->rightChild != nullptr)
			{
				/*
				 *       红
				 *      /  \
				 *del->黑  黑
				 *        /  \
				 *       红  红
				 */
				if (brotherSideToFather == ChildSide::RIGHT)
				{
					this->rotateLeft(father);
					brother->color = NodeColor::RED;
					father->color = NodeColor::BLACK;
					brother->rightChild->color = NodeColor::BLACK;
				}
				//对称的
				else
				{
					this->rotateRight(father);
					brother->color = NodeColor::RED;
					father->color = NodeColor::BLACK;
					brother->leftChild->color = NodeColor::BLACK;
				}
			}
			else if (brother->leftChild != nullptr && brother->rightChild == nullptr)
			{
				/*
				 *       红
				 *      /  \
				 *del->黑  黑
				 *        /
				 *       红
				 */
				if (brotherSideToFather == ChildSide::RIGHT)
				{
					this->rotateRight(brother);
					this->rotateLeft(father);
					father->color = NodeColor::BLACK;
				}
				/*
				 *       红
				 *      /  \
				 *	   黑  黑 <-del
				 *    /
				 *   红
				 */
				else
				{
					this->rotateRight(father);
					brother->color = NodeColor::RED;
					father->color = NodeColor::BLACK;
					brother->leftChild->color = NodeColor::BLACK;
				}
			}
			else if (brother->leftChild == nullptr && brother->rightChild != nullptr)
			{
				/*
				 *       红
				 *      /  \
				 *del->黑  黑
				 *           \
				 *           红
				 */
				if (brotherSideToFather == ChildSide::RIGHT)
				{
					this->rotateLeft(father);
					brother->color = NodeColor::RED;
					father->color = NodeColor::BLACK;
					brother->rightChild->color = NodeColor::BLACK;
				}
				/*
				 *       红
				 *      /  \
				 *     黑  黑 <-del
				 *      \
				 *      红
				 */
				else
				{
					this->rotateLeft(brother);
					this->rotateRight(father);
					father->color = NodeColor::BLACK;
				}
			}
			/*
			 *       红
			 *      /  \
			 *del->黑  黑
			 */
			else if (brother->leftChild == nullptr && brother->rightChild == nullptr)
			{
				father->color = NodeColor::BLACK;
				brother->color = NodeColor::RED;
			}
		} //父节点为红色
		//父节点为黑色
		else if (father->color == NodeColor::BLACK)
		{
			/*
			 *       黑										黑
			 *      /  \								   /  \
			 *del->黑  红       				              黑   黑
			 *        /  \                --------->     / \  / \
			 *       黑  黑						  (原父)红  N N  N
			 *       /\  /\                              \
			 *      N  N N N <-红或不存在                  N
			 */
			if (brother->color == NodeColor::RED)
			{
				if (brotherSideToFather == ChildSide::RIGHT)
				{
					brother->color = NodeColor::BLACK;
					father->color = NodeColor::RED;
					this->rotateLeft(father);
					this->rotateLeft(father);
					if (father->rightChild != nullptr)
						this->fitNeighbourRedNode(father->rightChild);
				}
				//对称的
				else
				{
					brother->color = NodeColor::BLACK;
					father->color = NodeColor::RED;
					this->rotateRight(father);
					this->rotateRight(father);
					if (father->leftChild != nullptr)
						this->fitNeighbourRedNode(father->leftChild);
				}
			}
			else if (brother->color == NodeColor::BLACK)
			{
				/*
				 *       黑
				 *      /  \
				 *del->黑  黑
				 *        /  \
				 *       红  红
				 */
				if (brother->leftChild != nullptr && brother->rightChild != nullptr)
				{
					if (brotherSideToFather == ChildSide::RIGHT)
					{
						brother->leftChild->color = NodeColor::BLACK;
						this->rotateRight(brother);
						this->rotateLeft(father);
					}
					//对称的
					else
					{
						brother->rightChild->color = NodeColor::BLACK;
						this->rotateLeft(brother);
						this->rotateRight(father);
					}
				}
				else if (brother->leftChild == nullptr && brother->rightChild != nullptr)
				{
					/*
					 *       黑
					 *      /  \
					 *del->黑  黑
					 *           \
					 *           红
					 */
					if (brotherSideToFather == ChildSide::RIGHT)
					{
						brother->rightChild->color = NodeColor::BLACK;
						this->rotateLeft(father);
					}
					/*
					 *       黑
					 *      /  \
					 *     黑  黑 <-del
					 *       \
					 *       红
					 */
					else
					{
						brother->rightChild->color = NodeColor::BLACK;
						this->rotateLeft(brother);
						this->rotateRight(father);
					}
				}
				else if (brother->leftChild != nullptr && brother->rightChild == nullptr)
				{
					/*
					 *       黑
					 *      /  \
					 *del->黑  黑
					 *        /
					 *       红
					 */
					if (brotherSideToFather == ChildSide::RIGHT)
					{
						brother->leftChild->color = NodeColor::BLACK;
						this->rotateRight(brother);
						this->rotateLeft(father);
					}
					/*
					 *       黑
					 *      /  \
					 *     黑  黑 <-del
					 *    /
					 *   红
					 */
					else
					{
						brother->leftChild->color = NodeColor::BLACK;
						this->rotateRight(father);
					}
				}
				else
				{
					/*
					 *       黑
					 *      /  \
					 *     黑  黑
					 */
					brother->color = NodeColor::RED;
					this->fitUnblancedBlackNode(father);
				}
			}
		}
	}

	return *this;
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
void BTree<KeyType, ValType>::releaseMemory(Node<KeyType, ValType> *node)
{
	// cout << cnt++ << '\n';
	if (node->leftChild != nullptr)
		releaseMemory(node->leftChild);
	if (node->rightChild != nullptr)
		releaseMemory(node->rightChild);
	delete node;
}

template <class KeyType, class ValType>
void BTree<KeyType, ValType>::rotateLeft(Node<KeyType, ValType> *node)
{
	Node<KeyType, ValType> *father = node->father;
	Node<KeyType, ValType> *old = node;
	Node<KeyType, ValType> *rightOld = node->rightChild;
	Node<KeyType, ValType> *rightLeftOld = rightOld->leftChild;
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
	if (rightLeftOld != nullptr)
		rightLeftOld->father = old;
	node->father = rightOld;
	node->rightChild = rightLeftOld;
	rightOld->father = father;
	rightOld->leftChild = old;
}

template <class KeyType, class ValType>
void BTree<KeyType, ValType>::rotateRight(Node<KeyType, ValType> *node)
{
	Node<KeyType, ValType> *father = node->father;
	Node<KeyType, ValType> *old = node;
	Node<KeyType, ValType> *leftOld = node->leftChild;
	Node<KeyType, ValType> *leftRightOld = leftOld->rightChild;

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
	if (leftRightOld != nullptr)
		leftRightOld->father = old;
	node->father = leftOld;
	node->leftChild = leftRightOld;
	leftOld->father = father;
	leftOld->rightChild = old;
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
void BTree<KeyType, ValType>::fitNeighbourRedNode(Node<KeyType, ValType> *node)
{
	Node<KeyType, ValType> *currentNode = node;
	Node<KeyType, ValType> *grandPa = node->father->father;
	Node<KeyType, ValType> *father = node->father;
	Node<KeyType, ValType> *uncle;
	while (grandPa != nullptr)
	{
		if (currentNode->father == nullptr)
		{
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
		if (uncle != nullptr && uncle->color == NodeColor::RED)
		{
			uncle->color = father->color = NodeColor::BLACK;
			grandPa->color = NodeColor::RED;
		}
		else
		{
			//情况一
			/***
			 *				黑
			 *			   /  \
			 *			 红    黑(NULL)
			 *			/ \   / \
			 *		   红 RB RB RB
			 **/
			if (father->leftChild == currentNode && grandPa->leftChild == father)
			{
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
			else if (father->rightChild == currentNode && grandPa->leftChild == father)
			{
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
			else if (father->rightChild == currentNode && grandPa->rightChild == father)
			{
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
			else if (father->leftChild == currentNode && grandPa->rightChild == father)
			{
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

template <class KeyType, class ValType>
void BTree<KeyType, ValType>::fitUnblancedBlackNode(Node<KeyType, ValType> *node)
{
	Node<KeyType, ValType> *currentNode = node;
	while (currentNode != root)
	{
		Node<KeyType, ValType> *brother = (currentNode->father->leftChild == currentNode ? currentNode->father->rightChild : currentNode->father->leftChild);
		Node<KeyType, ValType> *father = currentNode->father;
		ChildSide brotherSideToFather = (father->leftChild == currentNode ? ChildSide::RIGHT : ChildSide::LEFT);

		if (father->color == NodeColor::RED)
		{
			/***
			 *		红
			 *	   /  \
			 *	  黑  黑
			 *		 / \
			 *		黑 RB
			 **/
			if (brotherSideToFather == ChildSide::RIGHT && brother->leftChild->color == NodeColor::BLACK)
			{
				this->rotateLeft(father);
			}
			/***
			 *		红
			 *	   /  \
			 *	  黑  黑
			 *   / \
			 *	RB 黑
			 **/
			else if (brotherSideToFather == ChildSide::LEFT && brother->rightChild->color == NodeColor::BLACK)
			{
				this->rotateRight(father);
			}
			/***
			 *		红
			 *	   /  \
			 *	  黑  黑
			 *		 / \
			 *	    红 黑
			 *     / \
			 *    B   B
			 **/
			else if (brotherSideToFather == ChildSide::RIGHT && brother->rightChild->color == NodeColor::BLACK)
			{
				father->color = NodeColor::BLACK;
				brother->color = NodeColor::RED;
				this->fitNeighbourRedNode(brother->leftChild);
			}
			/***
			 *		红
			 *	   /  \
			 *	  黑  黑
			 *   / \
			 *	黑 红
			 *    / \
			 *   B   B
			 **/
			else if (brotherSideToFather == ChildSide::LEFT && brother->leftChild->color == NodeColor::BLACK)
			{
				father->color = NodeColor::BLACK;
				brother->color = NodeColor::RED;
				this->fitNeighbourRedNode(brother->rightChild);
			}
			/***
			 *		红
			 *	   /  \
			 *	  黑  黑
			 *		 / \
			 *	    红  红
			 *     / \  /\
			 *    B  B  B B
			 **/
			else if (brotherSideToFather == ChildSide::RIGHT && brother->rightChild->color == NodeColor::RED)
			{
				father->color = NodeColor::BLACK;
				brother->color = NodeColor::RED;
				brother->rightChild->color = NodeColor::BLACK;
				this->rotateLeft(father);
			}
			else if (brotherSideToFather == ChildSide::LEFT && brother->leftChild->color == NodeColor::RED)
			{
				father->color = NodeColor::BLACK;
				brother->color = NodeColor::RED;
				brother->leftChild->color = NodeColor::BLACK;
				this->rotateRight(father);
			}
			break;
		} //父节点是红色
		//父节点是黑色
		else if (father->color == NodeColor::BLACK)
		{
			if (brother->color == NodeColor::BLACK) {
				/***
				 *		黑
				 *	   /  \
				 *	  黑  黑
				 *		 / \
				 *	    黑  黑
				 **/
				if (brother->leftChild->color == NodeColor::BLACK && brother->rightChild->color == NodeColor::BLACK)
				{
					brother->color = NodeColor::RED;
					currentNode = father;
				}
				else if (brotherSideToFather == ChildSide::RIGHT)
				{
					/***
					 *		黑
					 *	   /  \
					 *	  黑  黑
					 *		 / \
					 *	    RB  红
					 **/
					if (brother->rightChild->color == NodeColor::RED)
					{
						brother->rightChild->color = NodeColor::BLACK;
						this->rotateLeft(father);
					}
					/***
					 *		黑
					 *	   /  \
					 *	  黑  黑
					 *		 / \
					 *	    红  黑
					 *     / \
					 *    RB RB
					 **/
					else if (brother->leftChild->color == NodeColor::RED)
					{
						brother->leftChild->color = NodeColor::BLACK;
						this->rotateRight(brother);
						this->rotateLeft(father);
					}
					break;
				}
				else if (brotherSideToFather == ChildSide::LEFT)
				{
					/***
					 *		黑
					 *	   /  \
					 *	  黑  黑
					 *	 / \
					 *  红 RB
					 **/
					if (brother->leftChild->color == NodeColor::RED)
					{
						brother->leftChild->color = NodeColor::BLACK;
						this->rotateRight(father);
					}
					/***
					 *		黑
					 *	   /  \
					 *	  黑  黑
					 *	 / \
					 *  黑 红
					 *    / \
					 *   RB RB
					 **/
					else if (brother->rightChild->color == NodeColor::RED)
					{
						brother->rightChild->color = NodeColor::BLACK;
						this->rotateLeft(brother);
						this->rotateRight(father);
					}
					break;
				}
			}//兄弟节点为黑
			//兄弟节点为红
			else {
				/***
				*		黑
				*	   /  \
				*	  黑  红
				*		 / \
				*	    黑  黑
				**/
				if (brotherSideToFather == ChildSide::RIGHT) {
					father->color = NodeColor::RED;
					brother->color = NodeColor::BLACK;
					this->rotateLeft(father);
				}
				//对称的
				else if (brotherSideToFather == ChildSide::LEFT) {
					father->color = NodeColor::RED;
					brother->color = NodeColor::BLACK;
					this->rotateRight(father);
				}
			}
		}
	}
}