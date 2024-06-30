#include<iostream>
#include<fstream>
#include<vector>
#include<map>
using namespace std;

#define UNDERLINE "\033[4m"
#define CLOSEUNDERLINE "\033[0m"

#define MIN_BALANCE 500     //needs space instaed of"="(macro item)
class InusfficientFunds{};

class Account{
    private:
    string firstName;
    string lastName;
    long accountNumber;
    float balance;
    static long NextAccountNumber; // Cannot be modified
    public:
    Account() {}
    Account(string fname, string lname, float balance){
        NextAccountNumber++;
        accountNumber=NextAccountNumber ;
        firstName=fname;
        lastName=lname ;
        this->balance=balance; //This pointer pointing the class varaibale to constructor varaiable(as they have same name)
    }
    long getAccountNumber(){
        return accountNumber;
    }
    string getFirstName(){
        return firstName;
    }
    string getLastName(){
        return lastName;
    }
    float getBalance(){
        return balance;
    }

    void Deposit(float amount);
    void Withdraw(float amount);
    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();

    friend ofstream & operator<<(ofstream &ofs, Account &acc); // to insert in file
    friend ifstream & operator>>(ifstream &ifs, Account &acc); // to read in file
    friend ostream & operator<<(ostream &os, Account & acc); // to dsiplay/extract file
};
long Account :: NextAccountNumber=0;

class Bank 
{
    private:
    map<long,Account> accounts;
    public:
    Bank();
    Account OpenAccount(string fname, string lname, float balance);
    Account BalanceEnquiry(long accountNumber);
    Account Deposit(long accountNumber, float amount);
    Account Withdraw(long accountNumber, float amount);
    void CloseAccount(long accountNumber);
    void ShowAllAccounts();
    ~Bank();
};

int main(){
    Bank b;
    Account acc;
    string fname,lname;
    long accountNumber;
    float balance,amount;
    int choice;
    while(choice!=7){
        cout<<endl<<UNDERLINE<<"\n\t Welcome, Please select one option from below"<<CLOSEUNDERLINE;
        cout<<"\n\t1 Open an Account";
        cout<<"\n\t2 Balance Enquiry";
        cout<<"\n\t3 Deposit Money in your Account";
        cout<<"\n\t4 Withdraw Money from your Account";
        cout<<"\n\t5 Close an Account";
        cout<<"\n\t6 Display All Accounts";
        cout<<"\n\t7 I would like to visit Afterwards, Thankyou";

        cout<<endl<<"\n Dear Customer, Enter you choice :  ";
        cin>>choice;
        switch(choice){
            case 1: 
            cout<<"Enter First Name: ";
            cin>>fname;
            cout<<"Enter Last Name: ";
            cin>>lname;
            cout<<"Enter intial Balance(Minimum 500 required): ";
            cin>>balance;
            acc=b.OpenAccount(fname,lname,balance);
            cout<<endl<<"\n Congratulations, Account Succesfully Created "<<endl<<acc;
            break;

            case 2:
            cout<<endl<<"Enter the number of your Account: "<<endl;
            cin>>accountNumber;
            acc=b.BalanceEnquiry(accountNumber);
            cout<<endl<<"Account: "<<accountNumber<<endl<<"Your Account Details: "<<endl<<acc;
            break;

            case 3:
            cout<<endl<<"Enter the number of your Account: ";
            cin>>accountNumber;
            cout<<"Enter Amount To be deposited: ";
            cin>>amount;
            acc=b.Deposit(accountNumber,amount);
            cout<<endl<<"Account: "<<accountNumber<<endl<<"Amount Deposited Succesfull, your updated balance is: "<<endl<<acc;
            break;

            case 4:
            cout<<endl<<"Enter the number of your Account: ";
            cin>>accountNumber;
            cout<<"Enter Amount to be Withdrawn: ";
            cin>>amount;
            acc=b.Withdraw(accountNumber,amount);
            cout<<endl<<"Account: "<<accountNumber<<endl<<"Amount Withdrawn Succesfull, your updated balance is: "<<endl<<acc;
            break;

            case 5:
            cout<<endl<<"Enter the number of your Account: ";
            cin>>accountNumber;
            b.CloseAccount(accountNumber);
            cout<<acc<<endl<<"Your account has been closed, Thankyou for being a part of us. ";
            break;

            case 6:
            cout<<endl<<UNDERLINE<<"  ** Accounts Summary ** "<<CLOSEUNDERLINE<<endl;
            b.ShowAllAccounts();
            cout<<endl<<"Thankyou for being with us "<<endl;
            break;

            case 7:
            cout<<"Ending Program";
            exit(0);

            default:
            cout<<"\n Dear Customer, Wrong Choice!!"<<endl<<"\nPlease Try Again"<<endl;
            break;
        }

    }

    return 0;
}

