#pragma once

//#define AKINATOR_SPEAK

#ifdef AKINATOR_SPEAK
    #define TX_USE_SPEAK
    const int MAX_SPEAK_LENGTH = 1000;
#endif // AKINATOR_SPEAK

#include <TXLib.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <locale.h>
#include <sys\stat.h>

typedef int64_t int_t;

const int USERS_INPUT_MAX_LENGTH = 30;
const int TEXT_MAX_LENGTH = 100;

/*!
node_type has 2 values:
if it is 'q' than Node has a question in it
if it is 'a' than Node has an answer in it
!*/
typedef struct Node {
    Node* left;
    Node* right;
    Node* parent;
    char* text;
    char type;
} Node;

typedef struct Tree {
    int_t size;
    int_t questions_count;
    int_t answers_count;
    Node* root;
} Tree;

typedef struct Sign {
    char* sign;
    char type;
} Sign;

typedef struct Defenition {
    char* word;
    Sign* definitions;
    int def_count;
} Defenition;

struct Str {
    char* Str = nullptr;
    int length = 0;
};

struct Text_info {
    int text_size = 0;
    int lines_count = 0;
    Str* lines = nullptr;
    char* buffer = nullptr;
};

enum ExitCodes {
    EXIT_OK = 0,
    BAD_INGAME_INPUT = 1,
    KEY_NOT_FOUND = 2,
};


Node* NodeConstruct(char* text, char type);
Node* MakeTextNode(char type);
ExitCodes NodeDestruct(Node* node);
ExitCodes DestructNodes(Node* node);
ExitCodes TreeConstruct(Tree* tree);
ExitCodes TreeDestruct(Tree** tree);
ExitCodes TreeClear(Tree* tree);
ExitCodes AddWord(Tree* tree, Node* insert_node);
ExitCodes MakeFirstQuestion(Tree* tree);
ExitCodes RunGame(Tree* tree);
ExitCodes RunComparison(Tree* tree);
ExitCodes RunDefinition(Tree* tree);
ExitCodes RunDump(Tree* tree);
ExitCodes MakeData(Tree* tree);
ExitCodes SaveData(Tree* tree);
ExitCodes Correct(Tree* tree);
ExitCodes ProceedTasks(Tree* tree);
void PrintInfo(void);
void TreeVertexPrint(Node* node, FILE* fp);
void TreeEdgesPrint(Node* node, FILE* fp);
void TreeVisualDump(Node* root, const char* file_name);

Node* FindWord(Tree* tree, char* key_word);
ExitCodes RunDefinition(Tree* tree);
void DefenitionConstruct(Tree* tree, Defenition* defenition);
void DefenitionDestruct(Defenition* defenition);
ExitCodes GetDefenition(Tree* tree, Defenition* defenition, char* key_word);
void PrintDefenition(Defenition* defenition);
void PrintDifference(Defenition* defenition1, Defenition* defenition2);
void PrintAndSpeak(char* format, ...);
void PrintNodes(Node* node, FILE* fp);
void PutText(Tree* tree, Node* node, char* text);
void MakeNodes(Tree* tree, Node* node, Text_info* text, int pos);
void PrintDefWord(Defenition* defenition, int pos);
int GetUsersAns(char* users_command);
