#include <iostream>
#include <cstring>
#include <string.h>
#include <fstream>
#include <iomanip>

using namespace std;

// system("color=04");
char files[10][50] {"Users.txt", "Librarians.txt", "Books.txt", "Activitylog.txt", "Report.txt", "temp.txt", "Requests.txt", "Accepted.txt", "Blacklist.txt"};

struct book{
    char title[50];
    char author[50];
    char id[50];
};

struct user{
    char name[50];
    char id[10];
    char pass[50];
    string type;
};

user currentuser;

// Function Prototype
void clear();
bool verifyPassword();
void encryptPassword();
void decryptPassword();
void signin(int);
bool login(int);
void addbook();
void searchbook();
void updatebook();
void deletebook();
void studentMenu();
void librarianMenu(int);
void adminMenu();
void firstMenu();
bool verifyPassword(int);
string securePassword(char*, char);
string replace(char*, char, char);

// Clear Screen
void clear(){
    system("CLS");
}

void cont(int t=0){
    char x[50];
    cout << "\n\n\t\t>> Press Enter to Continue :-)";
    if(t==0)
        cin.ignore();
    cin.getline(x, 50);
    clear();
}

// Hash passwords for security
string securePassword(char *pass, char t = 'e'){ // e - encrypt d - decrypt
    int len = strlen(pass);
    string hidden;
    for(int i=0; i<len; i++){
        if (t=='d'){
            if (pass[i] == ' ')
                pass[i] = '#';
            hidden += pass[i] + 1;
        }
        else{
            if (pass[i] == '#')
                pass[i] = ' ';
            hidden += pass[i] - 1;
        }
    }
    return hidden;
}

// Check if the password is correct
bool verifyPassword(int t){
      clear();
    string p, n; char u[50];
    bool verified = false;

    cout << "\n\n\t\t Hi " << replace(currentuser.name, '_', ' ') << ":-)\n\n\t\t>> Enter your Password: \t";
    // cin.ignore();
    cin.getline(currentuser.pass, 50);

    ifstream file(files[t]);
    while(file>>u>>p>>n){
        if(p==securePassword(currentuser.pass) && strcmp(u, currentuser.id) == 0){
        verified = true;
        break;
        }
    }
    file.close();
    clear();
    return verified;
}

// Replace ' ' blank space for files and return string
string replace(char *value, char x = ' ', char y= '_'){
    string v;
    for(int i=0; i<strlen(value); i++){
        if (value[i] == x)
            value[i] = y;
        v += value[i];
    }
    return v;
}

// Replace ' ' blank space from memory
void replace2(char *value, char x = ' ', char y= '_'){
     for(int i=0; i<strlen(value); i++){
         if (value[i] == x)
             value[i] = y;
     }
 }

// Check if the search value exists in the file
bool check(char x[50], int t=0)
{
    string u, p; char n[50]; bool exists = false;
    ifstream file(files[t]);

    while(file>>u>>p>>n){
        if(u==x){
            exists = true;
            break;
        }
    }
    file.close();
    return exists;
}

// User Registration
void signin(int t=0)
{
      clear();
    user newuser;
    cout << "\n\n\t\t----------------- Enter Sign in Info -----------------";

    cin.ignore();
    cout << "\n\n\t\t Enter Full Name: \t";
    cin.getline(newuser.name, 50);
    if(strlen(newuser.name) > 3 && newuser.name[0] != ' '){

    cout << "\n\n\t\t Hi " << newuser.name << ":-)\n\n\t\t Enter your Id No: \t";
    cin.getline(newuser.id, 10);
    if(!check(newuser.id, t)){
        if (strlen(newuser.id)>3){
            // cin.ignore();
            cout << "\n\n\t\t>Enter Password: \t";
            cin.getline(newuser.pass, 50);
            if(strlen(newuser.pass)>3){
                fstream file;
                file.open(files[t], ios::out | ios::app);
                file  << "\n" << setw(10) << left <<  newuser.id << "\t" <<  setw(20) << left << securePassword(newuser.pass) << "\t" << setw(20) << left <<  replace(newuser.name);
                file.close();
                cout << "\n\n\t\t Registration Successfull\n";
                fstream log; log.open(files[3], ios::out | ios::app);
                t == 1? log << "New Librarian " << newuser.id << " Added by Admin.\n": log << "New Student " << newuser.id << " Registered.\n";
                log.close();
            }else
                cout << "\n\t\tInvalid Password!\n";
        }else
            cout << "\n\t\tInvalid ID!\n";
    }else
        cout << "\n\n\t\t I think you already have an Account.\n\t\tTry Logging in..." << endl;
    }else
        cout << "\n\t\tInvalid Name. Please Enter your full name!\n";
    cont(1);
}

