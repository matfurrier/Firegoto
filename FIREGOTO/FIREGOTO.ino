/*
 *   FireGoTo - an Arduino Motorized Telescope Project for Dobsonian Mounts
    Copyright (C) 2020  Rangel Perez Sardinha / Marcos Lorensini

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

    TMC BRANCH
 * 
 */
#include <AccelStepper.h>
#include <Arduino.h>
#include <math.h>
#include <Time.h>
#include <TimeLib.h>
#include <DueTimer.h>
#include <DueFlashStorage.h>
#include <TMC2130Stepper.h>
#include <TMC2130Stepper_REGDEFS.h>
#include <TMC2130Stepper_UTILITY.h>



//DEBUG
int flagDebug = 0;

<<<<<<< Updated upstream
=======
//2. Pinos Joystick
#define xPin     A0   
#define yPin     A1   
#define kPin     A2   // botão

//Menu e joystick
int tCount1;
bool refresh;//lcd clear On/Off
//leerJoystick
int joyRead;
int joyPos; 
int lastJoyPos;
long lastDebounceTime = 0; 
long debounceDelay = 70;                 //user define
//Control Joystick
bool PQCP;
bool editMode;
//sistema de menus
int mNivel1;  
int mNivel2;  
//editmode
byte n[19];
int lastN;
int lcdX;
int jBut;
//int lcdY;
bool exiT;
bool moveRADEC;

>>>>>>> Stashed changes

// Variáveis de estado (incluído por L. H. Bonani)
bool focStatus = false;
bool manualMoveRADEC = false;
bool tmcFlag = true;

//  Variáveis para a identificação dos movimentos manuais dos motores (incluído por L. H. Bonani)
bool mRA_leste = false;
bool mRA_oeste = false;
bool mDEC_norte = false;
bool mDEC_sul = false;
int motorSpeedX = 0;
int motorSpeedY = 0;

//Criacao dos motores
#define MotorALT_Direcao 22
#define MotorALT_Passo 24
#define MotorALT_CS 30
#define MotorALT_CFG2 32
#define MotorALT_CFG1 34
#define MotorALT_Ativa 36
#define MotorAZ_Direcao 38
#define MotorAZ_Passo 40
#define MotorAZ_CS 46
#define MotorAZ_CFG2 48
#define MotorAZ_CFG1 50
#define MotorAZ_Ativa 52

// Variáveis para o controle do motor do focalizador usando portas do módulo GPS (incluído por L. H. Bonani)
#define MotorFoc_Direcao 31 // TX GPS
#define MotorFoc_Passo 33 // RX GPS
#define MotorFoc_Enable 35 // 

AccelStepper AltMotor(AccelStepper::DRIVER, MotorALT_Passo, MotorALT_Direcao);
AccelStepper AzMotor(AccelStepper::DRIVER, MotorAZ_Passo, MotorAZ_Direcao);
AccelStepper focMotor = AccelStepper(AccelStepper::DRIVER, MotorFoc_Passo, MotorFoc_Direcao); // Motor do focalizador (incluído por L. H. Bonani)

int accel = 1;


//LEDs
#define LedB 49
#define LedR 53
#define LedG 51
int ledStateR = LOW;
int ledStateB = LOW;
int ledStateG = LOW;
long lastInputCheckTime = 0;

/*valores maximo para o passo (Valor ideal 1286400)*/
<<<<<<< Updated upstream
double dMaxPassoAlt = 3844654; /* //valor de resolucao AR = Passo * MicroPasso * reducao ex(200*16*402)/4    (16*200*(117/11)*56)*/
double dMaxPassoAz = 3844654; /*/valor de resolucao AR = Passo * MicroPasso * reducao ex(200*16*402)   (16*200*(118/11)*57)*/
int dMinTimer = 500; /*/passo*/
double dMaxSpeedAlt = 3844654;
double dMaxSpeedAz = 3844654;
int dReducao = 16;
=======
double dMaxPassoAlt = 6384000; /* //valor de resolucao AR = Passo * MicroPasso * reducao ex(200*16*402)/4    (16*200*(117/11)*56)*/
double dMaxPassoAz = 6384000; //6400; //2304000; /*/valor de resolucao AZ = Passo * MicroPasso * reducao ex(200*16*402)   (16*200*(118/11)*57)*/
double dMaxPassoFoc = 1600;
int dMinTimer = 150; /*/passo*/
double dMaxSpeedAlt = 8000;
double dMaxSpeedAz = 8000; 
double dMaxSpeedFoc = 4000;
int dReducao = 16; // Quantidade de micropassos

