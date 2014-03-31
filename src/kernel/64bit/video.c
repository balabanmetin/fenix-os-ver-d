#include "globals.h"

struct screen_position
{
 unsigned char character; /*!< The character part of the byte tuple used for
                               each screen position. */
 unsigned char attribute; /*!< The character part of the byte tuple used for
                               each screen position. */
};
/*!< Defines a VGA text mode screen position. */

struct screen
{
 struct screen_position positions[MAX_ROWS][MAX_COLS];
};
/*!< Defines a VGA text mode screen. */

struct screen* const
screen_pointer = (struct screen*) 0xB8000;

static inline void
scroll(void)
{
 register int i,j;

 for(i=0; i<(MAX_ROWS-1); i++)
  for(j=0; j<MAX_COLS; j++)
   screen_pointer->positions[i][j]=
    screen_pointer->positions[i+1][j];

 for(j=0; j<MAX_COLS; j++)
  screen_pointer->positions[MAX_ROWS-1][j].character=' ';
} 

static inline void
kprintchar(const char character)
{
 static int curr_row = 5;
 static int curr_col = 0;

 if ('\n'==character)
 {
  curr_col=0;
  curr_row++;
  return;
 }

 if (curr_col >= MAX_COLS)
 {
  return;
 }

 while (curr_row >= MAX_ROWS)
 {
  scroll();
  curr_row--;
 }

 {
  screen_pointer->positions[curr_row][curr_col].character=character;
  curr_col++;
 }
}

void
kprints(const char* string)
{
 /* Loop until we have found the null character. */
 while(1)
 {
  register const char curr = *string++;

  if (curr)
  {
   kprintchar(curr);
  }
  else
  {
   return;
  }
 }
}

void
kprinthex(const register uint64_t value)
{
 const static char hex_helper[16]="0123456789abcdef";
 register int      i;

 /* Print each character of the hexadecimal number. This is a very inefficient
    way of printing hexadecimal numbers. It is, however, very compact in terms
    of the number of source code lines. */
 for(i=15; i>=0; i--)
 {
  kprintchar(hex_helper[(value>>(i*4))&15]);
 }
}
