//LIST OF THE FILES INCLUDED
/*
1. admin.dat --> adminstrator database
2. customer.dat --> customer database
3. transactlog.dat -->transaction database (customer)
4. loandata.dat -->loan database (customer)
5. deleter.dat -->customer delete record (customer + employee)
6. lastl.dat -->lastl time of the customer
*/


//Header files
#include <fstream.h>
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>		//Header file to get system time
#include "ASCIIART.h"	//custom header file for asciiart generation
#include "LOADIN.h"		//custom header file for display loading bar
#include "RAND.h"		//custom header file for display random generator

class bank
{
//Employee Member Functions And Data
private:
	struct employee     //Employee Data-Structure
	{
		char empid[6];  //Employee ID
		char name[100]; //Employee Name
		int pin;    //Employee Login PIN
		int age;    //Employee Age
		int level;  //Employee Access Level
	}empinfo;
public:
	void registeremployee();    //Register Employee
	void dispemployee();    //Display the employee (currently all the employee)
	void empmodify();   //Modify employee details
	void empdelete();   //Delete an employee
	int emplogin();     //Employee login function
	void dispcustomer();    //Display all the customers
	void toempdeletecustomer(); //executes the delete customer function
	int deletecustomer(char id[6]);     //to delete customer (function for both customer and employee)
	void toaddinteresttodeposit();

//Customer Member Functions And Data
private:
	struct customer     //Customer Data-Structure
	{
		char custid[6];     //Customer ID
		char name[100];     //Customer Name
		int pin;    //Customer Login PIN
		int age;    //Customer Age
		int loantaken;  // To Check If Customer Has Taken A Loan Or Not
		unsigned int salary;    //Customer Salary
		unsigned int balance;   //Customer Deposit Money
	}custinfo;
public:
	void registercustomer();    //register a customer
	int logincustomer();    //customer login
	void dispcustprofile(); //display customer profile
	void customermoneydeposit();    //deposit money from account
	void customermoneywithdraw();   //withdraw money from account
	void customerdetailsmodify();   //customer details modify
	void transactrecord(char custid[6], int &money, char typ); //save the transaction made into the file
	void disptransactrecord();  //log of all transactions made by the customer
	void loancheck();   //To take loan amount from account if customer has to pay as of now
	void loanmenu();    //to switch between seeing the details of the loan made or make a loan
	void loancreator(); //creates a loan for the customer
	void disploantaken();   // to display the loan taken by the customer
	void transfertoanotheraccount();    //To transfer money from one account to another
	int transfersendertransactlog(int &money);      //creates a log of the customer transfer record of the sender
	void recievefromanotheraccount(int &money);     //creates a log of the customer transfer record of the reciever
	int customerdeleteaccount();        //customer delete their account
}bankdetails;

struct logtransaction   //Transaction Log Database-Structure
{
	char id[6];
	struct time     //Transaction Time Data-Structure
	{
		int date;
		int month;
		int year;
		int hour;
		int minute;
		int second;
	}ti;
	int amount;
	char type[20];  //type of transaction made
	char remark[100];
}transact;

struct loan     //Loan Data-Structure
{
	long int amountloan;
	long int principal;
	int interest;
	int loantimescount;
	int minutes;
	int totalhours; //Total hours to pay the loan
	int date;
	int month;
	int year;
	int hours;
	int min;
	int sec;
	char loancustid[6];
	char loanid[10];    //Unique ID of the loan
}loanlist;

struct deletecustomer
{
	char informto[10];
	char reason[100];
	char id[6];
	char name[100];
	int date;
	int month;
	int year;
	int hour;
	int min;
	int sec;
}custdelete;

struct customerlastl
{
	char id[6];
	int hour;
	int min;
	int sec;
}lastenter;


float depositinterest=2.0;  //Bank deposit/withdrawal interest
float loaninterest=2.0;     //Bank loan interest
//Temporary variables to compare certain details after login
int tempafterloginloan;
int getlevelafterlogin;
char empidafterlogin[6];
char getcustidafterlogin[6];
char recievecustid[6];
char sendname[100];
char recievename[100];
char deletename[100];


void deletefromdeleterecord(char deleteid[6])
{
	ifstream fin;
	fin.open("deleter.dat", ios::in|ios::binary);
	if(!fin)
	{
		cout<<"\nError";
		exit(0);
	}
	ofstream fout;
	fout.open("temp.dat",ios::out|ios::binary);
	if(!fout)
	{
		cout<<"\nError";
		exit(0);
	}
	int found=0;
	fin.read((char*)&custdelete, sizeof(custdelete));
	while(!fin.eof())
	{
		if(strcmp(custdelete.id, deleteid)==0)
			found=1;
		else
			fout.write((char*)&custdelete, sizeof(custdelete));
		fin.read((char*)&custdelete, sizeof(custdelete));
	}
	if(found==0)
	{
		fout.close();
		fin.close();
		remove("temp.dat");
	}
	else
	{
		fout.close();
		fin.close();
		remove("deleter.dat");
		rename("temp.dat", "deleter.dat");
	}
}


//EMPLOYEE FUNCTIONS
void bank::registeremployee()   //Employee Registration
{
	ofstream fout;
	fout.open("admin.dat", ios::binary| ios::app);  //opening file admin.dat in write mode
	if(!fout)
	{
		cout<<"\nError";
		exit(0);
	}
	ifstream fin;
	fin.open("admin.dat", ios::binary| ios::beg| ios::in);  //opening file admin.dat in read mode
	if(!fin)
	{
		cout<<"\nError";
		exit(0);
	}
	clrscr();
	bank getidtemp;         //temporary bank variable
	//Getting employer name
	cout<<"\n----------------------------Employee Registration----------------------------";
	gotoxy(64,2);
	time_t now=time(0);
	tm *ltm=localtime(&now);
	cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
	if(ltm->tm_hour>=13)
		cout<<" PM";
	else
		cout<<" AM";
	gotoxy(12,1);
	cout<<"\n\nEnter the employee name: ";      //Getting employee name
	gets(empinfo.name);
	//Getting employer age
	cout<<"\nEnter the employee age: ";         //Getting Employee age
	cin>>empinfo.age;
	if(empinfo.age<18||empinfo.age>60)          //Checking employee age less than 18 or greater than 60
	{
		//if true such an employee cannot be created
		cout<<"\nSorry. The employee age is not within expected limits";
		return;
	}
	cout<<"\n1 - Top level - (full authorisation)"
		<<"\n2 - Lower level - (limited authorisation)";
	cout<<"\nEnter employee authorisation level: ";
	cin>>empinfo.level;                         //Getting employee's maximum access level granted
	while(empinfo.level>3||empinfo.level<0)   //to re-enter the employee access level if invalid
	{
		cout<<"\nInvalid Authorisation level appointed. Please re-enter: ";
		cin>>empinfo.level;
	}
	int gotoreenterpin;
	gotoreenterpin:
	cout<<"\nEnter employee PIN: ";             //Getting employee pin
	cin>>empinfo.pin;
	cout<<"\nConfirm PIN: ";                    //re-enter pin
	int repin;
	int wrongpin=0;
	cin>>repin;
	if(repin==empinfo.pin)                      //comparing the re-pin with original pin
		wrongpin=1;
	if(wrongpin==0)
	{
		cout<<"\nThe confirm PIN was wrong. Please re-enter: ";
		cout<<"\n\n\n";
		goto gotoreenterpin;
	}
	if(wrongpin==1)
	{
		cout<<"\nPIN CREATED!!!\n";
	}
	//Employer ID generator
	gen_random(empinfo.empid, 5);               //To create an unique employee ID
	fin.read((char*)&getidtemp.empinfo, sizeof(getidtemp.empinfo));
	while(!fin.eof())                           //To create an unique employee ID until it does not match other employee IDs
	{
		if(strcmp(empinfo.empid,getidtemp.empinfo.empid)==0)
			gen_random(empinfo.empid, 5);
		gen_random(empinfo.empid, 5);
		fin.read((char*)&getidtemp.empinfo, sizeof(getidtemp.empinfo));
	}
	gen_random(empinfo.empid, 5);
	fin.close();        //closing admin.dat file in read mode
	cout<<"\nYour employee number is: ";
	puts(empinfo.empid);
	fout.write((char*)&empinfo, sizeof(empinfo));
	fout.close();       //closing admin.dat file in write mode
	cout<<"\nREGISTRATION COMPLETE";
}

int bank::emplogin()        //Employee Login
{
	clrscr();
	ifstream fin;
	fin.open("admin.dat",ios::binary|ios::in);      //opening admin.dat file
	if(!fin)
	{
		cout<<"\nError";
		exit(0);
	}
	int login=0;
	gotoxy(64,2);
	time_t now=time(0);
	tm *ltm=localtime(&now);
	cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
	if(ltm->tm_hour>=13)
		cout<<" PM";
	else
		cout<<" AM";
	gotoxy(12,1);
	cout<<"\nEnter the employer ID: ";
	char tempid[6];
	gets(tempid);
	int trl=3;    //trails to enter pin
	fin.read((char*)&empinfo,sizeof(empinfo));
	while(!fin.eof())
	{
		if(strcmp(tempid,empinfo.empid)==0)     //comparing the given ID with admin.dat file database
		{
			while(trail!=0)     //maximum chance to enter the PIN
			{
				cout<<"\nEnter PIN: ";
				int temppin;
				cin>>temppin;
				if(temppin==empinfo.pin)        //comparing entered pin with saved pin database
				{
					strcpy(empidafterlogin,empinfo.empid);      //copying temporarily the employee id
					getlevelafterlogin=empinfo.level;           //copying temporarily the employee access level
					login=1;
					goto finishlogin;
					break;
				}
				else if(login==1)
					break;
				else
				{
					trail--;
					cout<<"\nIncorrect PIN. Attempts left: "<<trail<<" of 3";
				}
			}
			if(trail==0)
				break;
		}
		fin.read((char*)&empinfo,sizeof(empinfo));
	}
	int finishlogin;
	finishlogin:
	if(login==0)
	{
		cout<<"\nLOGIN FAILED!!!";
	}
	if(login==1)
	{
		return login;
		loadingbar();
	}
	fin.close();    //closing file
}

