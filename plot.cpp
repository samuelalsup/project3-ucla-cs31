//
//  plot.cpp
//  plot
//  Created by Sam Alsup on 10/21/19.
//  Copyright © 2019 Sam Alsup. All rights reserved.
//  Name : Samuel Alsup
//  UID : 805371633
//

#include "grid.h"
#include <iostream>
#include <cctype>
#include <string>

using namespace std;

const int HORIZ = 0;
const int VERT = 1;

const int FG = 0;
const int BG = 1;

const int LEFT = 0;
const int RIGHT = 1;
const int DOWN = 2;
const int UP = 3;

const int HMAX = 30; // horizontal maximum of the grid
const int HMIN = 0; // horizontal minimum of the grid
const int VMAX = 20; // vertical maximum of the grid
const int VMIN = 0; // vertical minimum of the grid

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg);

bool plotHorizontalLine(int r, int c, int distance, char ch, int fgbg);

bool plotVerticalLine(int r, int c, int distance, char ch, int fgbg);

int performCommands(string commandString, char& plotChar, int& mode, int& badPos);

int main()
{
    setSize(20, 30);
    char currentChar = '*';
    int currentMode = FG;
    for (;;)
    {
        currentChar = '*';
        currentMode = FG;
        cout << "Enter a command string: ";
        string cmd;
        getline(cin, cmd);
        if (cmd == "")
            break;
        int position;
        int status = performCommands(cmd, currentChar, currentMode, position);
        switch (status)
        {
            case 0:
                draw();
                break;
            case 1:
                cout << "Syntax error at position " << position+1 << endl;
                break;
            case 2:
                cout << "Cannot perform command at position " << position+1 << endl;
                break;
            default:
                // It should be impossible to get here.
                cerr << "performCommands returned " << status << "!" << endl;
        }
    }
}

bool plotHorizontalLine(int r, int c, int distance, char ch, int fgbg) // tries to plot a horizontal line; returns true if it can, false if it can't
{
    if ((c + distance > HMAX) || (c + distance < HMIN)) // checks to see if it can plot the line
        return false;
    if (fgbg == FG) // for foreground plotting
    {
        if (distance >= 0) // for plotting positive distances
            for (int k = 0; k <= distance; k++)
                setChar(r, c + k, ch);
        else // for plotting negative distances
            for (int k = 0; k >= distance; k--)
                setChar(r, c + k, ch);
    }
    else if (fgbg == BG) // for background plotting
    {
        if (distance >= 0) // for plotting positive distances
        {
            for(int i = 0; i <= distance; i++)
            {
                if (getChar(r, c + i) == ' ')
                    setChar(r, c + i, ch);
            }
        }
        else // for plotting negative distances
        {
            for(int i = 0; i >= distance; i--)
            {
                if (getChar(r, c + i) == ' ')
                    setChar(r, c + i, ch);
            }
        }
        
    }
    else
        return false;
    return true;
}

bool plotVerticalLine(int r, int c, int distance, char ch, int fgbg) // tries to plot a vertical line; returns true if it can; false if it can't
{
    if ((r + distance > VMAX) || (r + distance < VMIN)) // checks to see if it can plot the line
    {
        return false;
    }
    if (fgbg == FG) // for foreground plotting
    {
        if (distance >= 0) // for plotting positive distances
        {
            for (int k = 0; k <= distance; k++)
                setChar(r + k, c, ch);
        }
        else // for plotting negative distances
            for (int k = 0; k >= distance; k--)
                setChar(r + k, c, ch);
    }
    else if (fgbg == BG) // for background plotting
    {
        if (distance >= 0) // for plotting positive distances
        {
            for (int i = 0; i <= distance; i++)
            {
                if (getChar(r + i, c) == ' ')
                    setChar(r + i, c, ch);
            }
        }
        else // for plotting negative distances
        {
            for (int i = 0; i >= distance; i--)
            {
                if (getChar(r + i, c) == ' ')
                    setChar(r + i, c, ch);
            }
        }
        
    }
    else
        return false;
    return true;
}

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg) // plots the line using the functions plotHorizontalLine and plotVerticalLine
{
    if ((dir != HORIZ) && (dir != VERT)) // error checking
        return false;
    if ((fgbg != FG) && (fgbg != BG))
        return false;
    if ( ! (isprint(plotChar)))
        return false;
    if (((dir == HORIZ) && ( ! (plotHorizontalLine(r, c, distance, plotChar, fgbg)))) || ((dir == VERT) && ( ! (plotVerticalLine(r, c, distance, plotChar, fgbg))))) // tries to plot line
    {
        return false;
    }
    return true;
}


