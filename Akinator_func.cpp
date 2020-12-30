#include "Akinator_header.h"
#include "ReadFile.cpp"

Node* NodeConstruct(char* text, char type) {
    Node* new_node = (Node*) calloc(1, sizeof(Node));
    if (new_node == nullptr) {
        fprintf(stderr, "Can not allocate new Node\n");
        return nullptr;
    }

    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = nullptr;

    new_node->text = (char*) calloc(TEXT_MAX_LENGTH, sizeof(char));

    new_node->type = type;
    strcpy(new_node->text, text);

    return new_node;
}

Node* NodeConstructEmpty() {
    Node* new_node = (Node*) calloc(1, sizeof(Node));
    if (new_node == nullptr) {
        fprintf(stderr, "Can not allocate new Node\n");
        return nullptr;
    }

    new_node->left = nullptr;
    new_node->right = nullptr;
    new_node->parent = nullptr;

    return new_node;
}

ExitCodes NodeDestruct(Node* node) {
    assert(node != nullptr);

    if (node->text != nullptr) {
        free(node->text);
    }

    EXIT_OK;
}

ExitCodes TreeConstruct(Tree* tree) {
    assert(tree != nullptr);

    tree->root = nullptr;
    tree->size = 0;
    tree->questions_count = 0;
    tree->answers_count = 0;

    return EXIT_OK;
}

ExitCodes DestructNodes(Node* node) {
    assert(node != nullptr);

    if (node->left != nullptr) {
        return DestructNodes(node->left);
    }
    if (node->right != nullptr) {
        return DestructNodes(node->right);
    }
    NodeDestruct(node);
    free(node);

    return EXIT_OK;
}

ExitCodes TreeDestruct(Tree* tree) {
    assert(tree != nullptr);

    TreeClear(tree);

    tree->questions_count = -1;
    tree->answers_count = -1;
    tree->size = -1;

    return EXIT_OK;
}

ExitCodes TreeClear(Tree* tree) {
    assert(tree != nullptr);

    if (tree->root != nullptr) {
        DestructNodes(tree->root);
    }

    tree->root = nullptr;
    tree->questions_count = 0;
    tree->answers_count = 0;
    tree->size = 0;

    return EXIT_OK;
}

ExitCodes MakeData(Tree* tree) {
    assert(tree != nullptr);

    struct Text_info text = {};

    char* file_name = (char*) calloc(100, sizeof(char));
    assert(file_name != nullptr);
    PrintAndSpeak("Пожалуйста введите имя файла для загрузки\n");
    scanf("%s", file_name);

    FILE* fp = fopen(file_name, "r");
    if (fp == nullptr) {
        PrintAndSpeak("Файл ненайден, проверьте корректность имени и попробуйте еще раз\n");
        return EXIT_OK;
    }

    ReadTextAndMakeLines(&text, fp);

    if (text.lines_count < 7) {
        PrintAndSpeak("Некорректная база данных, завершаю загрузку");
    }

    TreeClear(tree);
    int pos = 0;
    tree->root = NodeConstructEmpty();
    PutText(tree, tree->root, text.lines[pos++].Str);

    ++pos;
    tree->root->right = NodeConstructEmpty();
    tree->root->right->parent = tree->root;

    MakeNodes(tree, tree->root->right, &text, pos);

    return EXIT_OK;
}

void MakeNodes(Tree* tree, Node* node, Text_info* text, int pos) {
    assert(tree != nullptr);
    assert(node != nullptr);
    assert(text != nullptr);

    PutText(tree, node, text->lines[pos++].Str);

    if (strchr(text->lines[pos].Str, '{') != nullptr) {
        ++pos;
        node->right = NodeConstructEmpty();
        node->right->parent = node;
        MakeNodes(tree, node->right, text, pos);
        return;
    }

    while (strchr(text->lines[pos].Str, '}') != nullptr) {
        ++pos;
        node = node->parent;
    }

    if (pos < text->lines_count - 1) {
        ++pos;
        node->left = NodeConstructEmpty();
        node->left->parent = node;
        MakeNodes(tree, node->left, text, pos);
    }
}

