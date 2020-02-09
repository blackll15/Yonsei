def countLetters(line):
    """ Count all letter characters in string ``line'' and write theresult to file ``answer.txt''.
    The number of letter characters must be written to the file:countLetters('abA1 23') -> writes 3
    countLetters('!') -> writes 0
    """
    myfile = open('answer.txt', 'w')
    count = 0
    for char in line:
        if((97 <= ord(char) and ord(char) <= 122) or (65 <= ord(char) and ord(char) <= 90)):
            count += 1
    myfile.write(str(count) + '\n')
    myfile.close()