void bank::dispemployee()   //Displaying Employee
{
	clrscr();
	gotoxy(64,2);
	time_t now=time(0);
	tm *ltm=localtime(&now);
	cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
	if(ltm->tm_hour>=13)
		cout<<" PM";
	else
		cout<<" AM";
	gotoxy(12,1);
	ifstream fin;
	fin.open("admin.dat",ios::in|ios::binary);  //opening admin.dat file
	if(!fin)
	{
		cout<<"\nError";
		exit(0);
	}
	fin.read((char*)&empinfo, sizeof(empinfo));
	while(!fin.eof())
	{
		cout<<"\nName: ";       //displaying employee name
		puts(empinfo.name);
		cout<<"Age: ";          //displaying employee age
		cout<<empinfo.age;
		cout<<"\nEmployee ID: ";    //displaying employee ID
		puts(empinfo.empid);
		cout<<"\nPIN: ";        //displaying employee PIN  (temporarily due to testing)
		cout<<empinfo.pin;
		cout<<"\nAuthorisation Level: ";    //displaying employee acess level
		cout<<empinfo.level;
		getch();
		fin.read((char*)&empinfo, sizeof(empinfo));
	}
	fin.clear();
	fin.close();    //closing admin.dat file
}


void bank::empmodify()      //Modify Emplpoyee Details
{
	clrscr();
	int modifyask;
	int found=0, loc;
	char anotmodask='y';
	fstream f;
	f.open("admin.dat",ios::binary|ios::out|ios::in);
	if(!f)
	{
		cout<<"\nError";
		exit(0);
	}
	cout<<"\nEnter the employee's ID whose details you wish to change: ";
	int modlevel;
	char searchid[6];
	gets(searchid);
	f.read((char*)&empinfo, sizeof(empinfo));
	while(!f.eof())
	{
		if(strcmp(searchid,empinfo.empid)==0)       //comparing ID with temporary ID to find location in file admin.dat
		{
			found = 1;
			loc = f.tellg() - sizeof(empinfo);      //saving location of the user in the file admin.dat
			modlevel=empinfo.level;
			break;
		}
		f.read((char*)&empinfo, sizeof(empinfo));
	}
	if(found == 0)  //if the such employee ID
		cout<<"Modify Fuction Scrambled (ERROR)";
	else if(found == 1)
	{
		while(anotmodask=='y'||anotmodask=='Y')
		{
			clrscr();
			gotoxy(64,2);
			time_t now=time(0);
			tm *ltm=localtime(&now);
			cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
			if(ltm->tm_hour>=13)
				cout<<" PM";
			else
				cout<<" AM";
			gotoxy(12,1);
			f.seekp(loc);
			cout<<"\n1. Name"
				<<"\n2. Age"
				<<"\n3. PIN"
				<<"\n4. Authorisation Level"
				<<"\n5. Cancel";
			cout<<"\nEnter the employee detail to change: ";        //asking which employees to delete
			cin>>modifyask;
			switch(modifyask)
			{
				case 1: if(strcmp(searchid, empidafterlogin)==0)
						{
							cout<<"\nEnter the employee new name: ";
							gets(empinfo.name);
						}
						else
							cout<<"\nYou cannot change the name of another employee";
						break;
				case 2: if(strcmp(searchid, empidafterlogin)==0)
						{
							cout<<"\nEnter the employee new age: ";
							cin>>empinfo.age;
							while(empinfo.age<18||empinfo.age>60)
							{
								cout<<"\nSorry. The employee age is not within expected limits";
								cout<<"\nPlease re-enter age: ";
								cin>>empinfo.age;
							}
						}
						else
							cout<<"\nYou cannot change the age of another employee";
						break;
				case 3: if(strcmp(searchid, empidafterlogin)==0)
						{
							int trail=3;
							while(trail!=0)
							{
								int checkpin, repin, wrongpin=1;
								cout<<"\nEnter the employee current PIN: ";
								cin>>checkpin;
								if(checkpin==empinfo.pin)
								{
									do
									{
										cout<<"\nEnter your new PIN: ";
										cin>>empinfo.pin;
										cout<<"\nEnter confirm PIN: ";
										cin>>repin;
										if(repin==empinfo.pin)
										{
											wrongpin=0;
											trail=0;
										}
										else
											cout<<"\nPIN did not match. Please re-enter";
									}while(wrongpin==1);
								}
								else
									cout<<"\nThe pin incorrect. \nAttempts Left: "<<trail--<<" of 3";
							}
						}
						else
							cout<<"\nYou cannot change the name of another employee";
						break;
				case 4: if(modlevelget==1)
						{
							cout<<"\nEnter the new authorisation level: ";
							int templevel;
							cin>>templevel;
							while(!(templevel==1||templevel==2))
							{
								cout<<"\nInvalid Authorisation level appointed. Please re-enter: ";
								cin>>templevel;
							}
							empinfo.level=templevel;
							getlevelafterlogin=templevel;
						}
						else
							cout<<"\nSorry you cannot change your access level";
						break;
				case 5: break;
				default: cout<<"\nINVALID CHOICE: ";
			}
			cout<<"\nDo you wish to change another detail (Y/N): ";     //asking if to change another employee detail
			cin>>anotmodask;
			while(!((anotmodask=='Y')||(anotmodask=='y')||(anotmodask=='N')||(anotmodask=='n')))
			{
				cout<<"\nINVALID OPTION!!!, please re-enter: ";
				cin>>anotmodask;
			}
			f.write((char*)&empinfo, sizeof(empinfo));
		}

		if(anotmodask!=5||anotmodask=='n'||anotmodask=='N')
			cout<<"\nRECORDS MODIFIED";
		cout<<"\nClosing Modify Operation.";
		delay(1000);
	}
	f.close();      //closing file admin.dat
}

void bank::empdelete()
{
	ifstream fin;
	fin.open("admin.dat", ios::binary|ios::in);     //opening file admin.dat in read mode
	if(!fin)
	{
		cout<<"\nError";
		getch();
	}
	ofstream fout;
	fout.open("temp.dat", ios::binary|ios::out);    //opening file admin.dat in write mode
	if(!fout)
	{
		cout<<"\nError";
		getch();
	}
	clrscr();
	gotoxy(64,2);
	time_t now=time(0);
	tm *ltm=localtime(&now);
	cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
	if(ltm->tm_hour>=13)
		cout<<" PM";
	else
		cout<<" AM";
	gotoxy(12,1);
	cout<<"\nEnter the employee to delete: ";
	char tempempid[6];
	gets(tempempid);
	int found=0;
	if(strcmp(empidafterlogin,tempempid)==0)        //to check if the entered employee to deleted is the one logged in
	{
		cout<<"\nYou cannot delete your own record.\nPlease login as another administrator to do this action";
		return;
	}
	fin.read((char*)&empinfo, sizeof(empinfo));
	while(!fin.eof())           //loop delete
	{
		if(strcmp(tempempid,empinfo.empid)==0)
			found=1;
		else
			fout.write((char*)&empinfo, sizeof(empinfo));
		fin.read((char*)&empinfo, sizeof(empinfo));
	}
	if(found==1)
	{
		fout.close();
		fin.close();
		remove("admin.dat");
		rename("temp.dat","admin.dat");
		cout<<"\nEmployee Removed";
	}
	if(found==0)
	{
		fin.close();
		fout.close();
		cout<<"\nSuch an employee ID does not exist";
		remove("temp.dat");
	}
}

int bank::deletecustomer(char id[6])
{
	ifstream fin;
	fin.open("customer.dat", ios::in|ios::binary);
	if(!fin)
	{
		cout<<"\nNo Customers Registered";
		return 5;
	}
	ofstream fout;
	fout.open("temp.dat", ios::out|ios::binary);
	if(!fout)
	{
		cout<<"\nError";
		exit(0);
	}
	int found=0;
	fin.read((char*)&custinfo, sizeof(custinfo));
	while(!fin.eof())
	{
		if(strcmp(id, custinfo.custid)==0)
		{
			found=1;
			if(custinfo.loantaken==1)
				return 2;
			strcpy(deletename, custinfo.name);
		}
		else
			fout.write((char*)&custinfo, sizeof(custinfo));
		fin.read((char*)&custinfo, sizeof(custinfo));
	}
	if(found==0)
	{
		fout.close();
		fin.close();
		remove("temp.dat");
		return 0;
	}
	else
	{
		fout.close();
		fin.close();
		remove("customer.dat");
		rename("temp.dat","customer.dat");
		return 1;
	}
}

void viewtransactemp()
{
	ifstream fin;
	fin.open("transactlog.dat",ios::in|ios::binary);
	if(!fin)
	{
		cout<<"\nNo transactions made";
		return;
	}
	int i=1;
	clrscr();
	cout<<"\n---------------------------Transactions Made--------------------------";
	gotoxy(64,3);
	time_t now=time(0);
	tm *ltm=localtime(&now);
	cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
	if(ltm->tm_hour>=13)
		cout<<" PM";
	else
		cout<<" AM";
	gotoxy(2,5);
	fin.read((char*)&transact, sizeof(transact));
	while(!fin.eof())
	{
		cout<<"-------------------------------------------------";
		cout<<"\n"<<i<<". Type: "<<transact.type;       //displaying the type transaction made (Deposit/Withdrawal)
		cout<<"\nAmount: "<<transact.amount;        //displaying the amount of money transacted
		cout<<"\tTime: ";           //displaying when the transaction was made
		cout<<transact.ti.date<<"."<<transact.ti.month<<"."<<transact.ti.year;
		cout<<"\t"<<transact.ti.hour<<":"<<transact.ti.minute<<":"<<transact.ti.second;
		cout<<"\n\nRemark: "<<transact.remark;      //diaplying the remark of the transaction
		cout<<"\n-----------------------------------------------";
		i++;        //to increase the index number value by 1
		getch();
		fin.read((char*)&transact, sizeof(transact));
	}
	fin.close();
}

