#include <sys/types.h> 
#include <dirent.h> 
#include <stdio.h> 
#include <sys/stat.h> 
#include <stdlib.h> 
#include <string.h> 


void print_directory_info(struct dirent *result, char *path_name)
{
    char copy_path_name[1024]; 
    int stat_result; 
    struct stat full_data; 

    strcpy(copy_path_name, path_name); 
    strcat(copy_path_name, result->d_name); 

    stat(copy_path_name, &full_data); 

    printf("inode: %d \n", result->d_ino); 
    printf("type: %d \n", result->d_type); 
    printf("file name: %s \n", result->d_name); 
    printf("owner: %d \n", full_data.st_uid); 
    printf("file size: %d \n", full_data.st_size);

    return;  
}

int main(int argument_number, char **argument_value)
{
    DIR *fd; 
    struct dirent *result; 
    char dir[1024]; 
    int len; 

    if (argument_number < 2){
        strcpy(dir, "./");
    } else {
        strcpy(dir, argument_value[1]); 
    }

    len = strlen(dir); 
    if (dir[len - 1] != '/') 
        strcat(dir, "/"); 

    fd = opendir(dir); 
    if (fd == NULL) {
        printf("error \n");
    } else {
        printf("success \n");

        while ((result = readdir(fd)) != NULL){
            print_directory_info(result, dir);
        }
    } 

    return 0; 
}