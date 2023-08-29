/*
 * SerialMP3Player.h - Library for Serial MP3 Player board from Catalex (GD3300 chip)
 * Created by Salvador Rueda Pau, July 23, 2016.
 * License as GPL v3 (http://www.gnu.org/licenses/gpl.html)
 *
 */

#include "Arduino.h"
#include "GD3300.h"


void GD3300::showDebug(bool op){
  // showDebug (op) 0:OFF 1:ON
    _showDebugMessages = op;
}


void GD3300::begin(Stream &s){
  serial = &s;
}


void GD3300::playNext(){
  sendCommand(CMD_NEXT);
}

void GD3300::playPrevious(){
  sendCommand(CMD_PREV);
}

void GD3300::volUp(){
   sendCommand(CMD_VOL_UP);
}

void GD3300::volDown(){
   sendCommand(CMD_VOL_DOWN);
}

void GD3300::setVol(byte v){
   // Set volumen (0-30)
   sendCommand(CMD_SET_VOL, v);
}

void GD3300::playSL(byte n){
   // Play single loop n file
   sendCommand(CMD_PLAY_SLOOP, n);
}

void GD3300::playSL(byte f, byte n){
   // Single loop play n file from f folder
   sendCommand(CMD_PLAY_SLOOP, f, n);
}
void GD3300::playL(bool on){
   // Single loop play n file from f folder
   if(on)sendCommand(CMD_SET_SPLAY, 0, 0);
   else sendCommand(CMD_SET_SPLAY, 0, 1);
}

void GD3300::play(){
   sendCommand(CMD_PLAY);
}

void GD3300::pause(){
   sendCommand(CMD_PAUSE);
}

void GD3300::play(byte n){
   // n number of the file that must be played.
   // n possible values (1-255)
   sendCommand(CMD_PLAYN, n);
}

void GD3300::play(byte n, byte vol){
   // n number of the file that must be played

   sendCommand(CMD_PLAY_W_VOL, vol, n);
}

void GD3300::playF(byte f){
   // Play all files in the f folder

   sendCommand(CMD_FOLDER_CYCLE, f, 0);
}
void GD3300::playF(byte f,byte n){
   // Play all files in the f folder

   sendCommand(CMD_PLAY_F_FILE, f, n);
}

void GD3300::stop(){
   sendCommand(CMD_STOP_PLAY);
}

void GD3300::qPlaying(){
  // Ask for the file is playing
   sendCommand(CMD_PLAYING_N);
}

void GD3300::qStatus(){
   // Ask for the status.
   sendCommand(CMD_QUERY_STATUS);
}

void GD3300::qVol(){
  // Ask for the volumen
   sendCommand(CMD_QUERY_VOLUME);
}

void GD3300::qFTracks(){    // !!! Nonsense answer
  // Ask for the number of tracks folders
   sendCommand(CMD_QUERY_FLDR_TRACKS);
}

void GD3300::qTTracks(){
  // Ask for the total of tracks
   sendCommand(CMD_QUERY_TOT_TRACKS);
}

void GD3300::qTFolders(){
  // Ask for the number of folders
   sendCommand(CMD_QUERY_FLDR_COUNT);
}

void GD3300::sleep(){
  // Send sleep command
  sendCommand(CMD_SLEEP_MODE);
}

void GD3300::wakeup(){
  // Send wake up command
  sendCommand(CMD_WAKE_UP);
}

void GD3300::reset(){
  // Send reset command
  sendCommand(CMD_RESET);
}



void GD3300::sendCommand(byte command){
  sendCommand(command, 0, 0);
}

void GD3300::sendCommand(byte command, byte dat2){
  sendCommand(command, 0, dat2);
}


void GD3300::sendCommand(byte command, byte dat1, byte dat2){
  byte Send_buf[8] = {0}; // Buffer for Send commands.
  String mp3send = "";

  // Command Structure 0x7E 0xFF 0x06 CMD FBACK DAT1 DAT2 0xEF

  //#ifndef NO_SERIALMP3_DELAY
  delay(20);
  //#endif

  Send_buf[0] = 0x7E;    // Start byte
  Send_buf[1] = 0xFF;    // Version
  Send_buf[2] = 0x06;    // Command length not including Start and End byte.
  Send_buf[3] = command; // Command
  Send_buf[4] = 0x01;    // Feedback 0x00 NO, 0x01 YES
  Send_buf[5] = dat1;    // DATA1 datah
  Send_buf[6] = dat2;    // DATA2 datal
  Send_buf[7] = 0xEF;    // End byte

  for(int i=0; i<8; i++)
  {
    serial->write(Send_buf[i]) ;
    mp3send+=sbyte2hex(Send_buf[i]);
  }
  if (_showDebugMessages){
     Serial.print("Sending: ");
     Serial.println(mp3send); // watch what are we sending
  }

 // #ifndef NO_SERIALMP3_DELAY
  delay(1000);
  // Wait between sending commands.
 // #endif
}

//String sanswer(void);
//int iansbuf = 0;                 // Index for answer buffer.
//static uint8_t ansbuf[10] = {0}; // Buffer for the answers.
uint8_t val;



