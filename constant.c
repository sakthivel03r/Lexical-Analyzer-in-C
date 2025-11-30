#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexical.h"

Result is_constant(char ch)
{
    if(isalnum(ch) || ch == '.' || ch == '_')    //Checking Character is Equal to isalnum or dot('.') or underscore('_')
        return e_nonzero;                        //Present return e_nonzero or return e_zero
    else
        return e_zero;
}

void Numerical_constant(Analysis * sptr)
{
    char buffer[30]; int i = 0;

    buffer[i++] = sptr->ch;                                             //Store First Character in First Position

    while((sptr->ch = fgetc(sptr->fp)) != EOF && is_constant(sptr->ch)) //Checking Character is Not equal to EOF or equal to is_consatnt
        buffer[i++] = sptr->ch;                                         //Store Charcter one by one in Buffer

    buffer[i] = '\0' ;                                                  //Store Null character in end of the buffer

    if(sptr->ch != EOF)                                                 //Checking character in Not equal to EOF
        ungetc(sptr->ch, sptr->fp);                                     //unget the character get using ungetc

    int Float = 0 , hexa = 0 , iden = 0 , Binary = 0;  i = 0;         

    while(buffer[i])                                                    //running loop upto Null charceter
    {
        if(buffer[i] == '.')                                            //Checking Characetr is equal to dot('.')
        {
            Float = 1;                                                  //assing 1 in variable and break the loop
            break;
        }

        else if(buffer[i] == 'X' || buffer[i] == 'x')                   //Checking character is equal to Lower or uppercae x
        {
            hexa = 1;                                                   //assign 1 in varaible and break the loop
            break;
        }

        else if(buffer[i] == 'b' || buffer[i] == 'B')                   //Checking Character is equal to Lower or uppercase b
        {
            Binary = 1;                                                 //assign value 1 & brek the loop
            break;
        }

        else if(isalpha(buffer[i]) || buffer[i] == '_')                 //Checking Character is Alphabet or Underscore
        {
            iden = 1;                                                   //Assign value 1 & break the loop
            break;
        }

        i++;
    }

    //Checking Float or hexa or Binary or iden or octal is High & Function and Check return type in Non-zero
    if(Float)
    {
        if(error_float(sptr , buffer))
            printf(O "%-20s"E"  ~> %s\n" ,"FLOAT CONSTANT", buffer);
    }

    else if(hexa)
    {
        if(error_hexa(sptr ,buffer))
            printf(O"%-20s"E"  ~> %s\n" ,"HEXADECIMAL CONSTANT", buffer);
    }

    else if(Binary)
    {
        if(error_binary(sptr,buffer))
            printf(O"%-20s"E"  ~> %s\n" ,"BINARY CONSTANT", buffer);
    }

    else if(buffer[0] == '0' && (strlen(buffer) != 1))
    {
        if(error_octal(sptr,buffer))
            printf(O"%-20s"E"  ~> %s\n" ,"OCTAL CONSTANT", buffer);
    }

    else if(iden)
        error_identifier(sptr , buffer , 2);

    else
        printf(O"%-20s"E"  ~> %s\n" ,"NUMERICAL CONSTANT", buffer);
}

void Bracket(Analysis * sptr)
{

    if(sptr->ch == '{')                                          //Checking character is equal to open flower bracket
    {
        sptr->flower[0]++;                                       //increment the Count in flower position 0    
        sptr->flow_pos[++sptr->flow_open] = sptr->row;           //after increment of flow_open(index) store the row value in flow_pos
    }

    else if(sptr->ch == '}')                                     //Checking Character is equal to close flower bracket 
    {
        sptr->flower[1]++;                                       //increment the count in flower position 1
        sptr->flow_open--;                                       //decrement the flow_open(index)
    }

    else if(sptr->ch == '(')                                     //Checking character is equal to open bracket
    {
        sptr->bracket[0]++;                                      //increment the Count in bracket position 0
        sptr->bracket_pos[++sptr->bracket_open] = sptr->row;     //after increment of bracket_open(index) store the row value in bracket_pos
    }

    else if(sptr->ch == ')')                                     //Checking Character is equal to close bracket
    {
        sptr->bracket[1]++;                                      //increment the count in bracket position 1
        sptr->bracket_open--;                                    //decrement the bracket_open(index)
    }

    else if(sptr->ch == '[')                                     //Checking character is equal to open square bracket
    {
        sptr->square[0]++;                                       //increment the count in square bracket position 0
        sptr->square_pos[++sptr->square_open] = sptr->row;       //after increment of bracket_open(index) store the row value in square_pos
    }

    else                                                         //Checking Character is equal to close square bracket
    {
        sptr->square[1]++;                                       //increment the count in square position 1
        sptr->square_open--;                                     //decrement the square_open(index)
    }

    error_parenthisis(sptr , 1);                                 //Function call
    if(is_bracket(sptr->ch) == e_open)                           //Checking is_bracket return is equal to 1
        printf(B "%-20s"E"  ~> %c\n" ,"OPENING BRACKET", sptr->ch);

    else                                                         //else return will be 2
        printf(B "%-20s"E"  ~> %c\n" ,"CLOSEING BRACKET", sptr->ch);
}

