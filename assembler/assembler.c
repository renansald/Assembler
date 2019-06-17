#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Endereco{
  char endereco[100];
  char binario[9];
} Endereco;

//Subrotinas
void PrimeiroPasso(char arquivo[], Endereco enderecos[]);
void SegundoPasso(char arquivo[], Endereco enderecos[]);
void ConvertDecimalToBinario(int num, char endereco[10]);
void ConvertDecimalToHexadecimal(int num, char endereco[2]);
void ConvertInstruction(char matriz[][200], Endereco enderecos[], char arquivo[], int linha);
void FindAddress(Endereco enderecos[], char endereco[], char binario[]);
void FindRegister(char registrador[], char binario[]);
void FirstWord(char registrador[], char binario[]);
void SaveFile(char arquivo[], char binario1[], char binario2[], int linha);
void ComplementoDeDois(char opCode[9], char binario[9]);
void BinaryToLoadC(char binario[], char binario2[], int num);
void Data(int numBytes, int valor, int linha, char arquivo[]);
void RemoveArquivo(char arquivo[]);

//Função Principal
int main(int argc, char const *argv[]) {
  FILE *pFile;
  char arquivo[10000];
  Endereco enderecos[64];
  printf("Informe o arquivo que desejar traduzir: ");
  scanf(" %[^\n]s", arquivo);

  while(1){
    pFile = fopen(arquivo, "rb");
    if(pFile != NULL){
      fclose(pFile);
      break;
    }
    printf("Arquivo inválido desejar informa outro arquivo? Digite s para sim e n para sair da aplicação: ");
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
  }
  PrimeiroPasso(arquivo, enderecos);
  SegundoPasso(arquivo, enderecos);
  printf("Arquivo montado com sucesso\n");
  return 0;
}


//Subrotina para executar o primeiro passo do montador
void PrimeiroPasso(char arquivo[], Endereco enderecos[]){
  FILE *pFile = fopen(arquivo, "rb");
  int aux = 1;
  int linhaDoc = 0;
  int ctrlVetorStruct = 0;
  char buffer;
  while(!feof(pFile)){
    if(linhaDoc > 127){
      printf("Erro extouro de memória, a aplicação será encerada\n");
      exit(0);
    }
    buffer = fgetc(pFile);
    if(buffer == '_'){
      int n = 1;
      char  enderecoAux2[200];
      enderecoAux2[0] = '_';
      while(1){
        buffer = fgetc(pFile);
        if(buffer == ':'){
          char primieriInstrucao[20], enderecoAux[9];
          buffer = fgetc(pFile);
          while(buffer == ' '){
            buffer = fgetc(pFile);
          }
          for(int i = 0; buffer != ' '; i++){
            primieriInstrucao[i] = buffer;
            buffer = fgetc(pFile);
            if(buffer == ' '){
              primieriInstrucao[i+1] = '\0';
            }
          }
          enderecoAux2[n] = '\0';
          if(strcmp(primieriInstrucao, ".data") == 0){
            int byte;
            fscanf(pFile," %d", &byte);
            if(byte%2 !=0){
              printf("Erro a instrução .data dever ter seu endereço como multiplos de 2, a aplicação será encerrada\n");
              exit(0);
            }
            aux = linhaDoc*2;
            strcpy(enderecos[ctrlVetorStruct].endereco, enderecoAux2);
            ConvertDecimalToBinario(aux, enderecoAux);
            strcpy(enderecos[ctrlVetorStruct].binario, enderecoAux);
            aux =  (byte/2);
          }
          else{
            aux = linhaDoc*2;
            strcpy(enderecos[ctrlVetorStruct].endereco, enderecoAux2);
            ConvertDecimalToBinario(aux,enderecoAux);
            strcpy(enderecos[ctrlVetorStruct].binario, enderecoAux);
            aux = 1;
          }
          ctrlVetorStruct++;
          break;
        }
        else if((buffer == ' ') || (buffer == ';')){
          break;
        }
        else if(buffer == '\n'){
          linhaDoc = linhaDoc + aux;
          aux = 1;
          break;
        }
        else{
          enderecoAux2[n] = buffer;
          n++;
        }
      }
    }
    else if(buffer == '\n'){
      linhaDoc+= aux;
    }
  }
  fclose(pFile);
  return;
}

