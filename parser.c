#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

static Token * tk;

// driver to test the scanner 
TreeNode * parser(FILE *fileIn) {
	
	fin = fileIn;

	TreeNode * root;

	tk = scanner();

	root = program();
	if (tk->tokenID == 1004){

	} else {

		printf("Error: %d Program didn't end correctly with EOF\n", -4);
		exit(0);
		
	}
	return root;
}

/*-----------------------------------------------------------*/
/*<program> -> start <vars> main <block> stop */
/*-----------------------------------------------------------*/

TreeNode * program(){

	TreeNode * n = newTreenode("<program>");


	if(strcmp(tk->tokenName, "start") == 0){

		tk = scanner();

		n->child1 = vars();
		
		if(strcmp(tk->tokenName, "main") == 0){
			tk = scanner();
			n->child2 = block();		
			
			if(strcmp(tk->tokenName, "stop") == 0) {
				tk = scanner();
				return n;
			} else {
				printf("Error: Unsupported program occurred in program stop");
				exit(0);
			}
		} else { 
			printf("Error: Unsupported program occured in program main");
			exit(0);
		}
		
	} else {
		printf("Error: Unsupported program, occurred in program()\n");
		exit(0);
	}
	return NULL;
}

/*====================================================*/
/* <block> -> {<vars><stats>}*/
/*====================================================*/

TreeNode * block(){
	TreeNode * n = newTreenode("<block>");
	if(strcmp(tk->tokenName, "{") == 0){

		tk = scanner();

		n->child1 = vars();
		n->child2 = stats();

		if(strcmp(tk->tokenName,"}") == 0){
			tk = scanner();
			return n;
		} else {
			printf("Error: Unsupported program, occurred in block() after }\n");
			exit(0);
		}
	} else {
		printf("Error: Unsupported program, occurred in block() before {.\n");
		exit(0);

	}
	return NULL;
}


/*=====================================================*/
/*<vars> -> empty | let Identifier : Integer <vars> */
/*======================================================*/

TreeNode * vars(){
	TreeNode * n = newTreenode("<vars>");
	if(strcmp(tk->tokenName,"let") == 0){
		tk = scanner();
	
		if(tk->tokenID == 1000){
			n->head = listInsert(n->head, tk);

			tk = scanner();

			if(strcmp(tk->tokenName, ":") == 0){
				tk = scanner();

				if(tk->tokenID == 1001){
					n->head = listInsert(n->head, tk);

					tk = scanner();
					n->child1 = vars();
					return n;
				} else {
					printf("Error: Unsupported program, occurred in vars() after ':'\n");
					exit(0);
				}
			} else {
				printf("Error: Unsupported program, occurred in vars() after Identifier\n");
				exit(0);
			}
		} else {
			printf("Error: Unsupported program, occurred in vars() after Keyword\n");
			exit(0);
		}
	} 
	return n;
}


/*===================================================*/
/*<expr> -> <N> | <expr> | <N> * <expr> | <N> */
/*===================================================*/

TreeNode * expr(){
	TreeNode * n = newTreenode("<expr>");
	n->child1 = N();
	if((strcmp(tk->tokenName, "*") == 0)){
		n->head = listInsert(n->head, tk);
		tk = scanner();
		n->child2 = expr();
	} 
	return n;
}

/*=================================================*/
/*<N> -> <A> + <N> | <A> - <N> | <A>*/
/*==============================================*/

TreeNode * N(){
	TreeNode * n = newTreenode("<N>");
	n->child1 = A();

	if((strcmp(tk->tokenName, "+") == 0) || (strcmp(tk->tokenName, "-") == 0)){
	n->head = listInsert(n->head, tk);
	tk = scanner();
	
	n->child2 = N();
	return n;
	}
	return n;
}

/*================================================*/
/*<A> -> %<A> | <R> */
/*================================================*/

TreeNode * A(){
	TreeNode * n = newTreenode("<A>");
	
	if((strcmp(tk->tokenName, "%") == 0)){
		n->head = listInsert(n->head, tk);
		tk = scanner();

		n->child1 = A();
		
	} else {
		n->child1 = R();
	}
	return n;
}


