#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

void ShowMainMenu();
void ShowQuickWithdrawScreen();
void ShowNormalWithdrawScreen();
void Login();

const string ClientsFileName = "ClientsFile.txt";

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;

    bool MarkForDelete = false;
};

enum enMainMenuOptions { eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3, eCheckBalance = 4, eExit = 5 };

sClient CurrentClient;

vector<string> SplitString(string Text, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord; // define a string variable

    // use find() function to get the position of delimiters
    while ((pos = Text.find(Delim)) != std::string::npos)
    {
        sWord = Text.substr(0, pos); // store the word
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        Text.erase(0, pos + Delim.length()); // erase() until position and move to next word
    }

    if (Text != "")
    {
        vString.push_back(Text); // it prints last word of the string
    }

    return vString;
}

sClient ConvertLineToRecord(string Record, string Delim)
{
    sClient Client;

    vector<string> vClientData = SplitString(Record, Delim);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

string ConvertRecordToLine(sClient Client, string Delim = "###")
{
    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Delim;
    stClientRecord += Client.PinCode + Delim;
    stClientRecord += Client.Name + Delim;
    stClientRecord += Client.Phone + Delim;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;
}

vector<sClient> LoadClientDataFromFile(string FileName)
{
    vector<sClient> vClients;

    fstream File;
    File.open(FileName, ios::in); //read mode

    if (File.is_open())
    {
        string Line;
        sClient Client;

        while (getline(File, Line))
        {
            Client = ConvertLineToRecord(Line, "/-/");
            vClients.push_back(Client);
        }

        File.close();
    }

    return vClients;
}

void SaveClientsDataToFile(string FileName, vector<sClient> vClients)
{
 //he used vector<sClient> istead of void

    fstream File;
    File.open(FileName, ios::out); //overwrite

    if (File.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete
                File << ConvertRecordToLine(C, "/-/") << endl;
            }
        }

        File.close();
    }

    // return vClients;
}

bool DepositBalanceByAccountNumber(string AccountNumber, double Amount, vector<sClient>& vClients)
{
    char Answer = 'N';

    cout << "\n\nAre you sure you want to perform this transaction? [Y/N]: ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New Balance is: " << C.AccountBalance;

                CurrentClient.AccountBalance += Amount;

                return true;
            }
        }
    }
    else
    {
        return false;
    }
}


void PrintClientBalance(sClient Client)
{
    cout << "Your Balance is: " << Client.AccountBalance << endl;
}

short ReadQuickWithdrawOption()
{
    short Choice = 0;
    do
    {
        cout << "\nChoose what to do from [1] to [9] ? ";
        cin >> Choice;

    } while (Choice < 1 || Choice > 9);

    return Choice;
}

short getQuickWithdrawAmount(short QuickWithdrawChoice)
{
    switch (QuickWithdrawChoice)
    {
    case 1:
        return 20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;
    default:
        return 0;
    }
}

void PerformQuickWithdrawOption(short QuickWithdrawChoice)
{
    if (QuickWithdrawChoice == 9) //exit
        return;

    short QuickWithdrawAmount = getQuickWithdrawAmount(QuickWithdrawChoice);

    if (QuickWithdrawAmount > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press any key to continue...\n";
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    }

    vector<sClient> vClients = LoadClientDataFromFile(ClientsFileName);
    DepositBalanceByAccountNumber(CurrentClient.AccountNumber, QuickWithdrawAmount * -1 , vClients);
}

void ShowQuickWithdrawScreen()
{
    system("cls");
    cout << "--------------------------------------------\n";
    cout << "\t\tQuick Withdraw\n";
    cout << "--------------------------------------------\n";

    cout << "\t[1] 20  \t[2] 50\n";
    cout << "\t[3] 100 \t[4] 200\n";
    cout << "\t[5] 400 \t[6] 600\n";
    cout << "\t[7] 800 \t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "--------------------------------------------\n";

    PrintClientBalance(CurrentClient);

    PerformQuickWithdrawOption(ReadQuickWithdrawOption());
}

