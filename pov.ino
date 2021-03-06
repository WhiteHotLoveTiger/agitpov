/*
 * AgitPOV
 * 12-LED POV for Arduino
 * Can display both latin and arabic letters.
 *
 * (c) 2011-2012
 
 Alan Kwok
 
 *       Alexandre Castonguay
 *       Sofian Audry
 *       Jean-Pascal Bellemare
 * (c) 2013, Changes: 7 leds to 12 and 5 to 7
 *		 Daniel Felipe Valencia
 *		 dfvalen0223@gmail.com
 * (c) 2013, Changes: added new pinout for 12 leds
 *               Andre Girard
 *               andre@andre-girard.com 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <avr/pgmspace.h>
#include <string.h>
#include <string>
#include <cwchar>


#ifndef POV_USE_ARABIC
#define POV_USE_LATIN
#endif


unsigned char povPins[]={18,19,4,5,6,7,8,9,10,11,12,13};




// Letter width. since i have 12 LED, then each letter would look like a 12 x 7 matrix
#define POV_WIDTH 7
#define POV_MAX_MESSAGE_LENGTH 26
//#define POV_US_BETWEEN_LETTERS 4500
//#define POV_US_BETWEEN_COLUMNS 3300




wchar_t povMessage[POV_MAX_MESSAGE_LENGTH];
unsigned char povMessageLength;

unsigned long povIntervalColumns = 3300;
unsigned long povIntervalLetters = 4500;
unsigned long povTimeStamp;



void povSetup() {
  
  // Set up pin 0 - 12 as OUTPUT.
  for (unsigned char k=0;k<12;k++) pinMode(povPins[k],OUTPUT);
  
  pinMode(A2,OUTPUT); // Pin as output
  digitalWrite(A2,HIGH); // Send 3V to hall sensor
  pinMode(A3,INPUT); // Pin as input
  digitalWrite(A3,HIGH); // Activate pull-up
 
  
}


// LATIN PRIVATE
#ifdef POV_USE_LATIN
prog_uint16_t PROGMEM POV_FONT [][7] =
//uint8_t LATIN_CHARS [][5] =
{   
   {0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000}, //space U+0020
   {0x000, 0x000, 0x33f, 0x37f, 0x000, 0x000, 0x000}, //!
   {0x000, 0x000, 0x003, 0x000, 0x003, 0x000, 0x000}, //"
   {0x090, 0x3fc, 0x090, 0x090, 0x090, 0x3fc, 0x090}, //#
   {0x08c, 0x112, 0x112, 0x3ff, 0x112, 0x122, 0x0c4}, //$   
   {0x100, 0x088, 0x040, 0x020, 0x010, 0x088, 0x004}, //%  //5
   {0x1ce, 0x3f9, 0x231, 0x261, 0x1f1, 0x393, 0x210}, //&
   {0x000, 0x000, 0x000, 0x003, 0x000, 0x000, 0x000}, //'
   {0x000, 0x000, 0x0fc, 0x1fe, 0x201, 0x000, 0x000}, //(
   {0x000, 0x000, 0x201, 0x1fe, 0x0fc, 0x000, 0x000}, //)
   {0x011, 0x00a, 0x004, 0x01f, 0x004, 0x00a, 0x011}, //*  //10
   {0x000, 0x020, 0x020, 0x0f8, 0x020, 0x020, 0x000}, //+
   {0x000, 0x000, 0xb00, 0x700, 0x000, 0x000, 0x000}, //,
   {0x000, 0x020, 0x020, 0x020, 0x020, 0x020, 0x000}, //-
   {0x000, 0x000, 0x300, 0x300, 0x000, 0x000, 0x000}, //.   
   {0x100, 0x080, 0x040, 0x020, 0x010, 0x008, 0x004}, ///   //15
   {0x0fc, 0x1fe, 0x303, 0x303, 0x303, 0x1fe, 0x0fc}, //0 U+0030
   {0x200, 0x204, 0x206, 0x3ff, 0x3ff, 0x200, 0x200}, //1
   {0x306, 0x383, 0x2c1, 0x261, 0x231, 0x21f, 0x20e}, //2
   {0x181, 0x311, 0x231, 0x239, 0x26d, 0x3c7, 0x183}, //3
   {0x01e, 0x01f, 0x010, 0x010, 0x3fc, 0x3fe, 0x010}, //4  //20
   {0x18f, 0x30f, 0x209, 0x209, 0x209, 0x3f9, 0x1f1}, //5
   {0x1fe, 0x3e3, 0x211, 0x211, 0x211, 0x3f3, 0x1e6}, //6
   {0x003, 0x381, 0x3c1, 0x061, 0x031, 0x01f, 0x00f}, //7
   {0x1ee, 0x3ff, 0x211, 0x211, 0x211, 0x3ff, 0x1ee}, //8
   {0x00e, 0x01f, 0x011, 0x011, 0x011, 0x3ff, 0x3ff}, //9  //25
   {0x000, 0x000, 0x30c, 0x30c, 0x000, 0x000, 0x000}, //:
   {0x000, 0x000, 0xb0c, 0x70c, 0x000, 0x000, 0x000}, //;
   {0x020, 0x070, 0x0d8, 0x18c, 0x306, 0x603, 0x401}, //<
   {0x000, 0x050, 0x050, 0x050, 0x050, 0x050, 0x000}, //=   
   {0x401, 0x603, 0x306, 0x18c, 0x0d8, 0x070, 0x020}, //>  //30
   {0x002, 0x003, 0x001, 0x361, 0x371, 0x01b, 0x00e}, //?   
   {0x0e0, 0x110, 0x248, 0x2a8, 0x2e8, 0x288, 0x070}, //@   
   {0x3f0, 0x3fc, 0x022, 0x021, 0x026, 0x3fc, 0x3f8}, //A U+0041
   {0x3ff, 0x3ff, 0x211, 0x211, 0x219, 0x3ff, 0x1ee}, //B
   {0x0fc, 0x1fe, 0x201, 0x201, 0x201, 0x102, 0x186}, //C  //35
   {0x3ff, 0x3ff, 0x201, 0x201, 0x303, 0x1fe, 0x0fc}, //D  
   {0x3ff, 0x3ff, 0x211, 0x211, 0x211, 0x211, 0x201}, //E
   {0x3ff, 0x3ff, 0x011, 0x011, 0x011, 0x011, 0x001}, //F
   {0x1fe, 0x3ff, 0x201, 0x221, 0x221, 0x3e7, 0x3e6}, //G
   {0x3ff, 0x3ff, 0x020, 0x020, 0x020, 0x3ff, 0x3ff}, //H  //40
   {0x201, 0x201, 0x3ff, 0x3ff, 0x201, 0x201, 0x000}, //I
   {0x000, 0x103, 0x301, 0x201, 0x201, 0x3ff, 0x1ff}, //J
   {0x3ff, 0x3ff, 0x030, 0x0c8, 0x18c, 0x306, 0x203}, //K
   {0x3ff, 0x3ff, 0x200, 0x200, 0x200, 0x200, 0x300}, //L
   {0x3ff, 0x3ff, 0x00e, 0x038, 0x00e, 0x3ff, 0x3ff}, //M  //45
   {0x3ff, 0x3ff, 0x004, 0x038, 0x040, 0x1ff, 0x3ff}, //N
   {0x1fc, 0x3fe, 0x301, 0x201, 0x203, 0x1ff, 0x0fe}, //O
   {0x3ff, 0x3ff, 0x021, 0x021, 0x021, 0x03f, 0x01e}, //P
   {0x1fc, 0x3fe, 0x301, 0x601, 0x603, 0x5ff, 0x4fe}, //Q
   {0x3ff, 0x3ff, 0x021, 0x0e1, 0x1a1, 0x33f, 0x21e}, //R  //50
   {0x10e, 0x31f, 0x219, 0x231, 0x261, 0x3e3, 0x1c2}, //S
   {0x000, 0x003, 0x001, 0x3ff, 0x3ff, 0x001, 0x001}, //T
   {0x1ff, 0x3ff, 0x200, 0x200, 0x200, 0x1ff, 0x3ff}, //U
   {0x00f, 0x0ff, 0x1f0, 0x300, 0x1f0, 0x0ff, 0x00f}, //V
   {0x3ff, 0x307, 0x1c0, 0x060, 0x1c0, 0x307, 0x3ff}, //W  //55
   {0x303, 0x3cf, 0x07c, 0x010, 0x078, 0x3e7, 0x303}, //X
   {0x00f, 0x03f, 0x3f0, 0x3e0, 0x03f, 0x00f, 0x000}, //Y
   {0x383, 0x3c1, 0x261, 0x231, 0x219, 0x20f, 0x307}, //Z
   {0x000, 0x000, 0x7ff, 0x7ff, 0x401, 0x401, 0x401}, //[
   {0x004, 0x008, 0x010, 0x020, 0x040, 0x080, 0x100}, //\   //60
   {0x401, 0x401, 0x401, 0x7ff, 0x7ff, 0x000, 0x000}, //]   
   {0x004, 0x006, 0x002, 0x001, 0x002, 0x006, 0x004}, //^   
   {0x200, 0x200, 0x200, 0x200, 0x200, 0x200, 0x200}, //_
   {0x000, 0x001, 0x001, 0x002, 0x002, 0x004, 0x000}, //`   
   {0x1c0, 0x3e8, 0x224, 0x224, 0x324, 0x1fc, 0x3f8}, //a U+0061  //65
   {0x1fe, 0x3ff, 0x210, 0x208, 0x208, 0x3f8, 0x1f0}, //b
   {0x0f0, 0x1f8, 0x204, 0x204, 0x204, 0x108, 0x090}, //c
   {0x1f0, 0x3f8, 0x208, 0x208, 0x208, 0x3fe, 0x3ff}, //d
   {0x0f0, 0x1f8, 0x224, 0x224, 0x224, 0x128, 0x0b0}, //e
   {0x008, 0x008, 0x3fe, 0x3ff, 0x009, 0x009, 0x000}, //f  //70
   {0x5b8, 0x9fc, 0x944, 0x944, 0x944, 0xf7c, 0x63c}, //g
   {0x3fe, 0x3ff, 0x010, 0x008, 0x008, 0x3f8, 0x3f0}, //h
   {0x000, 0x000, 0x000, 0x3e4, 0x3f4, 0x000, 0x000}, //i
   {0x400, 0x800, 0x800, 0xfe4, 0x7f4, 0x000, 0x000}, //j
   {0x3fe, 0x3ff, 0x060, 0x0f0, 0x198, 0x30c, 0x204}, //k  //75
   {0x000, 0x000, 0x000, 0x3fe, 0x3ff, 0x000, 0x000}, //l
   {0x3fc, 0x3f8, 0x004, 0x3f8, 0x004, 0x3fc, 0x3f8}, //m
   {0x3fc, 0x3f8, 0x004, 0x004, 0x004, 0x3fc, 0x3f8}, //n
   {0x1f0, 0x3f8, 0x304, 0x204, 0x20c, 0x1fc, 0x0f8}, //o
   {0xffc, 0xff8, 0x104, 0x104, 0x104, 0x1fc, 0x0f8}, //p  //80
   {0x0f8, 0x1fc, 0x104, 0x104, 0x084, 0xffc, 0xffc}, //q
   {0x3fc, 0x3f8, 0x004, 0x004, 0x004, 0x00c, 0x008}, //r
   {0x118, 0x23c, 0x224, 0x264, 0x244, 0x3c4, 0x188}, //s
   {0x000, 0x004, 0x004, 0x3fe, 0x3ff, 0x004, 0x004}, //t
   {0x1fc, 0x3fc, 0x200, 0x200, 0x200, 0x1fc, 0x3fc}, //u  //85
   {0x0fc, 0x1fc, 0x300, 0x300, 0x300, 0x1fc, 0x0fc}, //v
   {0x1fc, 0x1fc, 0x200, 0x1fc, 0x200, 0x3fc, 0x1fc}, //w
   {0x30c, 0x39c, 0x070, 0x020, 0x060, 0x3dc, 0x30c}, //x
   {0x83c, 0xc7c, 0x6c0, 0x380, 0x180, 0x0fc, 0x07c}, //y
   {0x30c, 0x384, 0x2c4, 0x264, 0x234, 0x21c, 0x20c}, //z  //90
   {0x020, 0x020, 0x1fc, 0x3de, 0x603, 0x401, 0x401}, //{
   {0x000, 0x000, 0x000, 0xfff, 0x000, 0x000, 0x000}, //|     
   {0x401, 0x401, 0x603, 0x3de, 0x1fc, 0x020, 0x020}, //}
   {0x060, 0x038, 0x018, 0x070, 0x0c0, 0x0e0, 0x030}, //~ U+007E

   {0x000, 0x000, 0xfec, 0xfcc, 0x000, 0x000, 0x000}, //¡ U+00A1  //95
   {0x0e0, 0x110, 0x110, 0x3f8, 0x110, 0x110, 0x000}, //¢
   {0x210, 0x1fe, 0x2ff, 0x211, 0x211, 0x211, 0x102}, //£
   {0x104, 0x0a8, 0x050, 0x050, 0x050, 0x0a8, 0x104}, //¤
   {0x000, 0x057, 0x05f, 0x3f8, 0x3f8, 0x05f, 0x057}, //¥
   {0x000, 0x000, 0x000, 0xf9f, 0x000, 0x000, 0x000}, //¦  //100
   {0x200, 0x464, 0x49a, 0x492, 0x592, 0x262, 0x004}, //§
   {0x000, 0x000, 0x002, 0x000, 0x000, 0x002, 0x000}, //¨
   {0x1f0, 0x208, 0x2f4, 0x294, 0x294, 0x104, 0x0f8}, //© U+00A9
   
   {0x020, 0x050, 0x088, 0x124, 0x050, 0x088, 0x104}, //« U+00AB

   {0x000, 0x004, 0x002, 0x002, 0x001, 0x001, 0x000}, //´ U+00B4  //105
   
   {0x104, 0x088, 0x050, 0x124, 0x088, 0x050, 0x020}, //» U+00BB  
   
   {0x700, 0xd80, 0x8ec, 0x86c, 0x800, 0xc00, 0x400}, //¿ U+00BF
   {0x3e0, 0x3f1, 0x049, 0x04a, 0x04a, 0x3f4, 0x3e0}, //À
   {0x3e0, 0x3f4, 0x04a, 0x04a, 0x049, 0x3f1, 0x3e0}, //Á
   {0x3e0, 0x3f4, 0x04a, 0x049, 0x04a, 0x3f4, 0x3e0}, //Â  //110
   {0x3e0, 0x3f2, 0x049, 0x049, 0x04a, 0x3f2, 0x3e1}, //Ã
   {0x3e0, 0x3f0, 0x04a, 0x048, 0x048, 0x3f2, 0x3e0}, //Ä
   {0x3e0, 0x3f0, 0x04a, 0x04d, 0x04d, 0x3f2, 0x3e0}, //Å
   {0x3f8, 0x026, 0x021, 0x3ff, 0x221, 0x221, 0x221}, //Æ
   {0x0fc, 0x1fe, 0x201, 0xa01, 0xe01, 0x102, 0x186}, //Ç  //115
   {0x3f8, 0x249, 0x249, 0x24a, 0x24a, 0x24c, 0x208}, //È
   {0x3f8, 0x24c, 0x24a, 0x24a, 0x249, 0x249, 0x208}, //É
   {0x3f8, 0x24c, 0x24a, 0x249, 0x24a, 0x24c, 0x208}, //Ê
   {0x3f8, 0x248, 0x24a, 0x248, 0x248, 0x24a, 0x208}, //Ë
   {0x208, 0x209, 0x3f9, 0x3fa, 0x20a, 0x20c, 0x000}, //Ì  //120
   {0x208, 0x20c, 0x3fa, 0x3fa, 0x209, 0x209, 0x000}, //Í
   {0x208, 0x20c, 0x3fa, 0x3f9, 0x20a, 0x20c, 0x000}, //Î
   {0x208, 0x20a, 0x3f8, 0x3f8, 0x20a, 0x208, 0x000}, //Ï
   {0x020, 0x3ff, 0x3ff, 0x221, 0x303, 0x1fe, 0x0fc}, //Ð
   {0x3f8, 0x3f2, 0x021, 0x041, 0x082, 0x1fa, 0x3f9}, //Ñ  //125
   {0x1f0, 0x3f8, 0x305, 0x205, 0x20e, 0x1fc, 0x0f8}, //Ò
   {0x1f0, 0x3f8, 0x306, 0x205, 0x20d, 0x1fc, 0x0f8}, //Ó
   {0x1f0, 0x3f8, 0x306, 0x205, 0x20e, 0x1fc, 0x0f8}, //Ô
   {0x1f0, 0x3fa, 0x305, 0x205, 0x20e, 0x1fe, 0x0f9}, //Õ
   {0x1f0, 0x3f8, 0x305, 0x204, 0x20c, 0x1fd, 0x0f8}, //Ö  //130
   {0x104, 0x088, 0x050, 0x020, 0x050, 0x088, 0x104}, //×
   {0x2f8, 0x104, 0x1c2, 0x132, 0x10e, 0x082, 0x07d}, //Ø
   {0x1f8, 0x3f9, 0x201, 0x202, 0x202, 0x3fc, 0x1f8}, //Ù
   {0x1f8, 0x3fc, 0x202, 0x202, 0x201, 0x3f9, 0x1f8}, //Ú
   {0x1f8, 0x3fc, 0x202, 0x201, 0x202, 0x3fc, 0x1f8}, //Û  //135
   {0x1f8, 0x3f8, 0x202, 0x200, 0x200, 0x3fa, 0x1f8}, //Ü
   {0x038, 0x07c, 0x3c2, 0x3c2, 0x079, 0x039, 0x000}, //Ý U+00DD 
   
   {0x180, 0x3c1, 0x251, 0x252, 0x352, 0x1f4, 0x3e0}, //à U+00E0
   {0x180, 0x3c4, 0x252, 0x252, 0x351, 0x1f1, 0x3e0}, //á
   {0x180, 0x3c4, 0x252, 0x251, 0x352, 0x1f4, 0x3e0}, //â  //140
   {0x180, 0x3c4, 0x252, 0x252, 0x354, 0x1f4, 0x3e2}, //ã
   {0x180, 0x3c0, 0x254, 0x250, 0x350, 0x1f4, 0x3e0}, //ä
   {0x180, 0x3c0, 0x252, 0x255, 0x355, 0x1f2, 0x3e0}, //å
   {0x1c8, 0x224, 0x224, 0x3fc, 0x224, 0x228, 0x1b0}, //æ
   {0x0f0, 0x1f8, 0x204, 0xa04, 0xe04, 0x108, 0x090}, //ç  //145
   {0x0e0, 0x1f1, 0x249, 0x24a, 0x24a, 0x254, 0x160}, //è
   {0x0e0, 0x1f4, 0x24a, 0x24a, 0x249, 0x251, 0x160}, //é
   {0x0e0, 0x1f4, 0x24a, 0x249, 0x24a, 0x254, 0x160}, //ê
   {0x0e0, 0x1f0, 0x24a, 0x248, 0x248, 0x252, 0x160}, //ë
   {0x000, 0x000, 0x002, 0x3f2, 0x3f4, 0x004, 0x008}, //ì  //150
   {0x000, 0x008, 0x004, 0x3f4, 0x3f2, 0x002, 0x000}, //í
   {0x000, 0x008, 0x004, 0x3f2, 0x3f4, 0x008, 0x000}, //î
   {0x000, 0x000, 0x004, 0x3f0, 0x3f0, 0x004, 0x000}, //ï U+00EF
   
   {0x3f8, 0x3f2, 0x009, 0x009, 0x00a, 0x3f2, 0x3f1}, //ñ U+00F1
   {0x1e0, 0x3f1, 0x309, 0x20a, 0x21a, 0x1fc, 0x0f0}, //ò  //155
   {0x1e0, 0x3f4, 0x30a, 0x20a, 0x219, 0x1f9, 0x0f0}, //ó
   {0x1e0, 0x3f4, 0x30a, 0x209, 0x21a, 0x1fc, 0x0f0}, //ô
   {0x1e0, 0x3f2, 0x309, 0x209, 0x21a, 0x1fa, 0x0f1}, //õ
   {0x1e0, 0x3f0, 0x30a, 0x208, 0x218, 0x1fa, 0x0f0}, //ö
   {0x000, 0x020, 0x020, 0x0a8, 0x020, 0x020, 0x000}, //÷  //160
   {0x2f0, 0x108, 0x1c4, 0x134, 0x10c, 0x084, 0x07a}, //ø
   {0x1f0, 0x3f1, 0x201, 0x202, 0x202, 0x1f4, 0x3f0}, //ù
   {0x1f0, 0x3f4, 0x202, 0x202, 0x201, 0x1f1, 0x3f0}, //ú
   {0x1f0, 0x3f4, 0x202, 0x201, 0x202, 0x1f4, 0x3f0}, //û
   {0x1f0, 0x3f0, 0x204, 0x200, 0x200, 0x1f4, 0x3f0}, //ü  //165
   {0x838, 0xc78, 0x6c4, 0x382, 0x182, 0x0f9, 0x079}, //ý U+00FD
   
   {0x838, 0xc78, 0x6c2, 0x380, 0x180, 0x0fa, 0x078}, //ÿ U+00FF
   
   {0x028, 0x1fe, 0x3ff, 0x229, 0x229, 0x229, 0x102}, //€ U+20AC euro   
   {0x300, 0x190, 0x090, 0x100, 0x210, 0x310, 0x180}, //⍨ U+2368 confused
   {0x684, 0x95c, 0x827, 0x807, 0x827, 0x95c, 0x684}, //☃ U+2603 snowman    //170
   {0xc60, 0xc66, 0x29f, 0x11f, 0x29f, 0xc66, 0xc60}, //☠ U+2620 scull & cross bones
   {0x1f8, 0x284, 0x452, 0x442, 0x452, 0x284, 0x1f8}, //☹ U+2639 frown 
   {0x0f8, 0x144, 0x292, 0x282, 0x292, 0x144, 0x0f8}, //☺ U+263A smiley
   {0x0f8, 0x1bc, 0x36e, 0x37e, 0x36e, 0x1bc, 0x0f8}, //☻ U+263B inverted smiley
   {0x01c, 0x062, 0x186, 0x208, 0x186, 0x062, 0x01c}, //♡ U+2661 heart           //175
   {0x01c, 0x07e, 0x1fe, 0x3f8, 0x1fe, 0x07e, 0x01c}, //♥ U+2665 filled heart
   {0x16a, 0x26a, 0x20a, 0x27a, 0x20a, 0x26a, 0x16a}  //〠 U+3020 Japanese postal mark
};

void povSetMessage( const wchar_t* str, int pml) {
  
  povMessageLength = pml; //std::wcslen (str);  //strlen(str);
  povMessageLength = min(povMessageLength, POV_MAX_MESSAGE_LENGTH);
  
  for (int i=0; i<povMessageLength; i++) {
    wchar_t c = str[i];
    wchar_t a;
    if (0x0020 <= c && c <= 0x007E)
      a = c - 0x0020;
    else if (0x00A1 <= c && c <= 0x00A9)
      a = c - 66; // (0xA1 - 95)
    else if (0x00AB == c)
      a = c - 67; // (0xAB - 104)
    else if (0x00B4 == c)
      a = c - 75; // (0xB4 - 105)
    else if (0x00BB == c)
      a = c - 81; // (0xBB - 106)
    else if (0x00BF <= c && c <= 0x00DD)
      a = c - 84; // (0xBF - 107)
    else if (0x00E0 <= c && c <= 0x00EF)
      a = c - 86; // (0xE0 - 138)
    else if (0x00F1 <= c && c <= 0x00FD)
      a = c - 87; // (0xF1 - 154)
    else if (0x00FF == c)
      a = c - 88; // (0xFF - 167)
    else {
      switch (c) {
      case L'€': 
        a = 168; 
        break;
      case L'⍨': 
        a = 169; 
        break;
      case L'☃': 
        a = 170; 
        break;
      case L'☠': 
        a = 171; 
        break;
      case L'☹': 
        a = 172; 
        break;
      case L'☺': 
        a = 173; 
        break;
      case L'☻': 
        a = 174; 
        break;
      case L'♡': 
        a = 175; 
        break;
      case L'♥': 
        a = 176; 
        break;
      case L'〠': 
        a = 177; 
        break;	
      default : 
        a = 0; 
        break;
      }
    }
    povMessage[i] = a;
    //Serial.print("c: ");
    //Serial.println(c);    
    //Serial.print("a: ");
    //Serial.println(a);
  }
}



#endif

// povDisplayRaw(LATIN_CHARS, alphaMsg, strlen(msg), 4500, 3300);

//method of displaying the letters. displaying each column (one hex number) of the letter one by one through PORTD. then after a letter, it puts a space in between and move onto the next letter. until however long of the String you wish to display.
//each letter is first turned into ASCII integer, then minus 65. It would then be the row of the letter from the library above. (ex. A is 65 in ASCII, but row 0 in my library)
// furthermore, because of the way i made the hardware, it displays each letter starting from the back of the sentence, and each letter from the right most column.
void povDisplay() { 
  
  
  for (int i = povMessageLength-1; i>=0; i--)
  {
    for (int j = (POV_WIDTH-1); j>=0; j--)
    {
      unsigned int b = pgm_read_word_near(&POV_FONT[povMessage[i]][j]);//message[(int)POVstring[i]-0][j];
//      byte b = fontSet[message[i]][j];
      for (int k=0; k<12; k++) digitalWrite(povPins[k], bitRead(~b,k));
        delayMicroseconds(povIntervalColumns); //delayMicroseconds(POV_US_BETWEEN_COLUMNS);
    }
    for (int k=0; k<12; k++) digitalWrite(povPins[k], 1);
      delayMicroseconds(povIntervalLetters); //delayMicroseconds(POV_US_BETWEEN_LETTERS);
      
     
  }
}

void povUpdateInterval() {
  
  unsigned long interval = max((millis() - povTimeStamp),2)-1;
   povTimeStamp = millis() ;
   povIntervalColumns = interval  * 30 / 10 ; //povIntervalColumns = interval / 10 * 33 ;
   povIntervalLetters = interval  * 43 / 10; //povIntervalLetters = interval / 10 * 45;
   
   
   povIntervalColumns = min(povIntervalColumns,1650);
   povIntervalLetters = min(povIntervalLetters,2250);
  
}

void povWaitAndDisplay() {

 // Wait for hall then display
 while ( digitalRead(A3) == HIGH );
 
 
   povUpdateInterval();
   
   povDisplay();
  
}




