#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "testtree.h"
#include "scanner.h"

TreeNode * parser(FILE *fileIn);
TreeNode * program();
TreeNode * block();
TreeNode * vars();
TreeNode * expr();
TreeNode * N();
TreeNode * A();
TreeNode * M();
TreeNode * R();
TreeNode * stats();
TreeNode * mStat();
TreeNode * stat();
TreeNode * in();
TreeNode * out();
TreeNode * If();
TreeNode * loop();
TreeNode * assign();
TreeNode * RO();