int ReadWithdrawAmount()
{
    int Amount = 0;
    do
    {
        cout << "Enter an amount multiple of 5's : ";
        cin >> Amount;

    } while (Amount % 5 != 0);

    return Amount;
}

void PerformNormalWithdraw()
{
    int WithdrawAmount = ReadWithdrawAmount();

    if (WithdrawAmount > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press any key to continue...\n";
        system("pause>0");
        ShowNormalWithdrawScreen();
        return;
    }

    vector<sClient> vClients = LoadClientDataFromFile(ClientsFileName);
    DepositBalanceByAccountNumber(CurrentClient.AccountNumber, WithdrawAmount * -1, vClients);
}

void ShowNormalWithdrawScreen()
{
    system("cls");
    cout << "--------------------------------------------\n";
    cout << "\t\tNormal Withdraw\n";
    cout << "--------------------------------------------\n";

    PerformNormalWithdraw();
}

double ReadDepositAmount()
{
    double Amount = 0;
    do
    {
        cout << "Please enter a positive deposit amount : ";
        cin >> Amount;

    } while (Amount <= 0);

    return Amount;
}

void PerformDeposit()
{
    double DepositAmount = ReadDepositAmount();

    vector<sClient> vClients = LoadClientDataFromFile(ClientsFileName);
    DepositBalanceByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients);
}

void ShowDepositScreen()
{
    system("cls");
    cout << "--------------------------------------------\n";
    cout << "\t\tDeposit\n";
    cout << "--------------------------------------------\n";

    PerformDeposit();
}

void ShowCheckBalanceScreen()
{
    system("cls");
    cout << "--------------------------------------------\n";
    cout << "\t\tCheck Balance\n";
    cout << "--------------------------------------------\n";

    PrintClientBalance(CurrentClient);
}

enMainMenuOptions ReadMainMenuOption()
{
    short Choice = 0;
    do
    {
        cout << "Choose what do you want to do? [1 to 5]: ";
        cin >> Choice;

    } while (Choice < 1 || Choice > 5);

    return (enMainMenuOptions)Choice;
}

void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to Main Menu... ";
    system("pause>0");
    ShowMainMenu();
}

void PerformMainMenuOption(enMainMenuOptions MainMenuOption)
{
    switch (MainMenuOption)
    {
    case::enMainMenuOptions::eQuickWithdraw:
    {
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToMainMenu();
        break;
    }
    case::enMainMenuOptions::eNormalWithdraw:
    {
        system("cls");
        ShowNormalWithdrawScreen();
        GoBackToMainMenu();
        break;
    }
    case::enMainMenuOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenu();
        break;
    }
    case::enMainMenuOptions::eCheckBalance:
    {
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToMainMenu();
        break;
    }
    case::enMainMenuOptions::eExit:
    {
        system("cls");
        Login();
        break;
    }
    }
}

void ShowMainMenu()
{
    system("cls");
    cout << "==============================================\n";
    cout << "\t\tATM Main Menu Screen\n";
    cout << "==============================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "==============================================\n";

    PerformMainMenuOption(ReadMainMenuOption());
}

bool IsCorrectPassword(string AccountNumber, string PinCode, sClient& Client)
{
    vector<sClient> vClients = LoadClientDataFromFile(ClientsFileName);

    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }

    return false;
}

bool CheckUserAndPassword(string AccountNumber, string PinCode)
{
    if (IsCorrectPassword(AccountNumber, PinCode, CurrentClient))
    {
        return true;
    }

    return false;
}

void Login()
{
    string AccountNumber, PinCode;
    bool LoginFailed = false;

    do
    {
        system("cls");

        cout << "--------------------------------------------\n";
        cout << "\t\tLogin Screen\n";
        cout << "--------------------------------------------\n";

        if (LoginFailed)
            cout << "Invalid AccountNumber/PinCode!\n";

        cout << "Please enter Account Number: ";
        cin >> AccountNumber;
        cout << "Please enter Pin Code: ";
        cin >> PinCode;

        LoginFailed = !CheckUserAndPassword(AccountNumber, PinCode);

    } while (LoginFailed);

    ShowMainMenu();
}

int main() {

    Login();


    // system("pause > 0");

    return 0;
}
