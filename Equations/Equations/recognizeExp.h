/* recognizeExp.h, Gerard Renardel, 29 January 2014 */

#ifndef RECOGNIZEEXP_H
#define RECOGNIZEEXP_H

int acceptNumber(List *lp);
int acceptIdentifier(List *lp, char **varName);
int acceptCharacter(List *lp, char c);
int acceptExpression(List *lp, char **varName, int *degree);
int acceptEquation(List *lp, char **varName, int *degree);
void recognizeExpressions();


#endif
