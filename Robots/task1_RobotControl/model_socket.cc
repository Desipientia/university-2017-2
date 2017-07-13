#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/select.h>

#include <thread>
#include <iostream>

#define PORT 9090


namespace gazebo {

    class ModelPush : public ModelPlugin {
        public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/) {
            this->model = _parent;

            std::thread sock (&ModelPush::listenSocket, this);
            sock.detach();

            std::cout << "Load" << std::endl;
        }

        public: void listenSocket() {
            int server_fd, new_socket;
            struct sockaddr_in address;
            int opt = 1;
            int addrlen = sizeof(address);
            char buffer[1024] = {0};

            std::cout << "Variables" << std::endl;

            if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
                perror("socket failed");
                exit(EXIT_FAILURE);
            }

            std::cout << "First if" << std::endl;

            if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
                perror("setsockopt");
                exit(EXIT_FAILURE);
            }

            std::cout << "Second if" << std::endl;

            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY;
            address.sin_port = htons(PORT);

            if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
                perror("bind failed");
                exit(EXIT_FAILURE);
            }

            std::cout << "Third if" << std::endl;

            if (listen(server_fd, 3) < 0) {
                perror("listen");
                exit(EXIT_FAILURE);
            }

            std::cout << "4th if" << std::endl;

            int force = 1000;

            while (1) {
                if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }

                while (1) {
                    recv(new_socket, buffer, 1024, 0);
                    if (buffer[0] == 'f') {
                        this->model->GetLink("link")->AddRelativeForce(math::Vector3(force, 0, 0));
                    } else if (buffer[0] == 's') {
                        this->model->GetLink("link")->AddRelativeForce(math::Vector3(-force, 0, 0));
                    } else if (buffer[0] == 'l') {
                        this->model->GetLink("link")->AddRelativeTorque(math::Vector3(0, 0, force));
                    } else if (buffer[0] == 'r') {
                        this->model->GetLink("link")->AddRelativeTorque(math::Vector3(0, 0, -force));
                    } else if (buffer[0] == 'q') {
                        break;
                    }
                }
            }

        }

        private: physics::ModelPtr model;

        private: event::ConnectionPtr updateConnection;
    };

    GZ_REGISTER_MODEL_PLUGIN(ModelPush)
}
