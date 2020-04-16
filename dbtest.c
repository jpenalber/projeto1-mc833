#include "dbinterface.h"

int main() {
    open_db("test.db");
    insertSala("Hollodeck");
    insertFilme("Frozen", "Rei leao no gelo so que com gente kkkk");
    return 0;
}