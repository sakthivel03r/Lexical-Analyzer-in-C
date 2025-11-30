#ifndef LEXICAL_H
#define LEXICAL_H

typedef struct Analysis
{
    char ch;
    FILE *fp;
    char * fname;
    int row;
    int flower[2];
    int square[2];
    int bracket[2];
    int flow_pos[10];
    int square_pos[10];
    int bracket_pos[10];
    int flow_open;
    int square_open;
    int bracket_open;

} Analysis;

extern Analysis s ;
extern Analysis * sptr;


typedef enum
{
    e_zero,
    e_nonzero

}Result;

typedef enum
{
    e_not,
    e_open,
    e_close

}bracket;

/* Token Identifing */
void Preprocessor(Analysis * );
void Analysis_code();
void Keyword_identifier(Analysis * );
void Character_constant(Analysis * );
void Numerical_constant(Analysis * );
void String_iter(Analysis * );
void Operator(Analysis *);
void Comment_line(Analysis *);
void Symbol(char );
void Bracket(Analysis * );

/* Sub Function for Checking */
Result is_operator(char ch);
Result is_symbol(char ch);
Result is_stray(char ch);
 bracket is_bracket(char ch);

/* Error Checking Functions */
Result error_preprocessor(Analysis * sptr , char buffer[]);
Result error_character(Analysis * sptr , char buffer[]);
Result error_string_iter(Analysis * sptr , char buffer[]);
Result error_comment_line(Analysis * sptr , char buffer[]);
Result error_operator(Analysis * sptr , char buffer[] , int type);
Result error_identifier(Analysis * sptr , char buffer[] , int iden);
Result error_float(Analysis * sptr , char buffer[]);
Result error_hexa(Analysis * sptr , char buffer[]);
Result error_binary(Analysis * sptr , char buffer[]);
Result error_parenthisis(Analysis * sptr , int out);
Result error_octal(Analysis * sptr , char buffer[]);

/* Colour code */
#define T "\033[38;2;0;206;209m"
#define R "\033[1;91m"
#define G "\033[1;92m"
#define B "\033[1;94m"
#define Y "\033[38;2;255;215;0m"
#define O "\033[38;2;255;140;0m"
#define P "\033[38;2;128;0;128m"
#define E "\033[0m"

#endif
