
#include <stdio.h>
#include <stdlib.h>
#include <string.h>/* memset */
#define MAX_SYMBOLS 256
#define MAX_LEN     256
struct tnode
{
	struct  tnode* left; /*used when in tree*/
	struct  tnode*right; /*used when in tree*/
	struct  tnode*parent;/*used when in tree*/
	struct  tnode* next; /*used when in list*/
	int		appear;
	int     isleaf;
	unsigned char     symbol;
};


/*global variables*/
char code[MAX_SYMBOLS][MAX_LEN];
struct tnode* root = NULL; /*tree of symbols*/
struct tnode* qhead = NULL; /*list of current symbols*/

/*
@function   talloc
@desc       allocates new node
*/
struct tnode* talloc(int symbol, int appear)
{
	struct tnode* p = (struct tnode*)malloc(sizeof(struct tnode));
	if (p != NULL)
	{
		p->left = p->right = p->parent = p->next = NULL;
		p->symbol = symbol;
		p->appear = appear;
		p->isleaf = 1;
	}
	return p;
}

/*
@function display_tnode_list
@desc     displays the list of tnodes during code construction
*/
void pq_display(struct tnode* head)
{
	struct tnode* p = NULL;
	printf("list:");
	for (p = head; p != NULL; p = p->next)
	{
		printf("(%c,%d) ", p->symbol, p->appear);
	}
	printf("\n");
}

/*
@function pq_insert
@desc     inserts an element into the priority queue
NOTE:     makes use of global variable qhead
*/
void pq_insert(struct tnode* p)
{
	struct tnode* curr = NULL;
	struct tnode* prev = NULL;
	//printf("inserting:%c,%f\n", p->symbol, p->freq);

	if (qhead == NULL){
		//일단 비어 있으니 내가 왕이다
		qhead = p;
	}
	else {
		//비어 있지 않으니 내 자리를 찾아가야 겠다
		//일단 시작점은 qhead 로 삼아야 한다
		curr = qhead;
		while (1) {
			//내자리 찾을 때까지 와일문 돈다
			if (p->appear <= curr->appear) {
				//현재 커서의 빈도수가 나의 빈도수보다 크다
				//즉 내가 이겼다
				//여기서 멈춘다

				//일단 커서의 parent 를 prev 로 지정한다
				prev = curr->parent;

				if (prev == NULL) {
					//만약 prev 가 NULL 이면 맨 앞에 넣어야 하고 ..
					//root 를 대체해야 함

					//qhead 에 p를 연결하고
					qhead = p;
					//p의 부모값으로 NULL 을 넣고
					p->parent = NULL;
					//p의 다음값으로 커서를 넣고
					p->next = curr;
					//커서의 부모값으로 p를 설정
					curr->parent = p;
					//그걸로 끝
					break;
				}
				else {
					//만약 NULL 이 아니면 있는거니 prev 와 curr 의 사이에 끼워넣기

					//prev->next 에 p를 대입
					prev->next = p;
					//curr->parent 에 p를 대입
					curr->parent = p;
					//p->next 에 curr 대입
					p->next = curr;
					//p->parent 에 prev 대입
					p->parent = prev;
					//이야 끝났다
					break;
				}

			}
			else if (p->appear > curr->appear) {
				//현재 커서의 빈도수가 나의 빈도수보다 작다
				//즉 나는 좀더 뒤로 가야 한다

				if (curr->next == NULL) {
					//만약 curr->next 가 NULL 이라면 내 자리는 거기다
					//바꾸고 멈춘다

					//curr->next 에 p를 붙인다
					curr->next = p;
					//p의 부모값으로 커서를 넣는다
					p->parent = curr;
					//p의 다음값으로 NULL 을 넣는다
					p->next = NULL;
					//그걸로 끝 (아마도)
					break;
				}
				else {
					//NULL 이 아니라면 다음루프가 결정해 줄것으로 믿고 미룬다
					curr = curr->next;
					continue;
				}
			}
			else {
				//암것도 하지마
				//printf("bug #1\n");
			}
		}
	}
}

/*
@function pq_pop
@desc     removes the first element
NOTE:     makes use of global variable qhead
*/

struct tnode* pq_pop()
{
	struct tnode* p = NULL;
	/*TODO: write code to remove front of the queue*/

	//현재 큐의 맨 앞에 있는 놈의 포인터를 p 로 옮겨놓는다
	p = qhead;
	//현재 큐의 맨 앞에 있는 놈의 next 를 현재 큐의 맨 앞 포인터(qhead)로 넣는다
	qhead = qhead->next;
	if (qhead == NULL) {
		//현재 큐의 맨 앞에 있는 놈의 부모를 NULL 로 설정한다
		//qhead->parent = NULL;

		//printf("popped:%c,%f\n", p->symbol, p->freq);
		return p;
	}
	else {
		//현재 큐의 맨 앞에 있는 놈의 부모를 NULL 로 설정한다
		qhead->parent = NULL;

		//printf("popped:%c,%f\n", p->symbol, p->freq);
		return p;
	}


}

