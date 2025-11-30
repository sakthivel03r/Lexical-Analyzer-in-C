#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexical.h"

Result is_operator(char ch)
{
    if(strchr("+-*/%.~!?<>^&|=" , ch)) //Checking Charcter is present in String using strchr (string) Function
        return e_nonzero;              //Present return e_nonzero or return e_zero

    else
        return e_zero;
}

Result is_symbol(char ch)
{
    if(ch == ';' || ch == ',')         //Checking Character is Symbol(',' , ';')
        return e_nonzero;              //Present return e_nonzero or return e_zero

    else
        return e_zero;
}

Result is_stray(char ch)
{
    if(ch == '$' || ch == '`' || ch == '@' || ch == '#')  //Checking Charcter is Stary (# , @ , $ , `)
        return e_nonzero;                                 //Present return e_nonzero or return e_zero

    else 
        return e_zero;
}

bracket is_bracket(char ch)
{
    if(ch == '(' || ch == '{' || ch == '[')               //Checking Character Bracket 
        return e_open;                                    //Return e_open

    else if(ch == ')' || ch == '}' || ch == ']')
        return e_close;                                   //return e_close

    else
        return e_not;                                     //return e_not
}

void Preprocessor(Analysis *sptr)
{
    //Declaration of variable
    char buffer[50]; int i = 0;
    buffer[i++] = sptr->ch;                                          //Store Character in Buffer in First Poisition
    
    while((sptr->ch = fgetc(sptr->fp)) != EOF && sptr->ch != '\n')   //Checking Charcter is Not equal to EOF or NOT
        buffer[i++] = sptr->ch;                                      //Store One charcter one by one

    buffer[0] = '\0';                                                //Adding Null charcter in end
    
    if(sptr->ch != EOF)                                              //Checking loop not end with EOF
        ungetc(sptr->ch , sptr->fp);                                 //Put Chatcter again in File
}

void Keyword_identifier(Analysis *sptr)
{
    char buffer[100]; int i = 0;
    //Store the all Keyword in 2D character array
    const char *keywords[] = {"char","int","float","double","signed","unsigned","short","long","const","volatile","for","while","do","goto",
        "break","continue","if","else","switch","case","default","auto","registor","static","extern","struct","union","enum","typedef",
        "void","return","sizeof"};

    int total_keywords = sizeof(keywords) / sizeof(keywords[0]);   //Store Size in total_keywords
    buffer[i++] = sptr->ch;                                        //Store First Charcter in First Position
                                                                   //Checking Character is Not equal to EOF and charcter is equal to underscore or isalnunm or is_stary
    while (((sptr->ch = fgetc(sptr->fp)) != EOF) && (isalnum(sptr->ch) || sptr->ch == '_' || is_stray(sptr->ch)))
        buffer[i++] = sptr->ch;                                    //Store each character in Buffer

    buffer[i] = '\0';                                              //Store Null Character in End of Buffer
    
    if (sptr->ch != EOF)                                           //Checking Character is Not equal to EOF
        ungetc(sptr->ch, sptr->fp);                                //Put Chatcter again in File

    for (i = 0; i < total_keywords; i++)                           //Running Loop upto Total Keyword
    {
        if (strcmp(buffer, keywords[i]) == 0)                      //Comparing buffer to all Keywrd in 2D array
        {
            printf(G "%-20s"E"  ~> %s\n","KEYWORD",buffer); 
            return;
        }
    }

    if(error_identifier(sptr , buffer , 1))                        //Function call
        printf(Y"%-20s"E"  ~> %s\n","IDENTIFIER",buffer);
}

void Character_constant(Analysis * sptr)
{
    char buffer[10] , prev = 0; int i = 0;
    buffer[i++] = sptr->ch;                       //Store First Character in First Position Of buffer
    
    while((sptr->ch = fgetc(sptr->fp)) != EOF)    //Checking Chatrcter is Not equal to EOF
    {
        buffer[i++] = sptr->ch;                   //Store Charcter one by one in Buffer
    
        if(sptr->ch == 39 && prev != '\\')        //Checking Charcter is equal to single quots(')
            break;                                //Break loop

        else if(sptr->ch == '\n')                 //Checking Charcter is equal to New line
        {
            sptr->row++;                          //Increment the row count and Break the loop
            break;
        }

        prev = sptr->ch ;
    }

    buffer[i] = '\0';                             //Store Null Charactre in end of the buffer
    
    if(error_character(sptr , buffer))            //Function call
        printf(O"%-20s"E"  ~> %s\n" ,"CHARACTER CONSTANT", buffer);
}  