int performCommands(string commandString, char& plotChar, int& mode, int& badPos)
{
    string distanceSpecified = ""; // used to give us the integer version of distanceSpecifiedInt
    int distanceSpecifiedInt = 0;
    int directionSpecified = HORIZ; // checks vertical line vs horizontal line desired
    int specificDirectionSpecified = LEFT; // used to make distance negative later on if the line should go left
    int currentXPosition = 1; // used to keep track of cursor's x position
    int currentYPosition = 1; // used to keep track of cursor's y position
    bool needToPlotLine = false;
    int futureIncrement = 0; // used for case when a 0 is used as a placeholder
    for(int i = 0; i != commandString.size(); i++)
    {
        futureIncrement = 0;
        needToPlotLine = false;
        distanceSpecified = "";
        distanceSpecifiedInt = 0;
        if (commandString[i] == 'h' || commandString[i] == 'H') // creates a distanceSpecifiedInt for h function call that gives us the parameter to pass to the plotLine function
        {
            directionSpecified = HORIZ;
            needToPlotLine = true;
            if ((commandString.size() >= i + 1) && (isdigit(commandString[i+1]))) // checks for a number at the next two characters, adding it to the distance that the line will go
            {
                if (commandString[i+1] == '0')
                    futureIncrement += 1;
                distanceSpecified += commandString[i+1];
                specificDirectionSpecified = RIGHT;
                if ((isdigit(commandString[i+2])) && commandString.size() >= i + 2)
                {
                    distanceSpecified += commandString[i+2];
                }
            }
            else if ((commandString.size() >= i + 1) && (commandString[i+1] == '-')) // checks for up to two negative numbers, adding it to the distance that the line will go
            {
                specificDirectionSpecified = LEFT; // tells us to make it a negative distance later
                if ((commandString.size() >= i + 2) && (isdigit(commandString[i+2])))
                {
                    if (commandString[i+2] == '0')
                        futureIncrement += 1;
                    distanceSpecified += commandString[i+2];
                    if ((commandString.size() >= i + 3) && (isdigit(commandString[i+3])))
                    {
                        distanceSpecified += commandString[i+3];
                    }
                }
                else
                {
                    badPos = i + 2;
                    return 1;
                }
            }
            else
            {
                badPos = i + 1;
                return 1;
            }
            distanceSpecifiedInt = stoi(distanceSpecified); // turns distance from a string to an int
            if (specificDirectionSpecified == LEFT) // makes distance negative if the line is to be plotted left
                distanceSpecifiedInt *= -1;
        }
        else if (commandString[i] == 'v' || commandString[i] == 'V') // manages vertical line commands
        {
            directionSpecified = VERT;
            needToPlotLine = true;
            if ((commandString.size() >= i + 1) && (isdigit(commandString[i+1]))) // looks for up to two numbers to add to the distance the plotted line will go
            {
                if (commandString[i+1] == '0')
                    futureIncrement += 1;
                distanceSpecified += commandString[i+1];
                specificDirectionSpecified = UP;
                if ((isdigit(commandString[i+2])) && commandString.size() >= i + 2)
                {
                    distanceSpecified += commandString[i+2];
                }
            }
            else if ((commandString.size() >= i + 1) && (commandString[i+1] == '-')) // looks for up to two negative numbers to add the distance the plotted line will go
            {
                specificDirectionSpecified = DOWN;
                if ((commandString.size() >= i + 2) && (isdigit(commandString[i+2])))
                {
                    if (commandString[i+2] == '0')
                        futureIncrement += 1;
                    distanceSpecified += commandString[i+2];
                    if ((commandString.size() >= i + 3) && (isdigit(commandString[i+3])))
                    {
                        distanceSpecified += commandString[i+3];
                    }
                }
                else
                {
                    badPos = i + 1;
                    return 1;
                }
            }
            else
            {
                badPos = i + 1;
                return 1;
            }
            distanceSpecifiedInt = stoi(distanceSpecified);
            if (specificDirectionSpecified == DOWN) // makes distance negative if the line goes down
                distanceSpecifiedInt *= -1;
        }
        else if (commandString[i] == 'f' || commandString[i] == 'F') // manages changing to foreground mode with a character
        {
            if ((commandString.size() >= i + 1) && (isprint(commandString[i+1])))
            {
                mode = FG;
                plotChar = commandString[i + 1];
                i++;
            }
            else
            {
                badPos = i + 1;
                return 1;
            }
        }
        else if (commandString[i] == 'b' || commandString[i] == 'B') // manages changing to background mode with a character
        {
            if ((commandString.size() >= i + 1) && (isprint(commandString[i+1])))
            {
                mode = BG;
                plotChar = commandString[i + 1];
                i++;
            }
            else
            {
                badPos = i + 1;
                return 1;
            }
        }
        else if (commandString[i] == 'c' || commandString[i] == 'C') // manages the clear function call
        {
            clearGrid();
            mode = FG;
            currentXPosition = 1;
            currentYPosition = 1;
            plotChar = '*';
        }
        else
        {
            badPos = i;
            return 1;
        }
        
        if (needToPlotLine) // checks and plots the line if it needs to plot a line
        {
            if ( ! (plotLine(currentXPosition, currentYPosition, distanceSpecifiedInt, directionSpecified, plotChar, mode)))
            {
                badPos = i;
                return 2;
            }
            
            if (directionSpecified == HORIZ) // changes x coordinate for next iteration
                currentYPosition += distanceSpecifiedInt;
            else if (directionSpecified == VERT)// changes y coordinate for next iteration
                currentXPosition += distanceSpecifiedInt;
            
            if (distanceSpecifiedInt >= 10) // increases i value to the index of the beginning of the next command
                i += 2;
            else if ((distanceSpecifiedInt < 10) && (distanceSpecifiedInt > 0))
                i++;
            else if ((distanceSpecifiedInt < 0) && (distanceSpecifiedInt > -10))
                i += 2;
            else if (distanceSpecifiedInt <= -10)
                i += 3;
        }
        if (futureIncrement == 1) // increments i if a 0 was used as a placeholder, ex: 03 instead of 3
            i++;
    }
    return 0;
}