void viewloanemp()
{
	ifstream fin;
	fin.open("loandata.dat", ios::in|ios::binary);
	if(!fin)
	{
		cout<<"\nNo loans taken";
		return;
	}
	int i=1;
	clrscr();
	cout<<"-------------------------------Loans Taken----------------------";
	gotoxy(64,3);
	time_t now=time(0);
	tm *ltm=localtime(&now);
	cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
	if(ltm->tm_hour>=13)
		cout<<" PM";
	else
		cout<<" AM";
	gotoxy(2,5);
	fin.read((char*)&loanlist, sizeof(loanlist));
	while(!fin.eof())
	{
		cout<<"\n------------------------------------------------------------------";
		cout<<"\n"<<i;
		cout<<". Loan ID: "<<loanlist.loanid;
		cout<<"\t\tCustomer ID: "<<loanlist.loancustid;
		cout<<"\nTime: ";
		cout<<"\nDate: "<<loanlist.date<<"."<<loanlist.month<<"."<<loanlist.year;
		cout<<"\t\tTime taken: "<<loanlist.hours<<":"<<loanlist.min<<":"<<loanlist.sec;
		cout<<"\nAmount: "<<loanlist.principal;
		cout<<"\nHours to finish: "<<loanlist.totalhours;
		cout<<"\n-------------------------------------------------------------------";
		getch();
		fin.read((char*)&loanlist, sizeof(loanlist));
	}

}

void bank::toempdeletecustomer()
{
	clrscr();
	gotoxy(64,2);
	time_t now=time(0);
	tm *ltm=localtime(&now);
	cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
	if(ltm->tm_hour>=13)
		cout<<" PM";
	else
		cout<<" AM";
	gotoxy(12,1);
	cout<<"\nEnter the customer ID to remove: ";
	char deletecust[6];
	gets(deletecust);
	int deletereply = deletecustomer(deletecust);
	if(deletereply==0)
	{
		cout<<"\nSuch a customer does not exist";
		return;
	}
	else if(deletereply==1)
	{
		cout<<"\nEnter the reason to delete the customer: ";
		gets(custdelete.reason);
		ofstream fout;
		fout.open("deleter.dat",ios::out|ios::app|ios::binary);
		if(!fout)
		{
			cout<<"\nError";
			exit(0);
		}
		strcpy(custdelete.informto, "customer");
		strcpy(custdelete.id,deletecust);
		strcpy(custdelete.name, deletename);
		time_t now = time(0);       //getting system time
		tm *ltm = localtime(&now);  //getting system current local time to save as the transaction time
		custdelete.date=ltm->tm_mday;
		custdelete.month= 1 + ltm->tm_mon;
		custdelete.year=1900 + ltm->tm_year;
		custdelete.hour=ltm->tm_hour;
		custdelete.min=ltm->tm_min;
		custdelete.sec=ltm->tm_sec;
		fout.write((char*)&custdelete, sizeof(custdelete));
		fout.close();
		ifstream fin;
		fin.open("lastl.dat", ios::in|ios::binary);
		if(!fin)
		{
			cout<<"\nError";
			exit(0);
		}
		ofstream fo;
		fo.open("temp.dat",ios::out|ios::binary);
		if(!fout)
		{
			cout<<"\nError";
			exit(0);
		}
		int deletefound=0;
		fin.read((char*)&lastenter, sizeof(lastenter));
		while(!fin.eof())
		{
			if(strcmp(lastenter.id, deletecust)==0)
				deletefound=1;
			else
				fo.write((char*)&lastenter, sizeof(lastenter));
			fin.read((char*)&lastenter, sizeof(lastenter));
		}
		if(deletefound==1)
		{
			fo.close();
			fin.close();
			remove("lastl.dat");
			rename("temp.dat", "lastl.dat");
		}
		else
		{
			fo.close();
			fin.close();
			remove("temp.dat");
		}
		fin.open("transactlog.dat", ios::in|ios::binary);
		if(!fin)
			return;
		fo.open("tempt.dat", ios::out|ios::binary);
		if(!fo)
		{
			cout<<"\nError";
			exit(0);
		}
		int tfound=0;
		fin.read((char*)&transact, sizeof(transact));
		while(!fin.eof())
		{
			if(strcmp(transact.id, deletecust)==0)
				tfound=1;
			else
				fout.write((char*)&transact, sizeof(transact));
			fin.read((char*)&transact, sizeof(transact));
		}
		if(tfound==1)
		{
			fo.close();
			fin.close();
			remove("lastl.dat");
			rename("temp.dat", "lastl.dat");
		}
		else
		{
			fo.close();
			fin.close();
			remove("temp.dat");
		}
	}
	else if(deletereply==2)
	{
		cout<<"\nYou cannot the customer as they have a loan to complete";
		return;
	}
	else if(deletereply==5)
		return;
}

void adminmenu()        //Administrator Menu (not a member function of  class)
{
	int found=0;
	gotoxy(64,2);
	time_t now=time(0);
	tm *ltm=localtime(&now);
	cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
	if(ltm->tm_hour>=13)
		cout<<" PM";
	else
		cout<<" AM";
	gotoxy(12,1);
	ifstream fin;
	fin.open("deleter.dat", ios::in|ios::binary);
	if(fin)
	{
		clrscr();
		int count=0;
		fin.read((char*)&custdelete, sizeof(custdelete));
		while(!fin.eof())
		{
			if(strcmp(custdelete.informto, "employee")==0)
			{
				found=1;
				count++;
				cout<<"\n-------------------------------------";
				cout<<"\nCustomer left from the bank: ";
				cout<<"\nCustomer ID: ";
				puts(custdelete.id);
				cout<<"Customer Name: ";
				puts(custdelete.name);
				cout<<"\nTime of deletion: ";
				cout<<"\n\nDate: ";
				cout<<custdelete.date<<"/"<<custdelete.month<<"/"<<custdelete.year;
				cout<<"\tTime: ";
				cout<<custdelete.hour<<":"<<custdelete.min<<":"<<custdelete.sec;
				cout<<"\n\nReason: ";
				puts(custdelete.reason);
				cout<<"\n-------------------------------------";
				deletefromdeleterecord(custdelete.id);
				getch();
			}
			fin.read((char*)&custdelete, sizeof(custdelete));
		}
		fin.close();
	}
	int adminmenuask;
	do
	{
		clrscr();
		gotoxy(64,2);
		time_t now=time(0);
		tm *ltm=localtime(&now);
		cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
		if(ltm->tm_hour>=13)
			cout<<" PM";
		else
			cout<<" AM";
		gotoxy(12,1);
		cout<<"\n1. Display employees"      //to display the employee
			<<"\n2. Delete employees"       //to delete an employee (authorised only to employees with access level)
			<<"\n3. Modify employees"       //to modify detail of the logged in employee (authorised only to employees with access level 1)
			<<"\n4. Register customer"      //to register a customer
			<<"\n5. Display all customers"  //display all the customers
			<<"\n6. View all customer transaction"
			<<"\n7. View all customer loans"
			<<"\n8. Delete a customer"      //to delete a customer
			<<"\n9. Logout";                //to logout (go to mainmenu)
		cout<<"\nEnter your choice: ";
		cin>>adminmenuask;
		switch(adminmenuask)
		{
			case 1: bankdetails.dispemployee();
					break;
			case 2: if(getlevelafterlogin==1)
						bankdetails.empdelete();
					else
						cout<<"\nYour authorisation level does have not access to this function";
					break;
			case 3: bankdetails.empmodify();
					break;
			case 4: char recreateask='y';
					do
					{
						bankdetails.registercustomer();
						cout<<"\nDo you wish create another employee (y/n): ";
						cin>>recreateask;
						while(!((recreateask=='Y')||(recreateask=='y')||(recreateask=='N')||(recreateask=='n')))
						{
							cout<<"\nINVALID OPTION!!!, please re-enter: ";
							cin>>recreateask;
						}
					}while(recreateask=='y'||recreateask=='Y');
					break;
			case 5: bankdetails.dispcustomer();
					break;
			case 6: viewtransactemp();
					break;
			case 7: viewloanemp();
					break;
			case 8: bankdetails.toempdeletecustomer();
					break;
			case 9: break;
			default: cout<<"\nInvalid choice!!!";
		}
		if(adminmenuask!=9)
			getch();
	}while(adminmenuask!=9);
}

void bank::toaddinteresttodeposit()
{
	time_t now = time(0);       //getting system time
	tm *ltm = localtime(&now);  //getting system current local time to save as the transaction time
	if(!ifstream("lastl.dat"))
	{
		ofstream fout;
		fout.open("lastl.dat",ios::out|ios::binary);
		if(!fout)
		{
			cout<<"\nError";
			exit(0);
		}
		strcpy(lastenter.id, getcustidafterlogin);
		lastenter.hour=ltm->tm_hour;
		lastenter.min=ltm->tm_min;
		lastenter.sec=ltm->tm_sec;
		fout.write((char*)&lastenter, sizeof(lastenter));
		fout.close();
		return;
	}
	fstream f;
	f.open("lastl.dat",ios::in|ios::out|ios::binary);
	if(!f)
	{
		cout<<"\nError";
		exit(0);
	}
	int floc,ffound=0;
	unsigned int ftime;
	fstream fc;
	fc.open("customer.dat",ios::in|ios::out|ios::binary);
	if(!fc)
	{
		cout<<"\nError";
		exit(0);
	}
	int fcloc,fcfound=0,tempbal;
	f.read((char*)&lastenter, sizeof(lastenter));
	while(!f.eof())
	{
		if(strcmp(lastenter.id, getcustidafterlogin)==0)
		{
			ffound=1;
			floc=f.tellg() - sizeof(lastenter);
			ftime=((lastenter.hour*3600)+(lastenter.min*60)+lastenter.sec);
			break;
		}
		f.read((char*)&lastenter, sizeof(lastenter));
	}
	if(ffound==0)
	{
		ofstream fout;
		fout.open("lastl.dat",ios::out|ios::binary);
		if(!fout)
		{
			cout<<"\nError";
			exit(0);
		}
		strcpy(lastenter.id, getcustidafterlogin);
		lastenter.hour=ltm->tm_hour;
		lastenter.min=ltm->tm_min;
		lastenter.sec=ltm->tm_sec;
		fout.write((char*)&lastenter, sizeof(lastenter));
		fout.close();
		return;
	}
	unsigned int systime = ((ltm->tm_hour*3600)+(ltm->tm_min*60)+ltm->tm_sec);
	unsigned int timestoadd = (systime-ftime)/(60*6);
	if(timestoadd==0)
		return;
	else
	{
		fc.read((char*)&custinfo, sizeof(custinfo));
		while(!fc.eof())
		{
			if(strcmp(custinfo.custid, getcustidafterlogin)==0)
			{
				fcfound=1;
				fcloc=fc.tellg() - sizeof(custinfo);
				tempbal=custinfo.balance;
				break;
			}
			fc.read((char*)&custinfo, sizeof(custinfo));
		}
		if(fcfound==0)
			return;
		for(int i=0; i<timestoadd; i++)
		{
			tempbal+=(tempbal*depositinterest/100);
		}
		fc.seekp(fcloc);
		custinfo.balance=tempbal;
		fc.write((char*)&custinfo, sizeof(custinfo));
		fc.close();
		f.seekp(floc);
		if(f.seekp(floc))
		lastenter.hour=ltm->tm_hour;
		lastenter.min=ltm->tm_min;
		lastenter.sec=ltm->tm_sec;
		f.write((char*)&lastenter, sizeof(lastenter));
		getch();
	}
}

