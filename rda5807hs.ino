// Very simple test code for the RDA5807M (RRD-102 module) FM radio receiver IC
// Luc Small
// lucsmall.com
// 20150518
//
// Find this code at: https://github.com/lucsmall/Arduino-RDA5807M
//
// See these videos for more:
// "Talking about my Arduino sketch for the RDA5807M FM Radio IC"
// http://youtu.be/9TegkAu96nE
//
// "Connecting the RDA5807M FM Radio IC to an Arduino"
// https://youtu.be/2j1hX4uzkQ8
//
// "Making my Arduino controlled RDA5807M FM Radio"
// https://youtu.be/4pZmkeqg5h8

#include <Wire.h>

// Select the frequency we want to tune to by way
// of selecting the channel for the desired frequency
//uint16_t channel = 209;
uint16_t channel_9300 = 60;
uint16_t channel_9160 = 46;
uint16_t channel_9050 = 35;
uint16_t channel_8000 = 40;  //80-76=4
   // assuming band starts at 87.0MHz (per settings below)
   // and channel spacing of 100kHz (0.1MHz) (per settings below)
   // then channel can be derived as follows:
   //
   // channel = (<desired freq in MHz> - 87.0) / 0.1 
   //
   // which is the same as:
   //
   // <10 x desired freq in MHz> - 870
   //
   // some examples:
   //
   // channel   0   dec =  87.0 MHz
   // channel  10   dec =  88.0 MHz
   // channel  20   dec =  89.0 MHz
   // channel  30   dec =  90.0 MHz
   // channel  40   dec =  91.0 MHz
   // channel  50   dec =  92.0 MHz
   // channel  60   dec =  93.0 MHz
   // channel  70   dec =  94.0 MHz
   // channel  80   dec =  95.0 MHz
   // channel  90   dec =  96.0 MHz
   // channel 100   dec =  97.0 MHz
   // channel 110   dec =  98.0 MHz
   // channel 120   dec =  99.0 MHz
   // channel 130   dec = 100.0 MHz
   // channel 140   dec = 101.0 MHz
   // channel 150   dec = 102.0 MHz
   
   
   
   // channel 145 dec = 101.5 MHz
   // channel 153 dec = 102.3 MHz
   // channel 177 dec = 104.7 MHz
   // channel 193 dec = 106.3 MHz
   // channel 209 dec = 107.9 MHz


// address of the RDA5807 on two wire bus
#define RDA5807M_ADDRESS  0b0010000 // 0x10

#define BOOT_CONFIG_LEN 12
#define TUNE_CONFIG_LEN 4

// These bytes set our initial configuration
// We don't bother to tune to a channel at this stage.
// But instead initiate a reset.
uint8_t boot_config[] = {
  /* register 0x02 */
  0b11000000,
    // DHIZ audio output high-z disable
    // 1 = normal operation
    
    // DMUTE mute disable 
    // 1 = normal operation
    
    // MONO mono select
    // 0 = stereo
    
    // BASS bass boost
    // 0 = disabled
    
    // RCLK NON-CALIBRATE MODE 
    // 0 = RCLK is always supplied
    
    // RCLK DIRECT INPUT MODE 
    // 0 = ??? not certain what this does
    
    // SEEKUP
    // 0 = seek in down direction
    
    // SEEK
    // 0 = disable / stop seek (i.e. don't seek)
    
    0b00000011,
    // SKMODE seek mode: 
    // 0 = wrap at upper or lower band limit and contiue seeking
    
    // CLK_MODE clock mode
    //  000 = 32.768kHZ clock rate (match the watch cystal on the module) 
    
    // RDS_EN radio data system enable
    // 0 = disable radio data system
    
    // NEW_METHOD use new demodulate method for improved sensitivity
    // 0 = presumably disabled 
    
    // SOFT_RESET
    // 1 = perform a reset
    
    // ENABLE power up enable: 
    // 1 = enabled 

  /* register 0x03 */
  // Don't bother to tune to a channel at this stage
  0b00000000, 
    // CHAN channel select 8 most significant bits of 10 in total
    // 0000 0000 = don't boher to program a channel at this time

  0b00000000,
    // CHAN two least significant bits of 10 in total 
    // 00 = don't bother to program a channel at this time
    
    // DIRECT MODE used only when test
    // 0 = presumably disabled
    
    // TUNE commence tune operation 
    // 0 = disable (i.e. don't tune to selected channel)
    
    // BAND band select
    // 00 = select the 87-108MHz band
    
    // SPACE channel spacing
    // 00 = select spacing of 100kHz between channels
    
  /* register 0x04 */
  0b00001010, 
    // RESERVED 15
    // 0
    
    // PRESUMABLY RESERVED 14
    // 0
    
    // RESERVED 13:12
    // 00
    
    // DE de-emphasis: 
    // 1 = 50us de-emphasis as used in Australia
    
    // RESERVED
    // 0
    
    // SOFTMUTE_EN
    // 1 = soft mute enabled
    
    // AFCD AFC disable
    // 0 = AFC enabled
    
  0b00000000, 
    // Bits 7-0 are not specified, so assume all 0's
    // 0000 0000
  
  /* register 0x05 */
  0b10001000, 
    // INT_MODE
    // 1 = interrupt last until read reg 0x0C
    
    // RESERVED 14:12 
    // 000
    
    // SEEKTH seek signal to noise ratio threshold
    // 1000 = suggested default 
  
  0b00001111,
  //0b00000000, //0
  //0b00000001, //1
  //0b00000010, //2
  //0b00000011, //3
  //0b00000100, //4
  //0b00000101, //5
  //0b00000110, //6
  //0b00000111, //7
  //0b00001000, //8
  //0b00001001, //9
  //0b00001010, //10
  //0b00001011, //11
  //0b00001100, //12
  //0b00001101, //13
  //0b00001110, //14
  //0b00001111, //15
   
    // PRESUMABLY RESERVED 7:6
    // 00
    
    // RESERVED 5:4
    // 00
    
    // VOLUME
    // 1111 = loudest volume
  
  /* register 0x06 */
  0b00000000, 
    // RESERVED 15
    // 0
    
    // OPEN_MODE open reserved registers mode
    // 00 = suggested default
    
    // Bits 12:8 are not specified, so assume all 0's
    // 00000 
   
  0b00000000, 
    // Bits 7:0 are not specified, so assume all 0's
    // 00000000
    
  /* register 0x07 */
  0b01000010, 
    // RESERVED 15 
    // 0
    
    // TH_SOFRBLEND threshhold for noise soft blend setting
    // 10000 = using default value
    
    // 65M_50M MODE 
    // 1 = only applies to BAND setting of 0b11, so could probably use 0 here too
    
    // RESERVED 8
    // 0    
  
  0b00000010, 
    // SEEK_TH_OLD seek threshold for old seek mode
    // 000000
    
    // SOFTBLEND_EN soft blend enable
    // 1 = using default value
    
    // FREQ_MODE
    // 0 = using defualt value  
};





