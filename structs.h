typedef struct asList{
	int word;
	int address;
	struct asList *next;
} assembledlist;

typedef struct lbprl{
	int address;
	char *label;
	struct lbprl *next;
} lbpr_list;

typedef struct dstprl{
	char *dist_param;
	int opcode_address;
	int address;
	struct dstprl *next;
} distpr_list;

struct oclist
{
	char *code;
	int numOfParams;
	int val;
	int leg_sop;
	int leg_dop;
	struct oclist *next;
};

typedef struct genlist{
    char *label;
    int address;
    struct genlist *next;
} generic_list;
