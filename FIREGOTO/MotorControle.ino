/*
 *  FireGoTo - an Arduino Motorized Telescope Project for Dobsonian Mounts
 *  https://firegoto.com.br
    Copyright (C) 2021  Rangel Perez Sardinha / Marcos Lorensini originally created by Reginaldo Nazar
    Thanks to Romulo Almeida, Regis Suzano da Costa and Luiz H. Bonani

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
void IniciaMotores()
{
  //Iniciar as variaveis do motor de passo
  // Modificado por L.H.Bonani para incluir o motor do focalizador
  pinMode(MotorALT_Direcao, OUTPUT);
  pinMode(MotorALT_Passo, OUTPUT);
  pinMode(MotorALT_Sleep, OUTPUT);
  pinMode(MotorALT_Reset, OUTPUT);
  pinMode(MotorALT_M2, OUTPUT);
  pinMode(MotorALT_M1, OUTPUT);
  pinMode(MotorALT_M0, OUTPUT);
  pinMode(MotorALT_Ativa, OUTPUT);
  pinMode(MotorAZ_Direcao, OUTPUT);
  pinMode(MotorAZ_Passo, OUTPUT);
  pinMode(MotorAZ_Sleep, OUTPUT);
  pinMode(MotorAZ_Reset, OUTPUT);
  pinMode(MotorAZ_M2, OUTPUT);
  pinMode(MotorAZ_M1, OUTPUT);
  pinMode(MotorAZ_M0, OUTPUT);
  pinMode(MotorAZ_Ativa, OUTPUT);
  pinMode(MotorFoc_Passo, OUTPUT);
  pinMode(MotorFoc_Direcao, OUTPUT);
  pinMode(MotorFoc_Enable, OUTPUT);
  //Aciona os pinos por padrão
  digitalWrite(MotorALT_Direcao, LOW);
  digitalWrite(MotorALT_Passo, LOW);
  digitalWrite(MotorALT_Sleep, HIGH);
  digitalWrite(MotorALT_Reset, HIGH);
  digitalWrite(MotorALT_M2, HIGH);
  digitalWrite(MotorALT_M1, HIGH);
  digitalWrite(MotorALT_M0, HIGH);
  digitalWrite(MotorALT_Ativa, LOW);
  digitalWrite(MotorAZ_Direcao, LOW);
  digitalWrite(MotorAZ_Passo, LOW);
  digitalWrite(MotorAZ_Sleep, HIGH);
  digitalWrite(MotorAZ_Reset, HIGH);
  digitalWrite(MotorAZ_M2, HIGH);
  digitalWrite(MotorAZ_M1, HIGH );
  digitalWrite(MotorAZ_M0, HIGH);
  digitalWrite(MotorAZ_Ativa, LOW);
  digitalWrite(MotorFoc_Passo, LOW);
  digitalWrite(MotorFoc_Direcao, LOW);
  digitalWrite(MotorFoc_Enable, HIGH);
  AltMotor.setMaxSpeed(dMaxSpeedAlt);
  AltMotor.setAcceleration(dReducao);
  AzMotor.setMaxSpeed(dMaxSpeedAz);
  AzMotor.setAcceleration(dReducao);
  focMotor.setMaxSpeed(dMaxSpeedFoc);
  focMotor.setAcceleration(8);
  Timer3.start(MinTimer);
  Timer3.attachInterrupt(runmotor);
}

void setMotorLowResolution() { // Configura baixa resolução de acordo com o driver (incluído por L.H.Bonani)
   if (tmcFlag == false) {
       // modo full step 
       digitalWrite(MotorALT_M2, LOW);
       digitalWrite(MotorALT_M1, LOW);
       digitalWrite(MotorALT_M0, LOW);
       digitalWrite(MotorAZ_M2, LOW);
       digitalWrite(MotorAZ_M1, LOW);
       digitalWrite(MotorAZ_M0, LOW);
       AltMotor.setAcceleration(dReducao * 4);
       AzMotor.setAcceleration(dReducao * 4);
    } else {
       // modo 1/8 
       digitalWrite(MotorALT_M1, LOW);
       digitalWrite(MotorALT_M0, LOW);
       digitalWrite(MotorAZ_M1, LOW);
       digitalWrite(MotorAZ_M0, LOW);
       AltMotor.setAcceleration(dReducao * 32);
       AzMotor.setAcceleration(dReducao * 32);
    }
}

void setMotorUserResolution() { // Configura a resolução original do driver definida pelo usuário (incluído por L.H.Bonani)
   if (tmcFlag == false) {
      digitalWrite(MotorALT_M2, AltaM2);
       digitalWrite(MotorALT_M1, AltaM1);
       digitalWrite(MotorALT_M0, AltaM0);
       digitalWrite(MotorAZ_M2, AltaM2);
       digitalWrite(MotorAZ_M1, AltaM1);
       digitalWrite(MotorAZ_M0, AltaM0);
    } else {
       digitalWrite(MotorALT_M1, AltaM1);
       digitalWrite(MotorALT_M0, AltaM0);
       digitalWrite(MotorAZ_M1, AltaM1);
       digitalWrite(MotorAZ_M0, AltaM0);
    }
}

void SentidodosMotores() // Modificado por L.H.Bonani para correspondência entre RA <-> Az e DEC <-> Alt (originalmente invertida)
{
  if (SentidoDEC == 1)
  {
    AltMotor.setPinsInverted(true);
  }
  else
  {
    AltMotor.setPinsInverted(false);

  }
  if (SentidoRA == 1)
  {
    AzMotor.setPinsInverted(true);
  }
  else
  {
    AzMotor.setPinsInverted(false);
  }
}

void runmotor () { // Modificado por L.H.Bonani para incluir o motor do focalizador e para permitir o movimento dos motores Alt e Az em velocidade máxima quando em operação manual
  if (setupflag == 0) {
    if (manualMoveRADEC == true) {
       AltMotor.runSpeed();
       AzMotor.runSpeed();
    } else {
       AltMotor.run();
       AzMotor.run();
    }
  } else {
    AltMotor.runSpeed();
    AzMotor.runSpeed();
  }
  focMotor.runSpeed();
}

void setaccel()
{
  double tempdis;
  tempdis = abs(AzMotor.distanceToGo());
  AzMotor.setMaxSpeed(tempdis * dReducao );
  tempdis = abs(AltMotor.distanceToGo());
  AltMotor.setMaxSpeed(tempdis * dReducao );
}

void setaccel(int Accel)
{
  AltMotor.setMaxSpeed(Accel);
  AzMotor.setMaxSpeed(Accel);
}

void addbackslash()
{
  if (AzMotor.distanceToGo() >= 0)
  {
    dirAz = 1;
  } else {
    dirAz = 0;
  }
  if (AltMotor.distanceToGo() >= 0)
  {
    dirAlt = 1;
  } else {
    dirAlt = 0;
  }
  if (AtivaBack == 1)
  {
    AtivaBack = 0;
    if (dirAlt != dirAltant)
    {
      if (dirAlt == 0)
      {
        Serial.println("alt:0");
        AltMotor.setCurrentPosition(AltMotor.currentPosition() + RAbacklash);
      } else
      {
        AltMotor.setCurrentPosition(AltMotor.currentPosition() - RAbacklash);
        Serial.println("alt:1");

      }
    }
    if (dirAz != dirAzant)
    {
      if (dirAz == 0)
      {
        AzMotor.setCurrentPosition( AzMotor.currentPosition() + DECbacklash);
        Serial.println("az: 0 ");

      } else
      {
        AzMotor.setCurrentPosition( AzMotor.currentPosition() - DECbacklash);
        Serial.println("az: 1");

      }
    }
  }
}


void syncro()
{
  AltMotor.setCurrentPosition((int)ALTmountAlvo);
  AzMotor.setCurrentPosition((int)AZmountAlvo);
  CalcPosicaoPasso();
}

void SetPosition()
{
  AltMotor.moveTo((int)ALTmountAlvo);
  AzMotor.moveTo((int)AZmountAlvo);
}


int paramotors()
{
  AZmountAlvo = AZmount;
  ALTmountAlvo = ALTmount;
  return (1);
}

void CalculaResolucao(){   
     ResolucaoeixoAltGrausDecimal = 360.0 / MaxPassoAlt ;
     ResolucaoeixoAzGrausDecimal = 360.0 / MaxPassoAz ;
     ResolucaoeixoAltPassoGrau = (MaxPassoAlt  / 360.0);
     ResolucaoeixoAzPassoGrau = (MaxPassoAz  / 360.0);
}


void CalcPosicaoPasso()
{
  ALTmount = AltMotor.currentPosition();
  AZmount = AzMotor.currentPosition();
  eixoAltGrausDecimal = ResolucaoeixoAltGrausDecimal * ALTmount;
  eixoAzGrausDecimal = ResolucaoeixoAzGrausDecimal * AZmount;
}

void controlMotorJoystick() {   // Movimento manual dos motores (incluído por L.H.Bonani)
  valPotX =  analogRead(xPin); 
  if (valPotX <= 449){ 
    mRA_leste = true;
    mRA_oeste = false;
    motorSpeedX = map(valPotX,499,0,0,maxSpeedMotorRA); // map(valPotX,500,0,0,maxSpeedX); 
    AzMotor.setSpeed(motorSpeedX);
  } else if (valPotX > 449 && valPotX < 573) { 
    mRA_leste = false;
    mRA_oeste = false;
    motorSpeedX = 0;
    AzMotor.setSpeed(motorSpeedX);
  } else if (valPotX >= 573){ 
    mRA_leste = false;
    mRA_oeste = true;
    motorSpeedX = map(valPotX,573,1023,0,maxSpeedMotorRA); // map(valPotX,515,1024,0,maxSpeedX); 
    AzMotor.setSpeed(-motorSpeedX);  
  }
  AzMotor.runSpeed();
  valPotY =  analogRead(yPin); 
  if (valPotY <= 449){
    mDEC_norte = true;
    mDEC_sul = false;
    motorSpeedY =map(valPotY,499,0,0,maxSpeedMotorDEC); 
    AltMotor.setSpeed(motorSpeedY);
  } else if (valPotY > 449 && valPotY < 573) {
    mDEC_norte = false;
    mDEC_sul = false;
    motorSpeedY = 0;
    AltMotor.setSpeed(motorSpeedY);
  } else if (valPotY >= 573){
    mDEC_norte = false;
    mDEC_sul = true;
    motorSpeedY = map(valPotY,573,1023,0,maxSpeedMotorDEC); 
    AltMotor.setSpeed(-motorSpeedY);
  }  
  AltMotor.runSpeed();
  movimentoRADEC();
}

void controlFocuser() { // Movimento manual do motor do focalizador (incluído por L.H.Bonani)
    valPotFoc =  analogRead(xPin); 
    if (valPotFoc <= 449){
      int motorSpeed =map(valPotFoc,449,0,0,maxSpeedMotorFoc); 
      focMotor.setSpeed(motorSpeed);
    } else if (valPotFoc > 449 && valPotFoc < 573) {
      int motorSpeed = 0;
      focMotor.setSpeed(motorSpeed);
    } else if (valPotFoc >= 565){
      int motorSpeed =map(valPotFoc,573,1023,0,maxSpeedMotorFoc); 
      focMotor.setSpeed(-motorSpeed);
    }
    focMotor.runSpeed();
}

void BaixaResolucao () { // Modificado por L.H.Bonani para permitir o uso do TMC2209
  if ( MaxPassoAz == dMaxPassoAz) {
    if (tmcFlag == false) {
      MaxPassoAz = dMaxPassoAz / dReducao;
      MaxPassoAlt = dMaxPassoAlt / dReducao;
      digitalWrite(MotorALT_M2, LOW);
      digitalWrite(MotorALT_M1, LOW);
      digitalWrite(MotorALT_M0, LOW);
      digitalWrite(MotorAZ_M2, LOW);
      digitalWrite(MotorAZ_M1, LOW );
      digitalWrite(MotorAZ_M0, LOW);
      AltMotor.setCurrentPosition((int)AltMotor.currentPosition() / dReducao);
      AzMotor.setCurrentPosition((int)AzMotor.currentPosition() / dReducao);
      AltMotor.setAcceleration(dReducao * 4);
      AzMotor.setAcceleration(dReducao * 4);
    } else {
      MaxPassoAz = dMaxPassoAz / dReducao * 8;
      MaxPassoAlt = dMaxPassoAlt / dReducao * 8;    
      digitalWrite(MotorALT_M1, LOW);
      digitalWrite(MotorALT_M0, LOW);
      digitalWrite(MotorAZ_M1, LOW );
      digitalWrite(MotorAZ_M0, LOW);  
      AltMotor.setCurrentPosition((int)AltMotor.currentPosition() / dReducao * 8);
      AzMotor.setCurrentPosition((int)AzMotor.currentPosition() / dReducao * 8);
    }
    CalculaResolucao();
    CalcPosicaoPasso();
    ledStateG = HIGH;
  }
}

void AltaResolucao () {  // Modificado por L.H.Bonani para permitir o uso do TMC2209
  if ( MaxPassoAz != dMaxPassoAz) {
    MaxPassoAz = dMaxPassoAz;
    MaxPassoAlt = dMaxPassoAlt;
    if (tmcFlag == false) {
       digitalWrite(MotorALT_M2, AltaM2);
       digitalWrite(MotorALT_M1, AltaM1);
       digitalWrite(MotorALT_M0, AltaM0);
       digitalWrite(MotorAZ_M2, AltaM2);
       digitalWrite(MotorAZ_M1, AltaM1);
       digitalWrite(MotorAZ_M0, AltaM0);
       AltMotor.setCurrentPosition((int)AltMotor.currentPosition() * dReducao);
       AzMotor.setCurrentPosition((int)AzMotor.currentPosition() * dReducao);
       AltMotor.setAcceleration(dReducao * dReducao *  2);
       AzMotor.setAcceleration(dReducao * dReducao * 2);
    } else {
       digitalWrite(MotorALT_M1, AltaM1);
       digitalWrite(MotorALT_M0, AltaM0);
       digitalWrite(MotorAZ_M1, AltaM1);
       digitalWrite(MotorAZ_M0, AltaM0);
       AltMotor.setCurrentPosition((int)AltMotor.currentPosition() * dReducao / 8);
       AzMotor.setCurrentPosition((int)AzMotor.currentPosition() * dReducao / 8);
    }
    CalculaResolucao();
    CalcPosicaoPasso();
    ledStateG = LOW;
  }
}

void BaixaResolucaoAz ()
{
  if ( MaxPassoAz == dMaxPassoAz)
  {
    MaxPassoAz = dMaxPassoAz / dReducao;
    digitalWrite(MotorAZ_M2, LOW);
    digitalWrite(MotorAZ_M1, LOW );
    digitalWrite(MotorAZ_M0, LOW);
    AzMotor.setCurrentPosition((int)AzMotor.currentPosition() / dReducao);
    AzMotor.setAcceleration(dReducao * 4);
    CalculaResolucao();
    CalcPosicaoPasso();
    ledStateG = HIGH;
  }
}
void BaixaResolucaoAlt ()
{
  if ( MaxPassoAlt == dMaxPassoAlt)
  {
    MaxPassoAlt = dMaxPassoAlt / dReducao;
    digitalWrite(MotorALT_M2, LOW);
    digitalWrite(MotorALT_M1, LOW);
    digitalWrite(MotorALT_M0, LOW);
    AltMotor.setCurrentPosition((int)AltMotor.currentPosition() / dReducao);
    AltMotor.setAcceleration(dReducao * 4);
    CalculaResolucao();
    CalcPosicaoPasso();
    ledStateG = HIGH;
  }
}


void AltaResolucaoAz ()
{
  if ( MaxPassoAz != dMaxPassoAz)
  {
    MaxPassoAz = dMaxPassoAz;
    digitalWrite(MotorAZ_M2, AltaM2);
    digitalWrite(MotorAZ_M1, AltaM1);
    digitalWrite(MotorAZ_M0, AltaM0);
    AzMotor.setCurrentPosition((int)AzMotor.currentPosition() * dReducao);
    CalculaResolucao();
    CalcPosicaoPasso();
    AzMotor.setAcceleration(dReducao * dReducao * 2);
    ledStateG = LOW;
  }
}
void AltaResolucaoAlt ()
{
  if ( MaxPassoAlt != dMaxPassoAlt)
  {
    MaxPassoAlt = dMaxPassoAlt;
    digitalWrite(MotorALT_M2, AltaM2);
    digitalWrite(MotorALT_M1, AltaM1);
    digitalWrite(MotorALT_M0, AltaM0);
    AltMotor.setCurrentPosition((int)AltMotor.currentPosition() * dReducao);
    CalculaResolucao();
    CalcPosicaoPasso();
    AltMotor.setAcceleration(dReducao * dReducao * 2);
    ledStateG = LOW;
  }
}





void protegemount()
{
  if ((eixoAltGrausDecimal < 1 ) || (eixoAltGrausDecimal > 90))
  {
    paramotors();
    ativaacom = 0;
  }


  if ((eixoAzGrausDecimal < 1) || (eixoAzGrausDecimal > 360))
  {
    paramotors();
    ativaacom = 0;
  }
}
