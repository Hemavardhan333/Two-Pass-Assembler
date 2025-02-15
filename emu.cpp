// Name:G.Hema Vardhan Reddy- 2301cs19
#include <bits/stdc++.h>
using namespace std;
// Utility function to convert integer to hexadecimal
string int_to_hex(int i)
{
    char buf[9];  // Buffer to store the hex string (8 characters + null terminator)
    snprintf(buf, sizeof(buf), "%08X", i);  // Format the integer to hex (uppercase)
    return string(buf);  // Return as a string
}


// GLobal variable for registers
int32_t A, B, PC, SP;

// Global memory array
int32_t memory[10000];

// Utility function to retrieve operand and opcode from machine code
tuple<int32_t, int32_t> inst_to_operand_opcode(int32_t instr)
{
	int32_t opcode, operand;
	// masking first 6 bits
	opcode = instr & 0xff;
	// masking last 2 bits
	operand = instr & 0xffffff00;
	// Arithematic shift by 2
	operand >>= 8;
	return(make_tuple(operand, opcode));
}

// Utility function for usage instructions
void prompt() {
    cout<<"Usage: ./emu.exe [option] file.o";
    cout<<endl
         << "Options:" << endl
         << "  -trace     Show instruction trace" << endl
         << "  -before    Show memory dump before execution" << endl
         << "  -after     Show memory dump after execution" << endl
         << "  -isa       Display ISA" << endl;
}
// Utility function for memory dump 
void mem_dump(int poc, ofstream& trcfile)
{
	cout << "Dumping from memory";
	trcfile << "Dumping from memory";
	int i = 0;
	while (i < poc)
	{
		if ((i%4)==0)
		{
            cout<<endl;
        cout << int_to_hex(i) << "\t" << int_to_hex(memory[i]) << " ";
        trcfile << "\n\n" << int_to_hex(i) << "\t" << int_to_hex(memory[i]) << " ";

		}
		else
		{
            cout << int_to_hex(memory[i]) << " ";
			trcfile << int_to_hex(memory[i]) << " ";

		}
		i=i+1;
	}
	cout << endl;
}
void ISA() {
    cout << "Opcode Mnemonic Operand" << endl;
    cout << "       data     value" << endl;
    cout << "0      ldc      value" << endl;
    cout << "1      adc      value" << endl;
    cout << "2      ldl      value" << endl;
    cout << "3      stl      value" << endl;
    cout << "4      ldnl     value" << endl;
    cout << "5      stnl     value" << endl;
    cout << "6      add" << endl;
    cout << "7      sub" << endl;
    cout << "8      shl" << endl;
    cout << "9      shr" << endl;
    cout << "10     adj      value" << endl;
    cout << "11     a2sp" << endl;
    cout << "12     sp2a" << endl;
    cout << "13     call     offset" << endl;
    cout << "14     return" << endl;
    cout << "15     brz      offset" << endl;
    cout << "16     brlz     offset" << endl;
    cout << "17     br       offset" << endl;
    cout << "18     HALT" << endl;
    cout << "       SET      value" << endl;
}


