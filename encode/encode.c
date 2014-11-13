
#include <stdio.h>
#include <stdlib.h>
#include <string.h>/* memset */
#define MAX_SYMBOLS 128
#define MAX_LEN     7
struct tnode
{
    struct  tnode* left; /*used when in tree*/
    struct  tnode*right; /*used when in tree*/  
    struct  tnode*parent;/*used when in tree*/
    struct  tnode* next; /*used when in list*/ 
    float   freq;
    int     isleaf;
    char     symbol;
};


/*global variables*/
char code[MAX_SYMBOLS][MAX_LEN];
struct tnode* root=NULL; /*tree of symbols*/
struct tnode* qhead=NULL; /*list of current symbols*/

/*
    @function   talloc
    @desc       allocates new node 
*/
struct tnode* talloc(int symbol,float freq)
{
    struct tnode* p=(struct tnode*)malloc(sizeof(struct tnode));
    if(p!=NULL)
    {
        p->left=p->right=p->parent=p->next=NULL;
        p->symbol=symbol;
        p->freq=freq;
		p->isleaf=1;
    }
    return p;
}

/*
    @function display_tnode_list
    @desc     displays the list of tnodes during code construction
*/
void pq_display(struct tnode* head)
{
    struct tnode* p=NULL;
    printf("list:");
    for(p=head;p!=NULL;p=p->next)
    {
        printf("(%c,%f) ",p->symbol,p->freq);
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
    struct tnode* curr=NULL;
    struct tnode* prev=NULL;
	printf("inserting:%c,%f\n",p->symbol,p->freq);

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
			if (p->freq <= curr->freq) {
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
			else if (p->freq > curr->freq) {
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
				printf("bug #1\n");
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
    struct tnode* p=NULL;
    /*TODO: write code to remove front of the queue*/

	//���� ť�� �� �տ� �ִ� ���� �����͸� p �� �Űܳ��´�
	p = qhead;
	//���� ť�� �� �տ� �ִ� ���� next �� ���� ť�� �� �� ������(qhead)�� �ִ´�
	qhead = qhead->next;
	if (qhead == NULL) {
		//���� ť�� �� �տ� �ִ� ���� �θ� NULL �� �����Ѵ�
		//qhead->parent = NULL;

		printf("popped:%c,%f\n", p->symbol, p->freq);
		return p;
	}
	else {
		//���� ť�� �� �տ� �ִ� ���� �θ� NULL �� �����Ѵ�
		qhead->parent = NULL;

		printf("popped:%c,%f\n", p->symbol, p->freq);
		return p;
	}

	
}

/*
	@function generate_code
	@desc     generates the string codes given the tree
	NOTE: makes use of the global variable root
*/
void generate_code(struct tnode* root,int depth)
{
	int symbol;
	int len; /*length of code*/
    int i=0;
	struct tnode * curr;
	if(root->isleaf)
	{
		symbol=root->symbol;
		len   =depth;
		/*start backwards*/
		code[symbol][len]=0;
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

		printf("built code:%c,%s\n",symbol,code[symbol]);
	}
	else
	{
		generate_code(root->left,depth+1);
		generate_code(root->right,depth+1);
	}
	
}

/*
	@func	dump_code
	@desc	output code file
*/
void dump_code(FILE* fp)
{
	int i=0;
	for(i=0;i<MAX_SYMBOLS;i++)
	{
		if(code[i][0]) /*non empty*/
			fprintf_s(fp, "%d %s\n", i, code[i]);
	}
}

/*
	@func	encode
	@desc	outputs compressed stream
*/
void encode(char* str,FILE* fout)
{
	while(*str)
	{
		fprintf_s(fout, "%s", code[*str]);
		str++;
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
int main()
{
    /*test pq*/
    struct tnode* p=NULL;
    struct tnode* lc,*rc;
    float freq[]={0.01f,0.04f,0.05f,0.11f,0.19f,0.20f,0.4f};
	int   NCHAR=7; /*number of characters*/
    int i=0;
	const char *CODE_FILE="code.txt";
	const char *OUT_FILE="encoded.txt";
	FILE* fout=NULL;
	/*zero out code*/
	memset(code,0,sizeof(code));

	/*testing queue*/
//    pq_insert(talloc('a',0.1));
//    pq_display(qhead);
//    pq_insert(talloc('b',0.2));
//    pq_display(qhead);
//    pq_insert(talloc('c',0.15));
//    pq_display(qhead);
    /*making sure it pops in the right order*/
//	puts("making sure it pops in the right order");
//	while((p=pq_pop()))
//    {
//        free(p);
//    }
	


	qhead=NULL;
    /*initialize with freq*/
    for(i=0;i<NCHAR;i++)
    {
        pq_insert(talloc('a'+i,freq[i]));
    }
    /*build tree*/
    for(i=0;i<NCHAR-1;i++)
    {
        lc=pq_pop();
        rc=pq_pop();
        /*create parent*/
        p=talloc(0,lc->freq+rc->freq);
        /*set parent link*/
        lc->parent=rc->parent=p;
        /*set child link*/
        p->right=rc; p->left=lc;
		/*make it non-leaf*/
		p->isleaf=0;
        /*add the new node to the queue*/
        pq_insert(p);
    }
    /*get root*/
    root=pq_pop();
	/*build code*/
	generate_code(root,0);
	/*output code*/
	puts("code:");
	fopen_s(&fout, CODE_FILE, "w");
	dump_code(stdout);
	dump_code(fout);
	fclose(fout);

	/*encode a sample string*/
	puts("orginal:aged egg faced bad cabbage");
	fopen_s(&fout, OUT_FILE, "w");
	encode("aged egg faced bad cabbage",stdout);
	encode("aged egg faced bad cabbage",fout);
	fclose(fout);
	printf("\n");
	/*TODO: clear resources*/
	freetree(root);
	freequeue(qhead);

    return 0;
}
