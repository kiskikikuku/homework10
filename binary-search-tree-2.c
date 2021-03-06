#include<stdio.h>
#include<stdlib.h>

typedef struct node{
        int key;
        struct node* left;
        struct node* right;
} Node;

#define MAX_STACK_SIZE 20
Node* stack[MAX_STACK_SIZE];
int top = -1;
Node* pop();
void push(Node* aNode); // stack

#define MAX_QUEUE_SIZE 20
Node* queue[MAX_QUEUE_SIZE];
int front = -1;
int rear = -1;
Node* deQueue();
void enQueue(Node* aNode); // queue

int initializeBST(Node** h);

void recursiveInorder(Node* ptr); // recursive indrder traversal
void iterativeInorder(Node* ptr); // iterative indrder traversal
void levelOrder(Node *ptr); // lever order traversal
int insert(Node* head, int key);
int deleteNode(Node* head, int key);
int freeBST(Node* head);

void printStack();

int main()
{
	char command;
	int key;
	Node* head = NULL;

	do{
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initializeBST(&head);
			break;
		case 'q': case 'Q':
			freeBST(head);
			break;
		case 'i': case 'I':
			printf("Your Key = ");
			scanf("%d", &key);
			insert(head, key);
			break;
		case 'd': case 'D':
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(head, key);
			break;

		case 'r': case 'R':
			recursiveInorder(head->left);
			break;
		case 't': case 'T':
			iterativeInorder(head->left);
			break;

		case 'l': case 'L':
			levelOrder(head->left);
			break;

		case 'p': case 'P':
			printStack();
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}


int initializeBST(Node** h){

    if(*h != NULL)
        freeBST(*h);

    *h = (Node*)malloc(sizeof(Node));
    (*h)->left = NULL;
    (*h)->right = NULL;
    (*h)->key = -9999;

    top = -1;
    front = rear = -1; // 스택, 큐도 초기화

    return 1;
}

void recursiveInorder(Node* ptr){

    if(ptr){
        recursiveInorder(ptr->left);
        printf("%d ", ptr->key);
        recursiveInorder(ptr->right);
    }
	 
}

void iterativeInorder(Node* ptr){
	while(1){

		while(ptr){ // left child 가 없을때까지 반복
			push(ptr); // 스택에 복귀주소 저장
			ptr = ptr->left; // left child로 이동
		}

		ptr = pop();
		if(!ptr) break;
		printf("%d ", ptr->key); // 방문 (key값 출력)

		ptr = ptr->right; // right child로 이동
	}

}

void levelOrder(Node *ptr){ // lever order traversal
	
	if(!ptr) return; // 공백 트리

	enQueue(ptr); // 루트노드 큐에 삽입

	while(1){

		ptr = deQueue();

		if(ptr){
			printf("%d ", ptr->key);

			if(ptr->left)
				enQueue(ptr->left);
			if(ptr->right)
				enQueue(ptr->right);
		}
		else break;

	}

}

int insert(Node* head, int key){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL; //삽입 노드 할당 및 초기화

    if (head->left == NULL)
    {   
        head->left = newNode;
        return 1; // 공백 BST의 경우, root 자리에 바로 삽입
    }
    
    Node* ptr = head->left;
    Node* parentNode = NULL; // 공백이 아닌 경우, 삽입 위치 찾을 ptr과 그 부모를 기록할 노드 포인터

    while(ptr != NULL){

        if(ptr->key == key) return 1; // 이미 key를 가진 노드가 존재, 종료한다.

        parentNode = ptr; // 부모 기록

        if(ptr->key < key) // 찾는 key값과 비교하여
            ptr = ptr->right; 
        else  				// 왼쪽, 오른쪽 탐색방향 결정
            ptr = ptr->left; 
    }

    if(parentNode->key > key) // 삽입할 부모 노드의 key와 비교
        parentNode->left = newNode; // 부모의 key가 크면, 왼쪽 자식
    else
        parentNode->right = newNode; // 삽입할 노드 key가 크면 오른쪽 자식으로 삽입

    return 1;
}

int deleteNode(Node* head, int key){
	if(head == NULL || head->left == NULL){
		printf("\n tree is NULL");
		return -1;
	}

	Node* root = head->left;

	Node* ptr = root; // 루트노드부터 탐색
	Node* parent = NULL; // ptr의 부모 노드 기록

	while((ptr != NULL)&&(ptr->key!=key)){
		if(ptr->key != key){

			parent = ptr; // 부모 노드 기록

			if(ptr->key > key)
				ptr = ptr->left;
			else
				ptr = ptr->right;
		}
	} // key를 가진 노드(삭제 대상)까지 이동

	if(ptr == NULL){
		printf("No node for key %d\n ", key);
		return -1;
	} // key를 가진 노드 X

	if(ptr->left == NULL && ptr->right == NULL) { // 1. leaf노드의 경우

		if(parent != NULL) { // 부모 여부 검사
			if(parent->left == ptr) // 삭제하려는 노드가 부모의 왼쪽 자식인 경우
				parent->left == NULL;
			else
				parent->right == NULL; // 오른쪽 자식
		} else {
			head->left = NULL; // root를 지우는 경우
		}

		free(ptr); // 할당 해제
		return 1;
	}

	if(ptr->left ==NULL || ptr->right == NULL){ // 2. 자식 노드가 1개인 경우

		Node* child;
		if (ptr->left != NULL)
			child = ptr->left; // 왼쪽 자식을 가진 경우 기록
		else 
			child = ptr->right; // 오른쪽 자식
		
		if(parent != NULL) { // 지우는 노드가 root가 아닌 경우
			
			if(parent->left == ptr) // ptr이 부모의 어느 방향 자식인지 확인
				parent->left = child;
			else
				parent->right = child;
		}else{ // root 노드를 지운 경우
			root = child; // 루트 교체
		}

		free(ptr); // 할당 해제
		return 1;
	}
	
	// 3. 자식이 2개인 노드의 경우
	Node* candidate; //좌측 서브트리에서 최대값 찾기
	parent = ptr;

	candidate = ptr->left;

	while (candidate->right != NULL)
	{	
		parent = candidate;
		candidate = candidate->right; // 좌측 서브트리의 최대값까지 이동
	}
	
	if(parent->left == candidate) 
		parent->left = candidate->left; 
	else
		parent->right = candidate->left; // 지워질 노드의 왼쪽 자식 링크 보존

	ptr->key = candidate->key; // 삭제될 노드의 key를 좌측 서브트리의 최댓값으로 대체

	free(candidate);
	return 1;
}

void freeNode(Node* ptr){
	
	if(ptr){
		freeNode(ptr->left);
		freeNode(ptr->right);
		free(ptr);
	}

}

int freeBST(Node* head){

    if(head->left == head){
        free(head);
        return 1;
    }

    Node* p = head->left;

    freeNode(p);

    free(head);
    return 1;
}

void printStack(){
	if(top == -1) {
		printf("Stack is empty \n");
		return;
	}

	for (int i = 0; i <= top; i++)
	{
		printf("stack[%d] : [%d]\n", i, stack[i]->key);
	}
	
	return;
}

Node* pop(){

	if( top < 0 ) return NULL;
	return stack[top--];
}

void push(Node* aNode){

	top++;
	stack[top] = aNode;
}

Node* deQueue(){

	if(front == rear){
		
		return NULL;
	}

	front = (front+1) % MAX_QUEUE_SIZE;

	return queue[front];
}

void enQueue(Node* aNode){

	rear = (rear+1)%MAX_QUEUE_SIZE;
	if(front == rear){
		printf("queue is full\n");
		return;
	}

	queue[rear] = aNode;
}
