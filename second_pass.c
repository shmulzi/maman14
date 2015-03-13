#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*main.c*/
void update_word_at_address(int address, int word);

/*label.c*/
int get_from_mllist(char *label);

/*tran_directive.c*/
int isextern(char *label);
void update_entries();
void update_externs();

/*tran_op.c*/
int calc_dist(char *dist_param, int opcode_address);

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

lbpr_list *lbpr_alloc(void)
{
    return (lbpr_list *)malloc(sizeof(lbpr_list));
}

lbpr_list *lbpr_append(int address, char *label, lbpr_list *lbpr)
{
    if (lbpr == NULL) {
        lbpr = lbpr_alloc();
        lbpr->address = address;
        lbpr->label = label;
        lbpr->next = NULL;
    } else {
        lbpr->next = lbpr_append(address,label,lbpr->next);
    }
    return lbpr;
}

distpr_list *distpr_alloc(void)
{
    return (distpr_list *)malloc(sizeof(distpr_list));
}

distpr_list *distpr_append(char *dist_param, int opcode_address, int address, distpr_list *distpr)
{
    if (distpr == NULL) {
        distpr = distpr_alloc();
        distpr->dist_param = dist_param;
        distpr->opcode_address = opcode_address;
        distpr->address = address;
        distpr->next = NULL;
    } else {
        distpr->next = distpr_append(dist_param,opcode_address,address,distpr->next);
    }
    return distpr;
}

lbpr_list *main_lbpr_list;
distpr_list *main_distpr_list;

void add_to_lbpr_list(int address, char *label)
{
	main_lbpr_list = lbpr_append(address, label, main_lbpr_list);
}

void add_to_distpr_list(char *dist_param, int opcode_address, int address)
{
	main_distpr_list = distpr_append(dist_param,opcode_address,address,main_distpr_list);
}

void update_label_addresses()
{
	lbpr_list *ptr;
	for(ptr = main_lbpr_list; ptr != NULL; ptr = ptr->next){
		if(isextern(ptr->label)){
			update_word_at_address(ptr->address, ((get_from_mllist(ptr->label) << 2) | 1));
		} else {
			update_word_at_address(ptr->address, ((get_from_mllist(ptr->label) << 2) | 2));
		}
	}
}

void update_dist_addresses()
{
	distpr_list *ptr;
	for(ptr = main_distpr_list; ptr != NULL; ptr = ptr->next){
		int dist = calc_dist(ptr->dist_param,ptr->opcode_address);
		update_word_at_address(ptr->address, dist);
	}
}

lbpr_list *get_main_lbpr_list()
{
	return main_lbpr_list;
}

void update_paramter_label_addresses()
{
	update_label_addresses();
	update_dist_addresses();
	update_entries();
	update_externs();
}

void print_lbpr()
{
	lbpr_list *ptr;
	printf("--- List of LBPR:\n");
	for(ptr = main_lbpr_list; ptr != NULL; ptr = ptr->next){
		printf("lbpr - %s           Address - %X\n",ptr->label,ptr->address);
	}
	printf("--- End list of LBPR.\n");
}
