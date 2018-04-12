void loadingbar()
{
  for(int i=0; i<3; i++)
  { 
    for(int j=0; j<1; j++)
    { 
      clrscr();
      gotoxy(26,12);
      cout<<"LOADING FILES, PLEASE WAIT...";
      cout<<"\n\t\t\t";
      for(int k=0; k<31; k++)
      { 
        cout<<"|";
        delay(30);
      }
    }
  }
}