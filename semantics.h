#ifndef SEMANTICS_H
#define SEMANTICS_H
#include<windows.h>
#include<wingdi.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include"y.tab.h"
#define MAX_CHARS 200


struct t_color  {
        unsigned char red;  //  红色分量
        unsigned char green; // 绿色分量
        unsigned char blue;  // 蓝色分量
		//t_color(unsigned char red=255,unsigned char green=0,unsigned char blue=0):red(red),green(green),blue(blue){}
} ;

struct t_color gColor;



HDC hDC;

HBRUSH hBrush;
typedef double (* FuncPtr)(double);
typedef double (*MathFuncPtr)(double);
struct Token {  // define in file “semantics.h”
	char *lexme;
	int type;
	double value;
	double(*FuncPtr)(double);
};

struct ExprNode
{  enum yytokentype OpCode; // 记号种类, 由YACC生成
   union  //重用：编译原理上机的实现
	{
		struct {struct ExprNode *Left,*Right;}CaseOperator;
		struct {struct ExprNode *Child;FuncPtr MathFuncPtr;}CaseFunc;
		double CaseConst;
		double *CaseParmPtr;
	}Content;
};
extern void DrawLoop(double Start,double End,double Step,struct ExprNode *x_ptr,struct ExprNode *y_ptr);
extern void DrawPixel(unsigned long x,unsigned long y);
extern struct ExprNode * MakeExprNode( enum yytokentype TokType,...);
extern double GetExprValue(struct ExprNode *root);
extern yyparse();
#endif
