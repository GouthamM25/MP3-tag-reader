#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "view.h"
#define BIG_TO_LITTLE_ENDIAN_32(val) (((val) >> 24) & 0xFF)

//When CLA not passed correctly
Status error_window()
{
    printf("\n------------------------------------------------------------------\n");
    printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
    printf("USAGE :\nTo view please pass like: ./a.out -v mp3filename\n");
    printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
    printf("To get help pass like : ./a.out --help\n");
    printf("------------------------------------------------------------------\n\n");
}

//validating the file extension is MP3 or not
Status validate(viewInfo *MP3)
{
    char* str = strchr(MP3->file_name,'.');
    if(str == NULL)
    {
        printf("ERROR: %s is not a .mp3 file\n",MP3->file_name);
        return e_failure;
    }
    else if(strcmp(str,".mp3")!=0)
    {
        printf("ERROR: %s is not a .mp3 file\n",MP3->file_name);
        return e_failure;
    }
}

//opening the file after validation
Status open_file(viewInfo *MP3)
{
    //copying the MP3 file name to the structure
    MP3->file_point=fopen(MP3->file_name,"r");
    if (MP3->file_point == NULL)
    {
        printf("Error: Could not open file %s\n", MP3->file_name);
        return e_failure;
    }
    return e_success;
}

//Checking ID3 or not
Status check_ID3(FILE* fptr)
{
    char buffer[4];
    fread(buffer,1,3,fptr);
    if(strcmp(buffer,"ID3"))
    {
        printf("ERROR: ID3 tag does not exist\n");
        return e_failure;
    }
}

//checking version is v2 or not
Status check_version(FILE* fptr)
{
    short version=0;
    fread(&version,2,1,fptr);
    if(version!=3)
    {
        printf("ERROR: Not a file of version v2\n");
        return e_failure;
    }
}

//skipping the first 10 bytes of header
Status skip_header(viewInfo *MP3)
{
    fseek(MP3->file_point,10,SEEK_SET);
}

//extracting the tag details from file and performing view operation
Status tag_details(viewInfo *MP3)
{
    printf("------------------------SELECTED VIEW--------------------------\n");
    printf("---------------------------------------------------------------\n");
    printf("\t\t MP3 TAG READER AND EDITOR FOR ID3v2\n");
    printf("---------------------------------------------------------------\n");
    for(int i=0;i<6;i++)
    {
    char tag[4];
    //reading and comparing tags to print in a format
    fread(tag,1,4,MP3->file_point);
    if(strcmp(tag,"TIT2") == 0)
    {
        printf("Title\t:\t");
    }
    else if(strcmp(tag,"TPE1") == 0)
    {
        printf("Artist\t:\t");
    }
    else if(strcmp(tag,"TALB") == 0)
    {
        printf("Album\t:\t");
    }
    else if(strcmp(tag,"TYER") == 0)
    {
        printf("Year\t:\t");
    }
    else if(strcmp(tag,"TCON") == 0)
    {
        printf("Music\t:\t");
    }
    else if(strcmp(tag,"COMM") == 0)
    {
        printf("Comment\t:\t");
    }
    int size;
    fread(&size,4,1,MP3->file_point);
    //converting the size of data from big endian to little endian
    int size1=BIG_TO_LITTLE_ENDIAN_32(size);
    fseek(MP3->file_point,3,SEEK_CUR);
    char title[size1];
    memset(title,0,size1);
    fread(title,1,size1-1,MP3->file_point);
    printf("%s \n",title);
    }   
    printf("---------------------------------------------------------------\n");
}

Status view_tags(viewInfo *MP3)
{
    if(validate(MP3) == e_failure)
    {
        return e_failure;
    }
    if (open_file(MP3) == e_failure)
    {
        return e_failure;
    }
    if (check_ID3(MP3->file_point) == e_failure)
    {
        return e_failure;
    }
    if (check_version(MP3->file_point) == e_failure)
    {
        return e_failure;
    }
    if(skip_header(MP3) == e_failure)
    {
        return e_failure;
    }
    if(tag_details(MP3) == e_failure)
    {
        return e_failure;
    }
}