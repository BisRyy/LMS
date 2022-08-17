#include <iostream>
#include <cstring>
#include <string.h>
#include <fstream>
#include <iomanip>

using namespace std;

// system("color=04")
char files[10][50] {"Users.txt", "Librarians.txt", "Books.txt", "activitylog.txt", "report.txt", "temp.txt", "Requests.txt", "accepted.txt", "Blacklist.txt"};

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
    cout << "\nPress Enter to Continue: ";
    if(t==0)
        cin.ignore();
    cin.getline(x, 50);
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
    string p, n; char u[50];
    bool verified = false;

    cout << "\nHi " << replace(currentuser.name, '_', ' ') << ",\nEnter your Password: ";
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

    return verified;
}

// Replace ' ' blank space for files
string replace(char *value, char x = ' ', char y= '_'){
    string v;
    for(int i=0; i<strlen(value); i++){
        if (value[i] == x)
            value[i] = y;
        v += value[i];
    }
    return v;
}

// char* replace2(char value[], char x = ' ', char y= '_'){
//     char v[50];
//     for(int i=0; i<strlen(value); i++){
//         if (value[i] == x)
//             value[i] = y;
//         v[i] = value[i];
//     } 
//     return *v;
// }

// Check if the search value exists in the file
bool check(char x[50], int t=0){
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
    user newuser;
    cout << "\n-----------------\nEnter Signin Info\n-----------------\n";

    cin.ignore();
    cout << "Enter Full Name: ";
    cin.getline(newuser.name, 50);

    cout << "\nHi " << newuser.name << ",\nEnter your Id No: ";
    cin.getline(newuser.id, 10);
    if(!check(newuser.id, t)){
        // cin.ignore();
        cout << "Enter Password: ";
        cin.getline(newuser.pass, 50);

        fstream file;
        file.open(files[t], ios::out | ios::app);
        file  << "\n" << setw(10) << left <<  newuser.id << "\t" <<  setw(20) << left << securePassword(newuser.pass) << "\t" << setw(20) << left <<  replace(newuser.name);
        file.close();
        cout << "Registration Successfull\n";
    }
    else
        cout << "I think you already have an Account.\nTry Logging in." << endl;
    cont(1);
}

// User Access Account
bool login(int t=0){
    if (t==5)
        return true;

    char u[50], p[50], n[50];
    bool success = false;
    cout << "\n-----------------\nEnter Login Info\n-----------------\n";
    if(t==0)
        cout << "Enter your Student ID No: ";
    else
        cout << "Enter your Librarian ID No: ";
    cin.ignore();
    cin.getline(currentuser.id, 50);

    ifstream file(files[t]);

    while(file>>u>>p>>n){
        if(strcmp(currentuser.id, u)==0){
            strcpy(currentuser.name, n);
            strcpy(currentuser.pass, p);
            t == 0 ? currentuser.type = "Student": currentuser.type = "Librarian";
            success = true;
            break;
        }
    }
    file.close();

    if(success)
        if (verifyPassword(t)){
            strcpy(currentuser.name, n);
            strcpy(currentuser.id, u);
            cout << "\nLog in Successfull.\n";
        }
        else{
            success = false;
            cout << "\nIncorrect Password. Try again later.\n";
        }
    else{
        success = false;
        cout << "\nSorry, Can't find your ID in the list. Try registering.\n";
        }
    cont(1);
    return success;
}

void myInfo(){
    cout << "Name: " << replace(currentuser.name, '_', ' ') << endl;
    cout << "  Id: " << currentuser.id << endl;
    cout << "Pass: " << currentuser.pass << endl;
    cout << "Type: " << currentuser.type << endl;
    cont();
}

void info(){
    cout << "\t\tLogged in as " << currentuser.type << " " << currentuser.id;
}