void PutText(Tree* tree, Node* node, char* text) {
    assert(node != nullptr);

    node->text = (char*) calloc(TEXT_MAX_LENGTH, sizeof(char));
    strcpy(node->text, strchr(text, '"') + 1);
    char* tmp  = strchr(node->text, '"');
    *tmp = '\0';
    ++tree->size;
}

ExitCodes SaveData(Tree* tree) {
    assert(tree != nullptr);

    char* file_name = (char*) calloc(100, sizeof(char));
    assert(file_name != nullptr);
    PrintAndSpeak("Пожалуйста введите имя файла для сохранения\n");
    scanf("%s", file_name);

    FILE* fp = fopen(file_name, "w");
    assert(fp != nullptr);

    PrintNodes(tree->root, fp);

    free(file_name);
    fclose(fp);

    return EXIT_OK;
}

void PrintNodes(Node* node, FILE* fp) {
    assert(node != nullptr);
    assert(fp != nullptr);

    fprintf(fp, "\"%s\"\n", node->text);

    if (node->right != nullptr) {
        fprintf(fp, "{\n");
        PrintNodes(node->right, fp);
        fprintf(fp, "}\n");
    }
    if (node->left != nullptr) {
        fprintf(fp, "{\n");
        PrintNodes(node->left, fp);
        fprintf(fp, "}\n");
    }
}

ExitCodes Correct(Tree* tree) {
    assert(tree != nullptr);

    PrintAndSpeak("А в Correct() ничего нет, это пока заглушка((\n");

    return EXIT_OK;
}

Node* MakeTextNode(char type) {
    char* text = (char*) calloc(TEXT_MAX_LENGTH, sizeof(char));
    assert(text != nullptr);
    //text = gets(text);
    scanf("%s", text);
    assert(text != nullptr);
    //printf("current text: %s", text);
    Node* new_node = NodeConstruct(text, type);

    return new_node;
}

ExitCodes AddWord(Tree* tree, Node* insert_node) {
    assert(tree != nullptr);
    assert(insert_node != nullptr);

    PrintAndSpeak("Пожалуйста введите ваше загаданное слово\n");
    Node* new_word_node = MakeTextNode('a');

    PrintAndSpeak("Чем %s отличается от %s\nОно...", new_word_node->text, insert_node->text); //загаданное слово подвешиваем в "да"
    Node* new_question_node = MakeTextNode('q');

    Node* parent_tmp = insert_node->parent;

    if (parent_tmp->left == insert_node) {
        parent_tmp->left = new_question_node;
    }
    else {
        parent_tmp->right = new_question_node;
    }
    new_question_node->parent = parent_tmp;

    new_question_node->right = new_word_node;
    new_word_node->parent = new_question_node;

    new_question_node->left = insert_node;
    insert_node->parent = new_question_node;

    PrintAndSpeak("Теперь я знаю что такое %s, вы меня не обманите!\n", new_word_node->text);

    tree->questions_count += 1;
    tree->answers_count += 1;
    tree->size += 2;

    return EXIT_OK;
};

ExitCodes MakeFirstQuestion(Tree* tree) {
    assert(tree != nullptr);

    PrintAndSpeak("Пожалуйста введите первое слово для моей базы данных\n");
    Node* new_node_right = MakeTextNode('a');

    PrintAndSpeak("Пожалуйста введите второе слово для моей базы данных\n");
    Node* new_node_left = MakeTextNode('a');

    PrintAndSpeak("Чем %s отличается от %s\nОно...", new_node_right->text, new_node_left->text);
    Node* new_question_node = MakeTextNode('q');

    tree->root = new_question_node;

    new_question_node->right = new_node_right;
    new_node_right->parent = new_question_node;

    new_question_node->left = new_node_left;
    new_node_left->parent = new_question_node;

    PrintAndSpeak("Теперь я знаю чем %s отличается от %s, спасибо за создание новой базы данных ;)\n",
           new_node_right->text, new_node_left->text);

    tree->questions_count += 1;
    tree->answers_count += 2;
    tree->size += 3;

    return EXIT_OK;
};

