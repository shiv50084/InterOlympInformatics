/**
* @file     ioi1prob1.c
* @author   Stavros Vagionitis
* @date     xx Mar 2015
* @brief    [Problem 1 in 1st International Olympiad in Informatics](http://www.ioinformatics.org/locations/ioi89/tasks89.txt)
*           Problem 1
*           =========
*           Given 2*N  boxes in  line  side  by  side  (N<=5).  Two
*           adjacent boxes are empty, and the other boxes contain N-1
*           symbols "A" and N-1 symbols "B".
*
*           Example for N=5:
*
*           | A | B | B | A |   |   | A | B | A | B |
*
*           Exchanging rule:
*
*           The content  of any two adjacent non-empty boxes can
*           be moved  into the  two empty ones, preserving their
*           order.
*
*           Aim:
*
*           Obtain a  configuration where  all A's are placed to
*           the left of all B's, no matter where the empty boxes
*           are.
*
*           Problem:
*
*           Write a program that:
*
*           1. Models  the exchanging of boxes, where the number
*           of boxes  and the  initial state  are to be input
*           from the  keyboard. Each exchange is input by the
*           number (from  1 to  N-1) of  the first of the two
*           neighboring boxes  which are to be exchanged with
*           the empty  ones. The  program must find the state
*           of the boxes after the exchange and display it.
*
*           2.  Given  an  initial  state  finds  at  least  one
*           exchanging plan,  which reaches the aim (if there
*           is such  a plan).  A plan  includes  the  initial
*           state and the intermediate states for each step.
*
*           3. Finds  the minimal  exchanging plan which reaches
*           the aim.
*
*
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/**
 * \brief Swap two adjecent elements which are defined
 *        by the position of the first, with other two
 *        adjecent elements which again are defined by
 *        the position of first. Next following an example:
 *
 *                1  4
 *                -- --
 *        Array: ABBAEEABAB
 *
 *        If we want to swap the ones with the lines above
 *        then we can call this function as
 *
 *        swap_two_adjacent(Array, 1, 4)
 *
 *        The result will be
 *
 *        Array: AEEABBABAB
 *
 * \param   arr     The array to swap the elements
 * \param   i       The position of the first of the
 *                  first two adjecent elements to be
 *                  swapped.
 * \param   j       The position of the first of the
 *                  second two adjecent elements to be
 *                  swapped.
 */
void swap_two_adjacent(char arr[], uint64_t i, uint64_t j)
{
    arr[i] ^= arr[j];
    arr[j] ^= arr[i];
    arr[i] ^= arr[j];

    arr[i+1] ^= arr[j+1];
    arr[j+1] ^= arr[i+1];
    arr[i+1] ^= arr[j+1];
}

/**
 * \brief Check that the contents of the boxes are according
 *        to the requirements.
 *        * N-1 boxes contain A
 *        * N-1 boxes contain B
 *        * 2 adjecent boxes are empty.
 *
 * \param   arr     The array to check.
 * \parma   sz      The size of the array.
 * \param   c       The character to check, allowed
 *                  A, B and E for empty.
 * \return 1 if everything is ok, 0 otherwise.
 */
uint8_t check_box(const char arr[], uint64_t sz, char c)
{
    uint64_t N = sz/2;
    uint64_t count = 0;
    uint8_t isAdjecent = 0;

    for(uint64_t i = 0;i<sz;i++)
    {
        if (arr[i] == c)
        {
            count++;
            // Check if there are 2 adjecent boxes
            if (c == 'E' && arr[i+1] == 'E')
                isAdjecent = 1;
        }
    }

    if (c == 'A' && count == (N - 1))
        return 1;
    else if (c == 'B' && count == (N - 1))
        return 1;
    else if (c == 'E' && count == 2 && isAdjecent)
        return 1;
    else
        return 0;
}

/**
 * \brief Get the first occurence of a specific character.
 *
 * \param   arr     Array to look for the character.
 * \param   sz      The size of the array.
 * \param   c       The character to find it's position
 * \return the position of the character specified.
 */
uint64_t get_char_pos(const char arr[], uint64_t sz, char c)
{
    for(uint64_t i = 0;i<sz;i++)
        if (arr[i] == c)
            return i;
}

/**
 * \brief Print the array in one line
 *
 * \param   arr     Array to print.
 * \param   sz      The size of the array.
 */
void print_array(const char arr[], uint64_t sz)
{
    for(uint64_t i = 0;i<sz;i++)
        fprintf(stdout, "%c", arr[i]);
    fprintf(stdout, "\n");
}

/**
 * \brief Check if the aim is met. In our case the aim is
 *        to obtain a configuration where all A's are placed to
 *        the left of all B's, no matter where the empty boxes are.
 *        An example will be the following
 *
 *        If the array is ABBAEEABAB some possible solutions are
 *
 *        AAAABBBBEE
 *        AAAAEEBBBB
 *        AAAABBEEBB
 *        AAEEAABBBB
 *
 * \param   arr     Array to print.
 * \param   sz      The size of the array.
 */
