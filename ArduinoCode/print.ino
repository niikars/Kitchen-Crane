/*
debug print for Kitchen Crane by Nikolay Krasilnikov
created 14-01-2024
updated 20.01.2024-21.01.2024 25.01.2024 27.01.2024 04.02.2024 04.03.2024
        08.03.2024 14.03.2024-18.03.2024 04.04.2024 06.04.2024 31.05.2024
        03.06.2024
*/

void dprint()
  {
    Serial.print("curMi = "); Serial.println(curMi, DEC);
    Serial.print("mi_S_Cr = ");Serial.print(mi_S_Cr, DEC);
    Serial.print(" State_S_Crane = "); Serial.println((State_S_Crane)?"true":"false");
  if (*(IntSensorCrane+1) != 0)
    {
      Serial.print("Interrupt = ");Serial.println(IntSensorCrane);
      IntSensorCrane = (char *)NoInt;  
    }
    if (*(IntSensorDr+1) != 0)
    {
      Serial.print("Interrupt = ");Serial.println(IntSensorDr);
      IntSensorDr = (char *)NoInt;  
    }
    if (*(IntMillWorking+1) != 0)
    {
      Serial.print("Interrupt = ");Serial.println(IntMillWorking);
      IntMillWorking = (char *)NoInt;  
    }
    if (*(IntWorkPermition+1) != 0)
    {
      Serial.print("Interrupt = ");Serial.println(IntWorkPermition);
      IntWorkPermition = (char *)NoInt;  
    }
    if (*(IntBut+1) != 0)
    {
      Serial.print("Interrupt = ");Serial.println(IntBut);
      IntBut = (char *)NoInt;  
    }
    //Serial.print("debug1 = "); Serial.print((debug1)?"true ":"false ");
    //Serial.print("debug2 = "); Serial.println((debug2)?"true":"false");

    Serial.print("State_Cr = "); Serial.print((State_Cr)?"true ":"false ");
    Serial.print("State_FCr = " ); Serial.println((State_FCr)?"true ":"false ");
  
    //Serial.print("State_Cold = "); Serial.println((State_Cold)?"true ":"false ");

    Serial.print("State_Dr = "); Serial.print((State_Dr)?"true ":"false ");
    Serial.print("State_FDr = "); Serial.println((State_FDr)?"true ":"false ");

    //Serial.print("State_W12 = "); Serial.print((State_W12)?"true ":"false ");
    //Serial.print("OLD = "); Serial.println((OldState_W12)?"true":"false");

    //Serial.print("State_M = "); Serial.print((State_M)?"true ":"false ");
    //Serial.print("State_V_M = "); Serial.println((State_V_M)?"true":"false");

    //Serial.print("W_V_M = "); Serial.print(W_V_M, DEC);
    //Serial.print(" mi_12 ");Serial.println(mi_12, DEC);
    //Serial.print("mi_B_Cr ");Serial.print(mi_B_Cr, DEC);
    //Serial.print(" mi_B_FCr ");Serial.print(mi_B_FCr, DEC);
    //Serial.print(" mi_B_Cold ");Serial.println(mi_B_Cold, DEC);
    //Serial.print("mi_B_Dr ");Serial.print(mi_B_Dr, DEC);
    //Serial.print(" mi_B_FDr ");Serial.println(mi_B_FDr, DEC);
    Serial.println("--");

  }
