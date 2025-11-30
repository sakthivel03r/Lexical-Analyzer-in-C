#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "lexical.h"

void print(Analysis * sptr , char buffer[])
{
    int size = strlen(buffer);
    
    printf("%5d| %*s"R "%s"E"\n" , sptr->row ,8 , "" , buffer );
    printf("%6c"R "%*c", '|' , 10 , '^');
    
    for(int i = 0 ; i < size - 2 ; i++)                      //Running Loop upto buffer size -2 
        printf("%c", '~');
    
    printf(E "\n");
    exit(1);                                                 //Function call Exit
}

Result error_character(Analysis * sptr , char buffer[])
{
    int size = strlen(buffer);
    if(size != 3 || buffer[size -1] == '\n')  //Checking buffer size is not equal to 3 or buffer end with Newline 
    {
        if(buffer[size -1] == '\n')           //Checking Buffer Last character is equal to NewLine
        {
            printf("%s:%d: "R "%s"E":%s %c %s\n" , sptr->fname , --sptr->row,"error", "missing terminating" , 39 , "character");
            buffer[size - 1] = '\0';
            
            print(sptr ,buffer);
            
            sptr->row++;
        }

        else if(buffer[1] != 92)              //Checking Buffer First character is not equal backslash(\); 
        {
            printf("%s:%d: "R "%s"E":%s\n" , sptr->fname , sptr->row ,"error", "multi-character character constant");
            print(sptr ,buffer);
        }

        else
            return e_nonzero;                 //return 1 if no error
    }

    else
        return e_nonzero;                     //return 1 if no error
}

Result error_string_iter(Analysis * sptr , char buffer[])
{
    int size = strlen(buffer);
    if(buffer[size-1] == '\n')                //Checking Buffer last Character is NewLine
    {
        printf("%s:%d: "R "%s"E":%s %c %s\n" , sptr->fname , --sptr->row ,"error", "missing terminating" , 34 , "character");
        buffer[size - 1] = '\0';
        
        print(sptr , buffer);
        
        sptr->row++;
    }

    else
        return e_nonzero;                     //return 1 if no error
}

Result error_comment_line(Analysis *sptr, char buffer[])
{
    int size = strlen(buffer);
    
    //Checking Second Character is * & checking Last two character is not equal to */
    if(buffer[1] == '*' && !(buffer[size-2] == '*' && buffer[size-1] == '/'))
    {
        printf("%s:%d: "R "%s"E":%s\n", sptr->fname, sptr->row, "error", "unterminated comment");
     
        for(int i = 0; i < size; i++) //Running Loop upto size times
        {
            if(buffer[i] == '\n')     //Checking Buffer character is equal to Newline & break the loop
            {
                buffer[i] = '\0';
                break;
            } 
        }

        print(sptr , buffer);
    }

    return e_nonzero;                 //return value 1 if no error
}

Result error_operator(Analysis * sptr , char buffer[] , int type)
{
    //Store each operator possiblity in one 2D array
    char * operator[] = {"+","-","*","/","%","++","--","=","+=","-=","*=","/=","%=","<<=",">>=","&=","^=","|=","==","!=",">","<",">=","<=",
        "&&","||","!","&","|","^","~","<<",">>","?",":",".","->"};

    //Finding size and Store in Variable
    int total_size = sizeof(operator) / sizeof(operator[0]);

    int i;
    for(i = 0 ; buffer[i] ; i++)              //running loop upto Null character
    {
        if(strchr("#@`$", buffer[i]))         //Checking Character present in string using strchr
        {
            printf("%s:%d: "R "%s"E":%s %c%c%c %s\n" , sptr->fname , sptr->row ,"error","stray",39 , buffer[i] , 39 ,"in program");
            print(sptr , buffer);
        }

    }

    for(i = 0; i < total_size; i++)           //Running loop upto total size -1 
    {
        if(strcmp(buffer , operator[i]) ==0 ) //Checking buffer is matching with 2D array by using strcmp
            return e_nonzero;
    }

    printf("%s:%d: "R "%s"E":%s\n" , sptr->fname , sptr->row ,"error","invalid operator");
    print(sptr , buffer);
}