// Para o controle manual dos motores (Incluído por L.H.Bonani)
int valPotFoc = 0;
int valPotX = 0;
int valPotY = 0;
int maxSpeedMotorFoc = 4000; // 1000 ok
int maxSpeedMotorRA = 13300; // Cálculo em função de dMaxPassoAz para uma velocidade de 10 graus/segundo (36 segundos para uma volta completa) - Incluído por L.H.Bonani
int maxSpeedMotorDEC = 13300 ; // Cálculo em função de dMaxPassoAlt para uma velocidade de 10 graus/segundo (36 segundos para uma volta completa) - Incluído por L.H.Bonani
 
>>>>>>> Stashed changes


//Variaveis de persistencia e estrutura de dados ----------------------------------------------------------------------------------------------------------------
DueFlashStorage dueFlashStorage;

// The struct of the configuration.
struct Configuration {
  int32_t Reducao;
  int32_t MaxPassoAlt;
  int32_t MaxPassoAz;
  int32_t MinTimer;
  int32_t AltaM2;
  int32_t AltaM1;
  int32_t AltaM0;
  int32_t SentidoDEC;
  int32_t SentidoRA;
  uint32_t DataHora;
  double latitude;
  double longitude;
  int32_t UTC;
  char* Local;
};
Configuration configuration;
Configuration configurationFromFlash; // create a temporary struct

int Reducao;
int MaxPassoAlt;
int MaxPassoAz;
int MaxSpeedAz = dMaxSpeedAz;
int MaxSpeedAlt = dMaxSpeedAlt;
int SentidoDEC = 0;
int SentidoRA = 0;
int MinTimer;
int AltaM2;
int AltaM1;
int AltaM0;
double latitude;
double longitude;
int UTC;



int fractime;
unsigned long currentMillis, previousMillis, PrimeiroCommanMillis, calculaRADECmountMillis = 0 , previousMillisLed = 0;


//Variaveis de controle para ler comandos LX200  ----------------------------------------------------------------------------------------------------------------
boolean cmdComplete = false, doispontos = true; // whether the string is complete
int pontBuffer = 0;
int numCommand = 0, numCommandexec = 0, flagCommand = 0;
char Command[15][15];



//Variaveis do SETUP
int setupflag = 0;

//Variaveis globais da MEADE
double SideralRate = 60.0; //HZ
int RAbacklash = 0; //(in ArcSec)
int DECbacklash = 0; //(in ArcSec)
int AtivaBack = 1;
int dirAlt, dirAz, dirAltant, dirAzant;

//Variaveis globais de posicao fisica do telescopio  ----------------------------------------------------------------------------------------------------------------
double eixoAltGrausDecimal = 0.0;
double eixoAzGrausDecimal = 0.0;
double ResolucaoeixoAltGrausDecimal = 0.0, ResolucaoeixoAltPassoGrau = 0.0;
double ResolucaoeixoAzGrausDecimal = 0.0, ResolucaoeixoAzPassoGrau = 0.0;
double RAmount = 0.0;
double DECmount = 0.0;
double AZmount = 0.0;
double ALTmount = 0.0;
double AZmountAlvo = 0.1;
double ALTmountAlvo = 0.1;


double DistanciaTotal = 0.0;

//Alvos a serem seguidos ou sincronizado
double RAAlvo = 0.0;
double DECAlvo = 0.0;
double AzAlvo = 0.0;
double AltAlvo = 0.0;
double ErroAlt = 0.0;
double ErroAz = 0.0;


int statusmovimentacao = 0;
int ativaacom = 0;
int gotomount = 0;
//limites da montagem
int HorizonteLimite = 0;
int AltitudeLimite = 90;

//Ajuste fino do tempo
int Segundo;
double Microssegundo = 0 , SegundoFracao = 0.0, MilissegundoSeg = 0.0, MilissegundoI = 0.0;


