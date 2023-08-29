/*
 * GD330.h - Library for Audio_trigger board from Proyectil Mx (GD3300 chip)
 * Created by Salvador Rueda Pau, July 23, 2016.
 * Modified by Misael Reyes
 * 
 */

#ifndef GD3300_h
#define GD3300_h

#include <Arduino.h>

#define CMD_NEXT             0X01 // Play next file
#define CMD_PREV             0X02 // Play previous file
#define CMD_PLAYN            0X03 // Play n file from (1-255)
#define CMD_VOL_UP           0X04 // Volumen up
#define CMD_VOL_DOWN         0X05 // Volumen down
#define CMD_SET_VOL          0X06 // Set Volume (0-30)
                          
#define CMD_PLAY_SLOOP       0X08 // Single loop play.

#define CMD_SEL_DEV          0X09 // Set device. 2 DEV_TF (SD Card)
#define CMD_SLEEP_MODE       0X0A
#define CMD_WAKE_UP          0X0B
#define CMD_RESET            0X0C

#define CMD_PLAY             0X0D // Play
#define CMD_PAUSE            0X0E // Pause

#define CMD_PLAY_F_FILE      0X0F // only avaible for Flash not SD Card.

#define CMD_STOP_PLAY        0X16 // Stop
#define CMD_FOLDER_CYCLE     0X17 // Play all the files in the f folder

#define CMD_PLAY_SHUFFLE     0x18 //

#define CMD_SET_SPLAY        0x19 // Start up single cycle play [00] close single cycle play [01]

#define CMD_PLAY_W_VOL       0X22 // Play with a volumen

// Query Commands
#define CMD_PLAYING_N         0x4C // qPlaying();  // Ask for the file is playing
#define CMD_QUERY_STATUS      0x42 // qStatus();   // Ask for the status
#define CMD_QUERY_VOLUME      0x43 // qVol();      // Ask for the volumen. // Sometime board fails
#define CMD_QUERY_FLDR_TRACKS 0x4E // qFTracks();  // Ask for the number of tracks folders
#define CMD_QUERY_TOT_TRACKS  0x48 // qTTracks();  // Ask for the total of tracks
#define CMD_QUERY_FLDR_COUNT  0x4F // qTFolders(); // Ask for the number of folders


/************ Opitons **************************/

class GD3300{

   public:
    

     void sendCommand(byte command);
     void sendCommand(byte command, byte dat2);
     void sendCommand(byte command, byte dat1, byte dat2);

     void begin(Stream&);
     
	 uint16_t MP3Answer();

     void playNext();             // Play next file
     void playPrevious();         // Play previous file
     void play();                 // Play
     void play(byte n);           // Play n file
     void play(byte n, byte vol); // Play n file with volume

     void volUp();            // Volumen Up
     void volDown();          // Volumen Down
     void setVol(byte v);     // Set Volumen (0-30)

     void playSL(byte n);         // Play Single loop  n file.
     void playSL(byte f, byte n); // Play Single loop play n file by name from f folder.
     void playL(bool on);


     void pause();                // Pause

     void playF(byte f);          // Play all files in the f folder
     void playF(byte f,byte n);

     void stop();                 // Stop playing

     // Query commands
     void qPlaying();   // Ask for the file is playing
     void qStatus();    // Ask for the status
     void qVol();       // Ask for the volumen
     void qFTracks();   // Ask for the number of tracks folders
     void qTTracks();   // Ask for the total of tracks
     void qTFolders();  // Ask for the number of folders

     void sleep();     // Send sleep command
     void wakeup();    // Send wake up command
     void reset();     // Send reset command

     void showDebug(bool on); // true to show debug messages
    
   private:
     Stream* serial;
      String sanswer();
     int shex2int(char *s, int n);
     String sbyte2hex(byte b);
     bool _showDebugMessages;
     byte ansbuf[15] = {0}; // Buffer for the answers.

};


#endif