//CUSTOMER FUNCTION
void bank::registercustomer()       //Customer Registration
{
	ofstream fout;
	fout.open("customer.dat", ios::binary|ios::app);        //opening customer.dat file in write mode
	if(!fout)
	{
		cout<<"\nError";
		exit(0);
	}
	ifstream fin;
	fin.open("customer.dat", ios::binary|ios::in);          //opening customer.dat file in read mode
	if(!fin)
	{
		cout<<"\nError";
		exit(0);
	}
	clrscr();
	gotoxy(64,2);
	time_t now=time(0);
	tm *ltm=localtime(&now);
	cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
	if(ltm->tm_hour>=13)
		cout<<" PM";
	else
		cout<<" AM";
	gotoxy(12,1);
	cout<<"\nEnter the customer name: ";        //getting customer name
	gets(custinfo.name);
	cout<<"\nEnter the customer age: ";         //getting customer age
	cin>>custinfo.age;
	while(custinfo.age<10)                      //checking if customer age is minimum of 10 years
	{
		//if true then the customer cannot be created
		cout<<"\nSorry your age is less to create a bank account";
		return;
	}
	bank getcustidtemp;
	int gotoreenterpin;
	gotoreenterpin:
	cout<<"\nEnter customer PIN: ";     //to get custmer login pin
	cin>>custinfo.pin;
	cout<<"\nEnter confirm PIN: ";      //to re-enter pin
	int repin;
	cin>>repin;
	if(repin!=custinfo.pin)             //to check the enter pin and re-entered are same
	{
		//if did not match to re-enter
		cout<<"\nConfirm PIN did not match. Please re-enter\n";
		goto gotoreenterpin;
	}
	gen_random(custinfo.custid, 5);     //to create unique customer id
	fin.read((char*)&getcustidtemp.custinfo, sizeof(getcustidtemp.custinfo));
	while(!fin.eof())
	{
		if(strcmp(custinfo.custid, getcustidtemp.custinfo.custid)==0)
			gen_random(custinfo.custid, 5);
		gen_random(custinfo.custid,5);  //to create unique id until end file every time a customer is encountered
		fin.read((char*)&getcustidtemp.custinfo, sizeof(getcustidtemp.custinfo));
	}
	gen_random(custinfo.custid, 5);
	cout<<"\nEnter your monthly salary: ";      //getting customer monthly salary
	cin>>custinfo.salary;
	cout<<"\nEnter your first deposit: ";       //to get first deposit of the customer
	cin>>custinfo.balance;
	while(custinfo.balance<1000)        //to check if deposit is greater than 1000
	{
		//if not then to re-enter first deposit again
		cout<<"\nMinimum balance in account should be 1000. Please re-enter first deposit: ";
		cin>>custinfo.balance;

	}
	cout<<"\nYour Customer ID: ";       //to display unique customer ID
	puts(custinfo.custid);
	custinfo.loantaken=0;               //loan taken set to false
	fout.write((char*)&custinfo, sizeof(custinfo));
	fout.close();           //closing customer.dat file in write mode
	fin.close();            //closing customer.dat file in read mode
	cout<<"\nOur bank will provde interest rates of the following ";
	cout<<"\nDeposit Interest: "<<"2% every 6 minutes will take effect from your first login";
	cout<<"\nLoan Interest: "<<"1% every 3 minutes";
	cout<<"\nCUSTOMER CREATED!!!";
}

int bank::logincustomer()       //Customere Login
{
	ifstream fin;
	fin.open("customer.dat",ios::in|ios::binary);       //opening customer.dat in read mode
	if(!fin)
	{
		cout<<"\nNo Customers Registered";
		return 0;
	}
	clrscr();
	gotoxy(64,2);
	time_t now=time(0);
	tm *ltm=localtime(&now);
	cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
	if(ltm->tm_hour>=13)
		cout<<" PM";
	else
		cout<<" AM";
	gotoxy(12,1);
	char tempcustid[6];
	cout<<"\nEnter your customer ID: ";         //getting id to login
	gets(tempcustid);
	fin.read((char*)&custinfo, sizeof(custinfo));
	int trycustpin=2;
	int found=0;
	int custlogintrue=0;
	while(!fin.eof())
	{
		if(strcmp(tempcustid, custinfo.custid)==0)      //compaing entered id with customer database
		{
			while(trycustpin>=0)
			{
				int temppin;
				cout<<"\nEnter your PIN: ";
				cin>>temppin;
				if(temppin==custinfo.pin)       //comparing entered pin with the saved pin in the database
				{
					custlogintrue=1;
					found=1;
					strcpy(getcustidafterlogin,custinfo.custid);        //copying temporarily customer ID
					if(custinfo.loantaken==1)
						tempafterloginloan=1;                           //to check if loan is taken by the customer
					else
						tempafterloginloan=0;
					break;
				}
				if(found==1)
					break;
				else
				{
					cout<<"\nWrong PIN. Left Attempt: "<<trycustpin<<" Please re-enter: ";
					trycustpin--;
				}
			}
		}
		fin.read((char*)&custinfo,sizeof(custinfo));
	}
	if(custlogintrue==0)
	{
		int found=0;
		ifstream fin;
		fin.open("deleter.dat", ios::in|ios::binary);
		if(!fin)
		{
			cout<<"\nLOGIN FAILED";
			return custlogintrue;
		}
		char deleteidtemp[6];
		fin.read((char*)&custdelete, sizeof(custdelete));
		while(!fin.eof())
		{
			if(strcmp(custdelete.informto, "customer")==0)
			{
				if(strcmp(custdelete.id, tempcustid)==0)
				{
					found=1;
					cout<<"\nYour account has been deleted by the bank.";
					cout<<"\nTime of deletion: ";
					cout<<"\n\nDate: ";
					cout<<custdelete.date<<"/"<<custdelete.month<<"/"<<custdelete.year;
					cout<<"\tTime: ";
					cout<<custdelete.hour<<":"<<custdelete.min<<":"<<custdelete.sec;
					cout<<"\n\nReason: ";
					puts(custdelete.reason);
					strcpy(custdelete.id, deleteidtemp);
					break;
				}
			}
			fin.read((char*)&custdelete, sizeof(custdelete));
		}
		if(found==0)
		{
			cout<<"\nLOGIN FAILED";
		}
		else
		{
			deletefromdeleterecord(deleteidtemp);
		}
		return custlogintrue;
	}
	if(custlogintrue==1)
	{
		loadingbar();
		return custlogintrue;
	}
	return custlogintrue;
	fin.close();        //closing customer.dat file
}

void bank::dispcustprofile()        //to display customer profile data
{
	ifstream fin;
	fin.open("customer.dat", ios::in|ios::binary);      //opening customer.dat file
	if(!fin)
	{
		cout<<"\nError";
		exit(0);
	}
	clrscr();
	gotoxy(64,2);
	time_t now=time(0);
	tm *ltm=localtime(&now);
	cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
	if(ltm->tm_hour>=13)
		cout<<" PM";
	else
		cout<<" AM";
	gotoxy(12,1);
	fin.read((char*)&custinfo, sizeof(custinfo));
	while(!fin.eof())
	{
		if(strcmp(getcustidafterlogin,custinfo.custid)==0)      //comparing customer id with customer database
		{
			cout<<"\nName: ";       //displaying customer name
			puts(custinfo.name);
			cout<<"Customer ID: ";  //displaying customer ID
			puts(custinfo.custid);
			cout<<"Age: ";          //displaying customer age
			cout<<custinfo.age;
			cout<<"\nPIN: ";        //customer PIN (temporarily due to testing)
			cout<<custinfo.pin;
			cout<<"\nSalary: ";     //displaying customer salary
			cout<<custinfo.salary;
			cout<<"\nAccount Balance: ";    //displaying customer account deposit
			cout<<custinfo.balance;
			cout<<"\nLoan Taken: "; //displaying customer has taken a loan or not
			if(custinfo.loantaken==1)
				cout<<"Yes";
			else if(custinfo.loantaken==0)
				cout<<"No";
			break;
		}
		fin.read((char*)&custinfo, sizeof(custinfo));
	}
	fin.close();        //closing customer.dat file
}

void bank::disptransactrecord()     //displaying all the transactions made by the logged-in customer
{
	ifstream fin;
	int i=1;        //used for displaying the index number
	clrscr();
	gotoxy(64,2);
	time_t now=time(0);
	tm *ltm=localtime(&now);
	cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
	if(ltm->tm_hour>=13)
		cout<<" PM";
	else
		cout<<" AM";
	gotoxy(12,1);
	fin.open("transactlog.dat", ios::in|ios::binary);       //opening transactlog.dat file in read mode
	if(!fin)
	{
		cout<<"No transactions were made";
		return;
	}
	fin.read((char*)&transact, sizeof(transact));
	cout<<"\nTransactions Made: \n\n";
	while(!fin.eof())
	{
		if(strcmp(transact.id, getcustidafterlogin)==0)     //comparing logged-in ID wijth customer ID in the file
		{
			cout<<"--------------------------------------";
			cout<<"\n"<<i<<". Type: "<<transact.type;       //displaying the type transaction made (Deposit/Withdrawal)
			cout<<"\nAmount: "<<transact.amount;        //displaying the amount of money transacted
			cout<<"\tTime: ";           //displaying when the transaction was made
			cout<<transact.ti.date<<"."<<transact.ti.month<<"."<<transact.ti.year;
			cout<<"\t"<<transact.ti.hour<<":"<<transact.ti.minute<<":"<<transact.ti.second;
			cout<<"\n\nRemark: "<<transact.remark;      //diaplying the remark of the transaction
			cout<<"\n--------------------------------------";
			i++;        //to increase the index number value by 1
		}
		fin.read((char*)&transact, sizeof(transact));
	}
	fin.close();        //closing transactlog.dat file
}