// User Access Account
bool login(int t=0){
      clear();
    if (t==5)
        return true;

    char u[50], p[50], n[50];
    bool success = false;
    cout << "\n\n\t\t----------------- Enter Login Info -----------------\n";
    if(t==0)
        cout << "\n\t\tEnter your Student ID No: \t";
    else
        cout << "\n\t\tEnter your Librarian ID No: \t";
    cin.ignore();
    cin.getline(currentuser.id, 50);

    ifstream file(files[t]);

    while(file>>u>>p>>n){
        if(strcmp(currentuser.id, u)==0){
            strcpy(currentuser.name, n);
            t == 0 ? currentuser.type = "Student": currentuser.type = "Librarian";
            success = true;
            break;
        }
    }
    file.close();

    if(success)
        if (verifyPassword(t)){
            cout << "\n\n\t\tLog in Successfull.\n";
            fstream log; log.open(files[3], ios::out | ios::app);
            log << "New " << currentuser.type << " " << currentuser.id << " Logged in.\n";
            log.close();
        }
        else{
            success = false;
            cout << "\n\n\t\tIncorrect Password. Try again later.\n";
        }
    else{
        success = false;
        cout << "\n\n\t\tSorry, Can't find your ID in the list. Try registering.\n";
        }
    cont(1);
    return success;
}

void myInfo(){
      clear();
    cout << "\n\n\t\tName: " << replace(currentuser.name, '_', ' ') << endl;
    cout << "\n\t\t  Id: " << currentuser.id << endl;
    cout << "\n\t\tPass: " << currentuser.pass << endl;
    cout << "\n\t\tType: " << currentuser.type << endl;
    cont();
}

void info(){
    cout << "\n\t\t>> Logged in as " << currentuser.type << " " << currentuser.id << " <<\n";
}

int counter(int t=0){
    int count = -4;
    string getcontent;
    ifstream openfile (files[t]);
        if(openfile.is_open()){
            while(!openfile.eof()){
            getline (openfile, getcontent);
            count++;
        }
        openfile.close();
    }
    return count;
}

void inventory(){
    clear();
    cout << "\n\n\t\t----------------- CURRENT INVENTORY AND USER STATUS -----------------\n";
    cout << "\n\t\t    Total Books: " << counter(2);
    cout << "\n\t\t  Total Borrows: " << counter(7);
    cout << "\n\t\t Total Requests: " << counter(6);
    cout << "\n\t\t    Total Users: " << counter();
    cout << "\n\t\tBlacklist Users: " << counter(8);
    cout << "\n\t\tTotal Librarian: " << counter(1);
    cout << endl;
    cont();

}
// Delete Account
void deleteAcc(int t=0){
      clear();
    char uid[50], p[50], n[50], search[50], np[50]; string cp;
    cout << "\n\n\t\t>> Enter your ID: ";
    cin >> search;

    if(check(search, t) && strcmp(search, currentuser.id)==0){
        ifstream open(files[t]);
        ofstream write("temp.txt");
        while (open >> uid >> p >> n) {
            if (strcmp(uid, search) == 0) {
                cout << "\n\n\t\tYour Account will be delete.\n\n\t\tEnter your password to confirm: \t";
                cin >> cp;
                if(cp== securePassword(p, 'd')){
                    cout << "\n\n\t\tAccount Deleted successfully!\n";
                    cont();
                    fstream log; log.open(files[3], ios::out | ios::app);
                    log << currentuser.type << " " << currentuser.id << " Deleted his/her Account.\n";
                    log.close();
                }
                else{
                    write << setw(10) << left <<  uid << "\t" <<  setw(20) << left << p << "\t" << setw(20) << left <<  n << endl;
                    cout << "\n\n\t\tIncorrect Password! Logging out....";
                    cont();
                }
            }else
                write << setw(10) << left <<  uid << "\t" <<  setw(20) << left << p << "\t" << setw(20) << left <<  n << endl;
        }
        write.close();
        open.close();
        remove(files[t]);
        rename("temp.txt", files[t]);
        firstMenu();
    }else
        cout << "\n\n\t\t >> Incorrect ID.\n";
    cont();
}

