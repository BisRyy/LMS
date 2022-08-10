#include <iostream>
#include <cstring>
#include <string.h>
#include <fstream>

using namespace std;

// system("color=04")
char files[10][50] {"Users.txt", "Librarians.txt", "Books.txt", "activitylog.txt", "report.txt", "temp.txt", "borrows.txt"};

struct book{
    char title[50];
    char author[50];
    char id[50];
};

struct user{
    char name[50];
    string sname;
    char id[10];
    string sid;
    char pass[50];
    string spass;

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
void librarianMenu();
void adminMenu();
void firstMenu();
bool verifyPassword();
string securePassword(char*, char);
string replace(char*, char, char);


void clear(){
    system("clearenv");
}

// Hash passwords for security purpose
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
bool verifyPassword(){
    string u, p, n;
    bool verified = false;

    cout << "Hi " << currentuser.sname << ",\nEnter your Password: ";
    cin.ignore();
    cin.getline(currentuser.pass, 50);

    ifstream file(files[0]);
    while(file>>u>>p>>n){
        if( p==securePassword(currentuser.pass) && u==currentuser.sid){
        verified = true;
        currentuser.sid = u;
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

// Check if exists
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
        file  << "\n" << newuser.id << "\t" << securePassword(newuser.pass) << "\t" << replace(newuser.name);
        file.close();
        cout << "Registration Successfull\n";
    }
    else
        cout << "I think you already have an Account.\nTry Logging in." << endl;
    
}

// User Access Account
bool login(int t=0){

    string u, p; char n[50];
    bool success = false;
    cout << "\n-----------------\nEnter Login Info\n-----------------\n";
    if(t==0)
        cout << "Enter your Student ID No: ";
    else
        cout << "Enter your Librarian ID No: ";
    cin >> currentuser.sid;

    ifstream file(files[t]);

    while(file>>u>>p>>n){
        if(u==currentuser.sid){
            currentuser.sname = replace(n, '_', ' ');
            success = true;
            break;
        }
    }
    file.close();

    if(success)
        if (verifyPassword())
            cout << "Log in Successfull.";
        else{
            success = false;
            cout << "Incorrect Password. Try again later.";
        }
    else{
        success = false;
        cout << "Sorry, Can't find your ID in the list. Try registering.";
        }
    return success;
}

void forgotpassword(int t=0){
    char u[50], p[50], n[50];
    // string u, p, n;
    cout << "\n-----------------\nRecover Password\n-----------------\n";
    user x;
    if (t==0)
        cout << "Enter your Student ID No: ";
    if (t==1)
        cout << "Enter your Librarian ID No: ";
    cin.ignore();
    cin.getline(x.id, 10);

    ifstream file(files[t]);
    bool flag = false;
    while(file>>u>>p>>n){
        if(strcmp(u, x.id) == 0){
            flag == true;
            cout << "Enter your Full Name: ";
            cin.ignore();
            cin.getline(x.name, 50);
            if(n== replace(x.name)){
                cout << "Your password is: " << securePassword(p, 'd') << endl << "Try Not to forget Again!";
            }else
                cout << "\nWho are you?\nGet out of here!" << endl;
        }
    }
    file.close();
    if(!flag)
       cout << "\nSorry, Can't find your ID in the list. Try registering.";
}

// Librarian Add Books
void addbook(){
    book newbook; int n;

    cout << "How many Books do you ant to add? :";
    cin >> n;

    fstream file;
    for(int i=0; i<n;i++){    
        file.open(files[2], ios::out | ios::app);
        cout << "Enter Book ID: ";
        cin >> newbook.id;
        if(!check(newbook.id, 2)){
            cin.ignore();
            cout << "Enter Book name: ";
            cin.getline(newbook.title, 50);

            cout << "Enter Author name: ";
            cin.getline(newbook.author, 50);
            file << newbook.id << "\t" << replace(newbook.title, ' ', '_') << "\t" << replace(newbook.author, ' ', '_') << "\n";
            file.close();
        }else
            cout << "\nThere is a Book with this ID.\nCheck the Id or Delete the existing Book.\n";
    }
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
            cout << "\n--------------\n";
            cout << endl << value[1] << endl << replace(value[2], '_', ' ') << endl << replace(value[3], '_', ' ') << endl;
        }
    }
    if (!found)
            cout << "Nothing found!";
}