void String_iter(Analysis * sptr)
{
    char buffer[300]; int i = 0;
    buffer[i++] = sptr->ch;                      //Store First character in First Position
    
    while((sptr->ch = fgetc(sptr->fp)) != EOF)   //Checking Charcter is not equal to EOF
    {
        buffer[i++] = sptr->ch;                  //Store Character one by one in buffer
    
        if(sptr->ch == 34)                       //Checking Charcter is Equal to double quots(")
            break;                               //break the loop

        else if(sptr->ch == '\n')                //Checking Charcter is equal to New line
        {
            sptr->row++;                         //Increment the row count and Break the loop
            break;
        }
    }

    buffer[i] = '\0' ;                           //Store Null Charactre in end of the buffer
    
    if(error_string_iter(sptr , buffer))         //Function call
        printf(O "%-20s"E"  ~> %s\n" ,"STRING ITER",buffer);
}

void Comment_line(Analysis *sptr)
{
    char buffer[200] , prev = 0;   int i = 0;
    buffer[i++] = sptr->ch;                                             //Store First character in First Position
    
    sptr->ch = fgetc(sptr->fp);                                         //Get Next Character
    buffer[i++] = sptr->ch;                                             //Store Next Character in Second Position
    
    if (sptr->ch == '/')                                                //Checking Charcter is Not equal to slash(/)
    {
        while((sptr->ch = fgetc(sptr->fp)) != EOF && sptr->ch != '\n')  //Checking Charcter is Not equal to EOF or Newline
            buffer[i++] = sptr->ch;

        if(sptr->ch == '\n') sptr->row++;                               //Checking Character is equal to Newline
    }

    else if (sptr->ch == '*')                                           //Checking Character is equal to star
    {
        while((sptr->ch = fgetc(sptr->fp)) != EOF)                      //Checking Character is Not Equal to EOF
        {
            buffer[i++] = sptr->ch;                                     //Store Character one by one in buffer

            if (prev == '*' && sptr->ch == '/')                         //Checking Previous Charcter is star and current Charcter is equal to slash
                break;                                                  //Break the loop

            if(sptr->ch == '\n') sptr->row++;                           //Checking Charcter is Newline then increment the Count
            prev = sptr->ch;                                            //store current charcter in previous
        }
    }

    else                                                                //Character is Not equal to slash or star then else work
    {
        ungetc(sptr->ch, sptr->fp);                                     //Put cgarcter gain in File using ungetc
        sptr->ch = '/';                                                 //store slash Character in structure Member
        Operator(sptr);                                                 //Function call
        return;
    }

    buffer[i] = '\0';                                                   //Store Null Charcter in end of buffer
    error_comment_line(sptr, buffer);                                   //Function call
}

void Operator(Analysis * sptr)
{
    char buffer[10] ; int i = 0;
    buffer[i++] = sptr->ch;                           //Put First Character in First position of Buffer
    
    while( (sptr->ch = fgetc(sptr->fp)) != EOF)       //Checking Charcter is Not equal to EOF
    {
        if(strchr("+-<>&|=",sptr->ch))                //Checking Charcter present in string using strchr
            buffer[i++] = sptr->ch;                   //Store Character in buffer one by one

        else if(is_stray(sptr->ch))                   //Checking charcter is stray
            buffer[i++] = sptr->ch;                   //Store Character in buffer one by one

        else                                          //if character is Not present in string or stary
        {
            ungetc(sptr->ch , sptr->fp);              //Put character in file again
            buffer[i] = '\0';                         //Store Null charcter in end of buffer
     
            if(error_operator(sptr , buffer , 1 ))    //Function call & Check Return type is Non-zero & break the loop
                break;
        }
    }

    printf(T "%-20s"E"  ~> %s\n" ,"OPERATOR", buffer);
}

void Symbol(char ch)
{
    printf(P"%-20s"E"  ~> %c\n" ,"SYMBOL", ch);
}