ExitCodes RunGame(Tree* tree) {
    assert(tree != nullptr);

    char* users_command = (char*) calloc(USERS_INPUT_MAX_LENGTH, sizeof(char));

    assert(users_command != nullptr);
    if (tree->size == 0) {

        PrintAndSpeak("Вы хотели начать новую игру, но моя база слов пуста((\n");
        PrintAndSpeak("Вы можете:\n");
        PrintAndSpeak("Загрузить базу данных и начать игру, введите \"загрузить\"\n");
        PrintAndSpeak("Добавить первый вопрос самостоятельно, введите \"добавить\"\n");
        PrintAndSpeak("Выйти из игры, введите \"выйти\"\n");

        scanf("%s", users_command);

        if (strcmp(users_command, "выйти") == 0) {
            PrintAndSpeak("Очень жаль\n");
            free(users_command);
            return EXIT_OK;
        }
        else if (strcmp(users_command, "загрузить") == 0) {
            MakeData(tree);
        }
        else if (strcmp(users_command, "добавить") == 0) {
            MakeFirstQuestion(tree);
        }
        else {
            PrintAndSpeak("Неизветсная комманда <%s>, завершаю текущую игру\n", users_command);
            free(users_command);
            return BAD_INGAME_INPUT;
        }

        PrintAndSpeak("Теперь вы можете насладиться как я отгадываю, хотите продолжить?\n");

        if (GetUsersAns(users_command) == 1) {
            PrintAndSpeak("Отлично!\n");
        }
        else {
            free(users_command);
            return EXIT_OK;
        }
    }

    PrintAndSpeak("Вы начали новую игру!\nОтвечайте на все вопросы \"да\" или \"нет\", но только честно");
    PrintAndSpeak("(на самом деле как хотите я ведь просто программа)\n");

    Node* current_node = tree->root;

    while (current_node->left != nullptr) {
        PrintAndSpeak("это %s?\n", current_node->text);
        int ans = GetUsersAns(users_command);

        if (ans == 1) {
            current_node = current_node->right;
        }
        else if (ans == 0) {
            current_node = current_node->left;
        }
        else {
            continue;
        }
    }

    PrintAndSpeak("Вы загадали %s?\n", current_node->text);
    scanf("%s", users_command);

    if (strcmp(users_command, "да") == 0) {
        PrintAndSpeak("Ну вот я так и знал\n");
    }
    else if (strcmp(users_command, "нет") == 0) {
        PrintAndSpeak("Вы слишком умны\n");
        PrintAndSpeak("Добавить ваше слово в мою базу данных?\n");

        int ans = GetUsersAns(users_command);
        if (ans == 1) {
            AddWord(tree, current_node);
        }
        else if (ans == 0) {
            PrintAndSpeak("Жаль что такие знания пропадают(\n");
        }
        else {
            free(users_command);
            return BAD_INGAME_INPUT;
        }
    }
    else {
        PrintAndSpeak("Неизветсная комманда <%s>, завершаю текущую игру\n", users_command);
        free(users_command);
        return BAD_INGAME_INPUT;
    }

    free(users_command);
    return EXIT_OK;
}

int GetUsersAns(char* users_command) {
    scanf("%s", users_command);

    if (strcmp(users_command, "да") == 0) {
        return 1;
    }
    else if (strcmp(users_command, "нет") == 0) {
        return 0;
    }
    else {
        PrintAndSpeak("Неизветсная комманда <%s>, введите \"да\" или \"нет\" \n", users_command);
        return -1;
    }
}

ExitCodes RunComparison(Tree* tree) {
    assert(tree != nullptr);

    char* key_word1 = (char*) calloc(TEXT_MAX_LENGTH, sizeof(char));
    char* key_word2 = (char*) calloc(TEXT_MAX_LENGTH, sizeof(char));

    PrintAndSpeak("Ведите первое слово для сравнения: ");
    scanf("%s", key_word1);
    PrintAndSpeak("Ведите второе слово для сравнения: ");
    scanf("%s", key_word2);

    Defenition defenition1 = {};
    DefenitionConstruct(tree, &defenition1);
    Defenition defenition2 = {};
    DefenitionConstruct(tree, &defenition2);

    if (GetDefenition(tree, &defenition1, key_word1) == KEY_NOT_FOUND) {
        return KEY_NOT_FOUND;
    }
    if (GetDefenition(tree, &defenition2, key_word2) == KEY_NOT_FOUND) {
        return KEY_NOT_FOUND;
    }

    PrintDifference(&defenition1, &defenition2);

    free(key_word1);
    free(key_word2);
    DefenitionDestruct(&defenition1);
    DefenitionDestruct(&defenition2);

    return EXIT_OK;
}

