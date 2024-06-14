/*
Kitchen Crane main loop by Nikolay Krasilnikov
created 14-01-2024
updated 20.01.2024-21.01.2024 25.01.2024 27.01.2024 04.02.2024 04.03.2024
        08.03.2024 14.03.2024-18.03.2024 04.04.2024 06.04.2024 31.05.2024
        02.06.2024-03.06.2024 14.06.2024
*/

void loop() 
{
  interrupts();
  // Ждём паузу  
  delay(MAIN_TOUT);
  noInterrupts();
  int SDr = digitalRead(S_Dr);
  if (SDr != Old_SDr) 
  {
    Old_SDr = SDr;
    SensorDr();
  }
  int SCr = digitalRead(S_Cr);
  if (SCr != Old_SCr) 
  {
    Old_SCr = SCr;
    SensorCr();
  }

  curMi = millis();
  dprint();
  // Добавляем времена работы канала мельницы пока он работает
  if (State_V_M) W_V_M += MAIN_TOUT;
  //Мельница включилась - включаем канал
  if (State_M && !State_V_M) 
  {
    //digitalWrite(V_M, OPEN);
    State_V_M = true;
  }
  //Пока мельница работает - обнуляем время работы канала
  if (State_M && State_V_M)
  {
    W_V_M = 0;
  }   
  //Мельница выключена и время канала более времени пролива воды, то выключаем мельницу
  if (!State_M && (W_V_M > Mill_TOUT))
  {
    digitalWrite(V_M, CLOSED);
    State_V_M = false;
  }

  // Если нет питания, но оно не пропало, то ничего не делаем
  // иначе
  if (OldState_W12 || State_W12)
  {
    //Если пропало питание, 
    if (OldState_W12 && !State_W12)
    {
      OldState_W12 = false;
      //то выключаем каналы, светодиоды и питание сенсоров.
      digitalWrite(V_Cr, CLOSED);
      digitalWrite(V_Cold, CLOSED); 
      digitalWrite(V_FCr, CLOSED);
      digitalWrite(V_Dr, CLOSED);
      digitalWrite(V_FDr, CLOSED); 
      digitalWrite(D_Dr, LOW);
      digitalWrite(D_FDr, LOW);
      digitalWrite(D_Cr, LOW);
      digitalWrite(D_FCr, LOW);
      digitalWrite(D_Cold, LOW);
      digitalWrite(D_Stop, LOW);
      digitalWrite(S_Dr_5, LOW);
      digitalWrite(S_Cr_5, LOW); 
     
      //Сбрасываем состояния работы и состояния кнопок,
      //кроме холодной воды
      State_V_Cr = false;
      State_V_Cold = false;
      State_V_FCr = false;
      State_V_Dr = false;
      State_V_FDr = false;
      State_Cr = false;
      State_FCr = false;
      State_Dr = false;
      State_FDr = false;
      State_S_Crane = false;
      State_S_Dr = false;
    }  
    // Иначе Если появилось питание,
    else if (!OldState_W12 && State_W12)
    {
      //то включаем сенсоры и светодиод "Stop" 
      digitalWrite(D_Stop, HIGH);
      digitalWrite(S_Dr_5, HIGH);
      digitalWrite(S_Cr_5, HIGH);
      //и сведодиод "Cold", если соответствующее состояние 
      if (State_Cold)  digitalWrite(D_Cold, HIGH);
      OldState_W12 = true;
    }
    // Иначе питание есть, 
    else
    // то перестраиваем каналы в соответствии с изменением состояний 
    {
      mi_12 += MAIN_TOUT;
//КУХОННЫЙ КРАН

      //Если было удержание крана более некоторого времени и кран отпущен,
      // то включаем / выключаем режим вода
      SCr = digitalRead(S_Cr);
      if (State_S_Crane)
      {
        // Если было касание, кран удерживается и прошло времени больше MIN_TUCH_TOUT,
        // то зажигаем или гасим светодиод в зависимости от состояния State_Cr,
        // показывая этим, что отпускание крана приведёт к изменению состояния.
        if ( (SCr == HIGH) 
             && ((curMi - mi_S_Cr) >  MIN_TUCH_TOUT)
             && ((curMi - mi_S_Cr) <  TUCH_TOUT))
        {
          digitalWrite(D_Cr, (!State_Cr)?HIGH:LOW);
        }
        // если удерживался менее MIN_TUCH_TOUT, или уже более TUCH_TOUT, то отменяем касание
        if ( ((SCr == LOW) && ((curMi - mi_S_Cr) <  MIN_TUCH_TOUT))
          || ((SCr == HIGH) && ((curMi - mi_S_Cr) >  TUCH_TOUT)))
        {
            State_S_Crane =  false;
        }
        else if(SCr == LOW)
        {
          State_Cr = !State_Cr;
          State_S_Crane = false;
        }
      }
      // полный напор -> включаем воду и полный напор
      if (State_FCr)
      {
        State_Cr = true;
        digitalWrite(D_Cr, HIGH);
        digitalWrite(V_FCr, OPEN);
        if(State_Cold )
        {
          digitalWrite(V_Cold, OPEN);
          digitalWrite(V_Cr, CLOSED);
        }
        else
        {
          digitalWrite(V_Cr, OPEN);
          digitalWrite(V_Cold, CLOSED);
        }
      }
      // вода включена 
      else if (State_Cr)
      {
        digitalWrite(V_FCr, CLOSED);
        if(State_Cold )
        {
          digitalWrite(V_Cold, OPEN);
          digitalWrite(V_Cr, CLOSED);
        }
        else
        {
          digitalWrite(V_Cr, OPEN);
          digitalWrite(V_Cold, CLOSED);
        }
      }
      //иначе всё выключаем
      else
      {
        digitalWrite(V_FCr, CLOSED);
        digitalWrite(V_Cold, CLOSED);
        digitalWrite(V_Cr, CLOSED);
      }
      //гасим светодиод, если нет флага касания и кран выключен
      if (!State_Cr & !State_S_Crane)   digitalWrite(D_Cr, LOW);
      //зажигаем светодиод, если нет флага касания и кран включен
      if (State_Cr & !State_S_Crane)   digitalWrite(D_Cr, HIGH);

//ПИТЬЕВАЯ 
      //Если полный напор и было касание крана и кран удерживается,
      //то переходим к обычному напору от касания крана
      if (State_S_Dr && State_FDr)
      {
        if (digitalRead(S_Dr) == HIGH) 
        {
          digitalWrite(V_FDr, CLOSED);
          digitalWrite(D_FDr, LOW);
          State_V_Dr = true;
          State_FDr = false;
          State_Dr = false;
        }
      }
      //Если было касание крана и вода не включена,
      // то включаем воду, если кран удерживается
      if (State_S_Dr && !State_Dr)
      {
        if (digitalRead(S_Dr) == HIGH) 
        {
          digitalWrite(V_Dr, OPEN);
          State_V_Dr = true;
        }
      }
      State_S_Dr = false; 
      //Если кран отпущен и нет состояния вода включена, то выключаем воду.
      if ((digitalRead(S_Dr) == LOW) && !State_Dr)
      {
        digitalWrite(V_Dr, CLOSED);
        State_V_Dr = false;
      } 
      //Если полный напор,
      if(State_FDr)
      // то устанавливаем состояние питьевая,
      //включаем каналы и светодиод питьевая
      {
        State_Dr = true;
        digitalWrite(D_Dr, HIGH);
        digitalWrite(V_Dr, OPEN);
        digitalWrite(V_FDr, OPEN);
      }
      //иначе, если состояние питьевая, 
      else if (State_Dr)
      {
        //то включаем канал "питьевая"
        digitalWrite(V_Dr, OPEN);
        digitalWrite(V_FDr, CLOSED);
        //!?выключаем питание сенсора
        //!?digitalWrite(S_Dr_5, LOW);  
      }
      //иначе выключаем всё, если не работа сенсорного крана
      else if (!State_V_Dr)
      {
        digitalWrite(V_Dr, CLOSED);
        digitalWrite(V_FDr, CLOSED);
      }
    }
  }
  interrupts();
}
