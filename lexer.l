%{
# include "y.tab.h"   // 语法分析器接口
# include "semantics.h" // 自己的头文件
struct Token tokens;// 识别到的记号: 常量、函数
unsigned int LineNo =1;
%}
%option case-insensitive
%x comment_entry c_comment_entry

name	   [a-z]([_a-z0-9])*
number     [0-9]+
ws         [\t ]+
newline    \n

%%
"//"|"--" { BEGIN comment_entry ;}  /* 行注释 开始 */
<comment_entry>.
<comment_entry>"\n"	 { BEGIN 0;  LineNo ++; }

"/*"	{ BEGIN c_comment_entry ;}   /* 块注释 开始 */
<c_comment_entry>"*/"		{ BEGIN 0; }
<c_comment_entry>.
<c_comment_entry>"\n"		{ ++LineNo; }

"-"	return MINUS;
"+"	return PLUS;
"*"	return MUL;
"/"	return DIV;
","	return COMMA;
";"	return SEMICO;
"("	return L_BRACKET;
")"	return R_BRACKET;
"**"	return POWER;

{ws}		;
{newline}	LineNo++;

"origin" 		return ORIGIN;
"scale"			return SCALE;
"rot"			return ROT;
"is"			return IS;
"for"			return FOR;
"from"			return FROM;
"to"			return TO;
"step"			return STEP;
"draw"			return DRAW;
"t"				return T;
"color"         return COLOR;

"pi"  {tokens.type = CONST_ID; tokens.value = 3.14159; return CONST_ID;}
"e"  	{tokens.type = CONST_ID; tokens.value = 2.71828; return CONST_ID;}
"sin" 	{tokens.type = FUNC; tokens.FuncPtr = sin;   return FUNC;}
"cos" {tokens.type = FUNC; tokens.FuncPtr = cos;   return FUNC;}
"tan" {tokens.type = FUNC; tokens.FuncPtr = tan;   return FUNC;}
"exp" {tokens.type = FUNC; tokens.FuncPtr = exp;   return FUNC;}
"ln"  {tokens.type = FUNC;  tokens.FuncPtr = log; return FUNC;}
"sqrt" {tokens.type = FUNC; tokens.FuncPtr = sqrt; return FUNC;}

{number}(\.{number})?  {	tokens.value = atof(yytext) ;
						tokens.type = CONST_ID;
						return CONST_ID ;
			   			}

{name}	 	return ERRTOKEN;
.			return ERRTOKEN;

%%
int   yywrap() {   return   1; }


