#define SERVER_PORT 5000

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

typedef struct LinkedList {
    void *data;
    struct LinkedList *next;
} linked_list;