void setup() {
  pinMode(LedR, OUTPUT);
  pinMode(LedG, OUTPUT);
  pinMode(LedB, OUTPUT);
  digitalWrite(LedR, ledStateR);
  digitalWrite(LedB, ledStateB);
  digitalWrite(LedG, ledStateG);
<<<<<<< Updated upstream

=======
  //Pinos Joystick
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(kPin, INPUT_PULLUP);
>>>>>>> Stashed changes


  if (ledStateR == LOW) {
    ledStateR = HIGH;
  } else {
    // ledStateR = LOW;
  }
  digitalWrite(LedR, ledStateR);

  Serial.begin(9600);
  Serial3.begin(9600);
  SerialUSB.begin(9600);
<<<<<<< Updated upstream
=======
  Wire1.begin();
>>>>>>> Stashed changes



  /* Flash is erased every time new code is uploaded. Write the default configuration to flash if first time */
  // running for the first time?
  uint8_t codeRunningForTheFirstTime = dueFlashStorage.read(0); // flash bytes will be 255 at first run
  Serial.print("Primeira Execucao: ");
  if (codeRunningForTheFirstTime) {
    Serial.println("yes");
    /* OK first time running, set defaults */
    configuration.Reducao = dReducao;
    configuration.MaxPassoAlt = dMaxPassoAlt;
    configuration.MaxPassoAz = dMaxPassoAz;
    configuration.MinTimer = dMinTimer;
    configuration.latitude = -25.40;;
    configuration.longitude = -49.20;
    configuration.SentidoDEC = 0;
    configuration.SentidoRA = 0;
    setTime(22, 00, 00, 23, 03, 2015);
    MilissegundoSeg = second();
    configuration.DataHora = now();
    configuration.UTC = -2;
    strcpy (configuration.Local, "Minha Casa");
    // write configuration struct to flash at adress 4
    byte b2[sizeof(Configuration)]; // create byte array to store the struct
    memcpy(b2, &configuration, sizeof(Configuration)); // copy the struct to the byte array
    dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash
    // write 0 to address 0 to indicate that it is not the first time running anymore
    dueFlashStorage.write(0, 0);
  }
  else {
    Serial.println("no");
  }


  
  byte* b = dueFlashStorage.readAddress(4); // byte array which is read from flash at adress 4
  memcpy(&configurationFromFlash, b, sizeof(Configuration)); // copy byte array to temporary struct
  Reducao = configurationFromFlash.Reducao;
   // Alteração da configuração dos motores em função do tipo de driver (incluído por L. H. Bonani) 
   if(Reducao==64) {
    if (tmcFlag == false) {
      Serial.print("Configuração de 64 micropassos para DVR8825 não suportada. Configurando para 32 micropassos. \n");
      AltaM2 = HIGH;
      AltaM1 = LOW;
      AltaM0 = HIGH;
    } else {
      AltaM0 = LOW;
      AltaM1 = HIGH;
    } 
  }
  if(Reducao==32) {
    if (tmcFlag == false) {
      AltaM2 = HIGH;
      AltaM1 = LOW;
      AltaM0 = HIGH;
    } else {
      AltaM0 = HIGH;
      AltaM1 = LOW;
    }
  }
  if(Reducao==16) {
<<<<<<< Updated upstream
    AltaM2 = LOW;
    AltaM1 = LOW;
    AltaM0 = LOW;
=======
    if (tmcFlag == false) {
      AltaM2 = HIGH;
      AltaM1 = LOW;
      AltaM0 = LOW;
    } else {
      AltaM0 = HIGH;
      AltaM1 = HIGH;
    }
>>>>>>> Stashed changes
  }
  if(Reducao==8) {
    if (tmcFlag == false) {
      AltaM2 = LOW;
      AltaM1 = HIGH;
      AltaM0 = HIGH;
    } else {
      AltaM0 = LOW;
      AltaM1 = LOW;
    }
  }
  if(Reducao==4) {
    if (tmcFlag == false) {
      AltaM2 = LOW;
      AltaM1 = HIGH;
      AltaM0 = LOW;
    } else {
      Serial.print("Configuração de 4 micropassos para TMC2209 não suportada. Configurando para 8 micropassos. \n");
      AltaM0 = LOW;
      AltaM1 = LOW;
    }
  }
  if(Reducao==2) {
    if (tmcFlag == false) {
      AltaM2 = LOW;
      AltaM1 = LOW;
      AltaM0 = HIGH;
    } else {
      Serial.print("Configuração de 2 micropassos para TMC2209 não suportada. Configurando para 8 micropassos. \n");
      AltaM0 = LOW;
      AltaM1 = LOW;
    }
  }
  MaxPassoAlt = configurationFromFlash.MaxPassoAlt;
  MaxPassoAz = configurationFromFlash.MaxPassoAz;
  dReducao = Reducao;
  dMaxPassoAlt =   MaxPassoAlt;
  dMaxPassoAz =  MaxPassoAz;
  MinTimer = configurationFromFlash.MinTimer;
  latitude = configurationFromFlash.latitude;
  longitude = configurationFromFlash.longitude;
  UTC = configurationFromFlash.UTC;
  SentidoDEC = configurationFromFlash.SentidoDEC;
  SentidoRA = configurationFromFlash.SentidoRA;
  setTime(configurationFromFlash.DataHora);
  Serial.print("Bem vindo ao FIREGOTO para setup inicial digitar :HSETUPON# \n");
  delay (2000);
  IniciaMotores();
  SentidodosMotores();
  SerialPrint("00:00:00#"); //RTA para leitura do driver ASCOM da MEADE autostar I
  delay (200);
  previousMillis = millis();
  PrimeiroCommanMillis = previousMillis;
  ErroAlt = ErroAz = 44.0;
  ResolucaoeixoAltGrausDecimal = 360.0 / MaxPassoAlt ;
  ResolucaoeixoAzGrausDecimal = 360.0 / MaxPassoAz ;
  ResolucaoeixoAltPassoGrau = (MaxPassoAlt  / 360.0);
  ResolucaoeixoAzPassoGrau = (MaxPassoAz  / 360.0);
<<<<<<< Updated upstream
=======
  //Instruções do LCD
  lcd.begin(Wire1); 
  lcd.backlight();
  lcd.clear();
>>>>>>> Stashed changes
}