/*==============================================*/
/*<R> -> [<expr>] | Identifier | Integer */
/*==================================================*/


TreeNode * R(){
	TreeNode * n = newTreenode("<R>");
	if(strcmp(tk->tokenName,"[") == 0){
		tk = scanner();
		n->child1 = expr();

		if(strcmp(tk->tokenName,"]") == 0){
			tk = scanner();

			return n;
		} else {
			printf("Error: Unsupported program, occurred in R() after '('\n");	
			exit(0);
		}
	} else if (tk->tokenID == 1000){
		n->head = listInsert(n->head, tk);
		tk = scanner();
		return n;
	} else if (tk->tokenID == 1001){
		n->head = listInsert(n->head, tk);
		tk = scanner();
		return n;
	} else {		
		fprintf(stderr, "Parsing error: Unsupported program, occurred in R() after ')'\n");
		exit(0);
	}
	return n;
}


/*=================================================*/
/*<stats> -> <stat> <mStat> */
/*================================================*/


TreeNode * stats(){
	TreeNode * n = newTreenode("<stats>");
	n->child1 = stat();
	n->child2 = mStat();
	return n;
}


/*================================================*/
/*<mStat> -> empty | <stat> <mStat> */
/*=================================================*/

TreeNode * mStat(){
	TreeNode * n = newTreenode("<mStat>");
	if(strcmp(tk->tokenName, "scanf") == 0 || strcmp(tk->tokenName, "printf") == 0 || strcmp(tk->tokenName, "start") == 0 || strcmp(tk->tokenName, "if") == 0 || strcmp(tk->tokenName, "loop") == 0 || strcmp(tk->tokenName, "let") == 0){
		n->child1 = stat();
		n->child2 = mStat();
		return n;
	} else {
		return NULL;
	}
}


/*==================================================*/
/*<stat> -> <in> . | <out> . | <block> | <if> .| <loop> . | <assign> .*/
/*====================================================*/

TreeNode * stat(){
	TreeNode * n = newTreenode("<stat>");
	if(strcmp(tk->tokenName, "scanf") == 0){
		n->child1 = in();
		if(strcmp(tk->tokenName, ".") == 0){
			tk = scanner();
			return n;
		}
	}
	else if(strcmp(tk->tokenName, "printf") == 0){
		n->child1 = out();
		if(strcmp(tk->tokenName, ".") == 0) {
			tk = scanner();
			return n;
		}
	} 
	else if(strcmp(tk->tokenName, "block") == 0){
		n->child1 = block();
		if(strcmp(tk->tokenName, ".") == 0) {
			tk = scanner();
			return n;
		}
	} 
	else if(strcmp(tk->tokenName, "if") == 0){
		n->child1 = If();
		if(strcmp(tk->tokenName, ".") == 0) {
			tk = scanner();
			return n;
		}
	} 
	else if(strcmp(tk->tokenName, "iter") == 0){
		n->child1 = loop();
		if(strcmp(tk->tokenName, ".") == 0) {
			tk = scanner();
			return n;
		}
	} 
	else if (strcmp(tk->tokenName, "let") == 0){
		n->child1 = assign();
		if(strcmp(tk->tokenName, ".") == 0) {
			tk = scanner();
			return n;
		}
	}
	else {
		printf("Error: Unsupported program, occurred in stat()\n");
		exit(0);
		return NULL;
	}
}


/*=================================================*/
/* <in> -> scanf[Identifier] */
/*===================================================*/


TreeNode * in(){
	TreeNode * n = newTreenode("<in>");
	tk = scanner();
	if(strcmp(tk->tokenName, "[" )== 0){
		tk = scanner();
		
				
		if(tk->tokenID == 1000){
			n->head = listInsert(n->head, tk);

			tk = scanner();
			if(strcmp(tk->tokenName, "]" ) == 0) {
				tk = scanner();
				return n;
			}
			else {
				printf("Error: Unsupported program, occured in in() after ]\n");
				exit(0);
			} 
		}else {
			printf("Error: Unsupported program, occurred in in() Identifier\n");
			exit(0);
		} 
	}else {
		printf("Error: Unsupported program, occured in in() after scanf\n");
		exit(0);
	}
	
	return NULL;
}