void SegundoPasso(char arquivo[], Endereco enderecos[]){
  FILE *pFile, *destFile;
  char arquivoDestino[1000];
  int linha = 0;
  char matriz[4][200];
  char buffer;
  int l = 0;
  int c = 0;
  for (int i = 0; ; i++) {
    if((arquivo[i] == '.') && (arquivo[i+1] == 'a')){
      arquivoDestino[i] = '\0';
      strcat(arquivoDestino, ".mif");
      break;
    }
    else{
      arquivoDestino[i] = arquivo[i];
    }
  }
  destFile = fopen(arquivoDestino, "wb+");
  fprintf(destFile, "DEPTH = 128;\nWIDTH = 8;\nADDRESS_RADIX = HEX;\nDATA_RADIX = BIN;\nCONTENT\nBEGIN\n\n");
  fclose(destFile);
  pFile = fopen(arquivo, "rb");
  while(!feof(pFile)){
    buffer = fgetc(pFile);
    char bufferAux;
    if((buffer != '_') && (buffer != ' ') && (buffer != EOF)){
      int ctrlLeitura = 0;
      matriz[l][c] = buffer;
      c++;
      while(1){
        if(feof(pFile)){
          ctrlLeitura = 1;
          break;
        }
        buffer = fgetc(pFile);
        if((buffer == ' ') && (bufferAux != ' ')){
          matriz[l][c] = '\0';
          l++;
          c = 0;
        }
        else if(buffer == ';'){
          matriz[l][c] ='\0';
          while(fgetc(pFile) != '\n'){
            if(feof(pFile)){
              fclose(pFile);
              buffer = EOF;
              break;
            }
          }
          l = 0;
          c = 0;
          break;
        }
        else if((buffer == '\n') || (buffer == EOF)){
          if(matriz[l][c-1] == '\r'){
            matriz[l][c-1] = '\0';
          }
          else{
            matriz[l][c] = '\0';
          }
          l = 0;
          c = 0;
          break;
        }
        else if((buffer == ' ') && (bufferAux == ' ')){
          bufferAux = buffer;
          continue;
        }
        else{
          matriz[l][c] = buffer;
          c++;
        }
        bufferAux = buffer;
      }
      if(ctrlLeitura != 0){
        break;
      }
      printf("%s: %s: %s\n", matriz[0], matriz[1], matriz[2]);
      ConvertInstruction(matriz, enderecos, arquivoDestino, linha);
      if(strcmp(matriz[0], ".data") == 0){
        int linhaAux = atoi(matriz[1]);
        linha = linha + (linhaAux);
      }
      else{
        linha += 2;
      }
    }
    else if(buffer == '_'){
      while(fgetc(pFile) != ' '){
      }
    }
    else if(buffer == EOF){
      break;
    }
  }
  fclose(pFile);
  destFile = fopen(arquivoDestino, "a");
  if(linha < 128){
    strcpy(matriz[0], "[");
    char aux[3];
    ConvertDecimalToHexadecimal(linha, aux);
    strcat(matriz[0], aux);
    strcat(matriz[0], "..7F]:  00000000;");
    fprintf(destFile,"%s\nEND;\n", matriz[0]);
  }
  else{
    fprintf(destFile,"END;\n");
  }
  fclose(destFile);
}

//Função de conversão de decimal para binário, necessária para converter e construir um os instruções
void ConvertDecimalToBinario(int num, char endereco[9]){
  for (int  i = 7; i >= 0; i--) {
    endereco[i] = '0' + num%2;
    num = num/2;
  }
  endereco[8] = '\0';
}

//Subrotina que converte de Decimal para Hexadecimal, necessária para atualizar os endereços do código
void ConvertDecimalToHexadecimal(int num, char endereco[3]){
  int n = 1;
  for(int n = 1; n >= 0; n--){
    switch (num%16) {
      case 10:
      endereco[n] = 'A';
      break;
      case 11:
      endereco[n] = 'B';
      break;
      case 12:
      endereco[n] = 'C';
      break;
      case 13:
      endereco[n] = 'D';
      break;
      case 14:
      endereco[n] = 'E';
      break;
      case 15:
      endereco[n] = 'F';
      break;
      default:
      endereco[n] = '0' + num%16;
    }
    num = num/16;
  }
  endereco[2] = '\0';
}

