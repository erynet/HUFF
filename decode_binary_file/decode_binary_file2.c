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
	unsigned char	strcode[MAX_LEN];
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
		}
		strcode[bitstream_index] = 0;

		curr = root;
		len = strlen(strcode);
		for (j = 0; j < len; j++)
		{
			/*TODO: create the tree as you go*/
			if (strcode[j] == '0') {
				//왼쪽에 이미 자식이 있나 없나 검사한다
				if (curr->left == NULL) {
					//왼쪽에 자식이 없다고 판단되면 새로운걸 만들어서 달아둔다
					curr->left = talloc();
				}
				//커서를 자식으로 옮긴다
				curr = curr->left;
			}
			else if (strcode[j] == '1') {
				//오른쪽에 이미 자식이 있나 없나 검사한다
				if (curr->right == NULL) {
					//오른쪽에 자식이 없다고 판단되면 새로운걸 만들어서 달아둔다
					curr->right = talloc();
				}
				//커서를 자식으로 옮긴다
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

void bs_open(FILE* fin) {
	fread_s(&streamLength, sizeof(unsigned long int), sizeof(unsigned long int), 1, fin);
	build_tree(fin);
	remainCountBS = 0;
}

int bs_read(FILE* fin) {
	int result;
	if (remainCountBS == 0) {
		if (feof(fin)) {
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
	
	bs_open(fin);
	while ((c = bs_read(fin)) != EOF)
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
	char OUTPUT_FILE[256];

	if (argc < 3) {
		printf("USAGE : %s file_to_decompress target_file", argv[0]);
		return 0;
	}
	else {
		strcpy_s(INPUT_FILE, _countof(INPUT_FILE), argv[1]);
		strcpy_s(OUTPUT_FILE, _countof(OUTPUT_FILE), argv[2]);
	}

	FILE* fin;
	FILE* fout;

	/*allocate root*/
	root = talloc();

	/*decode*/
	fopen_s(&fin, INPUT_FILE, "rb");
	fopen_s(&fout, OUTPUT_FILE, "wb");
	decode_bin(fin, fout);
	fclose(fin);
	fclose(fout);

	/*cleanup*/
	freetree(root);
	return 0;
}

