/*
- logowanie / rejestracja uzytkownika zapis do pliku txt;
- menu logowania;
- jesli zalogowany, wyswietl menu zalogowanego,
- pobierz kontakty uzytkownika zalogowanego,
*/
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;
struct UserInfo{
    int UserId;
    string nick, password;
};
struct ContactInfo{
    int ContactId, UserId;
    string name, surname, tel, address, email;
};

void LoginMenu();
int verifyChar();
string editTxt();
int registerForm(vector<UserInfo>&, int idNr);
UserInfo uploadUser(string txtLine);
void importUsers(vector<UserInfo>&);
int returnUserId(vector<UserInfo>&);
int loginForm(vector<UserInfo>&);
int LogOut(int id);
int registeredMenu(vector<UserInfo>&,int id);
ContactInfo uploadContacts(string txtLine);
void importUserContacts(vector<ContactInfo>&, int id);
void importAllContacts(vector<ContactInfo>&);
int returnLastContactId();
int addNewContact(vector<ContactInfo>&contacts,int userId, int contactsNr);
void displayAllContacts(vector<ContactInfo>&);
void findName(vector<ContactInfo>&);
void findSurname(vector<ContactInfo>&);
/********************************************************/

/************************************************************/
int main(){
    vector<UserInfo>users;
    int choice, usersNr,registeredId=0,LogedIn=0;
    importUsers(users);
    usersNr=returnUserId(users);
    while(true){
        if(LogedIn==0){
            LoginMenu();
            choice=verifyChar();
            switch(choice){
                case 1:{
                    registeredId=loginForm(users);
                    LogedIn=1;
                    break;
                }case 2:{
                    usersNr=registerForm(users, usersNr);
                    importUsers(users);
                    break;
                }case 3:{
                    cout<<"ilosc uz: "<<returnUserId(users);
                    getchar();getchar();
                    break;
                }case 0: exit(0);break;
                default: cout<<"Sorry wrong choice. Try again(0-2)"; Sleep(2000);
            }
        }else{
            LogedIn=registeredMenu(users, registeredId);
            if(LogedIn==0)registeredId=0;
        }
    }
    return 0;
}
/*********************************************************************/
void LoginMenu(){
        system("cls");
        cout << "\t\tMENU" << endl;
        cout << "====================================" << endl;
        cout << "1. LOGIN." << endl;
        cout << "2. Register." << endl;
        cout << "3.Test." << endl;
        cout << "0. Exit." << endl;
        cout << "====================================" << endl;
        cout << "Your choice: ";
}

int verifyChar(){
    string choice; int digit;

    while(cin>>choice){
    if(choice.length()>1) return 13;
        else{
            digit=choice[0]-48;
            if(digit>=0&&digit<=9) return digit;
            else return 13;
        }
    }
}

string editTxt(){
    string txt;
    cin.sync();
    getline(cin, txt);
    transform(txt.begin(), txt.end(),txt.begin(),::toupper);
    return txt;
}

UserInfo uploadUser(string txtLine){
    UserInfo user;
    string singleWord="";
    int dataNr=1;
        for(int i=0; i<txtLine.length(); i++){
            if(txtLine[i]!='|'){
                singleWord+=txtLine[i];
            }else{
            switch(dataNr){
            case 1:
                user.UserId=atoi(singleWord.c_str());
                break;
            case 2:
                user.nick=singleWord;
                break;
            case 3:
                user.password=singleWord;
                break;
            }
            singleWord="";
            dataNr++;
            }
        }
    return user;
}

void importUsers(vector<UserInfo>&users){
    UserInfo user;
    string txt="";
    fstream file;
    file.open("Users.txt", ios::in);
    if(file.good()==true){
        while(getline(file,txt)){
            user=uploadUser(txt);
            users.push_back(user);
        }
        file.close();
    }
}

int returnUserId(vector<UserInfo>&users){
    UserInfo user;
    int id;
    if(users.empty()==true) id=0;
    else{
        user.UserId=users.back().UserId;
        id=user.UserId;
    }
    return id;
}

