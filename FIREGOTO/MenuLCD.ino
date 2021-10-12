/*
 *  FireGoTo - an Arduino Motorized Telescope Project for Dobsonian Mounts
 *  https://firegoto.com.br
    Copyright (C) 2021  Rangel Perez Sardinha / Marcos Lorensini originally created by Reginaldo Nazar

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */

/*  
    Joytick & LCD & Menu   by JMLoureiro77

    http://jmloureiro77.blogspot.com.es

  328P Flash 5292 (17%) RAM 409(19%)

*/
//====================================================
// Menu
//====================================================
void menu(){
  switch (mNivel1){
    case 0:
      menu0(); // Tela de Inicio
      break;
    case 1:
        if(mNivel2==1){
          menu11(); // Micropassos
        
        }else if (mNivel2==2){
          menu12(); // Redução
        
        }else if (mNivel2==3){
          menu13(); // Velocidade do Motor
        
        }else if (mNivel2==4){
          menu14(); // Sentido RA
        
        }else if (mNivel2==5){
          menu15(); // Sentido DEC
        
        } else{
          menu1(); // Configurações
        }
        break;
    case 2:
        if(mNivel2==1){
          menu21(); // 1a Estrela
        }else if (mNivel2==2){
          menu22(); // 2a estrela
        }else if (mNivel2==3){
          menu23(); // 3a Estrela
        } else {
          menu2(); // Alinhamento
        }
        break;
    case 3: // Mover RA e DEC
      // Necessário fazer modificações para confirmação da função com o botão do Joystick 
      // para não entrar em conflito com o modo de navegação (incluído por L. H. Bonani)
      jBut = digitalRead(kPin);
      if (jBut == 0 && millis() > lastInputCheckTime + 350) {
        if (manualMoveRADEC == false) {
          //setMotorLowResolution();
          manualMoveRADEC = true;
          ativaacom = 0;
        } else {
         // setMotorUserResolution();
          manualMoveRADEC = false;
        }
        lastInputCheckTime = millis();
      }
      menu3(); 
      break;
    case 4: // Microfocalizar
      // Necessário fazer modificações para confirmação da função com o botão do Joystick 
      // para não entrar em conflito com o modo de navegação (incluído por L. H. Bonani)
      jBut = digitalRead(kPin);
      if (jBut == 0 && millis() > lastInputCheckTime + 350) {
        if (focStatus == false) {
          focStatus = true;
          digitalWrite(MotorFoc_Enable, LOW);
          ativaacom = 0;
        } else {
          focStatus = false;
          digitalWrite(MotorFoc_Enable, HIGH);
        }
        lastInputCheckTime = millis();
      }
      menu4(); 
      break;
  }//switch
}
//----------------------------------------------------
// Tela de Inicio
//----------------------------------------------------
void menu0(){
  // Modificação da relação RA com Az e DEC com ALT, pois originalmente estava invertida (incluído por L. H. Bonani)
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print("      FireGoTo      ");
  lcd.setCursor(0,1);
  //lcd.print("19/01/21 - 00:00:00");
  LCDClockDisplay();
  lcd.setCursor(0,2);
  lcd.print("RA:  ");
  lcd.setCursor(4,2);
  lcdAZmount();
  lcd.setCursor(0,3);
  lcd.print("DEC: ");
  lcd.setCursor(4,3);
  lcdALTmount();
}
//----------------------------------------------------
// Menu Configuracoes
//----------------------------------------------------
void menu1(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Configuracoes");
  lcd.setCursor(0,1);
}
//-------------------------------------------------1.1
void menu11(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Micropassos");
  //++++++++++++++++++++
  while(editMode){
    controlJoystick();
    lcd.setCursor(lcdX,1);
    if(n[lcdX]!=lastN){
      lcd.print(n[lcdX]);
      lastN=n[lcdX];
    }
  }
}
//-------------------------------------------------1.2
void menu12(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Reducao");
}
//-------------------------------------------------1.3
void menu13(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Velocidade do Motor");
}
void menu14(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Sentido RA");
}
void menu15(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Sentido DEC");
}
//----------------------------------------------------
// Menu Alinhamento
//----------------------------------------------------
void menu2(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Alinhamento");
}
//-------------------------------------------------2.1
void menu21(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">1a estrela");
}
//-------------------------------------------------2.2
void menu22(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">2a estrela");
}
//-------------------------------------------------2.3
void menu23(){
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">3a estrela");
}
//----------------------------------------------------
// Menu Mover DEC e RA
//----------------------------------------------------
void menu3(){ // Modificado para funcionar com a confirmação do funcionamento por meio do botão do Joystick (incluído por L.H.Bonani)
  if (refresh){
    lcd.clear();
    refresh=0;
  }
  lcd.setCursor(0,0);
  lcd.print(">Mover RA e DEC");
    controlJoystick2();
    lcd.setCursor(0,2);
    lcd.print("RA:  ");
    lcd.setCursor(4,2);
    lcdAZmount();
    lcd.setCursor(0,3);
    lcd.print("DEC: ");
    lcd.setCursor(4,3);
    lcdALTmount();
    if (manualMoveRADEC == true) {
     lcd.setCursor(0,1);
     lcd.print("Manual On ");
     controlMotorJoystick();
    } else {
     lcd.setCursor(0,1);
     lcd.print("Manual Off");
    }
}
//-------------------------------------------------2.1

