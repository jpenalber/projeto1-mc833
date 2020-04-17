#define SERVER_PORT 5000

enum packet_type {
    PT_INSERT_FILME,
    PT_REMOVE_FILME,
    PT_LIST_TITULO_SALA,
    PT_LIST_GENERO,
    PT_FILM_NAME_FROM_ID,
    PT_FILM_INFO_FROM_ID,
    PT_LIST_ALL,

    PT_INSERT_FILME_RESULT,
    PT_LIST_TITULO_SALA_RESULT,
    PT_LIST_GENERO_RESULT,
    PT_FILM_NAME_FROM_ID_RESULT,
    PT_FILM_INFO_FROM_ID_RESULT,
    PT_LIST_ALL_RESULT,
};

struct packet {
    enum packet_type type;
    char data[1024];
};

typedef struct LinkedList {
    void *data;
    struct LinkedList *next;
} linked_list;
