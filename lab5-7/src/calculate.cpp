#include <zmq.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <map> 
#include <sstream>
#include <thread>
#include <chrono>

int main(int argc, char** argv) {
    
    void* context = zmq_ctx_new();
    void* socket = zmq_socket(context, ZMQ_SUB);
    zmq_connect(socket, "tcp://0.0.0.0:5555");
    zmq_setsockopt(socket, ZMQ_SUBSCRIBE, "", 0);

    int ID = std::stoi(argv[1]);
    //std::cout << "bEBRA" << ID << std::endl;
    std::map<std::string, int> values;  

    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        //std::cout << "Not Received: "<< std::endl;
        zmq_recv(socket, buffer, sizeof(buffer), 0);
        std::string message(buffer);

        //std::cout << "Received: " << ID << ":" << message << std::endl;
        
        std::istringstream iss(message);
        std::string command;
        iss >> command;

        if (command == "create") {
            int pos;
            iss >> pos;
            if(ID == pos){
                //std::cout << "Creating in " << ID <<  std::endl;
                std::string id;
                iss >> id;
                pid_t pid = fork();
                if (pid == 0) {
                    //std::cout << "NEW FORK: " << std::endl;
                    execl("./calculate", "calculate", id.c_str(), NULL);
                    std::cout << "Error execl" << std::endl;
                    exit(1);
                }
                std::cout << "Ok: " << pid << std::endl;
            }
        }

        else if (command == "save") {
            //std::cout << "Saving: " << ID << std::endl;
            int id;
            iss >> id;
            if(ID == id){
                std::string name;
                int value;
                iss >> name >> value;

                values[name] = value;
                std::cout << "Ok:" << id << std::endl;
            }
        }

        else if (command == "load") {
            //std::cout << "Loading: " << ID << std::endl;
            int id;
            iss >> id;
            if(ID == id){
                std::string name;
                iss >> name;

                if (values.count(name) > 0) {
                    int value = values[name];
                    std::cout << "Ok:" << id << ": " << value << std::endl;
                }
                else{
                    std::cout << "Error:" << id << ": " << name << std::endl;
                }
            }
        }

        else if (command == "heartbit") {
            int time;
            iss >> time;
            for(int i = 0; i < 4; i++){
                std::cout << "Heartbit: " << ID << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(time));
            }
        }
        else if (command == "kill") {
            int id;
            iss >> id;
            if(ID == id){
                std::cout << "Killed:" << ID << std::endl;
                break;
            }
        }

        else if (command == "exit") {
            break;
        }
    }

    // Закрытие сокета и освобождение ресурсов
    zmq_close(socket);
    zmq_ctx_destroy(context);

    return 0;
}