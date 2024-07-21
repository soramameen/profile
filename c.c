#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN  69
#define MAX 1024
#define MAX_PROFILES 10000

int subst(char *str, char c1, char c2) {
    int count = 0;
    while (1) {
        if (*str == c1) {
            *str = c2;
            count++;
        }
        str++;
        if (*str == '\0') {
            return count;
        }
    }
}

int get_line(char *line) {
    if (fgets(line, 1024, stdin) != NULL) {
        subst(line, '\n', '\0');
        return 1;
    } else {
        return 0;
    }
}

int split(char *str, char *ret[], char sep, int max) {
    int i = 0;
    ret[i] = str;
    while (*str ) {
        if (*str == sep) {
            *str = '\0';
            ret[++i] = str + 1;
            
        } else if (*str == '\0') {
            return i + 1;
        }
        str++;
    }
    return i + 1;
}

struct date {
    int y;
    int m;
    int d;
};

struct profile {
    int id;
    char name[MAX_STR_LEN + 1];
    struct date birthday;
    char address[MAX_STR_LEN + 1];
    char *comment;
};

struct date *new_date(struct date *d, char *str) {
    char *ptr[3];

    if (split(str, ptr, '-', 3) != 3)
        return NULL;

    d->y = atoi(ptr[0]);
    d->m = atoi(ptr[1]);
    d->d = atoi(ptr[2]);

    return d;
}

struct profile *new_profile(struct profile *p, char *csv) {
    char *ptr[5];

    if (split(csv, ptr, ',', 5) != 5 )
        return NULL;

    p->id = atoi(ptr[0]);
    strncpy(p->name, ptr[1], MAX_STR_LEN);
    p->name[MAX_STR_LEN] = '\0';

    if (new_date(&p->birthday, ptr[2]) == NULL)
        return NULL;

    strncpy(p->address, ptr[3], MAX_STR_LEN);
    p->address[MAX_STR_LEN] = '\0';

    p->comment = (char *)malloc(sizeof(char) * (strlen(ptr[4]) + 1));
    if (p->comment == NULL) {
    fprintf(stderr, "Memory allocation failed for comment.\n");
    return NULL;
}
    strcpy(p->comment, ptr[4]);

    return p;
}

struct profile profile_data_store[MAX_PROFILES];
int profile_data_nitems = 0;
void swap_profile(struct profile *p1, struct profile *p2)
{
    struct profile tmp;

    tmp = *p2;
    *p2 = *p1;
    *p1 = tmp;
}
void print_profile(struct profile p) {
    printf("ID: %d\n", p.id);
    printf("Name: %s\n", p.name);
    printf("Birthday: %d-%d-%d\n", p.birthday.y, p.birthday.m, p.birthday.d);
    printf("Address: %s\n", p.address);
    printf("Comment: %s\n", p.comment);
}
void sort_id(struct profile *p,struct profile *q){
    char str_p[MAX] ="";
    char str_q[MAX]="";
    sprintf(str_p,"%d",p->id);
    sprintf(str_q,"%d",q->id);
    if (strcmp(str_p,str_q)>0){
        swap_profile(p,q);
    }
}
void sort_birthday(struct profile *p,struct profile *q){
    char str_p[MAX] ="";
    char str_q[MAX]="";
    sprintf(str_p,"%04d,%02d,%02d",p->birthday.y,p->birthday.m,p->birthday.d);
    sprintf(str_q,"%04d,%02d,%02d",q->birthday.y,q->birthday.m,q->birthday.d);
    
    if (strcmp(str_p,str_q)>0){
        swap_profile(p,q);
    }
}
void sort_name(struct profile *p,struct profile *q){
    char str_p[MAX] ="";
    char str_q[MAX]="";
    sprintf(str_p,"%s",p->name);
    sprintf(str_q,"%s",q->name);
    if (strcmp(str_p,str_q)>0){
        swap_profile(p,q);
    }
}
void sort_add(struct profile *p,struct profile *q){
    char str_p[MAX] ="";
    char str_q[MAX]="";
    sprintf(str_p,"%s",p->address);
    sprintf(str_q,"%s",q->address);
    if (strcmp(str_p,str_q)>0){
        swap_profile(p,q);
    }
}
void sort_comm(struct profile *p,struct profile *q){
    char str_p[MAX] ="";
    char str_q[MAX]="";
    sprintf(str_p,"%s",p->comment);
    sprintf(str_q,"%s",q->comment);
    if (strcmp(str_p,str_q)>0){
        swap_profile(p,q);
    }
}


void cmd_quit() {
    exit(0);
}

void cmd_check() {
    printf("%d profile(s)\n", profile_data_nitems);
    
}

