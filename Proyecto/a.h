void admin_register_user(int *current_menu)
{

    lcd.clear();
    bool end = false;
    int current_option = 0;

    while (!end)
    {
        // ASK FOR A NAME
        Serial1.write("name");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Nombre:");
        String nombre = "";

        //En teoria ya valida la longidud del nombre acá
        bool send_name = false;
        while (true)
        {
            while (Serial1.available())
            {
                delay(10);
                char c = Serial1.read();
                nombre += c;
                send_name = true;
            }
            if(send_name){
                if(nombre.length() > 16 || nombre.length() == 0){
                Serial1.write("Error: Nombre muy largo o muy corto");
                nombre = "";
                send_name = false;
                Serial1.write("Esperando info...");
                }else {
                    break;
                }
            }
        }
        
        lcd.clear();

        lcd.setCursor(0, 0);
        lcd.print("Apodo:");
        String apodo = "";
        while (true)
        {
            char key = keypad.getKey();
            if (key != NO_KEY)
            {
                apodo += key;
                lcd.setCursor(0, 1);
                lcd.print(apodo);
            }
            if (ok_button.is_pressed())
            {
                break;
            }
        }

        lcd.clear();

        lcd.setCursor(0, 0);
        lcd.print("Contraseña:");
        String password = "";
        while (true)
        {
            char key = keypad.getKey();
            if (key != NO_KEY)
            {
                password += key;
                lcd.setCursor(0, 1);
                lcd.print(password);
            }
            if (ok_button.is_pressed())
            {
                lcd.clear();
                User consumer = User();
                if (is_user_registered(apodo.c_str()))
                {
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("User exists");
                    // SEND TO THE APP
                    Serial1.write("Error: Usuario existente");
                    delay(1000);
                }
                else if(apodo.length() > 10 || apodo.length() == 0){
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Nick too long");
                    lcd.setCursor(0, 1);
                    lcd.print("or too short");
                    // SEND TO THE APP
                    Serial1.write("Error: Longitud Nick");
                    delay(1000);
                }
                else if(password.length() > 10 || password.length() == 0){
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Passwrd too long");
                    lcd.setCursor(0, 1);
                    lcd.print("or too short");
                    // SEND TO THE APP
                    Serial1.write("Error: Longitud Password");
                    delay(1000);
                }
                else
                {
                    consumer.isAdmin = false;
                    strcpy(consumer.name, nombre.c_str());
                    strcpy(consumer.nickname, apodo.c_str());
                    strcpy(consumer.password, password.c_str());
                    consumer.credits = 250;
                    Serial.print("Registrado ");
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("User registered");
                    // SEND TO THE APP
                    Serial1.write("Usuario registrado");
                    delay(1000);
                    write_user(consumer);
                }
                *current_menu = ADMIN_MAIN_DASHBOARD;
                end = true;
                break;
            }
        }
    }
}
