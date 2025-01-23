#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "view.h"
#define LITTLE_TO_BIG_ENDIAN_32(val) ((((val) & 0xFF) << 24) | (((val) & 0xFF00) << 8) | (((val) & 0xFF0000) >> 8) | (((val) & 0xFF000000) >> 24))

//validating the file is mp3 or not
Status validation(viewInfo *MP3)
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

//opening the file
Status open_files(viewInfo *MP3)
{
    //opening a new file 
    MP3->copy_name="new.mp3";
    MP3->copy_point=fopen(MP3->copy_name,"w+");
    MP3->file_point=fopen(MP3->file_name,"r");
    if(MP3->file_point==NULL)
    {
        return e_failure;
    }
}

//copying the header from source file to new file
Status copyheader(viewInfo *MP3)
{
    rewind(MP3->file_point);
    char buffer[10];
    fread(buffer,1,10,MP3->file_point);
    fwrite(buffer,1,10,MP3->copy_point);
}

//checking and copying the tag entered by user to edit
Status copy_tag(viewInfo *MP3)
{
    
    switch (MP3->tag)
    {
    case 't':MP3->edit = "TIT2";
        break;
    case 'a':MP3->edit = "TPE1";
        break;
    case 'A':MP3->edit = "TALB";
        break;
    case 'y':MP3->edit = "TYER";
        break;
    case 'm':MP3->edit = "TCON";
        break;
    case 'c':MP3->edit = "COMM";
        break;
    default:
        break;
    }
}

//copying the contents from original file to new file until the tag entered by user
Status copycontents(viewInfo *MP3)
{
int flag = 1;
while(flag)
{
    char buffer[4];
    fread(buffer,1,4,MP3->file_point);
    if(strcmp(buffer,MP3->edit)==0)
    {
        fwrite(buffer,1,4,MP3->copy_point);
      flag=0;
    }
    else
    {
        fwrite(buffer,1,1,MP3->copy_point);
        fseek(MP3->file_point,-3,SEEK_CUR);
    }
}
}

//checking the length of data enetered by user in CLA
int data_size(viewInfo *MP3)
{
    int len=strlen(MP3->new_name);
    MP3->length=len;
    len++;
    //converting size from little endian to big endian
    len=LITTLE_TO_BIG_ENDIAN_32(len);
    //writing the size to new file
    fwrite(&len,4,1,MP3->copy_point);
    return len;
}

Status new_data(viewInfo *MP3)
{
    char flag[3];
    fread(flag,1,3,MP3->file_point);
    fwrite(flag,1,3,MP3->copy_point);
    //writing new data passed in CLA
    fwrite(MP3->new_name,1,MP3->length,MP3->copy_point);
    printf("Edited successfully\n");
}

//copying remaining data
Status copy_data(viewInfo *MP3)
{
    //skipping the old details of tag in src file
    int size;
    fread(&size,4,1,MP3->file_point);
    size--;
    fseek(MP3->file_point,size,SEEK_CUR);
    //copying all data till end of file
    int ch;
    while((ch = getc(MP3->file_point))!=EOF)
    {
        putc(ch,MP3->copy_point);
    }
}

//to make the changes in the src file
Status save_changes(viewInfo *MP3)
{
    //rewinding the copy pointer
    rewind(MP3->copy_point);
    FILE* fptr = fopen(MP3->file_name,"w");
    int ch;
    while((ch = getc(MP3->copy_point))!=EOF)
    {
        putc(ch,fptr);
    }
    //closing both the pointers
    fclose(MP3->copy_point);
    fclose(fptr);
    //removing the copy file
    remove("new.mp3");
}


Status edit_tags(viewInfo *MP3)
{
    if(validation(MP3) == e_failure)
    {
        return e_failure;
    }
    if(open_files(MP3) == e_failure)
    {
        return e_failure;
    }
    if(copy_tag(MP3) == e_failure)
    {
        return e_failure;
    }
    if(copyheader(MP3) == e_failure)
    {
        return e_failure;
    }
    if(copycontents(MP3) == e_failure)
    {
        return e_failure;
    }
    if(data_size(MP3) == e_failure)
    {
        return e_failure;
    }
    if(new_data(MP3) == e_failure)
    {
        return e_failure;
    }
    if(copy_data(MP3) == e_failure)
    {
        return e_failure;
    }
    if(save_changes(MP3) == e_failure)
    {
        return e_failure;
    }
}