void cmd_print(int count) {
    int j = 0;

    if (count == 0) {
        count = profile_data_nitems;
    }
    if (count < 0) {
        count = -count;
        if (count > profile_data_nitems) {
            count = profile_data_nitems;
        }
        for (j = profile_data_nitems - count; j < profile_data_nitems; ++j) {
            printf("Id    : %d\n", profile_data_store[j].id);
            printf("Name  : %s\n", profile_data_store[j].name);
            printf("Birth : %04d-%02d-%02d\n", profile_data_store[j].birthday.y, profile_data_store[j].birthday.m, profile_data_store[j].birthday.d);
            printf("Addr. : %s\n", profile_data_store[j].address);
            printf("Comm. : %s\n\n", profile_data_store[j].comment);
        }
    } else {
        if (count > profile_data_nitems) {
            count = profile_data_nitems;
        }
        for (j = 0; j < count; ++j) {
            printf("Id    : %d\n", profile_data_store[j].id);
            printf("Name  : %s\n", profile_data_store[j].name);
            printf("Birth : %04d-%02d-%02d\n", profile_data_store[j].birthday.y, profile_data_store[j].birthday.m, profile_data_store[j].birthday.d);
            printf("Addr. : %s\n", profile_data_store[j].address);
            printf("Comm. : %s\n\n", profile_data_store[j].comment);
        }
    }
}



void cmd_read(char *filename) {
    // fprintf(stderr, "Read (%%R) command is invoked with arg: '%s'\n", filename);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file for reading");
        return;
    }

    char line[MAX + 1];
    while (fgets(line, sizeof(line), file)) {
        subst(line, '\n', '\0');
        if (profile_data_nitems < MAX_PROFILES) {
            struct profile new_profile_data;
            if (new_profile(&new_profile_data, line) != NULL) {
                profile_data_store[profile_data_nitems] = new_profile_data;
                profile_data_nitems++;
            } else {
                fprintf(stderr, "Failed to create profile from line: %s\n", line);
            }
        } else {
            fprintf(stderr, "Maximum number of profiles reached.\n");
            break;
        }
    }

    fclose(file);
    printf("Data read from %s successfully.\n", filename);
}

void cmd_write(char *filename) {
    // fprintf(stderr, "Write (%%W) command is invoked with arg: '%s'\n", filename);
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file for writing");
        return;
    }

    for (int i = 0; i < profile_data_nitems; i++) {
        struct profile *p = &profile_data_store[i];
        fprintf(file, "%d,%s,%04d-%02d-%02d,%s,%s\n",
                p->id,
                p->name,
                p->birthday.y,
                p->birthday.m,
                p->birthday.d,
                p->address,
                p->comment);
    }

    fclose(file);
    printf("Data written to %s successfully.\n", filename);
}

void cmd_find(char *word) {
    // fprintf(stderr, "Find (%%F) command is invoked with arg: '%s'\n", word);
    int i;
    struct profile *p ;
    

    for(i=0; i<sizeof(profile_data_store)/sizeof(profile_data_store[0]); i++) {

        p = &profile_data_store[i];
        char s[20];
        sprintf(s, "%d", p->id);
            if (strcmp(s, word) == 0){
                
            printf("idが一致しました\n");
            printf("Id    : %d\n", profile_data_store[i].id);
            printf("Name  : %s\n", profile_data_store[i].name);
            printf("Birth : %04d-%02d-%02d\n", profile_data_store[i].birthday.y, profile_data_store[i].birthday.m, profile_data_store[i].birthday.d);
            printf("Addr. : %s\n", profile_data_store[i].address);
            printf("Comm. : %s\n\n", profile_data_store[i].comment);
            continue;
        }

        
        if (p->comment && (!strcmp(word, p->comment) || !strcmp(word, p->name) || !strcmp(word, p->address))) {
            printf("名前,住所,コメントのいづれかがヒットしました\n");
            printf("Id    : %d\n", profile_data_store[i].id);
            printf("Name  : %s\n", profile_data_store[i].name);
            printf("Birth : %04d-%02d-%02d\n", profile_data_store[i].birthday.y, profile_data_store[i].birthday.m, profile_data_store[i].birthday.d);
            printf("Addr. : %s\n", profile_data_store[i].address);
            printf("Comm. : %s\n\n", profile_data_store[i].comment);
            continue;
        }

        
        char birth[20];
    sprintf(birth, "%04d-%02d-%02d", p->birthday.y, p->birthday.m, p->birthday.d);
    if (!strcmp(word, birth)) {
        printf("誕生日が一致しました\n");
        printf("Id    : %d\n", p->id);
        printf("Name  : %s\n", p->name);
        printf("Birth : %04d-%02d-%02d\n", p->birthday.y, p->birthday.m, p->birthday.d);
        printf("Addr. : %s\n", p->address);
        printf("Comm. : %s\n\n", p->comment);
    }
    }
}

