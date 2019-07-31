from sys import exit
from os import remove

class Assembler:


    def __init__(self, file):
        self.__file = file;
        self.__Adress = dict();

    def StepOne(self):
        aux = list()
        while True:
            try:
                pFile = open(self.__file, "r");
                break;
            except Exception as e:
                option = str(input("Arquivo não encontrado, deseja informar um novo arquivo [S/N]: ")).strip().lower();
                if ('n' in option):
                    sys.exit();
                elif ('s' in option):
                    self.__file = str(input("Informe o nome do arquivo: "));
        pFile = open(self.__file, "r");
        linha = 0;
        line = pFile.readline();
        while line:
            if (linha > 128):
                print("\33[7;31mERROR: STACK OVER FLOW\33[m");
                sys.exit();
            if (line[0] == "_"):
                word = line.split();
                self.__Adress[word[0][:len(word[0])-1]] = self.__ConvertDecimalToBinary(linha, 8);
            if (".data" in line):
                if (int(word[2])%2 != 0):
                    print(f"\33[7;31mERRO: the function \".data\" must be a pair size\33[m");
                    sys.exit();
                else:
                    linha = linha + ((int(word[2])));
            else:
                linha = linha + 2;
            line = pFile.readline();
        pFile.close();

    def Print(self):
        for k, v in self.__Adress.items():
            print(f"{k} : {v}");

    def __ConvertDecimalToBinary(self, num, size):
        for x in range(0, size):
            if (x == 0):
                binario = str(num%2);
                num = num//2;
            else:
                binario = binario+str(num%2);
                num = num//2;
            binario2 = binario[::-1];
        return str(binario2);

    def __ConvertDecimalToHex(self, num):
        hexAux = hex(num);
        if (len(hexAux[(hexAux.find('x')+1):]) == 1):
            return "0"+hexAux[(hexAux.find('x')+1):].upper();
        else:
            return hexAux[(hexAux.find('x')+1):].upper();

    def StepTwo(self):
        newFile = self.__file[:len(self.__file)-1]+"mif";
        fileDest = open(newFile, "w+");
        fileDest.write("DEPTH = 128;\nWIDTH = 8;\nADDRESS_RADIX = HEX;\nDATA_RADIX = BIN;\nCONTENT\nBEGIN\n\n");
        fileDest.close();
        pFile = open(self.__file, "r");
        line = pFile.readline();
        linha = 0;
        while line:
            print(line);
            if linha > 128:
                print("\33[7;31mERROR: STACK OVER FLOW\33[m");
                sys.exit();
            word = line.split();
            if (word[0][0] == "_"):
                if ((word[1] == "read") or (word[1] == "write") or (word[1] == "jump") or (word[1] == "push") or (word[1] == "pop") or (word[1] == "call") or (word[1] == "copyto")):
                    self.__ConvertToMachineLanguage(word[1], word[2], line = linha, file = newFile);
                elif (word[1] == "return"):
                    self.__ConvertToMachineLanguage(word[1], line = linha, file = newFile);
                else:
                    self.__ConvertToMachineLanguage(word[1], word[2], word[3], line = linha, file = newFile);
            else:
                if ((word[0] == "read") or (word[0] == "write") or (word[0] == "jump") or (word[0] == "push") or (word[0] == "pop") or (word[0] == "call") or (word[0] == "copyto")):
                    self.__ConvertToMachineLanguage(word[0], word[1], line = linha, file = newFile);
                elif ((word[0] == "return") or (word[0] == "stop")):
                    self.__ConvertToMachineLanguage(word[0], line = linha, file = newFile);
                else:
                    self.__ConvertToMachineLanguage(word[0], word[1], word[2], line = linha, file = newFile);
            if (".data" in  line):
                linha = linha + int(word[2]);
            else:
                linha = linha + 2;
            line = pFile.readline();
        fileDest = open(newFile, "a+");
        if (linha < 128):
            fileDest.write(f"[{self.__ConvertDecimalToHex(linha)}..7F]:  00000000;\n");
        fileDest.write("END;\n");

    def __ConvertToMachineLanguage(self, *instruction, line, file):
        if (instruction[0] == "stop"):
            pFile = open(file, "a+");
            pFile.write(f"{self.__ConvertDecimalToHex(line)}        :  00000000;\n{self.__ConvertDecimalToHex(line+1)}        :  00000000;\n");
            pFile.close();
        elif (instruction[0] == "load"):
            self.__MemoryTypes("00001", instruction[1], instruction[2], file, line);
        elif (instruction[0] == "store"):
            self.__MemoryTypes("00010", instruction[1], instruction[2], file, line);
        elif (instruction[0] == "read"):
            self.__SimpleType("00011", instruction[1], file, line);
        elif (instruction[0] == "write"):
            self.__SimpleType("00100", instruction[1], file, line);
        elif (instruction[0] == "add"):
            self.__MathTypes("00101", instruction[1], instruction[2], file, line);
        elif (instruction[0] == "subtract"):
            self.__MathTypes("00110", instruction[1], instruction[2], file, line);
        elif (instruction[0] == "multiply"):
            self.__MathTypes("00111", instruction[1], instruction[2], file, line);
        elif (instruction[0] == "divide"):
            self.__MathTypes("01000", instruction[1], instruction[2], file, line);
        elif (instruction[0] == "jump"):
            pFile = open(file, "a+");
            if(instruction[1].isdigit()):
                aux = self.__ConvertDecimalToBinary(int(instruction[1]), 11)
                pFile.write(f"{self.__ConvertDecimalToHex(line)}        :  01001{aux[0:3]};\n{self.__ConvertDecimalToHex(line+1)}        :  {aux[3:]};\n");
            else:
                pFile.write(f"{self.__ConvertDecimalToHex(line)}        :  01001000;\n{self.__ConvertDecimalToHex(line+1)}        :  {self.__Adress[instruction[1]]};\n");
            pFile.close();
        elif (instruction[0] == "jmpz"):
            self.__MemoryTypes("01010", instruction[1], instruction[2], file, line);
        elif(instruction[0] == "jmpn"):
            self.__MemoryTypes("01011", instruction[1], instruction[2], file, line);
        elif (instruction[0] == "move"):
            self.__MathTypes("01100", instruction[1], instruction[2], file, line);
        elif (instruction[0] == "push"):
            self.__SimpleType("01101", instruction[1], file, line);
        elif (instruction[0] == "pop"):
            self.__SimpleType("01110", instruction[1], file, line);
        elif (instruction[0] == "call"):
            pFile = open(file, "a+");
            pFile.write(f"{self.__ConvertDecimalToHex(line)}        :  01111000;\n{self.__ConvertDecimalToHex(line+1)}        :  {self.__Adress[instruction[1]]};\n");
            pFile.close();
        elif (instruction[0] == "return"):
            pFile = open(file, "a+");
            pFile.write(f"{self.__ConvertDecimalToHex(line)}        :  10000000;\n{self.__ConvertDecimalToHex(line+1)}        :  00000000;\n");
            pFile.close();
        elif (instruction[0] == "load_c"):
            pFile = open(file, "a+");
            if (int(instruction[2]) < 0):
                aux = self.__ConvertDecimalToBinary(((-1)*int(instruction[2])), 9);
                aux = self.__ComplentoDeDois(aux);
                pFile.write(f"{self.__ConvertDecimalToHex(line)}        :  10011{self.__Register2Bits(instruction[1], file)}{aux[0]};\n{self.__ConvertDecimalToHex(line+1)}        :  {aux[1:]};\n");
            else:
                aux = self.__ConvertDecimalToBinary(int(instruction[2]), 9);
                pFile.write(f"{self.__ConvertDecimalToHex(line)}        :  10011{self.__Register2Bits(instruction[1], file)}{aux[0]};\n{self.__ConvertDecimalToHex(line+1)}        :  {aux[1:]};\n");
            pFile.close();
        elif (instruction[0] == "copytop"):
            self.__SimpleType("10110", instruction[1], file, line);
        elif (instruction[0] == ".data"):
            pFile = open(file, "a+");
            if(int(instruction[2]) < 0):
                aux = self.__ConvertDecimalToBinary(((-1)*int(instruction[2])), (8*int(instruction[1])));
                aux = self.__ComplentoDeDois(aux);
                for x in range(0, len(aux), 8):
                    pFile.write(f"{self.__ConvertDecimalToHex(line)}        :  {aux[x:(x+8)]};\n");
                    line = line + 1;
            else:
                aux = self.__ConvertDecimalToBinary(int(instruction[2]), (8*int(instruction[1])));
                for x in range(0, len(aux), 8):
                    pFile.write(f"{self.__ConvertDecimalToHex(line)}        :  {aux[x:(x+8)]};\n");
                    line = line + 1;
            pFile.close()
        else:
            print(f"\33[7;31mERRO: There isn't this instruction {instruction[0]}\33[m");
            #pFile.close();
            #self.__Fail(file);

    def __MathTypes(self, cody, register1, register2, file, line):
        pFile = open(file, "a+");
        pFile.write(f"{self.__ConvertDecimalToHex(line)}        :  {cody}{self.__Register2Bits(register1, file)}0;\n{self.__ConvertDecimalToHex(line+1)}        :  000000{self.__Register2Bits(register2, file)};\n");
        pFile.close();

    def __MemoryTypes(self, cody, register1, register2, file, line):
        pFile = open(file, "a+");
        if(register2.isdigit()):
            aux = self.__ConvertDecimalToBinary(int(register2), 9);
            pFile.write(f"{self.__ConvertDecimalToHex(line)}        :  {cody}{self.__Register2Bits(register1, file)}{aux[0]};\n{self.__ConvertDecimalToHex(line+1)}        :  {aux[1:]};\n");
        else:
            pFile.write(f"{self.__ConvertDecimalToHex(line)}        :  {cody}{self.__Register2Bits(register1, file)}0;\n{self.__ConvertDecimalToHex(line+1)}        :  {self.__Adress[register2]};\n");
        pFile.close();

    def __SimpleType(self, cody, register, file, line):
        pFile = open(file, "a+");
        pFile.write(f"{self.__ConvertDecimalToHex(line)}        :  {cody}000;\n{self.__ConvertDecimalToHex(line+1)}        :  000000{self.__Register2Bits(register, file)};\n");
        pFile.close();

    def __Register2Bits(self, args, file):
        if (args == "A0"):
            return "00";
        elif (args == "A1"):
            return "01";
        elif (args == "A2"):
            return "10";
        elif (args == "A3"):
            return "11";
        else:
            print(f"\33[7;31mERRO: There isn't the register {args}\33[m");
            self.__Fail(file);

    def __Fail(self, file):
        remove(file);
        exit();

    def __ComplentoDeDois(self, binary):
        for x in range(0, len(binary)):
            if (x == 0):
                if (binary[x] == '1'):
                    aux = '0';
                else:
                    aux = '1';
            else:
                if (binary[x] == '1'):
                    aux = aux + '0';
                else:
                    aux = aux + '1';
        print(aux);
        for x in range((len(binary) - 1), 0, -1):
            if (x == len(binary) - 1):
                if (aux[x] == "1"):
                    binary1 = '0';
                else:
                    binary1 = aux[:len(binary) -1] + '1';
                    break;
            else:
                if (binary[x] == "1"):
                    binary1 = '0' + binary1;
                else:
                    binary1 = binary[:x] + '1' + binary1;
                    break;
        print(binary1);
        return binary1;