/*
@function generate_code
@desc     generates the string codes given the tree
NOTE: makes use of the global variable root
*/
void generate_code(struct tnode* root, int depth)
{
	int symbol;
	int len; /*length of code*/
	int i = 0;
	struct tnode * curr;
	if (root->isleaf)
	{
		symbol = root->symbol;
		len = depth;
		/*start backwards*/
		code[symbol][len] = 0;
		/*
		TODO: follow parent pointer to the top
		to generate the code string
		*/
		curr = root;
		while (!(curr->parent == NULL)) {
			//루트에 도달할 때까지 돈다(부모가 NULL 이면 루트다)

			if (curr->parent->left == curr) {
				//현재 커서가 부모의 왼쪽 자식과 동일하다 : '0'
				code[symbol][--len] = '0';
			}
			else if (curr->parent->right == curr) {
				//현재 커서가 부모의 오른쪽 자식과 동일하다 : '1'
				code[symbol][--len] = '1';
			}
			else {
				//이건 삐꾸다
			}
			curr = curr->parent;
		}

		//printf("built code:%c,%s\n", symbol, code[symbol]);
	}
	else
	{
		generate_code(root->left, depth + 1);
		generate_code(root->right, depth + 1);
	}

}

/*
@func	dump_code
@desc	output code file
*/
void dump_code(FILE* fp)
{
	int i = 0;
	for (i = 0; i<MAX_SYMBOLS; i++)
	{
		if (code[i][0]) /*non empty*/
			fprintf(fp, "%d %s\n", i, code[i]);
	}
}

/*
@func	encode
@desc	outputs compressed stream
*/
void encode(FILE* fin, FILE* fout)
{
	int c;
	while ((c = getc(fin)) != EOF) {
		fprintf_s(fout, "%s", code[c]);
	}
}

void freetree(struct tnode* root)
{
	if (root == NULL)
		return;
	freetree(root->right);
	freetree(root->left);
	free(root);
}

void freequeue(struct tnode* root)
{
	if (root == NULL)
		return;
	freequeue(root->next);
	free(root);
}

/*
@function main
*/
int main(int argc, char** argv)
{
	/*test pq*/
	struct tnode* p = NULL;
	struct tnode* lc, *rc;
	int NCHAR = 256; /*number of characters*/
	int i = 0;
	int c = 0;
	int usedAsciiKind = 0;

	float totalCount = 0.f;
	int count[256] = { 0 };
	float freq[256] = { 0.f };
	
	char INPUT_FILE[256];
	char CODE_FILE[256];
	char OUT_FILE[256];

	if (argc > 1) {
		if (atoi(argv[1]) == 1) {
			strcpy_s(INPUT_FILE, _countof(INPUT_FILE), "pg28233.txt");
			strcpy_s(CODE_FILE, _countof(INPUT_FILE), "code_28233_my.txt");
			strcpy_s(OUT_FILE, _countof(INPUT_FILE), "enc_28233_my.txt");
		}
		else if (atoi(argv[1]) == 2) {
			strcpy_s(INPUT_FILE, _countof(INPUT_FILE), "pg1342.txt");
			strcpy_s(CODE_FILE, _countof(INPUT_FILE), "code_1342_my.txt");
			strcpy_s(OUT_FILE, _countof(INPUT_FILE), "enc_1342_my.txt");
		}
	}
	
	FILE* fin = NULL;
	FILE* fout = NULL;
	FILE* fcode = NULL;
	/*zero out code*/
	memset(code, 0, sizeof(code));


	if (argc == 1) {
		printf("Input text file name: ");
		scanf_s("%s", INPUT_FILE);
		printf("Input code file name: ");
		scanf_s("%s", CODE_FILE);
		printf("Input encoded file name: ");
		scanf_s("%s", OUT_FILE);
	}

	//printf("C1\n");

	fopen_s(&fin, INPUT_FILE, "r");
	//일단 빈도 통계를 내야 하므로 처음부터 끝까지 읽어가며 각 아스키 캐릭터들의 수를 센다
	while ((c = getc(fin)) != EOF) {
		totalCount += 1.;
		count[c] += 1;
		//freq[c] = count[c] / totalCount;
	}
	fclose(fin);

	qhead = NULL;
	/*initialize with freq*/
	for (i = 0; i<NCHAR; i++)
	{
		if (count[i] > 0) {
			pq_insert(talloc(i, count[i]));
			usedAsciiKind++;
		}
	}

	/*build tree*/
	for (i = 0; i < (usedAsciiKind - 1); i++)
	{
		lc = pq_pop();
		rc = pq_pop();
		/*create parent*/
		p = talloc(0, lc->appear + rc->appear);
		/*set parent link*/
		lc->parent = rc->parent = p;
		/*set child link*/
		p->right = rc; p->left = lc;
		/*make it non-leaf*/
		p->isleaf = 0;
		/*add the new node to the queue*/
		pq_insert(p);
	}

	/*get root*/
	root = pq_pop();

	/*build code*/
	generate_code(root, 0);

	/*output code*/
	fopen_s(&fout, CODE_FILE, "w");
	dump_code(fout);
	fclose(fout);
	
	/*encode a sample string*/
	fopen_s(&fin, INPUT_FILE, "r");
	fopen_s(&fout, OUT_FILE, "w");
	encode(fin, fout);
	fclose(fin);
	fclose(fout);
	/*TODO: clear resources*/
	freetree(root);
	freequeue(qhead);

	return 0;
}