// Recover Forgotten Password
void forgotpassword(int t=0){
      clear();
    char u[50], p[50], n[50];
    cout << "\n\n\t\t----------------- Password Recovery -----------------\n";
    user x;

    if (t==0)
        cout << "\n\t\tEnter your Student ID No: \t";
    else if (t==1)
        cout << "\n\t\tEnter your Librarian ID No: \t";
    cin.ignore();
    cin.getline(x.id, 10);

    ifstream file(files[t]);
    bool flag = false;
    while(file>>u>>p>>n){
        if(strcmp(u, x.id) == 0){
            flag = true;
            cout << "\n\n\t\tEnter your Full Name: \t";
            // cin.ignore();
            cin.getline(x.name, 50);
            if(n== replace(x.name)){
                cout << "\n\n\t\t>> Your password is: \t" << securePassword(p, 'd') << endl << "\n\t\tTry Not to forget Again!\n";
                fstream log; log.open(files[3], ios::out | ios::app);
                log << "Student " << x.id << " Recovered his/her Password.\n";
                log.close();
            }else
                cout << "\n\n\t\t >> Who are you?\n\n\t\tTry Registering.\n" << endl;
        }
    }
    file.close();
    if(!flag)
       cout << "\n\n\t\tSorry, Can't find your ID in the list. Try registering.";
    cont();
      clear();
}

// Librarian Add Books
void addbook(){
      clear();
    book newbook; int n;

    cout << "\n\n\t\t>> How many Books do you want to add? : \t";
    cin >> n;

    ofstream file;
    for(int i=0; i<n;i++){
        file.open(files[2],ios::app);
        cout << "\n\n\t\tEnter Book ID: ";
        cin >> newbook.id;
        if(!check(newbook.id, 2)){
            cin.ignore();
            cout << "\n\n\t\tEnter Book name: ";
            cin.getline(newbook.title, 50);

            cout << "\n\n\t\tEnter Author name: ";
            cin.getline(newbook.author, 50);
            file << setw(10) << left << newbook.id << "\t" << setw(40) << left<< replace(newbook.title, ' ', '_') << "\t" << setw(20) << left << replace(newbook.author, ' ', '_') << "\n";
            file.close();

            fstream log; log.open(files[3], ios::out | ios::app);
            log << currentuser.type << " " << currentuser.id << " Added new book " << newbook.id <<".\n";
            log.close();
        }else
            cout << "\n\n\t\tThere is a Book with this ID.\n\n\t\tCheck the Id or Delete the existing Book.\n"; break;
    }
    cont(1);
}

// User Search for a book
void searchbook(){
      clear();
    int choice;
    cout << "\n\n\t\t----------------- Search By -----------------\n" << endl;
    cout << "\n\t\t1. ID\n\n\t\t2. Title\n\n\t\t3. Author\n\n\t\tEnter your choice: \t";
    cin >> choice;
    switch (choice)
    {
    case 1:
        cout << "\n\n\t\tEnter Book ID to search: \t";
        break;
    case 2:
        cout << "\n\n\t\tEnter Book Title to search: \t";
        break;
    case 3:
        cout << "\n\n\t\tEnter Author Name to search: \t";
        break;
    }

    char value[4][50];
    cin.ignore();
    cin.getline(value[0], 50);
    replace2(value[0]);
    bool found = false;

    ifstream searchb(files[2]);
    cout << "\n\n\t\t>> Search Results: \t";
    while(searchb>>value[1]>>value[2]>>value[3]){
        int x = strncasecmp(value[0], value[choice], strlen(value[0]));
        if(x == 0){
            found = true;
            cout << "\n\t\t--------------\n";
            cout << "\t\t" << setw(10) << left << value[1] << setw(40) << left << replace(value[2], '_', ' ') << "by " <<setw(10) << left  << replace(value[3], '_', ' ');
        }
    }
    if (!found)
            cout << "\n\n\t\t>> Nothing found!";
    fstream log; log.open(files[3], ios::out | ios::app);
    log << currentuser.type << " " << currentuser.id << " Searched for book " << value[0] <<".\n";
    log.close();
    cont(1);
}

// Display All File
void showfile(int t=2){
      clear();
    string getcontent;
    ifstream openfile (files[t]);
        if(openfile.is_open()){
            while(!openfile.eof()){
            getline (openfile, getcontent);
            cout << "\n\t\t" << getcontent << endl; // these two lines alone in a while loop display ALL records
        }
        openfile.close();
     }
     cont();
}