//----------------------------------------------------
// Menu Microfocalizador
//----------------------------------------------------
void menu4(){ // Funcionalidades para o funcionamento do focalizador (incluído por L.H.Bonani)
  if (refresh){lcd.clear();refresh=0;}
  lcd.setCursor(0,0);
  lcd.print(">Microfocalizar");
  if (focStatus == false) {
    lcd.setCursor(1,1);
    lcd.print("- Focalizador off");
  } else {
    lcd.setCursor(1,1);
    lcd.print("- Focalizador on ");
    controlFocuser();
  }
}

//====================================================
// Controlar Joystick
//====================================================
void controlJoystick(){ // Código original ---> Modificado por L.H.Bonani como controlJoystick2()
  leeJoystick();
  if(PQCP) {
    PQCP=0;
    if (joyPos==5){editMode=!editMode;}
    switch (editMode){
       case 1: 
          if(moveRADEC){
            if (joyPos==4){movenorte();   //cima
              lcd.setCursor(0,1);
              lcd.print("Movendo Norte");
              delay(150);
              lcd.setCursor(0,1);
              lcd.print("                    ");  
              refresh=0;}
            if (joyPos==3){movesul();   //baixo
              lcd.setCursor(0,1);
              lcd.print("Movendo Sul");
              delay(150);
              lcd.setCursor(0,1);
              lcd.print("                    ");  
              refresh=0;}
            if (joyPos==1){moveleste();        //direita
              lcd.setCursor(0,1);
              lcd.print("Movendo Leste");
              delay(150);
              lcd.setCursor(0,1);
              lcd.print("                    ");  
              refresh=0;}
            if (joyPos==2){moveoeste();         //esquerda
              lcd.setCursor(0,1);
              lcd.print("Movendo Oeste");
              delay(150);
              lcd.setCursor(0,1);
              lcd.print("                    ");  
              refresh=0;}
            if (joyPos==6){moveoeste();movenorte();       //noroeste
              lcd.setCursor(0,1);
              lcd.print("Movendo Noroeste");
              delay(150);
              lcd.setCursor(0,1);
              lcd.print("                    ");  
              refresh=0;}
            if (joyPos==7){moveleste();movenorte();       //nordeste
              lcd.setCursor(0,1);
              lcd.print("Movendo Nordeste");
              delay(150);
              lcd.setCursor(0,1);
              lcd.print("                    ");  
              refresh=0;}
            if (joyPos==8){moveleste();movesul();       //noroeste
              lcd.setCursor(0,1);
              lcd.print("Movendo Sudeste");
              delay(150);
              lcd.setCursor(0,1);
              lcd.print("                    ");  
              refresh=0;}
            if (joyPos==9){moveoeste();movesul();       //nordeste
              lcd.setCursor(0,1);
              lcd.print("Movendo Sudoeste");
              delay(150);
              lcd.setCursor(0,1);
              lcd.print("                    ");  
              refresh=0;}              
          }
          else {
            lcd.blink();
              if (joyPos==4&&n[lcdX]<9){n[lcdX]++;   //cima
                  refresh=0;}
              if (joyPos==3&&n[lcdX]>0){n[lcdX]--;   //baixo
                  refresh=0;} 
              if (joyPos==1&&lcdX<19){lcdX++;        //direita
                  refresh=0;}
              if (joyPos==2&&lcdX>0){lcdX--;         //esquerda
                  refresh=0;}
          }
        break;
        case 0:
          lcd.noBlink();
          if (mNivel1<4&&joyPos==3){mNivel1++;    //baixo
            refresh=1;
            mNivel2=0;}
          if (mNivel1>0&&joyPos==4){mNivel1--;    //cima
            mNivel2=0;
            refresh=1;}
          if (mNivel2<5&&joyPos==1){mNivel2++;   //direita
            refresh=1;}
          if (mNivel2>0&&joyPos==2){mNivel2--;    //esquerda
           refresh=1;}
        }//!edit
  }//PQCP
}

