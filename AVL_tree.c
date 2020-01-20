#include<stdio.h>
#include<malloc.h>
#include "AVLtree.h"

AVL_TREE * minValueNode(AVL_TREE* node)
{
	AVL_TREE* current = node;

	/* loop down to find the leftmost leaf */
	while (current->lChild != NULL)
		current = current->lChild;

	return current;
}

AVL_TREE *leftRotate(AVL_TREE *node)
{
	AVL_TREE *temp1 = node->rChild;
	AVL_TREE *temp2 = temp1->lChild;

	//PERFORM ROTATION
	temp1->lChild = node;
	node->rChild = temp2;

	//update heighgt
	node->height = 1 + Max(Height(node->lChild), Height(node->rChild));
	temp1->height = 1 + Max(Height(temp1->lChild), Height(temp1->rChild));

	return temp1;
}
AVL_TREE *rightRotate(AVL_TREE *node)
{
	AVL_TREE *temp1 = node->lChild;
	AVL_TREE *temp2 = temp1->rChild;

	//PERFORM ROTATION
	temp1->rChild = node;
	node->lChild = temp2;

	//update heighgt
	node->height = 1 + Max(Height(node->lChild), Height(node->rChild));
	temp1->height = 1 + Max(Height(temp1->lChild), Height(temp1->rChild));
	return temp1;
}


int Height(AVL_TREE *node)
{
	if (node == NULL)
		return 0;
	return node->height;
}
int Max(int lnodeheight, int rnodeheight)
{
	return (lnodeheight > rnodeheight) ? lnodeheight : rnodeheight;
}
int GetBalance(AVL_TREE *node)
{
	if (node == NULL)
		return 0;
	return (Height(node->lChild) - Height(node->rChild));
}



AVL_TREE *AllocateNewNode(int val)
{
	AVL_TREE *NewNode = (AVL_TREE *)malloc(sizeof(AVL_TREE));
	NewNode->Data = val;
	NewNode->lChild = NewNode->rChild = NULL;
	NewNode->height = 1;

	return NewNode;

}
AVL_TREE *InsertNewNode(AVL_TREE *node, int val)
{
	if (node == NULL)
	{
		return AllocateNewNode(val);
	}
	else
	{
		if (node->Data > val)
		{
			node->lChild = InsertNewNode(node->lChild, val);
		}
		else if (node->Data < val)
		{
			node->rChild = InsertNewNode(node->rChild, val);
		}
		else
			return node;
		//Update Height of ancestor node
		node->height = 1 + Max(Height(node->lChild), Height(node->rChild));
		//created node successfully
		//Now Get Balance Factor of this ancestor node to check whether this node is unbalanced

		int bal_factor = GetBalance(node);
		//Left Subtree Left Child case		--LL
		if (bal_factor > 1 && node->lChild->Data > val)
		{
			return rightRotate(node);
		}
		//Right Subtree Right Child case		--RR
		if (bal_factor < -1 && node->rChild->Data < val)
		{
			return leftRotate(node);
		}

		//Left Subtree Right Child case		--LR
		if (bal_factor > 1 && node->lChild->Data < val)
		{
			node->lChild = leftRotate(node->lChild);
			return rightRotate(node);
		}

		//Right Subtree Right Child case		--RL
		if (bal_factor < -1 && node->rChild->Data > val)
		{
			node->rChild = rightRotate(node->rChild);
			return leftRotate(node);
		}
		return node;
	}


}
AVL_TREE *DeleteNewNode(AVL_TREE *curr, int val)
{
	if (curr == NULL)
		return curr;

	if (val < curr->Data)
	{
		curr->lChild = DeleteNewNode(curr->lChild, val);
	}
	else if (val > curr->Data)
	{
		curr->rChild =DeleteNewNode(curr->rChild, val);
	}
	else
	{
		// node with only one child or no child 
		if ((curr->lChild == NULL) || (curr->rChild == NULL))
		{
			AVL_TREE *temp = curr->lChild ? curr->lChild : curr->rChild;

			// No child case 
			if (temp == NULL)
			{
				temp = curr;
				curr = NULL;
			}
			else // One child case 
				*curr = *temp; // Copy the contents // the non-empty child 
			free(temp);
		}
		else
		{
			// node with two children: Get the inorder 
			// successor (smallest in the right subtree) 
			AVL_TREE* temp = minValueNode(curr->rChild);

			// Copy the inorder successor's data to this node 
			curr->Data = temp->Data;

			// Delete the inorder successor 
			curr->rChild = DeleteNewNode(curr->rChild, temp->Data);
		}
	}
	if (curr == NULL)
		return curr;

	curr->height = 1 + Max(Height(curr->lChild), Height(curr->rChild));
	//created node successfully
	//Now Get Balance Factor of this ancestor node to check whether this node is unbalanced

	int bal_factor = GetBalance(curr);
	//Left Subtree Left Child case		--LL
	if (bal_factor > 1 && GetBalance(curr->lChild)>=0)
	{
		return rightRotate(curr);
	}
	//Right Subtree Right Child case		--RR
	if (bal_factor < -1 && GetBalance(curr->rChild) <= 0)
	{
		return leftRotate(curr);
	}

	//Left Subtree Right Child case		--LR
	if (bal_factor > 1 && GetBalance(curr->lChild) < 0)
	{
		curr->lChild = leftRotate(curr->lChild);
		return rightRotate(curr);
	}

	//Right Subtree Right Child case		--RL
	if (bal_factor < -1 && GetBalance(curr->rChild) > 0)
	{
		curr->rChild = rightRotate(curr->rChild);
		return leftRotate(curr);
	}
	return curr;


}
void PreOrderTraversal(AVL_TREE *root)
{
	AVL_TREE *temp = root;
	if (temp != NULL)
	{
		printf("%d -> ", temp->Data);
		PreOrderTraversal(temp->lChild);
		PreOrderTraversal(temp->rChild);
	}
}
