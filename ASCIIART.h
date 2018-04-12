void asciiart()
{
  clrscr();
  ifstream fin;
  fin.open("bankart.txt", ios::nocreate, ios::in);
  if(!fin)
  {
    cout<<"\nError";
    exit(0);
  }
  char getart[80];
  cout<<"\n\n\n\n";
  while(!fin.eof())
  {
    fin.getline(getart, 80);
    cout<<"\t     ";
    cout<<getart<<"\n";
  }
  fin.clear();
  fin.close();
}