// Change Book Info
void updatebook(){
      clear();
    char bid[50], bn[50], an[50], search[50];
    cout << "\n\n\t\t >> Enter the book id you want to modify: \t";
    cin >> search;

    if(check(search, 2)){
        ifstream open(files[2]);
        ofstream write("temp.txt");
        while (open >> bid >> bn >> an) {
            //Update data
            if (strcmp(bid, search) == 0) {
                cout << "\n\n\t\t>> Enter new records for the book with id " << bid;
                cout << "\n\n\t\tCurrent Book Name: \t" << bn <<"\n\n\t\tEnter new Book Name: \t";
                cin.ignore();
                cin.getline(bn, 50);
                cout << "\n\n\t\tCurrent Author Name: \t" << an  << "\n\n\t\tEnter new Author Name: \t";
                cin.getline(an, 50);
                fstream log; log.open(files[3], ios::out | ios::app);
                log << currentuser.type << " " << currentuser.id << " Updated book " << bid <<" info.\n";
                log.close();
            }
            write << setw(10) << left <<  bid << "\t" <<  setw(40) << left << replace(bn) << "\t" <<  setw(20) << left << replace(an) << endl;
        }
        write.close();
        open.close();
        remove(files[2]);
        rename("temp.txt", files[2]);
        cout << "\n\n\t\tUpdate successfull!\n\n";
    }else
        cout << "\n\n\t\tSorry, can't find a book with this ID.\n";
    cont(1);
}

// Remove Book Info
void deletebook(){
      clear();
    char bid[50], bn[50], an[50], search[50];
    cout << "\n\n\t\t>> Enter the book ID to delete: \t";
    cin >> search;

    if(check(search, 2)){
        ifstream open(files[2]);
        ofstream write("temp.txt");
        while (open >> bid >> bn >> an) {
            if (strcmp(bid, search) == 0) {
                string confirm; char y;
                cout << "\n\t\t----------------------------------\n";
                cout << bid << "\t" << replace(bn, '_', ' ') << " \tby " << replace(an, '_', ' ') << endl;
                cout << "\t\t----------------------------------\n";
                cout << "\n\n\t\t >> Are you sure? Do you want to delete the this book's data(Y/N)? \t";
                cin >> confirm;

                if (confirm[0] == 'Y' || confirm[0] == 'y')
                    y = 'y';
                else
                    y = 'n';

                switch(y){
                    case 'y':
                    case 'Y':{
                        cout << "\n\n\t\t>> Deleted successfully!\n";
                        fstream log; log.open(files[3], ios::out | ios::app);
                        log << currentuser.type << " " << currentuser.id << " Deleted book " << bid <<".\n";
                        log.close();
                        }break;
                    default:
                        cout << "\n\n\t\t>> Deletion canceled!\n";
                        write << setw(10) << left << bid << "\t" << setw(40) << left << bn << "\t" << setw(20) << left << an << endl;
                        break;
                }
                continue;
            }
            else{
                write << setw(10) << left << bid << "\t" << setw(40) << left << bn << "\t" << setw(20) << left << an << endl;
            }
        }
        write.close();
        open.close();
        remove(files[2]);
        rename("temp.txt", files[2]);
    }else
        cout << "\n\n\t\t >> Sorry, can't find a book with this ID.\n";
    cont();
}

// Request to borrow Book
void borrowBook(){
      clear();
    string bn, an;
    char value[50], bid[50];
    cin.ignore();
    cout << "\n\n\t\t>> Enter book ID to borrow: \t";
    cin.getline(value, 50);
    if(check(value, 2)){
        if(!check(value, 7)){
            ifstream open(files[2]);
            ofstream write(files[6], ios::app);
            while (open >> bid >> bn >> an) {
                if (strcmp(bid, value) == 0) {
                    string confirm; char y;
                    cout << "\n----------------------------------\n";
                    cout << bid << "\t" << bn << "\tby " << an << endl;
                    cout << "\n----------------------------------\n";
                    cout << "\n\n\t\t >> Are you sure? Do you want to borrow the this book(Y/N)? ";
                    cin >> confirm;

                    if (confirm[0] == 'Y' || confirm[0] == 'y')
                        y = 'y';
                    else
                        y = 'n';

                    switch(y){
                        case 'y':
                        case 'Y':{
                            write << setw(10) << left << bid << "\t" << setw(40) << left << bn << "\t" << setw(20) << left << currentuser.id << endl;
                            cout << "\n\n\t\t>> Request Successfull" << endl;
                            fstream log; log.open(files[3], ios::out | ios::app);
                            log << currentuser.type << " " << currentuser.id << " Requested to borrow book " << bid <<".\n"; log.close();
                            }break;
                        default:
                            cout << "\n\n\t\tRequest canceled!\n";
                            break;
                    }
                }
            }
            write.close();
        }else
            cout << "\n\n\t\t>> Someone Already borrowed the book! You can try another one." << endl;
    }else
        cout << "\n\n\t\t>> I couldn't find the book. Check your Book ID!" << endl;
    cont(1);
}

