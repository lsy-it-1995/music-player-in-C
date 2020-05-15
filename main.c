#include "avr.h"
#include "lcd.h"
#include <stdio.h>
#include <math.h>
#define SPK_ON SET_BIT(PORTA,0)
#define SPK_OFF CLR_BIT(PORTA,0)

#define W 8		// W = Whole
#define H 4
#define Q 2
#define E 1

#define h_A		440
#define h_As	466.16
#define h_B		493.88
#define h_C		523.25
#define h_Cs	554.37
#define h_D		587.33
#define h_E		659.26
#define h_F		698.46
#define h_Fs	739.99
#define h_G		783.99
#define h_Gs	830.61

#define l_A		220
#define l_As	233.08
#define l_B		246.94
#define l_C		261.63
#define l_Cs	277.18
#define l_D		293.66
#define l_E		329.63
#define l_F		349.23
#define l_Fs	369.99
#define l_G		392
#define l_Gs	415.3


typedef enum {true, false} bool;

float wait_value = 0.00005;

struct note{
	float freq, duration;	
};

#define numNote1 sizeof(SONG1) / sizeof(SONG1[0])
const struct note SONG1[]= {
	{l_C, Q}, {l_C, Q}, {l_G, Q}, {l_G, Q}, {h_A, Q}, {h_A, Q}, {l_G, H}, {l_F, Q},
	{l_F, Q}, {l_E, Q}, {l_E, Q}, {l_D, Q}, {l_D, Q}, {l_C, H}, {l_G, Q}, {l_G, Q},
	{l_F, Q}, {l_F, Q}, {l_E, Q}, {l_E, Q}, {l_D, H}, {l_G, Q}, {l_G, Q}, {l_F, Q},
	{l_F, Q}, {l_E, Q}, {l_E, Q}, {l_D, H}, {l_C, Q}, {l_C, Q}, {l_G, Q}, {l_G, Q},
	{h_A, Q}, {h_A, Q}, {l_G, H}, {l_F, Q}, {l_F, Q}, {l_E, Q}, {l_E, Q}, {l_D, Q},
	{l_D, Q}, {l_C, H},
};

#define numNote2 sizeof(SONG2) / sizeof(SONG2[0])
const struct note SONG2[] = {
	{l_D, Q}, {l_C, Q}, {l_As, Q}, {l_C, Q}, {l_D, Q}, {l_D, Q}, {l_D, H}, {l_C, Q},
	{l_C, Q}, {l_C, H}, {l_C, Q}, {l_D, Q}, {l_F, Q}, {l_F, H}, {l_D, Q}, {l_C, Q},
	{l_As, Q}, {l_C, Q}, {l_D, Q}, {l_D, Q}, {l_D, Q}, {l_D, Q}, {l_C, Q}, {l_C, Q},
	{l_D, Q}, {l_C, Q}, {l_As, W},
};

#define numNote3 sizeof(SONG3)/sizeof(SONG3[0])
const struct note SONG3[]={
	{l_C, E}, {l_C, E}, {l_D, Q}, {l_C, Q}, {l_F, Q}, {l_E, H}, {l_C, E}, {l_C, E},
	{l_D, Q}, {l_C, Q}, {l_G, Q}, {l_F, H}, {l_C, E}, {l_C, E}, {h_C, Q}, {l_A, Q},
	{l_F, Q}, {l_E, Q}, {l_D, H}, {l_As, E}, {l_As, E}, {l_A, Q}, {l_F, Q}, {l_G, Q},
	{l_F, H},
};

int volume_level = 1;
int pitch_level = 0;
int tempo_level = 0;

int song1_size = 26;
int song_index = 0;

void play_song();
void play_note(int freq, int dur);
int is_pressed(int r, int c);
int get_key(void);
void next_song();
void lcd_display();
void volume_display();
void volume_control();
void pitch_control();
void tempo_control();

