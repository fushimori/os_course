#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipe1[2]; // Pipe для передачи команд от родительского процесса к дочернему
    int pipe2[2]; // Pipe для передачи данных от дочернего процесса к родительскому
    
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        std::cerr << "Ошибка при создании pipe" << std::endl;
        return 1;
    }
    
    pid_t pid = fork();
    
    if (pid == -1) {
        std::cerr << "Ошибка при создании дочернего процесса" << std::endl;
        return 1;
    }
    
    if (pid == 0) {
        // Дочерний процесс
        
        // Закрытие ненужных концов pipe
        close(pipe1[1]); // Закрытие записывающего конца pipe1
        close(pipe2[0]); // Закрытие читающего конца pipe2
        
        // Перенаправление стандартного входного потока на pipe1
        dup2(pipe1[0], STDIN_FILENO);
        
        // Закрытие ненужных дескрипторов файлов
        close(pipe1[0]);
        close(pipe2[1]);
        
        // Получение имени файла от родительского процесса
        std::string filename;
        std::cin >> filename;
        
        // Открытие файла для записи
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Ошибка при открытии файла" << std::endl;
            return 1;
        }
        
        // Чтение данных от родительского процесса через pipe1 и запись их в файл
        char buffer[256];
        ssize_t bytesRead;
        while ((bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
            file.write(buffer, bytesRead);
        }
        
        // Закрытие файла
        file.close();
        
        // Завершение дочернего процесса
        return 0;
    } else {
        // Родительский процесс
        
        // Закрытие ненужных концов pipe
        close(pipe1[0]); // Закрытие читающего конца pipe1
        close(pipe2[1]); // Закрытие записывающего конца pipe2
        
        // Чтение имени файла от пользователя
        std::string filename;
        std::cout << "Введите имя файла: ";
        std::cin >> filename;
        
        // Открытие файла для записи
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Ошибка при открытии файла" << std::endl;
            return 1;
        }
        
        // Передача имени файла дочернему процессу через pipe1
        write(pipe1[1], filename.c_str(), filename.length());
        
        // Закрытие ненужных дескрипторов файлов
        close(pipe1[1]);
        close(pipe2[0]);
        
        // Перенаправление стандартного вывода на pipe2
        dup2(pipe2[1], STDOUT_FILENO);
        
        // Выполнение команд пользователей
        std::string command;
        std::cout << "Введите команду для дочернего процесса (exit для выхода): ";
        while (command != "exit") {
            std::cin >> command;
            // Передача команды дочернему процессу через pipe1
            write(pipe1[1], command.c_str(), command.length());
        }
        
        // Закрытие файла
        file.close();
        
        // Ожидание завершения дочернего процесса
        int status;
        waitpid(pid, &status, 0);
        
        // Завершение родительского процесса
        return 0;
    }
}