void Account::Deposit(float amount){
    balance+=amount;
}
void Account::Withdraw(float amount){
    int x=balance-amount;
    if(x<MIN_BALANCE){
        cout<<"Insufficinet Balance, Please try later.";
    }
    else{
        balance=x;
    }
}
void Account::setLastAccountNumber(long accountNumber)
{
    NextAccountNumber=accountNumber;
}
long Account::getLastAccountNumber()
{
    return NextAccountNumber;
}
ofstream & operator<<(ofstream &ofs, Account &acc){
    ofs<<acc.accountNumber<<endl;
    ofs<<acc.firstName<<endl;
    ofs<<acc.lastName<<endl;
    ofs<<acc.balance<<endl;
    return ofs;
}
ifstream &operator>>(ifstream &ifs, Account &acc)
{
    ifs>>acc.accountNumber;
    ifs>>acc.firstName;
    ifs>>acc.lastName;
    ifs>>acc.balance;
    return ifs ;
    // TODO: insert return statement here
}

ostream & operator<<(ostream &os, Account &acc){
    os<<"First Name: "<<acc.getFirstName()<<endl;
    os<<"Last Name: "<<acc.getLastName()<<endl;
    os<<"Account Number: "<<acc.getAccountNumber()<<endl;
    os<<"Balance: Rs"<<acc.getBalance()<<endl;
    return os;
}

Bank::Bank(){
    Account account;
    ifstream infile;
    infile.open("Bank.Data");
    if(!infile){
        cout<<"File  Not  found !!"<<endl;
        return;
    }
    while(!infile.eof())//while (!=end of file)
    {
        infile>>account;
        accounts.insert(pair<long,Account>(account.getAccountNumber(),account));
    }
    Account::setLastAccountNumber(account.getLastAccountNumber());

    infile.close();
}

Account Bank::OpenAccount(string fname, string lname, float balance){

    ofstream outfile;
    Account account(fname,lname,balance);
    accounts.insert(pair<long,Account>(account.getAccountNumber(),account));

    outfile.open("Bank.Data", ios::trunc);//new file created with fresh data(truncked data)
//Accesssing & storing in list
    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++){
        outfile<<itr->second; //Iterating in account class object(second)
    }
    outfile.close();
    return account;
}
Account Bank::BalanceEnquiry(long accountNumber){
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    return itr->second;
}
Account Bank::Deposit(long accountNumber, float amount){
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    itr->second.Deposit(amount);
    return itr->second;
}
Account Bank::Withdraw(long accountNumber, float amount){
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    itr->second.Withdraw(amount);
    return itr->second;
}

void Bank::CloseAccount(long accountNumber)
{
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    cout<<"Account Deleted : "<<itr->second;
    accounts.erase(accountNumber);
}
void Bank::ShowAllAccounts()
{
    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++){
        cout<<"Accounts Detail are as follow: "<<itr->first<<endl<<itr->second<<endl;
    }
}

Bank::~Bank(){
    ofstream outfile;
    outfile.open("Bank.Data", ios::trunc);

    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++){
        outfile<<itr->second;
    }
    outfile.close();
}
























    





