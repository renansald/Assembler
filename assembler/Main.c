#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Assembler.h"

//Função Principal
int main(int argc, char *argv[]) {
  FILE *pFile;
  //char arquivo[10000];
  Endereco enderecos[64];
  /*printf("Informe o arquivo que desejar traduzir: ");
  scanf(" %[^\n]s", arquivo);

  while(1){*/
    pFile = fopen(argv[1], "rb");
    if(pFile == NULL){
      fclose(pFile);
      printf("Argumento inválido saíndo do programa\n");
      exit(0);
      //break;
    }
    /*printf("Arquivo inválido desejar informa outro arquivo? Digite s para sim e n para sair da aplicação: ");
    while(1){
      char confirmaSair;
      scanf(" %c", &confirmaSair);
      if((confirmaSair == 'n') || (confirmaSair == 'N')){
        return 0;
      }
      else if((confirmaSair == 's') || (confirmaSair == 'S')){
        break;
      }
      else{
        printf("Opção inválida\n");
      }
      printf("Desejar informa outro arquivo? Digite s para sim e n para sair da aplicação: \n");
    }
    printf("Informe o arquivo que desejar traduzir: ");
    scanf(" %[^\n]s", arquivo);
  }*/
  PrimeiroPasso(argv[1], enderecos);
  SegundoPasso(argv[1], enderecos);
  printf("Arquivo montado com sucesso\n");
  return 0;
}
