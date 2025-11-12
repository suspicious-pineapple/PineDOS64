#include "kernel.h"
#include "drawing.h"






uint64_t character_bitmaps[128] = {
0x00000000000000,
0x00040004040404,
0x00000000000a0a,
0x0a0a1f0a1f0a0a,
0x040e140e050e04,
0x00191a040b1300,
0x16091502050502,
0x00000000000404,
0x08040202020408,
0x02040808080402,
0x00000a040a0000,
0x0004041f040400,
0x01020000000000,
0x0000000e000000,
0x00060600000000,
0x01020204080810,
0x0c12121212120c,
0x1c080808080c08,
0x1e02021c10100e,
0x06080806080806,
0x08081f090a0408,
0x040a080602020e,
0x0c12120e02020c,
0x0404040808101e,
0x0c12120c12120c,
0x0408101c12120c,
0x06060000000606,
0x0404040000040c,
0x00080402040800,
0x00000e000e0000,
0x00020408040200,
0x0400040810120c,
0x1e011d151d110e,
0x11110e0a0a0404,
0x0e12120e12120e,
0x0c02020202020c,
0x060a1212120a06,
0x0e02020e02020e,
0x0202020e02021e,
0x0e11151d01010e,
0x1212121e121212,
0x0e04040404040e,
0x040a0808080808,
0x11090502050911,
0x0e020202020202,
0x11111115151b11,
0x11110915121100,
0x0e111111110e00,
0x0202021e12121e,
0x16091511110e00,
0x11110907090907,
0x0c12100e02120c,
0x0404040404041f,
0x0c121212121200,
0x04040a11110000,
0x0a0a0a15151511,
0x11110a040a1100,
0x04040404040a11,
0x1f01020408100f,
0x0e02020202020e,
0x08080804040202,
0x0e08080808080e,
0x000000000a0400,
0x1e000000000000,
0x00000000080400,
0x000c121c100c00,
0x000c12120e0202,
0x00180404041800,
0x000c12121c1010,
0x000c020e120c00,
0x0004040c040418,
0x0e101e12120c00,
0x001212120e0202,
0x00080808000800,
0x08141010001000,
0x0014140c140400,
0x000c0404040400,
0x0011151b0a0000,
0x001212120c0000,
0x000c12120c0000,
0x020e12120c0000,
0x101c12120c0000,
0x00040404140800,
0x00060806020c00,
0x081404041e0404,
0x000e0a0a0a0000,
0x00040a0a0a0000,
0x000a1515110000,
0x000a04040a0000,
0x02040814120000,
0x000e04080e0000,
0x0c02020102020c,
0x00040404040400,
0x06080810080806,
0x00000815020000,
0x0a150a150a150a
};




void draw_pixel(uint16_t x, uint16_t y, uint32_t color){
    uint64_t index = y * (kglobals.framebuffer.pitch/4) + x;

    kglobals.framebuffer.address[index] = color;



}

void draw_rect(uint16_t x, uint16_t y, uint16_t width,uint16_t height,uint32_t color){

    uint64_t index = y * (kglobals.framebuffer.pitch/4) + x;
    for(uint16_t yoffset=0; yoffset < height; yoffset++){
        for(uint16_t xoffset=0; xoffset < width;xoffset++){
            kglobals.framebuffer.address[index] = color;
            index++;
        }
        index+=(kglobals.framebuffer.pitch/4)-width;
    }




}




void draw_character(uint8_t character, uint16_t x, uint16_t y, uint32_t fgcolor, uint32_t bgcolor, uint8_t scale){
    if(character < 32 || character >= 128){return;};
    uint64_t bitmap = character_bitmaps[character-32];
    for(uint8_t yoffset = 0; yoffset < 7; yoffset++){
        for(uint8_t xoffset=0; xoffset < 5; xoffset++){
            if(bitmap & 1){
                draw_rect(x+xoffset*scale,y+yoffset*scale,scale,scale,fgcolor);
            } else {
                draw_rect(x+xoffset*scale,y+yoffset*scale,scale,scale,bgcolor);
            }
            bitmap = bitmap >> 1;
        }
        bitmap = bitmap >> 3;

    }
}

void render_console(console_buffer_t *console){

    
    for(uint16_t row = 0; row < console->rows;row++){
        for(uint16_t column = 0; column < console->columns; column++)
            {
                
                draw_rect(column*(5+1)*console->display_scale,row*(7+2)*console->display_scale,7*console->display_scale,9*console->display_scale,0x2F0F4F);
                character_entry_t character = console->buffer[row*console->columns + column];
                if(character.character==0){character.character=32;character.background=console->default_background;};
                if(character.character>126 || character.character<32){character.character=127;character.background=console->default_background;};

                draw_character(character.character, column*(5+1)*console->display_scale,row*(7+2)*console->display_scale, character.foreground, character.background, console->display_scale);

            }
    }

}


void put_char(console_buffer_t *console, uint8_t character, uint16_t column, uint16_t row, uint32_t foreground, uint32_t background){
    
    console->buffer[console->columns*row+column].character = character;
    console->buffer[console->columns*row+column].foreground = foreground;
    console->buffer[console->columns*row+column].background = background;

}

void print_char(console_buffer_t *console, uint8_t character){

    


    if(character == '\r'){
    	console->cursor_col = 0;
	return;
    };
    

    
    if(character=='\n'){
    scroll_console(console);  
    return;
    };


    put_char(console, character, console->cursor_col, console->cursor_row, console->default_foreground, console->default_background);
    console->cursor_col++;

    if(console->cursor_col > console->columns){
        scroll_console(console);
        console->cursor_col = 0;
    }
    

}

void scroll_console(console_buffer_t *console){

    if(console->cursor_row+2 < console->rows){
        console->cursor_row++;
        return;
    }



    for(uint16_t y = 1; y < console->rows; y++){
        for(uint16_t x=0;x<console->columns;x++){

                console->buffer[console->columns*(y-1)+x].character = console->buffer[console->columns*(y)+x].character;
                console->buffer[console->columns*(y-1)+x].foreground = console->buffer[console->columns*(y)+x].foreground;
                console->buffer[console->columns*(y-1)+x].background = console->buffer[console->columns*(y)+x].background;

        }
    }

    
};


void print_string(console_buffer_t *console, char* string){
    uint32_t i = 0;
    while(1){
	if(string[i]==0){
	    break;
	}
	print_char(console, string[i]);
	i++;
    }

};


void print_hex64(console_buffer_t* console, uint64_t number){
    for(uint8_t i = 0; i < 8; i++){
        print_hex_byte(console, ((uint8_t*)(&number))[7-i]);
    }
};
void print_hex_byte(console_buffer_t* console, uint8_t number){

    uint8_t nibbles[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    print_char(console, nibbles[number>>4]);
    print_char(console, nibbles[number&0x0F]);
    return;
}






void hex_dump(console_buffer_t* console, size_t start, size_t length){
    uint8_t bytes_per_line = 8;
    
    for(size_t i = 0; i < length; i+=bytes_per_line){
        print_string(console, "\r\n");
        for(size_t j = 0; j < bytes_per_line; j++){
            if(i+j > length){
                //continue;
            }
            print_hex_byte(console, *(uint8_t*)(i+j+start));
            print_char(console,' ');
        }
        print_char(console,' ');
        print_char(console,' ');
        print_char(console,' ');
        for(size_t j = 0; j < bytes_per_line; j++){
            if(i+j > length){
                //continue;
            }
            print_char(console, *(uint8_t*)(i+j+start));
        }
        
        
    }
    print_string(console, "\r\n");
    print_string(console, "\r\n");

}