void cmd_sort(int column) {
    // fprintf(stderr, "Sort (%%S) command is invoked with arg: '%d'\n", column);
    int i = 0, j = 0;
 
    switch(column){

    case 1:
            for (i = 0; i < profile_data_nitems; i++) {
            for (j = 0; j < profile_data_nitems - 1; j++) {
                sort_id(&profile_data_store[j],&profile_data_store[j+1]);
                }
            }
            break;
    case 2:
        int i = 0, j = 0;
            for (i = 0; i < profile_data_nitems; i++) {
            for (j = 0; j < profile_data_nitems - 1; j++) {
                sort_name(&profile_data_store[j],&profile_data_store[j+1]);
                }
            }
            break;
    case 3:
        
            for (i = 0; i < profile_data_nitems; i++) {
            for (j = 0; j < profile_data_nitems - 1; j++) {
                sort_birthday(&profile_data_store[j],&profile_data_store[j+1]);
                }
            }
            break;
    case 4:
            for (i = 0; i < profile_data_nitems; i++) {
            for (j = 0; j < profile_data_nitems - 1; j++) {
                sort_add(&profile_data_store[j],&profile_data_store[j+1]);
                }
            }
            break;
    case 5:
            for (i = 0; i < profile_data_nitems; i++) {
            for (j = 0; j < profile_data_nitems - 1; j++) {
                sort_comm(&profile_data_store[j],&profile_data_store[j+1]);
                }
            }
        break;
    }
}
void cmd_delete(int id){
    int i=0, j=0;
    for (i = 0; i <profile_data_nitems; i++){
        if (profile_data_store[i].id == id){
            free(profile_data_store[i].comment);
            printf("Memory was freed successfully\n");
            for(j = i; j < profile_data_nitems; j++){
                profile_data_store[j] = profile_data_store[j+1];
            }
            profile_data_nitems--;
            i--;
        }

    }
   

}

int binary_search(int id) {
    int left = 0;
    int right = profile_data_nitems - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

     
        if (profile_data_store[mid].id == id) {
            return mid;
        }
     
        else if (profile_data_store[mid].id < id) {
            left = mid + 1;
        }
 
        else {
            right = mid - 1;
        }
    }

    return -1;
}


int is_sorted() {
    int i = 0;
    for (i = 0; i < profile_data_nitems-1; i++){
        if(profile_data_store[i].id > profile_data_store[i+1].id){
            printf("このデータはソートされていません\n");
            return 0;
        }
      
    }
    return 1;
}
void binary(int id){
    printf("binarysearchを行います\n");
    if (is_sorted()) {
        int result = 0;
        result = binary_search(id);
        if(result == -1) {
            printf("このidを持つprofileは存在しません\n");
        }
        else{
            printf("二分探索法によってこのidを持つものが見つかりました\n");
            print_profile(profile_data_store[result]);
        }
        
    }
}

void exec_command(char cmd, char *param) {
    switch (cmd) {
        case 'Q': cmd_quit(); break;
        case 'C': cmd_check(); break;
        case 'P': cmd_print(atoi(param)); break;
        case 'R': cmd_read(param); break;
        case 'W': cmd_write(param); break;
        case 'F': cmd_find(param); break;
        case 'S': cmd_sort(atoi(param)); break;
        case 'D': cmd_delete(atoi(param)); break;
        case 'B': binary(atoi(param)); break;

        default:
            fprintf(stderr, "Invalid command %c: ignored.\n", cmd);
            break;
    }

}

void parse_line(char *line) {
    char cmd;
    char *param;

    if (line[0] == '%') {
        cmd = line[1];
        param = line + 3;
        exec_command(cmd, param);
    } else {
        if (profile_data_nitems < MAX_PROFILES) {
            struct profile new_profile_data;
            if (new_profile(&new_profile_data, line) != NULL) {
                profile_data_store[profile_data_nitems] = new_profile_data;
                profile_data_nitems++;
            } else {
                fprintf(stderr, "Failed to create profile.\n");
            }
        } else {
            fprintf(stderr, "Maximum number of profiles reached.\n");
        }
    }
}
void free_profiles() {/*メモリ解放用関数*/
    int i = 0;
    for (i = 0; i < profile_data_nitems; i++) {
        free(profile_data_store[i].comment);
    }
}
int main() {
    char line[MAX + 1];
    while (get_line(line)) {
        parse_line(line);
    }
    free_profiles();
    return 0;
}
