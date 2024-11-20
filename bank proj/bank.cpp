#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

//global variables.
string user, pass, name, inUser, inPass;
int nid;
double balance;

//declaring all main functions.
void register_user();
void login();
bool CheckCredentials(string, string);
void account(string ,string );
int digit();

//function for randomly generating a sequence of numbers for user-id and card-id used in line#
int digit() 
{
    srand(time(nullptr));
    return rand() % 90000 + 10000;
}

//class for card, used in another class (User).
class Card
{
    public:
        int nid;
        int cardid;
        double cardbal = 0;
        bool iflimit = false;
        double limit = 10000; //limit set on the card, prevents user from depositing any more amount than the limit into that card.

        void displayCardDetails();
        void updatefile();    
};

//main class.
class User
{
    public:
        int nid;
        double balance = 0;
        string name;
        vector<Card> cards; //vector to store card details.
    
        void displayDetails();
        void getBalance();
        void withdraw();
        void deposit();
        void updatefile();
        void viewcard();
        void addcard();
        void removecard();
        void depositcard();
        void withdrawcard();
};

void User :: displayDetails() //displays details of the user.
{
    cout<<"\n------ Account Details --------"<<endl;
    cout<<"\nAccount Holder - \t"<<name<<endl;
    cout<<"Account N.ID - \t\t"<<nid<<endl;
}

void User :: getBalance() //displays current balance stored in  the user's bank account.
{
    cout<<"\n---------- Balance ------------"<<endl;
    cout<<"\nYour Current Balance is - "<<balance<<"$."<<endl;
}

void User :: withdraw() //method to withdraw entered amount from the user's bank account.
{
    double amount = 0;
    
    cout<<"\n---------- Withdrawal --------------"<<endl;
    cout<<"\nEnter amount to be withdrawn ---> ";
    cin >> amount;

    if (amount > balance) //will not withdraw if bank account doesn't have enough balance.
    {
        cout<<"\nnot enough in account"<<endl;
    }
    else
    {
        balance = balance - amount;
        cout<<"\nAmount has been withdrawn"<<endl;
        cout<<"Your account now holds "<<balance<<"$."<<endl;
        updatefile(); //updates customer.txt
    }
}

void User :: deposit()//method to deposit entered amount into the user's bank account.
{
    double amount = 0;

    cout<<"\n---------- Deposal -----------------"<<endl;
    cout<<"\nEnter amount to be deposited ---> ";
    cin >> amount;

    balance = balance + amount;
    cout<<"\nAmount has been deposited"<<endl;
    cout<<"Your account now holds "<<balance<<"$."<<endl;
    updatefile();//updates customer.txt
}

void User::updatefile() //updates customer.txt by creating a temp file and renaming it whilst deleting the outdated file.
{
    ifstream infile("customer.txt");
    ofstream outfile("temp.txt");

    if (!infile.is_open() || !outfile.is_open())
    {
        cout << "\nError\n" << endl;
        return;
    }

    string uuname, upass, ufname;
    double ubal;
    int uid;

    while (infile >> uuname >> upass >> ufname >> ubal >> uid) //reads data from 'customer.txt'.
    {
        if (uid == nid)
        {
            ubal = balance;
        }
        outfile << uuname << " " << upass << " " << ufname << " " << ubal << " " << uid << endl;//writes data into 'temp.txt'.
    }
    infile.close();
    outfile.close();
    remove("customer.txt");//swapping process.
    rename("temp.txt", "customer.txt");
}


void User :: viewcard()//allows user to view thier cards.
{
    int count = 1;

    cout <<"\n----------- Your Cards -----------"<<endl;
    for (auto &card : cards)
    {
        cout<<"\nCard No."<<count<<endl;
        card.displayCardDetails();
        count+=1;
    }
}

void User :: addcard()//creates a new card and adds it into vector.
{
    int limit; 
    int opt;
    int newCardID = digit();

    Card newCard{newCardID}; //creates a new object.
    newCard.cardid = newCardID;
    cout<<"\nWould you like to add a limit to your card? (minimun 100$ ?)"<<endl; 
    while (true)
    {
        cout<<"1. Yes\n2. No\n";
        cout<<"Enter option ---> ";
        cin>>opt;
        
        if (opt == 1)
        {
            while (limit < 100 || limit > 1000 ) //limit can only be between 100 to 1000 to avoid irrational limits.
            {
            cout<<"Enter limit value (100 to 1000) ---> ";
            cin >> limit;
            }
            newCard.limit = limit;
            break;
        }
        else if (opt == 2)
        {
            cout<<"\nok"<<endl;
            break;
        }
        else
        {
            cout<<"\nInvalid Option"<<endl;
        }
    }
    cards.push_back(newCard);
    cout<<"\nNew card added successfully"<<endl;
}