Node* TreeEdgesPrint(Node* node, char* key_word) {
    if (node != nullptr) {
        return TreeEdgesPrint(node->left, key_word);
        if (strcmp(node->text, key_word) == 0) {
            return node;
        }
        return TreeEdgesPrint(node->right, key_word);
    }
}

Node* FindWord(Tree* tree, char* key_word) {
    assert(tree != nullptr);

    Node* current = tree->root;
    Node* previous = nullptr;

    while (current != nullptr) {
        if (previous == current->parent) {
            previous = current;
            if (current->left != nullptr) {
                current = current->left;
            }
            else {
                if (strcmp(current->text, key_word) == 0) {
                    return current;
                }

                if (current->right != nullptr) {
                    current = current->right;
                }
                else {
                    current = current->parent;
                }
            }
        }
        else {
            if (previous == current->left) {
                previous = current;
                if (strcmp(current->text, key_word) == 0) {
                    return current;
                }

                if (current->right != nullptr) {
                    current = current->right;
                }
                else {
                    current = current->parent;
                }
            }
            else { // Traversing up the tree from the right.
                previous = current;
                current = current->parent;
            }
        }
    }

    return nullptr;
}

ExitCodes RunDefinition(Tree* tree) {
    assert(tree != 0);

    char* key_word = (char*) calloc(TEXT_MAX_LENGTH, sizeof(char));
    PrintAndSpeak("Ведите слово: ");
    scanf("%s", key_word);

    Defenition defenition = {};
    DefenitionConstruct(tree, &defenition);

    if (GetDefenition(tree, &defenition, key_word) == KEY_NOT_FOUND) {
        return KEY_NOT_FOUND;
    }

    PrintDefenition(&defenition);
    DefenitionDestruct(&defenition);

    return EXIT_OK;
}

void PrintDifference(Defenition* defenition1, Defenition* defenition2) {
    if (defenition1->definitions[defenition1->def_count - 1].type ==
        defenition2->definitions[defenition2->def_count - 1].type) {

        PrintAndSpeak("\nОни оба:");

        int pos1 = defenition1->def_count - 1;
        int pos2 = defenition2->def_count - 1;

        while (pos1 >= 0 && pos2 >= 0 && strcmp(defenition1->definitions[pos1].sign, defenition2->definitions[pos2].sign) == 0 &&
               defenition1->definitions[pos1].type == defenition2->definitions[pos2].type) {
            PrintDefWord(defenition1, pos1);
            --pos1;
            --pos2;
        }

        if (pos1 >= 0) {
            PrintAndSpeak("\nНо %s:", defenition1->word);
        }

        while (pos1 >= 0) {
            PrintDefWord(defenition1, pos1);
            --pos1;
        }

        if (pos2 >= 0) {
            PrintAndSpeak("\nА %s:", defenition2->word);
        }

        while (pos2 >= 0) {
            PrintDefWord(defenition2, pos2);
            --pos2;
        }
    }
    else {
        PrintDefenition(defenition1);
        PrintDefenition(defenition2);
    }
    PrintAndSpeak("\n");

}

void PrintDefenition(Defenition* defenition) {
    PrintAndSpeak("%s: ", defenition->word);

    for(int i = defenition->def_count - 1; i >= 0; --i) {
        PrintDefWord(defenition, i);
    }
}

void DefenitionConstruct(Tree* tree, Defenition* defenition) {
    defenition->word = nullptr;
    defenition->definitions = (Sign*) calloc(tree->size, sizeof(Sign));
    defenition->def_count = 0;
}

void DefenitionDestruct(Defenition* defenition) {
    free(defenition->definitions);
         defenition->definitions = nullptr;
    defenition->def_count = -1;
    defenition->word = nullptr;
}

