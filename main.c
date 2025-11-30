/*

Name    : Sakthivel R
Date    : 05-09-2025
Tittle  : Lexical Analysis

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexical.h"

Analysis s ;            //Structure Variable Declaration
Analysis * sptr = &s;   //Structure Pointer Declaration

Result main(int argc , char *argv[])
{
    if(argc != 2)                   //check the argument count Not equal to 2
    {
        printf(T"INFO : "E"Usage %s <filename>\n",argv[0]);
        return e_zero;
    }

    sptr->fname = argv[1];          //Store the file Name in Structure Member
    sptr->fp = fopen(argv[1], "r"); //Opening File in read mode

    if(sptr->fp == NULL)            //Check if the File is Not Opening 
    {
        printf("File Not Open\n");
        return e_zero;
    }

    Analysis_code();                //Function Call

    fclose(sptr->fp);               //Close the Open File
    return e_zero;
}

void Analysis_code()
{
    printf(T "%s"E" ~>  Start Analysing\n" ,"INFO ");
    //Seting the structure array value has Zero
    memset(sptr->flower, 0, sizeof(sptr->flower));  
    memset(sptr->square, 0, sizeof(sptr->square));
    memset(sptr->bracket, 0, sizeof(sptr->bracket));

    //Store the value -1 in structure Member
    sptr->flow_open = sptr->square_open = sptr->bracket_open = -1;

    sptr->row = 1;                               //Store the Value 1 in structure Member

    while((sptr->ch = fgetc(sptr->fp)) != EOF)   //Check the charcter is Not equal to EOF 
    {
        if(sptr->ch == '#')                      //Checking is character is #
            Preprocessor(sptr);                  //Function call preprocessor

        else if(isalpha(sptr->ch))               //Checking is Character is alphabet
            Keyword_identifier(sptr);            //Function call Keyword_identifier

        else if(sptr->ch == 39)                  //Checking is Character is single quote(')
            Character_constant(sptr);            //Function call

        else if(isdigit(sptr->ch))               //Checking is Character is Digit
            Numerical_constant(sptr);            //Function call

        else if(sptr->ch == 34)                  //Checking is Character is Double Quotes(")
            String_iter(sptr);                   //Function call

        else if(sptr->ch == '/')                 //Checking is Character is slash
            Comment_line(sptr);                  //Function cal

        else if(is_operator(sptr->ch))           //Checking is Character is Operator
            Operator(sptr);                      //Function call

        else if(is_symbol(sptr->ch))             //Checking Character is Symbol (',' , ';')
            Symbol(sptr->ch);                    //Function call

        else if(is_bracket(sptr->ch))            //Checking Character is Bracket
            Bracket(sptr);                       //Function call

        else if(sptr->ch == '\n')                //Checking Character is NewLine
            sptr->row++;                         //Increment the row value of structure Member
    }

    error_parenthisis(sptr , 2);                 //Function Call to Check Bracket Opening and Closing
    printf(G "%s"E" ~>  Analysing Completed\n" , "INFO ");
}


