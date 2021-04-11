#include "Variable.h"
#include "Utility.h"
#include <stdio.h>
#include <stdlib.h>

//Parse a constant value like a float, int, string or char
struct AstNode* AST_Parse_Variable_Constant(Token* Tokens, int* Index, int TokensSize, AstValue Identifier  )
{
  struct AstNode* node = malloc(sizeof(struct AstNode));

  if (!node)
    return NULL;

  node->Type = VariableDefinition;
  node->Children = malloc(0);
  node->ChildrenLength = 0;

  struct AstNode* NameNode = MakeValueNode(Identifier);

  if (!NameNode)
    return NULL;

  AppendChildNode(node, NameNode);

  struct AstNode* AssignNode = MakeValueNode(Tokens[*Index].Value);

  if (!AssignNode)
    return NULL;

  AppendChildNode(node, AssignNode);

  return node;
}

//Parse assigning a variable to another identifier (such as a function call, or just another variable)
struct AstNode* AST_Parse_Variable_Identifier(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{

}

//Parse bracket order e.g if we receive something like: var V := (1 + 1) * 2; Sort out the order and determine which functions to call
struct AstNode* AST_Parse_Variable_Bracket_Order(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{

}

struct AstNode* AST_Parse_Variable(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{
  if (Tokens[*Index + 1].Token != IDENTIFIER)
  {
    printf("EXPECTED IDENTIFIER AFTER KEYWORD var");
    return NULL;
  }

  else
  {
    *Index += 3;
    if (Tokens[*Index].Token == VALUE)
    {
      return AST_Parse_Variable_Constant(Tokens, Index, TokensSize, Tokens[*Index - 2].Value);
    }
  }
}
