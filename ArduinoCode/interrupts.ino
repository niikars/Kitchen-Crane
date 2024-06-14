/*
Interrupts functions for Kitchen Crane by Nikolay Krasilnikov
created 14-01-2024
updated 20.01.2024-21.01.2024 25.01.2024 27.01.2024 04.02.2024 04.03.2024
        08.03.2024 14.03.2024-18.03.2024 04.04.2024 06.04.2024 31.05.2024
        02.06.2024-03.06.2024 14.06.2024
*/

// функция прерывания по нажатию одной из кнопок
void BUTTONS()
{
  if (Starting) return;
  IntBut =  (char *)IntBut_s;
  if (! State_W12 ) return;
  debug1 = false;
  // если нажата кнопка стоп, то проверяем звон
  if (digitalRead(B_Stop) == HIGH)
  {
    delayMicroseconds(BOUNCE);
    if (digitalRead(B_Stop) == HIGH)
    // если нажата кнопка стоп, то 
    // выключаем все каналы кроме мельницы
    // гасим все светодиоды кроме Stop и Cold
    // Меняем как состояние, так и его старое значение,
    // чтобы избежать обработки в главной петле,
    // поскольку все действия мы выполняем в этом месте.
    // и выход
    {
      digitalWrite(V_Cold, CLOSED);
      delay(OPER_TOUT);
      digitalWrite(V_Cr, CLOSED);
      digitalWrite(D_Cr, LOW);
      delay(OPER_TOUT);
      State_V_Cr = false;
      State_Cr = false;
      digitalWrite(V_FCr, CLOSED);
      digitalWrite(D_FCr, LOW);
      delay(OPER_TOUT);
      State_V_FCr = false;
      State_FCr = false;
      digitalWrite(V_Dr, CLOSED);
      digitalWrite(D_Dr, LOW);
      delay(OPER_TOUT);
      State_V_Dr = false;
      State_Dr = false;
      digitalWrite(V_FDr, CLOSED);
      digitalWrite(D_FDr, LOW);
      delay(OPER_TOUT);
      State_V_FDr = false;
      State_FDr = false;
      State_S_Crane = false;
      return;
    }
    else 
    // Помеха
    {
      //debug1 = true;
      return; 
    } 
  } 
  //delayMicroseconds(BOUNCE);
  debug1 = true;
  int BCr = digitalRead(B_Cr);
  debug2 = BCr == HIGH;
  int BFCr = digitalRead(B_FCr);
  int BDr = digitalRead(B_Dr);
  int BFDr = digitalRead(B_FDr);
  int BCold = digitalRead(B_Cold);
  delayMicroseconds(BOUNCE);
  unsigned long curMi = millis();
  // Для каждой кнопки.
  // Если нажата кнопка и прошло более DEAD_LOCK,
  // то меняем соответствующее состояние и
  // гасим или зажигаем светодиод
  if (   (digitalRead(B_Cr) == HIGH)
      && (BCr == HIGH) 
      && (abs(curMi-mi_B_Cr) > DEAD_LOCK))
  {
    mi_B_Cr = curMi;
    State_Cr = !State_Cr;
    if(State_Cr) 
      digitalWrite(D_Cr, HIGH); 
    else 
    {
      State_FCr = false;
      digitalWrite(D_Cr, LOW);
      digitalWrite(D_FCr, LOW);
    }
  }
  if (   (digitalRead(B_FCr) == HIGH)
      && (BFCr == HIGH) 
      && (abs(curMi-mi_B_FCr) > DEAD_LOCK))
  {
    mi_B_FCr = curMi;
    State_FCr = !State_FCr;
    if(State_FCr) 
      digitalWrite(D_FCr, HIGH); else digitalWrite(D_FCr, LOW);
  }
  if (   (digitalRead(B_Dr) == HIGH)
      && (BDr == HIGH) 
      && (abs(curMi-mi_B_Dr) > DEAD_LOCK))
  {
    mi_B_Dr = curMi;
    State_Dr = !State_Dr;
    if(State_Dr) 
      digitalWrite(D_Dr, HIGH); 
    else 
    {
      digitalWrite(D_Dr, LOW);
      digitalWrite(D_FDr, LOW);
      State_Dr = false;
    }
  }
  if (   (digitalRead(B_FDr) == HIGH)
      && (BFDr == HIGH) 
      && (abs(curMi-mi_B_FDr) > DEAD_LOCK))
  {
    mi_B_FDr = curMi;
    State_FDr = !State_FDr;
    if(State_FDr) digitalWrite(D_FDr, HIGH); else digitalWrite(D_FDr, LOW);
  }
  if (   (digitalRead(B_Cold) == HIGH)
      && (BCold == HIGH) 
      && (abs(curMi-mi_B_Cold) > DEAD_LOCK))
  {
    mi_B_Cold = curMi;
    State_Cold = !State_Cold;
    if(State_Cold) digitalWrite(D_Cold, HIGH); else digitalWrite(D_Cold, LOW);
  }
}//BUTTONS

