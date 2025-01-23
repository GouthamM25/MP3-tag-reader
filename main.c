#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"view.h"

viewInfo MP3;

int main(int argc,char *argv[])
{
    //if CLA is less than 3
   if(argc<3)
   {
    error_window();

       if(argc != 1)
       operation_type(argv[1],&MP3);
       
       return e_failure;
    }
    //when CLA is = 3
    else if(argc == 3)
    {
        //copying the filename to the structure
     MP3.file_name=argv[2];
     operation_type(argv[1],&MP3);
    }

    //when length of CLA is 5
    else if(argc == 5)
    {
       MP3.tag=argv[2][1];
       MP3.file_name=argv[4];
       MP3.new_name=argv[3];
       operation_type(argv[1],&MP3);
    }
    //for invalid arguments
    else
    {
        printf("ERROR: Invalid arguments\n");
    }
}

//checking what type of operation to be performed
Status operation_type(char *argv,viewInfo *MP3)
{
    if(strcmp(argv,"--help")==0)
    {
    printf("\n-------------------------HELP MENU------------------------\n");
    printf("1. -v -> to view mp3 file contents\n2. -e -> to edit mp3 file contents\n");
    printf("\t 2.1. -t -> to edit song title\n\t 2.2. -a -> to edit artist name\n\t 2.3. -A -> to edit album name\n");
    printf("\t 2.4. -y -> to edit year\n\t 2.5. -m -> to edit content\n\t 2.6. -c -> to edit comment\n");
    printf("------------------------------------------------------------\n\n");
    return e_failure;
    }
    //for viewing the details
    else if(strcmp(argv,"-v")==0)
    {
        if(view_tags(MP3) == e_failure)
        {
            return e_failure;
        }
    }
    //for editing 
    else if(strcmp(argv,"-e")==0)
    {
        if(edit_tags(MP3)==e_failure)
        {
            return e_failure;
        }
    }
    //for invalid arguments
    else
    {
        printf("ERROR: Invalid arguments\n");
    }
}