int main(void)
{ 
	
	lcd_init();
	lcd_clr();
	DDRA = 1;
	int k;
	while (1){
		k = get_key(); 
		
		switch(k)
		{
			case 4:
				volume_level++;
				volume_control();
				break;
				
			case 8:
				volume_level--;
				volume_control();
				break;
				
			case 3:
				pitch_level++;
				pitch_control();
				break;
				
			case 7:
				pitch_level--;
				pitch_control();
				break;	
				
			case 2:
				tempo_level++;
				tempo_control();
				break;
				
			case 6:
				tempo_level--;
				tempo_control();
				break;
				
			case 1:
				next_song();
				break;	
				
			case 15:
				play_song();
				break;
				
			default:
				break;
		}
		avr_wait(1000);
    }
}
void tempo_control(){
	if(tempo_level == 3){
		tempo_level = 2;
	}else if(tempo_level ==-1){
		tempo_level = 0;
	}
}

void pitch_control(){
	if(pitch_level==3){
		pitch_level = 2;
	}else if(pitch_level==-1){
		pitch_level = 0;
	}
}

void volume_control(){
	if(volume_level==3){
		volume_level = 2;
	}else if(volume_level==-1){
		volume_level = 0;
	}
}

void next_song(){
	song_index++;
	if(song_index==3){
		song_index = 0;
	}
	volume_level = 1;	
	play_song();
}

void volume_display(){
	lcd_pos(1,0);	
	if(volume_level == 1){
		lcd_puts2("volume: Mid");
	}else if(volume_level == 0){
		lcd_puts2("volume: Min");
	}else if(volume_level == 2){
		lcd_puts2("volume: Max");
	}
}

void lcd_display(){
	lcd_pos(0,0);
	if(song_index==0){
		lcd_puts2("Happy Birthday");
	}else if(song_index == 1){
		lcd_puts2("Twinkle Twinkle Little Star");
	}else{
		lcd_puts2("Marry Had a little lamb");
	}
	volume_display();
}

int is_pressed(int r, int c){
	//set all
	DDRC = 0;
	PORTC = 0;
	
	CLR_BIT(DDRC,r);
	SET_BIT(PORTC,r);
	
	SET_BIT(DDRC,c+4);
	CLR_BIT(PORTC,c+4);
	return (GET_BIT(PINC,r))?0:1; //if r bit is 0 return 1 else return 0
} 

int get_key(void){
	int r, c;
	for(r = 0; r < 4; r++){
		for(c = 0; c < 4; c++){
			if(is_pressed(r,c)){
				return (r*4)+c+1;
			}
		}
	}
	return 0;
}

void play_song(){
	int duration_extension;
	int i;
	int song_len = 0;
	if(song_index == 0){
		song_len = numNote1;
	}else if(song_index == 1){
		song_len = numNote2;
	}else{
		song_len = numNote3 ;
	}
	lcd_display();

	if(tempo_level == 0){
		duration_extension = 3000;
	}else if(tempo_level == 1){
		duration_extension = 1500;
	}else{
		duration_extension = 300;
	}
	for(i = 0; i < song_len; i++){
		if(song_index == 0){
				play_note((int)(SONG1[i].freq+pitch_level*440), SONG1[i].duration*duration_extension);
			}else if(song_index == 1){
				play_note((int)(SONG2[i].freq+pitch_level*440), SONG2[i].duration*duration_extension);
			}else{
				play_note((int)(SONG3[i].freq+pitch_level*440), SONG3[i].duration*duration_extension);
		}
		if(get_key() == 16){
			SPK_OFF;
			break;
		}
		volume_control();
		tempo_control();
		pitch_control();
	}
}

void play_note(int freq, int dur){
	double period = 10000.0/(double)freq;
	int i, t = period/2; 
	int cycle= dur/period;

	double Ton,Toff;
	
	if(volume_level == 0){ //quietest
		Ton = 0;
	}else if(volume_level == 1){
		Ton = t*0.08;
	}else{//loudest
		Ton = t;
	}
	Toff = period - Ton;
	
	for(i = 0; i < cycle; i++){
		SPK_ON;
		avr_wait((int)Ton);
		SPK_OFF;
		avr_wait((int)Toff);
	}
}