// Display All Books
void showbooks(){
    string getcontent;
    ifstream openfile (files[2]);
        if(openfile.is_open()){
            while(!openfile.eof()){
            getline (openfile, getcontent);
            cout << getcontent << endl; // these two lines alone in a while loop display ALL records
        }
        openfile.close();
     }
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
            write << bid << "\t" << replace(bn) << "\t" << replace(an) << endl;
        }
        write.close();
        open.close();
        remove(files[2]);
        rename("temp.txt", files[2]);
        cout << "\nUpdate successfull!\n";
    }else
        cout << "Sorry, can't find a book with this ID.\n";
}

// Remove Book Info
void deletebook(){
    char bid[50], bn[50], an[50], search[50];
    cout << "Enter the book ID to delete: ";
    cin >> search;

    if(check(search, 2)){
        ifstream open("Books.txt");
        ofstream write("temp.txt");
        while (open >> bid >> bn >> an) {
            //Update data
            if (strcmp(bid, search) == 0) {
                char confirm;
                cout << bid << "\t" << bn << "\tby " << an << endl;
                cout << "Are you sure? Do you want to delete the following book data?";
                cin >> confirm;
                switch(confirm){
                    case 'y':
                    case 'Y':
                        cout << "\nDeleted successfully!\n";
                        break;
                    default:
                        cout << "\nDeletion canceled!\n";
                        write << bid << "\t" << bn << "\t" << an << endl;
                        break;
                }
                continue;
            }
            else{
                write << bid << "\t" << bn << "\t" << an << endl;
            }
        }
        write.close();
        open.close();
        remove("Books.txt");
        rename("temp.txt", "Books.txt");
    }else
        cout << "Sorry, can't find a book with this ID.\n";
}

