/* prefixExp.h, Gerard Renardel, 29 January 2014 */

#ifndef PREFIXEXP_H
#define PREFIXEXP_H

/* Here the definition of the type tree of binary trees with nodes containing tokens.
 */

typedef struct ExpTreeNode *ExpTree;

typedef struct ExpTreeNode {
    TokenType tt;
    Token t;
    ExpTree left;
    ExpTree right;
} ExpTreeNode;

/* STACKS */
typedef struct Stack { int *array;
    int top;
    int size;
} Stack;

ExpTree newExpTreeNode(TokenType tt, Token t, ExpTree tL, ExpTree tR);
int valueIdentifier(List *lp, char **sp);
int isNumerical(ExpTree tr);
double valueExpTree(ExpTree tr);
void printExpTreeInfix(ExpTree tr);
void prefExpTrees();
/* EIGEN */
int isChar(char c1, char c2);
int isPar(List *lp, int option);
int treeInfixFactor(List *lp, ExpTree *tp);
int treeInfixTerm (List *lp, ExpTree *tp, char c);
int treeInfixExpression(List *lp, ExpTree *tp);




#endif
