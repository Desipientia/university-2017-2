#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#include <thread>
#include <iostream>

#define PORT 8080

namespace gazebo {
    class ModelPush : public ModelPlugin {
        public: void Load(physics::ModelPtr _parent, sdf::ElementPtr) {
            this->model = _parent;

            std::thread sock (&ModelPush::listenSocket, this);
            sock.detach();
            // std::cout << "Load" << std::endl;
        }

        public: void OnUpdate(const common::UpdateInfo &) {
            this->model->SetLinearVel(math::Vector3(.5, 0, 0));
        }

        public: void listenSocket() {
            int server_fd, new_socket, valread;
            struct sockaddr_in address;
            int opt = 1;
            int addrlen = sizeof(address);
            char buffer[1024] = {0};
            char* hello = "Hello!";

            if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
                perror("socket failed");
                exit(EXIT_FAILURE);
            }

            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY;
            address.sin_port = htons(PORT);

            if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
                perror("bind failed");
                exit(EXIT_FAILURE);
            }

            if (listen(server_fd, 3) < 0) {
                perror("listen failed");
                exit(EXIT_FAILURE);
            }

            while (1) {
                if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                    perror("accept failed");
                    exit(EXIT_FAILURE);
                }

                recv(new_socket, buffer, 1024, 0);

                if (buffer[0] == 'r') {
                    std::cout << "run" << std::endl;
                    this->updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&ModelPush::OnUpdate, this, _1));
                } else {
                    std::cout << "stop" << std::endl;
                    this->model->SetLinearVel(math::Vector3(0, 0, 0));
                }

                send(new_socket, hello, strlen(hello), 0);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }

        }

        private: physics::ModelPtr model;

        private: event::ConnectionPtr updateConnection;
    }

    GZ_REGISTER_MODEL_PLUGIN(ModelPush);
}
