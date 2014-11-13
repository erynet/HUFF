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
unsigned long int streamLength;
unsigned char bufferBS;
unsigned int fillCountBS;

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

void print_bit(char c) {
	int m, i;
	for (i = 7; i >= 0; i--) {
		m = 1 << i;
		if ((c & m) == 0) {
			putc('0', stdout);
		}
		else {
			putc('1', stdout);
		}
	}
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
			}
		}
	}
}

/*
@function pq_pop
@desc     removes the first element
NOTE:     makes use of global variable qhead
*/
struct tnode* pq_pop() {
	struct tnode* p = NULL;
	/*TODO: write code to remove front of the queue*/

	//현재 큐의 맨 앞에 있는 놈의 포인터를 p 로 옮겨놓는다
	p = qhead;
	//현재 큐의 맨 앞에 있는 놈의 next 를 현재 큐의 맨 앞 포인터(qhead)로 넣는다
	qhead = qhead->next;
	if (qhead == NULL) {
		//현재 큐의 맨 앞에 있는 놈의 부모를 NULL 로 설정한다
		return p;
	}
	else {
		//현재 큐의 맨 앞에 있는 놈의 부모를 NULL 로 설정한다
		qhead->parent = NULL;
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
	for (i = 0; i<MAX_SYMBOLS; i++) {
		if (code[i][0]) /*non empty*/
			fprintf_s(fp, "%d %s\n", i, code[i]);
	}
}

unsigned int dump_code_bin(FILE* fp) {
	unsigned int i = 0;
	unsigned int shift_count = 0;
	unsigned char c = 0;
	unsigned short entry_count = 0;
	unsigned char bitstream_token = 0;
	unsigned char bitstream_length = 0;
	unsigned char bitstream_length_byte = 0;
	unsigned int total_size = 0;

	for (c = 0; c < MAX_SYMBOLS; c++) {
		if (code[c][0])
			entry_count++;
		if (c == 255)
			break;
	}
	fwrite(&entry_count, sizeof(unsigned short), 1, fp);
	total_size += sizeof(unsigned short);

	for (c = 0; c < MAX_SYMBOLS; c++) {
		if (code[c][0]) {
			fwrite(&c, sizeof(unsigned char), 1, fp);
			total_size += sizeof(unsigned char);

			bitstream_length = strlen(code[c]);
			bitstream_length_byte = bitstream_length / 8;
			if (bitstream_length % 8)
				bitstream_length_byte++;
			fwrite(&bitstream_length, sizeof(unsigned char), 1, fp);
			total_size += sizeof(unsigned char);

			fwrite(&bitstream_length_byte, sizeof(unsigned char), 1, fp);
			total_size += sizeof(unsigned char);

#ifdef _DEBUG
			printf("%d ", c);
#endif
			
			for (i = 0; i < bitstream_length; i++) {
				if (code[c][i] == '1')
					bitstream_token |= 1;

				if (shift_count == 7) {
					fwrite(&bitstream_token, sizeof(unsigned char), 1, fp);
					total_size += sizeof(unsigned char);
#ifdef _DEBUG
					print_bit(bitstream_token);
					putc(' ', stdout);
#endif
					bitstream_token = 0;
					shift_count = 0;
				}
				else {
					bitstream_token <<= 1;
					shift_count++;
				}
			}
			if (shift_count) {
				bitstream_token <<= (7 - shift_count);
				fwrite(&bitstream_token, sizeof(unsigned char), 1, fp);
				total_size += sizeof(unsigned char);
#ifdef _DEBUG
				print_bit(bitstream_token);
				putc(' ', stdout);
#endif

				bitstream_token = 0;
				shift_count = 0;
			}
		}
#ifdef _DEBUG
		if (code[c][0]) {
			putc('\n', stdout);
			printf("%d", c);
			for (i = 0; i < strlen(code[c]); i++) {
				if (!(i % 8))
					putc(' ', stdout);
			putc(code[c][i], stdout);
			}
			putc('\n', stdout);
		}
#endif
		
		if (c == 255)
			break;
	}
	return total_size;
}

void bs_begin(FILE* fout) {
	fwrite(&streamLength, sizeof(unsigned long int), 1, fout);
	dump_code_bin(fout);

	fillCountBS = 0;
}

void bs_append(FILE* fout, unsigned char c) {
	bufferBS <<= 1;
	if (c == '1')
		bufferBS |= 1;
	fillCountBS++;

	if (fillCountBS == 8) {
		fwrite(&bufferBS, sizeof(unsigned char), 1, fout);
		bufferBS = 0;
		fillCountBS = 0;
	}
}

void bs_end(FILE* fout) {
	while (fillCountBS < 8) {
		bufferBS <<= 1;
		fillCountBS++;
	}
	fwrite(&bufferBS, sizeof(unsigned char), 1, fout);
}

/*
@func	encode
@desc	outputs compressed stream
*/
void encode(FILE* fin, FILE* fout) {
	int c;
	while ((c = getc(fin)) != EOF) {
		fprintf_s(fout, "%s", code[c]);
	}
}

void encode_bin(FILE* fin, FILE* fout) {
	unsigned char c;
	unsigned int i;
	unsigned int codeLength;
	bs_begin(fout);

	while (1) {
		if (feof(fin))
			break;
		fread_s(&c, sizeof(unsigned char), sizeof(unsigned char), 1, fin);
		codeLength = strlen(code[c]);
		for (i = 0; i < codeLength; i++)
			bs_append(fout, code[c][i]);
	}
	bs_end(fout);
}

void freetree(struct tnode* root) {
	if (root == NULL)
		return;
	freetree(root->right);
	freetree(root->left);
	free(root);
}

void freequeue(struct tnode* root) {
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

	int i = 0;
	unsigned char c = 0;
	unsigned short usedSymbolCount = 0; /*number of characters*/
	int count[256] = { 0 };

	char INPUT_FILE[256];
	char OUTPUT_FILE[256];

	FILE* fin = NULL;
	FILE* fout = NULL;
	FILE* fcode = NULL;

	/*zero out code*/
	memset(code, 0, sizeof(code));

	if (argc == 1) {
		printf("USAGE : %s file_to_compress", argv[0]);
		return 0;
	}
	else {
		strcpy_s(INPUT_FILE, _countof(INPUT_FILE), argv[1]);
		strcpy_s(OUTPUT_FILE, _countof(OUTPUT_FILE), argv[1]);
		strcat_s(OUTPUT_FILE, _countof(OUTPUT_FILE), ".huff");
	}

	fopen_s(&fin, INPUT_FILE, "rb");
	fseek(fin, 0, SEEK_END);
	streamLength = ftell(fin);
	rewind(fin);
	//일단 빈도 통계를 내야 하므로 처음부터 끝까지 읽어가며 각 아스키 캐릭터들의 수를 센다
	while (1) {
		if (feof(fin))
			break;
		fread_s(&c, sizeof(unsigned char), sizeof(unsigned char), 1, fin);

		count[c]++;
	}
	fclose(fin);

	qhead = NULL;
	/*initialize with freq*/
	for (i = 0; i < MAX_SYMBOLS; i++) {
		if (count[i] > 0) {
			pq_insert(talloc(i, count[i]));
			usedSymbolCount++;
		}
	}
	usedSymbolCount--;

	/*build tree*/
	for (i = 0; i < usedSymbolCount; i++) {
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

	/*encode a sample string*/
	fopen_s(&fin, INPUT_FILE, "rb");
	fopen_s(&fout, OUTPUT_FILE, "wb");
	encode_bin(fin, fout);
	fclose(fin);
	fclose(fout);
	/*TODO: clear resources*/
	freetree(root);
	freequeue(qhead);

	return 0;
}