//Converte instrução em binário
void ConvertInstruction(char matriz[][200], Endereco enderecos[], char arquivo[], int linha){
  char binario[2][9];
  int numAux;
  if(strcmp(matriz[0], "stop") == 0){
    strcpy(binario[0], "00000000");
    strcpy(binario[1], "00000000");
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "load") == 0){
    strcpy(binario[0], "00001");
    FirstWord(matriz[1], binario[0]);
    FindAddress(enderecos, matriz[2], binario[1]);
    if((strcmp(binario[0], "00001xxx") == 0)|| (strcmp(binario[1], "xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "store") == 0){
    strcpy(binario[0], "00010");
    FirstWord(matriz[1], binario[0]);
    FindAddress(enderecos, matriz[2], binario[1]);
    if((strcmp(binario[0], "00010xxx") == 0)|| (strcmp(binario[1], "xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "read") == 0){
    strcpy(binario[0], "00011000");
    FindRegister(matriz[1], binario[1]);
    if((strcmp(binario[1], "xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "write") == 0){
    strcpy(binario[0], "00100000");
    FindRegister(matriz[1], binario[1]);
    if((strcmp(binario[1], "xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "add") == 0){
    strcpy(binario[0], "00101");
    FirstWord(matriz[1], binario[0]);
    FindRegister(matriz[2], binario[1]);
    if((strcmp(binario[0], "00101xxx") == 0)|| (strcmp(binario[1], "xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "subtract") == 0){
    strcpy(binario[0], "00110");
    FirstWord(matriz[1], binario[0]);
    FindRegister(matriz[2], binario[1]);
    if((strcmp(binario[0], "00110xxx") == 0)|| (strcmp(binario[1], "xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "multiply") == 0){
    strcpy(binario[0], "00111");
    FirstWord(matriz[1], binario[0]);
    FindRegister(matriz[2], binario[1]);
    if((strcmp(binario[0], "00111xxx") == 0)|| (strcmp(binario[1], "xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "divide") == 0){
    strcpy(binario[0], "01000");
    FirstWord(matriz[1], binario[0]);
    FindRegister(matriz[2], binario[1]);
    if((strcmp(binario[0], "01000xxx") == 0)|| (strcmp(binario[1], "xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "jump") == 0){
    strcpy(binario[0], "01001000");
    FindAddress(enderecos, matriz[1], binario[1]);
    if((strcmp(binario[1], "xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "jmpz") == 0){
    strcpy(binario[0], "01010");
    FirstWord(matriz[1], binario[0]);
    FindAddress(enderecos, matriz[2], binario[1]);
    if((strcmp(binario[0], "01010xxx") == 0)|| (strcmp(binario[1], "xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "jmpn") == 0){
    strcpy(binario[0], "01011");
    FirstWord(matriz[1], binario[0]);
    FindAddress(enderecos, matriz[2], binario[1]);
    if((strcmp(binario[0], "01011xxx") == 0)|| (strcmp(binario[1], "xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "move") == 0){
    strcpy(binario[0], "01100");
    FirstWord(matriz[1], binario[0]);
    FindRegister(matriz[2], binario[1]);
    if((strcmp(binario[0], "01100xxx") == 0)|| (strcmp(binario[1], "xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "push") == 0){
    strcpy(binario[0], "01101000");
    FindRegister(matriz[1], binario[1]);
    if((strcmp(binario[1], "xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "pop") == 0){
    strcpy(binario[0], "01110000");
    FindRegister(matriz[1], binario[1]);
    if((strcmp(binario[1], "xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "call") == 0){
    strcpy(binario[0], "01111000");
    FindAddress(enderecos, matriz[1], binario[1]);
    if((strcmp(binario[1], "xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "return") == 0){
    strcpy(binario[0], "10000000");
    strcpy(binario[1], "00000000");
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "load_c") == 0){
    strcpy(binario[0], "10011");
    FirstWord(matriz[1], binario[0]);
    if((strcmp(binario[0], "10011xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    for(int i = 0; enderecos[i].endereco[0] != '\0'; i++){
      if(strcmp(matriz[2], enderecos[i].endereco) == 0){
        strcpy(binario[1], enderecos[i].binario);
        SaveFile(arquivo, binario[0], binario[1], linha);
        return;
      }
    }
    numAux = atoi(matriz[2]);
    if((numAux > 511) || (numAux < -512)){
      printf("Extouro de memória, o intervalo suportado por essa instrução está entre -512 e 511, a aplicação será encerrada\n");
      RemoveArquivo(arquivo);
    }
    BinaryToLoadC(binario[0], binario[1], numAux);
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], "copytop") == 0){ //Falta Fazer
    strcpy(binario[0], "10110000");
    FindRegister(matriz[1], binario[1]);
    if((strcmp(binario[1], "xxx") == 0)){
      RemoveArquivo(arquivo);
    }
    SaveFile(arquivo, binario[0], binario[1], linha);
    return;
  }
  else if(strcmp(matriz[0], ".data") == 0){
    int numBytes = atoi(matriz[1]);
    int valor = atoi(matriz[2]);
    Data(numBytes, valor, linha, arquivo);
    return;
  }
  else{
    printf("ERROR\n instrução inexiste a aplicação será encerada\n");
    RemoveArquivo(arquivo);
  }
}

void FindAddress(Endereco enderecos[], char endereco[], char binario[]){
  for (int i = 0; enderecos[i].endereco[0] != '\0'; i++) {
    if(strcmp(enderecos[i].endereco, endereco) == 0){
      strcpy(binario, enderecos[i].binario);
      return;
    }
  }
  printf("ERRO: Endereço inexistente, a aplicação será encerrada");
  strcpy(binario, "xxx");
}

void FindRegister(char registrador[], char binario[]){
  if(strcmp(registrador, "A0") == 0){
    strcpy(binario, "00000000");
    return;
  }
  else if(strcmp(registrador, "A1") == 0){
    strcpy(binario, "00000001");
    return;
  }
  else if(strcmp(registrador, "A2") == 0){
    strcpy(binario, "00000010");
    return;
  }
  else if(strcmp(registrador, "A3") == 0){
    strcpy(binario, "00000011");
    return;
  }
  printf("ERROR\n registrador inexiste a aplicação será encerada\n");
  strcpy(binario, "xxx");
}

void FirstWord(char registrador[], char binario[]){
  if(strcmp(registrador, "A0") == 0){
    strcat(binario, "000");
    return;
  }
  else if(strcmp(registrador, "A1") == 0){
    strcat(binario, "010");
    return;
  }
  else if(strcmp(registrador, "A2") == 0){
    strcat(binario, "100");
    return;
  }
  else if(strcmp(registrador, "A3") == 0){
    strcat(binario, "110");
    return;
  }
  else{
    printf("ERROR\n registrador inexiste a aplicação será encerada\n");
    strcat(binario, "xxx");
  }
}

void SaveFile(char arquivo[], char binario1[], char binario2[], int linha){
  char aux[3], aux1[3];
  int linha2 = linha+1;
  ConvertDecimalToHexadecimal(linha, aux);
  ConvertDecimalToHexadecimal(linha2, aux1);
  FILE *pFile = fopen(arquivo, "a");
  fprintf(pFile, "%s        :  %s;\n", aux, binario1);
  fprintf(pFile, "%s        :  %s;\n", aux1, binario2);
  fclose(pFile);
}

void ComplementoDeDois(char opCode[9], char binario[9]){
  for(int i = 0; i < 8; i++){
    if(binario[i] == '1'){
      binario[i] = '0';
    }
    else{
      binario[i] = '1';
    }
  }
  if(opCode[7] == '1'){
    opCode[7] = '0';
  }
  else{
    opCode[7] = '1';
  }
  for (int i = 7; i >= 0; i--) {
    if(binario[i] == '1'){
      binario[i] = '0';
    }
    else{
      binario[i] = '1';
      return;
    }
  }
  if(opCode[7] == '1'){
    opCode[7] = '0';
  }
  else{
    opCode[7] = '1';
  }
}

void BinaryToLoadC(char binario[], char binario2[], int num){
  int aux;
  if(num < 0){
    aux = num * (-1);
    for (int i = 7; i >= -1; i--) {
      if(i != -1){
        binario2[i] = '0' + aux%2;
      }
      else{
        binario[7] = '0' + aux%2;
      }
      aux = aux/2;
    }
    ComplementoDeDois(binario, binario2);
    return;
  }
  else{
    for (int i = 7; i >= -1; i--) {
      if(i != -1){
        binario2[i] = '0' + num%2;
      }
      else{
        binario[7] = '0' + num%2;
      }
      num = num/2;
    }
  }
}

void Data(int numBytes, int valor, int linha, char arquivo[]){
  char binario[(8*numBytes)], aux[9], auxLinha[2];
  FILE *pFile = fopen(arquivo, "a");
  for (int  i = 8*numBytes - 1; i >= 0; i--) {
    binario[i] = '0' + valor%2;
    valor = valor/2;
  }
  binario[8*numBytes] = '\0';
  int cont = 1;
  while(cont <= numBytes){
    int a = 7;
    for(int i = cont*8-1; i > ((cont-1)*8-1); i--){
      aux[a] = binario[i];
      a--;
    }
    aux[8] = '\0';
    char bin[9];
    strcpy(bin, aux);
    ConvertDecimalToHexadecimal(linha, auxLinha);
    fprintf(pFile,"%s        :  %s;\n", auxLinha, bin);
    linha++;
    cont++;
  }
  fclose(pFile);
}

void RemoveArquivo(char arquivo[]){
  remove(arquivo);
  exit(0);
}
