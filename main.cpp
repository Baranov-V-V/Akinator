#include "Akinator_header.h"
#include "Akinator_func.cpp"

int main() {
    Tree tree = {};

    TreeConstruct(&tree);
    MakeData(&tree);
    //ProceedTasks(&tree);
    TreeClear(&tree);

    return 0;
}
