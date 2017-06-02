//#include <fstream>
//#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>

#define value(x) printf("%s = %d\n", #x, x);

void transfer(unsigned char* in1,unsigned char* in2,int height, int width, int k,unsigned char* out,int x, int y);

void readHeader(char *filename, unsigned char* head)
{
	FILE* fd = fopen(filename, "r");
	if(!fd)
		return;
	if(fread(head, sizeof(unsigned char), 54, fd) != 54)
		puts("Niepoprawnie wczytany naglowek");
	fclose(fd);
}

unsigned char* readBMP(char* filename, int *size_file,int *height,int *width)
{

	FILE* f = fopen(filename, "r");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f);

	
	*width = *(int*)&info[18];
	*height = *(int*)&info[22];
	*size_file = *(int*)&info[2]; 


	
    unsigned char* data = (unsigned char*)malloc(*size_file);
    fread(data, sizeof(unsigned char), *size_file, f); 
    fclose(f);

    return data;
}

int main(int argc, char **argv)
{
	if(argc == 4)
	{
		unsigned char header[54];
		int size1, size2,height1,width1,height2,width2;
		unsigned char* file1 = readBMP(argv[1], &size1, &height1, &width1);
		unsigned char* file2 = readBMP(argv[2], &size2, &height2, &width2);
		int k = atoi(argv[3]) ;

		allegro_init();
		set_window_title("AlfaBlending");
		set_color_depth( 32 );
		set_gfx_mode( GFX_AUTODETECT_WINDOWED, 1000, 1000, 0, 0 );
		install_keyboard();
		BITMAP * obrazek = NULL;

/// pętla 

		while( !key[ KEY_ESC ] )
		{
			int klawisz = 48;
			int val1 = 0 ,val2 = 0;
			int scancode;
			///reading X
				do
				{
					clear_to_color( screen, makecol( 0, 0, 0 ) );
				
					klawisz = klawisz-48;
					if(klawisz <0 || klawisz>9) break;        
					textout_ex(screen, font, "X: ", 30, 40,makecol(255, 255, 255), -1);
					
					val1 = val1*10 + klawisz;
					textprintf_ex( screen, font,50, 40,makecol( 255, 255, 255 ), 0,"%d",val1 );
				
					textout_ex(screen, font, "Alfablending vbeta.0", 10, 10,makecol(255, 255, 255), -1);
					klawisz = ureadkey(&scancode);
					if(klawisz == 27) 
					{
						free(file1);
						free(file2);
						allegro_exit();
						return 0;
					}
				}while( klawisz != 13 );
				klawisz = 48;
				while( klawisz != 13 )
				{
					if(klawisz == 27)
					if(klawisz == 27) 
					{
						free(file1);
						free(file2);
						allegro_exit();
						return 0;
					}
					clear_keybuf();
					clear_to_color( screen, makecol( 0, 0, 0 ) );
				
					klawisz = klawisz-48;
					if(klawisz <-1 || klawisz>9) break;
					val2 = val2*10 + klawisz;
					textout_ex(screen, font, "Alfablending vbeta.0", 10, 10,makecol(255, 255, 255), -1);
					
					textout_ex(screen, font, "X: ", 30, 40,makecol(255, 255, 255), -1);
					textprintf_ex( screen, font,50, 40,makecol( 255, 255, 255 ), 0,"%d",val1 );
					
					textout_ex(screen, font, "Y: ", 90, 40,makecol(255, 255, 255), -1);
					textprintf_ex( screen, font,110, 40,makecol( 255, 255, 255 ), 0,"%d",val2 );
				

					klawisz = ureadkey(&scancode);
				
				}
				

			unsigned char* file3 = (unsigned char*)malloc(size1);
			transfer(file1,file2,height1,width1,k,file3,val1,val2);
			readHeader(argv[1], header);
		
			FILE *fd = fopen("wynik.bmp", "w");
			if(!fd)
			{
				puts("Blad zapisu");
				free(file1);
				free(file2);
				allegro_exit();
				return 0;
			}
			fwrite(header, sizeof(unsigned char), 54, fd);
			fwrite(file3, sizeof(unsigned char), size1, fd);
			fclose(fd);
			free(file3);
		
		
			obrazek = load_bmp( "wynik.bmp", default_palette );
			if( !obrazek )
			{
				set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
				allegro_message( "nie mogę załadować obrazka obrazek !" );
				allegro_exit();
				return 0;
			}
			clear_to_color( screen, makecol( 0, 0, 0 ) );
			textout_ex(screen, font, "Alfablending vbeta.0", 10, 10,makecol(255, 255, 255), -1);
			textout_ex(screen, font, "Result: ", 20, 40,makecol(255, 255, 255), -1);
			blit( obrazek, screen, 0, 0, 100, 100, obrazek->w, obrazek->h );
			readkey();
		
		}
		free(file1);
		free(file2);
		return 0;
	}			
	puts("\n\tMissing arguments\n");
	return 0;
}
