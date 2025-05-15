#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

const string MyClientsFile = "MyClients.txt";//Global Variable
void PlayProject();
void TransacitonScreen();





struct stClientData {

	string Name, AccountNumber = "", PIN, PhoneNumber = "";
	double  Balance = 0;
	bool MarkForDelete = false;

};

enum enMainMenuScreen {
	eShowClientsList = 1, eAddNew = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eTransactions = 6, eExit = 7
};

enum enTransactionsScreen {
	eDeposit = 1, eWithdraw = 2, eTotalBalances = 3, eBackToMainMenue = 4
};
vector <string> SpliteEachWordInString(string S1, string Delimeter) {

	vector <string> vString;

	short Position = 0;
	string Word;


	while ((Position = S1.find(Delimeter)) != string::npos) {

		Word = S1.substr(0, Position);

		if (Word != "") {
			vString.push_back(Word);
		}

		S1.erase(0, Position + Delimeter.length());
	}

	if (S1 != "") {
		vString.push_back(S1);
	}

	return vString;
}

stClientData ConvertLineToRecord(string Line) {

	vector <string> ClientData;
	stClientData Client;
	ClientData = SpliteEachWordInString(Line, "#//#");

	Client.AccountNumber = ClientData[0];
	Client.PIN = ClientData[1];
	Client.Name = ClientData[2];
	Client.PhoneNumber = ClientData[3];
	Client.Balance = stod(ClientData[4]);


	return Client;
}

string ConvertRecordToLine(stClientData Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PIN + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.PhoneNumber + Seperator;
	stClientRecord += to_string(Client.Balance);
	return stClientRecord;
}

vector <stClientData> LoadFileDataToVector(string FileName) {

	fstream MyFile;
	vector <stClientData> ClientData;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {

		string Line;
		stClientData Client;

		while (getline(MyFile, Line)) {

			Client = ConvertLineToRecord(Line);
			ClientData.push_back(Client);
		}

		MyFile.close();
	}

	return ClientData;
}

vector <stClientData> LoadVectorDataToFile(vector <stClientData>& vClients, string FileName) {

	string Line;

	fstream MyFile;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open()) {

		for (stClientData& Client : vClients) {

			if (Client.MarkForDelete == false) {

				Line = ConvertRecordToLine(Client, "#//#");
				MyFile << Line << endl;
			}
		}

		MyFile.close();
	}

	return vClients;
}


void ShowClientCard(stClientData Client) {


	cout << "\n_______________________________________________\n";
	cout << "\n Client Deteals : \n";
	cout << " Account Number : " << Client.AccountNumber << endl;
	cout << " PIN :            " << Client.PIN << endl;
	cout << " Name :           " << Client.Name << endl;
	cout << " Phone :          " << Client.PhoneNumber << endl;
	cout << " Balcnce :        " << Client.Balance << endl;
	cout << "_______________________________________________\n";

}

// Pause Fucniton 
void GoBackToMainMenue() {

	cout << "\n\nPress Any Key to Go back To Main Menue ...";
	system("pause>0");

	PlayProject();
}
// Updating Client Data :

string ReadAccountNumberFromUser() {
	string Num = "";

	cout << "\n\nPlease enter the account number that u want ?\n";
	cin >> Num;

	return Num;
}

bool FindClientByAccountNumber(vector <stClientData>& FileData, string Target, stClientData& Client) {

	for (stClientData& C : FileData) {

		if (C.AccountNumber == Target) {

			Client = C;
			return true;
		}
	}

	return false;
}


stClientData UpdateClientData(string AccountNumber) {

	stClientData Client;
	Client.AccountNumber = AccountNumber;

	cout << "\nPIN Code :";
	getline(cin >> ws, Client.PIN);
	cout << "\nName :";
	getline(cin, Client.Name);
	cout << "\nPhone Number :";
	getline(cin, Client.PhoneNumber);
	cout << "\nBalancy :";
	cin >> Client.Balance;


	return Client;
}

