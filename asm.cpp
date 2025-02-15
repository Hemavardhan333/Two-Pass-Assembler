// Name:G.Hema Vardhan Reddy- 2301cs19
#include <bits/stdc++.h>
using namespace std;
#define pa pair<int,int>
#define pb push_back
#define vs vector <string>
#define s(x) x.size()
#define ms map<int,string>
#define ms1 map<string,int>
#define ll long long 
#define f .first
#define H .second
#define z(x,y) substr(x,y)

//Storing the data
struct node{
    string labelname;
    int current_prog;
    int ln_num;
    int opcode;
};
int PCount = 0;                // helps in keeping track of the instructions executed
int line_number = 0;                    // keeps track of the line being executed.
string str;
ms error_table;         // map to store the errors.
ms1 label_table;          // map to store labelnames.
string aux;                       // auxiliary string variables
vs original_file;          // storing the original file.
vector <node>labelCallValue;           // stores the called label names
vector <node>labelCallOffset;          // stores the called label names
vs lf_col1(1000);          // first column of .lst file.
vs lf_col2(1000);          // second column of .lst file.
int numberType;                         // helper variable to determine the type of the number
int hex_max_value = 16777215;           // the maxnumber used of finding hex of negative numbers
unsigned long long int hex_max_val8bit=4294967295;
int len;                                // temporary variable to store the length of the string being read.

string current="Hema Vardhan";
string previous="Hema Vardhan Reddy";
map <string,pa>instruct_table;      // map to store the opcode and operand type for each instruction.

void init()                             //initializing the instruction table with opcodes
{
    //0 means no value or offset
    //1 means value or label
    //2 means offset
    instruct_table["data"]     = {-1, 1};instruct_table["SET"]      = {-2, 1};
    instruct_table["ldc"]      = {0, 1};
    instruct_table["adc"]      = {1, 1};
    instruct_table["ldl"]      = {2, 2};
    instruct_table["stl"]      = {3, 2};
    instruct_table["ldnl"]     = {4, 2};
    instruct_table["stnl"]     = {5, 2};
    instruct_table["add"]      = {6, 0};
    instruct_table["sub"]      = {7, 0};
    instruct_table["shl"]      = {8, 0};
    instruct_table["shr"]      = {9, 0};
    instruct_table["adj"]      = {10, 1};
    instruct_table["a2sp"]     = {11, 0};
    instruct_table["sp2a"]     = {12, 0};
    instruct_table["call"]     = {13, 2};
    instruct_table["return"]   = {14, 0};
    instruct_table["brz"]      = {15, 2};
    instruct_table["brlz"]     = {16, 2};
    instruct_table["br"]       = {17, 2};
    instruct_table["HALT"]     = {18, 0};
    error_table[0] = "incorrect file format";
    error_table[1] = "extra on end of line";
    error_table[2] = "bogus label name";
    error_table[3] = "duplicate label definition";
    error_table[4] = "bogus mnemonic";
    error_table[5] = "improper operand";
    return;
}
string trimExtraSpaces(string line) {  // function to remove extra spaces from each line of code
    stringstream strStream(line);      // string stream to split the line into tokens
    vs words;              // stores individual words from the line
    string word;
    while (strStream >> word)          // add each word to the vector
        words.push_back(word);
    string result = "";
    int wordCount = words.size();
    int index = 0;
    while (index < wordCount) {        // iterate through each word
        result += words[index];
        if (index != wordCount - 1) {
            result += " ";
        }
        index++;
    }
    return result;
}
string stripComment(string line) {    // function to remove comments from each line
    string result = line;             // holds the output string without comments
    int length = line.size();
int idx = 0;
while (idx<length) {
    if (line[idx] == ';') {       // checks for the start of a comment
        result = line.substr(0, idx);
        break;
    }
    idx++;
}
    return result;
}
vs errorLog;                 // storing the errors.
int errorFlags[100005];
void error_in_code(int errorCode,int lineIdx) {  // function to add errors to the error log
    errorFlags[lineIdx]=1;                        // mark this line as having an error
    string errorMsg=error_table[errorCode];
    errorMsg=errorMsg+" on line ";
    errorMsg=errorMsg+to_string(lineIdx + 1);
    errorLog.push_back(errorMsg);                   // append the formatted error message
}