void movimentoRADEC() { // Rotina para identificação dos movimentos durante o controle manual dos motores (incluído por L.H.Bonani)
    if (mDEC_norte == true && mDEC_sul == false) {
      if (mRA_leste == false && mRA_oeste == false) { // Norte (N)
           lcd.setCursor(11,1);
           lcd.print("> Move N ");
      } else if (mRA_leste == true && mRA_oeste == false) { // Nordeste (NE)
           lcd.setCursor(11,1);
           lcd.print("> Move NE");
      } else if (mRA_leste == false && mRA_oeste == true) { // Noroeste (NW)
           lcd.setCursor(11,1);
           lcd.print("> Move NW");
      } 
  } else if (mDEC_norte == false && mDEC_sul == true) {
      if (mRA_leste == false && mRA_oeste == false) { // Sul (S)
           lcd.setCursor(11,1);
           lcd.print("> Move S ");
      } else if (mRA_leste == true && mRA_oeste == false) { // Sudeste (SE)
           lcd.setCursor(11,1);
           lcd.print("> Move SE");
      } else if (mRA_leste == false && mRA_oeste == true) { // Sudoeste (SW)
           lcd.setCursor(11,1);
           lcd.print("> Move SW");
      } 
  } else if (mDEC_norte == false && mDEC_sul == false) {
      if (mRA_leste == false && mRA_oeste == false) { // Sem movimento
           lcd.setCursor(11,1);
           lcd.print("         ");
      } else if (mRA_leste == true && mRA_oeste == false) { // Leste (E)
           lcd.setCursor(11,1);
           lcd.print("> Move  E");
      } else if (mRA_leste == false && mRA_oeste == true) { // Oeste (W)
           lcd.setCursor(11,1);
           lcd.print("> Move  W");
      } 
  } 
}

void controlJoystick2(){ // Modificação de controlJoystick() por L.H.Bonani
  leeJoystick();
  if(PQCP) {
    PQCP=0;
    if (joyPos==5){
      editMode=!editMode;
    }
    switch (editMode){
       case 1: 
          if (manualMoveRADEC == false)  {
            lcd.blink();
              if (joyPos==4&&n[lcdX]<9){
                 n[lcdX]++;   //cima
                 refresh=0;
              }
              if (joyPos==3&&n[lcdX]>0){
                n[lcdX]--;   //baixo
                refresh=0;
              } 
              if (joyPos==1&&lcdX<19){
                lcdX++;        //direita
                refresh=0;
              }
              if (joyPos==2&&lcdX>0){
                lcdX--;         //esquerda
                refresh=0;
              }
          }
        break;
        case 0:
         if (manualMoveRADEC == false)  {
          lcd.noBlink();
          if (mNivel1<4&&joyPos==3){
            mNivel1++;    //baixo
            refresh=1;
            mNivel2=0;
          }
          if (mNivel1>0&&joyPos==4){
            mNivel1--;    //cima
            mNivel2=0;
            refresh=1;
          }
          if (mNivel2<5&&joyPos==1){
            mNivel2++;   //direita
            refresh=1;
          }
          if (mNivel2>0&&joyPos==2){
            mNivel2--;    //esquerda
            refresh=1;
          }
         }
        }//!edit
  }//PQCP
}


int leeJoystick(){
    int x = analogRead(xPin);
    int y = analogRead(yPin);
    int k = digitalRead(kPin);
    if(x>900&&y>500&&y<600){
      joyRead=1;        //x+
    }else if(x<100&&y>500&&y<600){
      joyRead=2;  //x-
    }else if(y>900&&x>500&&x<600){
      joyRead=3;  //y+
    }else if(y<100&&x>500&&x<600){
      joyRead=4;  //y-
    }else if(x<100&&y<100){
      joyRead=6;
    }else if(x>900&&y<100){
      joyRead=7;
    }else if(x>900&&y>900){
      joyRead=8;
    }else if(x<100&&y>900){
      joyRead=9;
    }else if(!k){
      joyRead=5;
    }else{
      joyRead=0;
    }
  if (joyRead != lastJoyPos){
    lastDebounceTime = millis();
  }
  if(((millis() - lastDebounceTime) > debounceDelay)&&(joyRead!=joyPos)){
    joyPos=joyRead;
    if(!PQCP){
      PQCP=1;
    }
  }
  if(((millis() - lastDebounceTime) > (5*debounceDelay))){
    joyPos=joyRead;                     //repeat time only for UP/DOWN
    if(!PQCP){
      PQCP=1;
    }
  }
  lastJoyPos=joyRead;
}


/* MAPA DE DIRECOES
 *      y-      
    NO  N  NE     
      6 4 7   
 x- O 2 5 1 L x+
      9 3 8   
    SO  S  SE     
        y+        
 * 
 */