bool MainUpdatingClientData() {

	cout << "\n===================================\n";
	cout << "\tUpdating Client Info Screen";
	cout << "\n===================================\n";

	vector <stClientData> FileData = LoadFileDataToVector(MyClientsFile);

	stClientData Client;
	string Target = ReadAccountNumberFromUser();

	if (FindClientByAccountNumber(FileData, Target, Client))
	{

		for (stClientData& C : FileData) {

			if (C.AccountNumber == Target) {
				C = UpdateClientData(Target);
				break;
			}
		}
		LoadVectorDataToFile(FileData, MyClientsFile);

		cout << "\nClient Data Updated Successfully !! \n";
		return true;
	}
	else
	{
		cout << "\n This Account Number is not Found !! \n";
	}
}
//Show All Clients  :

void ShowClientDataInTable(stClientData Client) {

	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PIN;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.PhoneNumber;
	cout << "| " << setw(12) << left << Client.Balance;

}

void ShowAllClinetsData() {

	vector <stClientData> vFileData = LoadFileDataToVector(MyClientsFile);

	cout << "\n\t\t\t\t\tClient List (" << vFileData.size() << ")Client(s).";

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	for (stClientData& Client : vFileData) {

		ShowClientDataInTable(Client);
		cout << endl;
	}
}


// Adding New Client :

stClientData ReadClinetData(vector <stClientData> FileData) {

	stClientData Info;

	cout << "\nPlease Enter Cilent Data : \n\n";

	cout << "Enter Account Number : ";
	getline(cin >> ws, Info.AccountNumber); //ws > to clear white spaces


	while (FindClientByAccountNumber(FileData, Info.AccountNumber, Info)) // checking if we already have the number
	{

		cout << "The Account Number is already exists , Please enter another one : ";
		getline(cin >> ws, Info.AccountNumber);
	}


	cout << "Enter PIN Code : ";
	getline(cin, Info.PIN);

	cout << "Enter Name : ";
	getline(cin, Info.Name);

	cout << "Enter Phone Number : ";
	getline(cin, Info.PhoneNumber);

	cout << "Enter Account Balance : ";
	cin >> Info.Balance;

	return Info;
}

void AddNewClient(vector <stClientData>& ProjectData) {


	stClientData Client;

	Client = ReadClinetData(ProjectData);
	ProjectData.push_back(Client);

	LoadVectorDataToFile(ProjectData, MyClientsFile);
}
void AddMoreClients() {

	cout << "\n===================================\n";
	cout << "\tAdding New Client Screen";
	cout << "\n===================================\n";

	vector <stClientData> FileData = LoadFileDataToVector(MyClientsFile);

	char AddMore = 'Y';


	do {

		AddNewClient(FileData);

		cout << "\n*** Data Are Saved Succesfully, Do you want to enter more Clients ?? Y/N.";
		cin >> AddMore;



	} while (toupper(AddMore) == 'Y');
}

//Deleting Client : 

bool MarkClientForDelete(vector <stClientData>& FileData, string Target) {


	for (stClientData& C : FileData) {

		if (C.AccountNumber == Target) {

			C.MarkForDelete = true;
			return true;
		}
	}

	return false;
}

bool DeleteClientByAccountNumber() {

	cout << "\n===================================\n";
	cout << "\tDeletig Client Screen";
	cout << "\n===================================\n";


	vector <stClientData> FileData = LoadFileDataToVector(MyClientsFile);

	stClientData Client;

	string Target = ReadAccountNumberFromUser();

	FileData = LoadFileDataToVector(MyClientsFile);

	if (FindClientByAccountNumber(FileData, Target, Client)) {

		ShowClientCard(Client);

		MarkClientForDelete(FileData, Target);
		LoadVectorDataToFile(FileData, MyClientsFile);

		FileData = LoadFileDataToVector(MyClientsFile);// refresh Data

		cout << "\n\n Client is Deleted Succesfully. \n\n";

		return true;
	}
	else {
		cout << "\n\nSorry, I Did not find the account number [" << Target << "] :-(\n";
		return false;
	}
}