ExitCodes GetDefenition(Tree* tree, Defenition* defenition, char* key_word) {
    assert(tree != nullptr);
    assert(defenition != nullptr);

    Node* def_node = FindWord(tree, key_word);

    if (def_node == nullptr) {
        PrintAndSpeak("не нашел слово <%s>\n", key_word);
        free(key_word);
        return KEY_NOT_FOUND; //CHANGE
    }

    defenition->word = def_node->text;

    while (def_node->parent != nullptr && def_node != nullptr) {
        if (def_node->parent->left == def_node) {
            defenition->definitions[defenition->def_count].sign = def_node->parent->text;
            defenition->definitions[defenition->def_count].type = 'n';
            defenition->def_count++;
        }
        else if (def_node->parent->right == def_node) {
            defenition->definitions[defenition->def_count].sign = def_node->parent->text;
            defenition->definitions[defenition->def_count].type = 'y';
            defenition->def_count++;
        }
        def_node = def_node->parent;
    }

    return EXIT_OK;
}

void PrintDefWord(Defenition* defenition, int pos) {
    if (defenition->definitions[pos].type == 'y') {
        PrintAndSpeak(" %s,", defenition->definitions[pos].sign);
    }
    else {
        PrintAndSpeak(" не %s,", defenition->definitions[pos].sign);
    }
}

void PrintInfo(void) {
    printf("\n");
    PrintAndSpeak("Вы можете выбрать одну из следующих команд:\n");
    PrintAndSpeak("Чтобы начать игру                     введите \"начать\"    или \"н\"\n");
    PrintAndSpeak("Чтобы распечатать дерево              введите \"печать\"    или \"п\"\n");
    PrintAndSpeak("Чтобы напечатать описание объекта     введите \"описание\"  или \"о\"\n");
    PrintAndSpeak("Чтобы сравнить два объекта            введите \"сравнить\"  или \"cр\"\n");
    PrintAndSpeak("Чтобы изменить базу данных            введите \"изменить\"  или \"и\"\n");
    PrintAndSpeak("Чтобы завершить работу Акинатора      введите \"завершить\" или \"з\"\n");
    PrintAndSpeak("Чтобы сохранить базу данных Акинатора введите \"сохранить\" или \"сх\"\n\n");
}

ExitCodes ProceedTasks(Tree* tree) {
    assert(tree != nullptr);

    char* users_command = (char*) calloc(USERS_INPUT_MAX_LENGTH, sizeof(char));// 30

    PrintAndSpeak("Вы запустили Акинатора\n");

    while (strcmp(users_command, "завершить") != 0 || strcmp(users_command, "з") != 0) {
        PrintInfo();

        scanf("%s", users_command);

        if (strcmp(users_command, "начать") == 0 || strcmp(users_command, "н") == 0) {
            RunGame(tree);
        }
        else if (strcmp(users_command, "печать") == 0 || strcmp(users_command, "п") == 0) {
            RunDump(tree);
        }
        else if (strcmp(users_command, "изменить") == 0 || strcmp(users_command, "и") == 0) {
            MakeData(tree);
        }
        else if (strcmp(users_command, "сравнить") == 0 || strcmp(users_command, "ср") == 0) {
            RunComparison(tree);
        }
        else if (strcmp(users_command, "описание") == 0 || strcmp(users_command, "о") == 0) {
            RunDefinition(tree);
        }
        else if (strcmp(users_command, "сохранить") == 0 || strcmp(users_command, "сх") == 0) {
            SaveData(tree);
        }
        else if (strcmp(users_command, "завершить") == 0 || strcmp(users_command, "з") == 0) {
            PrintAndSpeak("С вами было классно играть!\n");
            break;
        }
        else {
            PrintAndSpeak("Неизветсная комманда <%s>, попробуйте еще раз\n", users_command);
            continue;
        }
    }

    free(users_command);
    return EXIT_OK;
}

void PrintAndSpeak(char* format, ...) {
    va_list args = {};
    va_start(args, format);

    vprintf(format, args);

    #ifdef AKINATOR_SPEAK
    char* buffer = (char*) calloc(MAX_SPEAK_LENGTH, sizeof(char));
    assert(buffer != nullptr);

    vsprintf(buffer, format, args);

    txSpeak(buffer);

    free(buffer);
    #endif // AKINATOR_SPEAK

    va_end(args);
}

