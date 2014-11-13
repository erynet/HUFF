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
	int	symbol;
	unsigned char	strcode[MAX_LEN];
	int		items_read;
	int		i, len;
	struct	tnode* curr = NULL;
	//struct  tnode* temp = NULL;

	while (!feof(fp))
	{
		items_read = fscanf_s(fp, "%d %s\n", &symbol, strcode, _countof(strcode));
		if (items_read != 2) break;
		curr = root;
		len = strlen(strcode);
		for (i = 0; i<len; i++)
		{
			/*TODO: create the tree as you go*/
			if (strcode[i] == '0') {
				//왼쪽에 이미 자식이 있나 없나 검사한다
				if (curr->left == NULL) {
					//왼쪽에 자식이 없다고 판단되면 새로운걸 만들어서 달아둔다
					curr->left = talloc();
				}
				//커서를 자식으로 옮긴다
				curr = curr->left;
			}
			else if (strcode[i] == '1') {
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
		curr->symbol = symbol;
		//printf("\n");
		//printf("inserted symbol:%c\n", symbol);
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
			//printf("%c", curr->symbol);
			fprintf(fout, "%c", curr->symbol);
			curr = root;
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
	char OUT_FILE[256];

	if (argc > 1) {
		if (atoi(argv[1]) == 1) {
			strcpy_s(INPUT_FILE, _countof(INPUT_FILE), "enc_28233_my.txt");
			strcpy_s(CODE_FILE, _countof(INPUT_FILE), "code_28233_my.txt");
			strcpy_s(OUT_FILE, _countof(INPUT_FILE), "dec_28233_my.txt");
		}
		else if (atoi(argv[1]) == 2) {
			strcpy_s(INPUT_FILE, _countof(INPUT_FILE), "enc_1342_my.txt");
			strcpy_s(CODE_FILE, _countof(INPUT_FILE), "code_1342_my.txt");
			strcpy_s(OUT_FILE, _countof(INPUT_FILE), "dec_1342_my.txt");
		}
	}

	if (argc == 1) {
		printf("Input encoded file name: ");
		scanf_s("%s", INPUT_FILE);
		printf("Input code file name: ");
		scanf_s("%s", CODE_FILE);
		printf("Input decoded file name: ");
		scanf_s("%s", OUT_FILE);
	}

	FILE* fin;
	FILE* fcode;
	FILE* fout;
	
	/*allocate root*/
	root = talloc();
	fopen_s(&fcode, CODE_FILE, "r");
	/*build tree*/
	build_tree(fcode);
	fclose(fcode);

	/*decode*/
	fopen_s(&fin, INPUT_FILE, "r");
	fopen_s(&fout, OUT_FILE, "w");
	decode(fin, fout);
	fclose(fin);
	fclose(fout);
	/*cleanup*/
	freetree(root);
	return 0;
}

