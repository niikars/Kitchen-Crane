/*
Setup procedure for Kitchen Crane by Nikolay Krasilnikov
created 14-01-2024
updated 20.01.2024-21.01.2024 25.01.2024 27.01.2024 04.02.2024 04.03.2024
        08.03.2024 14.03.2024-18.03.2024 04.04.2024 06.04.2024-07.04.2024
        31.05.2024
*/
// Настраиваем пины микросхемы
void setup() 
{
  Serial.begin(9600);
  Serial.println("Setup");
  // Выходы управления
  pinMode(D_Dr, OUTPUT);
  pinMode(D_FDr, OUTPUT);
  pinMode(D_Cr, OUTPUT);
  pinMode(D_FCr, OUTPUT);
  pinMode(D_Cold, OUTPUT);
  pinMode(D_Stop, OUTPUT);
  pinMode(V_Dr, OUTPUT);
  pinMode(V_FDr, OUTPUT);
  pinMode(V_Cr, OUTPUT);
  pinMode(V_FCr, OUTPUT);
  pinMode(V_Cold, OUTPUT);
  pinMode(V_M, OUTPUT);
  //Питание сенсоров
  pinMode(S_Cr_5, OUTPUT);
  pinMode(S_Dr_5, OUTPUT);
  // выключаем всё
  digitalWrite(D_Dr, LOW);
  digitalWrite(D_FDr, LOW);
  digitalWrite(D_Cr, LOW);
  digitalWrite(D_FCr, LOW);
  digitalWrite(D_Cold, LOW);
  digitalWrite(D_Stop, LOW);
  digitalWrite(V_Dr, CLOSED);
  digitalWrite(V_FDr, CLOSED);
  digitalWrite(V_Cr, CLOSED);
  digitalWrite(V_FCr, CLOSED);
  digitalWrite(V_Cold, CLOSED);
  digitalWrite(V_M, CLOSED);
  digitalWrite(S_Dr_5, LOW);
  digitalWrite(S_Cr_5, LOW);
  // входы кнопок
  pinMode(B_Dr, INPUT_PULLUP);
  pinMode(B_FDr, INPUT_PULLUP);
  pinMode(B_Cr, INPUT_PULLUP);
  pinMode(B_FCr, INPUT_PULLUP);
  pinMode(B_Cold, INPUT_PULLUP);
  pinMode(B_Stop, INPUT_PULLUP);
  pinMode(U_Int, INPUT_PULLUP);
  pinMode(B_Int, INPUT_PULLUP);
  pinMode(S_Cr, INPUT);
  pinMode(S_Dr, INPUT);
  pinMode(M_I, INPUT_PULLUP);
  
  // прерывания
  Serial.println("Interrupts");
  noInterrupts();
  attachInterrupt(digitalPinToInterrupt(B_Int), BUTTONS, FALLING);
  attachInterrupt(digitalPinToInterrupt(U_Int), WorkPermition, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(S_Cr), SensorCr, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(S_Dr), SensorDr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(M_I), MillWorking, CHANGE);
  delay(MAIN_TOUT*2);
  Starting = false;
  //Если подано питание, то включаем режим разрешения работы
  State_W12 = digitalRead(U_Int) == HIGH;
  interrupts();
  Serial.println("Setup end");
}
