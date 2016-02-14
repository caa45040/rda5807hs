//*****************************************************
// 2015/02/13
// arunino uno
// RDA5807,RDA5807HS
// by akiba keniti
//*****************************************************

#include <Wire.h>

// Select the frequency we want to tune to by way
// of selecting the channel for the desired frequency
//uint16_t channel = 209;
uint16_t channel_9300 = 60;
uint16_t channel_9160 = 46;
uint16_t channel_9050 = 35;
uint16_t channel_8000 = 40;  //80-76=4
uint16_t channel_8700 = 0;
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
uint8_t boot_config[8] = {
  /* register 0x02 */
  0b11000000,  //c0
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
    
    //0b00000011, //03
    
    0b00000001, //1
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

  //0b00000000,
  0b00001000,
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
  //0b00001010,
  
    0b00000000, 
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
    
//  0b00000000,
    0b01000000, 

    // Bits 7-0 are not specified, so assume all 0's
    // 0000 0000
  
  /* register 0x05 */
  0b10001000, //88
    // INT_MODE
    // 1 = interrupt last until read reg 0x0C
    
    // RESERVED 14:12 
    // 000
    
    // SEEKTH seek signal to noise ratio threshold
    // 1000 = suggested default 
  
  //0b00001111,
  
   0b101000001  //a8
  
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

// After reset, we can tune the device
// We only need program the first 4 bytes in order to do this
uint8_t tune_config_8700[] = {
   0b11000000,   
   0b00000001, 
   (channel_8700 >> 2),              //  BB
   ((channel_8700 & 0b11) << 6 ) | 0b00011000
};

void setup()
{
  Serial.begin(9600);
  Serial.println("\n\nA20150415 RDA5807M FM Tuner Example\n\n");
  
  Wire.begin(); // join i2c bus (address optional for master)
}

 int volq = 5;
 int tunq = 0;
//int mod = 1,vol
//int mod = 2,tun
int vol_tun(int mod)
{
  int va;
  int ret = 0;

   
 if(mod == 1) ret = volq;
 if(mod == 2) ret = tunq;
  
  if(mod == 1 ) {
      va = analogRead(A0);
  } else {
      va = analogRead(A1);
  }

if (  (va % 32) != 0  ) { ret = va / 32; }
 
 
 if(mod == 1) {
   ret = ret / 2;
   volq = ret;   
  } else {
    // mod == 2
    tunq = ret;
  }

  return(ret);
} //ret


void set_vol_tun(int vol,int tun)
{
int ss = 0;

  //リセット
  Serial.print("Sending boot configuration (device should reset)...");
  Wire.beginTransmission(RDA5807M_ADDRESS);
  // Write the boot configuration bytes to the RDA5807M
  boot_config[0] = 0;
  boot_config[1] = 0x02; //リセット
  Wire.write(boot_config,  8);    //BOOT_CONFIG_LEN);
  Wire.endTransmission();    // stop transmitting  
  Serial.println("Done.");

delay(500); 

  //初期値の設定
  Serial.print("Sending boot configuration (device should reset)...");
  Wire.beginTransmission(RDA5807M_ADDRESS);
  // Write the boot configuration bytes to the RDA5807M
  boot_config[0] = 0xd2;
  boot_config[1] = 0x01; //初期値の設定
  boot_config[7] = 0xa0 + vol;
  Wire.write(boot_config,  8);    //BOOT_CONFIG_LEN);
  Wire.endTransmission();    // stop transmitting  
  Serial.println("Done.");

delay(500); 

if( tun == 14){
  //周波数の設定
  Serial.print("Tuning to channel... 93.0");
  Wire.beginTransmission(RDA5807M_ADDRESS);
  // Write the tuning configuration bytes to the RDA5807M
  Wire.write(tune_config_9300, TUNE_CONFIG_LEN);
  Wire.endTransmission();    // stop transmitting 
  Serial.println("Done.");
  ss = 1;
}
 
if(tun == 15) {
  Serial.print("Tuning to channel... 90.5");
  Wire.beginTransmission(RDA5807M_ADDRESS);
  // Write the tuning configuration bytes to the RDA5807M
  Wire.write(tune_config_9050, TUNE_CONFIG_LEN);
  Wire.endTransmission();    // stop transmitting 
  Serial.println("Done.");
  ss = 1;
}

if(tun == 16){ 
  Serial.print("Tuning to channel... 91.6");
  Wire.beginTransmission(RDA5807M_ADDRESS);
  // Write the tuning configuration bytes to the RDA5807M
  Wire.write(tune_config_9160, TUNE_CONFIG_LEN);
  //Wire.write(boot_config, BOOT_CONFIG_LEN);
  Wire.endTransmission();    // stop transmitting 
  Serial.println("Done.");
  ss = 1;
}

if( tun == 17){
  Serial.print("Tuning to channel... 80.0");
  Wire.beginTransmission(RDA5807M_ADDRESS);
  // Write the tuning configuration bytes to the RDA5807M
  Wire.write(tune_config_8000, TUNE_CONFIG_LEN);
  //Wire.write(boot_config, BOOT_CONFIG_LEN);
  Wire.endTransmission();    // stop transmitting 
  Serial.println("Done.");
}

if( ss == 0){
  Serial.print("Tuning to channel... 87.0");
  Wire.beginTransmission(RDA5807M_ADDRESS);
  // Write the tuning configuration bytes to the RDA5807M
  Wire.write(tune_config_8700, TUNE_CONFIG_LEN);
  //Wire.write(boot_config, BOOT_CONFIG_LEN);
  Wire.endTransmission();    // stop transmitting 
  Serial.println("Done.");
}

delay(500); 
}

int vol = 0;
int volbk = 1;
int tun = 0;
int tunbk = 1;
 
void loop() { 
 
 vol = vol_tun(1); 
 if( vol != volbk){
    Serial.print("vol=");
    Serial.println(vol);
    volbk = vol;
    set_vol_tun(vol,tun);
 } 
  
 tun = vol_tun(2);  
 if( tun != tunbk){
    Serial.print("tun=");
    Serial.println(tun);
    tunbk = tun;
    set_vol_tun(vol,tun);
 } 
 
   delay(2000); 
} //end loop