// Delete Account
void deleteAcc(int t=0){
    char uid[50], p[50], n[50], search[50], np[50]; string cp;
    cout << "Enter your ID: ";
    cin >> search;
    
    if(check(search, t) && strcmp(search, currentuser.id)==0){
        ifstream open(files[t]);
        ofstream write("temp.txt");
        while (open >> uid >> p >> n) {
            if (strcmp(uid, search) == 0) {
                cout << "\nYour Account will be delete.\nEnter your password to confirm: ";
                cin >> cp;
                if(cp== securePassword(p, 'd')){
                    cout << "\nAccount Deleted successfully!\n";  
                }
                else{
                    write << setw(10) << left <<  uid << "\t" <<  setw(20) << left << p << "\t" << setw(20) << left <<  n << endl;
                    cout << "\nIncorrect Password!\nLogging out....";
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
        cout << "Incorrect ID.\n";
    cont();
}

// Recover Forgotten Password
void forgotpassword(int t=0){
    char u[50], p[50], n[50];
    cout << "\n-----------------\nPassword Recovery\n-----------------\n";
    user x;

    if (t==0)
        cout << "Enter your Student ID No: ";
    else if (t==1)
        cout << "Enter your Librarian ID No: ";
    cin.ignore();
    cin.getline(x.id, 10);

    ifstream file(files[t]);
    bool flag = false;
    while(file>>u>>p>>n){
        if(strcmp(u, x.id) == 0){
            flag = true;
            cout << "Enter your Full Name: ";
            // cin.ignore();
            cin.getline(x.name, 50);
            if(n== replace(x.name)){
                cout << "\nYour password is: " << securePassword(p, 'd') << endl << "Try Not to forget Again!\n";
            }else
                cout << "\nWho are you?\nTry Registering.\n" << endl;
        }
    }
    file.close();
    if(!flag)
       cout << "\nSorry, Can't find your ID in the list. Try registering.";
    cont();
}

// Librarian Add Books
void addbook(){
    book newbook; int n;

    cout << "How many Books do you want to add? :";
    cin >> n;

    ofstream file;
    for(int i=0; i<n;i++){    
        file.open(files[2],ios::app);
        cout << "\nEnter Book ID: ";
        cin >> newbook.id;
        if(!check(newbook.id, 2)){
            cin.ignore();
            cout << "Enter Book name: ";
            cin.getline(newbook.title, 50);

            cout << "Enter Author name: ";
            cin.getline(newbook.author, 50);
            file << setw(10) << left << newbook.id << "\t" << setw(40) << left<< replace(newbook.title, ' ', '_') << "\t" << setw(20) << left << replace(newbook.author, ' ', '_') << "\n";
            file.close();
        }else
            cout << "\nThere is a Book with this ID.\nCheck the Id or Delete the existing Book.\n"; break;
    }
    cont();
}

// User Search for a book
void searchbook(){
    int choice;
    cout << "\n-----------------\nSearch By:\n-----------------\n" << endl;
    cout << "1.ID\n2.Title\n3.Author\n\nEnter your choice: ";
    cin >> choice;
    switch (choice)
    {
    case 1:
        cout << "Enter Book ID to search: ";
        break;
    case 2:
        cout << "Enter Book Title to search: ";
        break;
    case 3:
        cout << "Enter Author Name to search: ";
        break;
    }

    char value[4][50];
    cin.ignore();
    cin.getline(value[0], 50);

    bool found = false;

    ifstream searchb(files[2]);
    cout << "Search Results:";
    while(searchb>>value[1]>>value[2]>>value[3]){
        int x = strncasecmp(value[0], value[choice], strlen(value[0]));
        if(x == 0){
            found = true;
            cout << "\n\t--------------\n";
            cout << setw(10) << left << value[1] << setw(40) << left << replace(value[2], '_', ' ') << "by " <<setw(10) << left  << replace(value[3], '_', ' ');
        }
    }
    if (!found)
            cout << "Nothing found!";
    cont(1);
}

// Display All File
void showfile(int t=2){
    string getcontent;
    ifstream openfile (files[t]);
        if(openfile.is_open()){
            while(!openfile.eof()){
            getline (openfile, getcontent);
            cout << getcontent << endl; // these two lines alone in a while loop display ALL records
        }
        openfile.close();
     }
     cont();
}

// Change Book Info
void updatebook(){
    char bid[50], bn[50], an[50], search[50];
    cout << "Enter the book id you want to modify: ";
    cin >> search;
    
    if(check(search, 2)){
        ifstream open(files[2]);
        ofstream write("temp.txt");
        while (open >> bid >> bn >> an) {
            //Update data
            if (strcmp(bid, search) == 0) {
                cout << "\n\tEnter new records for the book with id " << bid;
                cout << "\nCurrent Book Name: " << bn <<"\nEnter new Book Name: ";
                cin.ignore();
                cin.getline(bn, 50);
                cout << "\nCurrent Author Name: " << an  << "\nEnter new Author Name: ";
                cin.getline(an, 50);
            }
            write << setw(10) << left <<  bid << "\t" <<  setw(40) << left << replace(bn) << "\t" <<  setw(20) << left << replace(an) << endl;
        }
        write.close();
        open.close();
        remove(files[2]);
        rename("temp.txt", files[2]);
        cout << "\nUpdate successfull!\n";
    }else
        cout << "Sorry, can't find a book with this ID.\n";
    cont();
}

// Remove Book Info
void deletebook(){
    char bid[50], bn[50], an[50], search[50];
    cout << "Enter the book ID to delete: ";
    cin >> search;

    if(check(search, 2)){
        ifstream open(files[2]);
        ofstream write("temp.txt");
        while (open >> bid >> bn >> an) {
            if (strcmp(bid, search) == 0) {
                string confirm; char y;
                cout << "\n----------------------------------\n";
                cout << bid << "\t" << replace(bn, '_', ' ') << " \tby " << replace(an, '_', ' ') << endl;
                cout << "----------------------------------\n";
                cout << "Are you sure? Do you want to delete the this book's data? ";
                cin >> confirm;

                if (confirm[0] == 'Y' || confirm[0] == 'y')
                    y = 'y';
                else
                    y = 'n';

                switch(y){
                    case 'y':
                    case 'Y':
                        cout << "\nDeleted successfully!\n";
                        break;
                    default:
                        cout << "\nDeletion canceled!\n";
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
        cout << "Sorry, can't find a book with this ID.\n";
    cont();
}

// Request to borrow Book
void borrowBook(){
    string bn, an;
    char value[50], bid[50];
    cin.ignore();
    cout << "Enter book ID to borrow: ";
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
                    cout << "Are you sure? Do you want to borrow the this book? ";
                    cin >> confirm;
                    
                    if (confirm[0] == 'Y' || confirm[0] == 'y')
                        y = 'y';
                    else
                        y = 'n';

                    switch(y){
                        case 'y':
                        case 'Y':
                            write << setw(10) << left << bid << "\t" << setw(40) << left << bn << "\t" << setw(20) << left << currentuser.id << endl;
                            cout << "Request Successfull" << endl;
                            break;
                        default:
                            cout << "\nRequest canceled!\n";
                            break;
                    }
                }
            }
            write.close();
        }else
            cout << "Someone Already borrowed the book! You can try another one." << endl;
    }else
        cout << "I couldn't find the book. Check your Book ID!" << endl;
    cont(1);
}

// Change account name
void changename(int t=0){
    char uid[50], p[50], n[50], search[50], nn[50]; string np;
    cout << "Enter your ID: ";
    cin >> search;
    
    if(check(search, t) && strcmp(search, currentuser.id)==0){
        ifstream open(files[t]);
        ofstream write("temp.txt");
        while (open >> uid >> p >> n) {
            if (strcmp(uid, search) == 0) {
                cout << "\nCurrent Name: " << replace(n, '_', ' ') <<"\nEnter new Name: ";
                cin.ignore();
                cin.getline(nn, 50);
                cout << "\nEnter your password to confirm: ";
                cin >> np;
                if(np== securePassword(p, 'd')){
                    strcpy(currentuser.name, nn);
                    write << setw(10) << left <<  uid << "\t" <<  setw(20) << left << p << "\t" <<  setw(20) << left << replace(nn) << endl;
                    cout << "\nUpdate successfull!\n";  
                }
                else{
                    write << setw(10) << left <<  uid << "\t" <<  setw(20) << left << p << "\t" << setw(20) << left <<  n << endl;
                    cout << "\nIncorrect Password\n";
                }
            }else
                write << setw(10) << left <<  uid << "\t" <<  setw(20) << left << p << "\t" << setw(20) << left <<  n << endl;
        }
        write.close();
        open.close();
        remove(files[t]);
        rename("temp.txt", files[t]);
    }else
        cout << "Incorrect ID.\n";
    cont();
}

// Change account Password
void changepass(int t=0){
    char uid[50], p[50], n[50], search[50], np[50]; string cp;
    cout << "Enter your ID: ";
    cin >> search;
    
    if(check(search, t) && strcmp(search, currentuser.id)==0){
        ifstream open(files[t]);
        ofstream write("temp.txt");
        while (open >> uid >> p >> n) {
            if (strcmp(uid, search) == 0) {
                cout << "\nEnter your current password: ";
                cin >> cp;
                if(cp== securePassword(p, 'd')){
                    cout <<"\nEnter new Password: ";
                    cin.ignore();
                    cin.getline(np, 50);
                    write << setw(10) << left <<  uid << "\t" <<  setw(20) << left << securePassword(np) << "\t" <<  setw(20) << left << n << endl;
                    strcpy(currentuser.pass, np);
                    cout << "\nPassword changed successfully!\n";  
                }
                else{
                    write << setw(10) << left <<  uid << "\t" <<  setw(20) << left << p << "\t" << setw(20) << left <<  n << endl;
                    cout << "\nIncorrect Password!\n";
                }
            }else
                write << setw(10) << left <<  uid << "\t" <<  setw(20) << left << p << "\t" << setw(20) << left <<  n << endl;
        }
        write.close();
        open.close();
        remove(files[t]);
        rename("temp.txt", files[t]);
    }else
        cout << "Incorrect ID.\n";
    cont();
}

// Account Menu
void accountManagement(int t=0){
    int choice;
    do{
        cout << "\n-----------------\nACCOUNT SETTING"; info();
        cout << "\n-----------------\n" << endl;
        cout << "1.My Information\n2.Change Name\n3.Change Password\n4.Delete Account \n6.Back\n0.Exit\n\nEnter your choice: ";
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
            cout << "Wrong Input! \n See you later!";
            break;
        }
    }while (choice != 6);   
}

// Student Menu
void studentPage(){
    int choice;
    do{
        cout << "\n-----------------\nWELCOME TO AASTU LMS";info();
        cout << "\n-----------------\n" << endl;
        cout << "1.Show Books\n2.Search Book\n3.Request to Borrow Book\n4.Account Setting\n6.Back\n0.Exit\n\nEnter your choice: ";
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
            cout << "Wrong Input! \n See you later!";
            break;
        }
    }while (choice != 6);
}

// Student Log in menu
void studentMenu(){
    // clear();
    int choice;
    do{
        cout << "\n-----------------\nUser Login";
        cout << "\n-----------------\n" << endl;
        cout << "1.Log in\n2.Register\n3.Forgot Password\n4.Back\n0.Exit\n\nEnter your choice: ";
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
            break;
        }
    }while(choice!=4);
}

void bookManagement(){
    int choice;
    do{
        cout << "\n-----------------\nBOOK MANAGEMENT"; info();
        cout << "\n-----------------\n" << endl;
        cout << "1.Add Book\n2.Update Book\n3.Delete Book\n4.Show Books\n5.\n6.Back\n0.Exit\n\nEnter your choice: ";
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
            cout << "Wrong Input! \n See you later!";
            break;
        }
    }while (choice != 6);
}

