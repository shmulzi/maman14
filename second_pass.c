#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void update_word_at_address(int address, int word);
int get_from_mllist(char *label);
void update_entries();
void update_externs();
int calc_dist(char *dist_param);

typedef struct lbprl{
	int address;
	char *label;
	struct lbprl *next;
} lbpr_list;

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

lbpr_list *main_lbpr_list;

void add_to_lbpr_list(int address, char *label)
{
	main_lbpr_list = lbpr_append(address, label, main_lbpr_list);
}

void update_paramter_label_addresses()
{
	lbpr_list *ptr;
	for(ptr = main_lbpr_list; ptr != NULL; ptr = ptr->next){
		update_word_at_address(ptr->address, get_from_mllist(ptr->label));
	}
	update_entries();
	update_externs();
}
