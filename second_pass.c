#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "structs.h"

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

/*add_to_lbpr_list - adds the label and address to the main lbpr list*/
void add_to_lbpr_list(int address, char *label)
{
	main_lbpr_list = lbpr_append(address, label, main_lbpr_list);
}

/*add_to_distpr_list - adds the distance param, opcode address and adress to the main distpr list*/
void add_to_distpr_list(char *dist_param, int opcode_address, int address)
{
	main_distpr_list = distpr_append(dist_param,opcode_address,address,main_distpr_list);
}

/*update_label_addresses - goes through the main lbpr list (which contains all the updated addresses of
 * labels and externs called before they were defined) and updates the actual addresses in the main memory
 * with the real address per label (with era 1 for externs and era 2 for labels)*/
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

/*update_dist_addresses - goes through the main distpr list (which contains distance paramaters that have labels
 * and externs that are not yet addressed in the main memory) updates their addresses and then recalulates the distance
 * accordingly and replaces it in the correct place in the main memory */
void update_dist_addresses()
{
	distpr_list *ptr;
	for(ptr = main_distpr_list; ptr != NULL; ptr = ptr->next){
		int dist = calc_dist(ptr->dist_param,ptr->opcode_address);
		update_word_at_address(ptr->address, dist);
	}
}

/*get_main_lbpr_list - returns the main lbpr list*/
lbpr_list *get_main_lbpr_list()
{
	return main_lbpr_list;
}

/*update_paramter_label_addresses - updates the main memory for parameters that haven't been addressed yet*/
void update_paramter_label_addresses()
{
	update_label_addresses();
	update_dist_addresses();
	update_entries();
	update_externs();
}
