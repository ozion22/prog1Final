#!/usr/local/bin/python3
import sys
import os
import subprocess

def parse(inputstring):
    operationList = []
    counter = 0
    returnstring = ""
    tokens = inputstring.split()
    for token in tokens:
        if token == "+" or token == "-" or token == "/" or token == "^" or token == "*":
            try:
                tokens[counter-1] = "(" + tokens[counter-1] + ")"
            except:
                print("Invalid input! + before operation")
        for char in token:
            if char == "^":
                token = token.replace("^", "**")
        counter+=1
        operationList.append(token)
    for operation in operationList:
        returnstring += str(operation)
    return returnstring

def evaluateAtGivenX(inputstring,currentX):
    operation = str(parse(inputstring))
    for char in operation:
        if char == "x":
            operation = operation.replace("x", "(" + str(currentX) + ")")
        if char == "X":
            operation = operation.replace("X","(" + str(currentX) + ")")
    try:
        return eval(operation)
    except:
        print("You entered an invalid input, please try again\n")


#print(str(evaluateAtGivenX("x^2 + 2",2)))

def EvaluateGivenRange(bottom, top, screenWidth, inputString):
    parsedString = parse(inputString)
    step = (int(top)-int(bottom))/int(screenWidth)
    returnList = []

    iterator = bottom
    for _ in range(int(screenWidth)+1):
        returnList.append(evaluateAtGivenX(inputstring=parsedString, currentX=iterator))
        iterator+=step
    f = open("pythonOutput.txt", "w" or "x")
    for calculatedX in list(returnList):
        f.write(str(calculatedX) + ";")
    f.close


def main():
    bottom = float(input("Enter the lower bound: "))
    top = float(input("Enter the upper bound: "))
    (screenWidth,screenHeight) = os.get_terminal_size()
    print(f"Please enter a mathematical function in the format x^2 + 2*x + 3\nSquare roots are not supported, use x^0.5\nRemember multiplication signs between the variable x and a constant.\n")
    inputString = str(input(">"))
    EvaluateGivenRange(bottom=bottom, top=top, screenWidth=screenWidth, inputString=inputString)
    stringToRun = "/home/timsaa/finalProject/finishedProject/plot " + str(top) + ", " + str(bottom)
    os.system("clear")
    os.system(stringToRun)
    quit(0)
main()
