    Usuarios user1 = Usuarios();
    user1.isAdmin = false;
    char nombre1[] = "a";// Pruebas solamente "ADMIN*94069"
    char nombre_cifrado1[sizeof(nombre1)];
    strcpy(nombre_cifrado1, nombre1);
    dobleCifradoXOR(nombre_cifrado1);

    char numero1[] = "94069";
    char numero_cifrado1[sizeof(numero1)];
    strcpy(numero_cifrado1, numero1);
    dobleCifradoXOR(numero_cifrado1);

    char contrasenia1[] = "1";// Pruebas solamente "GRUPO17"
    char contrasenia_cifrada1[sizeof(contrasenia1)];
    strcpy(contrasenia_cifrada1, contrasenia1);
    dobleCifradoXOR(contrasenia_cifrada1);

    strcpy(user1.nombre, nombre_cifrado1);
    strcpy(user1.numero, numero_cifrado1);
    strcpy(user1.contrasenia, contrasenia_cifrada1);
    Serial.println(nombre_cifrado1);
    Serial.println(contrasenia_cifrada1);
    write_user(user1);