// функция прерывания по включению (выключению) силового питания.
void WorkPermition()
{
  if (Starting) return;
  IntWorkPermition =  (char *)IntWorkPermition_s;
  delayMicroseconds(BOUNCE);
  int UInt = digitalRead(U_Int);
  delayMicroseconds(BOUNCE);
  // Если не звон, то меняем соответствующее состояние.
  if (digitalRead(U_Int) == UInt)
  {
    State_W12 = digitalRead(U_Int) == HIGH;
  }
  if (State_W12) mi_12 = 0;
}

// функция прерывания по включению (выключению) мельницы.
void MillWorking()
{
  if (Starting) return;
  IntMillWorking =  (char *)IntMillWorking_s;
  delayMicroseconds(BOUNCE);
  int MI = digitalRead(M_I);
  delayMicroseconds(BOUNCE);
  // Если не звон, то меняем соответствующее состояние.
  if (digitalRead(M_I) == MI)
  {
    State_M = digitalRead(M_I) == MILL_ON;
    if (State_M) digitalWrite(V_M, OPEN);
  }
}
// функция прерывания по касанию сенсора питьевой воды.
void SensorDr()
{
  if (Starting || (mi_12 < MAIN_TOUT)) return;
  IntSensorDr =  (char *)IntSensorDr_s;
  delayMicroseconds(BOUNCE);
  int SDr = digitalRead(S_Dr);
  delayMicroseconds(BOUNCE);
  // Если не звон, то меняем соответствующее состояние.
  if (digitalRead(S_Dr) == SDr)
  {
    if (SDr == HIGH)
    {
      State_S_Dr = true;
      // Зажигаем или гасим светодиод в зависимости от состояния State_Dr 
      digitalWrite(D_Dr, (!State_Dr)?HIGH:LOW);
    }
    else digitalWrite(D_Dr, (State_Dr)?HIGH:LOW);
  }
}
// функция прерывания по касанию сенсора кухонного крана.
void SensorCr()
{
  if (Starting || (mi_12 < MAIN_TOUT) || State_V_Dr) return;
  IntSensorCrane =  (char *)IntSensorCrane_s;
  delayMicroseconds(BOUNCE);
  int SCr = digitalRead(S_Cr);
  delayMicroseconds(BOUNCE);
  // Если не звон, то фиксируем состояние касания.
  if ((digitalRead(S_Cr) == SCr))
  {
    if (SCr == HIGH)
    {
      State_S_Crane = true;
      // Зажигаем или гасим светодиод в зависимости от состояния State_Cr,
      // тольов том случае, если минимальное время касания превышено
      // и отпускание крана приведёт к изменению состояния
      //digitalWrite(D_Cr, (!State_Cr)?HIGH:LOW);
      // Запоминаем время касания, чтобы включить или выключить воду,
      // только в случае касания некоторой длительности.
      mi_S_Cr = millis(); 
    }
    else digitalWrite(D_Cr, (State_Cr)?HIGH:LOW);
  }
}