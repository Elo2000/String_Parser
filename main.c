#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//maximum length of a sentence/line
#define MAX_SENTENCE 510
//filename for storing the words
#define LIST_FILE "list.txt"

struct list {
    char * word;
    struct list * next;
};

//List head and tail
static struct list * head = NULL, * tail = NULL;

//Add a word to list
static int list_add(const char * word){
    struct list * item = (struct list *) malloc(sizeof(struct list));
    if(item == NULL){
        perror("malloc");
        return -1;
    }
    //copy word to item
    item->word = strdup(word);
    if(item->word == NULL){
        perror("strdup");
        return -1;
    }
    item->next = NULL;
    //link item into list
    if(head == NULL){
        head = tail = item;
    }
    else{
        tail->next = item;
        tail = item;
    }
    return 0;
}

//Load the words from file to list.txt
static int load_list(const char * filename){
    char word[MAX_SENTENCE+1];
    //open the file in read mode
    FILE * fp = fopen(filename, "r");
    if(fp == NULL){
        return 0;
    }
    //read each line from file
    while(fgets(word, MAX_SENTENCE, fp) != NULL){
        //remove any newlines
        size_t len = strlen(word);
        if(word[len-1] == '\r') word[--len] = '\0';
        if(word[len-1] == '\n') word[--len] = '\0';

        //add the word to list
        list_add(word);
    }
    fclose(fp);
    return 0;
}
//Save the words from list to file
static int save_list(const char * filename){
    //open the file for writing
    FILE * fp = fopen(filename, "w");
    if(fp == NULL){
        return 0;
    }
    //read each line from file
    struct list * item = head;
    while(item){
        fprintf(fp, "%s\n", item->word);
        item = item->next;
    }
    fclose(fp);
    return 0;
}
//Release list memory
static void free_list(){
    struct list * item = head;  //start from head
    while(item){  //for each item in list
        //take pointer to next, because we free item
        struct list * next = item->next;
        //release the memory for this item
        free(item->word);
        free(item);
        //move to next
        item = next;
    }
}
//Print the list with numbers
static void print_list(){
    int i = 0;
    struct list * item = head;
    while(item){
        printf("%d: %s\n", i++, item->word);
        item = item->next;
    }
}
int main(void){
    int stop = 0;
    char sentence[MAX_SENTENCE+1];
    //load wordlist from file
    load_list(LIST_FILE);
    while(!stop){
        printf("Enter String, or \"exit\" to end the program:\n");
        if(fgets(sentence, MAX_SENTENCE, stdin) == NULL){
            break;
        }
        //remove newlines from sentence
        size_t len = strlen(sentence);
        if(sentence[len-1] == '\r') sentence[--len] = '\0';
        if(sentence[len-1] == '\n') sentence[--len] = '\0';

        int words = 0, chars = 0; //counters for words and characters
        //tokenize the sentence
        char * word = strtok(sentence, " ");
        while(word){
            if(strcmp(word, "exit") == 0){
                stop = 1;
                break;
            }
            else if(strcmp(word, "history") == 0){
                print_list();
            }
            else{
                //increment counters
                chars += strlen(word);
                words += 1;
                //add word to list
                list_add(word);
            }
            //move to next word
            word = strtok(NULL, " ");
        }
        if(words > 0){
            printf("%d words\n%d chars\n", words, chars);
        }
    }
    save_list(LIST_FILE);
    free_list();
    return 0;
}