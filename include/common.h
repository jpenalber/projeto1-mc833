enum packet_type {
    PT_INSERT_SALA,
    PT_INSERT_FILME,

    PT_INSERT_SALA_ID,
    PT_INSERT_FILME_ID
};

struct packet {
    enum packet_type type;
    unsigned char data[1024];
};