// After reset, we can tune the device
// We only need program the first 4 bytes in order to do this
uint8_t tune_config_9300[] = {
  /* register 0x02 */
  0b11000000, 
    // DHIZ audio output high-z disable
    // 1 = normal operation
    
    // DMUTE mute disable 
    // 1 = normal operation
    
    // MONO mono select
    // 0 = stereo
    
    // BASS bass boost
    // 0 = disabled
    
    // RCLK NON-CALIBRATE MODE 
    // 0 = RCLK is always supplied
    
    // RCLK DIRECT INPUT MODE 
    // 0 = ??? not certain what this does
    
    // SEEKUP
    // 0 = seek in down direction
    
    // SEEK
    // 0 = disable / stop seek (i.e. don't seek)
    
   0b00000001, 
    // SKMODE seek mode: 
    // 0 = wrap at upper or lower band limit and contiue seeking
    
    // CLK_MODE clock mode
    //  000 = 32.768kHZ clock rate (match the watch cystal on the module) 
    
    // RDS_EN radio data system enable
    // 0 = disable radio data system
    
    // NEW_METHOD use new demodulate method for improved sensitivity
    // 0 = presumably disabled 
    
    // SOFT_RESET
    // 0 = don't reset this time around
    
    // ENABLE power up enable: 
    // 1 = enabled 

   /* register 0x03 */
   /* Here's where we set the frequency we want to tune to */
   (channel_9300 >> 2), 
    // CHAN channel select 8 most significant bits of 10 in total   

   ((channel_9300 & 0b11) << 6 ) | 0b00010000
    // CHAN two least significant bits of 10 in total 
    
    // DIRECT MODE used only when test
    // 0 = presumably disabled
    
    // TUNE commence tune operation 
    // 1 = enable (i.e. tune to selected channel)
    
    // BAND band select
    // 00 = select the 87-108MHz band
    
    // SPACE channel spacing
    // 00 = select spacing of 100kHz between channels  
};



// After reset, we can tune the device
// We only need program the first 4 bytes in order to do this
uint8_t tune_config_9050[] = {
   0b11000000,   
   0b00000001, 
   (channel_9050 >> 2), 
   ((channel_9050 & 0b11) << 6 ) | 0b00010000
};

// After reset, we can tune the device
// We only need program the first 4 bytes in order to do this
uint8_t tune_config_9160[] = {
   0b11000000,   
   0b00000001, 
   (channel_9160 >> 2), 
   ((channel_9160 & 0b11) << 6 ) | 0b00010000
};


// After reset, we can tune the device
// We only need program the first 4 bytes in order to do this
uint8_t tune_config_8000[] = {
   0b11000000,   
   0b00000001, 
   (channel_8000 >> 2),              //  BB
   ((channel_8000 & 0b11) << 6 ) | 0b00011000
};


