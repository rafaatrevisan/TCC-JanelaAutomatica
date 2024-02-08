#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Stepper.h>

// Wifi network station credentials
#define WIFI_SSID "NOME_DO_WIFI"
#define WIFI_PASSWORD "SENHA_DO_WIFI"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "6190509228:AAGxj91Rv7h4Qpm9FiqFetwEHxxVloWjYH4"

const unsigned long BOT_MTBS = 1000; // mean time between scan messages

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // last time messages' scan has been done

const int pinoSensor = D5; //PINO DIGITAL UTILIZADO PELO SENSOR
int chuva;
const int pinoAberto = D7;
const int pinoFechado = D6;
bool Janela;
const int passos = 64;
Stepper motor(passos, D1, D3, D2, D4);

void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";

    if (text == "/abrir")
    {
      if (digitalRead(pinoAberto) == LOW) {
          bot.sendMessage(chat_id, "A janela já está aberta.", "");
      } else {
    
      while (digitalRead(pinoAberto) == HIGH) {
        motor.step(-1);
        delay(10);
      }

      bot.sendMessage(chat_id, "Janela aberta.", "");
      delay(1000);
      Janela = true;
       }
    }

    if (text == "/fechar")
    {
      if (digitalRead(pinoFechado) == LOW) {
          bot.sendMessage(chat_id, "A janela já está fechada.", "");
      } else {
      
       while (digitalRead(pinoFechado) == HIGH) {
        motor.step(1);
        delay(10);
      }

      bot.sendMessage(chat_id, "Janela fechada.", "");
      delay(1000);
      Janela = false;
      }

    }

    if (text == "/status")
    {
      if (digitalRead(pinoAberto) == LOW) {
          bot.sendMessage(chat_id, "A janela está aberta.", "");
      }
      if (digitalRead(pinoFechado) == LOW) {
          bot.sendMessage(chat_id, "A janela está fechada.", "");
      }
    }


    if (text == "/start")
    {
      String welcome = "Bem-vindo ao bot da OpenWindow, " + from_name + ".\n";
      welcome += "Aqui você pode controlar sua janela remotamente.\n\n";
      welcome += "/abrir : para abrir a janela\n";
      welcome += "/fechar : para fechar a janela\n";
      welcome += "/status : para saber se a janela está fechada ou aberta\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}


void setup()
{
  Serial.begin(115200);
  pinMode(pinoAberto, INPUT_PULLUP);
  pinMode(pinoFechado, INPUT_PULLUP);
  pinMode(pinoSensor, INPUT);

  // attempt to connect to Wifi network:
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Check NTP/Time, usually it is instantaneous and you can delete the code below.
  Serial.print("Retrieving time: ");
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void loop()
{
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }

  motor.setSpeed(200);

  if (digitalRead(pinoFechado) == LOW) {
  Serial.println("JANELA FECHADA.");
  delay(1000);
  Janela = false;
  } else {
    Janela = true;

    chuva = analogRead(pinoSensor);
    Serial.begin(115200);
    
    if (chuva == 0 && Janela == true)
    {
      Serial.println("CHUVA DETECTADA, FECHANDO JANELA");
      delay(1000);

      while (digitalRead(pinoFechado) == HIGH) {
        motor.step(1);
        delay(10);
      }

    }

    else
    {
      Serial.println("NAO ESTA CHOVENDO.");
      delay(2000);
    }
  }



  // Aguarda um breve período antes da próxima leitura
  delay(100);
}