uint8_t check_aim(const char arr[], uint64_t sz)
{
    uint64_t pos_A = -1, pos_B = -1;
    uint64_t count_A = 0, count_B = 0;

    for(uint64_t i = 0;i<sz;i++)
    {
        if (arr[i] == 'A')
        {
            pos_A = i;
            count_A++;
        }

        if (arr[i] == 'B')
        {
            pos_B = i;
            count_B++;
        }

        // Check that the positions values are not the ones
        // that initialized and also check the position of the
        // latest B with the position of the latest A.
        if (pos_B != -1 && pos_A != -1 &&
            pos_B < pos_A)
            return 0;

        // If all the A and B were counted, the remaining
        // are empty, so break the loop.
        if (count_A == 4 && count_B == 4)
            break;
    }

    return 1;
}

uint8_t algo(char arr[], uint64_t sz)
{
    uint64_t count = 0;
    uint64_t N = sz/2;

    while(1)
    {
        // Get the position of empty boxes
        uint64_t pos_E = get_char_pos(arr, sz, 'E');
        uint64_t pos_E_end = pos_E + 1;

        for (uint64_t i = 0;i<sz-1;i++)
        {
            // The empty boxes are at the left half of the array
            // The current index is at the left half of the array
            if (pos_E_end < N && i < N)
            {
                // If there is an AA after empty boxes on the left
                // half of the array then swap. An example
                // AEEAA... -> AAAEE...
                if (arr[i] == 'A' && arr[i+1] == 'A' && i > pos_E)
                {
                    swap_two_adjacent(arr, i, pos_E);
                    break;
                }
#if 1
                // ABAEE... -> EEAAB...
                else if (arr[i] == 'A' && arr[i+1] == 'B' && i < pos_E)
                {
                    swap_two_adjacent(arr, i, pos_E);
                    break;
                }
#endif
            }
            // The empty boxes are at the left half of the array
            // The current index is at the right half of the array
            // *General rule*: we want everything with A
            // from right to go left.
            else if (pos_E_end < N && i >= N)
            {
                // If there is an AA at the right half of the array
                // and the empty boxes are at the left then swap. An
                // example
                // AEEBABAA... -> AAABABEE...
                if (arr[i] == 'A' && arr[i+1] == 'A')
                {
                    swap_two_adjacent(arr, i, pos_E);
                    break;
                }
                // The same as above with AB. An example
                // AEEBABAB... -> AABBABEE...
                else if (arr[i] == 'A' && arr[i+1] == 'B')
                {
                    swap_two_adjacent(arr, i, pos_E);
                    break;
                }
            }
            // The empty boxes are at the right half of the array
            // The current index is at the left half of the array
            // *General rule*:  we want everything with B
            // from left to go right.
            else if (pos_E_end >= N && i < N)
            {
                // If there is a BB at the left half of the array
                // and the empty boxes are the right then swap. An
                // example
                // ABBABAEE... -> AEEABABB...
                if (arr[i] == 'B' && arr[i+1] == 'B')
                {
                    swap_two_adjacent(arr, i, pos_E);
                    break;
                }
                // The same applies for BA. An example
                // ABAABAEE... -> AEEABABA...
                else if (arr[i] == 'B' && arr[i+1] == 'A')
                {
                    swap_two_adjacent(arr, i, pos_E);
                    break;
                }
            }
            // The empty boxes are at the right half of the array
            // The current index is at the right half of the array
            else if (pos_E_end >= N && i >= N)
            {
                // ...BBEEB -> ...EEBBB
                if (arr[i] == 'B' && arr[i+1] == 'B' && i < pos_E)
                {
                    swap_two_adjacent(arr, i, pos_E);
                    break;
                }
                // ...EEBAB -> ...ABBEE
                else if (arr[i] == 'A' && arr[i+1] == 'B' && i > pos_E)
                {
                    swap_two_adjacent(arr, i, pos_E);
                    break;
                }
            }
        }
        print_array(arr, sz);

        // If the aim is met, then break
        if (check_aim(arr, sz))
            break;

        count++;
    }
}

/**
 * \brief Usage of this program.
 *
 * \param   argv0   The first argument string which is the current executable.
 * \return  -1.
 */
int usage(char *argv0)
{
    fprintf(stdout, "%s <2N boxes where N <= 5>\n", argv0);
    fprintf(stdout, "   N-1 boxes contain A\n");
    fprintf(stdout, "   N-1 boxes contain B\n");
    fprintf(stdout, "   2 adjecent boxes are empty\n");
    return -1;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        return usage(argv[0]);

    char *boxes = argv[1];
    uint64_t boxes_sz = strlen(boxes);
    uint64_t N = boxes_sz/2;

    if (N > 5)
        return usage(argv[0]);

    // TODO Just use only one function to check
    if (!check_box(boxes, boxes_sz, 'A') ||
        !check_box(boxes, boxes_sz, 'B') ||
        !check_box(boxes, boxes_sz, 'E'))
        return usage(argv[0]);

    algo(boxes, boxes_sz);

    return 1;
}
