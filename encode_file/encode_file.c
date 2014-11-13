
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
		//�ϴ� ��� ������ ���� ���̴�
		qhead = p;
	}
	else {
		//��� ���� ������ �� �ڸ��� ã�ư��� �ڴ�
		//�ϴ� �������� qhead �� ��ƾ� �Ѵ�
		curr = qhead;
		while (1) {
			//���ڸ� ã�� ������ ���Ϲ� ����
			if (p->appear <= curr->appear) {
				//���� Ŀ���� �󵵼��� ���� �󵵼����� ũ��
				//�� ���� �̰��
				//���⼭ �����

				//�ϴ� Ŀ���� parent �� prev �� �����Ѵ�
				prev = curr->parent;

				if (prev == NULL) {
					//���� prev �� NULL �̸� �� �տ� �־�� �ϰ� ..
					//root �� ��ü�ؾ� ��

					//qhead �� p�� �����ϰ�
					qhead = p;
					//p�� �θ����� NULL �� �ְ�
					p->parent = NULL;
					//p�� ���������� Ŀ���� �ְ�
					p->next = curr;
					//Ŀ���� �θ����� p�� ����
					curr->parent = p;
					//�װɷ� ��
					break;
				}
				else {
					//���� NULL �� �ƴϸ� �ִ°Ŵ� prev �� curr �� ���̿� �����ֱ�

					//prev->next �� p�� ����
					prev->next = p;
					//curr->parent �� p�� ����
					curr->parent = p;
					//p->next �� curr ����
					p->next = curr;
					//p->parent �� prev ����
					p->parent = prev;
					//�̾� ������
					break;
				}

			}
			else if (p->appear > curr->appear) {
				//���� Ŀ���� �󵵼��� ���� �󵵼����� �۴�
				//�� ���� ���� �ڷ� ���� �Ѵ�

				if (curr->next == NULL) {
					//���� curr->next �� NULL �̶�� �� �ڸ��� �ű��
					//�ٲٰ� �����

					//curr->next �� p�� ���δ�
					curr->next = p;
					//p�� �θ����� Ŀ���� �ִ´�
					p->parent = curr;
					//p�� ���������� NULL �� �ִ´�
					p->next = NULL;
					//�װɷ� �� (�Ƹ���)
					break;
				}
				else {
					//NULL �� �ƴ϶�� ���������� ������ �ٰ����� �ϰ� �̷��
					curr = curr->next;
					continue;
				}
			}
			else {
				//�ϰ͵� ������
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

	//���� ť�� �� �տ� �ִ� ���� �����͸� p �� �Űܳ��´�
	p = qhead;
	//���� ť�� �� �տ� �ִ� ���� next �� ���� ť�� �� �� ������(qhead)�� �ִ´�
	qhead = qhead->next;
	if (qhead == NULL) {
		//���� ť�� �� �տ� �ִ� ���� �θ� NULL �� �����Ѵ�
		//qhead->parent = NULL;

		//printf("popped:%c,%f\n", p->symbol, p->freq);
		return p;
	}
	else {
		//���� ť�� �� �տ� �ִ� ���� �θ� NULL �� �����Ѵ�
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
			//��Ʈ�� ������ ������ ����(�θ� NULL �̸� ��Ʈ��)

			if (curr->parent->left == curr) {
				//���� Ŀ���� �θ��� ���� �ڽİ� �����ϴ� : '0'
				code[symbol][--len] = '0';
			}
			else if (curr->parent->right == curr) {
				//���� Ŀ���� �θ��� ������ �ڽİ� �����ϴ� : '1'
				code[symbol][--len] = '1';
			}
			else {
				//�̰� �߲ٴ�
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
	//�ϴ� �� ��踦 ���� �ϹǷ� ó������ ������ �о�� �� �ƽ�Ű ĳ���͵��� ���� ����
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