Result error_identifier(Analysis * sptr , char buffer[] , int iden)
{
    int i = 0 , flag = 0;
    if(iden == 1)                         //Checking iden is equal to 1
    {
        while(buffer[i++])                //running loop upto Null characetr
        {
            if(is_stray(buffer[i]))       //Checking buffer character is stary(@ , # , $ , `);
            {
                printf("%s:%d: "R "%s"E":%s %c%c%c %s\n" , sptr->fname , sptr->row ,"error","stray",39 , buffer[i] , 39 ,"in program");
                print(sptr , buffer);
            }
        }
    }

    if(iden == 2)                        //Checking iden is equal to 2
    {
        printf("%s:%d: "R "%s"E":%s %c" , sptr->fname , sptr->row ,"error","invalid suffix",'"');
        while(buffer[i])                 //Running Loop upto Null character
        {
            if(flag)                     //Checking Flag is Non zero
                printf("%c" , buffer[i]);

            if(flag)                     //Checking Flag is Non zero
            {
                if(isalpha(buffer[i]))   //Checking buffer character is alphabet
                {
                    printf("%c", buffer[i]);
                    flag = 1;            //assign flag is 1
                }
            }

            i++;                         //increment i
        }

        printf("%c %s\n", 34 , "on integer constant");
        print(sptr , buffer);
    }

    return e_nonzero;                    //return 1 if no error found
}

Result error_float(Analysis * sptr , char buffer[])
{
    int size = strlen(buffer);
    if(isalpha(buffer[size - 1]))                               //Checking buffer last before character is alphabet
    {
        if(buffer[size - 1] != 'f' || buffer[size - 1] != 'F')  //Checking buffer last before character is upper or lowercase f
        {
            printf("%s:%d: "R "%s"E":%s %c%c%c %s\n",sptr->fname,sptr->row,"error","invalid suffix",34,buffer[size-1],34,"on floating constant");
            print(sptr , buffer);
        }

        else
            return e_nonzero;                                   //return 1 if buffer last before character is upper or lowercase f
    }

    int i = 0 , count = 0;
    while(buffer[i++])                                          //running loop upto Null character
    {
        if(buffer[i] == '.')                                    //checking Character is equal to dot
            count++;                                            //Increment the count
    }

    if(count != 1)                                              //Checking Count is Not equal to 1
    {
        printf("%s:%d:"R " %s"E" %s\n" , sptr->fname , sptr->row ,"error","too many decimal points in number");
        print(sptr , buffer);
    }

    return e_nonzero;                                           //return 1 if No error
}

Result error_hexa(Analysis * sptr , char buffer[])
{
    int i = 0 , flag = 0;
    //Checking buffer first two character is '0' & either uppercase x or lowercase x
    if(!(buffer[0] == '0' && (buffer[1] == 'x' || buffer[1] == 'X')))
        flag = 1;                       //assign flag value 1

    if(!flag)                           //Checking flag is Non zero
    {
        i = 2;
        while(buffer[i])                //running loop upto Null character
        {
            if(!isxdigit(buffer[i]))    //Checking buffer character is hexadigit using isxdigit function
            {
                flag = 1;               //assing flag value 1 & break the loop
                break;
            }
            i++;
        }
    }

    if(flag)                            //Checking flag is Non zero
    {
        printf("%s:%d: "R "%s"E":%s %c" , sptr->fname , sptr->row ,"error","invalid suffix", 34);

        while(buffer[i])                //running loop upto Null character
            printf("%c" , buffer[i++]);

        printf("%c %s\n", '"' , "on integer constant");
        print(sptr , buffer);
    }

    else
        return e_nonzero;               //return 1 if no error
}

