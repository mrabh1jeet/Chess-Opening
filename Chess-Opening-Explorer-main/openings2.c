#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node {
    char* ECO;
    char* name;
    char* move;
    int childnum;
    struct node* child[20];
};

int size = 0;

void split(char* arr, char** answer) {
    char* token = strtok(arr, " ");
    int i = 0;
    while (token != NULL) {
        answer[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    if (i > 0) {
        answer[i - 1] = strtok(answer[i - 1], "\n");
    }
    size = i;
}

struct node* createNode(char* ECO, char* name, char* move) {
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    if (!temp) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    for (int i = 0; i < 20; i++) {
        temp->child[i] = NULL;
    }
    temp->childnum = 0;
    temp->ECO = strdup(ECO);
    temp->name = strdup(name);
    temp->move = strdup(move);
    return temp;
}

void display(struct node* head) {
    if (head == NULL) {
        return;
    }
    for (int i = 0; i < head->childnum; i++) {
        printf("ECO: %s; ", head->child[i]->ECO);
        printf("Name: %s; ", strtok(head->child[i]->name, "\t"));
        printf("Move: %s\n", head->child[i]->move);
        display(head->child[i]);
    }
}

void push(char* ECO, char* name, char** move, struct node* head) {
    struct node* temp = head;
    for (int i = 0; i < size; i++) {
        int j;
        for (j = 0; j <= temp->childnum; j++) {
            if (temp->child[j] == NULL) {
                break;
            }
            if (strcmp(temp->child[j]->move, move[i]) == 0) {
                break;
            }
        }
        if (temp->child[j] == NULL) {
            temp->child[j] = createNode(ECO, name, move[i]);
            temp->childnum++;
        }
        temp = temp->child[j];
    }
}

void search(char** move, struct node* head) {
    struct node* temp = head;
    char* ECO = "A00";
    char* name = "Flank Openings";
    for (int i = 0; i < size; i++) {
        int j;
        for (j = 0; j <= temp->childnum; j++) {
            if (temp->child[j] == NULL) {
                break;
            }
            if (strcmp(temp->child[j]->move, move[i]) == 0) {
                break;
            }
        }
        if (temp->child[j] == NULL) {
            break;
        }
        ECO = temp->child[j]->ECO;
        name = temp->child[j]->name;
        temp = temp->child[j];
    }
    printf("ECO is %s\n", ECO);
    printf("Name is %s\n", name);
}

int main() {
    struct node* head = malloc(sizeof(struct node));
    head->ECO = strdup("A00");
    head->name = strdup("Flank Openings");
    char** move = (char**)malloc(20 * sizeof(char*));
    for (int i = 0; i < 20; i++) {
        head->child[i] = NULL;
        move[i] = (char*)malloc(50 * sizeof(char));
    }
    head->childnum = 0;

    FILE* fp = fopen("openings1.txt", "r");
    if (fp == NULL) {
        printf("Can't open file\n");
        return 1;
    } else {
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), fp)) {
            char* token1 = strtok(buffer, ";");
            char* token2 = strtok(NULL, ";");
            char* token3 = strtok(NULL, ";");
            split(token3, move);
            push(token1, token2, move, head);
        }
        fclose(fp);
    }

    char** answer = (char**)malloc(20 * sizeof(char*));
    for (int i = 0; i < 20; i++) {
        answer[i] = (char*)malloc(50 * sizeof(char));
    }
    char input[250];
    printf("\nEnter your move: ");
    scanf(" %[^\n]s", input);
    split(input, answer);
    search(answer, head);
    return 0;
}
