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

#define PORT 8080


namespace gazebo {

    class ModelPush : public ModelPlugin {
        public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/) {
            this->model = _parent;
            topicName = "/gazebo/default/Robot5/hokuyo/link/laser/scan";

            transport::NodePtr node(new transport::Node());
            node->Init("default");
            mySubscriber = node->Subscribe(topicName, &ModelPush::OnUpdate, this);

            std::thread sock (&ModelPush::listenSocket, this);
            sock.detach();
        }

        public: void OnUpdate(ConstLaserScanStampedPtr & msg) {
            if (!new_socket) return;

            int size = msg->scan().ByteSize();
            void * buffer = malloc(size);
            msg->scan().SerializeToArray(buffer, size);

            send(new_socket, buffer, size, 0);

            std::cout << size << std::endl;

            /*std::cout << "==============================" << std::endl;
            std::cout << "frame: " << msg->scan().frame() << std::endl;
            std::cout << "angle_min: " << msg->scan().angle_min() << std::endl;
            std::cout << "angle_max: " << msg->scan().angle_max() << std::endl;
            std::cout << "angle_step: " << msg->scan().angle_step() << std::endl;
            std::cout << "range_min: " << msg->scan().range_min() << std::endl;
            std::cout << "range_max: " << msg->scan().range_max() << std::endl;
            std::cout << "count: " << msg->scan().count() << std::endl;
            std::cout << "==============================" << std::endl;*/



            /*for (int i = 0; i < msg->scan().ranges().size(); i++) {
                //std::string str << "" + i + "=" + msg->scan().ranges().Get(i);
                std::cout << "H: " << i << msg->scan().ranges().data() << std::endl;
            }*/
        }

        public: void listenSocket() {
            int server_fd;
            struct sockaddr_in address;
            int opt = 1;
            int addrlen = sizeof(address);
            char buffer[1024] = {0};
      
            // Creating socket file descriptor
            if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
                perror("socket failed");
                exit(EXIT_FAILURE);
            }
              
            // Forcefully attaching socket to the port 8080
            if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
                perror("setsockopt");
                exit(EXIT_FAILURE);
            }

            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY;
            address.sin_port = htons(PORT);
              
            // Forcefully attaching socket to the port 8080
            if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
                perror("bind failed");
                exit(EXIT_FAILURE);
            }

            if (listen(server_fd, 3) < 0) {
                perror("listen");
                exit(EXIT_FAILURE);
            }

            while (1) {
                if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
              
                while (1) {
                    recv(new_socket, buffer, 1024, 0);

                    if (buffer[0] == 'f') {
                        this->model->GetLink("link_0")->AddRelativeForce(math::Vector3(10000, 0, 0));
                    } else if (buffer[0] == 's') {
                        this->model->GetLink("link_0")->AddRelativeForce(math::Vector3(-10000, 0, 0));
                    } else if (buffer[0] == 'l') {
                        this->model->GetLink("link_0")->AddRelativeTorque(math::Vector3(0, 0, 1000));
                    } else if (buffer[0] == 'r') {
                        this->model->GetLink("link_0")->AddRelativeTorque(math::Vector3(0, 0, -1000));
                    } else if (buffer[0] == 'q') {
                        break;
                    }
                }
            }

        }

        private: physics::ModelPtr model;

        private: int new_socket;

        private: std::string topicName;
        private: transport::SubscriberPtr mySubscriber;

        private: event::ConnectionPtr updateConnection;
    };

    GZ_REGISTER_MODEL_PLUGIN(ModelPush)
}
