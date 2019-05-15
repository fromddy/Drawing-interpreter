%{
#define YYSTYPE struct ExprNode *
#include"semantics.h"
#include"y.tab.h"
extern int yylex (void) ;
extern unsigned char *yytext;
double Parameter=0,// 参数T的存储空间
  Origin_x=0, Origin_y=0,	// 横、纵平移距离
  Scale_x=1, 	Scale_y=1, 	// 横、纵比例因子
  Rot_angle=0,start=0,end=0,step=0;
extern struct Token tokens;
extern unsigned int LineNo;
extern struct t_color gColor;
%}
%token CONST_ID FUNC FOR FROM DRAW TO STEP ORIGIN;
%token SCALE ROT IS T ERRTOKEN;
%token SEMICO COMMA L_BRACKET R_BRACKET COLOR LINEWIDTH;


%left  PLUS MINUS;
%left  MUL DIV;
%right UNSUB;
%right POWER;
%start Program;

%%
Program :  /* ε */
	| Program Statement SEMICO
	;
Statement
: FOR T FROM Expr TO Expr STEP Expr DRAW L_BRACKET Expr COMMA Expr R_BRACKET
    {start = GetExprValue($4);
     end   = GetExprValue($6);
     step  = GetExprValue($8);
     DrawLoop(start,end,step,$11,$13);
    }
|ORIGIN IS L_BRACKET Expr COMMA Expr R_BRACKET
	{ Origin_x = GetExprValue($4);
	  Origin_y = GetExprValue($6);
	}
|ROT IS Expr
	{ Rot_angle = GetExprValue($3); }
|SCALE IS L_BRACKET Expr COMMA Expr R_BRACKET
{Scale_x = GetExprValue($4);
 Scale_y = GetExprValue($6);
}
|COLOR IS  Expr Expr Expr
{

gColor.red=GetExprValue($3);
gColor.green=GetExprValue($4);
gColor.blue=GetExprValue($5);

}

;



Expr
: T               { $$ = MakeExprNode(T); }
| CONST_ID 			  {$$=MakeExprNode(CONST_ID,tokens.value);}
| Expr PLUS Expr    { $$ = MakeExprNode(PLUS, $1, $3);}
| Expr MINUS Expr   { $$ = MakeExprNode(MINUS,$1,$3);}
| Expr MUL Expr     { $$ = MakeExprNode(MUL,  $1, $3);}
| Expr DIV Expr     { $$ = MakeExprNode(DIV,  $1, $3);}
| Expr POWER Expr	{ $$=MakeExprNode(POWER, $1, $3);}
| L_BRACKET Expr R_BRACKET  { $$ = $2; }
| PLUS Expr %prec UNSUB		{ $$ = $2; }
| MINUS Expr %prec UNSUB 	{$$ = MakeExprNode(MINUS, MakeExprNode(CONST_ID, 0.0), $2);}
| FUNC L_BRACKET Expr R_BRACKET   {$$ = MakeExprNode(FUNC, tokens.FuncPtr, $3);}
| ERRTOKEN {yyerror("error token in the input.");}
;
%%
int yyerror (const char *Msg)
{	char headmsg [200];
	memset(headmsg,0,200);
	sprintf(headmsg, "Line %d: %s", LineNo, yytext);
	MessageBox(NULL,headmsg,Msg,MB_OK);

}

struct ExprNode * MakeExprNode( enum yytokentype TokType,...)
{
  va_list ArgPtr;
  struct ExprNode* ExprPtr=(struct ExprNode*)malloc(sizeof(struct ExprNode));
  ExprPtr->OpCode=TokType;
  va_start(ArgPtr,TokType);
  switch( TokType )
  { case CONST_ID:  {ExprPtr->Content.CaseConst = (double)va_arg(ArgPtr,double); break;}
    case T:   {ExprPtr->Content.CaseParmPtr = &Parameter;   break;}
    case FUNC:   {
                 ExprPtr->Content.CaseFunc.MathFuncPtr = (FuncPtr)va_arg(ArgPtr,FuncPtr);
                 ExprPtr->Content.CaseFunc.Child =(struct ExprNode *)va_arg(ArgPtr,struct ExprNode*);
                 break;
                 }
    default:     {
                 ExprPtr->Content.CaseOperator.Left =(struct ExprNode *)va_arg(ArgPtr,struct ExprNode*);
                 ExprPtr->Content.CaseOperator.Right=(struct ExprNode *)va_arg(ArgPtr,struct ExprNode*);
                 break;
                 }
  }
  va_end(ArgPtr);
  return ExprPtr;
}