bool isValidLabelName(string label) {    // function to check if the label name is valid
    int length = label.size();
    int index = 0;
    while (index < length) {
        if(label[index]=='_') {
            index++;
            continue;
        }
        if(index==0) { 
            if((label[index]>='A'&&label[index]<='Z')){
                index++;
                continue;
            }
            else if(label[index] >= 'a' && label[index] <= 'z'){
                index++;
                continue;
            }
            return false;
        } else {             // subsequent characters can be letters, digits, or underscores
            if ((label[index] >= 'A' && label[index] <= 'Z')) {
                index++;
                continue;
            }
            else if(label[index] >= 'a' && label[index] <= 'z'){
                index++;
                continue;
            }
            else if (label[index] >= '0' && label[index] <= '9') {
                index++;
                continue;
            }
            return false;
        }
    }
    return true;
}
string opcode_to_hex_string(unsigned ll int num, int bits) //function to convert the opcode number to hex
{
    string ret(bits, '0'); // Create a string of '0's with length 'bits'
    int idx = bits - 1;
    for (; num != 0 && idx >= 0; num /= 16, idx--) 
    {
        int curr = num % 16;
        if (curr >9)
        ret[idx] = (char)(65 + curr - 10);
            
        else
        ret[idx] = (char)(48 + curr);
            
    }
    return ret;
}
bool isDecimal(string numStr) {  // function to check if the string is a decimal number
    int length = numStr.size();
    int idx = 0;
    while (idx < length) {
        if (numStr[idx] >= '0' && numStr[idx] <= '9') {
            idx++;
            continue;
        }
        return false;
    }
    return true;
}
bool isOctal(string numStr) {  // function to check if the string is an octal number
    int length = numStr.size();
    int idx = 0;
    while (idx < length) {
        if (numStr[idx] >= '0' && numStr[idx] <= '7') {
            idx++;
            continue;
        }
        return false;
    }
    return true;
}


bool isHex(string str)                  //function to check if the str is hexadecimal
{
    int llnght=s(str);
    if(llnght==0)
        return false;
int i = 0;
while (i < llnght) 
{
    if ((str[i] >= '0' && str[i] <= '9')) 
    {
        i++; // Increment to move to the next character
        continue;
    }
    else if ((str[i] >= 'A' && str[i] <= 'F')) 
    {
        i++;
        continue;
    }
    else if ((str[i] >= 'a' && str[i] <= 'f')) 
    {
        i++;
        continue;
    }
    
    // If none of the conditions are met, return false
    return false;
}

    return true;
}

bool isValidValue(string str) {           // Function to check if str is a valid value
    int length = s(str);
    int tt = 0;
    if (length == 0) return false;
    if (str[tt] =='-'|| str[tt] == '+') {  // Check for an optional sign
        tt=tt+1;
        if (length == 1) return false;  // A single sign character is not a valid value
    }
    if (str[tt] == '0') {  // Potential octal or hexadecimal
        tt=tt+1;
        if (tt == length) {
            numberType = 1;  // '0' alone is treated as an octal zero
            return true;
        }
        if (str[tt] == 'x' || str[tt] == 'X') {  // Hexadecimal check
            tt=tt+1;  
            string str_hex = str.substr(tt);
            if (isHex(str_hex)) {
                numberType = 2;  // Hexadecimal
                return true;
            }
            return false;
        } 
        else {  // Octal check
            string str_oct = str.substr(tt);
            if (isOctal(str_oct)) {
                numberType = 1;  // Octal
                return true;
            }
            return false;
        }
    }

// Check for decimal if not hexadecimal or octal
if (isDecimal(str.substr(tt))) {
    numberType = 0;  // Decimal
    return true;
}
    return false;
}
bool checkMnemonic(const string str) {  
    // Check if the mnemonic exists in the instruction table
    if (instruct_table.find(str) != instruct_table.end()) {
        return true;  // Valid mnemonic found
    }
    return false;  // Not found in the instruction table, so it is invalid
}