// Change account name
void changename(int t=0){
      clear();
    char uid[50], p[50], n[50], search[50], nn[50]; string np;
    cout << "\n\n\t\t >> Enter your ID: \t";
    cin >> search;

    if(check(search, t) && strcmp(search, currentuser.id)==0){
        ifstream open(files[t]);
        ofstream write("temp.txt");
        while (open >> uid >> p >> n) {
            if (strcmp(uid, search) == 0) {
                cout << "\n\n\t\t Current Name: \t" << replace(n, '_', ' ') <<"\n\n\t\tEnter new Name: \t";
                cin.ignore();
                cin.getline(nn, 50);
                cout << "\n\n\t\tEnter your password to confirm: \t";
                cin >> np;
                if(np== securePassword(p, 'd')){
                    strcpy(currentuser.name, nn);
                    write << setw(10) << left <<  uid << "\t" <<  setw(20) << left << p << "\t" <<  setw(20) << left << replace(nn) << endl;
                    cout << "\n\n\t\tUpdate successfull!\n";
                    fstream log; log.open(files[3], ios::out | ios::app);
                    log << currentuser.type << " " << currentuser.id << " Changed his/her name from " << n << " to " << nn <<".\n";
                    log.close();
                }
                else{
                    write << setw(10) << left <<  uid << "\t" <<  setw(20) << left << p << "\t" << setw(20) << left <<  n << endl;
                    cout << "\n\n\t\tIncorrect Password\n";
                }
            }else
                write << setw(10) << left <<  uid << "\t" <<  setw(20) << left << p << "\t" << setw(20) << left <<  n << endl;
        }
        write.close();
        open.close();
        remove(files[t]);
        rename("temp.txt", files[t]);
    }else
        cout << "\n\n\t\t>> Incorrect ID.\n";
    cont();
}

// Change account Password
void changepass(int t=0){
      clear();
    char uid[50], p[50], n[50], search[50], np[50]; string cp;
    cout << "\n\n\t\tEnter your ID: \t";
    cin >> search;

    if(check(search, t) && strcmp(search, currentuser.id)==0){
        ifstream open(files[t]);
        ofstream write("temp.txt");
        while (open >> uid >> p >> n) {
            if (strcmp(uid, search) == 0) {
                cout << "\n\n\t\tEnter your current password: \t";
                cin >> cp;
                if(cp== securePassword(p, 'd')){
                    cout <<"\n\n\t\tEnter new Password: \t";
                    cin.ignore();
                    cin.getline(np, 50);
                    write << setw(10) << left <<  uid << "\t" <<  setw(20) << left << securePassword(np) << "\t" <<  setw(20) << left << n << endl;
                    strcpy(currentuser.pass, np);
                    cout << "\n\n\t\t>> Password changed successfully!\n";
                    fstream log; log.open(files[3], ios::out | ios::app);
                    log << currentuser.type << " " << currentuser.id << " Changed password."<<".\n";
                    log.close();
                }
                else{
                    write << setw(10) << left <<  uid << "\t" <<  setw(20) << left << p << "\t" << setw(20) << left <<  n << endl;
                    cout << "\n\n\t\t>>Incorrect Password!\n";
                }
            }else
                write << setw(10) << left <<  uid << "\t" <<  setw(20) << left << p << "\t" << setw(20) << left <<  n << endl;
        }
        write.close();
        open.close();
        remove(files[t]);
        rename("temp.txt", files[t]);
    }else
        cout << "\n\n\t\t>> Incorrect ID.\n";
    cont(1);
}

// Account Menu
void accountManagement(int t=0){
      clear();
    int choice;
    do{
        cout << "\n\n\t\t----------------- ACCOUNT SETTING -----------------\n\n"; info();
        cout << "\n\n\t\t1. My Information\n\n\t\t2. Change Name\n\n\t\t3. Change Password\n\n\t\t4. Delete Account \n\n\t\t6. Back\n\n\t\t0. Exit\n\n\t\t>>Enter your choice: \t";
        cin >> choice;
        switch (choice)
        {
        case 1:
            myInfo();
            break;
        case 2:
            changename(t);
            break;
        case 3:
            changepass(t);
            break;
        case 4:
            deleteAcc(t);
            break;
        case 5:
            // requestbook();
            break;
        case 6:
            break;
        case 0:
            exit(0);
        default:
            clear();
            cout << "\n\n\t\t>> Wrong Input! \n\n\t\t Try Again!";
            break;
        }
    }while (choice != 6);
      clear();
}

