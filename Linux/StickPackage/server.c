#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"

void sendMsg(int fd, const char* data, int length)
{
    // 申请内存来存储数据包
    char* buf = (char*)malloc(length + sizeof(int));
    // 将数据包长度转换为网络字节序
    int netlen = htonl(length);
    // 拷贝数据
    memcpy(buf, &netlen, sizeof(int));
    memcpy(buf + sizeof(int), data, length);
    // 发送数据
    writen(fd, buf, length + sizeof(int));
}

int main()
{
    // 1.创建用于监听的套接字
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1)
    {
        perror("socket");
        exit(1);
    }

    // 2.绑定本地ip和port
    struct sockaddr_in addr_server;
    addr_server.sin_family = AF_INET;                  // ipv4
    addr_server.sin_addr.s_addr = INADDR_ANY;          // <-> = 0
    addr_server.sin_port = htons(8989);                // 主机字节序 -> 网络字节序
    int ret = bind(listen_fd, (struct sockaddr*)&addr_server, sizeof(addr_server));
    if(ret == -1)
    {
        perror("bind");
        exit(1);
    }

    // 3.设置监听
    ret = listen(listen_fd, 100);
    if(ret == -1)
    {
        perror("listen");
        exit(1);
    }
    
    // 4.等待，接受连接请求
    struct sockaddr_in addr_client;
    int len = sizeof(addr_client);
    printf("Waiting for a connection from the client......\n");
    int connect_fd = accept(listen_fd, (struct sockaddr*)&addr_client, &len);
    if(connect_fd == -1)
    {
        perror("accept");
        exit(1);
    }

    const char* buf[] = {
        "豫章故郡，洪都新府。\n",
        "星分翼轸，地接衡庐。\n",
        "襟三江而带五湖，控蛮荆而引瓯越。\n",
        "物华天宝，龙光射牛斗之墟；\n",
        "人杰地灵，徐孺下陈蕃之榻。\n",
        "雄州雾列，俊采星驰。\n",
        "台隍枕夷夏之交，宾主尽东南之美。\n",
        "都督阎公之雅望，棨戟遥临；\n",
        "宇文新州之懿范，襜帷暂驻。\n",
        "十旬休假，胜友如云；\n",
        "千里逢迎，高朋满座。\n",
        "腾蛟起凤，孟学士之词宗；\n",
        "紫电青霜，王将军之武库。\n",
        "家君作宰，路出名区；\n",
        "童子何知，躬逢胜饯。\n",
        "时维九月，序属三秋。\n",
        "潦水尽而寒潭清，烟光凝而暮山紫。\n",
        "俨骖騑于上路，访风景于崇阿；\n",
        "临帝子之长洲，得天人之旧馆。\n",
        "层峦耸翠，上出重霄；\n",
        "飞阁流丹，下临无地。\n",
        "鹤汀凫渚，穷岛屿之萦回；\n",
        "桂殿兰宫，即冈峦之体势。\n",
        "披绣闼，俯雕甍，山原旷其盈视，川泽纡其骇瞩。\n",
        "闾阎扑地，钟鸣鼎食之家；\n",
        "舸舰弥津，青雀黄龙之舳。\n",
        "云销雨霁，彩彻区明。\n",
        "落霞与孤鹜齐飞，秋水共长天一色。\n",
        "渔舟唱晚，响穷彭蠡之滨；\n",
        "雁阵惊寒，声断衡阳之浦。\n",
        "遥襟甫畅，逸兴遄飞。\n",
        "爽籁发而清风生，纤歌凝而白云遏。\n",
        "睢园绿竹，气凌彭泽之樽；邺水朱华，光照临川之笔。\n",
        "四美具，二难并。\n",
        "穷睇眄于中天，极娱游于暇日。\n",
        "天高地迥，觉宇宙之无穷；\n",
        "兴尽悲来，识盈虚之有数。\n",
        "望长安于日下，目吴会于云间。\n",
        "地势极而南溟深，天柱高而北辰远。\n",
        "关山难越，谁悲失路之人？\n",
        "萍水相逢，尽是他乡之客。\n",
        "怀帝阍而不见，奉宣室以何年？\n",
        "嗟乎！时运不齐，命途多舛。\n",
        "冯唐易老，李广难封。\n",
        "屈贾谊于长沙，非无圣主；\n",
        "窜梁鸿于海曲，岂乏明时？\n",
        "所赖君子见机，达人知命。\n",
        "老当益壮，宁移白首之心？\n",
        "穷且益坚，不坠青云之志。\n",
        "酌贪泉而觉爽，处涸辙以犹欢。\n",
        "北海虽赊，扶摇可接；\n",
        "东隅已逝，桑榆非晚。\n",
        "孟尝高洁，空余报国之情；\n",
        "阮籍猖狂，岂效穷途之哭！\n",
        "勃，三尺微命，一介书生。\n",
        "无路请缨，等终军之弱冠；\n",
        "有怀投笔，慕宗悫之长风。\n",
        "舍簪笏于百龄，奉晨昏于万里。\n",
        "非谢家之宝树，接孟氏之芳邻。\n",
        "他日趋庭，叨陪鲤对；\n",
        "今兹捧袂，喜托龙门。\n",
        "杨意不逢，抚凌云而自惜；\n",
        "钟期既遇，奏流水以何惭？\n",
        "呜乎！胜地不常，盛筵难再；\n",
        "兰亭已矣，梓泽丘墟。\n",
        "临别赠言，幸承恩于伟饯；\n",
        "登高作赋，是所望于群公。\n",
        "敢竭鄙怀，恭疏短引；\n",
        "一言均赋，四韵俱成。\n",
        "请洒潘江，各倾陆海云尔：\n",
        "滕王高阁临江渚，佩玉鸣鸾罢歌舞。\n",
        "画栋朝飞南浦云，珠帘暮卷西山雨。\n",
        "闲云潭影日悠悠，物换星移几度秋。\n",
        "阁中帝子今何在？槛外长江空自流。\n"
    };

    // 通信
    int length = sizeof(buf) / sizeof(buf[0]);
    for(int i = 0; i < length; i ++)
    {
        sendMsg(connect_fd, buf[i], strlen(buf[i]));
        usleep(1000* 500);      // 500 ms
    }

    printf("数据发送完毕，按任意键结束......");
    getchar();

    // 释放资源
    close(listen_fd);       // 监听
    close(connect_fd);      // 通信

    return 0;
}