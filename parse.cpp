// Project 3: Parse Expressions
// CE 40-153
// By: Pegah Jandaghi(90105978)


#include <cstdlib>
#include <string>
#include <iostream>
#include <cmath>
#include <cstring>
#define NUM_ROWS 50
#define NUM_COLS 26

using namespace std;

static int matrix_table[NUM_ROWS][NUM_COLS];
char alphabet_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


// lookup functions, defined in spread.cpp
double get_value (char col, int row);
double get_value (string name); 
double calcE (string a, int &end, int i);


//find the size of string range
int range_length (string a, int start)
{
    int len = start;
     while(a[len] != ')')
       len++;
       
     len -= start;
     len++;
     return len;
}


//check if the char is a capital letter
int is_letter (char a)
{
    if (a >= 'A' && a <= 'Z')
       return 1;
    
    return 0;
}


//find the first and last cell in range
void read_range (string range, char str[], int num[])
{
     int i = 0;
     for (i = 0; i < 2; i++)
        num[i] = 0;
     
     i = 0;
     while (range[i] == ' ')
        i++;
        
     if (is_letter(range[i]))
     {
         str[0] = range[i];
         i++;
     }
     
     num[0] += range[i] - '0';
        i++;
     
     if (range[i] >= '0' && range[i] <= '9')
     {
         num[0] *= 10;
         num[0] += range[i] - '0';
         i++;
     }
     
     while (range[i] == ' ')
        i++;
     
     if (range[i] == ':')
        i++;
     
     while (range[i] == ' ')
       i++;
       
     str[1] = range[i];
     i++;
     num[1] += range[i] - '0';
       i++;
     
     if (range[i] >= '0' && range[i] <= '9')
     {
        num[1] *= 10;
        num[1] += range[i] - '0';
     }
     
     return;
}


//--------------- Range Functions ---------------
// return minimum element in a range
double range_min (string range) {
    int i, j, k = 0;
    char str[2];
    int num[2] = {0};
    
    read_range (range, str, num); 
    int height = str[1] - str[0] + 1;
    int width = num[1] - num[0] + 1;
    
    for (i = 0; i < 22; i++)
    {
        if (str[0] == alphabet_table[i])
        {
          k = i;
          break;
        }       
    }
    
    double res = get_value (alphabet_table[k], num[0]);
    
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (get_value (alphabet_table[k + i], num[0] + j) < res)
               res = get_value (alphabet_table[k + i], num[0] + j);
        }
    }
    return res;
}


// return maximum element in a range
double range_max (string range) {
    int i, j, k = 0;
    char str[2];
    int num[2] = {0};
    
    read_range(range, str, num);
    int height = str[1] - str[0] + 1;
    int width = num[1] - num[0] + 1;
    
    for (i = 0; i < 22; i++)
    {
        if (str[0] == alphabet_table[i])
        {
          k = i;
          break;
        }       
    }
    
    double res = get_value (alphabet_table[k], num[0]);
    
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (get_value (alphabet_table[k + i], num[0] + j) > res)
               res = get_value (alphabet_table[k + i], num[0] + j);
        }
    }
    return res;
}


// return sum of the elements in a range
double range_sum (string range) {
    int i, j, k = 0;
    double res = 0;
    char str[2];
    int num[2] = {0};
    
    read_range (range, str, num);
    int height = str[1] - str[0] + 1;
    int width = num[1] - num[0] + 1;
    
    for (i = 0; i < 22; i++)
    {
        if (str[0] == alphabet_table[i])
        {
            k = i; 
            break;
        }
    }
    
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)      
               res += get_value (alphabet_table[k + i], num[0] + j);
    }
    
    return res;
}


// return average of the elements in a range
double range_average (string range) {
   int i, j, k = 0;
   double res = 0;
   char str[2];
   int num[2] = {0};
    
    read_range(range, str, num);
    int height = str[1] - str[0] + 1;
    int width = num[1] - num[0] + 1;
        
    for (i = 0; i < 22; i++)
    {
        if (str[0] == alphabet_table[i])
        {
           k = i; 
           break;
        }
    }
    
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
               res += get_value (alphabet_table[k + i], num[0] + j);
    }
    
    res /= height * width;
    return res;
}


//--------------- Parse Expressions ---------------
//Check if the char is letter
int letter (string a, int &end , int i)
{
    if (a[i] >= 'a' && a[i] <= 'z')
      return 1;
      
    if (a[i] >= 'A' && a[i] <= 'Z')
      return 1;
    
    return 0;
}