void loop() {
  if (ledStateR == LOW) {
    ledStateR = HIGH;
  } else {
    // ledStateR = LOW;
  }
  currentMillis = millis();
  CalcPosicaoPasso();
  if (SerialUSB.available() || Serial.available() || Serial3.available()) serialEvent();

  if ((numCommand != numCommandexec) && (flagCommand == 0))
  {
    SerialPrintDebug(String(numCommandexec));
    SerialPrintDebug(String(numCommand));
    cmdComplete = true;
    executecommand();
    SerialPrintDebug(String(numCommandexec));
    SerialPrintDebug(String(numCommand));
  }
  if ( setupflag == 0 )
  {
    if (PrimeiroCommanMillis < currentMillis)

    {
      PrintLocalHora();
      SerialPrintDebug(String(Hora2DecHora(hour(), minute(), SegundoFracao), 10)) ;
      PrimeiroCommanMillis = PrimeiroCommanMillis + 1001;
    }
  }
  if ( setupflag == 1 )
  {
    RotinadeSetup();
  }
  if (calculaRADECmountMillis < currentMillis)
  {
    calculaRADECmountMillis = calculaRADECmountMillis + 113;
    calculaRADECmount();
  }

  AlteraMicroSeg();
  if (previousMillisLed < currentMillis)
  {
    previousMillisLed = currentMillis + 150;
    digitalWrite(LedR, ledStateR);
    digitalWrite(LedB, ledStateB);
    digitalWrite(LedG, ledStateG);
    ledStateB = LOW;
  }


  // print the string when a newline arrives:
  // protegemount();

  if (ativaacom != 0)
  {
    if (previousMillis < currentMillis)
    {
      setaccel();
      previousMillis = currentMillis + 237;
      DistanciaTotal = abs(AltMotor.distanceToGo()) + abs(AzMotor.distanceToGo());
      if ( MaxPassoAlt == dMaxPassoAlt)
      {
        if ( DistanciaTotal > (MaxPassoAz / 90))
        {
          BaixaResolucao();
        }
      }
      else
      {
        if ( DistanciaTotal < (MaxPassoAz / 180))
        {
          AltaResolucao();
        }
      }
      SerialPrintDebug("Distancia Total: ");
      SerialPrintDebug(String(DistanciaTotal));
      SerialPrintDebug(String(MaxPassoAz));
    }
    SetPosition();
    acompanhamento();
  }
  else
  {
    previousMillis = millis();
  }
  AlteraMicroSeg();
<<<<<<< Updated upstream

=======
  controlJoystick2();
  menu();
>>>>>>> Stashed changes
}