Result error_octal(Analysis * sptr , char buffer[])
{
    char max = '0'; int flag = 0 , i = 0;
    while(buffer[i])                                   //running loop upto Null character
    {
        if(!(buffer[i] >= '0' && buffer[i] <= '7'))    //Checking Character is not between 0 and 7
        {
            if(buffer[i] > max)                        //Checking character is greater than max
                max = buffer[i];                       //store that character in max

            flag = 1;                                  //Making falg Non zero
        }
        i++;
    }

    if(flag)                                           //Checking Flag is Non zero
    {
        printf("%s:%d: "R "%s"E":%s %c%c%c %s\n",sptr->fname,sptr->row,"error","invalid digit",'"' ,max,'"',"on octal constant");
        print(sptr , buffer);
    }

    else
        return e_nonzero;                              //return 1 if no error
}

Result error_binary(Analysis * sptr , char buffer[])
{
    int i = 0 , flag = 0;
    //Checking first two character is 0 and either uppercase b or lowercase b
    if(!(buffer[0] == '0' && (buffer[1] == 'B' || buffer[1] == 'b')))  
        flag = 1;                                          //Making flag Non zero

    if(!flag)                                              //Checking Falg is Non zero
    {
        i = 2;
        while(buffer[i])                                   //Running loop upto Null character
        {
            if(!(buffer[i] == '1' || buffer[i] == '0'))    //checking character is either equal to 1 or 0
            {
                flag = 1;                                  //Making flag Non zero & break
                break;
            }

            i++;
        }
    }

    if(flag)                                               //Checking flag is Non zero
    {
        printf("%s:%d: "R "%s"E":%s %c" , sptr->fname , sptr->row ,"error","invalid suffix",'"');
        while(buffer[i])                                   //running loop upto Null character
            printf("%c" , buffer[i++]);

        printf("%c %s\n", '"' , "on integer constant");
        print(sptr , buffer);
    }

    else
        return e_nonzero;                                  //return 1 if non error
}

Result error_parenthisis(Analysis * sptr , int out)
{
    char buffer[2];
    if(out == 1)                                                  //Checking out equal to 1
    {
        //Checking Flower or Square or Bracket position first is greater than second
        if((sptr->flower[0] < sptr->flower[1]) || (sptr->square[0] < sptr->square[1] ) || (sptr->bracket[0] < sptr->bracket[1]))
        {
            printf("%s:%d: "R "%s"E":%s '%c' %s\n" , sptr->fname , sptr->row ,"error","expected statement before", sptr->ch ,"token");
            buffer[0] = sptr->ch; buffer[1] = '\0';
            print(sptr , buffer);
        }
    }

    if(out == 2)                                                  //Checking out equal to 2
    {
        if(sptr->flower[0] > sptr->flower[1])                     //Checing Flower position First is Greater than secoond
        {
            sptr->ch = '{';                                       //assign '{' in structure Member
            sptr->row = sptr->flow_pos[sptr->flow_open];          //assign value of Flower array position of flow_open in row
        }

        else if(sptr->square[0] > sptr->square[1] )               //Checing square position First is Greater than secoond
        {
            sptr->ch = '[';                                       //assign '[' in structure Member
            sptr->row = sptr->square_pos[sptr->square_open];      //assign value of square array position of square_open in row
        }

        else if(sptr->bracket[0] > sptr->bracket[1])              //Checing square position First is Greater than secoond
        {
            sptr->ch = '(';                                       //assign '[' in structure Member
            sptr->row = sptr->bracket_pos[sptr->bracket_open];    //assign value of square array position of square_open in row
        }

        if(sptr->ch == '{' || sptr->ch == '[' || sptr->ch == '(') //Checking Character is eithier one of this '{','[','('
        {
            printf("%s:%d: "R "%s"E":%s '%c' %s\n" , sptr->fname , sptr->row ,"error","not closed ", sptr->ch ,"token");
            buffer[0] = sptr->ch; buffer[1] = '\0';
            print(sptr , buffer);
        }
    }
    return e_nonzero;                                             //return 1 if nonerror
}

