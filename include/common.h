enum packet_type {
    PT_INSERT_SALA,
    PT_INSERT_FILME
};

struct packet {
    enum packet_type type;
    unsigned char data[1024];
};