// Student Menu
void studentPage(){
      clear();
    int choice;
    do{
        cout << "\n\n\t\t----------------- WELCOME TO AASTU LMS -----------------\n\n";info();
        cout << "\n\n\t\t1. Show Books\n\n\t\t2. Search Book\n\n\t\t3. Request to Borrow Book\n\n\t\t4. Account Setting\n\n\t\t6. Back\n\n\t\t0. Exit\n\n\t\t>> Enter your choice: \t";
        cin >> choice;
        switch (choice)
        {
        case 1:
            showfile();
            break;
        case 2:
            searchbook();
            break;
        case 3:
            borrowBook();
            break;
        case 4:
            accountManagement();
            break;
        case 5:
            // requestbook();
            break;
        case 6:
            break;
        case 0:
            exit(0);
        default:
            clear();
            cout << "\n\n\t\t>> Wrong Input! \n\n\t\t Try Again!";
            break;
        }
    }while (choice != 6);
      clear();
}

// Student Log in menu
void studentMenu(){
    clear();
    int choice;
    do{
        cout << "\n\n\t\t----------------- User Login -----------------\n";
        // cout << "\n\n\t\t>> please Enter any option \n\n";
        cout << "\n\t\t1. Log in\n\n\t\t2. Register\n\n\t\t3. Forgot Password\n\n\t\t4. Back\n\n\t\t0. Exit\n\n\t\t>> Enter your choice: \t";
        cin >> choice;
        switch (choice)
        {
        case 1:
            if(login())
                studentPage();
            break;
        case 2:
            signin();
            break;
        case 3:
            forgotpassword();
            break;
        case 0:
            exit(0);
        default:
            clear();
            cout << "\n\n\t\t>> Wrong Input! \n\n\t\t Try Again!";
            break;
        }
    }while(choice!=4);
    clear();
}

void bookManagement(){
      clear();
    int choice;
    do{
        cout << "\n\n\t\t----------------- BOOK MANAGEMENT -----------------\n"; info();
        cout << "\n\n\t\t1. Add Book\n\n\t\t2. Update Book\n\n\t\t3. Delete Book\n\n\t\t4. Show Books\n\n\t\t5.\n\n\t\t6. Back\n\n\t\t0. Exit\n\n\t\t>> Enter your choice: \t";
        cin >> choice;
        switch (choice)
        {
        case 1:
            addbook();
            break;
        case 2:
            updatebook();
            break;
        case 3:
            deletebook();
            break;
        case 4:
            showfile(2);
            break;
        case 5:

            break;
        case 6:
            break;
        case 0:
            exit(0);
        default:
            cout << "\n\n\t\t>> Wrong Input! \n See you later!";
            break;
        }
    }while (choice != 6);
      clear();
}

// Librarian accept book borrow request
void acceptrequest(){
      clear();
    string bn, uid;
    char value[50], bid[50];
    cin.ignore();
    cout << "\n\n\t\t>> Enter book ID to request borrow: \t";
    cin.getline(value, 50);
    if(check(value, 6)){
        if(!check(value, 7)){
            ifstream open(files[6]);
            ofstream write(files[7], ios::app);
            ofstream neww("temp.txt");
            while (open >> bid >> bn >> uid) {
                if (strcmp(bid, value) == 0) {
                    string confirm; char y;
                    cout <<"\n\n\t\tUser ID: \t" << uid << endl << "\n\t\tBook ID: \t" << bid << endl << "\n\t\tBook Title: \t" << bn << endl;
                    cout << "\n\t\t>> Confirm Transaction? y/n :";
                    cin >> confirm;

                    if (confirm[0] == 'Y' || confirm[0] == 'y')
                        y = 'y';
                    else
                        y = 'n';

                    switch(y){
                        case 'y':
                        case 'Y':
                            write << setw(10) << left << bid << "\t" << setw(40) << left << bn << "\t" << setw(20) << left << uid << endl;
                            cout << "\n\t\t>> Transfer Successfull\n" << endl;
                            cout << "\n\t\tUser with ID: \t" << uid << " lent The Book: \t" << bn << endl;
                            break;
                        default:
                            cout << "\n\n\t\t>> Transfer canceled!\n";
                            break;
                    }
                }
            }
            write.close();
            open.close();
            remove(files[6]);
            rename("temp.txt", files[6]);
        }else
            cout << "\n\n\t\t>> Book Already lent!\n" << endl;
    }else
        cout << "\n\n\t\tNo Request for This Book!\n";
    cont(1);
}