// Mot table
vector<string>Mot(21);
void Mot_init()
{
	Mot[0] = "ldc";
	Mot[1] = string("adc");
	Mot[2] = string("ldl");
	Mot[3] = string("stl");
	Mot[4] = string("ldnl");
	Mot[5] = string("stnl");
	Mot[6] = string("add");
	Mot[7] = string("sub");
	Mot[8] = string("shl");
	Mot[9] = string("shr");
	Mot[10] = string("adj");
	Mot[11] = string("a2sp");
	Mot[12] = string("sp2a");
	Mot[13] = string("call");
	Mot[14] = string("return");
	Mot[15] = string("brz");
	Mot[16] = string("brlz");
	Mot[17] = string("br");
	Mot[18] = string("HALT");
	Mot[19] = string("data");
	Mot[20] = string("SET");
}
int trace(int PC, int poc, ofstream& trcfile)   // Function to trace individual instructions
{
	// Number of executed instructions
	int count = 0;

	// Loop to execute until HALT
for (;;)
{
    // Book keeping for infinite loop
    int old_pc = PC;

    // Int 32 for operand and opcode
    int32_t operand, operation;

    // Instruction at particular PC
    int32_t instr = memory[PC];

    // Retrieve operand and opcode
    tie(operand, operation) = inst_to_operand_opcode(instr);

    // Print statement for tracing
    cout << "PC: " << int_to_hex(PC) << "\tSP: " << int_to_hex(SP) << "\tA: " 
         << int_to_hex(A) << "\tB: " << int_to_hex(B) << "\t" << Mot[operation] 
         << " " << operand;
            cout<<"\n";

    trcfile << "PC: " << int_to_hex(PC) << "\tSP: " << int_to_hex(SP) << "\tA: " 
            << int_to_hex(A) << "\tB: " << int_to_hex(B) << "\t" << Mot[operation] 
            << " " << operand;
            cout<<"\n";
                // Replacing switch-case with if-else statements
    if (operation == 0) {
        B = A;
        A = operand;
    }
    else if (operation == 1) {
        A += operand;
    }
    else if (operation == 2) {
        B = A;
        A = memory[SP + operand];
    }
    else if (operation == 3) {
        memory[SP + operand] = A;
        A = B;
    }
    else if (operation == 4) {
        A = memory[A + operand];
    }
    else if (operation == 5) {
        memory[A + operand] = B;
    }
    else if (operation == 6) {
        A += B;
    }
    else if (operation == 7) {
        A = B - A;
    }
    else if (operation == 8) {
        A = B << A;
    }
    else if (operation == 9) {
        A = B >> A;
    }
    else if (operation == 10) {
        SP += operand;
    }
    else if (operation == 11) {
        SP = A;
        A = B;
    }
    else if (operation == 12) {
        B = A;
        A = SP;
    }
    else if (operation == 13) {
        B = A;
        A = PC;
        PC += operand;
    }
    else if (operation == 14) {
        PC = A;
        A = B;
    }
    else if (operation == 15) {
        if (A == 0)
            PC += operand;
    }
    else if (operation == 16) {
        if (A < 0)
            PC += operand;
    }
    else if (operation == 17) {
        PC += operand;
    }
    else if (operation == 18) {
        cout << count << " number of instructions executed" << endl << endl;
        return 0;
    }

    // Increment PC
    PC=PC+1;

    // Check for infinite loop anomaly
    if (old_pc == PC) {
        cout << "Infinite loop detected" << endl;
        break;
    }

    // Increment executed instruction count
    count=count+1;
}

	// Total number of executed instructions
	cout << count << "number of instructions executed";
    cout<<"\n";
}
int main(int argc, char* argv[])
{
	// Correct input command should be given
	if(argc<3)
	{
	// Print prompt for usage
		prompt();
		return(0);
	}
	// Initialize the machine operand table
	Mot_init();
	// Argument for input file
	string in_file = argv[2];
	// Argument for output file
	string trace_file = in_file.substr(0, in_file.find(".", 0)) + ".trace";

	// Input object file
	ifstream infile;

	// Output trace file
	ofstream trcfile; 

	// Open file
	infile.open(in_file, ios::out | ios::binary);
	trcfile.open(trace_file);

	// 32 bit integer for machine code
	int32_t instr_code;

	// Variable for number of instructions
	int poc = 0;

	// Loop to store mchine code into main memory
for (; infile.read((char*) &instr_code, sizeof(instr_code)); )
{
    int32_t operand, operation;
    tie(operand, operation) = inst_to_operand_opcode(instr_code);
    
    // If instruction is DATA or SET then store the value into memory
if (operation == 19 || operation == 20)
{
    memory[poc++] = operand;
}
if (operation >= 0 and operation < 20 and !(operation == 19 || operation == 20))
{
    memory[poc++] = instr_code;
}
if (operation<0)
{
    cout << "Invalid instruction";
    cout<<"\n";
    return(0);
}
if(operation>20){
    cout << "Invalid instruction";
    cout<<"\n";
    return(0);
}

}

	A = 0;
	B = 0;
	PC = 0;
	SP = sizeof(memory)/sizeof(memory[0])-1;
	
	// Memory dump before execution
	if(string(argv[1]) == "-before")
		mem_dump(poc, trcfile);

	// Tracing for each executed instruction
	if(string(argv[1]) == "-trace")
		trace(0, poc, trcfile);

	// Memory dump after execution
	if(string(argv[1]) == "-after")
	{
		trace(0, poc, trcfile);
		mem_dump(poc, trcfile);
	}

	// Close file
	infile.close();
}