// Find Spicific Client :

void FindSpicificClient() {

	cout << "\n===================================\n";
	cout << "\tFinding Client Screen";
	cout << "\n===================================\n";

	vector <stClientData> FileData = LoadFileDataToVector(MyClientsFile);

	string Target = ReadAccountNumberFromUser();
	stClientData Client;

	if (FindClientByAccountNumber(FileData, Target, Client)) {
		ShowClientCard(Client);
	}
	else {
		cout << "\n\nSorry, I Did not find the account number [" << Target << "] :-(\n";
	}
}
// Exit Screen
void ExitScreen() {

	cout << "\n=================================\n";
	cout << "*** The Program is Ended ***";
	cout << "\n\=================================\n";

}

// New Extention , Transactions Screen :

void GoBackToTransactionsScreen() {

	cout << "\n\nPress any key to go to Transactions Screen ..." << endl;
	system("pause>0");

	TransacitonScreen();
}

// Deposit :
double AmountOfOperation() {

	double Num = 0;
	cout << "Please Enter the Amount ? ";
	cin >> Num;
	return Num;
}

bool  ChangeClientBalancy(vector <stClientData>& FileData, string Target, double Amount) {

	char DoTheOperation = 'y';

	cout << "\nAre u shire that you want to do this Transaction ? y/n ";
	cin >> DoTheOperation;

	if (toupper(DoTheOperation) == 'Y') {

		for (stClientData& C : FileData) {

			if (C.AccountNumber == Target) {

				C.Balance = C.Balance + Amount;
				LoadVectorDataToFile(FileData, MyClientsFile);

				cout << "\nThe Transaction is Done Successfully, New Balanc is : " << C.Balance;

			}
		}
		return true;
	}

	return false;
}
void ShowDepositScreen() {

	cout << "\n=================================\n";
	cout << "*** Deposit Screen ***";
	cout << "\n\=================================\n";


	vector <stClientData> FileData = LoadFileDataToVector(MyClientsFile);
	stClientData Client;
	string Target = ReadAccountNumberFromUser();


	while (!FindClientByAccountNumber(FileData, Target, Client)) {

		cout << "The Account Number " << Target << " is Not Found !!";
		Target = ReadAccountNumberFromUser();
	}

	ShowClientCard(Client);
	double Amount = AmountOfOperation();

	ChangeClientBalancy(FileData, Target, Amount);


}

//Withdraw screen


void ShowWithDrawScreen() {

	cout << "\n=================================\n";
	cout << "\t*** Withdraw Screen ***";
	cout << "\n\=================================\n";


	vector <stClientData> FileData = LoadFileDataToVector(MyClientsFile);
	stClientData Client;
	string Target = ReadAccountNumberFromUser();


	while (!FindClientByAccountNumber(FileData, Target, Client)) {

		cout << "The Account Number " << Target << " is Not Found !!";
		Target = ReadAccountNumberFromUser();
	}

	ShowClientCard(Client);
	double Amount = AmountOfOperation();

	while (Amount > Client.Balance) {

		cout << "\nThe Amount is Extend the Total Balance, You can withdraw up to : " << Client.Balance << endl;
		Amount = AmountOfOperation();

	}
	ChangeClientBalancy(FileData, Target, Amount * -1);

}


// Total Balances :

