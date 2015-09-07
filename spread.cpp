// Project 3: Spreadsheet
// CE 40-153
// By: Pegah Jandaghi(90105978)


#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Chart.H>
#include <FL/Fl_Scroll.H>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>

#define MAX_DIGITS 50
#define NUM_ROWS 50
#define NUM_COLS 26


using namespace std;

void get_matrix (int num[][NUM_COLS]);
string big_parse(string exp);
double parse (string exp);
int range_length (string a, int start);
void read_range (string range, char str[], int num[]);

char alpha_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char *strV[] = {"1 ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "10",
                     "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
                     "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
                     "31", "32", "33", "34", "35", "36", "37", "38", "39", "40",
                     "41", "42", "43", "44", "45", "46", "47", "48", "49", "50"};
                     
                     
char *strH[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
                    "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    
/* -------------------- Table -------------------- */
struct Cell {
    string data;    // cell's input data
    double value;   // cell's value
    Fl_Input *box;  // pointer to the cell's input box
    int empty;      //cell is emty or full
    int matrix[NUM_ROWS][NUM_COLS];   // matrice mojaverat
    int is_string;   //if the cell is string
    
    Cell() 
    { 
           empty = 0;  //cell is empty
           value = 0;
           is_string = 0;
           for (int i = 0; i < NUM_ROWS; i++)
           {
               for (int j = 0; j < NUM_COLS; j++)
                   matrix[NUM_ROWS][NUM_COLS] = 0;
           }
    }  // initializer
};


Cell table[NUM_ROWS][NUM_COLS]; // spreadsheet table


 /*-------------------bignum table --------------------*/
 
struct BigCell {
    string data;    // cell's input data
    string value;   // cell's value
    Fl_Input *box;  // pointer to the cell's input box
    int empty;      // if the cell is empty or full
    
    BigCell()
    {
             empty = 0; //the cell is empty
             value[0] = NULL;
    }
};


BigCell tableBig[NUM_ROWS][NUM_COLS];


// lookup the value of a cell by its index
double get_value (char col, int row) {
    return table[row - 1][col - 'A'].value;
}


// lookup the value of a cell by its name
double get_value (string name) {
    return get_value (name[0], atoi(name.c_str() + 1));
}

//lookup the value of a big cell by its name
string big_get_value (char col, int row){  
       return tableBig[row - 1][col - 'A'].value;
}


/*--------------- Big Number Operations --------------- */
// add two big numbers (a = a + b)
void big_add (int a[], int b[]) {
    int i;
    int temp[MAX_DIGITS] = {0};
    
    for (i = 0; i < MAX_DIGITS; i++)
        temp[i] = a[i] + b[i];
        
    for (i = 0; i < MAX_DIGITS - 1; i++)
    {
        temp[i + 1] += temp[i] / 10;
        temp[i] %= 10;
    }
    
    for (i = 0; i < MAX_DIGITS; i++)
       a[i] = temp[i];
}


//relax for substract
void relaxf (int array[]){
     int i;
     
     for (i = 0; i < MAX_DIGITS - 1; i++)
     {
         if (array[i] < 0)
         {
            array[i + 1] -= 1;
            array[i] = 10 + array[i];
         }
     }
}


// subtract two big numbers (a = a - b)
void big_subtract (int a[], int b[]) {
    int i;
    int temp[MAX_DIGITS] = {0};
       
    for (i = 0; i < MAX_DIGITS; i++)
        temp[i] = a[i] - b[i];
    
    relaxf(temp);
    
    for (i = 0; i < MAX_DIGITS; i++)
        a[i] = temp[i];
}


// multiply two big numbers (a = a * b)
void big_multiply (int a[], int b[]) {
    int i, j;    
    int temp[MAX_DIGITS] = {0};

    for (i = 0; i < MAX_DIGITS; i++)
    {
       for (j = 0; j < MAX_DIGITS; j++)
            temp[i + j] += a[i] * b[j];
    }
    
    for (i = 0; i < MAX_DIGITS - 1; i++)
    {
        temp[i + 1] += temp[i] / 10;
        temp[i] %= 10;
    }
      
      
    for (i = 0; i < MAX_DIGITS; i++)
        a[i] = temp[i];
}


//find the length of  big string
int length (string a)
{
    int len = 0 ;
    while (a[len] >= '0' && a[len] <= '9' )
       len++;
       
    return len;
}


//convert string to big num
void str_to_big (string exp, int num[], int len, int x = 2)
{
     int end = len;
     if (end > MAX_DIGITS)
        end = MAX_DIGITS;
     
     int i;
     int j = 0;
     for (i = end - x; i >= 0; i--)
     {
         num[j] = exp[i] - '0';
         j++;
     }
     
     return;
}


//comparison 2 big numbers
int big_comparison (string big_num1, string big_num2)
{
    int length1 = big_num1.length();
    int length2 = big_num2.length();
    
    if (length1 < length2)
        return -1;
        
    if (length2 < length1)
       return 1;
       
    int i;
    for (i = 0; i < length1; i++)
    { 
        if (big_num1[i] > big_num2[i])
            return 1;
           
        if (big_num1[i] < big_num2[i])
            return -1;
              
    }
    return 0;
}


/*-------------------bignum parse-----------------*/
void calce (string b, int start, int &end, int num[]);

//calcn for bignum
void calcn (string a, int start, int &end, int num[])
{
     int len;
     while (a[start] == ' ')
        start++;
        
     if (a[start] == '(')
     {
        calce (a, start + 1, end, num);
        if (a[end] == ')')
          end ++;
        
        while (a[end] == ' ')
           end ++;
        return;
     }
     
     if (a[start] == 's' && a[start + 1] == 'u')
     {
           int i, j, k = 0;
           char str[2];
           int num2[2];
           start += 2;
           
           while (a[start] != '(')
              start++;
           
           int size = range_length (a, start + 1);
           string my_range = a.substr (start + 1, size);
           end = start;
           
           //find the cells in range
           read_range (my_range, str, num2);
           int height = str[1] - str[0] + 1;
           int width = num2[1] - num2[0] + 1;
           
           end += size;
           
           for (i = 0; i < 22; i++)
           {
               if (str[0] == alpha_table[i])
               {
                  k = i; 
                  break;
               }
           } 
            
           //find the sum of cells  in range
           for (i = 0; i < height; i++)
           {
               for (j = 0; j < width; j++){ 
                   int temp[MAX_DIGITS] = {0};
                   int len;
                   
                   //get the cell value and find the length of num
                   string cell_value = big_get_value (alpha_table[k + i], num2[0] + j);
                   for (len = 0; len < MAX_DIGITS; len++)
                   {
                       if (cell_value[len] == '\0')
                          break;
                   }
                   
                   str_to_big (cell_value, temp, len - 1, 0)  ; 
                   big_add (num, temp);
               }
         }
            
            if (a[end] == ')')
               end++;
                     
            while (a[end] == ' ')
              end++;
                     
            return;
     }
     
      if (a[start] == 'm' && a[start + 1] == 'i')
     {
           int i, j, k = 0;
           char str[2];
           int num2[2];
           start += 2;
           
           while (a[start] != '(')
              start++;
           
           int size = range_length (a, start + 1);
           string my_range = a.substr (start + 1, size);
           end = start;
           
           //find the cells in range
           read_range (my_range, str, num2);
           int height = str[1] - str[0] + 1;
           int width = num2[1] - num2[0] + 1;
           
           end += size;
           
           for (i = 0; i < 22; i++)
           {
               if (str[0] == alpha_table[i])
               {
                  k = i; 
                  break;
               }
           } 
            
          
           //find the min of cells  in range
           int y = 0;
           int x = 0; 
           
           for (i = 0; i < height; i++)
           {
               for (j = 0; j < width; j++)
               {
                   int len;
                   if (big_comparison(big_get_value(alpha_table[k + y], num2[0] + x), big_get_value(alpha_table[k + i], num2[0] + j)) == 1)
                   {
                       x = j;
                       y = i;
                   }
               }
           } 
           
           string res = big_get_value(alpha_table[k + y], num2[0] + x);       
            for (len = 0; len < MAX_DIGITS; len++)
            {
                       if (res[len] == '\0')
                          break;
            }
                   
           str_to_big (res, num, len - 1 , 0); 
           
            if (a[end] == ')')
               end++;
                     
            while (a[end] == ' ')
              end++;
              
            return;
     }
     
     
      if (a[start] == 'm' && a[start + 1] == 'a')
     {
           int i, j, k = 0;
           char str[2];
           int num2[2];
           start += 2;
           
           while (a[start] != '(')
              start++;
           
           int size = range_length (a, start + 1);
           string my_range = a.substr (start + 1, size);
           end = start;
           
           
           //find cells in range
           read_range (my_range, str, num2);
           int height = str[1] - str[0] + 1;
           int width = num2[1] - num2[0] + 1;
           
           end += size;
           
           for (i = 0; i < 22; i++)
           {
               if (str[0] == alpha_table[i])
               {
                  k = i; 
                  break;
               }
           } 
            
           
           //find the max of cells  in range
           int y = 0;
           int x = 0; 
           for (i = 0; i < height; i++)
           {
               for (j = 0; j < width; j++)
               { 
                   int temp[MAX_DIGITS] = {0};
                   int len;
                   if (big_comparison(big_get_value(alpha_table[k + y], num2[0] + x), big_get_value(alpha_table[k + i], num2[0] + j)) == -1)
                   {
                       x = j;
                       y = i;
                   }
               }
           } 
           
           string res = big_get_value(alpha_table[k + y], num2[0] + y);       
           for (len = 0; len < MAX_DIGITS; len++)
           {
               if (res[len] == '\0')
                  break;
           }
                   
           str_to_big (res, num, len - 1 , 0); 
           
            if (a[end] == ')')
               end++;
                     
            while (a[end] == ' ')
              end++;

            return;
     }
     
     
     //the variable part
     if (a[start] >= 'A' && a[start] <= 'Z')
     {
          char col = a[start];
          start++;
          int k = a[start] - '0';
         
          
          if (a[start + 1] >= '0' && a[start + 1] <= '9')
          {
              k = k * 10 + a[start + 1] - '0';
              start ++;
          }
           
          start++;
          while (a[start] == ' ')
          start ++;
          end = start;
               
          string ans = big_get_value (col, k);
          int len = length (ans); 
          str_to_big (big_get_value (col, k), num, len, 1);
          return;
           
           if (tableBig[col - 'A'][k - 1].data[0] == NULL)
              return;  
     
          return;
     }
     
     else
     {
         int i = start;
         while (a[i] >= '0' && a[i] <= '9')
               i++;
     
         len = i - start + 1;
         string b = a.substr (start, len); 

     
         for (int j = 0; j < MAX_DIGITS; j++)
             num[j] = 0;
         
         str_to_big(b, num, len);
     
         while (a[i] == ' ')
               i++;
        
        end = i;
        return;
     }
}


//calcp for bignum
void calcp (string b, int start, int &end, int num[])
{
     int temp[MAX_DIGITS] = {0};
     calcn (b, start, end,  num);
     
     while (b[end] == '*')
     {
           calcn (b, end + 1, end, temp);
           big_multiply (num, temp); 
     }
     return;
}


//calce for big num
void calce (string exp, int start, int &end, int num[])
{
     int temp[MAX_DIGITS] = {0};
     if (exp[start] == '=')
        start ++;
     calcp (exp, start, end,  num);
     
     while (exp[end] == '+' || exp[end] == '-')
     {
           if (exp[end] == '+')
           {
              calcp (exp, end + 1, end, temp);
              big_add (num, temp);
           }
           
           if (exp[end] == '-')
           {
                calcp (exp, end + 1, end, temp);
                big_subtract (num, temp);
           } 
     }
     return;
}


//count the digits in big number
int num_digits (const int array[]){
    int i, k;
    
    for (i = MAX_DIGITS - 1; i >= 0; i--)
    {
        if (array[i] != 0)
        {
           k = i;
           break;
        }
    } 
    return k + 1;
}


// convert a big number to a string 
void big_to_string (const int big_num[], char str[]) {
    int i;
    int k = num_digits (big_num);
    
    for (i = 0; i < k; i++)
        str[i] = big_num[k - i - 1] + '0';
    
    str[k] = '\0';
}  


//parse a bignum expresion
string big_parse (string exp) 
{
    int start = 0;
    int end = 0;
    int i = 0;
    int num[MAX_DIGITS] = {0};
    
    calce (exp, start, end, num); 
     
    for (i = MAX_DIGITS - 1; i >= 0; i--)
    {
        if (num[i] != 0)
           break;   
    }
    
    if (i == -1)
        return "0";
       
    char res[MAX_DIGITS];
    big_to_string (num, res);
    
    string a = string (res);
    return a;
    
	return 0;
}


/* ------------- Window --------------- */
Fl_Input *textbar;  // top text-bar
Fl_Input *big_textbar;  //top text-bar in bignum window


// update data of a big num cell
void big_update (BigCell *cell) {
    const char *data = cell -> box-> value(); 
    string big_parse (string exp);
             
    cell-> value = big_parse (data);
    cell-> data = data;
}


// update data of a cell
void update (Cell *cell) {
    const char *data = cell -> box -> value(); 
    double parse (string exp); // defined in parse.cpp
    
    cell-> value = (data[0] == '=') ? parse(data + 1) : atof(data);
    cell-> data = data;   
}


//update value of big cell
void my_big_update (BigCell *cell)
{
     if (cell -> data[0] != NULL)
     {
          string data = cell -> data;
          cell -> value = big_parse(data);
          
          char str2[100];
          sprintf (str2, "%s", cell -> value.c_str());
          cell -> box -> value (str2);
     }
     else 
     return;
}


//update value of a cell in table without changing data
void my_update (Cell *cell)
{
     const char *data = cell -> data.c_str();
     cell-> value = (data[0] == '=') ? parse(data + 1) : atof(data);
     
     if (cell -> data[0] == NULL)
        return;
        
     char str2[100];
     sprintf (str2, "%g", cell -> value);
          
    if (cell -> data[0] != NULL){     
        if (cell -> data[0] >= '0' && cell -> data[0] <= '9'){
            int len = cell -> data.length();
            int i = 0;
            while (cell -> data[i] >= '0' && cell -> data[i] <= '9')
                i++;
            if (i < len) // data of cell is string
            {
                sprintf (str2, "%s", cell -> data.c_str());
                cell -> value = 0;
                cell -> is_string = 1;  
                cell -> box -> value (str2);                
            }
            else{
                cell -> is_string =  0;
                cell -> box -> value (str2);
            }}
        else if (cell -> data[0] == '='){
            cell -> box -> value (str2);
            cell -> is_string = 0;
            }
            
        else // data of cell is string
        {
            sprintf (str2, "%s", cell -> data.c_str());
            cell -> value = 0;
            cell -> is_string = 1;
            cell -> box -> value (str2);
        }
    }
}


// event handler in bignum window
void big_call_back(Fl_Widget* widget) {
    static char str[200];
    char a[100];
    BigCell *cell = (BigCell *)widget -> user_data(); 
    big_update (cell);
   
    
    // update big_textbar 
    int index = cell - tableBig[0];
    int row = index / NUM_COLS;
    int col = index % NUM_COLS; 
    sprintf(str, "[%c%d] %s: %s",col + 'A', row + 1, cell -> data.c_str(), cell -> value.c_str());   
    big_textbar -> value(str);  
 
    //update all cells in table
    int i, j, k;
    for (k = 0; k < NUM_ROWS * NUM_COLS / 2; k++)
    {
        for (i = 0; i < NUM_ROWS; i++)
        {
            for (j = 0; j < NUM_COLS; j++)
            {
               if (tableBig[i][j].data[0] != NULL)
                  my_big_update (&tableBig[i][j]);
            }
        }
     }
}


//updates cells recursively
void my_update_function(int row, int col, int help_table[][NUM_COLS])
{
     my_update (&table[row][col]);
     int i, j;
     
     //update the cells which are related directly
     for (i = 0; i < NUM_ROWS; i++)
     {
         for (j = 0; j < NUM_COLS; j++)
         {
             if (help_table[i][j] == 2)
                continue;
             if (table[row][col].matrix[i][j] == 1)
               my_update(&table[i][j]);
         }
     }
      
      
      //update the cells which are related indirectly
     for (i = 0; i < NUM_ROWS; i++)
     {
         for (j = 0; j < NUM_COLS; j++)
         {
             if (help_table[i][j] == 2)
                continue;
             if (table[row][col].matrix[i][j] == 1)
             {
               help_table[i][j] = 2;
               my_update_function(i, j, help_table);
               help_table[i][j] = 1;
             }
         }
     }
}



// event handler
void call_back(Fl_Widget* widget) {
    char str[200];
    char a[100];
    Cell *cell = (Cell *)widget -> user_data(); 
    update (cell);

    int i, j, k;
    int index = cell - table[0];
    int row = index / NUM_COLS;
    int col = index % NUM_COLS;
    
    
    my_update(&table[row][col]);
    int temp[NUM_ROWS][NUM_COLS];
    get_matrix (temp);
    
     //fill the matrix
     for (i = 0; i < NUM_ROWS; i++)
     {
        for (j = 0; j < NUM_COLS; j++)
        {  
             table[i][j].matrix[row][col] = temp[i][j];
        }
     }
     
    
     //update cells which are directly related to this cell
     for (i = 0; i < NUM_ROWS; i++)
     {
        for (j = 0; j < NUM_COLS; j++)
        {
            if (table[i][j].matrix[row][col] == 1)
               my_update (&table[i][j]);
        }
      }
    
    //update cells  which are indirectly related to this cell
    int helping_table[NUM_ROWS][NUM_COLS] = {0};
    helping_table[row][col] = 2;
     for (i = 0; i < NUM_ROWS; i++)
     {
        for (j = 0; j < NUM_COLS; j++)
        {
            if (table[i][j].matrix[row][col] == 1){
              helping_table[i][j] = 2;
              my_update_function (i, j, helping_table);
               }
        }
      }
     
     my_update (&table[row][col]);
     
     //find out if the cell is filled with string
      for (i = 0; i < NUM_ROWS; i++)
     {
        for (j = 0; j < NUM_COLS; j++)
        {  
             if(temp[i][j] == 1)
             {
                if (table[i][j].is_string == 1)
                {
                    table[row][col].is_string = 1;
                    table[row][col].data = table[i][j].data;
                    my_update(&table[row][col]);
                    break;
                    
                }
                else
                table[row][col].is_string = 0;
             }
        }
     }
     
     
    my_update (&table[row][col]);
    
    //update textbar
    sprintf(str, "[%c%d] %s : %g", col + 'A', row + 1, cell -> data.c_str(), cell -> value);    
    textbar -> value (str);  
}


/* -----------------------menu callbacks-------------------*/
Fl_Window *save_input_window;  //window for writing filename for save
Fl_Window *open_input_window;  //window for writing filename for open
Fl_Input *open_box;    //box for writing the filename for open
Fl_Input *savebox;     //box for writing the filename for save


//exit callback
void exit_callback (Fl_Widget * , void *){
     exit(0);
}


//new callback
void new_cb (Fl_Widget*, void *)
{
     int i, j;
     char *str = NULL;
     
     for (i = 0; i < NUM_ROWS; i++)
     {
         for (j = 0; j < NUM_COLS; j++)
         {
            table[i][j].empty = 0;
            table[i][j].value = 0;
            table[i][j].data[0] = '\0';
            table[i][j]. box -> value(str);
             textbar -> value (str);
        }
     }
}


//save call back
void save_cb (Fl_Widget *, void *)
{
     int i, j;
     
     ofstream fout;
     fout.open ("output.txt");
    
     for (i = 0; i < NUM_ROWS; i++)
     {
         for (j = 0; j < NUM_COLS; j++)
         {
             if (table[i][j].data[0] != NULL)
                table[i][j].empty = 1;
             else
                table[i][j].empty = 0;
         }
     }
     
    
     for (i = 0; i < NUM_ROWS; i++)
     {
         for (j = 0; j < NUM_COLS; j++)
         {
            fout << table[i][j].empty << " ";
            fout << table[i][j].value << " ";
         }
     }
     
     fout << "\n";
     
     for (i = 0; i < NUM_ROWS; i++)
     {
         for (j = 0; j < NUM_COLS; j++)
         {
             if (table[i][j].empty == 1)
              fout << table[i][j].data << "\n" ;
         }
     }
     
     fout.close();
}



//functions for opening  new sheet
//open file
void open_window (Fl_Widget *, void *)
{ 
     int i, j;
     string str1;
     char *str = NULL;
     
     //clear the window
     for (i = 0; i < NUM_ROWS; i++)
     {
         for (j = 0; j < NUM_COLS; j++)
         {
            table[i][j].empty = 0;
            table[i][j].value = 0;
            table[i][j].data[0] = NULL;
            table[i][j]. box -> value(str);
            textbar -> value (str);
         }
     }
     
     ifstream fin;
     if (open_box -> value() != NULL)
        fin.open (open_box -> value());
     else
     fin.open ("out.txt");

     for (i = 0; i < NUM_ROWS; i++)
     {
         for (j = 0; j < NUM_COLS; j++)
         {
            fin >> table[i][j].empty ;
            fin >> table[i][j].value ;
         }
     }
     
     getline (fin, str1);
     for (i = 0; i < NUM_ROWS; i++)
     {
         for (j = 0; j < NUM_COLS; j++)
         {
             if (table[i][j].empty == 1)
               getline (fin, table [i][j].data) ;
         }
     }
     
     for (i = 0; i < NUM_ROWS; i++)
     {
         for (j = 0; j < NUM_COLS; j++)
         {
            if (table[i][j].empty == 1)
            {
               if (table[i][j].value == 0 && table[i][j].data[0] != '=')
               {
                   if (table[i][j].data[0] != '0')
                   {
                       char temp[200];
                       int r = 0;
                       while (table[i][j].data[r] != '\0')
                       {
                             temp[r] = table[i][j].data[r];
                             r++;
                       }
                       temp[r] = '\0';
                       table[i][j].box -> value (temp);
                   }
               }
               
               else
               {
                   char str[100];
                   sprintf (str, "%g", table[i][j].value);
                   table[i][j].box -> value (str);
               }
            }
            else
            table[i][j].box -> value(NULL);      
         }
     }  
     fin.close();
     open_input_window -> ~Fl_Window();
        
}


// function for closong the open window
void close_Cb (Fl_Widget*, void *)
{
     open_input_window -> ~Fl_Window();
}


//open callback
int open_cb(Fl_Widget *, void *)
{     
     Fl_Button *ok;
     Fl_Button *cancel;
     Fl_Text_Display *label;
     
     open_input_window = new Fl_Window (300, 150, "Name for open");
     open_input_window -> color (0xe2e2e210);
     open_box = new Fl_Input (40, 35, 220, 35);
     label = new Fl_Text_Display (40, 35, 220, 35, "Write file name (ending with .txt)");
     ok = new Fl_Button (65, 90, 80, 40, "Ok");
     ok -> callback ((Fl_Callback*)open_window);
     cancel = new Fl_Button (155, 90, 80, 40, "Cancel");
     cancel -> callback (close_Cb);
     
     open_input_window -> end();
     open_input_window -> show();
     return Fl::run(); 
}



//functions for save as
void saveas_function (Fl_Widget*, void *) {
     int i, j;
     
     ofstream fout;
     fout.open(savebox -> value()); //get the name of file from input
     
     for (i = 0; i < NUM_ROWS; i++)
     {
         for (j = 0; j < NUM_COLS; j++)
         {
             if (table[i][j].data[0] != NULL)
                table[i][j].empty = 1;
             else
                table[i][j].empty = 0;
         }
     }
     
    
     for (i = 0; i < NUM_ROWS; i++)
     {
         for (j = 0; j < NUM_COLS; j++)
         {
            fout << table[i][j].empty << " ";
            fout << table[i][j].value << " ";
         }
     }
     
     for (i = 0; i < NUM_ROWS; i++)
     {
         for (j = 0; j < NUM_COLS; j++)
         {
             if (table[i][j].empty == 1)
              fout << "\n" << table[i][j].data;
         }
     }
     
     fout.close();
     save_input_window -> ~Fl_Window();
}


//close save as window
void close_cb (Fl_Widget*, void *)
{
    save_input_window -> ~Fl_Window();
}


//saveas call back
int saveas_cb (Fl_Widget *, void *)
{
     
     Fl_Button *ok;
     Fl_Button *cancel;
     Fl_Text_Display *label;
     
     save_input_window = new Fl_Window (300, 150, "Name for save");
     save_input_window -> color (0xe2e2e210);
     savebox = new Fl_Input (50, 50, 200, 35);
     ok = new Fl_Button (65, 100, 80, 40, "Ok");
     ok -> callback ((Fl_Callback*) saveas_function);
     cancel = new Fl_Button (160, 100, 80, 40, "Cancel"); 
     cancel -> callback (close_cb);
     label = new Fl_Text_Display (50, 50, 200, 35, "Write file name ending with .txt");
     
     save_input_window -> end();
     save_input_window -> show();
     return Fl::run();
}


/*------------------------------chart------------------------------*/
Fl_Input *box_y;   //input the y column
Fl_Input *box_x;    //input the x column
Fl_Input *box_x_start;  //start of x column
Fl_Input *box_x_end;    //end of x column
Fl_Input *box_y_start;   //start of y column
Fl_Input *box_y_end;     //end of y column
Fl_Choice *choose_color;  
Fl_Choice *choose_chart;
Fl_Window *chart_input_window;  //chart for geting the columns for drawing chart 


//draw chart
int draw_chart (Fl_Widget *, void*)
{
     
     Fl_Window *chart_window;
     chart_window = new Fl_Window (400, 400, "Chart");
     chart_window -> color (0xe2e2e210);
     Fl_Chart  *chart = new Fl_Chart (20, 20, chart_window -> w ()-40, chart_window -> h()-40, "chart");
          
     
     //the type of chart
     if (choose_chart -> value() == 0) 
          chart -> type (FL_BAR_CHART);
     else if (choose_chart -> value() == 1)
          chart -> type (FL_LINE_CHART);
     else if (choose_chart -> value() == 2)
          chart -> type (FL_HORBAR_CHART);
     else if (choose_chart -> value() == 3)
          chart -> type(FL_FILLED_CHART);
     else if (choose_chart -> value() == 4)
          chart -> type(FL_PIE_CHART);
     else
          chart -> type (FL_SPIKE_CHART);
     
     
     //the parameters of charts
     const char *box_xdata = box_x -> value();
     const char *box_ydata = box_x -> value();
    
     char col_x = box_xdata[0];
     char col_y = box_ydata[0];
     
     const char *box_X_start = box_x_start -> value();
     const char *box_Y_start = box_y_start -> value();
     const char *box_X_end = box_x_end -> value();
     const char *box_Y_end = box_y_end -> value();
     
     int start_X = atoi (box_X_start);
     int end_X = atoi (box_X_end);
     int start_Y = atoi (box_Y_start);
     int end_Y = atoi (box_Y_end);
     
     
     int i;
     for (i = 0; i < end_X - start_X + 1 ; i++)
     {
         double val = get_value (col_y, start_Y + i);
         static char val_str[10] = "";
         sprintf (val_str, "%.2lf", get_value (col_x, start_X + i));
         
         //the color of chart
         if (choose_color -> value() == 0)
              chart -> add (val, val_str, FL_GREEN);
         else if(choose_color -> value() == 1)
              chart -> add (val, val_str, FL_RED);
         else if (choose_color -> value() == 2)
              chart -> add (val, val_str, FL_BLUE);
         else 
              chart -> add (val, val_str, FL_WHITE);
     }
     
    chart_window -> resizable (chart_window);
    chart_window -> end();
    chart_window -> show();
    return Fl::run();
}


//close the chart window
void close_chart (Fl_Widget *, void*)
{
     chart_input_window -> ~Fl_Window();
}


//chart callback
int chart_cb (Fl_Widget *, void *)
{
     
     Fl_Button *ok_button;
     Fl_Button *close_button;
     Fl_Text_Display *labelx;
     Fl_Text_Display *labely;
     Fl_Box *range_box;
      
     chart_input_window = new Fl_Window (400, 220, "Chart inputs");
     chart_input_window -> color (0xe2e2e210);
     labelx = new Fl_Text_Display (80, 50, 50, 30, "x range: ");
     labelx -> align (FL_ALIGN_LEFT);
     labely = new Fl_Text_Display (80, 100, 50, 30 , "y range: ");
     labely -> align (FL_ALIGN_LEFT);
     
     range_box = new Fl_Box (80, 20, 50, 30, "Column");
     range_box = new Fl_Box (130, 20, 50, 30, "Start");
     range_box = new Fl_Box (180, 20, 50, 30, "End");
     
     box_x = new Fl_Input (80, 50, 50, 30);
     box_x_start = new Fl_Input (130, 50, 50, 30);
     box_x_end = new Fl_Input (180, 50, 50, 30);
     
     box_y = new Fl_Input (80, 100, 50, 30);
     box_y_start = new Fl_Input (130, 100, 50, 30);
     box_y_end = new Fl_Input (180, 100, 50, 30);
     
     
     choose_color = new Fl_Choice (280, 100, 70, 30, "Color");
     choose_color -> add ("green", 0, 0);
     choose_color -> add ("red", 0, 0);
     choose_color -> add ("blue", 0, 0);
     choose_color -> add ("white", 0, 0);
     choose_color -> value(0);
     
     
     choose_chart = new Fl_Choice (280, 50, 70, 30, "Type");
     choose_chart -> add ("Bar", 0, 0);
     choose_chart -> add ("Line", 0, 0);
     choose_chart -> add ("Horbar", 0, 0);
     choose_chart -> add ("Fill", 0, 0);
     choose_chart -> add ("Pie", 0, 0);
     choose_chart -> add ("Spike", 0, 0);
     choose_chart -> value(0);
     
     ok_button = new Fl_Button (110, 160, 80, 40, "OK");
     close_button = new Fl_Button (210, 160, 80, 40, "Close");
     close_button -> callback (close_chart);
     ok_button -> callback ((Fl_Callback*)draw_chart);
     
     chart_input_window -> end();
     chart_input_window -> show();
     return Fl::run();
}


/*------------------big num window----------------------*/
int big_win (Fl_Widget*, void *)
{
     int i, j, k;
     
     Fl_Window *big_num_window = new Fl_Window (800, 600, "Bignum window");
     big_num_window -> color(0xe2e2e210);
     Fl_Input *big_box;
     Fl_Box *big_label;
     Fl_Scroll *scroll;
     
     //scrollbar
     scroll = new Fl_Scroll (5, 60, 795, 540);
     scroll -> color(0xe2e2e210);
     
     
     //Input boxes
     for (i = 0; i < NUM_ROWS; i++)
     {
         for (j = 0; j < NUM_COLS; j++)
         {
             big_box = new Fl_Input (j * 250 + 50, i * 25 + 60, 250, 25);
             big_box -> callback (big_call_back);
             big_box -> when (FL_WHEN_RELEASE);
             big_box -> user_data (&tableBig[i][j]);
             tableBig[i][j].box = big_box;
         }
     }
     
     //labels
     for (k = 0; k < 50; k++)
         big_label = new Fl_Box (5, k * 25 + 60, 40, 25, strV[k]);
        
     for (k = 0; k < 26; k++)
        big_label = new Fl_Box (k * 250 + 50, 30, 250, 25, strH[k]);
      
     scroll -> end();    
    
     //textbar
     big_textbar = new Fl_Input (50, 15, 500, 26);
	 big_textbar -> box (FL_BORDER_BOX);
     big_textbar -> color (0xf2f2f210);
     
     big_num_window -> end();
     big_num_window -> show();
     return Fl::run();
}


/*-----------------------------sort------------------------*/
Fl_Input *col;  // box for geting the column for sort
Fl_Input *start;  // box for geting the start of the column
Fl_Input *end;    //boz for getting the end of the column
Fl_Window *sort_window;   //window for getting the column for sort
 
//swap 2 cells
void swap(char col, int n, int k)
{
     string temp_data = table[n][col - 'A'].data;
     double temp_value = table[n][col - 'A'].value;
     table[n][col - 'A'].data = table[k][col - 'A'].data;
     table[n][col - 'A'].value = table[k][col - 'A'].value;
     table[k][col - 'A'].data = temp_data;
     table[k][col - 'A'].value = temp_value;
}

//sort callbacks
void sortCb(Fl_Widget *, void*)
{
    int i, j;
    string a = col -> value();
    string init = start -> value();
    string k = end -> value();
    int end = k[0] - '0' - 1;
    int start = init[0] - '0' - 1;
    for (i = 0; i < end - start + 1; i++)
    {
        for (j = end - start; j > i; j--)
        {
            if (table[j + start][a[0] - 'A'].value < table[j + start - 1][a[0] - 'A'].value)
               swap(a[0], j + start , j + start - 1);
        }
    }
    
    for (i = 0; i < end - start + 1; i++)
        my_update (&table[i + start][a[0] - 'A']);
        
    sort_window -> ~Fl_Window();
}


//exit sort window
void sort_exit (Fl_Widget*, void*)
{
     sort_window -> ~Fl_Window();
}


//sort window
int sort_cb (Fl_Widget*, void *)
{
     Fl_Text_Display *label_start;
     Fl_Text_Display *label_end;
     Fl_Text_Display *label_column;
     
     sort_window = new Fl_Window (265, 150, "Sort Input");
     sort_window -> color (0xe2e2e210);
     
     col = new Fl_Input (60, 30, 50, 30);
     start = new Fl_Input (110, 30, 50, 30);
     end = new Fl_Input (160, 30, 50, 30);
     label_column = new Fl_Text_Display (60, 30, 50, 30, "Column");
     label_start = new Fl_Text_Display (110, 30, 50, 30, "Start");
     label_end = new Fl_Text_Display (160, 30, 50, 30, "End");
     
     Fl_Button *ok = new Fl_Button (50, 80, 80, 40, "Ok");
     ok -> callback (sortCb); 
     Fl_Button *cancel = new Fl_Button (140, 80, 80, 40, "Cancel");
     cancel -> callback (sort_exit);
     
     sort_window -> end();
     sort_window -> show();
     return Fl:: run();
}

// main program
int main() {
    Fl_Window *window;
    Fl_Input *box;
    Fl_Box *label;
    Fl_Scroll *scroll;
	Fl_Menu_Bar *menu;
    Fl_Button *sort;
    int k;
   	
    
    window = new Fl_Window(800, 600, "My Spread Sheet");
	window -> color(0xe2e2e210);
    
 
   // menu
	menu = new Fl_Menu_Bar(0, 0, 100, 25);
	menu -> add ("File", 0, 0, 0, FL_SUBMENU);
	menu -> add ("Tools", 0, 0, 0, FL_SUBMENU);
	menu -> add ("Tools/Chart", 0,(Fl_Callback *)chart_cb);
	menu -> add ("Tools/Bignum", 0, (Fl_Callback*)big_win);
	menu -> add ("Tools/Sort", 0,(Fl_Callback*)sort_cb); 
	menu -> add ("File/New", FL_CTRL + 'n', (Fl_Callback *)new_cb);
    menu -> add ("File/Open", FL_CTRL + 'o', (Fl_Callback*)open_cb);
	menu -> add ("File/Save", FL_CTRL + 's',(Fl_Callback*)save_cb);
	menu -> add ("File/Save as", 0, (Fl_Callback*)saveas_cb);
    menu -> add ("File/Quit", FL_CTRL + 'q', exit_callback, 0, 0); 
    menu -> color (0xa3d1ee10);
    
    //scrollbar   
    scroll = new Fl_Scroll (0, 65, 800, 535); 
    scroll -> color(0xe2e2e210);
    
    //Input boxes
    for (int i = 0; i < NUM_ROWS; i++)
    {
		    for (int j = 0; j < NUM_COLS; j++) 
            {
			    box = new Fl_Input(j * 60 + 30, i * 25 + 100, 60, 25);
			    box->callback(call_back);
			    box->when(FL_WHEN_RELEASE);
			    box->user_data(&table[i][j]);
                table[i][j].box = box;
                 
		        }
      }

   
    //Horizontal  labels
    for (k = 0; k < 26; k++)
        label = new Fl_Box (k * 60 + 30, 80, 60, 20, strH[k]);
        
        
     //Vertical labels
    for (k = 0; k < 50; k++)
        label = new Fl_Box (0, k * 25 + 100, 30, 25, strV[k]);

              
  	scroll -> end();
   
    //textbar
    textbar = new Fl_Input(30, 35, 400, 26);
	textbar -> box (FL_BORDER_BOX);
    textbar -> when (FL_WHEN_CHANGED);
	textbar->color(0xf2f2f210);
    
    
	window -> end();
	window -> show();
	return Fl::run();
}
