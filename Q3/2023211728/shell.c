#include "head.h"


// 判断cd命令输入的参数个数是否合法
bool argumentNum(char *cmd, int num, char *tips) {
	int arg = 0;
	// 统计空格数，cd命令空格数应该是1
	for (int i = 0; i < strlen(cmd); i++) {
		if (cmd[i] == ' ') arg++;
	}
	if (arg != num) {
		fprintf(stderr, "Usage: %s", tips); //stderr是标准错误流
		return false;
	}
	return true;
}

// pwd函数
void mypwd() {
	char dirbuf[100];
	getcwd(dirbuf, 100);
	printf("%s\n", dirbuf);
	return;
}

// cd函数
void mycd(char *cmd) {
	char *pathname = strstr(cmd, " ") + 1; //找到字符串cmd中第一个空格字符的位置，并将该位置的下一个字符的指针赋值给pathname
	// 判断是否有此目录（能否打开）
	if ((opendir(pathname)) == NULL) {
		perror("opendir"); //当系统调用失败时，perror函数可以打印出相应的错误信息
		return;
	}
	// 改变工作目录
	chdir(pathname);
	return;
}

// 子进程调用sh来执行其他函数ls ps等
void mysh(char *cmd) {
	pid_t pid; //在父进程中，fork返回新创建子进程的进程ID；在子进程中，fork返回0；如果出现错误，fork返回一个负值；
	char *cmdp = NULL;
	if ((pid = fork()) < 0) {
		perror("fork");
		return;
	}
	if (pid == 0) {
		int fd=-1;
		if (strstr(cmd, "2>>") != NULL) {
			char *redir = strstr(cmd, "2>>");
			*redir = '\0';
			redir += 3;
			fd = open(redir, O_WRONLY | O_CREAT | O_APPEND , 0644);
			if (fd == -1) {
				perror("open");
				return;
			}
			dup2(fd, STDERR_FILENO);
			close(fd);
		} else if (strstr(cmd, "&>>") != NULL) {
			char *redir = strstr(cmd, "&>>");
			*redir = '\0';
			redir += 3;
			fd = open(redir, O_WRONLY | O_CREAT | O_APPEND , 0644);
			if (fd == -1) {
				perror("open");
				return;
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		} else if (strstr(cmd, "2>") != NULL) {
			char *redir = strstr(cmd, "2>");
			*redir = '\0';
			redir += 2;
			fd = open(redir, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1) {
				perror("open");
				return;
			}
			dup2(fd, STDERR_FILENO);
			close(fd);
		} else if (strstr(cmd, "&>") != NULL) {
			char *redir = strstr(cmd, "&>");
			*redir = '\0';
			redir += 2;
			fd = open(redir, O_WRONLY | O_CREAT, 0644);
			if (fd == -1) {
				perror("open");
				return;
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		} else if (strstr(cmd, ">>") != NULL) {
			char *redir = strstr(cmd, ">>");
			*redir = '\0';
			redir += 2;
			fd = open(redir, O_WRONLY | O_CREAT | O_APPEND , 0644);
			if (fd == -1) {
				perror("open");
				return;
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}

		else if (strstr(cmd, ">") != NULL) {
			char *redir = strstr(cmd, ">");
			*redir = '\0';
			redir += 1;
			fd = open(redir, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1) {
				perror("open");
				return;
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (strstr(cmd, "./") != NULL) {
                        char *redir = strstr(cmd, "./");
                        *redir = '\0';
                        redir += 2;
                        if (execv(redir, NULL) < 0) {
                        perror("execv");
                        return;
            }
        }


		if ((cmdp = strstr(cmd, "ls")) != NULL) {
			if (execl("/home/yt/桌面/indoor/ls", "ls", NULL) < 0) {
				perror("execl");
				return;
			}
		}

		else if ((cmdp = strstr(cmd, "mkdir")) != NULL) {
			execl("/home/yt/桌面/indoor/mkdir", "mkdir", cmd,NULL);
			return;
		}
	} else {
		wait(NULL); //等待子进程结束，这个wait还蛮重要的，避免了“僵尸进程”和“孤儿进程”
		return;
	}
}


//O_WRONLY：表示以只写模式打开文件。
//O_CREAT：表示如果文件不存在，则创建该文件。
//O_APPEND：表示将数据追加到文件末尾，而不是覆盖原有内容。
//O_TRUNC：表示将数据覆盖到文件
//0644：表示设置文件的权限为可读写（用户、组和其他人），具体来说，用户具有读/写权限，组和其他人只有读权限。


int main(int argc, char **argv) {
	char cmd[100];
	while (1) {
		printf("$ ");
		scanf("%[^\n]s", cmd); //使用正则表达式直到读取到回车才停下，若使用%s遇到空格就会停止读取
		getchar();
		char *cmdp = NULL;
		int type = 0;
		if ((cmdp = strstr(cmd, "pwd")) != NULL) {
			mypwd();
		} else if ((cmdp = strstr(cmd, "cd")) != NULL) {
			if (argumentNum(cmd, 1, "cd dirname")) {
				mycd(cmd);
			} else continue;
		} else if ((cmdp = strstr(cmd, "exit")) != NULL) {
			exit(0);
		} else {


			mysh(cmd);
		}
	}
	return 0;
}


