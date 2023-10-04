#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
using namespace std;

void editTable(vector<vector<string> > &table);
void saveTable(vector<vector<string> > &table);
bool isValidHex(string input);
void printTable(const vector<vector<string> > &table, string base16);
string concatenateRow(const vector<string> &row);
int getSumRow(const vector<string> &row);
string getCheckSum(int totalSum);
void createFileWithCustomExtension(const string &filename,vector<vector<string> > &table);
int getTotalSumRow(const vector<vector<string> > &table, string base16, string dataType, int index);
int hexToInt(const string &data);
string intToHex(int intValue);

int main()
{
    string base16 = "10";
    string dataType = "00";
    vector<vector<string> > table;
    int menuChoice;

    // Initialize the table with rows and columns
    for (int i = 0; i < 16; i++)
    {
        vector<string> row;
        for (int j = 0; j < 17; j++)
        {
            if(i==15 && j==15)
                row.push_back("01");
            else
                row.push_back("00");
        }
        table.push_back(row);
    }

    while (true)
    {
        cout<<"Sanchez Velazquez Amilcar Z. 4CM23"<<endl<<endl;
        cout << "Bienvenido al generador Checksum" << endl<<endl;
        cout << "Menu" << endl<<endl
             << "1.Editar o asignar valores a la tabla" << endl<<endl
             << "2.Ver tabla" << endl<<endl
             << "3.Exportar archivo" << endl<<endl;
        cout << "Selecciona una opcion para continuar: ";
        cin >> menuChoice;
        switch (menuChoice)
        {
        case 1:
            editTable(table); // Pass table by reference
            break;
        case 2:
            printTable(table, base16);
            break;
        case 3:
            saveTable(table);
            break;
        default:
            cout << "No pudimos procesar su respuesta" << endl;
            break;
        }
    }
    return 0;
}

int getSumRow(const vector<string> &row)
{
    int totalSum = 0;
    for (int i = 0; i < row.size() - 1; i++)
    {
        totalSum += hexToInt(row[i]);
    }
    return totalSum;
}

string getCheckSum(int totalSum)
{
    int checkSumValue = (256 - (totalSum % 256)); // Calculate checksum value
    return intToHex(checkSumValue);
}

int getTotalSumRow(const vector<vector<string> > &table, string base16, string dataType, int index)
{
    int totalSum = 0;
    string valueDirection = intToHex(index);
    totalSum += hexToInt(base16);
    totalSum += hexToInt(dataType);
    totalSum += hexToInt(valueDirection + "0");
    totalSum += getSumRow(table[index]);
    return totalSum;
}

void editTable(vector<vector<string> > &table)
{
    int choiceMenuDir, byteDir;
    string choiceByteDir, contentDir;
    system("clear");
    system("cls");
    cout << "Direcciones de memoria" << endl;
    cout << "1. 0000" << endl;
    cout << "2. 0010" << endl;
    cout << "3. 0020" << endl;
    cout << "4. 0030" << endl;
    cout << "5. 0040" << endl;
    cout << "6. 0050" << endl;
    cout << "7. 0060" << endl;
    cout << "8. 0070" << endl;
    cout << "9. 0080" << endl;
    cout << "10. 0090" << endl;
    cout << "11. 00A0" << endl;
    cout << "12. 00B0" << endl;
    cout << "13. 00C0" << endl;
    cout << "14. 00D0" << endl;
    cout << "15. 00E0" << endl;
    cout << "16. 00F0" << endl;
    cout << "Seleccione una direccion de memoria: ";
    cin >> choiceMenuDir;
    cout << "Ingrese el byte de la direccion entre 0-F: Byte";
    cin >> choiceByteDir;
    cout << "Ingrese el contenido en hexadecimal valido a insertar: ";
    cin >> contentDir;

    if (isValidHex(contentDir)){
    byteDir = hexToInt(choiceByteDir);

    table[choiceMenuDir - 1][byteDir] = contentDir;

    // Update the checksum for the modified row
    for (int i = 0; i < 16; i++)
    {
        int totalSum = getTotalSumRow(table, "10", "00", i);
        table[i][16] = getCheckSum(totalSum);
    }

    printTable(table, "10");}
    else{
        cout << "Lo siento no es un digito valido"<<endl;
    }
    cout << endl;
}

void printTable(const vector<vector<string> > &table, string byte16)
{
    cout << "DT DIR ";
    for (size_t i = 0; i < table.size(); ++i)
    {
        cout << "BY" << intToHex(i) << " ";
    }
    cout << "CKSM";
    cout << endl;
    for (size_t i = 0; i < table.size(); ++i)
    {
        cout << byte16 << " " <<"0"+intToHex(i) + "0"
             << "  ";
        for (size_t j = 0; j < table[i].size(); ++j)
        {
            cout <<" "+table[i][j] << "  ";
        }
        cout << endl;
    }
}

int hexToInt(const string &data)
{
    char *endptr; // Used to check for conversion errors
    unsigned long decimalValue = strtoul(data.c_str(), &endptr, 16);

    if (*endptr != '\0')
    {
        cerr << "Oh! Error: valor Hex incorrecto: " << data << endl;
        return 0; // You can handle the error as needed
    }

    return static_cast<int>(decimalValue);
}

string intToHex(int intValue) {
    ostringstream ss;
    ss << hex << setw(2) << setfill('0') << intValue;
    return ss.str();
}

bool isValidHex(string input) {
    if (input.empty() || input.length() % 2 != 0) {
        return false;
    }
    for (int i = 0; i<input.size();i++) {
       
        if (!((input[i] >= '0' && input[i] <= '9') || (input[i] >= 'A' && input[i] <= 'F') || (input[i] >= 'a' && input[i] <= 'f'))) {
            return false;
        }
    }

    return true;
}


string concatenateRow(const vector<string> &row)
{
    string totalString;
    for (int i = 0; i < row.size(); i++)
    {
        totalString += row[i];
    }
    return totalString;
}

void createFileWithCustomExtension(const string &filename, vector<vector<string> > &table)
{
    string fullFilename = filename + ".HEX";

    // Open the file for writing
    ofstream outputFile(fullFilename.c_str());

    if (outputFile.is_open())
    {
        // Write data to the file if needed
        outputFile << ":020000040000FA" << endl;
        for (int i = 0; i < 16; i++)
        {
            outputFile << ":" << "1" << "00" + intToHex(i) << "000" << concatenateRow(table[i]) << endl;
        }

        outputFile << ":00000001FF";
        // Close the file
        outputFile.close();

        cout << "Archivo '" << fullFilename << "' creado correctamente." << endl<<endl;
    }
    else
    {
        cout << "Error al crear el archivo." << endl;
    }
}

void saveTable(vector<vector<string> > &table)
{
    string filename;
    cout << "El archivo sera guardado con la extension .HEX" << endl;
    cout << "Guardar archivo como: ";
    cin >> filename;
    createFileWithCustomExtension(filename, table);
}
