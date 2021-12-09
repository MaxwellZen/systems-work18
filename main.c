#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define READ 0
#define WRITE 1

int main() {
	int send2child[2], send2parent[2];
	pipe(send2child);
	pipe(send2parent);

	int f = fork();
	if (f) {
		close(send2child[READ]);
		close(send2parent[WRITE]);
		while (1) {
			printf("Enter input: ");
			char line[1000];
			for (int i = 0; i < 1000; i++) line[i] = 0;
			fgets(line, 1000, stdin);
			write(send2child[WRITE], line, strlen(line));
			printf("Output: ");
			for (int i = 0; i < 1000; i++) line[i] = 0;
			read(send2parent[READ], line, sizeof(line));
			printf("%s\n", line);
		}
	} else {
		close(send2child[WRITE]);
		close(send2parent[READ]);
		while (1) {
			char line[1000];
			for (int i = 0; i < 1000; i++) line[i] = 0;
			read(send2child[READ], line, sizeof(line));
			for (int i = 0; line[i]; i++) {
				if (line[i] >= 'a' && line[i] <= 'z') line[i] = line[i] - 'a' + 'A';
			}
			write(send2parent[WRITE], line, strlen(line));
		}
	}
}
