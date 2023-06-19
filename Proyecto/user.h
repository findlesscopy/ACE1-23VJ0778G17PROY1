#ifndef USER_H
#define USER_H

struct User {
    char name[12];
    char phoneName[8];
    char password[12];
    unsigned int address;
    bool isAdmin;

    bool validate_credentials(String phoneName, String password) {
        return (strcmp(this->phoneName, phoneName.c_str()) == 0) && (strcmp(this->password, password.c_str()) == 0);
    }

    bool is_valid() {
        return strcmp(this->phoneName, "") != 0;
    }
};

#endif