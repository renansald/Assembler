#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

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

#endif