int registerForm(vector<UserInfo>&users, int idNr){
    UserInfo tmpUser;
    bool found=false;
    int id=idNr;

    cout<<"Enter Name/Nick: ";
    tmpUser.nick=editTxt();
    for(vector<UserInfo>::iterator itr=users.begin(); itr!=users.end(); itr++){
        if(itr->nick==tmpUser.nick){
            found=true;
        }
    }
    if(found==true){
        cout<<"Sorry nick already registered, try again.";
        Sleep(2000);
    }else{
        cout<<"Enter Password: ";
        cin>>tmpUser.password;
        tmpUser.UserId=idNr+1;

        fstream file;
        file.open("Users.txt",ios::out | ios::app);

        if (file.good() == true){
            file<<tmpUser.UserId<<"|"<<tmpUser.nick<<"|"<<tmpUser.password<<"|"<<endl;
            file.close();
        }else{
            cout << "ERROR: User not saved." << endl;
            system("pause");
        }
        cout << endl << "User successfully registered." << endl;
        cout << "You can Log In now to start using Contact Book.\n" << endl;
        id=idNr+1;
        system("pause");
    }
        return id;
}

int loginForm(vector<UserInfo>&users){
    UserInfo tmpUser;
    int id=0;
    string tmpPass="";
    bool found=false;
    cout<<"Enter Nick/Name: ";
    tmpUser.nick=editTxt();
    for(vector<UserInfo>::iterator itr=users.begin(); itr!=users.end(); itr++){
          if(itr->nick==tmpUser.nick){
                found=true;
              for(int pin=0; pin<3; pin++){
                cout<<"Enter password: ";
                cin>>tmpUser.password;
                if(tmpUser.password==itr->password){
                    id=itr->UserId;
                    return id;
                }else{
                    cout<<"Sorry wrong password.\n";
                    Sleep(2000);
                }
            }
            cout<<"Sorry you entered 3X wrong password!!!";
            Sleep(2000);
            exit(0);
          }
    }
    if (found==false){
       cout<<"Sorry user with "<<tmpUser.nick<<" is not registered.\n";
        Sleep(2000);
    }
    return 0;
}


int registeredMenu(vector<UserInfo>&users,int id){
    vector<ContactInfo>contacts;
    int choice;
    int contactsNr=returnLastContactId();
    importUserContacts(contacts, id);
    UserInfo tmpUser;

    for(vector<UserInfo>::iterator itr=users.begin(); itr!=users.end(); itr++){
     if(id==itr->UserId) tmpUser.nick=itr->nick;
    }
    system("cls");
    cout<<"HELLO: "<<tmpUser.nick<<endl;
    cout << "\t\tMENU" << endl;
        cout << "====================================" << endl;
        cout << "1. Add new person." << endl;
        cout << "2. Find person by name." << endl;
        cout << "3. Find person by surname." << endl;
        cout << "4. Display all contacts." << endl;
        cout << "5. Remove contact." << endl;
        cout << "6. Edit contact." << endl;
        cout << "0. Log out." << endl;
        cout << "====================================" << endl;
        cout << "Your choice: ";
        choice=verifyChar();
        switch(choice){
            case 1:{
                    contactsNr=addNewContact(contacts,id,contactsNr);
                break;
            }case 2:{
                    findName(contacts);
                break;
            }case 3:{
                    findSurname(contacts);
                break;
            }case 4:{
                    displayAllContacts(contacts);
                break;
            }case 5:{
                 //removeContact(contacts);
                break;
            }case 6:{
                //editContact(contacts);
                break;
            }case 0: id=0;
                break;
            default: cout<<"Sorry wrong choice. Try again(0-6)"; Sleep(2000);
         }
         return id;
}

ContactInfo uploadContacts(string txtLine){
     ContactInfo tmpContact;
    string singleWord="";
    int dataNr=1;
        for(int i=0; i<txtLine.length(); i++){
            if(txtLine[i]!='|'){
                singleWord+=txtLine[i];
            }else{
            switch(dataNr){
            case 1:
                tmpContact.ContactId=atoi(singleWord.c_str());
                break;
            case 2:
                tmpContact.UserId=atoi(singleWord.c_str());
                break;
            case 3:
                tmpContact.name=singleWord;
                break;
            case 4:
                tmpContact.surname=singleWord;
                break;
            case 5:
                tmpContact.tel=singleWord;
                break;
            case 6:
                tmpContact.address=singleWord;
                break;
            case 7:
                tmpContact.email=singleWord;
                break;
            }
            singleWord="";
            dataNr++;
            }
        }
    return tmpContact;
}