// Function to convert a string representing a number in decimal, octal, or hexadecimal format to an integer
// Parameters:
//    str  - the string representation of the number
//    type - the base of the number system: 10 for decimal, 8 for octal, and 16 for hexadecimal
int DecOctHexToInt(const string& str,int type) {  
    int result = 0;      // Holds the final integer result
    int placeValue = 1;  // Tracks the power of the base to multiply by (1, base^1, base^2, etc.)
    int length = s(str);

    // Loop through each character of the string from right to left
for (int i = 0; i< length; ++i) {
    int currentDigit;

    // Determine the numeric value of the current character
if (isdigit(str[i])) {
    currentDigit = str[i]-'0';  // Converts '0'-'9' to 0-9
} 
else {
    currentDigit = (toupper(str[i]) - 'A') + 10;  // Converts 'A'-'F' or 'a'-'f' to 10-15
}

    // Update result by shifting the previous value by the base and adding the current digit
    result = result * type + currentDigit;
}

    
    return result;  // Return the computed integer value
}
string value_to_opcode(string str, int type, int par = 0)  // Converts the value string `str` to opcode format
{
    int cnt = (str[0] == '-' || str[0] == '+') ? 1 : 0;  // Initialize `cnt` to 1 if there's a sign, otherwise 0
    bool isPositive = (str[0] != '-');  // Determine if the value is positive based on the first character
    
    int length = s(str);
    
    // Check if the value is a standalone '0' (with sign) and return default opcode for zero
    if (cnt == 1 && str[1] == '0' && length==2) {
        return "000000";
    }
    
    cnt=cnt+type;  // Adjust `cnt` based on the number system type (decimal, octal, or hex)
    string val_str = str.substr(cnt, length - cnt);  // Extract the portion of `str` containing the numeric value
    unsigned long long int val = 0;

    // Determine numeric conversion based on the base type
    switch (type) {
        case 0: val = DecOctHexToInt(val_str, 10); break;  // Convert decimal value
        case 1: val = DecOctHexToInt(val_str, 8);  break;  // Convert octal value
        case 2: val = DecOctHexToInt(val_str, 16); break;  // Convert hexadecimal value
    }

    // If `par` is 1, handle conversion as 8-bit hex value with optional negativity adjustment
    if (par == 1) {
        if (!isPositive) {
            val = hex_max_val8bit - val + 1;  // Adjust for negative values in 8-bit hex range
        }
        return opcode_to_hex_string(val, 8);  // Return the 8-bit opcode string
    }
    
    // Handle default conversion for non-positive values in general cases
    if (!isPositive) {
        val = hex_max_value - val + 1;  // Adjust for negativity in default hex range
    }
    
    return opcode_to_hex_string(val, 6);  // Return the 6-bit opcode string for other cases
}
void Token_2(vs &str_tok)  // Function to check instructions if number of tokens are 2
{
    string mnemonic = str_tok[0];
    if (!checkMnemonic(mnemonic)) {
        lf_col2[line_number] = "        ";
        error_in_code(4, line_number);
        current = previous;
        return;
    }

    int opcode_mnemonic = instruct_table[mnemonic].first;
    string operand = str_tok[1];

    if (opcode_mnemonic == -1 || opcode_mnemonic == -2) {
        if (!isValidValue(operand)) {
            error_in_code(5, line_number);
            current = previous;
            return;
        }
        
        string set_opc = value_to_opcode(operand, numberType, 1);
        lf_col2[line_number] = set_opc;

        if (opcode_mnemonic == -2 && current != previous) {
            label_table[current] = DecOctHexToInt(set_opc, 16);
        }
    } 
    else{
        string suff_opcode = opcode_to_hex_string(opcode_mnemonic, 2);
        int num_of_operands = instruct_table[mnemonic].second;

        if(num_of_operands == 0) {
            error_in_code(5, line_number);
            lf_col2[line_number] = "        ";
        } 
        else{
            if(!isValidValue(operand)) {
                if(num_of_operands == 1) {
                    labelCallValue.push_back({operand, PCount, line_number, opcode_mnemonic});
                }
                 else{
                    labelCallOffset.push_back({operand, PCount, line_number, opcode_mnemonic});
                }
            } 
            else{
                string pref_opcode = value_to_opcode(operand, numberType);
                lf_col2[line_number] = pref_opcode + suff_opcode;
            }
        }
    }
    current=previous;
}
void check_instruc(string str)  // Function to check and process instructions
{
    vs str_tok;
    int tokens = 0;
    
    // Using a for loop to split the string into tokens
    stringstream ss(str);
    for (string aux; ss >> aux; tokens++) {
        str_tok.push_back(aux);
    }
    
    if (tokens > 2) {
        PCount++;
        error_in_code(1, line_number);
        lf_col2[line_number] = "        ";
    } 
    else if (tokens == 2){
        PCount++;
        Token_2(str_tok);
    } 
    else if (tokens == 1){
        PCount++;
        string mnemonic = str_tok[0];
        
        if (!checkMnemonic(mnemonic)){
            lf_col2[line_number] = "        ";
            error_in_code(4, line_number);
            current = previous;
            return;
        }


        int opcode_mnemonic = instruct_table[mnemonic]f;
        string suff_opcode = opcode_to_hex_string(opcode_mnemonic, 2);
        int num_of_operands = instruct_table[mnemonic]H;
        if (num_of_operands == 0) {
            lf_col2[line_number] = "000000";
            lf_col2[line_number] =lf_col2[line_number] +suff_opcode;
        } 
        else {
            lf_col2[line_number] = "        ";
            error_in_code(5, line_number);
        }
    } 
    else{
        lf_col2[line_number] = "        ";
    }

    current = previous;
}
void Pass1(char** argv) // Pass 1 of the assembler
{
    string tmp = argv[1];
    int lnght = s(tmp);
    string fname = tmp.substr(0, lnght - 4);
    string inp_file = fname + ".asm";
    ifstream input(inp_file);
    current = previous;

    for (line_number = 0; getline(input, str); line_number++) 
    {
        original_file.pb(str); // Store the original asm code if required for further use.
        len = s(str);
        str = stripComment(str); // Removes comments from each line.
        str = trimExtraSpaces(str); // Removes unnecessary spaces in between.
        len = s(str);

        if (len == 0) // Empty line
        {
            lf_col1[line_number] = lf_col2[line_number] = "        ";
            continue;
        }

        int colon_idx = str.find(':'); // Use string::find to get first occurrence of colon

        string pc = opcode_to_hex_string(PCount, 8); // Compute program counter string

        if (colon_idx == string::npos) // No colon found
        {
            lf_col1[line_number] = pc;
            check_instruc(str);
        }
else // Colon found, indicating a label definition
{
    lf_col1[line_number] = pc;
    string label = str.substr(0, colon_idx);

    // Validate the label name and ensure it is unique
    if (label_table.count(label))  // Check if the label is already defined
    {
        error_in_code(3, line_number);  // Report a duplicate label error
    } 
    else if (isValidLabelName(label)) 
    {
         // If valid and unique, set the current label and store it in the label table
        current = label;
        label_table[label] = PCount;
    } 
    else 
    {
        error_in_code(2, line_number);  // Report an invalid label name error
    }

    // Extract the instruction part after the label and validate it
    string instruc = str.z(colon_idx + 1, len - colon_idx - 1);
    check_instruc(instruc);
}

    }
}