/******************************************************************************/
/*funcion :MP3Answer() devuelve el bit correspondiente a la solicitud de estatus*/
/*/* Valores que devuelve MP3Answer(); 
  * 0x0001 -> memory card insert
  * 0x0002 -> Complete play
  * 0x0003 -> Error
  * 0x0004 -> Data recive correct
  *  -> status
  *  - 0x000A-> stopped
  *  - 0x000B-> playing  
  *  - 0x000C-> pause  
  * 0x0005 -> Vol playing
  * 0x0006 -> File count
  * 0x0007 -> playing
  * 0x0008 -> folder file  count
  * 0x0009 -> folder count
  * 
  */
  /* a15 a14 a13 a12 a11 a10 a9 a8 a7 a6 a5 a4 a3 a2 a1 a0
  */
/* Misael Reyes */
uint16_t GD3300::MP3Answer(){
 // Response Structure  0x7E 0xFF 0x06 RSP 0x00 0x00 DAT 0xFE 0xBA 0xEF
  //
  // RSP Response code
  // DAT Response additional data
  
  uint16_t decodedMP3Answer = 0x0000;
  
  String answer = sanswer();

   

     switch (ansbuf[3])
     {
    case 0x3A:
      decodedMP3Answer = 0x0001;//|= (1<<0);//+= " -> Memory card inserted.";
      break;
   case 0x3B:
      decodedMP3Answer = 0x0011;//|= (1<<0);//+= " -> Memory card inserted.";
      break;

    case 0x3D:
      decodedMP3Answer = 0x0002;/// |= (1<<1); //+= " -> Completed play num " + String(ansbuf[6], DEC);
      break;

    case 0x40:
      decodedMP3Answer = 0x0003;//|= (1<<2); //+= " -> Error";
      break;

    case 0x41: 
	   
       decodedMP3Answer = 0x0004;// |= (1<<3); //+= " -> Data recived correctly. ";
	  
      break;

    case 0x42:
	     
      switch(ansbuf[6]){
        case 0: decodedMP3Answer = 0x0A;break;//|= (1<<4) | (1<<12); break;//" -> Status: stopped"; break;
        case 1: decodedMP3Answer = 0x0B;break;//|= (1<<4) | (1<<11); break;//" -> Status: playing"; break;
        case 2: decodedMP3Answer = 0x0C;break;//|= (1<<4) | (1<<10); break;// " -> Status: paused"; break;
      }
      break;

    case 0x43:
       //+= " -> Vol playing: " + String(ansbuf[6], DEC);

    decodedMP3Answer = ansbuf[6];
	  
      break;

    case 0x48://" -> File count: "
    decodedMP3Answer = ansbuf[6];
   
      break;


    case 0x4C://" -> Playing: "
    decodedMP3Answer = ansbuf[6];
      break;

    case 0x4E: //" -> Folder file count: "
    decodedMP3Answer = ansbuf[6];
   
      break;

    case 0x4F://" -> Folder count: "
    decodedMP3Answer = ansbuf[6];
	
      break;
     }


   ansbuf[3]= 0; // Clear ansbuff.
   //ansbuf[6] = 0; // Clear ansbuff.
   return decodedMP3Answer;
}



//=================================================================================
/********************************************************************************/
/*Function: sbyte2hex. Returns a byte data in HEX format.	                */
/*Parameter:- uint8_t b. Byte to convert to HEX.                                */
/*Return: String                                                                */


String GD3300::sbyte2hex(byte b)
{
  String shex;

  //Serial.print("0x");
  shex="0X";

  //if (b < 16) Serial.print("0");
  if (b < 16) shex+="0";
  //Serial.print(b, HEX);
  shex+=String(b,HEX);
  //Serial.print(" ");
  shex+=" ";
  return shex;
}


/********************************************************************************/
/*Function: sanswer. Returns a String answer from mp3 UART module.	            */
/*Return: String.  the answer                                                   */


String GD3300::sanswer(void){
  // Response Structure  0x7E 0xFF 0x06 RSP 0x00 0x00 DAT 0xFE 0xBA 0xEF
  //
  // RSP Response code
  // DAT Response additional data

  // if there are something available start to read from mp3 serial.
  // if there are "0x7E" it's a beginning.
  //
  //  read while something readed and it's not the end "0xEF"


  byte b;
  String mp3answer="";                // Answer from the Serial3.
  int iansbuf = 0;

  while (serial->available() ){//&& b!=0xef
   //do{
    b = serial->read();

    if(b == 0x7E){  // if there are "0x7E" it's a beginning.
      iansbuf=0;    //  ansbuf index to zero.
      mp3answer="";
	  
    }

    ansbuf[iansbuf] = b;
    mp3answer+=sbyte2hex(ansbuf[iansbuf]);
    iansbuf++; //  increase this index.
    
	if(b==0x3A){
	    ansbuf[3] = 0x3a;
	}
	if(b==0x3b){
	    ansbuf[3] = 0x3b;
	}
	if(b==0x3D){
	    ansbuf[3] = 0x3d;
		
	}
	if(b==0xef){
	   // Serial.print("ansbuf[3]:0x");
       // Serial.println(ansbuf[3],HEX);
	    //Serial.print("ansbuf[6]:0x");
	    //Serial.println(ansbuf[6],HEX);
		
	}
   //}while(b != 0xEF);
   // while there are something to read and it's not the end "0xEF"

  }
  
       
  
  return mp3answer;
 }