//Calculate N
double calcN (string a, int &end, int i)
{         
       double res = 0;
       int flag = 0;
       
       while (a[i] == ' ')
          i++;
          
       // minus unary
      while (a[i] == '-')
      {
         i++;
         flag ++;
         end++;
      }
  
       if (a[i] == '(')
       {
           res += calcE (a, end, i + 1);
           if (a[end] == ')')
              end ++;
         
           while (a[end] == ' ')
              end++;
              
         return res;   
       }
       
    if (letter(a, end, i))
    {   
        //trigonometry functions
        //sin(exp) 
       if (a[i] == 's' && a[i + 1] == 'i')
       {
             int temp = i + 3;
             if (a[temp - 1] == 'n')
             {
                  while (a[temp] == ' ')
                        temp++;
            
                  if (a[temp] == '(')
                  {
                       res = sin (calcE (a, end, temp + 1));
                       
                       if (a[end] == ')')
                          end++;
                       
                       while (a[end] == ' ')
                          end++;
                       
                       return res;
                  }
             }
        }
   
        //cos(exp)
       if (a[i] == 'c' && a[i + 1] == 'o')
       {
          int temp = i + 3;
          if (a[temp - 1] == 's')
          {
              while (a[temp] == ' ')
                  temp++;
            
              if (a[temp] == '(')
              {
                  res = cos (calcE (a, end, temp + 1));
                  
                  if (a[end] == ')')
                     end++;
                  
                  while (a[end] == ' ')
                     end++;
                  
                  return res;
              }
           }
        }
   
        //tan(exp)
       if (a[i] == 't' && a[i + 1] == 'a')
       {
           int temp = i + 3;
            if (a[temp - 1] == 'n')
            {
                 while (a[temp] == ' ')
                    temp++;
            
                 if (a[temp] == '(')
                 {
                     res = tan (calcE (a, end, temp + 1));
                     
                     if (a[end] == ')')
                         end++;
                     
                     while (a[end] == ' ')
                         end++;
                     
                     return res;
                  }
            }
       }
   
       //arctan(exp) 
       if (a[i] == 'a' && a[i + 1] == 'r')
       {
            int temp = i + 6;
            if (a[temp - 4] == 'c' && a[temp - 3] == 't')
            {
                if (a[temp - 2] == 'a' && a[temp - 1] == 'n')
                {
                   while (a[temp] == ' ')
                      temp++;
            
                      if (a[temp] == '(')
                      {
                         res = atan (calcE (a, end, temp + 1));
                     
                         if (a[end] == ')')
                            end++;
                         
                         while (a[end] == ' ')
                            end++;
                         
                         return res;
                      }
                }
            }
       }
       
       
       // minimum in a range
       if (a[i] == 'm' && a[i + 1] == 'i')
       {
           int temp = i + 3;
            if (a[temp - 1] == 'n')
            {
                 while (a[temp] == ' ')
                    temp++;
            
                 if (a[temp] == '(')
                 {
                     char str[2];
                     int num[2];
                     end = temp;
                     int size = range_length (a, temp + 1);
                     string my_range = a.substr (temp + 1, size);
                     end += size;
                     res += range_min (my_range);
                     
                     if (a[end] == ')')
                         end++;
                     
                     while (a[end] == ' ')
                         end++;
                     
                     char str2[2];
                     int num2[2] = {0};
                     read_range(my_range, str2, num2);
                     int height = str2[1] - str2[0] ;
                     int width = num2[1] - num2[0] ;
                     
                     for (int m = 0; m < height; i++)
                     {
                         for (int h = 0; h < width; h++)
                             matrix_table[num2[0] + m - 1][str2[0] - 'A' + h] = 1;
                     } 
                     return res;
                  }
            }
       }
       
       
       //max in range
       if (a[i] == 'm' && a[i + 1] == 'a')
       {
           int temp = i + 3;
            if (a[temp - 1] == 'x')
            {
                 while (a[temp] == ' ')
                    temp++;
            
                 if (a[temp] == '(')
                 {
                     char str[2];
                     int num[2];
                     end = temp;
                     int size = range_length (a, temp + 1);
                     string my_range = a.substr (temp + 1, size);
                     end += size;
                     res += range_max (my_range);
                     
                     if (a[end] == ')')
                         end++;
                     
                     while (a[end] == ' ')
                         end++;
                     
                     char str2[2];
                     int num2[2] = {0};
                     read_range(my_range, str2, num2);
                     int height = str2[1] - str2[0] ;
                     int width = num2[1] - num2[0] ;
                     
                     for (int m = 0; m < height; i++)
                     {
                         for (int h = 0; h < width; h++)
                             matrix_table[num2[0] + m - 1][str2[0] - 'A' + h] = 1;
                     } 
                     
                     return res;
                  }
            }
       }
       
       //sum of range
        if (a[i] == 's' && a[i + 1] == 'u')
       {
           int temp = i + 3;
            if (a[temp - 1] == 'm')
            {
                 while (a[temp] == ' ')
                    temp++;
            
                 if (a[temp] == '(')
                 {
                     char str[2];
                     int num[2];
                     end = temp;
                     int size = range_length (a, temp + 1);
                     string my_range = a.substr (temp + 1, size);
                     end += size;
                     res += range_sum (my_range);
                     
                     if (a[end] == ')')
                         end++;
                     
                     while (a[end] == ' ')
                         end++;
                     
                     char str2[2];
                     int num2[2] = {0};
                     read_range(my_range, str2, num2);
                     int height = str2[1] - str2[0] ;
                     int width = num2[1] - num2[0] ;
                     for (int m = 0; m < height; i++)
                     {
                         for (int h = 0; h < width; h++)
                             matrix_table[num2[0] + m - 1][str2[0] - 'A' + h] = 1;
                         
                     }      
                     return res;
                  }
            }
       }
       
       //avrerage of range
       if (a[i] == 'a' && a[i + 1] == 'v')
       {
           int temp = i + 6;
            if (a[temp - 4] == 'e' && a[temp - 3] == 'r')
            {
                if (a[temp - 2] == 'a' && a[temp - 1] == 'g')
                {
                   if (a[temp] == 'e')
                   {
                      temp ++;
                
                      while (a[temp] == ' ')
                            temp++;
            
                      if (a[temp] == '(')
                      {
                         char str[2];
                         int num[2];
                         end = temp;
                         int size = range_length (a, temp + 1);
                         string my_range = a.substr (temp + 1, size);
                         end += size;
                         res += range_average (my_range);
                     
                         if (a[end] == ')')
                            end++;
                     
                         while (a[end] == ' ')
                         end++;
                         
                         char str2[2];
                         int num2[2] = {0};
                         read_range(my_range, str2, num2);
                         int height = str2[1] - str2[0] ;
                         int width = num2[1] - num2[0] ;
                         
                         for (int m = 0; m < height; i++)
                         {
                             for (int h = 0; h < width; h++)
                              matrix_table[num2[0] + m - 1][str2[0] - 'A' + h] = 1;
                         }
                      
                     
                     return res;
                     }
                    }
               }
            }
       }
   
       //variables
       if (letter(a, end, i))
       {        
           int r = i;
           i++;
           int temp = a[i] - '0';
           i++;
           if (a[i] >= '0' && a[i] <= '9')
          {
             temp *= 10;
             temp += a[i] - '0';
             i++;
          }
          string add;
          if (temp > 9)
             add = a.substr(r, 3);
          else
             add = a.substr(r, 2);
          matrix_table[temp - 1][a[r] - 'A'] = 1;
          res += get_value (a[r], temp);
       }
   }
   
       
   //convert string to double
   else 
   {
        if (a[i] >= '0' && a[i] <= '9')
        {
             while (a[i] >= '0' && a[i] <= '9')
             {
                  res *= 10;
                  res += a[i] - '0';
                  i++;
                  if (a[i] > '9' || a[i] < '0')
                  {
                     if (a[i] == '.')
                     {
                        double k = 0.1;
                        i++;

                        while (a[i] >= '0' && a[i] <= '9')
                        {
                               res += (a[i] - '0') * k;
                               k /= 10;
                               i++;
                        }
                       }
                  }  
               }
         }
     }  
      
     if (flag % 2 == 1)
        res *= -1.0;
     
     while (a[i] == ' ')
         i++;
          
       end = i;
       return res;    
}


