/* Includes ------------------------------------------------------------------*/
#include "SST25VF016B.h"
#include "FlashDriver.h"
#include "GLCD.h" 
#include "BMP.c"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
	uint8_t  pic_head[2];				
	uint16_t pic_size_l;			    
	uint16_t pic_size_h;			   
	uint16_t pic_nc1;				  
	uint16_t pic_nc2;				   
	uint16_t pic_data_address_l;	   
	uint16_t pic_data_address_h;	
	uint16_t pic_message_head_len_l;
	uint16_t pic_message_head_len_h;
	uint16_t pic_w_l;				
	uint16_t pic_w_h;				   
	uint16_t pic_h_l;				   
	uint16_t pic_h_h;				   	
	uint16_t pic_bit;				   
	uint16_t pic_dip;				   
	uint16_t pic_zip_l;			       
	uint16_t pic_zip_h;			       
	uint16_t pic_data_size_l;		   
	uint16_t pic_data_size_h;		   
	uint16_t pic_dipx_l;			   
	uint16_t pic_dipx_h;			   	
	uint16_t pic_dipy_l;			   
	uint16_t pic_dipy_h;			   
	uint16_t pic_color_index_l;	       
	uint16_t pic_color_index_h;	       
	uint16_t pic_other_l;			   
	uint16_t pic_other_h;			   
	uint16_t pic_color_p01;		       
	uint16_t pic_color_p02;		       
	uint16_t pic_color_p03;		        
	uint16_t pic_color_p04;		       
	uint16_t pic_color_p05;		       
	uint16_t pic_color_p06;		       
	uint16_t pic_color_p07;		       
	uint16_t pic_color_p08;			    			
}BMP_HEAD;

BMP_HEAD bmp;

typedef struct
{
	uint16_t x;							/* LCD X */
	uint16_t y;							/* LCD Y	*/
	uint8_t  r;				            /* RED */
	uint8_t  g;						    /* GREEN */
	uint8_t  b;							/* BLUE	*/
}BMP_POINT;

BMP_POINT point;

/* Private define ------------------------------------------------------------*/
#define BMP_ADDR  (1024*0)

int main(void)
{
  uint16_t count;

  // SPI-Flash und LCD initialisieren
  SPI_FLASH_Init();
  LCD_Initializtion();
  SPI_FLASH_Test();
  SSTF016B_Erase( BMP_ADDR/SEC_SIZE , ( BMP_ADDR+sizeof(BMP_DATA) )/SEC_SIZE +1 );
  LCD_Clear(White); 

  // Statusmeldung schreiben
  GUI_Text(0,152,"Bild in SPI-Flash schreiben...",Black,White);
  
  // Bitmap aus BMP.c in SPI-Flash schreiben
  df_write_open( BMP_ADDR );

  for(count=0;count<sizeof(BMP_DATA)/255;count++)
  {
    df_write( (uint8_t*)&BMP_DATA[count*255],255); 
    df_write_close(); 
  }

  df_write( (uint8_t*)&BMP_DATA[count*255],sizeof(BMP_DATA)%255);  
  df_write_close();   
   
  // Bild aus SPI-Flash lesen
  df_read_open( BMP_ADDR );
  df_read( (uint8_t *)&bmp, sizeof(bmp));  // Bitmap-Header lesen
    
  if( (bmp.pic_head[0]=='B') && (bmp.pic_head[1]=='M') )
  {
    uint16_t  tx,ty;
	uint8_t buffer[1024]; 

	df_read_seek( BMP_ADDR + ( bmp.pic_data_address_h<<16 | bmp.pic_data_address_l ) );

    // Bitmap zeilenweise auslesen
	for(ty=0;ty<bmp.pic_h_l;ty++)
	{	
	  df_read( &buffer[0] , (bmp.pic_w_l)*3 );

	  // Pixel aus jeder Zeile auslesen
	  for(tx=0;tx<bmp.pic_w_l;tx++)
	  {
        point.r = *(tx*3 +2+buffer);
	    point.g = *(tx*3 +1+buffer);
	    point.b = *(tx*3 +0+buffer);								
	    point.x = tx;
	    point.y = ty;

	    // Pixel anzeigen
        LCD_SetPoint(point.x ,319-point.y,RGB565CONVERT(point.r,point.g,point.b));
      }                         
	}
  } 
  /* Infinite loop */
  while (1) ;
}