void bank::transactrecord(char custid[6], int &money, char typ)     //to create log of the customer transaction into the database
{
	ofstream fout;
	fout.open("transactlog.dat", ios::out|ios::app|ios::binary);        //opening transactlog.dat file in write mode
	if(!fout)
	{
		cout<<"\nError";
		exit(0);
	}
	time_t now = time(0);       //getting system time
	tm *ltm = localtime(&now);  //getting system current local time to save as the transaction time
	cout<<"\nRemarks: ";        //getting customer's remark for transaction made
	gets(transact.remark);
	strcpy(transact.id, custid);    //copying the customer ID to the file
	transact.amount = money;        //copying the amount of money transacted to the file
	//copying the type of the type of transaction made by the customer
	if(typ == 'd')
		strcpy(transact.type,"Deposit");
	else if(typ == 'w')
		strcpy(transact.type,"Withdraw");
	transact.ti.date=ltm->tm_mday;      //copying the day of transaction made
	transact.ti.month= 1 + ltm->tm_mon; //copying the month of transaction made
	transact.ti.year=1900 + ltm->tm_year;   //copying the year of transaction made
	transact.ti.hour=ltm->tm_hour;  //copying  the hour the transaction made
	transact.ti.minute=ltm->tm_min; //copying  the minute the transaction made
	transact.ti.second=ltm->tm_sec; //copying  the second the transaction made
	fout.write((char*)&transact, sizeof(transact)); //writing the information the file
	fout.close();       //closing transactlog file
}

void bank::customermoneydeposit()   //to make deposit by a customer
{
	fstream f;
	f.open("customer.dat", ios::in|ios::out | ios:: binary);    //opening customer.dat file in read and write mode
	if(!f)
	{
		cout<<"\nError";
		exit(0);
	}
	clrscr();
	gotoxy(64,2);
	time_t now=time(0);
	tm *ltm=localtime(&now);
	cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
	if(ltm->tm_hour>=13)
		cout<<" PM";
	else
		cout<<" AM";
	gotoxy(12,1);
	int adddeposit;
	f.read((char*)&custinfo, sizeof(custinfo));
	while(!f.eof())
	{
		if(strcmp(getcustidafterlogin,custinfo.custid)==0)      //comparing logged-in ID with customer database to find its location in the file
		{
			f.seekg(0,ios::cur);
			cout<<"\nEnter the amount to deposit: ";        //enter the amount of money to deposit into the account
			cin>>adddeposit;
			custinfo.balance+=adddeposit;       //adding the amount the balance in the customer database
			transactrecord(custinfo.custid, adddeposit, 'd');       //saving the transaction the transaction database
			f.seekp(f.tellg() - sizeof(custinfo));      //moving the writing pointer to the customer location in the database
			f.write((char*)&custinfo, sizeof(custinfo));    //writing the data to the file
			break;
		}
		f.read((char*)&custinfo, sizeof(custinfo));
	}
	f.close();      //closing the customer.dat file
}

void bank::customermoneywithdraw()      //to withdraw money from the account
{
	fstream f;
	f.open("customer.dat", ios::in|ios::out | ios:: binary);    //opening the file customer.dat
	if(!f)
	{
		cout<<"\nError";
		exit(0);
	}
	clrscr();
	gotoxy(64,2);
	time_t now=time(0);
	tm *ltm=localtime(&now);
	cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
	if(ltm->tm_hour>=13)
		cout<<" PM";
	else
		cout<<" AM";
	gotoxy(12,1);
	f.read((char*)&custinfo, sizeof(custinfo));
	char withdrawask;
	int withdrawbalance;
	while(!f.eof())
	{
		if(strcmp(getcustidafterlogin,custinfo.custid)==0)      //to compare the logged-in with customer database
		{
			int withdrawreenter;
			withdrawreenter:
			f.seekg(0,ios::cur);
			cout<<"\nEnter the amount to withdraw: ";
			cin>>withdrawbalance;       //getting the amount to withdraw
			if((custinfo.balance-withdrawbalance)>=1000)        //checking if the account balance is not less than 1000 after the withdraw of money
			{
				custinfo.balance-=withdrawbalance;
				transactrecord(custinfo.custid, withdrawbalance, 'w');      //creating the transaction in the transaction log database
			}
			else
			{
				cout<<"\nYour balance is: "<<custinfo.balance<<".";
				cout<<"\nAfter withdrawal your balance amount will be less the minimum balance amount.";
				cout<<"\nDo you wish to enter another withdraw amount";
				cin>>withdrawask;
				while(!((withdrawask=='Y')||(withdrawask=='y')||(withdrawask=='N')||(withdrawask=='n')))
				{
					cout<<"\nINVALID OPTION!!!, please re-enter: ";
					cin>>withdrawask;
				}
				if(withdrawask=='Y'||withdrawask=='y')
					goto withdrawreenter;
				else if(withdrawask=='N'||withdrawask=='n')
				{
					cout<<"\nTransaction Failed";
					break;
				}
			}
			f.seekp(f.tellg() - sizeof(custinfo));      //moving writing pointer to the customer location in the database
			f.write((char*)&custinfo, sizeof(custinfo));        //updating the database with the information
			break;
		}
		f.read((char*)&custinfo, sizeof(custinfo));
	}
	f.close();
}

void bank::loancreator()        //To Create A Loan For The Customer
{
	ofstream fout;
	fout.open("loandata.dat", ios::out|ios::binary|ios::app);       //opening loandata.dat in write mode
	fstream f;
	f.open("customer.dat", ios::in|ios::out|ios::binary);               //opening customer.dat file in write/read mode
	if(!fout||!f)
	{
		cout<<"\nError";
		exit(0);
	}
	ifstream loanidcheck;
	loanidcheck.open("loandata.dat", ios::in|ios::binary);          //opening loandata.dat file in read mode
	if(!loanidcheck)
	{
		cout<<"\nError";
		exit(0);
	}
	time_t now = time(0);   //getting the system time
	tm*ltm=localtime(&now);     //getting the localtime from the system
	int time;
	int found=0,loc;
	long int tempsalary;        //getting the customer salary temporarily to provide the loan
	f.read((char*)&custinfo, sizeof(custinfo));
	while(!f.eof())
	{
		if(strcmp(getcustidafterlogin, custinfo.custid)==0)
		{
			found = 1;
			tempsalary=custinfo.salary;             //copying the salary to the temporary salary variable
			loc = f.tellg() - sizeof(custinfo);     //getting the location of the customer inn the database
			break;
		}
		f.read((char*)&custinfo, sizeof(custinfo));
	}
	if(found==0)
		cout<<"\nLoan Data Scrambled";
	else if(found==1)
	{
		f.seekp(loc);           //moving writing pointer of the file to the customer's location
		char temp[10];
		gen_random(temp, 10);
		loanidcheck.read((char*)&loanlist, sizeof(loanlist));
		while(!loanidcheck.eof())
		{
			gen_random(temp, 10);       //creating unique Loan ID until end of file
			loanidcheck.read((char*)&loanlist, sizeof(loanlist));
		}
		strcpy(loanlist.loancustid,getcustidafterlogin);    //copying the logged-in customer ID into the loan structure
		strcpy(loanlist.loanid, temp);      //copying the unique loan id into the loandata structure
		int reenteramount;
		reenteramount:
		clrscr();
		gotoxy(64,2);
		cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
		if(ltm->tm_hour>=13)
			cout<<" PM";
		else
			cout<<" AM";
		gotoxy(12,1);
		cout<<"\n Loan ID: ";       //Displaying the unique loan ID
		cout<<loanlist.loanid;
		cout<<"\nEnter the loan amount: ";      //getting amount requested as loan
		cin>>loanlist.principal;
		while(loanlist.principal>(tempsalary*15))       //if loan amount requested is more than 15% of the customer's monthly salary
		{
			char loanamountask='a';
			cout<<"\nSorry the amount you have entered is larger than 15 times your salary.";
			cout<<"\nPress A to enter another amount or C to cancel loan";  //giving option to re-enter the loan amount or to cancel the loan
			cin>>loanamountask;
			while(!(loanamountask=='A'||loanamountask=='a'||loanamountask=='C'||loanamountask=='c'))
			{
				cout<<"\nInvalid option";
				cout<<"\nPlease re-enter(C\A): ";
				cin>>loanamountask;
			}
			loanamountask=tolower(loanamountask);
			switch(loanamountask)
			{
				case 'a': goto reenteramount;
						  break;
				case 'c': return;
						  break;
			}
		}
		int reentertime;
		reentertime:
		cout<<"\nEnter the hours to complete the loan: ";       //asking how many hours (temporarily will be changed to months and days if program is pass) required to complete the loan
		cin>>loanlist.totalhours;
		char loantimeask;
		while(loanlist.totalhours>5)            //maximum amount of hours is 5 hours(temporarily)
		{
			cout<<"\nMaximum amount of time allowed 5 hours.";
			cout<<"\nPress R to re-enter the time required or C to cancel loan: ";
			cin>>loantimeask;       //giving option to re-enter amount of hours or to cancel the loan
			if(!(loantimeask=='R'||loantimeask=='r'||loantimeask=='N'||loantimeask=='n'))
			{
				cout<<"\nInvalid option.\nPlease re-enter: ";
				cin>>loantimeask;
			}
			loantimeask=tolower(loantimeask);
			switch(loantimeask)
			{
				case 'r': goto reentertime;
						  break;
				case 'c': return;
						  break;
			}
		}
		//copying the current localtime so as to compare to know when to pay next
		loanlist.hours = ltm->tm_hour;      //copying the local system hour
		loanlist.min = ltm->tm_min;         //copying the local system minute
		loanlist.sec = ltm->tm_sec;         //copying the local system second
		loanlist.minutes = loanlist.totalhours*60;      //getting the total number of minutes to pay the loan
		loanlist.date = ltm->tm_mday;
		loanlist.month = 1+ltm->tm_mon;
		loanlist.year = 1900 + ltm->tm_year;
		loanlist.interest = loaninterest;       //copying current loan interest so that future changes in interest won't affect the current loan that has been taken
		loanlist.loantimescount = (loanlist.minutes/3);     //calculating and storing the total number of times the customer has to pay the hourly due money
		loanlist.amountloan = (loanlist.principal/loanlist.loantimescount)+((loanlist.interest*loanlist.principal)/100);        //calculating the amount of loan to paid every time
		custinfo.loantaken = 1;     //updating the customer database with the current customer from loan taken false -> true
		f.seekp(loc);       //moving writing pointer to customer's location
		f.write((char*)&custinfo, sizeof(custinfo));
		fout.write((char*)&loanlist, sizeof(loanlist));
	}
	f.close();      //closing customer.dat file
	fout.close();       //closing loandata.dat file in write mode
	loanidcheck.close();        //closing loandata.dat file in read mode
}

