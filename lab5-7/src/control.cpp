#include <zmq.h>
#include "binTree.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <regex>
#include <chrono>
#include <thread>


int main() {

    void* context = zmq_ctx_new();
    void* socket = zmq_socket(context, ZMQ_PUB);
    zmq_bind(socket, "tcp://0.0.0.0:5555");

    BinarySearchTree tree;
    tree.createNode(-1);

    std::regex createReg("^create ([0-9]+)$");
    std::regex execSaveReg("^exec ([0-9]+) ([A-Za-z0-9]+) ([0-9]+)$");
    std::regex execLoadReg("^exec ([0-9]+) ([A-Za-z0-9]+)$");
    std::regex killReg("^kill ([0-9]+)$");
    std::regex heartbitReg("^heartbit ([0-9]+)$");
    std::regex printReg("^print$");
    std::regex exitReg("^exit$");
    std::smatch matches;

    std::cout << "Started" << std::endl;
    int timebreak;
    std::string cmd;
    while (true){
        timebreak = 500;
        std::cout << "Enter command: ";
        std::getline(std::cin, cmd);
        //std::cout << '\n';

        //std::cout << "cmd: " << cmd << "" << std::endl;

        if (std::regex_match(cmd, matches, createReg)) {

            int id = std::stoi(matches[1]);
            std::string answer = tree.createNode(id);
            int pos = tree.findParentId(id);

            //std::cout << "POS" << pos << std::endl;
            if(pos == -1){
                if(answer == "Ok:"){
                    pid_t pid = fork();
                    if (pid == 0) {
                        execl("./calculate", "calculate", std::to_string(id).c_str(), NULL);
                        std::cout << "Error execl" << std::endl;
                        exit(1);
                    }
                    std::cout << answer << " " << pid << std::endl;
                    
                }
                else{
                    std::cout << answer << std::endl;
                }
            }
            else{
                if(answer == "Ok:"){
                    //std::cout << "CREATE IN DAD" << std::endl;
                    std::string msg = "create " + std::to_string(pos) + " " + std::to_string(id);
                    zmq_send(socket, msg.c_str(), msg.length(), 0);
                }
                else{
                    std::cout << answer << std::endl;
                }
            }
        }

        else if (std::regex_match(cmd, matches, execSaveReg)) {
            int id = std::stoi(matches[1]);

            std::string answer = tree.findNode(id);
            if(answer == "Ok"){
                std::string msg = "save " + matches[1].str() + " " + matches[2].str() + " " + matches[3].str();
                zmq_send(socket, msg.c_str(), msg.length(), 0);
            }
            else{
                std::cout << answer << std::endl;
            }

        }

        else if (std::regex_match(cmd, matches, execLoadReg)) {
            int id = std::stoi(matches[1]);

            std::string answer = tree.findNode(id);
            if(answer == "Ok"){
                std::string msg = "load " + matches[1].str() + " " + matches[2].str();
                zmq_send(socket, msg.c_str(), msg.length(), 0);
            }
            else{
                std::cout << answer << std::endl;
            }
        }

        else if (std::regex_match(cmd, matches, heartbitReg)) {
            int time = std::stoi(matches[1]);
            timebreak = time *4;
            std::string msg = "heartbit " + std::to_string(time);
            zmq_send(socket, msg.c_str(), msg.length(), 0);
        }

        else if (std::regex_match(cmd, matches, killReg)) {
            int id = std::stoi(matches[1]);
            std::cout << tree.killNodeAndChildren(id, socket) << std::endl;
        }

        else if (std::regex_match(cmd, matches, printReg)) {
            tree.printTree();
        }

        else if (std::regex_match(cmd, matches, exitReg)) {
            std::cout << "Exit" << std::endl;
            zmq_send(socket, "exit", 4, 0);
            break;
        
        }

        else{
            std::cout << "Wrong command" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(timebreak));
    }

    zmq_close(socket);
    zmq_ctx_destroy(context);

    return 0;
}