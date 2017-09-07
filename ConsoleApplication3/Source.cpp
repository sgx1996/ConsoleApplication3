#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdbool>
#include <cstring>
#include <cstdlib>





enum
{
	INVALID_ACCOUNT_NO = 0,
};



typedef struct _ClientData
{
	int acctNum;
	char lastName[15];
	char firstName[10];
	double balance;
}ClientData, *ClientDataPtr;

static ClientDataPtr s_clients = 0;

typedef struct _Node
{

	ClientData data;
	struct _Node* next;
} Node, *NodePtr;

int  enterChoice(void);

int newRecord(NodePtr *, ClientData);
void textFile(NodePtr *);
void processList(NodePtr);
int updateRecord(NodePtr *);

int deleteRecord(NodePtr *);

bool fileExist(const char* file)
{
	FILE *pf = fopen(file, "r");
	if (pf == 0)
		return false;
	fclose(pf);
	return true;
}
int main()
{
	NodePtr header = 0;
	Node* pCurNode = (Node*)malloc(sizeof(Node));
	memset(pCurNode, 0, sizeof(Node));



	int choice;
	while ((choice = enterChoice()) != 4)
	{

		switch (choice)
		{

		case 1:
			updateRecord(&header);
			break;
		case 2:
			fputs("input format:\n"
				"[acctNum] [firstname] [lastname] [balance]\n", stdout);
			scanf("%d%s%s%lf", &pCurNode->data.acctNum, pCurNode->data.firstName,
				pCurNode->data.lastName, &pCurNode->data.balance);
			newRecord(&header, pCurNode->data);
			processList(header);
			break;
		case 3:
			deleteRecord(&header);
			processList(header);
			break;
		}
	}
	textFile(&header);
	return 0;
}



void textFile(NodePtr *pNode)
{
	NodePtr reNode = *pNode;
	FILE *writePtr = 0;
	Node* newNode = (Node*)malloc(sizeof(Node));
	memset(newNode, 0, sizeof(Node));

	if ((writePtr = fopen("accounts.txt", "w")) == NULL)
	{
		printf("File could not be opened.\n");
	}
	else
	{
		fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
			"Acct", "Last Name", "First Name", "Balance");

		while (reNode != 0)
		{


			fprintf(writePtr, "%-6d%-16s%-11s%10.2f\n",
				reNode->data.acctNum, reNode->data.lastName,
				reNode->data.firstName, reNode->data.balance);

			reNode = reNode->next;
		}

		fclose(writePtr);
	}

}

int updateRecord(NodePtr *fPtr)
{
	NodePtr upNode = *fPtr;
	int account;

	printf("Enter account to update ( 1 - 100 ): ");
	scanf("%d", &account);

	while (upNode->next != 0)
	{
		if (upNode->data.acctNum != account)
		{
			printf("Acount #%d has no information.\n", account);
			return 0;
		}
		else
		{
			printf("%-6d%-16s%-11s%10.2f\n\n",
				upNode->data.acctNum, upNode->data.firstName,
				upNode->data.lastName, upNode->data.balance);
			printf("Enter charge ( + ) or payment ( - ): ");
			double transaction;
			scanf("%lf", &transaction);
			upNode->data.balance += transaction;
			printf("%-6d%-16s%-11s%10.2f\n",
				upNode->data.acctNum, upNode->data.firstName,
				upNode->data.lastName, upNode->data.balance);
		}
		upNode = upNode->next;
	}
	return 0;
}

int deleteRecord(NodePtr *fPtr)
{
	if (fPtr == 0 || *fPtr == 0)
	{
		printf("no information\n");
		return 0;
	}
	NodePtr deNode = *fPtr;

	int accountNum;
	printf("Enter account number to "
		"delete ( 1 - 100 ): ");
	scanf("%d", &accountNum);
	NodePtr preNode = 0;
	while (deNode != 0)
	{
		if (deNode->data.acctNum == accountNum)
		{
			if (preNode == 0)
			{
				*fPtr = (*fPtr)->next;
			}
			else if (deNode->next == 0)
			{
				preNode->next = NULL;
			}
			else
			{
				preNode->next = deNode->next;
			}
			free(deNode);
			break;

		}
		preNode = deNode;
		deNode = deNode->next;
	}

	return 0;
}
int insertData(NodePtr *ppNode, ClientData data)
{
	NodePtr pCurNode = *ppNode;

	if (pCurNode == 0) //表为空的情况（插入节点位置为空）
	{
		NodePtr ptr = (NodePtr)malloc(sizeof(Node));
		memset(ptr, 0, sizeof(Node));

		if (ppNode != 0)
		{
			*ppNode = ptr;
		}//*ppNode = new_node(0);

		pCurNode = *ppNode;
	}
	else
	{
		//1. 生成新Node
		NodePtr pNewNode = (Node*)malloc(sizeof(Node));
		memset(pNewNode, 0, sizeof(Node));
		//2. 更新插入位置节点的原下个节点与新节点的关系

		pNewNode->next = pCurNode->next;
		//3. 更新插入位置节点与新节点的关系
		pCurNode->next = pNewNode;


		pCurNode = pNewNode;//更新当前结点
	}
	//复制数据（给节点数据赋值）
	pCurNode->data = data;

	return 0;
}
int newRecord(NodePtr *pNode, ClientData data)
{

	NodePtr npNode = *pNode;
	if (npNode == 0)
	{
		return insertData(pNode, data);

	}

	while (npNode->next != 0)
	{
		npNode = npNode->next;
	}
	NodePtr pNewNode = (Node*)malloc(sizeof(Node));
	memset(pNewNode, 0, sizeof(Node));
	npNode->next = pNewNode;

	pNewNode->data = data;

	//printf("Enter lastname, firstname, balance\n? ");
	//scanf("%s%s%lf", &client.lastName, &client.firstName,
	//	&client.balance);
	//client.acctNum = accountNum;

	return 0;
}



int enterChoice(void)
{
	int menuChoice;

	printf("\nEnter your choice\n"
		"1 - update an account\n"
		"2 - add a new account\n"
		"3 - delete an account\n"
		"4 - end program and store a formatted text file\n of acounts called"
		"\"accounts.txt\" for printing\n? ");
	scanf("%d", &menuChoice);
	return menuChoice;
}

void processList(NodePtr list)
{
	//遍历单链表，输出数据
	Node* pNode = list;

	while (pNode != 0)
	{
		printf(":%d\t%s\t%s\t%lf\n", pNode->data.acctNum, pNode->data.firstName,
			pNode->data.lastName, pNode->data.balance);
		pNode = pNode->next;

	}

	fputs("\n", stdout);
}