void User :: removecard()//erases card from the vector.
{
    int index;
    cout<<"Enter Card No. ---> ";//card number will be displayed in 'View Cards' option.
    cin>>index;
    
    if (index >= 0 && index < cards.size() + 1)
    {
        index = index - 1;
        cards.erase(cards.begin() + index);
        cout<<"\nCard Removed"<<endl;
    }
    else
    {
        cout<<"\nError"<<endl;
    }
}

void User :: withdrawcard()//asks user amount to be withdrawn from 
{
    int index;
    int amount;
    cout<<"Enter Card No. ---> ";
    cin>>index;
    
    if (index >= 0 && index < cards.size() + 1)
    {
        index = index - 1;
        cout<<"Enter amount to be withdrawn ---> ";
        cin >> amount;

        if (amount > cards[index].cardbal) //will not withdraw if card doesnt hold enough balance.
        {
            cout<<"Insufficient Funds"<<endl;
        }
        else
        {
            cards[index].cardbal -= amount;
            cout << "Amount has been withdrawn from Card ID " << cards[index].cardid << endl;
            cout << "Card Balance: " << cards[index].cardbal << "$" << endl;
            cards[index].updatefile();
        }
    }
    else
    {
        cout<<"\nNo Cards"<<endl;
    }
}

void User :: depositcard()
{
    int index;
    int amount;
    cout<<"Enter Card No. ---> ";
    cin>>index;
    
    if (index >= 0 && index < cards.size() + 1)
    {
        index = index - 1;
        cout<<"Enter amount to be deposited ---> ";
        cin >> amount;

        if (amount < 0)
        {
            cout<<"\nInvalid Input"<<endl;
        }
        else
        {
            if (cards[index].cardbal + amount > cards[index].limit) //card will not exceed the limit it has been set to.
            {
                cout<<"Cannot deposit amount, Card will exceed limit"<<endl;
            }
            else
            {
            cards[index].cardbal += amount;
            cout << "Amount has been deposited into Card ID " << cards[index].cardid << endl;
            cout << "Card Balance: " << cards[index].cardbal << "$" << endl;
            cards[index].updatefile();
            }
        }
    }
    else
    {
        cout<<"\nNo Cards"<<endl;
    }
}

void Card :: displayCardDetails() //prints card details, called from viewcard().
{
    cout << "------ Card Details ------\n";
    cout << "Card ID: " << cardid << endl;
    cout << "Card Balance: " << cardbal << "$\n";
} 

void Card :: updatefile() //updates "card.txt" by creating "temp.txt" and renaming it.
{
    ifstream infile("cards.txt");
    ofstream outfile("temp_cards.txt");

    if (!infile.is_open() || !outfile.is_open())
    {
        cout << "\nError opening files\n";
        return;
    }

    string cardInfo;
    vector<string> allCardInfo;

    while (getline(infile, cardInfo))
    {
        allCardInfo.push_back(cardInfo);
    }

    for (auto &cardInfo : allCardInfo)
    {
        stringstream ss(cardInfo);
        int cardIDFromFile;
        double cardBalanceFromFile;
        bool limitFromFile;

        ss >> cardIDFromFile >> cardBalanceFromFile >> limitFromFile;

        if (cardIDFromFile == cardid) 
        {
            outfile << cardid << " " << cardbal << " " << limit << endl;
        } 
        else 
        {
            outfile << cardInfo << endl;
        }
    }

    infile.close();
    outfile.close();

    remove("cards.txt"); //swapping process.
    rename("temp_cards.txt", "cards.txt");
}

//main functions.
void register_user() //registers users so they can login as this function adds their detials to "login.txt" and "customer.txt".
{
    cout<<"\n--------- REGISTRATION ---------"<<endl;
    cout << "\n\n" << "New Username: ";
    cin >> user;
    cout << endl;
    cout << "New Password: ";
    cin >> pass;
    cout << endl;
    cout << "First name: ";
    cin >> name;
    cout << endl;
    cout << "Registered Successfully!" << endl;
    cout << endl;
    nid = digit(); //random id.
    cout<<"\n Your N-ID is "<<nid<<endl;

    ofstream l("login.txt", ios::app);
    if (!l.is_open())
    {
        cout << "\ncould not open file \n";
    }

    l << user << " " << pass << endl;
    l << endl;
    l.close();

    ofstream c("customer.txt", ios::app);
    if (!c.is_open())
    {
        cout << "\ncould not open file \n";
    }

    c << user << " " << pass << " " << name << " " << balance << " " << nid <<  endl;
    c << endl;
    c.close();
}

