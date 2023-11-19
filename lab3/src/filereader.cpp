#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *fname = "toji";
    int fdin = open(fname, O_RDONLY); // Открываем файл в режиме чтения

    if (fdin == -1) {
        perror("open error");
        return 1;
    }

    int buffer[5];
    ssize_t bytesRead = read(fdin, buffer, sizeof(buffer));
    
    if (bytesRead == -1) {
        perror("read error");
        return 1;
    }

    close(fdin); // Закрываем файл

    // Печатаем прочитанные данные
    for (int i = 0; i < bytesRead / sizeof(int); i++) {
        printf("%d\n", buffer[i]);
    }

    return 0;
}