void bank::disploantaken()      //To display the loan taken by the customer
{
	ifstream fin;
	fin.open("loandata.dat", ios::in|ios::binary);      //opening loandata.dat file in read mode
	if(!fin)
	{
		cout<<"\nError";
		exit(0);
	}
	fin.read((char*)&loanlist, sizeof(loanlist));
	while(!fin.eof())
	{
		if(strcmp(getcustidafterlogin,loanlist.loancustid)==0)
		{
			clrscr();
			gotoxy(64,2);
			time_t now=time(0);
			tm *ltm=localtime(&now);
			cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
			if(ltm->tm_hour>=13)
				cout<<" PM";
			else
				cout<<" AM";
			gotoxy(12,1);
			cout<<"\nLOAN DETAILS\n\n";
			cout<<"\nLoan ID: ";        //displaying Loan ID
			cout<<loanlist.loanid;
			cout<<"\nTime : ";          //displaying the total hours taken to complete the loan
			cout<<loanlist.totalhours<<" hours";
			cout<<"\nLoan Amount Taken: ";      //displaying the loan amount taken
			cout<<loanlist.principal;
			cout<<"\nAmount Due Per 3 Minutes: ";       //displaying the payment to paid hourly
			cout<<loanlist.amountloan;
			cout<<"\nNo. Of Installment:  ";
			cout<<loanlist.loantimescount;
			return;
		}
		fin.read((char*)&loanlist, sizeof(loanlist));
	}
	fin.close();        //closing loandata.dat file
}

void bank::loancheck()      //To take loan amount from account if customer has to pay as of now
{
	clrscr();
	fstream f;
	f.open("loandata.dat",ios::in|ios::out|ios::binary);    //opening latadata.dat in read/write mode
	if(!f)
	{
		cout<<"\nError";
		exit(0);
	}
	int lloc, timestopay;   //lloc: for the location of customer in the loandata.dat file   //timestopay: to find the total number of times the customer has to pay the loan
	int amounttoeverytime, taking=0;    //amounttoeverytime: amount to pay every 3 minutes  //taking: stores the amount of money that has been taken from the account
	int found=0;
	unsigned int lastsaveloantime;//to calculate the saved time in seconds
	f.read((char*)&loanlist, sizeof(loanlist));
	while(!f.eof())
	{
		if(strcmp(getcustidafterlogin,loanlist.loancustid)==0)
		{
			lloc=f.tellg() - sizeof(loanlist);
			lastsaveloantime = (loanlist.hours*3600)+(loanlist.min*60)+loanlist.sec; //calculation of the saved time in seconds
			found=1;
			timestopay=loanlist.loantimescount; //storing the total number of times the customer has to pay for the loan
			amounttoeverytime=loanlist.amountloan;
			break;
		}
		f.read((char*)&loanlist, sizeof(loanlist));
	}
	int finishloan=0;
	if(found==1)
	{
		time_t now = time(0);   //getting the system time
		tm*ltm=localtime(&now);     //getting the localtime from the system
		unsigned int currenttimeinsecond = ((ltm->tm_hour)*3600)+((ltm->tm_min)*60)+ltm->tm_sec;    //calculate the system time in seconds
		unsigned int differencetimeinsec = currenttimeinsecond - lastsaveloantime;      //calculate the difference between the system time and saved time in seconds
		int timesleftcalculated = (differencetimeinsec/60)/3;   //converting the difference from seconds to minutes and finding how many 3 minutes have been elapsed
		if((timestopay-timesleftcalculated)<=0) //checking if loan is complete or not
		{
			timesleftcalculated=timestopay;
			finishloan=1;
		}
		int tempunchangebalance, tempchangebalance;
		int custloc;
		fstream custf;
		custf.open("customer.dat", ios::in|ios::out|ios::binary);       //opening customer.dat file in read/write mode
		if(!custf)
		{
			cout<<"\nError";
			exit(0);
		}
		custf.read((char*)&custinfo, sizeof(custinfo));
		while(!custf.eof())
		{
			if(strcmp(custinfo.custid, getcustidafterlogin)==0)
			{
				tempchangebalance=tempunchangebalance=custinfo.balance;
				custloc=custf.tellg() - sizeof(custinfo);
				break;
			}
			custf.read((char*)&custinfo, sizeof(custinfo));
		}
		for(int i=0; i<timesleftcalculated; i++)        //to take the amount from the account, number of times until now to pay
		{
			if((tempchangebalance- amounttoeverytime)<=1000)    //checking if the balance will become thousand or below if the amount is taken from the account
			{
				// if true telling them deposit amount before moving on
				int tempaddmoney;
				cout<<"\nYour account balance is: "<<tempchangebalance;
				cout<<"\nPlease deposit an amount of "<<amounttoeverytime*(i+1)<<" to complete the loan amount until now: ";
				cin>>tempaddmoney;
				tempchangebalance+=tempaddmoney;
				i--;
			}
			else
			{
				tempchangebalance-=amounttoeverytime;
				taking=taking+amounttoeverytime;
			}
		}
		if(finishloan==1)   //if loan is finished then to remove the customer loan record and setting the loantaken variable as false
		{
			cout<<"\nYou have successful finished your loan. You can now take another loan.";
			cout<<"\nThank you for co-operation";
			//deleting the customer loan record from the loandata.dat file
			ifstream fin;
			fin.open("loandata.dat",ios::in|ios::binary);
			if(!fin)
			{
				cout<<"\nError";
				exit(0);
			}
			ofstream fout;
			fout.open("temp.dat", ios::out|ios::binary);
			if(!fout)
			{
				cout<<"\nError";
				exit(0);
			}
			int deletecustidfromloan=0;
			fin.read((char*)&loanlist, sizeof(loanlist));
			while(!fin.eof())
			{
				if(strcmp(loanlist.loancustid, getcustidafterlogin)==0)
					deletecustidfromloan=1;
				else
					fout.write((char*)&loanlist, sizeof(loanlist));
				fin.read((char*)&loanlist, sizeof(loanlist));
			}
			if(deletecustidfromloan==1)
			{
				fin.close();
				fout.close();
				remove("loandata.dat");
				rename("temp.dat", "loandata.dat");
			}
			else
			{
				fin.close();
				fout.close();
				remove("temp.dat");
				cout<<"\nLoan Check Delete Function Scrambled";
				getch();
			}
			custf.seekp(custloc);
			custinfo.loantaken=0;
			custinfo.balance=tempunchangebalance;
			custf.write((char*)&custinfo, sizeof(custinfo));
			getch();
		}
		else if(timesleftcalculated!=0)     //if not finished then to update the time of the loan and reduce the number of times the loan has to paid
		{
			tempunchangebalance=tempchangebalance;
			f.seekp(lloc);
			loanlist.loantimescount-=timesleftcalculated;
			loanlist.hours=ltm->tm_hour;
			loanlist.min=ltm->tm_min;
			loanlist.sec=ltm->tm_sec;
			f.write((char*)&loanlist, sizeof(loanlist));
			custf.seekp(custloc);
			custinfo.balance=tempunchangebalance;   //removing amount from account
			custf.write((char*)&custinfo, sizeof(custinfo));
			if(taking!=0)
			{
				cout<<"\n\nAn amount of "<<taking<<" has been taken from deposit as payment of the loan";
				cout<<"\nThank You for your co-operation";
				cout<<"\n\nPress a key to continue";
				getch();
			}
		}
	}
}

void bank::loanmenu()       //if customer has taken a loan then it display the loan or it allows the customer to create a loan.(only one loan can be taken at a time)
{
	ifstream fin;
	fin.open("customer.dat", ios::in|ios::binary);
	if(!fin)
	{
		cout<<"\nError";
		exit(0);
	}
	fin.read((char*)&custinfo, sizeof(custinfo));
	while(!fin.eof())
	{
		if(strcmp(getcustidafterlogin,custinfo.custid)==0)
		{
			if(custinfo.loantaken==0)       //checking if the customer has taken a loan or not
			{
				//if loan taken false, call function to create a loan
				loancreator();
			}
			if(custinfo.loantaken==1)
			{
				//if loan taken true, call function to display the loan taken by the customer
				disploantaken();
			}
			break;
		}
		fin.read((char*)&custinfo, sizeof(custinfo));
	}
	fin.close();        //closing customer.dat file
}

