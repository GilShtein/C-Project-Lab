struct symbolNode 
{
  char symbol[32];
  int value;
  char attribute[20]; /*Change to minimum size later*/
  int entryFlag;
  struct symbolNode *next;
};

struct dataNode
{
  int address;
  int bytesNum; 
  unsigned char binary_db_asciz[1];
  unsigned char binary_dh[2];
  unsigned char binary_dw[4];
  struct dataNode *next;
};

struct instructionNode
{
  int address;
  unsigned char binary[4];
  char instruct[5];
  int rs;
  int rt; 
  int immed;
  int flag;
  int labelArgPos;
  struct instructionNode *next;
};

struct externNode
{
 char labelName[32];
 int address;
 struct externNode *next;
};

typedef struct symbolNode symbolRow;

typedef struct dataNode dataRow;

typedef struct instructionNode instructionRow;

typedef struct externNode externRow;

