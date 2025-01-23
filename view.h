 typedef enum
{
    e_success,
    e_failure
} Status;
typedef struct view
{
    char *file_name;
    char *type;
    FILE *file_point;
    char *copy_name;
    FILE *copy_point;
    char  *edit;
    char  tag;
    int length;
    char *new_name;
}viewInfo;
//prints user help
Status error_window();
//perform view operation
Status view_tags(viewInfo *MP3);
//perform edit operation
Status edit_tags(viewInfo *MP3);
//check operation type 
Status operation_type(char *argv,viewInfo *MP3);