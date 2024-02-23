#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <directory_name>\n", argv[0]);
        return 1;
    }

    const char *dir_path = argv[1];
    int result = mkdir(dir_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    //S_IRWXU：用户（文件所有者）具有读、写和执行权限。
    //S_IRWXG：组具有读、写和执行权限。
    //S_IROTH：其他用户具有读权限。
    //S_IXOTH：其他用户具有执行权限。
    if (result == -1) {
        perror("Error creating directory");
        return 1;
    }

    printf("Directory created successfully: %s\n", dir_path);
    return 0;
}