// Librarian accept book return
void acceptreturn(){
    string bn, uid;
    char value[50], bid[50];
    cin.ignore();
    cout << "\n\n\t\tEnter book ID to return: ";
    cin.getline(value, 50);
    if(check(value, 7)){
            ifstream open(files[7]);
            // ofstream write(files[7], ios::app);
            ofstream neww("temp.txt");
            while (open >> bid >> bn >> uid) {
                if (strcmp(bid, value) == 0) {
                    string confirm; char y;
                    cout <<"User ID: " << uid << endl << "Book ID: " << bid << endl << "Book Title: " << bn << endl;
                    cout << "Confirm Book Return? y/n :";
                    cin >> confirm;

                    if (confirm[0] == 'Y' || confirm[0] == 'y')
                        y = 'y';
                    else
                        y = 'n';

                    switch(y){
                        case 'y':
                        case 'Y':
                            // write << setw(10) << left << bid << "\t" << setw(40) << left << bn << "\t" << setw(20) << left << uid << endl;
                            cout << "\nBook Return Successfull\n" << endl;
                            cout << "User with ID: " << uid << " returned The Book: " << bn << endl;
                            break;
                        default:
                            neww << setw(10) << left << bid << "\t" << setw(40) << left << bn << "\t" << setw(20) << left << uid << endl;
                            cout << "\nTransfer canceled!\n";
                            break;
                    }
                }
                else
                    neww << setw(10) << left << bid << "\t" << setw(40) << left << bn << "\t" << setw(20) << left << uid << endl;
            }
            open.close();
            remove(files[7]);
            rename("temp.txt", files[7]);
    }else
        cout << "\n\n\t\tBook not Lent!\n";
    cont();
}


// Librarian accept book return
void addblacklist(){
    string un, up;
    char value[50], uid[50], reason[100];
    cin.ignore();
    cout << "\n\n\t\tEnter User ID to add to blacklist: ";
    cin.getline(value, 50);
    if(check(value, 0)){
            ifstream open(files[7]);
            // ofstream write(files[7], ios::app);
            ofstream neww("temp.txt");
            while (open >> uid >> up >> un) {
                if (strcmp(uid, value) == 0) {
                    string confirm; char y;
                    cout <<"\n\t\tUser ID: " << uid << endl << "\n\t\tUser name: " << un << endl;
                    cout << "\n\n\t\tConfirm to add user to blacklist? y/n :";
                    cin >> confirm;

                    if (confirm[0] == 'Y' || confirm[0] == 'y')
                        y = 'y';
                    else
                        y = 'n';

                    switch(y){
                        case 'y':
                        case 'Y':
                            cout << endl << "\n\n\t\tEnter Reason: " ;
                            cin.ignore();
                            cin.getline(reason, 100);

                            // write << setw(10) << left << bid << "\t" << setw(40) << left << bn << "\t" << setw(20) << left << uid << endl;
                            cout << "\n\n\t\tUser " << uid << "added to blacklist for " << reason << endl;
                            break;
                        default:
                            neww << setw(10) << left << uid << "\t" << setw(20) << left << up << "\t" << setw(20) << left << un << endl;
                            cout << "\n\n\t\tProcess canceled!\n";
                            break;
                    }
                }
                else
                    neww << setw(10) << left << uid << "\t" << setw(40) << left << up << "\t" << setw(20) << left << un << endl;
            }
            // write.close();
            open.close();
            remove(files[7]);
            rename("temp.txt", files[7]);
    }else
        cout << "\n\n\t\tNo user with this ID found\n";
    cont();
}

void showUsers(int t=0){
    char u[50], p[50], n[50];
     switch(t){
     case 0:
        cout << "\n\n\t\t----------------- Users List -----------------\n";
        break;
     case 1:
        cout << "\n\n\t\t----------------- Blacklist -----------------\n";
        break;
     case 8:
        cout << "\n\n\t\t----------------- Librarian List-----------------\n";
        break;
     default:
        break;
        }
    user x;

    cin.getline(x.id, 10);

    ifstream file(files[t]);
    bool flag = false;
    while(file>>u>>p>>n){
        cout << "\n\n\t\t" << setw(10) << left << u << setw(10) << right << n << endl;
    }
    file.close();
    cont(1);
}

// Lend Transaction Menu
void lendManagement(){
      clear();
    int choice;
    do{
        cout << "\n\n\t\t----------------- BOOK LEND MANAGEMENT -----------------\n "; info();
        cout << "\n\n\t\t1. Show Requests\n\n\t\t2. Accept Requests\n\n\t\t3. Show Borrowed Books\n\n\t\t4. Accept Book Return\n\n\t\t5. \n\n\t\t6. Back\n\n\t\t0. Exit\n\n\t\t>> Enter your choice: \t";
        cin >> choice;
        switch (choice){
        case 1:
            showfile(6);
            break;
        case 2:
            acceptrequest();
            break;
        case 3:
            showfile(7);
            break;
        case 4:
            acceptreturn();
            break;
        case 5:

            break;
        case 6:
            break;
        case 0:
            exit(0);
        default:
            clear();
            cout << "\n\n\t\t>> Wrong Input! \n\n\t\t Try Again!";
            break;
        }
    }while (choice != 6);
      clear();
}

