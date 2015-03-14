/*structs.h - contains all of the project structs so this file can be added in an include instead of declaring them for each file that uses them*/

/*assembledList - a struct list that will contain the main memory for the assembler, containing an integer that will be the final assembled word, and its corresponding address*/
typedef struct asList{
	int word;
	int address;
	struct asList *next;
} assembledlist;

/*lbpr_list - a struct list that contains every label name and the address it was mentioned on, this will be used to update label addresses on the second passthrough*/
typedef struct lbprl{
	int address;
	char *label;
	struct lbprl *next;
} lbpr_list;

/*distpr_list - a struct list that contains every distance parameter tha has labels in them that arent addresssed yet - in the second passthrough this parameter will be recalculated*/
typedef struct dstprl{
	char *dist_param;
	int opcode_address;
	int address;
	struct dstprl *next;
} distpr_list;

/*struct oclist - a struct list that contains the predetermined opcodes and their details */
struct oclist
{
	char *code;
	int numOfParams;
	int val;
	int leg_sop;
	int leg_dop;
	struct oclist *next;
};

/*generic_list - a struct list that contains entires and externs and their addresses*/
typedef struct genlist{
    char *label;
    int address;
    struct genlist *next;
} generic_list;
