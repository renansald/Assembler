from Assembler import Assembler;


file = str(input("Informe o nome do arquivo que deseja converter: ")).strip();
obj = Assembler(file);
obj.StepOne();
obj.Print();
obj.StepTwo();