void login() //function to link the user to their bank account after they are able to login.
{
    int opt;
    int opt1;
    do
    {
        cout<<"\n----------- LOGIN -----------"<<endl;
        cout << "\nUsername: ";
        cin >> inUser;
        cout << "Password: ";
        cin >> inPass;


        if (CheckCredentials(inUser, inPass) == true) //another main function.
        {
            cout << "\nLogin sucessful!\n\n" << endl;
            cout << "Welcome, " << inUser << endl;
            account(inUser, inPass);
        }
        else
        {
        cout << "\nInvalid username or password. " << endl;
        while (true)
        {
        cout << "1. Try again\n2. Register"<<endl; //if the user gets username and pw wrong, they can retry or create a new account.
        cout<<"Enter option ---> ";
        cin >> opt1;

        if (opt1 == 1)
        {
            break;
        }
        else if (opt1 == 2)
        {
            register_user();
        }
        else
        {
            cout<<"\nInvalid Option\n";
        }
        }
        }

        } while (CheckCredentials(inUser, inPass) != true);
}

void account(string inUser,string inPass) //creates the user classes and provides a menu for their bank account.
{
    string uuname;
    string upass;
    string ufname;
    double ubal;
    int uid;
    int opt;

    ifstream f("customer.txt");

    if (!f.is_open())
    {
        cout << "Unable to open customer file!" << endl;
        return;
    }

    while (f >> uuname >> upass >> ufname >> ubal >> uid)
    {
        if (inUser == uuname && inPass == upass)
        {
            f.close();
            User user{uid , ubal , ufname};
            user.nid = uid;

            while (opt != 5)
            {
            cout<<"\n----------- MY ACCOUNT -----------"<<endl;
            cout<<"\n1. Display Details "<<endl;
            cout<<"2. Display Current Balance "<<endl;
            cout<<"3. Withdraw "<<endl;
            cout<<"4. Deposit "<<endl;
            cout<<"5. My Cards"<<endl;
            cout<<"6. Logout "<<endl;
            cout<<"\n Enter an option ---> ";
            cin>>opt;
            if (opt == 1)
            {
                user.displayDetails();
            }
            else if (opt == 2)
            {
                user.getBalance();
            }
            else if (opt == 3)
            {
                user.withdraw();
            }
            else if (opt == 4)
            {
                user.deposit();
            }
            else if (opt == 6)
            {
                cout<<"\nLogging off...\n"<<endl;
                break;
            }
            else if (opt == 5)
            {
                int opt1;

                while (opt != 6)
                {
                cout<<"\n----------- CARD HOMEPAGE -----------";
                cout<<"\n1. View Cards"<<endl;
                cout<<"2. Add Card"<<endl;
                cout<<"3. Remove Card"<<endl;
                cout<<"4. Deposit into card"<<endl;
                cout<<"5. Withdraw from card"<<endl;
                cout<<"6. Exit"<<endl;
                cout<<"\nEnter option ---> ";
                cin>>opt1;

                if (opt1 == 1)
                {
                    user.viewcard();
                }
                else if (opt1 == 2)
                {
                    user.addcard();
                }
                else if (opt1 == 3)
                {
                    user.removecard();
                }
                else if (opt1 == 4)
                {
                    user.depositcard();
                }
                else if (opt1 == 5)
                {
                    user.withdrawcard();
                }
                else if (opt1 == 6)
                {
                    cout<<"exiting...";
                    break;
                }
                else
                {
                    cout<<"\nInvalid Option\n"<<endl;
                }
                }
            }
            else
            {
                cout<<"\nInvalid Option\n"<<endl;
            }
            }

        }
    }
}

bool CheckCredentials(string inUser, string inPass)//reads from "login.txt" to check if user can access thier account.
{
    string u;
    string p;

    ifstream f;
    f.open("login.txt");

    if (!f.is_open())
    {
        cout << "\nUnable to open file!\n";
        return false;
    }

    while (f >> u >> p)
    {
        if (inUser == u && inPass == p)
        {
            f.close();
            return true;
        }
    }

    f.close();
    return false;
}

void menu() //menu() called from main().
{
    bool loop = true;
    int opt = 0;

    while (opt != 3)
    {
        cout<<"\n------------- MENU -------------";
        cout<<"\n1. Register"<<endl;
        cout<<"2. Login"<<endl;
        cout<<"3. Exit"<<endl;
        cout<<"Enter option ---> ";
        cin >> opt;

        if (opt == 1)
        {
            register_user();
        }
        else if (opt == 2)
        {
            login();
        }
        else if (opt == 3)
        {
            cout<<"\nExiting..."<<endl;            
            break;
        }
        else
        {
            cout<<"\nInvalid Option\n";
        }
    }
}

int main() 
{
    cout<<"\n\n-------- WELCOME --------"<<endl;
    cout<<"---------- TO -----------"<<endl;
    cout<<"-------- NTU BANK -------"<<endl;
    menu();
    cout<<"\n------ CODE OVER ------"<<endl;
    return 0;
}