void borrowBook(){
    string bn, an;
    char value[50], bid[50];
    cin.ignore();
    cout << "Enter book ID to to borrow: ";
    cin.getline(value, 50);
    if(check(value, 2)){
        if(!check(value, 6)){      
            ifstream open(files[2]);      
            ofstream write(files[6], ios::app);
            while (open >> bid >> bn >> an) {
                if (strcmp(bid, value) == 0) {
                    char confirm;
                    cout << bid << "\t" << bn << "\tby " << an << endl;
                    cout << "Are you sure? Do you want to borrow the thes book?";
                    cin >> confirm;
                    switch(confirm){
                        case 'y':
                        case 'Y':
                            write << bid << "\t" << bn << "\t" << currentuser.id << endl;
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
        cout << "I couldn't find the book. Check your ID!";
}

void studentPage(){
    int choice;
    do{
        cout << "\n-----------------\nWELCOME TO AASTU LMS\n-----------------\n" << endl;
        cout << "1.Show Books\n2.Search Book\n3.Request to Borrow Book\n4.Account Setting\n6.Back\n0.Exit\n\nEnter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            showbooks();
            break;
        case 2:
            searchbook();
            break;
        case 3:
            borrowBook();
            break;
        case 4:
            // returnbook();
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

void studentMenu(){
    // clear();
    int choice;
    do{
        cout << "\n-----------------\nUser Login\n-----------------\n" << endl;
        cout << "1.Login\n2.Register\n3.Forgot Password\n4.Back\n0.Exit\n\nEnter your choice: ";
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
        cout << "\n-----------------\nBOOK MANAGEMENT\n-----------------\n" << endl;
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
            showbooks();
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

void librarianMenu(){
    cout << "\n-----------------\nLibrarian Login\n-----------------\n" << endl;
    if (login(1)){
        int choice;
        do{
            cout << "\n-----------------\nWELCOME TO AASTU LMS\n-----------------\n" << endl;
            cout << "1.Manage Books\n2.Transactions\n3.Manage Users\n4.Inventory\n5.Activity Log\n6.Back\n0.Exit\n\nEnter your choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                bookManagement();
                break;
            case 2:
                // transaction();
                break;
            case 3:

                break;
            case 4:

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
}

void adminMenu(){
    int choice;
    do{
        cout << "\n-----------------\nWELCOME BOSS\n-----------------\n" << endl;
        cout << "1.Add Librarian\n2.Activity Log\n3.Librarian mode\n4.BAck\n0.Destroy System\n\nEnter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            signin(1);
            break;
        case 2:
            // updatebook();
            break;
        case 3:
            // deletebook();
            break;
        case 0:
            exit(0);
        default:
            cout << "Wrong Input! \n See you later!";
            break;
        }
    }while (choice != 4);
}

void firstMenu(){
    int choice;
    do {
    cout << "\n-----------------\nWELCOME TO AASTU LMS\n-----------------\n" << endl;
    cout << "  I'm a:\n1.Student\n2.Librarian\n\nEnter your choice: ";
    cin >> choice;
        switch (choice)
        {
        case 1:
            studentMenu();
            break;
        case 2:
            librarianMenu();
            break;
        case 0:
            adminMenu();
            break;
        default:
            cout << "Wrong Input! Try Again";
            break;
        }
    }while(choice != 0);
    cout << "Bye, See you later!";
}


int main()
{
    // ifstream file;
    // file.open("record.txt");
    // insert_data(file);

    // Student x;
    // x = get_data(file);
    // print_data(x);

    // delete_data(123, file);
    // cout << "done";


    firstMenu();
    // studentMenu();
    // librarianMenu();
    // addbook();
    // signin();
    // login();
    // forgotpassword();
    // addbook();
    // searchbook();
    // updatebook();
    // deletebook();
    // showbooks();
    // borrowBook();

    // char x[50] = "addis ababa";
    // cout << replace(x, 'd', 'b');

    // char w[50]="at@123%^&", y[50] = "`s?012$]%";
    // cout << securePassword(w, 'e') << endl;
    // cout << securePassword(y, 'd') << endl;

    return 0;
}

    
// Testing

struct Student{
	int id;
	string name;
	int marks;
	int age;
};
void insert_data(ofstream &write) {
	Student s1;
	cout << "\n\tEnter id : ";
	cin >> s1.id;
	cout << "\n\tEnter name : ";
	cin.get();
	getline(cin, s1.name);
	cout << "\n\tenter marks : ";
	cin >> s1.marks;
	cout << "\n\tEnter age : ";
	cin >> s1.age;

	write << s1.id << endl;
	write << s1.name;
	write << endl << s1.marks << endl << s1.age << endl;
}
Student get_data(ifstream &read) {
	Student obj;
	read >> obj.id;
	read.get();
	getline(read, obj.name);
	read >> obj.marks;
	read >> obj.age;
	return obj;
}
void print_data(Student s) {
	cout << "\n\tRecord is : ";
	cout << "\n\tId : " << s.id;
	cout << "\n\tName : " << s.name;
	cout << "\n\tMarks : " << s.marks;
	cout << "\n\tAge : " << s.age;
}
void delete_data(int id, ifstream &read) {
	//We get all data from our file i.e record.txt
	//We have to write data into temp file
	//We have to use rename and remove
	ofstream nf;
	nf.open("temp.txt");
	Student s1;
	s1 = get_data(read);
	while (!read.eof()) {
		//There we delete record
		if (s1.id != id) {
			nf << s1.id << endl << s1.name;
			nf << endl << s1.marks << endl;
			nf << s1.age << endl;
		}
		s1 = get_data(read);
	}

	nf.close();
	read.close();

	remove("record.txt");
	rename("temp.txt", "record.txt");
}

void update_record(int id, ifstream &read) {
	ofstream write("temp1.txt");
	Student s;
	s = get_data(read);
	while (!read.eof()) {
		//Update data
		if (s.id == id) {
			cout << "\n\tEnter new record for id : " << id;
			insert_data(write);
		}
		else {
			write << s.id << endl << s.name << endl;
			write << s.marks << endl << s.age;
			write << endl;
		}
		s = get_data(read);
	}

	write.close();
	read.close();
	remove("record.txt");
	rename("temp1.txt", "record.txt");
}

// --------

    // Test

    // int sum = 0;
    // for (int i =0; i<16; i++)
    //     for (int j=0; j<=i;j++)
    //         sum +=j;
    // cout << sum;

    // int n=1000, c=5;
    // do{
    //     n/=c;
    //     if(c==2)break;
    // }while (c--);
    // cout << n;

    // int arr[]={10,20,30,40,50,60};
    // int *p1, *p2, *p3;
    // p1 = arr;
    // p2 = &arr[3];
    // p3 = arr +3;

    // cout << *p1 << endl;
    // cout << *p2 << endl;
    // cout << *p3 << endl;

    // cout << p1 << endl;
    // cout << p2 << endl;
    // cout << p3 << endl;

    // cout << *p1++ << endl;
    // cout << (*p2)++ << endl;
    // cout << *(p3++) << endl;