// Users List Menu
void userManagement()
{
    clear();
    int choice;
    do{
        cout << "\n\n\t\t----------------- BOOK MANAGEMENT -----------------\n"; info();
        cout << "\n\n\t\t1. Show Users\n\n\t\t2. Blacklists\n\n\t\t3. Add to Blacklist\n\n\t\t4. Remove from Blacklist\n\n\t\t5. \n\n\t\t6. Back\n\n\t\t0. Exit\n\n\t\t>> Enter your choice: \t";
        cin >> choice;
        switch (choice){
        case 1:
            showUsers();
            break;
        case 2:
            showUsers(8);
            break;
        case 3:
            addblacklist();
            break;
        case 4:
            // removeblacklist();
            break;
        case 5:

            break;
        case 6:
            break;
        case 0:
            exit(0);
        default:
            clear();
            cout << "\n\n\t\t>> Wrong Input! \n\n\t\t Try Again!";
            break;
        }
    }while (choice != 6);
      clear();
}

// Librarian Menu
void librarianMenu(int t=1){
    clear();
    if (login(t)){
        int choice;
        do{
            cout << "\n\n\t\t----------------- WELCOME TO AASTU LMS -----------------\n\n"; info();
            cout << "\n\n\t\t1. Manage Books\n\n\t\t2. Transactions\n\n\t\t3. Manage Users\n\n\t\t4. Inventory\n\n\t\t5. Activity Log\n\n\t\t6. Account Setting\n\n\t\t7. Back\n\n\t\t0. Exit\n\n\t\t>> Enter your choice: \t";
            cin >> choice;

            switch (choice){
            case 1:
                bookManagement();
                break;
            case 2:
                lendManagement();
                break;
            case 3:
                userManagement();
                break;
            case 4:
                inventory();
                break;
            case 5:
                showfile(3);
                break;
            case 6:
                accountManagement(1);
                break;
            case 7:
                break;
            case 0:
                exit(0);
            default:
                clear();
                cout << "\n\n\t\t>> Wrong Input! \n\n\t\t Try Again!";
                break;
            }
        }while (choice != 7);
        clear();
    }
}

// Admin Menu
void adminMenu(){
    clear();
    int choice;
    currentuser.type = "Admin";
    for(int i=0; i<5; i++){
        currentuser.name[i] = currentuser.type[i];
        currentuser.id[i] = ' ';}

    do{
        cout << "\n\n\t\t----------------- WELCOME ADMIN -----------------\n\n"; info();
        cout << "\n\n\t\t1. Add Librarian\n\n\t\t2. Librarian List\n\n\t\t3. Activity Log\n\n\t\t4. Librarian mode\n\n\t\t5. Student Mode\n\n\t\t6. Back\n\n\t\t0. Destroy System\n\n\t\t>> Enter your choice: \t";
        cin >> choice;
        switch (choice){
        case 1:
            signin(1);
            break;
        case 2:
            showUsers(1);
            break;
        case 3:
            showfile(3);
            break;
        case 4:
            librarianMenu(5);
            break;
        case 5:
            studentPage();
            break;
        case 6:
            inventory();
            break;
        case 7:
            break;
        case 0:
            clear();
            cout << "\n\n\t\tBOOOOOOOOOOOOOMMMMMMM" << endl;
            exit(0);
        default:
            clear();
            cout << "\n\n\t\t>> Wrong Input! \n\n\t\t Try Again!";
            break;
        }
    }while (choice != 7);
      clear();
}

// Main Menu
void firstMenu(){
    int choice;
    do {
        cout << "\n\n\t\t----------------- WELCOME TO AASTU LMS -----------------\n" << endl;
        cout << "\t\t >> I'm: \n\n\t\t1. Student\n\n\t\t2. Librarian\n\n\t\t3. Exit\n\n\t\t0. Admin\n\n\t\tEnter your choice: \t";
        cin >> choice;

        switch (choice){
        case 1:
            studentMenu();
            break;
        case 2:
            librarianMenu();
            break;
        case 3:
            break;
        case 0:
            clear();
            int x; cout << "\n\n\t\t>> Passcode: \t"; cin>>x;
            x==9 ? adminMenu() : exit(0);
            break;
        default:
            clear();
            cout << "\n\n\t\t>> Wrong Input! Try Again";
            break;
        }
    }while(choice != 3);
    clear();
    cout << "\n\n\t\tBye, See you later";
}

int main()
{
    firstMenu();
    return 0;
}