void writeObjFile(string fname)         // Writing into the object file
{
    string obj_file = fname + ".o";
    unsigned int machine_code_int[line_number]; // Object file contents
    int idx = 0;
    int i = 0; // Initialize index for the `while` loop

    // Replace `for` loop with `while` loop
    while (i < line_number) 
    {
        if (lf_col2[i][0] != ' ') 
        {
            machine_code_int[idx++] = DecOctHexToInt(lf_col2[i], 16);
        }
        i++; // Increment `i` in each iteration
    }

    FILE* obj_fptr;
    obj_fptr = fopen(obj_file.c_str(), "wb");
    fwrite(machine_code_int, sizeof(int), idx, obj_fptr);
}
void Pass2(char** argv)         // Pass 2 of the assembler
{
    // Process label call offsets
    for (size_t i = 0; i < labelCallOffset.size(); ++i) 
    {
        auto& it = labelCallOffset[i];
        bool labelExists = label_table.find(it.labelname) != label_table.end();
        if (!labelExists) 
        {
            error_in_code(2, it.ln_num);
            continue;
        }
        
        int address = label_table[it.labelname] - it.current_prog;
        address = (address < 0) ? (hex_max_value + address + 1) : address;
        lf_col2[it.ln_num] = opcode_to_hex_string(address, 6) + opcode_to_hex_string(it.opcode, 2);
    }

    // Process label call values
    for (size_t i=0;i<s(labelCallValue); ++i) 
    {
        auto& it = labelCallValue[i];
        bool labelExists = label_table.find(it.labelname) != label_table.end();
        if (!labelExists) 
        {
            error_in_code(2, it.ln_num);
            continue;
        }

        int address = label_table[it.labelname];
        address = (address < 0) ? (hex_max_value + address + 1) : address;
        lf_col2[it.ln_num] = opcode_to_hex_string(address, 6);
         lf_col2[it.ln_num]=lf_col2[it.ln_num]+opcode_to_hex_string(it.opcode, 2);
    }

    // Generate error file
    string tmp = argv[1];
    int lnght = s(tmp);
    string fname = tmp.substr(0, lnght - 4);
    string error_file = fname + ".log";
    ofstream outfile;
    outfile.open(error_file);

    // Check if there are errors
    bool errFlag = !errorLog.empty();
    if (!errFlag)
        outfile << "Compiled without any errors" << endl;
    else
        cout << "error messages:\n";
    
    for (size_t i = 0; i < errorLog.size(); ++i) 
    {
        outfile << "ERROR: " << errorLog[i] << endl;
        cout << "ERROR: " << errorLog[i] << endl;
    }
    outfile.close();

    outfile.close();

    // Generate listing file
    string list_file = fname + ".lst";
    outfile.open(list_file);

    int i=0; // Initialize the loop variable for `while`
    while (i<line_number) 
    {
        if (!errorFlags[i])  // Only write lines without errors
        {
            string out=lf_col1[i]+ " ";
            out=out+lf_col2[i]+" ";
            out=out+original_file[i];
            outfile<<out<<endl;
        }
        i++; // Increment `i` within the loop
    }

    outfile.close();
    // Write object file if there were no errors
    if (!errFlag)
        writeObjFile(fname);


    // Write object file if there were no errors
    if (!errFlag)
        writeObjFile(fname);
}
int main(int argc, char** argv)         //Main function of the program
{ 
    init();                 // filling the map with opcodes for instructions.
    string tmpp=argv[1];
    string fext="";
    ll int idx=0;
    ll int ln=s(tmpp);
    int i = 0;
    while(i<ln) 
    {
        if(tmpp[i]=='.') 
        {
            idx=i;
            break;
        }
        i++; // Increment `i` within the loop
    }
    for(int i=idx+1;i<ln;i++)
        fext=fext+tmpp[i];
    if(fext=="asm")
    {
    Pass1(argv);            // Pass 1 of the assembler
    Pass2(argv);            // Pass 2 of the assembler
    }
    else{
                cout<<"Error:Given file format is not supported\n";
        return 0;
    }
    return 0;
}