uint8_t vol_config[] = {
  /*V register 0x02 */
  0b11000000,
//0b00000011,
  0b00000001,
    //V SKMODE seek mode: 
    //V 0 = wrap at upper or lower band limit and contiue seeking
    
    //V CLK_MODE clock mode
    //V  000 = 32.768kHZ clock rate (match the watch cystal on the module) 
    
    //V RDS_EN radio data system enable
    //V 0 = disable radio data system
    
    //V NEW_METHOD use new demodulate method for improved sensitivity
    //V 0 = presumably disabled 
    
    //V SOFT_RESET
    //V 1 = perform a reset
    
    //V ENABLE power up enable: 
    //V 1 = enabled 

  /*V register 0x03 */
//  0b00000000, 
//
//  0b00000000,
////0b00010000,

   (channel_9050 >> 2), 
   ((channel_9050 & 0b11) << 6 ) | 0b00000000,
                                 //0b00010000,

  
  /*V register 0x04 */
  0b00001010, 
    //V RESERVED 15
    //V 0
    
    //V PRESUMABLY RESERVED 14
    //V 0
    
    //V RESERVED 13:12
    //V 00
    
    //V DE de-emphasis: 
    //V 1 = 50us de-emphasis as used in Australia
    
    //V RESERVED
    //V 0
    
    //V SOFTMUTE_EN
    //V 1 = soft mute enabled
    
    //V AFCD AFC disable
    //V 0 = AFC enabled
    
  0b00000000, 
    //V Bits 7-0 are not specified, so assume all 0's
    //V 0000 0000
  
  /*V register 0x05 */
  0b10001000, 
    //V INT_MODE
    //V 1 = interrupt last until read reg 0x0C
    
    //V RESERVED 14:12 
    //V 000
    
    //V SEEKTH seek signal to noise ratio threshold
    //V 1000 = suggested default 
  
  //0b00001111,
  //0b00000000, //0 v
  //0b00000001, //1 v
  //0b00000010, //2 v
  //0b00000011, //3 v
  //0b00000100, //4 v
  0b00000101, //5 v
  //0b00000110, //6 v 
  //0b00000111, //7 v
  //0b00001000, //8 v
  //0b00001001, //9 v
  //0b00001010, //10 v
  //0b00001011, //11 v
  //0b00001100, //12 v 
  //0b00001101, //13 v
  //0b00001110, //14 v
  //0b00001111, //15 v
   
    // PRESUMABLY RESERVED 7:6
    // 00
    
    // RESERVED 5:4
    // 00
    
    // VOLUME
    // 1111 = loudest volum
};



void setup()
{
 
  Serial.begin(9600);
  Serial.println("\n\nA20150415 RDA5807M FM Tuner Example\n\n");
  
  Wire.begin(); // join i2c bus (address optional for master)
  
  Serial.print("Sending boot configuration (device should reset)...");
  Wire.beginTransmission(RDA5807M_ADDRESS);
  // Write the boot configuration bytes to the RDA5807M
  Wire.write(boot_config, BOOT_CONFIG_LEN);
  Wire.endTransmission();    // stop transmitting  
  Serial.println("Done.");

  Serial.print("Tuning to channel... 93.0");
  Wire.beginTransmission(RDA5807M_ADDRESS);
  // Write the tuning configuration bytes to the RDA5807M
  Wire.write(tune_config_9300, TUNE_CONFIG_LEN);
  Wire.endTransmission();    // stop transmitting 
  Serial.println("Done.");
 
 delay(5000);
 
 // Serial.print("Tuning to channel... 90.5");
 // Wire.beginTransmission(RDA5807M_ADDRESS);
 // // Write the tuning configuration bytes to the RDA5807M
 // Wire.write(tune_config_9050, TUNE_CONFIG_LEN);
 // //Wire.write(boot_config, BOOT_CONFIG_LEN);
 // Wire.endTransmission();    // stop transmitting 
 // Serial.println("Done.");
// 
//  delay(5000);
 
 // Serial.print("Tuning to channel... 91.6");
 // Wire.beginTransmission(RDA5807M_ADDRESS);
 // // Write the tuning configuration bytes to the RDA5807M
 // Wire.write(tune_config_9160, TUNE_CONFIG_LEN);
 // //Wire.write(boot_config, BOOT_CONFIG_LEN);
 // Wire.endTransmission();    // stop transmitting 
 // Serial.println("Done.");

 //  delay(5000);
 
 // Serial.print("Tuning to channel... 80.0");
 // Wire.beginTransmission(RDA5807M_ADDRESS);
 // // Write the tuning configuration bytes to the RDA5807M
 // Wire.write(tune_config_8000, TUNE_CONFIG_LEN);
 // //Wire.write(boot_config, BOOT_CONFIG_LEN);
 // Wire.endTransmission();    // stop transmitting 
 // Serial.println("Done.");
 

  Serial.print("Sending vol configuration (device should no reset)...");
  Wire.beginTransmission(RDA5807M_ADDRESS);
  // Write the boot configuration bytes to the RDA5807M
  Wire.write(vol_config, 6 ); //BOOT_CONFIG_LEN);
  Wire.endTransmission();    // stop transmitting  
  Serial.println("Done.");


  
  
}

void loop()
{
  Serial.print(".");
  delay(1000);
}
