/*!
 * \file ShdList.l
 * \brief Shader list lexer.
 * \author Nus
 * \date 2011/12/25 21:24
 */
 
string                  [A-Za-z0-9_\/.-]+

%{

#include "ShdList.tab.h"
#include "Utility.h"

void EliminateCommentRegion(void);
void EliminateCommentLine(void);

%}

%%

"/*"                    EliminateCommentRegion();
"//"                    EliminateCommentLine();

\n|\r\n?                yylineno++;

"\""                    return QUOTE;

":"                     return COLON;
,                       return COMMA;
;                       return SEMI_COLON;

\{                      return OPEN_BRACE;
\}                      return CLOSE_BRACE;
\(                      return OPEN_BRACKET;
\)                      return CLOSE_BRACKET;

=                       return EQUAL;

"Preprocessor"          return PREPROCESSOR;
"Program"               return PROGRAM;
"VertexShader"          return VERTEX_SHADER;
"VertexPreprocessor"    return VERTEX_PREPROCESSOR;
"FragmentShader"        return FRAGMENT_SHADER;
"FragmentPreprocessor"  return FRAGMENT_PREPROCESSOR;

{string}                {
                          yylval = yyaddstr(yytext);
                          return STRING;
                        }

[ \t]+                  /* ignore whitespace */;

%%

void EliminateCommentRegion(void)
{
  /* Eat up all comments. */
  int ch;
  while(1) {
    ch = input();
    if(ch == '*') {
      int end = 0;
      do {
        ch = input();
        if(ch == '/') {
          end = 1;
          break;
        } else if(ch == '\n') {
          yylineno++;
        } else if(ch == '\r') {
          yylineno++;
          if(input() != '\n')
            unput(yytext[yyleng-1]);
        }
      } while(ch == '*');
      if(end == 1)
        break;
    } else if(ch == EOF) {
      yyerror("EOF in comment.");
      break;
    } else if(ch == '\n') {
      yylineno++;
    } else if(ch == '\r') {
      yylineno++;
      if(input() != '\n')
        unput(yytext[yyleng-1]);
    }
  }
}

void EliminateCommentLine(void)
{
  /* Eat up all comments. */
  int ch;
  while(1) {
    ch = input();
    if(ch == '\n') {
      yylineno++;
      break;
    } else if(ch == '\r') {
      yylineno++;
      if(input() != '\n')
        unput(yytext[yyleng-1]);
      break;
    }
  }
}