void ShowTotalBalances() {

	vector <stClientData> FileData = LoadFileDataToVector(MyClientsFile);
	double TotalBalances = 0;

	cout << "\n\t\t\t\t\tClient List (" << FileData.size() << ")Client(s).";

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";

	cout << "| " << left << setw(12) << "Balance";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;


	if (FileData.size() == 0)
	{
		cout << "\t\t\t\tNo Available Clients in System  :-( ";
	}
	else
	{
		for (stClientData& Client : FileData) {

			cout << "| " << setw(15) << left << Client.AccountNumber;
			cout << "| " << setw(40) << left << Client.Name;
			cout << "| " << setw(12) << left << Client.Balance;
			cout << endl;

			TotalBalances += Client.Balance;
		}
	}


	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "\t\t\t" << setw(15) << "Total Balances : " << TotalBalances << endl;


}
void DoingTransactionsChoice(enTransactionsScreen Choice) {

	switch (Choice) {

	case enTransactionsScreen::eDeposit:
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionsScreen();
		break;

	case enTransactionsScreen::eWithdraw:
		system("cls");
		ShowWithDrawScreen();
		GoBackToTransactionsScreen();
		break;

	case enTransactionsScreen::eTotalBalances:
		system("cls");
		ShowTotalBalances();
		GoBackToTransactionsScreen();
		break;

	case enTransactionsScreen::eBackToMainMenue:
		system("cls");
		PlayProject();
		break;

	}
}

short ReadTransactionsScreenNumber() {

	short Num = 0;
	do {
		cout << "Chosse what do you want to do ?? [1/4] ";
		cin >> Num;
	} while (Num < 1 || Num > 4);

	return Num;
}
void showTransactionsScreen() {

	system("cls");

	cout << "\n==================================================\n";
	cout << "\t\t Transactions Screen \t\t\t";
	cout << "\n==================================================\n\n";
	cout << "\t" << "[1] Deposit " << endl;
	cout << "\t" << "[2] Withdraw" << endl;
	cout << "\t" << "[3] Total Balacnses" << endl;
	cout << "\t" << "[4] Main Menue Screen" << endl;
	cout << "\n==================================================\n\n";
}
void TransacitonScreen() {

	showTransactionsScreen();
	DoingTransactionsChoice((enTransactionsScreen)ReadTransactionsScreenNumber());
}



//Main Code of project : 
void DoingUserChosse(enMainMenuScreen Chosse) {

	switch (Chosse) {

	case enMainMenuScreen::eShowClientsList:
		system("cls");
		ShowAllClinetsData();
		GoBackToMainMenue();
		break;

	case enMainMenuScreen::eAddNew:
		system("cls");
		AddMoreClients();
		GoBackToMainMenue();
		break;

	case enMainMenuScreen::eDeleteClient:
		system("cls");
		DeleteClientByAccountNumber();
		GoBackToMainMenue();
		break;

	case enMainMenuScreen::eUpdateClient:
		system("cls");
		MainUpdatingClientData();
		GoBackToMainMenue();
		break;

	case enMainMenuScreen::eFindClient:
		system("cls");

		FindSpicificClient();
		GoBackToMainMenue();
		break;

	case enMainMenuScreen::eTransactions:
		system("cls");
		TransacitonScreen();
		break;
	case enMainMenuScreen::eExit:
		system("cls");
		ExitScreen();
		break;
	}
}
void ShowMainMenuScreen() {

	system("cls");

	cout << "\n==================================================\n";
	cout << "\t\t Main Menu Secreen \t\t\t";
	cout << "\n==================================================\n\n";
	cout << "\t" << "[1] Show Clients List " << endl;
	cout << "\t" << "[2] Add New Client" << endl;
	cout << "\t" << "[3] Delete Client" << endl;
	cout << "\t" << "[4] Update Client Information" << endl;
	cout << "\t" << "[5] Find Client" << endl;
	cout << "\t" << "[6] Transactions" << endl;
	cout << "\t" << "[7] Exit" << endl;
	cout << "\n==================================================\n\n";

}

short ChosseOptionFromList() {

	short Num = 0;
	do {
		cout << "Chosse what do you want to do ?? [1/7] ";
		cin >> Num;
	} while (Num < 1 || Num > 7);

	return Num;
}

void PlayProject() {


	ShowMainMenuScreen();
	DoingUserChosse((enMainMenuScreen)ChosseOptionFromList());

}

int main() {

	PlayProject();
	return 0;
}


