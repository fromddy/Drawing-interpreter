#include "semantics.h"
extern double Scale_x;
extern double Scale_y;
extern double Origin_x;
extern double Origin_y;
extern double Rot_angle;
extern double Parameter;
extern double start,end,step;
extern unsigned int LineNo;
extern struct t_color;





void DrawLoop(double Start,double End,double Step,struct ExprNode *x_ptr,struct ExprNode *y_ptr);
void DrawPixel(unsigned long x,unsigned long y);
double GetExprValue(struct ExprNode *root);
static void Errsmg(char *string);
static void CalcCoord(struct ExprNode *x_ptr,struct ExprNode *y_ptr,double *x_val,double *y_val);
void errsmg(char *string){
	exit(1);
}
double GetExprValue(struct ExprNode *root)
{
	if(root==NULL) return 0.0;
	switch(root->OpCode)
	{
		case PLUS:return GetExprValue(root->Content.CaseOperator.Left)+GetExprValue(root->Content.CaseOperator.Right);
		case MINUS:	return GetExprValue(root->Content.CaseOperator.Left)-GetExprValue(root->Content.CaseOperator.Right);
		case MUL:return GetExprValue(root->Content.CaseOperator.Left)*GetExprValue(root->Content.CaseOperator.Right);
		case DIV:return GetExprValue(root->Content.CaseOperator.Left)/GetExprValue(root->Content.CaseOperator.Right);
		case POWER:return pow(GetExprValue(root->Content.CaseOperator.Left),GetExprValue(root->Content.CaseOperator.Right));
		case FUNC:return (*root->Content.CaseFunc.MathFuncPtr)(GetExprValue(root->Content.CaseFunc.Child));
		case CONST_ID:return root->Content.CaseConst;
		case T:return *(root->Content.CaseParmPtr);

		default:return 0.0;
	}
}

void DrawLoop(double Start,double End,double Step,struct ExprNode *x_ptr,struct ExprNode *y_ptr)
{	extern double Parameter;	// ����T�Ĵ洢�ռ�
	double x, y;
	for(Parameter=Start; Parameter<=End; Parameter+=Step)
	{
	CalcCoord(x_ptr, y_ptr, &x, &y); // ����ʵ������
	DrawPixel((unsigned long)x, (unsigned long)y);
	}
}
void CalcCoord (struct ExprNode * x_nptr,struct ExprNode * y_nptr,double * x_val,double * y_val )
{   double local_x, local_y, temp;
    local_x =GetExprValue(x_nptr); 	// ������ԭʼ����
    local_y =GetExprValue(y_nptr);
    local_x *= Scale_x; 			// �����任
    local_y *= Scale_y;
    temp=local_x*cos(Rot_angle)+local_y*sin(Rot_angle);
    local_y=local_y*cos(Rot_angle)-local_x*sin(Rot_angle);
    local_x = temp; 			// ��ת�任
    local_x += Origin_x;			// ƽ�Ʊ任
    local_y += Origin_y;
    *x_val = local_x; 	// ���ر任��������
    *y_val = local_y;
}

/*
void DrawPixel(unsigned long x,unsigned long y)
{
	SetPixel(hDC,x,y,blue);


}
*/



void DrawPixel( unsigned long x, unsigned long y)
{
	COLORREF color = RGB( gColor.red, gColor.green, gColor.blue);
	SetPixel(hDC, x, y, color);
	SetPixel(hDC, x+1, y, color);
	SetPixel(hDC, x, y+1, color);
	SetPixel(hDC, x+1, y+1, color);
}