void bank::customerdetailsmodify()      //To modify the details of the customer
{
	int custmodifymenuask;
	fstream f;
	f.open("customer.dat", ios::in|ios::out| ios::binary);      //opening customer.dat file in read/write mode
	if(!f)
	{
		cout<<"\nError";
		exit(0);
	}
	int found=0, loc;
	f.read((char*)&custinfo, sizeof(custinfo));
	while(!f.eof())
	{
		if(strcmp(getcustidafterlogin,custinfo.custid)==0)
		{
			found = 1;
			loc = f.tellg() - sizeof(custinfo);     //finding location of customer in database
			break;
		}
		f.read((char*)&custinfo, sizeof(custinfo));
	}
	if(found==0)
		cout<<"\nModify Function Scrambled";
	else if(found==1)
	{
		clrscr();
		gotoxy(64,2);
		time_t now=time(0);
		tm *ltm=localtime(&now);
		cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
		if(ltm->tm_hour>=13)
			cout<<" PM";
		else
			cout<<" AM";
		gotoxy(12,1);
		char custmodifymenuanotherask='y';
		while(custmodifymenuanotherask=='Y'||custmodifymenuanotherask=='y')
		{
			f.seekp(loc);       //moving reading pointer to customer location in the database
			cout<<"\n1. Name"
				<<"\n2. PIN"
				<<"\n3. Age"
				<<"\n4. Salary"
				<<"\n5. Cancel";
			cout<<"\nEnter your choice : ";     //asking which detail to modify
			cin>>custmodifymenuask;
			switch(custmodifymenuask)
			{
				case 1: cout<<"\nEnter the new name to change: ";
						gets(custinfo.name);
						break;
				case 2: int trail=3;
						do
						{
							cout<<"\nEnter your previous PIN: ";
							int checkpin,repin, wrongpin=0;
							cin>>checkpin;
							if(checkpin==custinfo.pin)
							{
								do
								{
									cout<<"\nEnter new PIN: ";
									cin>>custinfo.pin;
									cout<<"\nConfirm PIN: ";
									cin>>repin;
									if(repin==custinfo.pin)
										wrongpin=1;
									else
										cout<<"\nThe PIN did not match. Please re-enter: ";
								}while(wrongpin==0);
							}
							else
								cout<<"\nPIN is incorrect. Attempts left: "<<trail--<<" of 3";
						}while(trail!=0);
						break;
				case 3: cout<<"\nEnter your new age: ";
						cin>>custinfo.age;
						while(custinfo.age>=18)
						{
							cin>>custinfo.age;
						}
						break;
				case 4: if(custinfo.loantaken==0)
						{
							cout<<"\nEnter your new salary: ";
							cin>>custinfo.salary;
						}
						else if(custinfo.loantaken==1)
							cout<<"\nYou have taken a loan. \nYou cannot change your salary right now";
						break;
				case 5: break;
				default: cout<<"\nINVALID CHOICE";
			}
			cout<<"\nDo you wish to change another detail (Y/N): ";     //asking if another customer detail has to changed
			cin>>custmodifymenuanotherask;
			while(!(custmodifymenuanotherask!='Y'||custmodifymenuanotherask!='y'||custmodifymenuanotherask!='N'||custmodifymenuanotherask!='n'))
			{
				cout<<"\nInvlid Option. Please re-enter: ";
				cin>>custmodifymenuanotherask;
			}
			f.write((char*)&custinfo, sizeof(custinfo));
		}
	}
	if(custmodifymenuask!=5||custmodifymenuask=='n'||custmodifymenuask=='N')
		cout<<"\nRECORDS MODIFIED";
	cout<<"\nClosing Modify Operation.";
	delay(1000);
	f.close();      //closing customer.dat file
}

void bank::transfertoanotheraccount()       //function to transfer amount to another account
{
	clrscr();
	gotoxy(64,2);
	time_t now=time(0);
	tm *ltm=localtime(&now);
	cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
	if(ltm->tm_hour>=13)
		cout<<" PM";
	else
		cout<<" AM";
	gotoxy(12,1);
	ifstream fin;
	fin.open("customer.dat", ios::in|ios::binary);      //opening customer.dat file in read mode
	if(!fin)
	{
		cout<<"\nError";
		exit(0);
	}
	int count=0;
	fin.seekg(0,ios::beg);
	fin.read((char*)&custinfo, sizeof(custinfo));
	while(!fin.eof())
	{
		count++;
		fin.read((char*)&custinfo, sizeof(custinfo));
	}
	if(count==1)
	{
		cout<<"\nThere are no other customers in the bank to transfer. \nPlease try again later";
		cout<<"\nPress any key to continue";
		getch();
		return;
	}
	cout<<"\nEnter the customer ID you want transfer to: ";     //getting customer ID to transfer the money
	char transfercustid[6];
	gets(transfercustid);
	if(strcmp(transfercustid, getcustidafterlogin)==0)      //checking if the entered customer name is of the same customer who is transferring the money
	{
		cout<<"\nYou cannot tranfer money to your own account";
		cout<<"\nTransfer Failed";
		return;
	}
	int found=0;
	fin.close();
	fin.open("customer.dat", ios::in|ios::binary);
	if(!fin)
	{
		cout<<"\nError";
		exit(0);
	}
	fin.read((char*)&custinfo, sizeof(custinfo));
	while(!fin.eof())
	{
		if(strcmp(transfercustid, custinfo.custid)==0)      //checking if such a customer exist in file
		{
			found=1;
			strcpy(recievename,custinfo.name);
		}
		fin.read((char*)&custinfo, sizeof(custinfo));
	}
	if(found==0)
	{
		cout<<"\nSuch an customer ID does not exist";
		cout<<"\nTransfer Failed";
		return;
	}
	else
	{
		cout<<"\nEnter the amount to transfer: ";       //getting the amount to transfer
		int transferamount;
		cin>>transferamount;
		strcpy(recievecustid, transfercustid);      //copying the customer ID to transfer amount to a global variable
		int fail = transfersendertransactlog(transferamount);       //use fail is to check if balance of the customer who is sending the money does not go below the minimum (checking done in the next below function)
		if(fail == 0)
			recievefromanotheraccount(transferamount);
	}
	fin.close();
}

int bank::transfersendertransactlog(int &money)
{
	fstream f;
	f.open("customer.dat", ios::out|ios::in|ios::binary);       //opening the customer.dat file in read/write mode
	if(!f)
	{
		cout<<"\nError";
		exit(0);
	}
	int loc;
	f.read((char*)&custinfo, sizeof(custinfo));
	while(!f.eof())
	{
		if(strcmp(custinfo.custid, getcustidafterlogin)==0)
		{
			if((custinfo.balance-money)<1000)       //checking the amount will go below the minimum (use of fail in the previous function)
			{
				cout<<"\nYour account balance is low";
				cout<<"\nTransfer Fail";
				return 1;
			}
			strcpy(sendname,custinfo.name);     //copying the name of the customer who is sending the money to a global variable
			loc=f.tellg()-sizeof(custinfo);
			break;
		}
		f.read((char*)&custinfo, sizeof(custinfo));
	}
	f.seekp(loc);
	custinfo.balance-=money;        //removing money from the account to transfer
	f.write((char*)&custinfo, sizeof(custinfo));
	f.close();
	ofstream fout;
	fout.open("transactlog.dat", ios::out|ios::app|ios::binary);        //opening transactlog.dat file in write mode
	if(!fout)
	{
		cout<<"\nError";
		exit(0);
	}
	time_t now = time(0);       //getting system time
	tm *ltm = localtime(&now);  //getting system current local time to save as the transaction time
	strcpy(transact.remark, "Transferred to: ");    //remark is automatically saved as transferring to
	strcat(transact.remark, recievename);           //(continuation of previous comment) concatinating the the remaining of the string with the name of the customer
	strcpy(transact.id, getcustidafterlogin);   //copying the customer ID to the file
	transact.amount = money;        //copying the amount of money transacted to the file
	strcpy(transact.type,"Transfer");       //type of the transaction is saved as transfer
	transact.ti.date=ltm->tm_mday;      //copying the day of transaction made
	transact.ti.month= 1 + ltm->tm_mon; //copying the month of transaction made
	transact.ti.year=1900 + ltm->tm_year;   //copying the year of transaction made
	transact.ti.hour=ltm->tm_hour;  //copying  the hour the transaction made
	transact.ti.minute=ltm->tm_min; //copying  the minute the transaction made
	transact.ti.second=ltm->tm_sec; //copying  the second the transaction made
	fout.write((char*)&transact, sizeof(transact)); //writing the information the file
	fout.close();       //closing transactlog file
	return 0;
}

void bank::recievefromanotheraccount(int &money)
{
	fstream f;
	f.open("customer.dat", ios::out|ios::in|ios::binary);       //opening customer.dat file in read/write mode
	if(!f)
	{
		cout<<"\nError";
		exit(0);
	}
	int loc;
	f.read((char*)&custinfo, sizeof(custinfo));
	while(!f.eof())
	{
		if(strcmp(custinfo.custid, recievecustid)==0)
		{
			loc=f.tellg()-sizeof(custinfo);
			break;
		}
		f.read((char*)&custinfo, sizeof(custinfo));
	}
	f.seekp(loc);
	custinfo.balance+=money;    //adding the amount of money transferred to this customer the account balance
	f.write((char*)&custinfo, sizeof(custinfo));
	f.close();
	ofstream fout;
	fout.open("transactlog.dat", ios::out|ios::app|ios::binary);        //opening transactlog.dat file in write mode
	if(!fout)
	{
		cout<<"\nError";
		exit(0);
	}
	time_t now = time(0);       //getting system time
	tm *ltm = localtime(&now);  //getting system current local time to save as the transaction time
	strcpy(transact.remark, "Recieved From: ");     //remark is automatically saved as recieved from
	strcat(transact.remark, sendname);      //(continuation of previous comment) concatinating the the remaining of the string with the name of the customer
	strcpy(transact.id, recievecustid); //copying the customer ID to the file
	transact.amount = money;        //copying the amount of money transacted to the file
	strcpy(transact.type,"Recieved");   //type of the transaction is saved as recieved
	transact.ti.date=ltm->tm_mday;      //copying the day of transaction made
	transact.ti.month= 1 + ltm->tm_mon; //copying the month of transaction made
	transact.ti.year=1900 + ltm->tm_year;   //copying the year of transaction made
	transact.ti.hour=ltm->tm_hour;  //copying  the hour the transaction made
	transact.ti.minute=ltm->tm_min; //copying  the minute the transaction made
	transact.ti.second=ltm->tm_sec; //copying  the second the transaction made
	fout.write((char*)&transact, sizeof(transact)); //writing the information the file
	fout.close();       //closing transactlog file
}