void TreeVertexPrint(Node* node, FILE* fp) {
    assert(fp != nullptr);
    if (node != nullptr) {
        TreeVertexPrint(node->left, fp);
        if (node->left != nullptr) {
            fprintf(fp, "node%p [label =\"<question> Is it %s?\", color=\"grey14\", fillcolor=\"seagreen3\"]\n",
                    node, node->text);
        }
        else {
            fprintf(fp, "node%p [label =\"<answer> %s\", color=\"grey14\", fillcolor=\"orchid\"]\n",
                    node, node->text);
        }
        TreeVertexPrint(node->right, fp);
    }
}

void TreeEdgesPrint(Node* node, FILE* fp) {
    assert(fp != nullptr);

    if (node != nullptr) {
        TreeEdgesPrint(node->left, fp);
        if (node->left != nullptr) {
            fprintf(fp, "    node%p:sw -> node%p [color=\"navy\"];\n", node, node->left);
        }
        if (node->right != nullptr) {
            fprintf(fp, "    node%p:se -> node%p [color=\"crimson\"];\n", node, node->right);
        }
        TreeEdgesPrint(node->right, fp);
    }
}

void TreeVisualDump(Node* root, const char* file_name) {
    assert(root != NULL);
    assert(file_name != NULL);

    char* command_dot = (char*) calloc(100, sizeof(char));
    char* graph_file  = (char*) calloc(strlen(file_name) + 10, sizeof(char));

    strcpy(graph_file, file_name);
    strcat(graph_file, ".dot");
    FILE* fp = fopen(graph_file, "w");
    assert(fp != NULL);

    strcat(command_dot, "dot -Tpng ");   //graph_test1.dot -o Demo3.png"
    strcat(command_dot, graph_file);
    //strcat(command_dot, " -Gcharset = latin1");
    strcat(command_dot, " -o ");
    strcat(command_dot, file_name);
    strcat(command_dot, "_graph.png");

    fprintf(fp, "digraph structs {\n        node [shape=\"record\", style=\"filled\"];");  //rankdir = LR\n

    TreeVertexPrint(root, fp);
    TreeEdgesPrint(root, fp);

    fprintf(fp, "}");

    fclose(fp);

    fprintf(stderr, "graphing done with code: %d\n", system(command_dot));
}

ExitCodes RunDump(Tree* tree) {
    assert(tree != nullptr);

    char* file_name = (char*) calloc(100, sizeof(char));
    assert(file_name != nullptr);
    PrintAndSpeak("Пожалуйста введите имя файла для дампа\n");
    scanf("%s", file_name);

    TreeVisualDump(tree->root, file_name);

    PrintAndSpeak("Готово, результат лежит в папке с програмой\n");
    free(file_name);
    return EXIT_OK;
}

void MakeTestTree(Tree* tree) {
    tree->root = NodeConstruct("question_2", 'q');
    tree->root->left = NodeConstruct("question_1", 'q');
    tree->root->left->parent = tree->root;

    Node* node_tmp = tree->root->left;

    node_tmp->left = NodeConstruct("answer_1", 'a');
    node_tmp->left->parent = node_tmp;
    node_tmp->right = NodeConstruct("answer_2", 'a');
    node_tmp->right->parent = node_tmp;

    tree->root->right = NodeConstruct("question_3", 'q');
    tree->root->right->parent = tree->root;

    node_tmp = tree->root->right;
    node_tmp->left = NodeConstruct("answer_3", 'a');
    node_tmp->left->parent = node_tmp;
    node_tmp->right = NodeConstruct("question_4", 'q');
    node_tmp->right->parent = node_tmp;

    node_tmp = node_tmp->right;
    node_tmp->left = NodeConstruct("answer_4", 'a');
    node_tmp->left->parent = node_tmp;
    node_tmp->right = NodeConstruct("answer_5", 'a');
    node_tmp->right->parent = node_tmp;

    tree->size = 9;
    tree->answers_count = 5;
    tree->questions_count = 4;
}
