#pragma once

// 终端节点的创建
extern int client_end_point();
extern int server_end_point();

// 创建 socket
extern int create_tcp_socket();
extern int create_acceptor_socket();

// 绑定 acceptor
extern int bind_accptor_socket();

// 连接指定的端点
extern int connect_to_end();