int bank::customerdeleteaccount()
{
	clrscr();
	int deletereply = deletecustomer(getcustidafterlogin);
	if(deletereply==0)
	{
		cout<<"\nSuch a customer does not exist";
		return 1;
	}
	else if(deletereply==1)
	{
		cout<<"\nWe are sorry to hear that you are leaving our bank. \nPlease write a feedback";
		cout<<"\nFeedback: ";
		gets(custdelete.reason);
		ofstream fout;
		fout.open("deleter.dat",ios::out|ios::app|ios::binary);
		if(!fout)
		{
			cout<<"\nError";
			exit(0);
		}
		strcpy(custdelete.informto, "employee");
		strcpy(custdelete.id,getcustidafterlogin);
		strcpy(custdelete.name, deletename);
		time_t now = time(0);       //getting system time
		tm *ltm = localtime(&now);  //getting system current local time to save as the transaction time
		custdelete.date=ltm->tm_mday;
		custdelete.month= 1 + ltm->tm_mon;
		custdelete.year=1900 + ltm->tm_year;
		custdelete.hour=ltm->tm_hour;
		custdelete.min=ltm->tm_min;
		custdelete.sec=ltm->tm_sec;
		fout.write((char*)&custdelete, sizeof(custdelete));
		fout.close();
		ifstream fin;
		fin.open("lastl.dat", ios::in|ios::binary);
		if(!fin)
		{
			cout<<"\nError";
			exit(0);
		}
		ofstream fo;
		fo.open("temp.dat",ios::out|ios::binary);
		if(!fout)
		{
			cout<<"\nError";
			exit(0);
		}
		int deletefound=0;
		fin.read((char*)&lastenter, sizeof(lastenter));
		while(!fin.eof())
		{
			if(strcmp(lastenter.id, getcustidafterlogin)==0)
				deletefound=1;
			else
				fo.write((char*)&lastenter, sizeof(lastenter));
			fin.read((char*)&lastenter, sizeof(lastenter));
		}
		if(deletefound==1)
		{
			fo.close();
			fin.close();
			remove("lastl.dat");
			rename("temp.dat", "lastl.dat");
		}
		else
		{
			fo.close();
			fin.close();
			remove("temp.dat");
		}
		return 2;
	}
	else if(deletereply==2)
	{
		cout<<"\nYou cannot delete your account as you have a loan to complete";
		return 3;
	}
}

void customermenu()     //Customer Menu (Not a member function of the class)
{
	bankdetails.toaddinteresttodeposit();
	if(tempafterloginloan==1)       //to check if an loan due amount has to be paid and to take from the deposit of the customer
		bankdetails.loancheck();
	int customermenuask;
	do
	{
		clrscr();
		gotoxy(64,2);
		time_t now=time(0);
		tm *ltm=localtime(&now);
		cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
		if(ltm->tm_hour>=13)
			cout<<" PM";
		else
			cout<<" AM";
		gotoxy(12,1);
		cout<<"\n1. Display profile"    //to display the profile of the customer
			<<"\n2. Deposit money"      //to deposit money into the account
			<<"\n3. Withdraw money"     //to withdraw money from the account
			<<"\n4. Transaction Log"    //to see list all the transactions made
			<<"\n5. Modify profile"     //to make changes in the customer details by the customer
			<<"\n6. Loan"               //to a take a loan if not taken and  if loan is taken to display the information of the taken amount by the customer
			<<"\n7. Transfer money"     //to transfer money to another account
			<<"\n8. To delete account"      //customer account to delete
			<<"\n9. Logout";            //to go to the mainmenu
		cout<<"\nEnter your choice: ";
		cin>>customermenuask;
		switch(customermenuask)
		{
			case 1: bankdetails.dispcustprofile();
					break;
			case 2: bankdetails.customermoneydeposit();
					break;
			case 3: bankdetails.customermoneywithdraw();
					break;
			case 4: bankdetails.disptransactrecord();
					break;
			case 5: bankdetails.customerdetailsmodify();
					break;
			case 6: bankdetails.loanmenu();
					break;
			case 7: bankdetails.transfertoanotheraccount();
					break;
			case 8: int remove = bankdetails.customerdeleteaccount();
				if(remove==3||remove==1)
						break;
					else if(remove==2)
					{
						cout<<"\n\nAccount deleted";
						getch();
						return;
					}
					break;
			case 9: break;
			default: cout<<"\nInvalid Choice!!!";
		}
		if(customermenuask!=9)
			getch();
	}while(customermenuask!=9);
}

void bank::dispcustomer()       //to display all the customers in the bank
{
	ifstream fin;
	fin.open("customer.dat",ios::in|ios::binary);       //opening customer.dat file in read mode
	if(!fin)
	{
		cout<<"\nNo Customers Registered";
		return;
	}
	clrscr();
	gotoxy(64,2);
	time_t now=time(0);
	tm *ltm=localtime(&now);
	cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
	if(ltm->tm_hour>=13)
		cout<<" PM";
	else
		cout<<" AM";
	gotoxy(12,1);
	cout<<"\nList of all customer are below: ";
	cout<<"\n\n";
	fin.read((char*)&custinfo, sizeof(custinfo));
	while(!fin.eof())
	{
		cout<<"\nName: ";       //to display the name of the customer
		puts(custinfo.name);
		cout<<"\nCustomer ID: ";        // to display the customer ID
		cout<<custinfo.custid;
		cout<<"\nAge: ";        //to display the customer age
		cout<<custinfo.pin;
		cout<<"\nPIN: ";        //to display the customer PIN (temporary will removed after development)
		cout<<custinfo.pin;
		cout<<"\nSalary: ";     //to display the customer's monthly salary
		cout<<custinfo.salary;
		cout<<"\nBalance: ";    //to display the current balance in the account
		cout<<custinfo.balance;
		cout<<"\n\n";
		fin.read((char*)&custinfo, sizeof(custinfo));
	}
	fin.close();        //closing customer.dat file
	cout<<"\nEND OF RECORD!!!";
}

void main()
{
	int exist=1, custexist=1;
	if(!ifstream("admin.dat"))      //to check if employee file already exists or not
	{
		exist=0;
	}
	clrscr();
	//below decorations are functional are currently stopped until construction due to pause at start during start of program
	//you can remove the comments of the function, the program will work but be warned it will disturbance during debugging of the program
	asciiart();
	delay(2000);
	if(exist==0)
	{
	clrscr();
	gotoxy(26,7);
	cout<<"\n\t\t\t\tNo Employees found";
	cout<<"\n\t\t    Press R for registration or E to Exit (R/E): ";
	char ask;
	cin>>ask;
	while(!(ask=='r'||ask=='R'||ask=='e'||ask=='E'))
	{
		cout<<"\n\t\t\t\tInvalid choice!!!.\n\t\t\t\tPlease re-enter(R/E): ";
		cin>>ask;
	}
	ask=tolower(ask);
	switch(ask)
	{
		case 'e': clrscr();
				  cout<<"\n\n";
				  cout<<"\n\t\t\t\tClosing program";
				  delay(1000);
				  exit(0);
		case 'r': break;
	}
	}
	loadingbar();
	 char mainmenuchoice;
	do
	{
		//MAIN MENU OF THE PROGRAM
		if(!ifstream("customer.dat"))       //to check if customer file already exists
			custexist=0;
		else
			custexist=1;
		if(exist==1)
		{
			clrscr();
			cout<<"\n---------------------------------Main Menu-------------------------------";
			gotoxy(64,4);
			time_t now=time(0);
			tm *ltm=localtime(&now);
			cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec;
			if(ltm->tm_hour>=13)
				cout<<" PM";
			else
				cout<<" AM";
			gotoxy(12,3);
			cout<<"\nLogin As: ";
			cout<<"\n\n1. Adminstrator"
				<<"\n2. Customer"
				<<"\n3. Exit";
			gotoxy(35,24);      // to move to the output writing pointer to the end of the screen to print the below statement
			cout<<"Press 'r' to access the registration section.";
			gotoxy(12,8);       // to go back to the previous location of the output pointer
			cout<<"\nEnter your choice: ";      //asking for to login type or to register an account
			cin>>mainmenuchoice;
		}
		else
			mainmenuchoice='r';
		switch(mainmenuchoice)
		{
			case '1':   int adminlogin=bankdetails.emplogin();
						if(adminlogin==1)
							adminmenu();
						break;

			case '2':     if(custexist==1)
						{
							int clientlogin=bankdetails.logincustomer();
							if(clientlogin==1)
								customermenu();
						}
						else
						{
							cout<<"\nNo Customer Registered";
						}
						break;

			case '3':   break;

			case 'r':   //Registration Menu
						char recreateadminask;
						char recreatecustask;
						int registadminask;
						clrscr();
						cout<<"\n---------------------------------Registration Section---------------------------------";
						if(exist==1)
						{
							cout<<"\n1. Employee";
							cout<<"\n2. Customer ";
							cout<<"\nEnter your choice: ";      //type of account to be created
							cin>>registadminask;
						}
						else
							registadminask=1;
						switch(registadminask)
						{
							case 1: do
										{
										bankdetails.registeremployee();
										exist=1;
										cout<<"\nDo you wish create another employee (y/n): ";
										cin>>recreateadminask;
										while(!((recreateadminask=='Y')||(recreateadminask=='y')||(recreateadminask=='N')||(recreateadminask=='n')))
										{
											cout<<"\nINVALID OPTION!!!, please re-enter: ";
											cin>>recreateadminask;
										}
									}while(recreateadminask=='y'||recreateadminask=='Y');
									break;
							case 2: do
									{
										bankdetails.registercustomer();
										cout<<"\nDo you wish create another customer (y/n): ";
										cin>>recreatecustask;
										while(!((recreatecustask=='Y')||(recreatecustask=='y')||(recreatecustask=='N')||(recreatecustask=='n')))
										{
											cout<<"\nINVALID OPTION!!!, please re-enter: ";
											cin>>recreatecustask;
										}
									}while(recreatecustask=='y'||recreatecustask=='Y');
									break;
							 default: cout<<"\nINVALID CHOICE!!!";
						}
						break;
			default :   cout<<"\nINVALID CHOICE!!!";
		}
		if(mainmenuchoice!='3')
			getch();
	}while(mainmenuchoice!='3') ;
}