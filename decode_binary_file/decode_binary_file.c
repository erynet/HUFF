#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 256
#define MAX_LEN     256

struct tnode
{
	struct  tnode* left; /*used when in tree*/
	struct  tnode*right; /*used when in tree*/
	int     isleaf;
	unsigned char     symbol;
};


/*global variables*/
struct tnode* root = NULL; /*tree of symbols*/
unsigned long int streamLength;
unsigned int remainCountBS;
char bufferBS;

/*
@function   talloc
@desc       allocates new node
*/
struct tnode* talloc()
{
	struct tnode* p = (struct tnode*)malloc(sizeof(struct tnode));
	if (p != NULL)
	{
		p->left = p->right = NULL;
		p->symbol = 0;
		p->isleaf = 0;
	}
	return p;
}

/*
@function build_tree
@desc     builds the symbol tree given the list of symbols and code.h
NOTE: alters the global variable root that has already been allocated in main
*/
void build_tree(FILE* fp)
{
	//int	symbol;
	unsigned char	strcode[MAX_LEN];
	//int		items_read;
	int		i, j, k, l, len;
	struct	tnode* curr = NULL;
	
	unsigned short entry_count = 0;
	unsigned char character_symbol = 0;
	unsigned char bitstream_length = 0;
	unsigned char bitstream_index = 0;
	unsigned char bitstream_length_byte = 0;
	unsigned char bitstream_token = 0;

	fread_s(&entry_count, sizeof(unsigned short), sizeof(unsigned short), 1, fp);

	for (i = 0; i < entry_count; i++) {
		fread_s(&character_symbol, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

		fread_s(&bitstream_length, sizeof(unsigned char), sizeof(unsigned char), 1, fp);
		
		fread_s(&bitstream_length_byte, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

		bitstream_index = 0;
		//debug
		//printf("[%c] length %d, length_byte %d\n", character_symbol, bitstream_length, bitstream_length_byte);
		//debug
		for (j = 0; j < bitstream_length_byte; j++) {
			fread_s(&bitstream_token, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

			if (bitstream_length >= 8) {
				l = 8;
				bitstream_length -= 8;
			}
			else {
				l = bitstream_length % 8;
			}

			for (k = 0; k < l; k++) {
				if (bitstream_token & 128)
					strcode[bitstream_index] = '1';
				else
					strcode[bitstream_index] = '0';
				bitstream_token <<= 1;
				bitstream_index++;
			}
			//debug
			//printf("%s\n", strcode);
			//debug
		}
		strcode[bitstream_index] = 0;
		//debug
		printf("%d %s\n", character_symbol, strcode);
		//debug


		//if (items_read != 2) break;
		curr = root;
		len = strlen(strcode);
		for (j = 0; j < len; j++)
		{
			/*TODO: create the tree as you go*/
			if (strcode[j] == '0') {
				//���ʿ� �̹� �ڽ��� �ֳ� ���� �˻��Ѵ�
				if (curr->left == NULL) {
					//���ʿ� �ڽ��� ���ٰ� �ǴܵǸ� ���ο�� ���� �޾Ƶд�
					curr->left = talloc();
				}
				//Ŀ���� �ڽ����� �ű��
				curr = curr->left;
			}
			else if (strcode[j] == '1') {
				//�����ʿ� �̹� �ڽ��� �ֳ� ���� �˻��Ѵ�
				if (curr->right == NULL) {
					//�����ʿ� �ڽ��� ���ٰ� �ǴܵǸ� ���ο�� ���� �޾Ƶд�
					curr->right = talloc();
				}
				//Ŀ���� �ڽ����� �ű��
				curr = curr->right;
			}
		}
		/*assign code*/
		curr->isleaf = 1;
		curr->symbol = character_symbol;// symbol;
	}
}
/*
function decode
*/
void decode(FILE* fin, FILE* fout)
{
	int c;
	struct tnode* curr = root;
	while ((c = getc(fin)) != EOF)
	{
		/*TODO:
		traverse the tree
		print the symbols only if you encounter a leaf node
		*/
		if (c == '0') {
			curr = curr->left;
		}
		else if (c == '1') {
			curr = curr->right;
		}
		if (curr->isleaf) {
			fwrite(&(curr->symbol), sizeof(unsigned char), 1, fout);
			curr = root;
		}
	}
}

void printBit(char c) {
	int m, i;
	putc('b', stdout);
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

void openBS(FILE* fin) {
	fread_s(&streamLength, sizeof(unsigned long int), sizeof(unsigned long int), 1, fin);
	remainCountBS = 0;
	//debug
	printf("streamLength : %lu\n", streamLength);
	//debug
}

int readBS(FILE* fin) {
	int result;
	if (remainCountBS == 0) {
		if (feof(fin)) {
			//debug
			printf("End of File\n");
			//debug
			return EOF;
		}
		else {
			fread_s(&bufferBS, sizeof(char), sizeof(char), 1, fin);
		}
		remainCountBS = 8;
	}
	result = bufferBS & (1 << 7);
	bufferBS <<= 1;
	remainCountBS--;
	if (result)
		return '1';
	else
		return '0';
}

/*
function decode_bin
*/
void decode_bin(FILE* fin, FILE* fout)
{
	int c;
	struct tnode* curr = root;
	openBS(fin);
	while ((c = readBS(fin)) != EOF)
	{
		/*TODO:
		traverse the tree
		print the symbols only if you encounter a leaf node
		*/
		if (c == '0') {
			curr = curr->left;
		}
		else if (c == '1') {
			curr = curr->right;
		}

		if (curr->isleaf) {
			fwrite(&(curr->symbol), sizeof(unsigned char), 1, fout);
			curr = root;
			streamLength--;
		}

		if (!streamLength) {
			//debug
			printf("End of Processing\n");
			//debug
			break;
		}
	}
}


/*
@function freetree
@desc	  cleans up resources for tree
*/

void freetree(struct tnode* root)
{
	if (root == NULL)
		return;
	freetree(root->right);
	freetree(root->left);
	free(root);
}

int main(int argc, char** argv)
{
	char INPUT_FILE[256];
	char CODE_FILE[256];
	char OUTPUT_FILE[256];

	if (argc < 4) {
		printf("USAGE : %s file_to_decompress code_file target_file", argv[0]);
		return 0;
	}
	else {
		strcpy_s(INPUT_FILE, _countof(INPUT_FILE), argv[1]);
		strcpy_s(CODE_FILE, _countof(CODE_FILE), argv[2]);
		strcpy_s(OUTPUT_FILE, _countof(OUTPUT_FILE), argv[3]);
	}

	FILE* fin;
	FILE* fcode;
	FILE* fout;

	/*allocate root*/
	root = talloc();
	fopen_s(&fcode, CODE_FILE, "rb");

	/*build tree*/
	//debug
	printf("A\n");
	//debug
	build_tree(fcode);
	//debug
	printf("B\n");
	//debug
	fclose(fcode);

	/*decode*/
	fopen_s(&fin, INPUT_FILE, "rb");
	fopen_s(&fout, OUTPUT_FILE, "wb");
	//debug
	printf("C\n");
	//debug
	decode_bin(fin, fout);
	//debug
	printf("D\n");
	//debug
	fclose(fin);
	fclose(fout);
	/*cleanup*/
	freetree(root);
	return 0;
}

