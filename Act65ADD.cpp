#include "pch.h"

CAct65ADD::CAct65ADD()
{
}

CAct65ADD::~CAct65ADD()
{
}

bool CAct65ADD::Create(CAstNode* pChild, CAstNode* pNext)
{
	SetNodeName(m_pNodeTyypeName);
	return CAstNode::Create(pChild, pNext);
}

CValue* CAct65ADD::Process()
{
	CAstNode* pChild = 0, * pNext = 0;
	CValue* pValueChild = 0, * pValueNext = 0
		;
	pChild = GetChild();
	if (pChild)
	{
		pNext = pChild->GetNext();
	}
	if (pChild)
	{
		pValueChild = pChild->Process();
	}
	if (pNext)
	{
		pValueNext = pNext->Process();
	}
	//-----------------------
	// Code Generation
	//-----------------------
    return pValueChild;
}
void CAct65ADD::Print(FILE* pOut, int Indent, char* s, int l)
{
	CAstNode::Print(pOut, Indent, s, l);
}
/*
static value* binary(NODE* n, int op)
{
	NODE* n1, * n2;		/*	child nodes	*/
//	value* v1, * v2;      /*	values returned from processor nodes	*/
//	value* rV = NULL;
//	value* Lv;		//left value
//
//	if (Debug) printf("BINOP=%s\n", binop_strings[op]);
//	n1 = n->down;
//	n2 = n1->next;
//	Lv = (value*)n->aux;
//	if (Lv) fprintf(stderr, "Levft Value is %s\n", Lv->name);
//	v1 = (node_proc[n1->id])(n1);		/*	process nodes	*/
//	v2 = (node_proc[n2->id])(n2);
//	if (v1 && v2)	//good values?
//	{
//		//printf("VALUE1 %s  VALUE2 %s\n",v1->name,v2->name);
//		if (do_binary_const(&v1, binop_ops[op], &v2))		/*	was this a const	*/
//		{
//			release_value(v2);
//			if (Debug) printf("VALUE=%d\n", v1->type->V_INT);
//			rV = v1;
//		}
//		else
//		{
//			if ((SizeOfRef(v1->type) != SizeOfRef(v2->type)) && (!IS_CONSTANT(v1->type) && !IS_CONSTANT(v2->type)))
//				//----------------------------
				//OK, these guys are not the
				//smae size and they need to
				// be in order to combine them
				//----------------------------
//				if (SizeOfRef(v1->type) > SizeOfRef(v2->type))
//				{
////					v2 = ConvertTypeUp(OutFile, v2, v1->type);
//					//---------------------------
//					// this operation leaves the
////					// in registers if v2 is not
//					// a long. so we need to save
					// v2 to a temp otherwise
					//--------------------------
//				}
//				else if (SizeOfRef(v2->type) > SizeOfRef(v1->type))
////				{
//					v1 = ConvertTypeUp(OutFile, v1, v2->type);
//				}
//			}
//			if (ValInMem(v1) && !ValInMem(v2))
//			{
//				if (NotRelOp(op))	//we can swap operands
//				{
//					value* t;
//					t = v1;
//					v1 = v2;
//					v2 = t;		//swap around the args
//				}
//				else
//				{
//					v2 = SaveToTemp(OutFile, v2);	//put in temp
//				}
//			}
//			else if (!ValInMem(v1) && !ValInMem(v2))
//			{
//				v2 = SaveToTemp(OutFile, v2);	//put V2 in a temp
//			}
//			rV = DoBinary(OutFile, v1, op, v2, 1, 1, NULL, NULL, Lv);
			//			release_value(v2);
//		}
//	}
//	return rV;
//}
