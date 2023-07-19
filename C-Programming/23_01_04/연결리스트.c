#include <stdio.h>
#include <stdlib.h>    //malloc()을 실행하기 위한 헤더파일

typedef struct ListNode  	                //연결리스트를 구현할 구조체
{
	int value;                           //구조체 멤버 int값
	struct ListNode *next;              //구조체 자료형 포인터 변수 
}ListNode;                               //변수 선언

int insertNode(ListNode **ptrHead,int value)
{
	ListNode*tempNode = (ListNode*)malloc(sizeof(ListNode));  //헤드노드 생성
	if(!tempNode){             //tempNode가 아닐 경우
		return -1;             //메모리 여유가 없을 경우
	}
	tempNode->value =value;     //순회용 노드 생성
	tempNode->next =*ptrHead;   //tempNode의 다음은 헤드
	*ptrHead =tempNode;
	return 1;
}

/*int insertAtEnd(ListNode **ptrHead,int value)
{
	ListNode*head = *ptrHead;
	ListNode*tempNode = (ListNode*)malloc(sizeof(ListNode));
	
	if(!tempNode){
		return -1;
	}
	tempNode->value =value;
	tempNode->next =NULL;
	
	if(head==NULL){
	tempNode->next =*ptrHead;
	*ptrHead =tempNode;
	return 1;
	}
	while(head->next !=NULL){
		head = head->next;
	}
	tempNode->next = head->next;
	head->next = tempNode;
	return 1;
}*/

void printList(ListNode*head)
{
	printf("리스트 출력:");
	while (head){
		printf("%d", head->value);
		head=head->next;
	}
	printf("\n");
}

int main()
{
	ListNode *head = NULL;
	insertNode(&head,1);    //리스트에 원소 추가
	insertNode(&head,5);
	insertNode(&head,3);
	insertNode(&head,7);
	printList(head);        //리스트의 모든 내용을 출력
	
	return 0;
}