void importUserContacts(vector<ContactInfo>&contacts, int id){
    ContactInfo tmpContact;
     string txt="";
    fstream file;
    file.open("ContactsBook.txt", ios::in);
    if(file.good()==true){
        while(getline(file,txt)){
            tmpContact=uploadContacts(txt);
            if(id==tmpContact.UserId)
            contacts.push_back(tmpContact);
        }
        file.close();
    }
}

int returnLastContactId(){
    ContactInfo tmpContact;
    int id=0;
    string singleContact;
    fstream file;
    file.open("ContactsBook.txt", ios::in);
    if(file.good()==true){
        while(getline(file,singleContact)){
            tmpContact=uploadContacts(singleContact);
            id=tmpContact.ContactId;
        }
        file.close();
    }
    return id;
}

int addNewContact(vector<ContactInfo>&contacts,int userId, int contactsNr){
        ContactInfo contact;
        contact.ContactId=contactsNr+1;
        contact.UserId=userId;
        cout << "Enter Name: ";
        contact.name = editTxt();
        cout << "Enter Surname: ";
        contact.surname = editTxt();
        cout << "Phone Nr: ";
        contact.tel = editTxt();
        cout << "Email: ";
        contact.email = editTxt();
        cout << "Address: ";
        contact.address = editTxt();

    fstream file;
    file.open("ContactsBook.txt",ios::out | ios::app);

    if (file.good() == true){
        file<<contact.ContactId<<"|"<<contact.UserId<<"|"<<contact.name<<"|"
            <<contact.surname<<"|"<<contact.tel
            <<"|"<<contact.email<<"|"<<contact.address <<"|"<< endl;
        file.close();
    }else{
        cout << "ERROR: Contact not saved." << endl;
        system("pause");
    }
    cout << endl << "New contact added." << endl; system("pause");
    return contactsNr+1;
}

void displayAllContacts(vector<ContactInfo>& contacts){
    if(!contacts.empty()){
     for(vector<ContactInfo>::iterator itr=contacts.begin(); itr!=contacts.end(); itr ++){
            cout<<"\nid: "<<itr->ContactId<<endl;
            cout<<"\tName: "<<itr->name<<"\t"<<"Surname: "<<itr->surname<<endl;
            cout<<"\tPhone: "<<itr->tel<<endl;
            cout<<"\tEmail: "<<itr->email<<endl;
            cout<<"\tAddress: "<<itr->address<<endl;
            cout<<"    =================================="<<endl;
            }
    }else cout<<"Address Book empty !!!";
    getchar();getchar();
}

void findName(vector<ContactInfo>& contacts){
    string tmpName;
    int tmp_nr=0, vectorSize;
    cout<<"Enter Name: ";
    tmpName=editTxt();
    if(!contacts.empty()){
     for(vector<ContactInfo>::iterator itr=contacts.begin(); itr!=contacts.end(); itr ++){
        if(tmpName==itr->name){
            cout<<"\nid: "<<itr->ContactId<<endl;
            cout<<"\tName: "<<itr->name<<"\tSurname: "<<itr->surname<<endl;
            cout<<"\tPhone: "<<itr->tel<<endl;
            cout<<"\tEmail: "<<itr->email<<endl;
            cout<<"\tAddress: "<<itr->address<<endl;
            cout<<"   =======================================\n";
            tmp_nr++;
        }
     }
    }
    if(tmp_nr==0) cout<<"No contact has been found.";
        tmp_nr=0;
    getchar();
}

void findSurname(vector<ContactInfo>& contacts){
    string tmpSurname;
    int tmp_nr=0, vectorSize;
    cout<<"Enter Surname: ";
    tmpSurname=editTxt();
    if(!contacts.empty()){
     for(vector<ContactInfo>::iterator itr=contacts.begin(); itr!=contacts.end(); itr ++){
        if(tmpSurname==itr->surname){
            cout<<"\nid: "<<itr->ContactId<<endl;
            cout<<"\tName: "<<itr->name<<"\tSurname: "<<itr->surname<<endl;
            cout<<"\tPhone: "<<itr->tel<<endl;
            cout<<"\tEmail: "<<itr->email<<endl;
            cout<<"\tAddress: "<<itr->address<<endl;
            cout<<"   =======================================\n";
            tmp_nr++;
        }
     }
    }
    if(tmp_nr==0) cout<<"No contact has been found.";
        tmp_nr=0;
    getchar();
}


