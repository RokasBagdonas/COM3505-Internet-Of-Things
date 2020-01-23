/**
 * For handling messages via Telegram API bot.
 * Requires to have Telegram bot api key, that is inserted in private.h.
 * Uses Universal Telegram Bot lib:  https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot 
 * */
#include "telegram.h"
String telegramApiKey = _TELEGRAM_API_KEY;
WiFiClientSecure netSSL;
UniversalTelegramBot bot(telegramApiKey, netSSL);

String responseTimeMessage = "Messages Received! Please be aware messages are processed in batches, so there may be a small delay between you (the user) sending a command, and the command begin received and processed";
/**
 * called from main::loop() every BOT_INTERVAL ms. 
 * @return number of new messages. The number is used in handleNewMessages.
 * */
int checkMessages()
{
  return bot.getUpdates(bot.last_message_received + 1);
}

/**
 * Checks every message and calls an appropriate plug toggling function 
 * if input message is recognised. 
 * */
void handleNewMessages(int numNewMessages)
{
    // Generic reponse about wait times
    bot.sendSimpleMessage(bot.messages[0].chat_id, responseTimeMessage, "");
    
    // Process each message
    for (int i=0; i < numNewMessages; i++)
    {
      String text = bot.messages[i].text;
      if(text[0] == '/') text.remove(0, 1);
      Serial.printf("Telegram: ");
      Serial.println(text.c_str());
      if (text == "2_ON")
      {
        plug2On();
        Serial.println("plug2On");
      }
      else if (text == "2_OFF")
      {
        plug2Off();
        Serial.println("plug2Off");
      }
      else if (text == "3_ON")
      {
        plug3On();
        bot.sendSimpleMessage(bot.messages[i].chat_id, "1408_3 ON", "");
        Serial.println("plug3On");
      }
      else if (text == "3_OFF")
      {
        plug3Off();
        bot.sendSimpleMessage(bot.messages[i].chat_id, "1408_3 OFF", "");
        Serial.println("plug3Off");
      }
        else if (text == "start")
        {
            bot.sendSimpleMessage(bot.messages[i].chat_id,"Hi!", "");
        }
      else
      {
          bot.sendSimpleMessage(bot.messages[i].chat_id,"Command Not Recognised", "");
        //respond that the command is not recognised
      }
        
    }
}

void setTelegramApiKey(String newTelegramApiKey){
    try {
        telegramApiKey = newTelegramApiKey;
    }
    catch (...){
        resetTelegramApiKey();
    };
}

void resetTelegramApiKey(){
  telegramApiKey = _TELEGRAM_API_KEY;
}