/*==========================================================*/
/*<out> -> printf[<expr>] */
/*==========================================================*/

TreeNode * out(){
	TreeNode * n = newTreenode("<out>");
	tk = scanner();
	if(strcmp(tk->tokenName, "[") == 0){
		tk = scanner();

		n->child1 = expr();

		if(strcmp(tk->tokenName, "]") == 0){
			tk = scanner();

		} else {
			printf("Error: Unsupported program, occurred in out() after '['\n");
			exit(0);
		}
	} else {
		printf("Error: Unsupported program, ocurred in out() before '['\n");
		exit(0);
	}
	return NULL;
}

/*=======================================================*/
/*<if> -> if[<expr> <RO> <expr> ] then <block> */
/*========================================================*/

TreeNode * If(){
	TreeNode * n = newTreenode("<if>");
	tk = scanner();
	if(strcmp(tk->tokenName, "[") == 0){
		tk = scanner();
		n->child1 = expr();
		n->child2 = RO();
		n->child3 = expr();

		if(strcmp(tk->tokenName, "]") == 0){
			tk = scanner();
			n->child4 = block();
			return n;
		} else {
			printf("Error: Unsupported program, ocurred in If() after '('\n");
			exit(0);
		}
	} else {
		printf("Error: Unsupported program, ocurred in If() before '('\n");
		exit(0);
	}
	return NULL;
}


/*==========================================================*/
/*<loop> -> iter[<expr> <RO> <expr> ] <block> */
/*==========================================================*/
TreeNode * loop(){
	TreeNode * n = newTreenode("<loop>");
	tk = scanner();
	if(strcmp(tk->tokenName, "[") == 0){
		tk = scanner();

		n->child1 = expr();
		n->child2 = RO();
		n->child3 = expr();

		if(strcmp(tk->tokenName, "]") == 0){
			tk = scanner();

			n->child4 = block();
			return n;
		} else {
			printf("Error: Unsupported program, ocurred in loop() after '('\n");
			exit(0);
		}
	} else {
		printf("Error: Unsupported program, ocurred in loop() before '('\n");
		exit(0);
	}
	return NULL;
}

/*===========================================================*/
/*<assign> -> Identifier = <expr> */
/*============================================================*/

TreeNode * assign(){
	TreeNode * n = newTreenode("<assign>");
	tk = scanner();
	if(tk->tokenID == 1000){
		n->head = listInsert(n->head, tk);
		tk = scanner();

		if(strcmp(tk->tokenName, "=") == 0){
			n->head = listInsert(n->head, tk);
			tk = scanner();
			n->child1 = expr();

		} else {
			printf("Error: Unsupported program, occurred in assign() after 'Identifier'\n");
			exit(0);
		}
	} else {
		printf("Error: Unsupported program, occurred in assign() before 'Identifier'\n");
		exit(0);
	}
	return NULL;
}


/*==========================================================*/
/*<RO> -> =< | => | == | : : */
/*==========================================================*/

TreeNode * RO(){
	TreeNode * n = newTreenode("<RO>");
	if(strcmp(tk->tokenName, "<") == 0){
		n->head = listInsert(n->head, tk);
		
		tk = scanner();

		if(strcmp(tk->tokenName, "=") == 0){
			n->head = listInsert(n->head, tk);

			tk = scanner();
			return n;
		} else {
			return n;
		}
	} else if (strcmp(tk->tokenName, ">") == 0){
		n->head = listInsert(n->head, tk);

		tk = scanner();

		if(strcmp(tk->tokenName, "=") == 0){
			n->head = listInsert(n->head, tk);

			tk = scanner();
			return n;
		} else {
			return n;
		}
	} else if(strcmp(tk->tokenName, "=") == 0){
		n->head = listInsert(n->head, tk);

		tk = scanner();

		if(strcmp(tk->tokenName, "=") == 0){
			n->head = listInsert(n->head, tk);

			tk = scanner();
			return n;
		} else {
			return n;
		}
	} else {
		printf("Error: Unsupported program, occurred in RO()\n");
		exit(0);
	}
	return NULL;
}