//Cacculate P
double calcP (string a, int &end, int i)
{         
       double res = 0;
       res += calcN (a, end, i);

       while (a[end] == '*' || a[end] == '/')
       {
             if (a[end] == '*' )
               res *= calcN (a, end, end + 1);
               
             else 
             {
                if(a[end] == '/')
                res /= calcN (a, end, end +1);
             }
       }
       
       return res;
}


//Calculate E
double calcE (string a, int &end, int i)
{          
       double res = 0;
       res += calcP (a, end, i);
   
       while (a[end] == '+' || a[end] == '-')
       {
             if (a[end] == '-')
                  res -= calcP (a, end, end + 1);
             
             if (a[end] == '+')
                res += calcP (a, end, end + 1);
       }
       
       return res;
}


//get matrix 
void get_matrix (int num[][NUM_COLS])
{
  
             
     for (int i = 0; i < NUM_ROWS; i++)
     {
         for (int j = 0; j < NUM_COLS; j++)
         {
             num[i][j] = matrix_table[i][j];
         }
     }
     return;
}


// parse an expression
double parse(string exp) 
{
       
    double res = 0;
    int i = 0, k, j;
    int end = 0;
    
    for (j = 0; j < NUM_ROWS; j++)
    {
        for (k = 0; k < NUM_COLS; k++)
            matrix_table[NUM_ROWS][NUM_COLS] = 0;
        
    }
    res += calcE (exp, end, i);
    
    return res;
	return 0;
}



