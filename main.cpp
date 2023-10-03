#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;

void editTable(vector<vector<string> > &table);
void saveTable(vector<vector<string> > &table);
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
    cout << "Checksum" << endl;
    int menuChoice;

    // Initialize the table with rows and columns
    for (int i = 0; i < 16; i++)
    {
        vector<string> row;
        for (int j = 0; j < 17; j++)
        {
            row.push_back("00");
        }
        table.push_back(row);
    }

    while (true)
    {
        cout<<"Sanchez Velazquez A. Z. 4CM23"<<endl;
        cout << "Bienvenido al generador Checksum" << endl;
        cout << "Menu" << endl
             << "1.Asignar y/o editar valores a la tabla" << endl
             << "2.Ver tabla" << endl
             << "3.Generar archivo" << endl;
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
    cout << "Direcciones" << endl;
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
    cout << "Seleccione una direccion: ";
    cin >> choiceMenuDir;
    cout << "Ingrese el byte de la direccion entre 0-F: Byte";
    cin >> choiceByteDir;
    cout << "Ingrese el contenido en hex a insertar: ";
    cin >> contentDir;
    byteDir = hexToInt(choiceByteDir);

    table[choiceMenuDir - 1][byteDir] = contentDir;

    // Update the checksum for the modified row
    for (int i = 0; i < 16; i++)
    {
        int totalSum = getTotalSumRow(table, "10", "00", i);
        table[i][16] = getCheckSum(totalSum);
    }

    printTable(table, "10");
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
        cout << byte16 << " " << intToHex(i) + "0"
             << "  ";
        for (size_t j = 0; j < table[i].size(); ++j)
        {
            cout << table[i][j] << "  ";
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
        std::cerr << "Oh! Error: Invalid hexadecimal string: " << data << std::endl;
        return 0; // You can handle the error as needed
    }

    return static_cast<int>(decimalValue);
}

string intToHex(int intValue)
{
    ostringstream ss;
    ss << hex << intValue;
    return ss.str();
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
            outputFile << ":" << "10" << "00" + intToHex(i) << "00" << concatenateRow(table[i]) << endl;
        }

        outputFile << ":00000001FF" << endl;
        // Close the file
        outputFile.close();

        cout << "Archivo '" << fullFilename << "' creado correctamente." << endl;
    }
    else
    {
        cout << "Error al crear el archivo." << endl;
    }
}

void saveTable(vector<vector<string> > &table)
{
    string filename;
    cout << "El archivo sera del tipo .HEX" << endl;
    cout << "Guardar archivo como: ";
    cin >> filename;
    createFileWithCustomExtension(filename, table);
}