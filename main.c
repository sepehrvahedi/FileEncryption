#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chacha20.h"
#include "hash.h"

void encode(char *, char * , char *);

void decode(char *, char * , char *);

int main() {
    printf("%70s\n" , "Hello welcome to advanced encoder ");
    int contin = 1 ;
    while(contin)
    {
        printf("%s\n" , "enter (e) for encode and (d) for decode  ... ");
        char str[10];
        gets(str);
        if((!strcmp(str , "e")) || (!strcmp( str, "E")))
        {
            char str_start[1001] , str_des[1001] , pass[101];
            printf("%s\n","Enter the path of the file you want to encode :");
            gets(str_start);
            printf("%s\n","Enter the path where you want to store the encoded file : ( if this file exists it is going to be overwritten)");
            gets(str_des);
            int valid_pass = 0 ;
            while(!valid_pass)
            {
                printf("%s\n","Please enter your password : ( 8 - 100 chars )");
                gets(pass);
                if(strlen(pass) >= 8 && strlen(pass) <= 100)
                {
                    valid_pass = 1;
                }
            }
            encode(str_start , str_des , pass);
        }
        else if((!strcmp(str , "d")) || (!strcmp( str, "D")))
        {
            char str_start[1001] , str_des[1001] , pass[101];
            printf("%s\n","Enter the path of the file you want to decode :");
            gets(str_start);
            printf("%s\n","Enter the path where you want to store the decoded file : ( if this file exists it is going to be overwritten)");
            gets(str_des);
            int valid_pass = 0 ;
            while(!valid_pass)
            {
                printf("%s\n","Please enter your password : ( 8 - 100 chars )*note : wrong passwords dont decode your file properly !!");
                gets(pass);
                if(strlen(pass) >= 8 && strlen(pass) <= 100)
                {
                    valid_pass = 1;
                }
                else
                {
                    printf("%s\n" , "try again !! password not valid . ");
                }
            }
            decode(str_start , str_des , pass);
        }
        else
        {
            printf("%s\n" , "input not valid try again ..");
        }
        printf("%s\n" , "do you want to continue ? (y) for yes , (n) for no .. ");
        char str_cont[100] = "y";
        gets(str_cont);
        if((!strcmp( str_cont, "n")) || (!strcmp(str_cont , "n")))
        {
            contin = 0 ;
        }
    }
    puts("have a nice day !!");
    return 0 ;

    //encode("D:\\file.txt", "D:\\file2.txt");
    //decode("D:\\file2.txt", "D:\\file3.txt");
}

void encode(char *path_start, char *path_destination , char* password) {
    FILE *f_input = fopen(path_start, "rb");
    if (f_input == NULL) {
        puts("something went wrong ! try again .. ");
        fclose(f_input);
        return;
    }
    FILE *f_output = fopen(path_destination, "wb");
    if (f_output == NULL) {
        puts("something went wrong ! try again .. ");
        fclose(f_input);
        fclose(f_output);
        return;
    }
    int has_char = 1;
    char nonce[12] = {0};
    //char key[32];
    char * key = key_gen(password);
    // read
    while (has_char) {
        char str_64[64];
        int read_size = fread(str_64, sizeof(char), sizeof(str_64), f_input);
        if (feof(f_input)) {
            for (int j = read_size; j < 63; j++) {
                str_64[j] = 0;
            }
            str_64[63] = 63 - read_size;
            has_char = 0;
        }
        encrypt(str_64, key, nonce, 64);
        fwrite(nonce, sizeof(nonce), 1, f_output);
        fwrite(str_64, sizeof(str_64), 1, f_output);
        increase_nonce((unsigned char *) nonce);
    }

    puts("file encoded !! process finished !!");
    fclose(f_input);
    fclose(f_output);
}

void decode(char *path_start, char *path_destination , char * password) {
    FILE *f_input = fopen(path_start, "rb");
    if (f_input == NULL) {
        puts("something went wrong ! try again .. ");
        fclose(f_input);
        return;
    }
    FILE *f_output = fopen(path_destination, "wb");
    if (f_output == NULL) {
        puts("something went wrong ! try again .. ");
        fclose(f_input);
        fclose(f_output);
        return;
    }
    puts("connections established !! ");
    int aval = 1;
    char nonce[12];
    //char key[32];
    char * key = key_gen(password) ;
    char old[64];
    while (1) {
        // read data an nonce
        fread(nonce, sizeof(nonce), 1, f_input);
        char temp_str[64];
        fread(temp_str, sizeof(char), sizeof(temp_str), f_input);
        // check end of file now; not after encrypt
        if (feof(f_input)) {
            fwrite(old, sizeof(temp_str) - 1 - temp_str[63], 1, f_output);
            break;
        } else if (!aval) {
            fwrite(old, sizeof(temp_str), 1, f_output);
        }
        // decrypt
        encrypt(temp_str, key, nonce, 64);
        memcpy(old, temp_str, 64);
        aval = 0;
    }
    puts("decode completed !! process finished !!");
    fclose(f_input);
    fclose(f_output);
}