// Librarian accept book borrow request
void acceptrequest(){
    string bn, uid;
    char value[50], bid[50];
    cin.ignore();
    cout << "Enter book ID to request borrow: ";
    cin.getline(value, 50);
    if(check(value, 6)){
        if(!check(value, 7)){
            ifstream open(files[6]); 
            ofstream write(files[7], ios::app);
            ofstream neww("temp.txt");
            while (open >> bid >> bn >> uid) {
                if (strcmp(bid, value) == 0) {
                    string confirm; char y;
                    cout <<"User ID: " << uid << endl << "Book ID: " << bid << endl << "Book Title: " << bn << endl;
                    cout << "Confirm Transaction? y/n :";
                    cin >> confirm;

                    if (confirm[0] == 'Y' || confirm[0] == 'y')
                        y = 'y';
                    else
                        y = 'n';

                    switch(y){
                        case 'y':
                        case 'Y':
                            write << setw(10) << left << bid << "\t" << setw(40) << left << bn << "\t" << setw(20) << left << uid << endl;
                            cout << "\nTransfer Successfull\n" << endl;
                            cout << "User with ID: " << uid << " lent The Book: " << bn << endl; 
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
            write.close();
            open.close();
            remove(files[6]);
            rename("temp.txt", files[6]);
        }else
            cout << "\nBook Already lent!\n" << endl;
    }else
        cout << "\nNo Request for This Book!\n";
    cont(1);
}

// Librarian accept book return
void acceptreturn(){
    string bn, uid;
    char value[50], bid[50];
    cin.ignore();
    cout << "Enter book ID to return: ";
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
            // write.close();
            open.close();
            remove(files[7]);
            rename("temp.txt", files[7]);
    }else
        cout << "\nBook not Lent!\n";
    cont();
}


// Librarian accept book return
void addblacklist(){
    string un, up;
    char value[50], uid[50], reason[100];
    cin.ignore();
    cout << "Enter User ID to add to blacklist: ";
    cin.getline(value, 50);
    if(check(value, 0)){
            ifstream open(files[7]);
            // ofstream write(files[7], ios::app);
            ofstream neww("temp.txt");
            while (open >> uid >> up >> un) {
                if (strcmp(uid, value) == 0) {
                    string confirm; char y;
                    cout <<"User ID: " << uid << endl << "User name: " << un << endl;
                    cout << "Confirm to add user to blacklist? y/n :";
                    cin >> confirm;

                    if (confirm[0] == 'Y' || confirm[0] == 'y')
                        y = 'y';
                    else
                        y = 'n';

                    switch(y){
                        case 'y':
                        case 'Y':
                            cout << endl << "Enter Reason: " ;
                            cin.ignore();
                            cin.getline(reason, 100);

                            // write << setw(10) << left << bid << "\t" << setw(40) << left << bn << "\t" << setw(20) << left << uid << endl;
                            cout << "\nUser " << uid << "added to blacklist for " << reason << endl;
                            break;
                        default:
                            neww << setw(10) << left << uid << "\t" << setw(20) << left << up << "\t" << setw(20) << left << un << endl;
                            cout << "\nProcess canceled!\n";
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
        cout << "\nNo user with this ID found\n";
    cont();
}

void showUsers(int t=0){
    char u[50], p[50], n[50];
     t == 1 ? cout << "\n-----------------\n Blacklist\n-----------------\n" : cout << "\n-----------------\n Users List\n-----------------\n";
    user x;

    cin.getline(x.id, 10);

    ifstream file(files[t]);
    bool flag = false;
    while(file>>u>>p>>n){
        cout << setw(10) << left << u << setw(10) << right << n << endl;
    }
    file.close();
    cont(1);
}

// Lend Transaction Menu
void lendManagement(){
    int choice;
    do{
        cout << "\n-----------------\nBOOK LEND MANAGEMENT"; info();
        cout << "\n-----------------\n" << endl;
        cout << "1.Show Requests\n2.Accept Requests\n3.Show Borrowed Books\n4.Accept Book Return\n5.\n6.Back\n0.Exit\n\nEnter your choice: ";
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
            cout << "Wrong Input! \n See you later!";
            break;
        }
    }while (choice != 6);
}

// Users List Menu
void userManagement(){
    int choice;
    do{
        cout << "\n-----------------\nBOOK MANAGEMENT"; info();
        cout << "\n-----------------\n" << endl;
        cout << "1.Show Users\n2.Show Blacklist\n3.Add to Blacklist\n4.Remove from Blacklist\n5.\n6.Back\n0.Exit\n\nEnter your choice: ";
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
            cout << "Wrong Input! \n See you later!";
            break;
        }
    }while (choice != 6);
}

// Librarian Menu
void librarianMenu(int t=1){
    if (login(t)){
        int choice;
        do{
            cout << "\n-----------------\nWELCOME TO AASTU LMS"; info();
            cout << "\n-----------------\n" << endl;
            cout << "1.Manage Books\n2.Transactions\n3.Manage Users\n4.Inventory\n5.Activity Log\n6.Back\n0.Exit\n\nEnter your choice: ";
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
                // inventory();
                break;
            case 5:
                showfile(3);
                break;
            case 6:
                break;
            case 0:
                exit(0);
            default:
                cout << "Wrong Input! \n See you later!";
                break;
            }
        }while (choice != 6);
    }
}

// Admin Menu
void adminMenu(){
    int choice;
    currentuser.type = "Admin";
    for(int i=0; i<5; i++){
        currentuser.name[i] = currentuser.type[i];
        currentuser.id[i] = ' ';}

    do{
        cout << "\n-----------------\nWELCOME BOSS"; info();
        cout << "\n-----------------\n" << endl;
        cout << "1.Add Librarian\n2.Display Activity Log\n3.Librarian mode\n4.Student Mode\n5.Back\n0.Destroy System\n\nEnter your choice: ";
        cin >> choice;
        switch (choice){
        case 1:
            signin(1);
            break;
        case 2:
            showfile(3);
            break;
        case 3:
            librarianMenu(5);
            break;
        case 4:
            studentPage();
            break;
        case 5:
            break;
        case 0:
            cout << "BOOOOOOOOOOOOOMMMMMMM" << endl;
            exit(0);
        default:
            cout << "Wrong Input! \n See you later!";
            break;
        }
    }while (choice != 5);
}

// Main Menu
void firstMenu(){
    int choice;
    do {
        cout << "\n-----------------\nWELCOME TO AASTU LMS\n-----------------\n" << endl;
        cout << "  I'm a:\n1.Student\n2.Librarian\n3.Exit\n0.Admin\n\nEnter your choice: ";
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
            int x; cout << "Passcode: "; cin>>x;
            x==9 ? adminMenu() : exit(0);
            break;
        default:
            cout << "Wrong Input! Try Again";
            break;
        }
    }while(choice != 3);
    cout << "Bye, See you later!";
}

int main()
{
    firstMenu();
    return 0;
}
